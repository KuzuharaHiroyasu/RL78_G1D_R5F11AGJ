/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : eep.c															*/
/* �@�\         : EEP�i�֐�,RAM,ROM��`�j										*/
/* �ύX����     : 2018.01.25 Axia Soft Design ���� ��	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/

/********************/
/*     include      */
/********************/
#include	"header.h"		//���[�U�[��`


/************************************************************/
/* �v���g�^�C�v�錾											*/
/************************************************************/
void eep_init( void );
STATIC void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
STATIC void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
void wait_ms( int ms );
#if 0	//�e�X�g�R�[�h
STATIC void test_code_eep(void);
#endif

/********************/
/*     �����ϐ�     */
/********************/
// I2C�h���C�o�̃t���O ��i2c�h���C�o�Őݒ肳���
STATIC UB i2c_snd_flg;			//���M�t���O(I2C)
STATIC UB i2c_rcv_flg;			//��M�t���O(I2C)
// RD8001�b��FEEP�ُ̈펞�̓��삪�d�l�㖢��`
STATIC UB i2c_err_flg;			//�G���[�t���O(I2C)


/************************************************************/
/* �q�n�l��`												*/
/************************************************************/

/************************************************************************/
/* �֐�     : eep_init													*/
/* �֐���   : EEP����������												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : EEP����������													*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void eep_init( void )
{
	//�f�[�^������
	i2c_snd_flg = OFF;
	i2c_rcv_flg = OFF;
	i2c_err_flg = OFF;
	
	// I2C������
	R_IICA0_Create();
	
	// �e�X�g�R�[�h
	//	test_code_eep();
	//	eep_all_erase();
}


/************************************************************************/
/* �֐�     : eep_write													*/
/* �֐���   : EEP��������												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : EEP��������													*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void eep_write( UW wr_adrs, UB* wr_data, UH len, UB wait_flg )
{
	UB device_adrs;
	UB wr_buf[EEP_ACCESS_ONCE_SIZE + EEP_ADRS_SIZE];
	
	// �T�C�Y�`�F�b�N
	if( len > EEP_ACCESS_ONCE_SIZE ){
		err_info(ERR_ID_EEP);
		return;
	}
	
	// �A�h���X��4��2byte�֕ύX
	if( wr_adrs < EEP_DATA_SIZE_HALF ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( wr_adrs < EEP_DATA_SIZE_ALL ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
	}else{
		err_info(ERR_ID_EEP);
		return;
	}
	wr_buf[0] = (UB)(( wr_adrs >> 8 ) & 0xff );		//�A�h���X���
	wr_buf[1] = (UB)( wr_adrs & 0x00ff );				//�A�h���X����
	
	
	memcpy( &wr_buf[2], wr_data, len);
	
	//��������
	i2c_write_sub( device_adrs, &wr_buf[0], ( len + EEP_ADRS_SIZE ), wait_flg );	
}

/************************************************************************/
/* �֐�     : i2c_write_sub												*/
/* �֐���   : I2C�������݃T�u											*/
/* ����     : device_adrs	�f�o�C�X�A�h���X							*/
/*          : wr_data		�������݃f�[�^								*/
/*          : len			�f�[�^��									*/
/*          : wait_flg		�������ݑ҂��t���O							*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : I2C�������݃T�u												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg )
{
	UW lcok_cnt = 0;

	if( 0 != R_IICA0_Master_Send( device_adrs, wr_data, len, I2C_WAIT)){
		err_info(ERR_ID_EEP);
		i2c_err_flg = ON;
	}else{
		while(1){
			if( OFF == i2c_snd_flg ){
				// ���M����
				break;
			}
			if( lcok_cnt++ >= EEP_I2C_LOCK_ERR_VAL ){
				// �ُ펞
				err_info(ERR_ID_EEP);
				i2c_err_flg = ON;
				break;
			}
		}
		if( OFF == i2c_err_flg ){
			R_IICA0_StopCondition();		//�@�ُ픭�����͗\�����ʏ������݂�h�~����ׂɃX�g�b�v�R���f�B�V�������s���Ȃ�
		}
	}
	
	while(0U == SPD0){}

	if( wait_flg == ON ){
		wait_ms(5);			/* Write Cycle Time */
	}
	
	if( ON == i2c_err_flg ){
		i2c_err_flg = OFF;
		R_IICA0_Create();		// I2C������
	}
}


/************************************************************************/
/* �֐�     : eep_read													*/
/* �֐���   : EEP�ǂݏo��												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : EEP�ǂݏo��													*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void eep_read( UW rd_adrs, UB* rd_data, UH len )
{
	UB device_adrs;
	UH read_adrs;
	
	// �A�h���X��4��2byte�֕ύX
	if( rd_adrs < EEP_DATA_SIZE_HALF ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( rd_adrs < EEP_DATA_SIZE_ALL ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
	}else{
		return;
	}
	
	read_adrs = (UH)( rd_adrs & 0xFFFF );
	
	i2c_read_sub( device_adrs, read_adrs, rd_data, len );
}

/************************************************************************/
/* �֐�     : i2c_read_sub												*/
/* �֐���   : I2C�ǂݏo���T�u											*/
/* ����     : device_adrs	�f�o�C�X�A�h���X							*/
/*          : read_adrs		�ǂݏo���A�h���X							*/
/*          : read_data		�ǂݏo���f�[�^								*/
/*          : len			�f�[�^��									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : I2C�ǂݏo���T�u												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len )
{
	UB adrs[2];
	UB adrs_size = 0;
	UW lcok_cnt = 0;

	adrs[0] = (UB)(( read_adrs >> 8 ) & 0xff );		//�A�h���X���
	adrs[1] = (UB)( read_adrs & 0x00ff );			//�A�h���X����
	adrs_size  = 2;

	if( 0 != R_IICA0_Master_Send( device_adrs, &adrs[0], adrs_size, I2C_WAIT )){
		err_info(ERR_ID_EEP);
	}else{
		while(1){
			if( OFF == i2c_snd_flg ){
				// ���M����
				break;
			}
			if( lcok_cnt++ >= EEP_I2C_LOCK_ERR_VAL ){
				// �ُ펞
				err_info(ERR_ID_EEP);
				i2c_err_flg = ON;
				break;
			}
		}

// RD8001�b��F�ǂ̗l(�o�X�J��or�X�g�b�v�R���f�B�V����)�ɑΉ����邩�⍇����
#if 0
	//�X�g�b�v�R���f�B�V��������
		R_IICA0_StopCondition();
	}
	/* �X�g�b�v�R���f�B�V������̑҂� */
	while(0U == SPD0){}
//	WAIT_5US();
//	WAIT_5US();
//	wait_ms(5);
#else
	}
	//�o�X�J��
	R_IICA0_Stop();
	R_IICA0_Create();
	
#endif

	if( 0 != R_IICA0_Master_Receive(device_adrs, read_data, len, I2C_WAIT)){
		err_info(ERR_ID_EEP);
	}else{
		while(1){
			if( OFF == i2c_rcv_flg ){
				// ��M����
				break;
			}
			if( lcok_cnt++ >= EEP_I2C_LOCK_ERR_VAL ){
				// �ُ펞
				err_info(ERR_ID_EEP);
				i2c_err_flg = ON;
				break;
			}
		}
		if( OFF == i2c_err_flg ){
			R_IICA0_StopCondition();		//�@�ُ픭�����͗\�����ʏ������݂�h�~����ׂɃX�g�b�v�R���f�B�V�������s���Ȃ�
		}
	}
	while(0U == SPD0){}
	
	if( ON == i2c_err_flg ){
		i2c_err_flg = OFF;
		R_IICA0_Create();		// I2C������
	}
}


/************************************************************************/
/* �֐�     : i2c_set_snd_flg											*/
/* �֐���   : ���M�t���O�ݒ�											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���M�t���O�ݒ�												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void i2c_set_snd_flg( UB data )
{
	i2c_snd_flg = data;
}

/************************************************************************/
/* �֐�     : i2c_set_snd_flg											*/
/* �֐���   : ��M�t���O�ݒ�											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ��M�t���O�ݒ�												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void i2c_set_rcv_flg( UB data )
{
	i2c_rcv_flg = data;
}

/************************************************************************/
/* �֐�     : i2c_set_err_flg											*/
/* �֐���   : �G���[�t���O�ݒ�											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �G���[�t���O�ݒ�												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void i2c_set_err_flg( UB data )
{
	i2c_err_flg = data;
}


// ==================================
// =========�ȍ~�e�X�g�R�[�h=========
// ==================================
#if 0
#define I2C_RCV_SND_SIZE	20
UB eep_rx_data[I2C_RCV_SND_SIZE];        /* iica0 receive buffer */
UB eep_tx_data[I2C_RCV_SND_SIZE];        /* iica0 send buffer */

STATIC void test_code_eep(void)
{
	//�G���[���荞�݂͔������邪����ɓǂݏ����o���Ă������n�[�h�ɂ��
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 0, &eep_rx_data[0], 2 );
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 5, &eep_rx_data[2], 2 );
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
///	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 8, &eep_rx_data[4], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 13, &eep_rx_data[6], 2 );
//	memset( &eep_rx_data, 0, sizeof(eep_rx_data) );
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
	eep_tx_data[2] = 11;
	eep_tx_data[3] = 22;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(5);
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
	eep_tx_data[2] = 33;
	eep_tx_data[3] = 44;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
	eep_tx_data[2] = 55;
	eep_tx_data[3] = 66;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
	eep_tx_data[2] = 77;
	eep_tx_data[3] = 88;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(5);
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 0, &eep_rx_data[0], 2 );
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 5, &eep_rx_data[2], 2 );
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
///	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 8, &eep_rx_data[4], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 13, &eep_rx_data[6], 2 );
//	wait_ms(5);

	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	

}
#endif


/************************************************************/
/* END OF TEXT												*/
/************************************************************/
