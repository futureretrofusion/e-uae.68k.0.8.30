 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Library of functions to make emulated filesystem as independent as
  * possible of the host filesystem's capabilities.
  *
  * Copyright 1999 Bernd Schmidt
  * Copyright 2025 Future Retro Fusion
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "uae.h"
#include "memory.h"
#include "custom.h"
#include "newcpu.h"
#include "filesys.h"
#include "autoconf.h"
#include "fsusage.h"
#include "scsidev.h"
#include "fsdb.h"

/* The on-disk format is as follows:
 * Offset 0, 1 byte, valid
 * Offset 1, 4 bytes, mode
 * Offset 5, 257 bytes, aname
 * Offset 263, 257 bytes, nname
 * Offset 519, 81 bytes, comment
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Index entry: points into tmpbuf; lifetime tied to tmpbuf */
typedef struct { const char* aname; int off; } fsdb_idx_t;

static int fsdb_cmp_idx_aname(const void *pa, const void *pb)
{
    const fsdb_idx_t *a = (const fsdb_idx_t *)pa;
    const fsdb_idx_t *b = (const fsdb_idx_t *)pb;
    return strcmp(a->aname, b->aname);
}

struct fsdb_key_t { const char* s; };

static int fsdb_cmp_key_to_idx(const void *pkey, const void *pelem)
{
    const struct fsdb_key_t *k = (const struct fsdb_key_t *)pkey;
    const fsdb_idx_t *e       = (const fsdb_idx_t *)pelem;
    return strcmp(k->s, e->aname);
}

#define TRACING_ENABLED 0
#if TRACING_ENABLED
#define TRACE(x)	do { write_log x; } while(0)
#else
#define TRACE(x)
#endif

char *nname_begin (char *nname)
{
    char *p = strrchr (nname, FSDB_DIR_SEPARATOR);
    if (p)
	return p + 1;
    return nname;
}

#if 1 //ndef _WIN32
/* Find the name REL in directory DIRNAME.  If we find a file that
 * has exactly the same name, return REL.  If we find a file that
 * has the same name when compared case-insensitively, return a
 * malloced string that contains the name we found.  If no file
 * exists that compares equal to REL, return 0.  */
char *fsdb_search_dir (const char *dirname, char *rel)
{
    char *p = 0;
    struct dirent *de;
    DIR *dir = opendir (dirname);

    /* This really shouldn't happen...  */
    if (! dir)
	return 0;

    while (p == 0 && (de = readdir (dir)) != 0) {
	if (strcmp (de->d_name, rel) == 0)
	    p = rel;
	else if (strcasecmp (de->d_name, rel) == 0)
	    p = my_strdup (de->d_name);
    }
    closedir (dir);
    return p;
}
#endif

static FILE *get_fsdb (a_inode *dir, const char *mode)
{
    char *n;
    FILE *f;

    n = build_nname (dir->nname, FSDB_FILE);
    f = fopen (n, mode);
    free (n);
    return f;
}

static void kill_fsdb (a_inode *dir)
{
    char *n = build_nname (dir->nname, FSDB_FILE);
    unlink (n);
    free (n);
}

static void fsdb_fixup (FILE *f, char *buf, int size, a_inode *base)
{
    char *nname;
    int ret;

    if (buf[0] == 0)
	return;
    nname = build_nname (base->nname, buf + 5 + 257);
    ret = fsdb_exists (nname);
    if (ret) {
	free (nname);
	return;
    }
    TRACE (("uaefsdb '%s' deleted\n", nname));
    /* someone deleted this file/dir outside of emulation.. */
    buf[0] = 0;
    free (nname);
}

/* Prune the db file the first time this directory is opened in a session.  */
void fsdb_clean_dir (a_inode *dir)
{
    char buf[1 + 4 + 257 + 257 + 81];
    char *n = build_nname (dir->nname, FSDB_FILE);
    FILE *f = fopen (n, "r+b");
    off_t pos1 = 0, pos2;

    if (f == 0) {
	free (n);
	return;
    }
    for (;;) {
	pos2 = ftell (f);
	if (fread (buf, 1, sizeof buf, f) < sizeof buf)
	    break;
	fsdb_fixup (f, buf, sizeof buf, dir);
	if (buf[0] == 0)
	    continue;
	if (pos1 != pos2) {
	    fseek (f, pos1, SEEK_SET);
	    fwrite (buf, 1, sizeof buf, f);
	    fseek (f, pos2 + sizeof buf, SEEK_SET);
	}
	pos1 += sizeof buf;
    }
    fclose (f);
    truncate (n, pos1);
    free (n);
}

static a_inode *aino_from_buf (a_inode *base, char *buf, long off)
{
    uae_u32 mode;
    a_inode *aino = (a_inode *) xcalloc (sizeof (a_inode), 1);

    mode = do_get_mem_long ((uae_u32 *)(buf + 1));
    buf += 5;
    aino->aname = my_strdup (buf);
    buf += 257;
    aino->nname = build_nname (base->nname, buf);
    buf += 257;
    aino->comment = *buf != '\0' ? my_strdup (buf) : 0;
    fsdb_fill_file_attrs (base, aino);
    aino->amigaos_mode = mode;
    aino->has_dbentry = 1;
    aino->dirty = 0;
    aino->db_offset = off;
    return aino;
}

a_inode *fsdb_lookup_aino_aname (a_inode *base, const char *aname)
{
    FILE *f = get_fsdb (base, "r+b");

    if (f == 0)
	return 0;

    for (;;) {
	char buf[1 + 4 + 257 + 257 + 81];
	if (fread (buf, 1, sizeof buf, f) < sizeof buf)
	    break;
	if (buf[0] != 0 && same_aname (buf + 5, aname)) {
	    long pos = ftell (f) - sizeof buf;
	    fclose (f);
	    return aino_from_buf (base, buf, pos);
	}
    }
    fclose (f);
    return 0;
}

a_inode *fsdb_lookup_aino_nname (a_inode *base, const char *nname)
{
    FILE *f = get_fsdb (base, "r+b");

    if (f == 0)
	return 0;

    for (;;) {
	char buf[1 + 4 + 257 + 257 + 81];
	if (fread (buf, 1, sizeof buf, f) < sizeof buf)
	    break;
	if (buf[0] != 0 && strcmp (buf + 5 + 257, nname) == 0) {
	    long pos = ftell (f) - sizeof buf;
	    fclose (f);
	    return aino_from_buf (base, buf, pos);
	}
    }
    fclose (f);
    return 0;
}

int fsdb_used_as_nname (a_inode *base, const char *nname)
{
    FILE *f = get_fsdb (base, "r+b");
    char buf[1 + 4 + 257 + 257 + 81];

    if (f == 0)
	return 0;
    for (;;) {
	if (fread (buf, 1, sizeof buf, f) < sizeof buf)
	    break;
	if (buf[0] == 0)
	    continue;
	if (strcmp (buf + 5 + 257, nname) == 0) {
	    fclose (f);
	    return 1;
	}
    }
    fclose (f);
    return 0;
}

static int needs_dbentry (a_inode *aino)
{
    const char *nn_begin;

    if (aino->deleted)
	return 0;

    if (! fsdb_mode_representable_p (aino) || aino->comment != 0)
	return 1;

    nn_begin = nname_begin (aino->nname);
    return strcmp (nn_begin, aino->aname) != 0;
}

static void write_aino (FILE *f, a_inode *aino)
{
    char buf[1 + 4 + 257 + 257 + 81];
    buf[0] = aino->needs_dbentry;
    do_put_mem_long ((uae_u32 *)(buf + 1), aino->amigaos_mode);
    strncpy (buf + 5, aino->aname, 256);
    buf[5 + 256] = '\0';
    strncpy (buf + 5 + 257, nname_begin (aino->nname), 256);
    buf[5 + 257 + 256] = '\0';
    strncpy (buf + 5 + 2*257, aino->comment ? aino->comment : "", 80);
    buf[5 + 2 * 257 + 80] = '\0';
    aino->db_offset = ftell (f);
    fwrite (buf, 1, sizeof buf, f);
    aino->has_dbentry = aino->needs_dbentry;
    TRACE (("%d '%s' '%s' written\n", aino->db_offset, aino->aname, aino->nname));
}

/* Write back the db file for a directory.  */

void fsdb_dir_writeback (a_inode *dir)
{
    FILE *f;
    int changes_needed = 0;
    int entries_needed = 0;
    a_inode *aino;
    char *tmpbuf;
    int size, i;

    TRACE (("fsdb writeback %s\n", dir->aname));
    /* First pass: clear dirty bits where unnecessary, and see if any work
     * needs to be done.  */
    for (aino = dir->child; aino; aino = aino->sibling) {
/*
	int old_needs_dbentry = aino->needs_dbentry || aino->has_dbentry;
	aino->needs_dbentry = needs_dbentry (aino);
	entries_needed |= aino->has_dbentry | aino->needs_dbentry;
*/
	int old_needs_dbentry = aino->has_dbentry;
	int need = needs_dbentry (aino);
	aino->needs_dbentry = need;
	entries_needed |= need;
	if (! aino->dirty)
	    continue;
	if (! aino->needs_dbentry && ! old_needs_dbentry)
	    aino->dirty = 0;
	else
	    changes_needed = 1;
    }
    if (! entries_needed) {
	kill_fsdb (dir);
	TRACE (("fsdb removed\n"));
	return;
    }

    if (! changes_needed) {
	TRACE (("not modified\n"));
	return;
    }

    f = get_fsdb (dir, "r+b");
    if (f == 0) {
        if (/*(currprefs.filesys_custom_uaefsdb && (dir->volflags & MYVOLUMEINFO_STREAMS)) ||*/ currprefs.filesys_no_uaefsdb) {
            for (aino = dir->child; aino; aino = aino->sibling) {
                aino->dirty = 0;
                aino->has_dbentry = 0;
                aino->needs_dbentry = 0;
            }
            return;
        }
        f = get_fsdb (dir, "w+b");
        if (f == 0) {
            TRACE (("failed\n"));
            return;
        }
    }

   /* Larger stdio buffer to cut syscall chatter. */
setvbuf(f, NULL, _IOFBF, 16*1024);

/* Snapshot old file to memory once. */
fseek(f, 0, SEEK_END);
size = (int)ftell(f);
fseek(f, 0, SEEK_SET);
tmpbuf = (size > 0) ? (char*)malloc(size) : NULL;
if (size > 0 && tmpbuf)
    fread(tmpbuf, 1, size, f);
TRACE(("**** updating '%s' %d\n", dir->aname, size));

/* Build an index of {aname->offset} from tmpbuf so we avoid O(N^2). */
{
    int recsz = 1 + 4 + 257 + 257 + 81;   /* record size in bytes */
    int nrecs = (size > 0) ? (size / recsz) : 0;
    fsdb_idx_t* idx = NULL;
    int iidx = 0;

    if (nrecs > 0 && tmpbuf) {
        int off;
        idx = (fsdb_idx_t*)malloc(nrecs * sizeof(fsdb_idx_t));
        if (idx) {
            for (off = 0; off + recsz <= size; off += recsz) {
                char *rec = tmpbuf + off;
                if (rec[0] == 0)
                    continue;               /* empty slot */
                idx[iidx].aname = rec + 5;  /* aname at +5 */
                idx[iidx].off   = off;
                iidx++;
            }
            if (iidx > 1)
                qsort(idx, iidx, sizeof(fsdb_idx_t), fsdb_cmp_idx_aname);
        }
    }

    /* Write back dirty children using the index (bsearch) or append. */
    aino = dir->child;
    while (aino) {
        if (aino->dirty) {
            long target_off = -1;
            aino->dirty = 0;

            if (iidx > 0 && idx) {
                struct fsdb_key_t key;
                fsdb_idx_t *hit;
                key.s = aino->aname;
                hit = (fsdb_idx_t*)bsearch(&key, idx, iidx,
                                           sizeof(fsdb_idx_t),
                                           fsdb_cmp_key_to_idx);
                if (hit)
                    target_off = hit->off;
            }

            if (target_off < 0) {
                fseek(f, 0, SEEK_END);
                aino->has_dbentry = 1;
            } else {
                fseek(f, target_off, SEEK_SET);
                aino->has_dbentry = 1;
                aino->db_offset = (long)target_off;
            }
            write_aino(f, aino); /* existing helper */
        }
        aino = aino->sibling;
    }

    TRACE(("end\n"));
    fclose(f);
    if (tmpbuf) free(tmpbuf);
    if (idx)    free(idx);
}

    fseek (f, 0, SEEK_END);
    size = ftell (f);
    fseek (f, 0, SEEK_SET);
    tmpbuf = 0;
    if (size > 0) {
	tmpbuf = malloc (size);
	fread (tmpbuf, 1, size, f);
    }
    TRACE (("**** updating '%s' %d\n", dir->aname, size));

    for (aino = dir->child; aino; aino = aino->sibling) {
	if (! aino->dirty)
	    continue;
	aino->dirty = 0;

	i = 0;
	while (!aino->has_dbentry && i < size) {
	    if (!strcmp (tmpbuf + i + 5, aino->aname)) {
		aino->has_dbentry = 1;
		aino->db_offset = i;
	    }
	    i += 1 + 4 + 257 + 257 + 81;
	}

	if (! aino->has_dbentry) {
	    fseek (f, 0, SEEK_END);
	    aino->has_dbentry = 1;
	} else {
	    fseek (f, aino->db_offset, SEEK_SET);
	}
	write_aino (f, aino);
    }
    TRACE (("end\n"));
    fclose (f);
    free (tmpbuf);
}
