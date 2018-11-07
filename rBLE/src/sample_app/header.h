/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : header.h														*/
/* 機能         : ヘッダインクルード用											*/
/* 変更履歴		: 2018.01.25 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/

#ifndef _HEADER_H_
#define _HEADER_H_

/* 標準 */
//#include <stdio.h>
//#include <limits.h>
//#include <stdlib.h>
//#include <ctype.h>
#include <string.h>
//#include <stdarg.h>
#include <math.h>

/* システム共通定義 */
#include "sys.h"

/* ドライバ */
#include "compiler.h"
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_iica.h"
#include "r_cg_port.h"
#include "r_cg_intp.h"
#include "r_cg_it.h"

/* ミドル */
#include "eep.h"
#include "cpu_com.h"
#include "calc_data.h"

/* アプリケーション */
#include "user_main.h"

#if FUNC_DEBUG_CALC_NON == OFF
#include "calc/pulseoximeter/calc_pulseoximeter.h"
#include "calc/apnea/calc_apnea.h"
#include "calc/apnea/calc_snore.h"
#endif

#endif /*_HEADER_H */
