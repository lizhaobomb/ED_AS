#ifndef __COMPLEX_H__
#define __COMPLEX_H__
#include <math.h>
double _caccr=0., _cacci=0.;    /* pracovni promenne pro makra    */

#define cabs2(c)        ((c).real * (c).real + (c).image * (c).image) /* vraci |c|*|c|  */
#define cabs(c)	        sqrt ((double)  cabs2 (c))      /* vraci |c|      */
#define cadd(c,b,a)     ((c).real = (a).real + (b).real, (c).image = (a).image + (b).image)
							/* c = a + b      */
#define csub(c,a,b)     ((c).real = (a).real - (b).real, (c).image = (a).image - (b).image)
							/* c = a - b      */
#define cconj(b,a)	((b).real = (a).real, (b).image = - (a).image)
							/* b = a*         */
#define cmult(c,b,a)	(_caccr = (a).real * (b).real - (a).image * (b).image,     \
			 (c).image = (a).real * (b).image + (a).image * (b).real,      \
			 (c).real = _caccr)                /* c = a * b      */
#define cnull(a)	((a).real = (a).image = 0)             /* c = 0          */
#define ccopy(b,a)	((b).real = (a).real, (b).image = (a).image)  /* b = a          */
#define cdiv(c,a,b)	(_caccr = (_cacci = 1 / cabs2 (b)) *              \
			((a).real * (b).real + (a).image * (b).image),                  \
			(c).image = _cacci * ((a).image * (b).real - (a).real * (b).image), \
			(c).real = _caccr)                /*  c = a / b     */
#define cneg(b,a)       ((b).real = -(a).real, (b).image = -(a).image)/*  b = -a        */

#define cisReal(z)      ((z).image==0.0)
#define cdist2(z1,z2)   (((z1).real-(z2).real)*((z1).real-(z2).real) + ((z1).image-(z2).image)*((z1).image-(z2).image))
#define cdist(z1,z2)    sqrt(cdist2(z1,z2))

#define cEQ(c1,c2)    ((c1).real==(c2).real && (c1).image==(c2).image)
#define cNE(c1,c2)    !cEQ(c1,c2)



#endif