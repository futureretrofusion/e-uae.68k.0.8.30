/* Future Retro Fusion */
/*
 * Minimal Linux framebuffer backend for E-UAE.
 *
 * Goal:
 *  - no SDL
 *  - no X11
 *  - direct /dev/fb0 output
 *  - simple shadow framebuffer -> fb copy
 */

#include "sysconfig.h"
#include "sysdeps.h"

#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "options.h"
#include "custom.h"
#include "drawing.h"
#include "xwin.h"
#include "keyboard.h"
#include "inputdevice.h"
#include "uae.h"

static int fb_fd = -1;
static uae_u8 *fb_mem;
static size_t fb_size;
static struct fb_var_screeninfo fb_var;
static struct fb_fix_screeninfo fb_fix;

static uae_u8 *shadow_buf;

static int current_width;
static int current_height;
static int current_depth;
static int current_pixbytes;
static int center_x;
static int center_y;

static int bitsInMask (unsigned long mask)
{
    int n = 0;
    while (mask) {
        n += mask & 1;
        mask >>= 1;
    }
    return n;
}

static int maskShift (unsigned long mask)
{
    int n = 0;
    while (mask && !(mask & 1)) {
        n++;
        mask >>= 1;
    }
    return n;
}

static int init_colors_fb (void)
{
    alloc_colors64k (
        bitsInMask (fb_var.red.length ? ((1 << fb_var.red.length) - 1) : 0x1f),
        bitsInMask (fb_var.green.length ? ((1 << fb_var.green.length) - 1) : 0x3f),
        bitsInMask (fb_var.blue.length ? ((1 << fb_var.blue.length) - 1) : 0x1f),
        fb_var.red.offset,
        fb_var.green.offset,
        fb_var.blue.offset,
        0, 0, 0, 0);

    return 1;
}

static int fbdev_lock (struct vidbuf_description *gfxinfo)
{
    return 1;
}

static void fbdev_unlock (struct vidbuf_description *gfxinfo)
{
}

static void fbdev_flush_line (struct vidbuf_description *gfxinfo, int line_no)
{
}

static void fbdev_flush_block (struct vidbuf_description *gfxinfo, int first_line, int last_line)
{
}

static void fbdev_flush_screen (struct vidbuf_description *gfxinfo, int first_line, int last_line)
{
    int y;

    if (!fb_mem || !shadow_buf)
        return;

    for (y = first_line; y <= last_line; y++) {
        uae_u8 *src = shadow_buf + (y * gfxvidinfo.rowbytes);
        uae_u8 *dst = fb_mem
            + ((y + center_y) * fb_fix.line_length)
            + (center_x * current_pixbytes);

        memcpy (dst, src, current_width * current_pixbytes);
    }
}

static void fbdev_flush_clear_screen (struct vidbuf_description *gfxinfo)
{
    if (fb_mem)
        memset (fb_mem, 0, fb_size);
}

int graphics_setup (void)
{
    fb_fd = open ("/dev/fb0", O_RDWR);
    if (fb_fd < 0) {
        write_log ("FBDEV: unable to open /dev/fb0\n");
        return 0;
    }

    if (ioctl (fb_fd, FBIOGET_FSCREENINFO, &fb_fix) < 0)
        return 0;

    if (ioctl (fb_fd, FBIOGET_VSCREENINFO, &fb_var) < 0)
        return 0;

    fb_size = fb_fix.smem_len;

    fb_mem = mmap (0, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (fb_mem == MAP_FAILED) {
        fb_mem = 0;
        return 0;
    }

    current_depth = fb_var.bits_per_pixel;
    current_pixbytes = (current_depth + 7) / 8;

    write_log ("FBDEV: %dx%d %dbpp\n",
        fb_var.xres,
        fb_var.yres,
        current_depth);

    return 1;
}

int graphics_init (void)
{
    fixup_prefs_dimensions (&currprefs);

    current_width = currprefs.gfx_width_win;
    current_height = currprefs.gfx_height_win;

    center_x = (fb_var.xres - current_width) / 2;
    center_y = (fb_var.yres - current_height) / 2;

    if (center_x < 0)
        center_x = 0;

    if (center_y < 0)
        center_y = 0;

    shadow_buf = calloc (1, current_width * current_height * current_pixbytes);
    if (!shadow_buf)
        return 0;

    gfxvidinfo.width = current_width;
    gfxvidinfo.height = current_height;
    gfxvidinfo.pixbytes = current_pixbytes;
    gfxvidinfo.rowbytes = current_width * current_pixbytes;
    gfxvidinfo.bufmem = shadow_buf;
    gfxvidinfo.emergmem = 0;
    gfxvidinfo.maxblocklines = MAXBLOCKLINES_MAX;
    gfxvidinfo.linemem = 0;

    gfxvidinfo.lockscr = fbdev_lock;
    gfxvidinfo.unlockscr = fbdev_unlock;
    gfxvidinfo.flush_line = fbdev_flush_line;
    gfxvidinfo.flush_block = fbdev_flush_block;
    gfxvidinfo.flush_screen = fbdev_flush_screen;
    gfxvidinfo.flush_clear_screen = fbdev_flush_clear_screen;

    reset_drawing ();
    init_row_map ();

    return init_colors_fb ();
}

void graphics_leave (void)
{
    if (shadow_buf) {
        free (shadow_buf);
        shadow_buf = 0;
    }

    if (fb_mem) {
        munmap (fb_mem, fb_size);
        fb_mem = 0;
    }

    if (fb_fd >= 0) {
        close (fb_fd);
        fb_fd = -1;
    }
}

void graphics_notify_state (int state)
{
}

void handle_events (void)
{
}
