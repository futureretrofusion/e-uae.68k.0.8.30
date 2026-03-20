/* Future Retro Fusion */
#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "custom_private.h"
#include "blitter.h"
#include "blitfunc.h"

void blitdofast_0 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (dodst) chipmem_wput (dstp, dstd);
		dstd = (0) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&1) ptd += b->bltdmod;
}
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_0 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = (0) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&1) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((~srca & srcc)) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((~srca & srcc)) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_2a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc & ~(srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_2a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc & ~(srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_30 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srca & ~srcb)) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_30 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srca & ~srcb)) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_3a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcb ^ (srca | (srcb ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_3a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcb ^ (srca | (srcb ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_3c (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srca ^ srcb)) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_3c (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srca ^ srcb)) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_4a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & (srcb | srcc)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_4a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & (srcb | srcc)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_6a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_6a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_8a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc & (~srca | srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_8a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc & (~srca | srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_8c (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcb & (~srca | srcc))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_8c (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcb & (~srca | srcc))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_9a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & ~srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_9a (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & ~srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_a8 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc & (srca | srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_a8 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc & (srca | srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_aa (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = (srcc) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_aa (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = (srcc) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_b1 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = (~(srca ^ (srcc | (srca ^ srcb)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_b1 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = (~(srca ^ (srcc | (srca ^ srcb)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_ca (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & (srcb ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_ca (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srca & (srcb ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_cc (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = (srcb) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&4) ptb += b->bltbmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_cc (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = (srcb) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&4) ptb -= b->bltbmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_d8 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srca ^ (srcc & (srca ^ srcb)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_d8 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srca ^ (srcc & (srca ^ srcb)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_e2 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srcb & (srca ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_e2 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc ^ (srcb & (srca ^ srcc)))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_ea (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc | (srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_ea (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srcc | (srca & srcb))) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_f0 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = (srca) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_f0 (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = (srca) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_fa (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&2) { srcc = chipmem_wget (ptc); ptc += 2; }
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srca | srcc)) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&2) ptc += b->bltcmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_fa (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&2) { srcc = chipmem_wget (ptc); ptc -= 2; }
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srca | srcc)) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&2) ptc -= b->bltcmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_fc (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
int i,j,dodst=0;
uae_u32 totald = 0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;

		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb += 2;
			srcb = (((uae_u32)prevb << 16) | bltbdat) >> b->blitbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta += 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)preva << 16) | bltadat) >> b->blitashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srca | srcb)) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd += 2; }
	}
	if (chen&8) pta += b->bltamod;
	if (chen&4) ptb += b->bltbmod;
	if (chen&1) ptd += b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
void blitdofast_desc_fc (uae_u8 chen, uaecptr pta, uaecptr ptb, uaecptr ptc, uaecptr ptd, struct bltinfo *b)
{
uae_u32 totald = 0;
int i,j,dodst=0;
uae_u32 preva = 0;
uae_u32 prevb = 0, srcb = b->bltbhold;
uae_u32 srcc = b->bltcdat;
uae_u32 dstd=0;
uaecptr dstp = 0;
for (j = b->vblitsize; j--; ) {
	for (i = 0; i < b->hblitsize; i++) {
		uae_u32 bltadat, srca;
		if (chen&4) {
			uae_u32 bltbdat = blt_info.bltbdat = chipmem_wget (ptb); ptb -= 2;
			srcb = ((bltbdat << 16) | prevb) >> b->blitdownbshift;
			prevb = bltbdat;
		}
		if (chen&8) { bltadat = blt_info.bltadat = chipmem_wget (pta); pta -= 2; } else { bltadat = blt_info.bltadat; }
		bltadat &= blit_masktable[i];
		srca = (((uae_u32)bltadat << 16) | preva) >> b->blitdownashift;
		preva = bltadat;
		if (dodst) chipmem_wput (dstp, dstd);
		dstd = ((srca | srcb)) & 0xFFFF;
		totald |= dstd;
		if (chen&1) { dodst = 1; dstp = ptd; ptd -= 2; }
	}
	if (chen&8) pta -= b->bltamod;
	if (chen&4) ptb -= b->bltbmod;
	if (chen&1) ptd -= b->bltdmod;
}
b->bltbhold = srcb;
b->bltcdat = srcc;
		if (dodst) chipmem_wput (dstp, dstd);
if (totald != 0) b->blitzero = 0;
}
