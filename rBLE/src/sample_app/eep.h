/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : eep.h															*/
/* �@�\         : EEP(�}�N����`�A�^��`�A�֐��̊O���Q�Ɛ錾)					*/
/* �ύX����     : 2018.01.25 Axia Soft Design ���� ��	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
#ifndef		_EEP_H_			/* ��d��`�h�~ */
#define		_EEP_H_

/************************************************************/
/* �}�N��													*/
/************************************************************/
// �f�o�C�X�֘A
#define EEP_DEVICE_ADR			0xA0				// �f�o�C�X�A�h���X
#define EEP_RECORD_SIZE			8					// 1���R�[�h�T�C�Y ��256�̖񐔂Ƃ���y�[�W�ׂ����s�������Ȃ���
#define EEP_ADRS_SIZE			2					// �A�h���X�T�C�Y

#define EEP_DATA_SIZE_ALL		131072				// EEP�S�T�C�Y
#define EEP_DATA_SIZE_HALF		( 131072 / 2 )		// EEP�����T�C�Y

#define EEP_ACCESS_ONCE_SIZE	256					// �A�N�Z�X�T�C�Y�P��

#define EEP_PAGE_CNT_MAX		( EEP_DATA_SIZE_ALL / EEP_ACCESS_ONCE_SIZE )		//�y�[�W���ő�


// �f�[�^��
#define EEP_CALC_DATA_NUM			1440	// 12���ԕ�(1�f�[�^/30�b)
#define EEP_FRAME_MAX				7		// 1�T�ԕ�

// EEP�f�[�^�T�C�Y
#define EEP_CALC_DATA_SIZE						8		// 
#define EEP_RESERVE_SIZE						5760	// �ۑ��f�[�^1�ɑ΂���4�o�C�g�󂫂�݂���i4�o�C�g �~ 1440�� = 5760�o�C�g)
#define EEP_FRAM_ADD_SIZE						128		// �t���[���p�t�����(����,���т����m���`�ō����ċz,���Z��)
#define EEP_FRAME_SIZE							(UW)(( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + EEP_FRAM_ADD_SIZE )
#define EEP_SETTING_SIZE						3
#define EEP_ALARM_SIZE							5
#define EEP_DATE_SIZE							7
#define EEP_IBIKI_DETECT_CNT_SIZE				2
#define EEP_MUKOKYU_DETECT_CNT_SIZE				2
#define EEP_IBIKI_TIME_SIZE						2
#define EEP_MUKOKYU_TIME_SIZE					2
#define EEP_MAX_MUKOKYU_TIME_SIZE				2
#define EEP_DEVICE_SET_INFO_SIZE				1
#define EEP_DEVICE_SET_SUPPRESS_START_TIME_SIZE	1


// �擪�A�h���X
#define EEP_ADRS_TOP_FRAME										0													// �t���[���̐擪
#define EEP_ADRS_TOP_FRAME_DATE									(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 0 )
#define EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT						(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 8 )
#define EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT					(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 10 )
#define EEP_ADRS_TOP_FRAME_IBIKI_TIME							(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 12 )
#define EEP_ADRS_TOP_FRAME_MUKOKYU_TIME							(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 14 )
#define EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME						(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 16 )
#define EEP_ADRS_TOP_FRAME_DEVICE_SET_INFO						(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 18 )
#define EEP_ADRS_TOP_FRAME_DEVICE_SET_SUPPRESS_START_TIME		(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 19 )
#define EEP_ADRS_TOP_FRAME_CALC_CNT								(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 20 )

#define EEP_ADRS_TOP_SETTING			(UW)( EEP_FRAME_SIZE * EEP_FRAME_MAX )				// �ݒ�
#define EEP_ADRS_TOP_ALARM				(UW)( EEP_ADRS_TOP_SETTING + EEP_SETTING_SIZE )		// �x���@�\
#define EEP_ADRS_DATA_TYPE				(UW)( EEP_DATA_SIZE_ALL - 1 )						// EEP���(�ŏI�A�h���X) ���ʏ펞(0x00),�v���O�����]��(0xAA)



// ���x��
#define EEP_DATA_TYPE_NORMAL				0x11		// �ʏ펞(0x11)
#define EEP_DATA_TYPE_PRG_H1D				0xAA		// H1D�v���O�����]��(0xAA)


#define EEP_I2C_LOCK_ERR_VAL			10000		// I2C�ُ픻���(���b�N)


/************************************************************/
/* �^��`													*/
/************************************************************/


/************************************************************/
/* �O���Q�Ɛ錾												*/
/************************************************************/
extern void eep_init( void );

extern void eep_write( UW wr_adrs, UB* wr_data, UH len, UB wait_flg );
extern void eep_read( UW rd_adrs, UB* rd_data, UH len );
extern void eep_setting_area_erase( void );

#endif

/************************************************************/
/* END OF TEXT												*/
/************************************************************/

