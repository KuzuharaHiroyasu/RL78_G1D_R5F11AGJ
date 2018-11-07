/*==============================================================================*/
/* include																		*/
/*==============================================================================*/
#include	<stdio.h>
#include	<stdlib.h>
#include "sys.h"
/*-------------------------------------------------------------------------------*/
static	void	MOD( double PM_M, double PM_0, double PM_P, int m, double DELTA, double *FI, double *PP );
/*==============================================================================*/
/*	peak_modify	極大値補正														*/
/*==============================================================================*/
int peak_modify																	/* ☆ */
(
	double	in_data[],	/* IN：入力信号の格納されたバッファ						*/
	H		in_res[],	/* IN：判定結果（0=NO、1=YES)							*/
	double	ot_data[],	/* OT：出力信号パワースペクトル							*/
	double	ot_hz[],	/* OT：出力信号周波数									*/
	int		size,		/* IN：入力信号・出力信号のバッファのサイズ				*/
	double	delta		/* IN：FFTサンプリング周波数							*/
)
{
	int pos = 0;
	int i;
	/*--------------------------------------------------------------------------*/
	for( i = 0 ; i < size ; i++ )
	{
		ot_data[i] = 0.0;
		ot_hz  [i] = 0.0;
	}

	/*--------------------------------------------------------------------------*/
	for( i = 1; i < (size - 1) ; i++ )
	{
		if( in_res[ i ] == 1 )
		{
			double PM_M = in_data[ i - 1 ];
			double PM_0 = in_data[ i ];
			double PM_P = in_data[ i + 1 ];
			double FI   = 0.0;
			double PP   = 0.0;

			MOD( PM_M, PM_0, PM_P, i, delta, &FI, &PP );

			ot_data[pos] = PP;
			ot_hz  [pos] = FI;
			pos+=1;
		}
	}
	
	return pos;
}
/*==============================================================================*/
/*	modify		極大値補正														*/
/*==============================================================================*/
/*==============================================================================*
 [入力]PM_M  : ローカルピーク周波数のひとつ前の周波数（パワースペクトル）
 [入力]PM_0  : ローカルピーク周波数（パワースペクトル）
 [入力]PM_P  : ローカルピーク周波数のひとつ後の周波数（パワースペクトル）
 [入力]DELTA : サンプリング周波数（１単位）＝定数
 [入力]m     : ローカルピーク周波数（パワースペクトル）の位置＝データの配列番号に等しい
 [出力]FI    : 算出ピーク値の周波数
 [出力]PP    : 算出ピーク値のパワースペクトル
 *==============================================================================*/
static	void	MOD																	/* ☆ */
(
	double PM_M,
	double PM_0,
	double PM_P,
	int    m,
	double DELTA,
	double *FI,
	double *PP
)
{
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;

	*FI = 0.0;
	*PP = 0.0;

	a   = ( ( PM_P + PM_M ) / 2.0 ) - PM_0;			/* (3.41)参照				*/
	b   = ( ( PM_P - PM_M ) / 2.0 );				/* (3.41)参照				*/
	c   = PM_0;										/* (3.41)参照				*/

	*FI  = ( (-b)/( 2.0 * a ) + m ) * DELTA;		/* (3.44)参照				*/
	*PP  = c - ( ( b * b ) / ( 4.0 * a ) );			/* (3.45)参照				*/
}
/* EOF */
