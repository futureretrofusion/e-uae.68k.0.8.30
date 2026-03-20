/* Future Retro Fusion */
#ifndef UAE_GUI_H
#define UAE_GUI_H

/*
 * UAE - The Un*x Amiga Emulator
 *
 * Interface to the Tcl/Tk GUI
 *
 * Copyright 1996 Bernd Schmidt
 */

#include "sysconfig.h"
#include "sysdeps.h"
#include "uae.h"

/* --- GUI API ------------------------------------------------------------ */

extern void gui_init (int argc, char **argv);
extern int  gui_open (void);
extern int  gui_update (void);
extern void gui_exit (void);
extern void gui_led (int, int);
extern void gui_handle_events (void);
extern void gui_filename (int, const char *);
extern void gui_fps (int fps, int idle);
extern void gui_hd_led (int);
extern void gui_cd_led (int);
extern unsigned int gui_ledstate;
extern void gui_display (int shortcut);
extern void gui_notify_state (int state);

/* Frameskip HUD */
extern void gui_frameskip_set  (int value);   /* update displayed number */
/* polarity: +1 = plus (flash RED), -1 = minus (flash GREEN), 0 = clear */
extern void gui_frameskip_flash(int polarity);

/* --- GUI state ---------------------------------------------------------- */

struct gui_info
{
    uae_u8  drive_motor[4];       /* motor on off */
    uae_u8  drive_track[4];       /* rw-head track */
    uae_u8  drive_writing[4];     /* drive is writing */
    uae_u8  drive_disabled[4];    /* drive is disabled */
    uae_u8  powerled;             /* state of power led */
    uae_u8  drive_side;           /* floppy side */
    uae_u8  drive_selected;       /* mask of selected floppies */
    uae_u8  hd;                   /* harddrive */
    uae_u8  cd;                   /* CD (kept for compat, not shown) */
    int     fps, idle;
    char    df[4][256];           /* inserted image */
    uae_u32 crc32[4];             /* crc32 of image */

    /* Frameskip HUD state */
    int     frameskip;                /* current frameskip value shown in HUD */
    uae_u8  frameskip_flash_polarity; /* 0 none, 1 minus(GREEN), 2 plus(RED) */
    uae_u8  frameskip_flash_ticks;    /* pulse timer like HD LED */
};

/* Current compact status row:
   0: IDLE, 1: FPS, 2: PWR, 3: HD, 4: DF0, 5: DF1, 6: FRAMESKIP */
#define NUM_LEDS 7

extern struct gui_info gui_data;

#endif /* UAE_GUI_H */
