/*==============================================================================*/
/* include																		*/
/*==============================================================================*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include "pulse_param.h"
/*==============================================================================*/
/*	cal_sp1																		*/
/*==============================================================================*/
void cal_sp1
(
	double	mx1,		/* IN：赤外最大値										*/
	double	mx2,		/* IN：赤色最大値										*/
	int		*sp			/* OT：SPO2												*/
)
{
	const double	eo1 = _PULSE_PARAM_EO1;
	const double	eo2 = _PULSE_PARAM_EO2; 
	const double	ed1 = _PULSE_PARAM_ED1;
	const double	ed2 = _PULSE_PARAM_ED2;
	double	f1   = log(10)*mx1*eo1 - eo2;
	double	f2   = ed2 - log(10)*mx2*ed1;
	double tmp = 1.0 + f1/f2;
	double tmp2 = 1.0 / tmp;

	*sp = (int)(tmp2 * 100.0 + 0.5);
}
/*=============================================================================*/
/* EOF */
