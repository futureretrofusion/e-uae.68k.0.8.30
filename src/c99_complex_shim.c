/* Future Retro Fusion */
#include <math.h>
typedef __complex__ double cdouble;
cdouble cexp(cdouble z) {
    double a = __real__ z, b = __imag__ z;
    double ea = exp(a);
    cdouble r;
    __real__ r = ea * cos(b);
    __imag__ r = ea * sin(b);
    return r;
}
