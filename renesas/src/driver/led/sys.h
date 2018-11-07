/********************************************************************************/
/* システム名   : RD8001														*/
/* ファイル名   : sys.h															*/
/* 機能         : システム共通(マクロ定義、型定義、関数の外部参照宣言)			*/
/* 変更履歴     : 2017.12.20 Axia Soft Design 西島		初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/
#ifndef		_SYS_H_			/* 二重定義防止 */
#define		_SYS_H_

/************************************************************/
/* マクロ													*/
/************************************************************/
/* 汎用定義 */

#ifndef OFF
#define		OFF					(0)							/* オフ					*/
#define		ON					(1)							/* オン					*/
#define		UNKNOWN				(2)							/* 状態不明 ※初期状態	*/
#endif

#define		OK					(0)							/* 正常					*/
#define		NG					(1)							/* 異常					*/

#ifndef TRUE
#define		TRUE				(1)							/* 真					*/
#define		FALSE				(0)							/* 偽					*/
#endif

#define		VALID				(0)							/* 有効					*/
#define		INVALID				(-1)						/* 無効					*/

#define		LOW					(0)							/* ポート生データLOW	*/
#define		HIGH				(1)							/* ポート生データHIGH	*/

#define		ACT_L				(0)							/* ポート論理(LOW)	*/
#define		ACT_H				(1)							/* ポート論理(HIGH)	*/

#define		SET					(1)							/* セット	*/
//#define		CLR					(0)							/* クリア	*/


/* ビット定義 */
#define		BIT00				(0x00000001)				/* bit0					*/
#define		BIT01				(0x00000002)				/* bit1					*/
#define		BIT02				(0x00000004)				/* bit2					*/
#define		BIT03				(0x00000008)				/* bit3					*/
#define		BIT04				(0x00000010)				/* bit4					*/
#define		BIT05				(0x00000020)				/* bit5					*/
#define		BIT06				(0x00000040)				/* bit6					*/
#define		BIT07				(0x00000080)				/* bit7					*/
#define		BIT08				(0x00000100)				/* bit8					*/
#define		BIT09				(0x00000200)				/* bit9					*/
#define		BIT10				(0x00000400)				/* bit10				*/
#define		BIT11				(0x00000800)				/* bit11				*/
#define		BIT12				(0x00001000)				/* bit12				*/
#define		BIT13				(0x00002000)				/* bit13				*/
#define		BIT14				(0x00004000)				/* bit14				*/
#define		BIT15				(0x00008000)				/* bit15				*/
#define		BIT16				(0x00010000)				/* bit16				*/
#define		BIT17				(0x00020000)				/* bit17				*/
#define		BIT18				(0x00040000)				/* bit18				*/
#define		BIT19				(0x00080000)				/* bit19				*/
#define		BIT20				(0x00100000)				/* bit20				*/
#define		BIT21				(0x00200000)				/* bit21				*/
#define		BIT22				(0x00400000)				/* bit22				*/
#define		BIT23				(0x00800000)				/* bit23				*/
#define		BIT24				(0x01000000)				/* bit24				*/
#define		BIT25				(0x02000000)				/* bit25				*/
#define		BIT26				(0x04000000)				/* bit26				*/
#define		BIT27				(0x08000000)				/* bit27				*/
#define		BIT28				(0x10000000)				/* bit28				*/
#define		BIT29				(0x20000000)				/* bit29				*/
#define		BIT30				(0x40000000)				/* bit30				*/
#define		BIT31				(0x80000000)				/* bit31				*/

/* 汎用エラー定義 */
#define		E_OK		((INT)0)			/* 正常終了 */
#define		E_GENE		((INT)1)			/* 汎用エラー */
#define		E_OBJ		((INT)2)			/* オブジェクト状態エラー */
#define		E_PAR		((INT)3)			/* パラメータエラー */
#define		E_QOVR		((INT)4)			/* キューイングオーバーフロー */
/* nopの定義 */
/* 12.000MHz駆動の1クロック83.333‥ns */
#define	Nop1()		NOP();															/* 83.333ns */
#define	Nop5()		NOP();NOP();NOP();NOP();NOP();									/* 416.666ns */
#define	Nop10()		Nop5();Nop5();													/* 833.333ns */
#define	Nop20()		Nop10();Nop10();												/* 1.666μs */
#define	Nop40()		Nop20();Nop20();												/* 3.333μs */
#define	Nop120()	Nop40();Nop40();Nop40();										/* 10μs */

#define WAIT_1US()		Nop10();Nop1();Nop1();										/* 約1us待ち */
#define WAIT_2US()		WAIT_1US();WAIT_1US();										/* 約2us待ち */
#define WAIT_5US()		WAIT_1US();WAIT_1US();WAIT_1US();WAIT_1US();WAIT_1US();		/* 約1us待ち */

#define WAIT_10US()		WAIT_5US();WAIT_5US();										/* 約10us待ち */

#define WAIT_EEP_STOP_AFTER			WAIT_10US();


/* サム値関連 */
#define	CALC_SUM_INIT		((UB)0x00)			/* サム値計算の初期値 */
#define	CALC_SUM_INIT_UH	((UH)0x0000)		/* サム値計算の初期値 */
#define	CALC_SUM_INIT_UW	((UW)0x00000000)	/* サム値計算の初期値 */

/* CRC関連 */
#define CRCD_DEFAULT_LIB	((UH)0xFFFF)		/* CRCデータレジスタ 初期値 */

/* エンディアン変換 */
#define	CHANGE_UB2_UW		0			/* UBx2 → UWx1 */
#define	CHANGE_UW_UB2		1			/* UWx1 → UBx2 */

/* =====汎用マクロ関数===== */
/* maxまでdataをインクリメント */
#define	INC_MAX(data,max)				\
{										\
	if( data < max ){					\
		data++;							\
	}									\
}

/* maxまでdataをインクリメントして上限超えたら初期化 */
#define	INC_MAX_INI(data,max,ini)		\
{										\
	if( data < max ){					\
		data++;							\
	}else{								\
		data = ini;						\
	}									\
}

/* minまでdataをデクリメント */
#define	DEC_MIN(data,min)				\
{										\
	if( data > min ){					\
		data--;							\
	}									\
}

/* minまでdataをデクリメントしてminになったら初期化 */
#define	DEC_MIN_INI(data,min,ini)		\
{										\
	if( data > min ){					\
		data--;							\
	}else{								\
		data = ini;						\
	}									\
}

/* type別のuhの上位桁もしくは下位桁を返す */
#define	UH_U		0		/* 上位アドレス取得 */
#define	UH_D		1		/* 下位アドレス取得 */

/* 単体デバッグ用定義 */
#define STATIC							static				/* 単体デバッグ実施時はSTATICを無のデファインにしても良い */

/* =====機能有効無効(ON/OFF)===== */
#define	FUNC_RELEASE					OFF					/* リリース */
															/* 正式リリースが始まった後は基本的にONで運用→リリース時の間違いをなくす */
															/* OFFは、デバッグ時にローカルで修正 */

/* リリースオプション */
#if FUNC_RELEASE == ON
	//随時追加すること
#else
															/* ※デバッガ接続時は自己診断ポートがLOWになるため無効化する */
#endif

/*----------------*/
/* 機能の有効無効 */
/*----------------*/
#define		FUNC_DEBUG_LOG							ON					/* PCへのログ通信機能 *//* リリース時はOFFする事 */
#define		FUNC_DEBUG_CPU_COM						OFF					/* CPU間通信をログ通信でデバッグする機能 *//* リリース時はOFFする事 */
																		/* 使用する時はログ通信もOFFする事 */
#define		FUNC_DEBUG_EEP_NON						ON					/* G1DにEEPなしハードでデバッグする機能 *//* リリース時はOFFする事 */




/************************************************************/
/* 型定義													*/
/************************************************************/
/* 変数型の再定義 */
typedef signed char		B;		/* 符号付き8ビット整数 */
typedef unsigned char	UB;		/* 符号無し8ビット整数 */
typedef signed short	H;		/* 符号付き16ビット整数 */
typedef unsigned short 	UH;		/* 符号無し16ビット整数 */
typedef signed long		W;		/* 符号付き32ビット整数 */
typedef unsigned long	UW;		/* 符号無し32ビット整数 */
typedef signed int		INT;	/* プロセッサに自然なサイズの符号付き整数 */
typedef unsigned int	UINT;	/* プロセッサに自然なサイズの符号無し整数 */
//typedef int				BOOL;	/* 真偽値(TRUE or FALSE) */
typedef float 			FLOAT;	/* 単精度浮動小数点 */
typedef double			DOUBLE;	/* 倍単精度浮動小数点 */
typedef int				ER;		/* 汎用エラー(μiTRON表記) */

/* 日時 */
typedef struct{
	INT	tm_sec;		/* 秒 */
	INT	tm_min;		/* 分 */
	INT	tm_hour;	/* 時 */
	INT	tm_mday;	/* 日 */
	INT	tm_mon;		/* 月 */
	INT	tm_year;	/* 年 */
	INT	tm_wday;	/* 曜日 */
	INT	tm_yday;	/* 日(1年の何日目) */
	INT	tm_isdst;	/* サマータイムフラグ */
}DATE;


/* リングバッファ(1バイト用) */
typedef struct{
	UB*	buf;					/* リングバッファ用ポインタ */
	UH	wr_pos;					/* 書き込み位置 */
	UH	rd_pos;					/* 読み出し位置 */
	UH	size;					/* サイズ */
}RING_BUF;

/* リングバッファ(2バイト用) */
typedef struct{
	UH*	buf;					/* リングバッファ用ポインタ */
	UH	wr_pos;					/* 書き込み位置 */
	UH	rd_pos;					/* 読み出し位置 */
	UH	size;					/* サイズ */
}RING_BUF_UH;

/* リモコン用 */
typedef struct{
	UW	time;						/* 時間 */
	UB	key_code;					/* キーコード */
	UB	kind;						/* リモコン種別(セキュリティ/通常) */
	UB	staff_code;					/* スタッフコード */
}REMO;

/* リングバッファ(時間付きバイトデータ) */
typedef struct{
	REMO*	buf;			/* リングバッファ用ポインタ */
	UH	wr_pos;					/* 書き込み位置 */
	UH	rd_pos;					/* 読み出し位置 */
	UH	size;					/* サイズ */
}RING_BUF_REMO;

/* 入出力情報用 */
typedef struct{
	UB	sync;					/* 同期用データ */
	UB	port1;					/* 入出力情報1 */
	UB	port2;					/* 入出力情報2 */
	UB	port3;					/* 入出力情報3 */
}IO_INFO;

/* リングバッファ(3バイト(24bit)用) */
typedef struct{
	IO_INFO*	buf;			/* リングバッファ用ポインタ */
	UH	wr_pos;					/* 書き込み位置 */
	UH	rd_pos;					/* 読み出し位置 */
	UH	size;					/* サイズ */
}RING_BUF_IO;


/* 座標位置 */
typedef struct{
	H	x;			/* x座標 ポジション情報またはAD変換値 */
	H	y;			/* y座標 ポジション情報またはAD変換値 */
}POS_INFO;


/* マスカブル割り込み禁止/許可 */
/* 割込みの禁止/許可は#pragma拡張機能でサポートされている */
//#ifndef __RL78__
//#define	DI()	asm("FCLR I")
//#define	EI()	asm("FSET I")
//#endif

/* マスカブル割り込み禁止/許可(前回状態への復帰) */
/* 注意事項：前回状態への復帰となるためかならず同一関数内でセットで使用して下さい */
#if 0
#define DI_RET()		\
{						\
	__asm("PUSH PSW");	\
	DI();				\
}
#define EI_RET()		\
{						\
	__asm("POP  PSW");	\
}
#else
// RD8001暫定 上記でエラーが出る。
#define DI_RET()		\
{						\
}
#define EI_RET()		\
{						\
}



#endif

/************************************************************/
/* 外部参照宣言												*/
/************************************************************/
void ring_buf_init( RING_BUF* p_ring, UB* p_data, UH size);
INT read_ring_buf( RING_BUF* p_ring, UB* p_data );
INT write_ring_buf( RING_BUF* p_ring ,UB data );
UB	hex2bin(UB num);
UB	uwhex2bin(UW *bin, UB *pstr);
UB bcdbin( UB bin );
INT bcd2bin( UB *bin, const UB *src_bcd );
void dummy( void );
UH crc16( UB* p_in, int len );


#endif
/************************************************************/
/* END OF TEXT												*/
/************************************************************/


