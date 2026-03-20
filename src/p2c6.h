/* Future Retro Fusion */
#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
void pfield_6p_16px_to8b_asm(const uint16_t *p0,
                             const uint16_t *p1,
                             const uint16_t *p2,
                             const uint16_t *p3,
                             const uint16_t *p4,
                             const uint16_t *p5,
                             uint8_t *dst);
#ifdef __cplusplus
}
#endif
