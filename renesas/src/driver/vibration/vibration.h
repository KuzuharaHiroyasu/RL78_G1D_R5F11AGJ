/********************************************************************************/
/* �V�X�e����   : RD8001														*/
/* �t�@�C����   : vibration.h														*/
/* �@�\         : 		*/
/* �ύX����     : 		���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/

#ifndef	__VIBRATION_INC__
#define	__VIBRATION_INC__

#include	"sys.h"


#define VIB_CTL						P0_bit.no1
#define VIB_ENA						P0_bit.no0

#define	VIB_ONE_SET					3					// 1�Z�b�g3��
#define VIB_THREE_ORBIT_ONE_SET		(VIB_ONE_SET * 1)	// �o�C�u���� 3 �~ 1
#define VIB_THREE_ORBIT_THREE_SET	(VIB_ONE_SET * 3)	// �o�C�u���� 3 �~ 3

#define VIB_SET_MODE_WEAK					0
#define VIB_SET_MODE_DURING					1
#define VIB_SET_MODE_STRENGTH				2
#define VIB_SET_MODE_GRADUALLY_STRONGER		3

typedef enum{
	VIB_MODE_ON,						// ON(�P������)
	VIB_MODE_OFF,						// OFF(�P������)
	VIB_MODE_WEAK,						// ��
	VIB_MODE_DURING,					// ��
	VIB_MODE_DURING_THREE,				// �� �~ 3
	VIB_MODE_STRENGTH,					// ��
	VIB_MODE_STRENGTH_THREE,			// �� �~ 3
	VIB_MODE_GRADUALLY_STRONGER_THREE,	// ���X�ɋ����~ 3
	VIB_MODE_STANDBY,					// �ҋ@���[�h�ڍs��
	VIB_MODE_SENSING,					// �Z���V���O���[�h�ڍs��
	VIB_MODE_INTERVAL,					// �o�C�u�̃Z�b�g�ԃC���^�[�o��
	VIB_MODE_INITIAL
}VIB_MODE;

typedef enum{
	VIB_LEVEL_1 = 0,
	VIB_LEVEL_2,
	VIB_LEVEL_3,
	VIB_LEVEL_4,
	VIB_LEVEL_MAX
}VIB_LEVEL;

/******************/
/*  �O���Q�Ɛ錾  */
/******************/
extern void vib_start(UH vib_timer);
extern void set_vib(VIB_MODE mode);
extern VIB_MODE set_vib_mode(UB yokusei_str);
extern void set_vib_level(B level);

#endif // __VIBRATION_INC__
