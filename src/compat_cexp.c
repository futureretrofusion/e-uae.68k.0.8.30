/* Future Retro Fusion */
#include <complex.h>
#include <math.h>

/* Provide cexp() when the C library doesn't. */
double complex cexp(double complex z) {
    double x = creal(z);
    double y = cimag(z);
    double ex = exp(x);
    /* e^(x+iy) = e^x (cos y + i sin y) */
    return ex * (cos(y) + I * sin(y));
}
