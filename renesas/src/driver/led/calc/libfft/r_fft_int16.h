/*
     FFT Library

    Copyright (C) 2006, 2015 RENESAS ELECTRONICS CORPORATION AND
                   RENESAS SYSTEM DESIGN CO., LTD. ALL RIGHT RESERVED
*/

/* $Id$ */
/* $Date$ */

#if !defined(_R_FFT_INT16_H_)

#define _R_FFT_INT16_H_

#if defined(MACOS)
#include <stdint.h>
typedef int32_t natural_int_t;
#else
#include "r_stdint.h"
#endif


#if defined(MACOS)
#define far
#endif

typedef int16_t R_FFT_OUTDATA;
typedef int16_t R_FFT_DATA;
typedef int16_t R_FFT_INDATA;
typedef int16_t R_FFT_WINDATA;

#define RFFT256P (256)
#define RFFT128P (128)
#define RFFT64P  (64)

void R_rfft256_int16(R_FFT_INDATA *, R_FFT_OUTDATA *, R_FFT_WINDATA *, R_FFT_DATA *);
void R_rfft128_int16(R_FFT_INDATA *, R_FFT_OUTDATA *, R_FFT_WINDATA *, R_FFT_DATA *);
void R_rfft64_int16(R_FFT_INDATA *, R_FFT_OUTDATA *, R_FFT_WINDATA *, R_FFT_DATA *);

#endif

/*
    Copyright (C) 2006, 2015 RENESAS ELECTRONICS CORPORATION AND
                   RENESAS SYSTEM DESIGN CO., LTD. ALL RIGHT RESERVED
*/
