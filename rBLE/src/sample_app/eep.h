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
#define EEP_RECORD_SIZE			8					// 1���R�[�h�T�C�Y ��256�̖񐔂ł���K�v������
#define EEP_ADRS_SIZE			2					// �A�h���X�T�C�Y

#define EEP_DATA_SIZE_ALL		131072				// EEP�S�T�C�Y
#define EEP_DATA_SIZE_HALF		( 131072 / 2 )		// EEP�����T�C�Y

#define EEP_ACCESS_ONCE_SIZE	256					// �A�N�Z�X�T�C�Y�P��


// �@�\�d�l
#define EEP_CACL_DATA_SIZE			8		//�_�~�[�܂�
#define EEP_CACL_DATA_NUM			1440

#define EEP_FRAM_OTHER_SIZE			16		// ���̑���(����,�ō����ċz,���Z��)

#define EEP_FRAME_SIZE				(UW)(( EEP_CACL_DATA_SIZE * 1440 ) + EEP_FRAM_OTHER_SIZE )

#define EEP_FRAME_MAX				10


// EEP�A�h���X�̐擪
#define EEP_ADRS_TOP_FRAME_CALC_CNT		(UW)( ( EEP_CACL_DATA_SIZE * 1440 ) + 10 )

#define EEP_ADRS_TOP_FRAME				0											//�t���[���̐擪
#define EEP_ADRS_TOP_SETTING			(UW)( EEP_FRAME_SIZE * EEP_FRAME_MAX )		//�ݒ�





// RD8001�b��F�f�o�b�O�R�[�h
#define I2C_RCV_SND_SIZE	20		//RD8001�b��F�T�C�Y�v����




/************************************************************/
/* �^��`													*/
/************************************************************/


/************************************************************/
/* �O���Q�Ɛ錾												*/
/************************************************************/
extern void eep_init( void );

extern void R_IICA0_Create(void);
extern void R_IICA0_StopCondition(void);
extern void err_info( int id );
extern void eep_write( UW wr_adrs, UB* wr_data, UH len, UB wait_flg );
extern void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
extern void eep_read( UW rd_adrs, UB* rd_data, UH len );
extern void_eep_all_erase( void );


#endif

/************************************************************/
/* END OF TEXT												*/
/************************************************************/

