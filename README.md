Future Retro Fusion

# E-UAE FRF Fork

This repository is a cleaned public fork of **E-UAE 0.8.30.2026.64** with later work focused on the AmigaOS build, including scaling, mouse handling, GUI work, savestate-related work, and other local fixes.

## Repository layout

- Original upstream top-level project tree is preserved
- Modified `src/` tree from the current fork is included

E-UAE 0.8.30.2026.64

What is E-UAE?
E-UAE is an Amiga emulator that allows you to run software designed for Amiga computers on other platforms. It is based on UAE (the original Ubiquitous Amiga Emulator) and WinUAE (the Windows version of UAE). The Future Retro Fusion edition targets native AmigaOS 3.2 running on 68k hardware, with a full native Intuition/GadTools GUI and numerous display and usability improvements.
E-UAE is open-source software made available under the terms of the GPL. See the COPYING file included with the E-UAE archive for details.
Note: to make full use of E-UAE you will need an image of an Amiga Kickstart ROM. Kickstart ROMs are copyrighted and may not be freely distributed. ROM images are available for purchase from Cloanto at http://www.amigaforever.com/

What's New in 0.8.30 (Future Retro Fusion Edition)
This release represents a major overhaul of the AmigaOS display, GUI, filesystem and event subsystems. All changes target the 68k AmigaOS build.

Native Intuition GUI (ami-gui.c — complete rewrite)
The old MUI/MUIRexx-dependent GUI has been replaced with a fully self-contained native Intuition/GadTools GUI. It requires no external libraries beyond what is present in a standard AmigaOS 3.2 installation.
    • Opens on its own RTG screen (finds best-fit CyberGfx/P96 mode >= 640x480 @ 8-bit automatically; falls back to native Hires if no RTG is present).
    • The GUI screen is completely independent of the emulator display — it never overlaps or blocks it.
    • Hotkey: Ctrl+Left Alt+B opens and closes the GUI from anywhere. The emulator continues running while the GUI is open.
    • GUI does not auto-open at startup — the emulator window receives all keyboard events immediately on launch.
    • Five pages selectable via tab buttons: Drives, State, Emulator, Settings, and States.


GUI Pages
Drives Page
    • Insert or eject disk images into DF0–DF3 via ASL file requester.
    • Live drive type selection per drive (3.5" DD / 3.5" HD / 5.25" SD / None).
    • Live LED indicators for PWR and DF0–DF3 with current disk filename displayed.
    • Directory memory — the requester reopens in the last-used directory.

State Page
    • Load State and Save State via ASL file requester (.uss files).
    • Save/load uses a safe deferred mechanism (pending flags consumed by handle_events()) — never called directly from GUI event context, preventing crashes.
    • Saving a state automatically appends an entry to S:uae-states.csv with the filename, CPU type, and memory configuration.
    • Soft Reset and Hard Reset buttons.

Emulator Page
    • Sound mode cycle: Off / Low / Best — takes effect immediately.
    • Volume slider (0–100) with live drag update.
    • Frameskip cycle (0–8) — takes effect immediately.
    • LoRes toggle, Aspect Correction toggle, On-screen LEDs toggle.
    • All controls initialised from currprefs so current state is always reflected.

Settings Page
    • CPU type cycle (68000 / 68010 / 68020 / 68020+FPU) and CPU compatible mode.
    • Current Kickstart ROM path displayed; Select ROM button opens ASL requester.
    • Load Config and Save Config buttons (.uaerc files).
    • Screen Mode button: opens the ASL screenmode requester to pick a new display mode without restarting the emulator.

States Page
    • Automatically populated from S:uae-states.csv — one button per saved state.
    • Button labels show the state name plus the CPU type and memory configuration it was saved with (e.g. "mygame | 68000 Chip:2MB Fast:8MB"), so you can check compatibility before loading.
    • Clicking a button loads that state via the safe deferred mechanism.
    • If no CSV is present, a message directs you to save a state first.
    • A companion AmigaDOS script (MakeStateCSV) can scan a directory and build the CSV from any collection of existing .uss files.

Screenmode Requester (ami-win.c)
    • New Screen Mode button in the Settings page opens the standard ASL screenmode picker at runtime without restarting.
    • Implemented as a deferred pending flag (pending_screenmode) dispatched at the safe top of handle_events(), identical pattern to toggle_fullscreen and toggle_lores.
    • Suppresses the focus-loss pause guard during the screen transition so the emulator does not lock up while the requester is open.
    • The force_screenmode_req flag bypasses the get_displayid() cache that previously caused the requester to be silently skipped.

Scaling / Toggle LoRes (ami-win.c)
    • Ctrl+Left Alt+L now correctly cycles through all three display modes: 320x256 (LoRes) -> 640x512 (HiRes) -> 800x600 stretched -> 320x256.
    • Fixed: the cycle position was previously read from scale_x (which resets to 1 on each open_display call), causing the 800x600 stretched step to be skipped. Now tracked in a persistent last_display_w / last_display_h pair that survives open_display calls.
    • Fullscreen mode still uses a simple LoRes <-> HiRes two-step toggle (no 800x600 stretch in fullscreen).
    • Revert path (if the new mode fails to open) correctly restores last_display_w/h so the next toggle starts from the right position.

Savestate Load/Save (ami-gui.c + ami-win.c)
    • Fixed: savestate load from the GUI was previously non-functional. The old code set savestate_state = STATE_DORESTORE directly from the GUI event handler, which raced with the emulator CPU loop.
    • Both save and load now use gui_request_save(path) and gui_request_load(path) — functions defined in ami-win.c that set pending flags consumed safely at the top of handle_events() once per frame.
    • The same deferred mechanism is also serviced in ami_pause_loop() so saves and loads queued while paused are acted on correctly.

Event Scheduler Fix (events.c)
    • Removed the eventtab_dirty optimisation that was added in a previous session. This optimisation caused a system freeze before the first frame was rendered.
    • Root cause: the dirty-flag guard meant events_schedule() silently skipped recomputing nextevent unless events_mark_dirty() had been called first. Files that were never modified (blitter.c, disk.c, audio.c, etc.) called events_schedule() without mark_dirty(), so their event updates were lost and nextevent went stale. The main loop then spun indefinitely waiting for a timestamp that never arrived.
    • events_schedule() now always recomputes nextevent — the original behaviour. events_mark_dirty() is kept as a no-op for source compatibility with cia.c and custom.c.

Filesystem Protection Bit Removal (fsdb.c, fsdb_unix.c, filesys.c)
    • Files on the host filesystem are now always presented to the guest Amiga as fully accessible (RWED bits cleared).
    • Three-layer fix: fsdb_unix.c always sets amigaos_mode = 0 and never calls chmod(); fsdb.c strips RWED denial bits from stale .uaem database entries; filesys.c strips RWED bits at get_fileinfo() time, catching all cached ainos.
    • The Script bit (0x10) is intentionally not forced — it must be set by the user with Protect file +s, which is the expected AmigaOS behaviour.

ARexx Port
    • The UAE ARexx port is initialised at startup via rexx_init() and remains live for the lifetime of the emulator.
    • Available ARexx commands: QUIT/BYE, RESET, INSERT 0 path, EJECT 0, SOUND ON/OFF/BEST, FRAMERATE n, DISPLAY 0/1, QUERY LED_DF0/NAME_DF0/SOUND/FRAMERATE, FEEDBACK, UAEEXE, VERSION.
    • Example: RX "ADDRESS UAE; QUIT" or from a script: ADDRESS UAE then the command.

MakeStateCSV — AmigaDOS Script
    • Companion script that scans a directory for .uss files and generates S:uae-states.csv from them.
    • Usage: MakeStateCSV [DIR] [CPU] [MEM]
    • Example: MakeStateCSV DH0:States 68020 "Chip:2MB Fast:8MB"
    • Uses a two-pass LIST/LFORMAT/Execute approach to correctly expand shell variables into the CSV output (direct LFORMAT variable expansion does not work in AmigaDOS).
    • Install to C: and set the script bit: Protect C:MakeStateCSV +s

Hotkey Reference
    • Ctrl + Left Alt + B  —  Open / close the native GUI
    • Ctrl + Left Alt + Q  —  Quit UAE
    • Ctrl + Left Alt + R  —  Soft reset
    • Ctrl + Left Alt + Shift + R  —  Hard reset
    • Ctrl + Left Alt + L  —  Cycle display mode (320x256 / 640x512 / 800x600 stretched)
    • Ctrl + Left Alt + U  —  Toggle fullscreen
    • Ctrl + Left Alt + G  —  Toggle mouse grab
    • Ctrl + Left Alt + P  —  Freeze / pause emulation
    • Ctrl + Left Alt + H  —  Toggle HUD
    • Ctrl + Left Alt + F5  —  Begin 3-character quick save code entry
    • Ctrl + Left Alt + F6  —  Begin 3-character quick load code entry
    • Ctrl + Left Alt + F1–F4  —  Open disk requester for DF0–DF3
    • Numpad 7 / 8  —  Volume down / up
    • Numpad + / -  —  Frameskip up / down

S:uae-states.csv Format
The States page is driven by S:uae-states.csv. Each line has four comma-separated fields:
    • Field 1: full AmigaOS path to the .uss file (e.g. DH0:States/mygame.uss)
    • Field 2: filename (e.g. mygame.uss) — displayed with .uss stripped
    • Field 3: CPU type string (e.g. 68000, 68020)
    • Field 4: memory configuration string (e.g. Chip:2MB Fast:8MB Bogo:512K)
This file is appended to automatically each time you save a state via the GUI. Lines beginning with ; are treated as comments and ignored. You can also build or rebuild it from an existing collection of .uss files using the MakeStateCSV script.

Still To Do
    • Serial and parallel port emulation.
    • AHI soundcard emulation.
    • Catweasel support.
    • Graphics filters.
    • Floppy drive sounds.
    • PICASSO96 virtual RTG card (uaegfx.card) — the host-side stubs are present and -DPICASSO96 can be added to the build flags to enable the emulated RTG card in the guest Amiga.

Known Issues
    • JIT direct memory access only works on Linux/x86. On AmigaOS 68k the JIT is not used; the interpretive CPU emulator is used throughout.
    • Virtual filesystem does not support UTF-8 filenames. Characters outside core ASCII will be mangled.
    • Full documentation is still a work in progress.


Compiling E-UAE
The AmigaOS 68k build uses m68k-amigaos-gcc with the clib2 C runtime:
m68k-amigaos-gcc -mcrt=clib2 -m68060 -m68881 -Ofast -ffast-math
See docs/compiling.txt for full details. Key build flags:
    • -DFPUEMU — floating point unit emulation
    • -DAGA — AGA chipset support
    • -DAUTOCONFIG — Zorro autoconfig support
    • -DFILESYS — host filesystem access
    • -DSAVESTATE — save/restore emulator state
    • -DPICASSO96 — virtual RTG card (optional, requires uaegfx.card in guest)

Original Authors & Contact
    • UAE original author: Bernd Schmidt
    • WinUAE maintainer: Toni Wilen
    • E-UAE maintainer: Richard Drummond <uae@rcdrummond.net>
    • 0.8.30 FRF edition: Future Retro Fusion
    • E-UAE mailing list: http://www.freelists.org/list/uae
