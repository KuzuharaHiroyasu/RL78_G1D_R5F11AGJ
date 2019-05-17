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
void wait_ms( int ms );
#if 0	//�e�X�g�R�[�h
STATIC void test_code_eep(void);
#endif

/********************/
/*     �����ϐ�     */
/********************/

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
