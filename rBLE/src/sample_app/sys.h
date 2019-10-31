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
/*  8.000MHz駆動の1クロック 125ns */
/* 16.000MHz駆動の1クロック62.5ns */
#define	Nop1()		NOP();
#define	Nop5()		NOP();NOP();NOP();NOP();NOP();
#define	Nop10()		Nop5();Nop5();
#define	Nop20()		Nop10();Nop10();
#define	Nop40()		Nop20();Nop20();
#define	Nop120()	Nop40();Nop40();Nop40();

#if defined(CLK_FCLK_8MHZ)
#define WAIT_1US()		Nop5();Nop1();Nop1();Nop1();								/* 約1us待ち[8MHz] */
#else
#define WAIT_1US()		Nop10();Nop5();Nop1();										/* 約1us待ち[16MHz] */
#endif
//#define WAIT_1US()		Nop10();Nop5();Nop1();										/* 約1us待ち */

#define WAIT_2US()		WAIT_1US();WAIT_1US();											/* 約2us待ち */
#define WAIT_5US()		WAIT_1US();WAIT_1US();WAIT_1US();WAIT_1US();WAIT_1US();			/* 約1us待ち */

#define WAIT_10US()		WAIT_5US();WAIT_5US();											/* 約10us待ち */

#define WAIT_50US()		WAIT_10US();WAIT_10US();WAIT_10US();WAIT_10US();WAIT_10US();	/* 約50us待ち */

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
// RD8001暫定：デバッグ機能 ※最終リリース時はOFFする事
#define		FUNC_DEBUG_LOG							ON					/* PCへのログ通信機能 */
#define		FUNC_DEBUG_WAVEFORM_LOG					OFF					/* PCへのログ通信機能(波形確認&判定結果確認版) */
#define		FUNC_DEBUG_SLEEP_NON					OFF					/* G1DにSLEEPなしでデバッグする機能 */
#define		FUNC_DEBUG_CALC_NON						OFF					/* G1Dに演算なしでデバッグする機能 */

#define		FUNC_DEBUG_FIN_NON						OFF					/* GET時の完了通知不要版(ユニアーススマホで取得するときはこれをON) */

#define		FUNC_DEBUG_PRG_H1D_U					OFF					/* 評価ボード単独で擬似デバッグ(上位側) */
#define		FUNC_DEBUG_PRG_H1D_D					OFF					/* 評価ボード単独で擬似デバッグ(下位側) */

#define		FUNC_DEBUG_PORT							ON					/* デバッグポート機能 */

#define		FUNC_DEBUG_BREAK_POINT					ON					/* デバッグ時のブレイクポイント設置有効 */
#if FUNC_DEBUG_BREAK_POINT == ON
#define		NO_OPERATION_BREAK_POINT();				Nop5();				// ブレイクポイント設置用
#else
#define		NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
#endif


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
//typedef int				BOOL;	/* 真偽値(TRUE or FALSE) *///プラットフォームで定義されており重複の為にコメントアウト
typedef float 			FLOAT;	/* 単精度浮動小数点 */
typedef double			DOUBLE;	/* 倍単精度浮動小数点 */
typedef int				ER;		/* 汎用エラー(μiTRON表記) */

/* リングバッファ(1バイト用) */
typedef struct{
	UB*	buf;					/* リングバッファ用ポインタ */
	UH	wr_pos;					/* 書き込み位置 */
	UH	rd_pos;					/* 読み出し位置 */
	UH	size;					/* サイズ */
}RING_BUF;

/* マスカブル割り込み禁止/許可(前回状態への復帰) */
/* 注意事項：前回状態への復帰となるためかならず同一関数内でセットで使用して下さい */
/* 注意：iflgはリエントラントを考慮しローカル変数とし参照元で用意する */
#define		IFLG_BIT			BIT07			/* PSWのIフラグのビット */

/* 割り込み禁止(前回状態への復帰あり) */
/* 説明：Iフラグの状態をiflgに入れる */
#define	DI_RET( iflg )					\
{										\
	if( IFLG_BIT & __get_psw() ){		\
		iflg = ON;						\
	}else{								\
		iflg = OFF;						\
	}									\
	DI();								\
}

/* 割り込み許可(前回状態への復帰あり) */
/* 説明：DIRET()で取得したIフラグの状態をiflgに設定する */
#define	EI_RET( iflg )					\
{										\
	if( ON == iflg ){					\
		EI();							\
	}else{								\
		DI();							\
	}									\
}

/************************************************************/
/* 外部参照宣言												*/
/************************************************************/
UB calc_sum( UB *p_in, INT len );
void calc_sum_uw_cont( UW* sum, UB *p_in, INT len );
void ring_buf_init( RING_BUF* p_ring, UB* p_data, UH size);
INT read_ring_buf( RING_BUF* p_ring, UB* p_data );
INT write_ring_buf( RING_BUF* p_ring ,UB data );
void dummy( void );
UH crc16( UB* p_in, int len );
UB bin2bcd( UB bin );
INT bcd2bin( UB *bin, const UB *src_bcd );
void wait_ms( int ms );


#endif
/************************************************************/
/* END OF TEXT												*/
/************************************************************/


