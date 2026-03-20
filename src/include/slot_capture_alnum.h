/* Future Retro Fusion */
/* ===== UAE AmigaOS — Slot/Tag capture helpers (digits + letters) =====
   Drop this in once (e.g., near the top of ami-win.c after includes).
   It replaces any old slotcap_* globals and scancode_to_digit().
   Guards prevent multiple definitions across translation units.
=========================================================================== */
#ifndef UAE_AMI_SLOT_CAPTURE_ALNUM_BLOCK
#define UAE_AMI_SLOT_CAPTURE_ALNUM_BLOCK

/* Mode: 0=off, 1=save, 2=load */
static int  slotcap_mode   = 0;
/* length captured so far (0..3) */
static int  slotcap_len    = 0;
/* literal chars captured, always uppercase '0'..'9','A'..'Z' */
static char slotcap_buf[4] = {0,0,0,0};

/* Reset */
static inline void slotcap_reset(void) {
    slotcap_mode = 0;
    slotcap_len  = 0;
    slotcap_buf[0] = slotcap_buf[1] = slotcap_buf[2] = 0;
}

/* Map raw scancodes to alnum chars; returns '\0' if not alnum.
   Top row numbers: 0x01..0x09 => '1'..'9', 0x0A => '0'
   Numpad digits:   0x43..0x45, 0x40..0x42, 0x3D..0x3F, 0x46
   Letters: common rawkey set (Amiga) — adjust if your keymap differs.
*/
static inline char scancode_to_alnum(unsigned sc) {
    /* digits top row */
    switch (sc) {
        case 0x01: return '1'; case 0x02: return '2'; case 0x03: return '3'; case 0x04: return '4';
        case 0x05: return '5'; case 0x06: return '6'; case 0x07: return '7'; case 0x08: return '8';
        case 0x09: return '9'; case 0x0A: return '0';
    }
    /* digits numpad */
    switch (sc) {
        case 0x43: return '1'; case 0x44: return '2'; case 0x45: return '3';
        case 0x40: return '4'; case 0x41: return '5'; case 0x42: return '6';
        case 0x3D: return '7'; case 0x3E: return '8'; case 0x3F: return '9';
        case 0x46: return '0';
    }
    /* letters: a..z */
    switch (sc) {
        case 0x20: return 'Q'; case 0x21: return 'W'; case 0x22: return 'E'; case 0x23: return 'R'; case 0x24: return 'T';
        case 0x25: return 'Y'; case 0x26: return 'U'; case 0x27: return 'I'; case 0x28: return 'O'; case 0x29: return 'P';
        case 0x10: return 'A'; case 0x11: return 'S'; case 0x12: return 'D'; case 0x13: return 'F'; case 0x14: return 'G';
        case 0x15: return 'H'; case 0x16: return 'J'; case 0x17: return 'K'; case 0x18: return 'L';
        case 0x30: return 'Z'; case 0x31: return 'X'; case 0x32: return 'C'; case 0x33: return 'V'; case 0x34: return 'B';
        case 0x35: return 'N'; case 0x36: return 'M';
    }
    return '\0';
}

/* Push a char if it's 0-9 or A-Z; returns current length (0..3) */
static inline int slotcap_push_char(char ch) {
    if (!ch) return slotcap_len;
    /* normalize to uppercase */
    if (ch >= 'a' && ch <= 'z') ch = (char)(ch - 'a' + 'A');
    if (!((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z'))) return slotcap_len;
    if (slotcap_len < 3) slotcap_buf[slotcap_len++] = ch;
    return slotcap_len;
}

/* Value helpers ---------------------------------------------------------- */

/* Interpret buffer as decimal digits only; letters count as 0.
   Returns 0..999. Use with %03d for filenames like 000.uss */
static inline int slotcap_value_dec(void) {
    int a = (slotcap_len > 0 && slotcap_buf[0] >= '0' && slotcap_buf[0] <= '9') ? (slotcap_buf[0] - '0') : 0;
    int b = (slotcap_len > 1 && slotcap_buf[1] >= '0' && slotcap_buf[1] <= '9') ? (slotcap_buf[1] - '0') : 0;
    int c = (slotcap_len > 2 && slotcap_buf[2] >= '0' && slotcap_buf[2] <= '9') ? (slotcap_buf[2] - '0') : 0;
    return a*100 + b*10 + c;
}

/* Base-36 decode (0-9 = 0..9, A-Z = 10..35). Returns 0..46655 */
static inline int slotcap_value_base36(void) {
    int v = 0;
    for (int i=0;i<3;i++) {
        int d = 0;
        char ch = slotcap_buf[i];
        if (ch >= '0' && ch <= '9') d = ch - '0';
        else if (ch >= 'A' && ch <= 'Z') d = 10 + (ch - 'A');
        v = v * 36 + d;
    }
    return v;
}

/* Safe getters ----------------------------------------------------------- */
static inline const char* slotcap_str3(void) {
    static char out[4];
    out[0] = slotcap_len > 0 ? slotcap_buf[0] : '0';
    out[1] = slotcap_len > 1 ? slotcap_buf[1] : '0';
    out[2] = slotcap_len > 2 ? slotcap_buf[2] : '0';
    out[3] = '\0';
    return out;
}

#endif /* UAE_AMI_SLOT_CAPTURE_ALNUM_BLOCK */
