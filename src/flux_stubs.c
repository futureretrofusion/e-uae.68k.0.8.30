/* Future Retro Fusion */
/* Stubs so the build succeeds without libdisk/scp support */
void *libdisk_open(const char *path) { (void)path; return (void*)0; }
void  libdisk_close(void *h)         { (void)h; }
int   libdisk_loadrevolution(void *h,int t,unsigned char **b,int *l)
{ (void)h; (void)t; (void)b; (void)l; return -1; }
int   libdisk_loadtrack(void *h,int t,unsigned char **b,int *l)
{ (void)h; (void)t; (void)b; (void)l; return -1; }

void *scp_open(const char *path) { (void)path; return (void*)0; }
void  scp_close(void *h)         { (void)h; }
int   scp_loadrevolution(void *h,int t,unsigned char **b,int *l)
{ (void)h; (void)t; (void)b; (void)l; return -1; }
int   scp_loadtrack(void *h,int t,unsigned char **b,int *l)
{ (void)h; (void)t; (void)b; (void)l; return -1; }
