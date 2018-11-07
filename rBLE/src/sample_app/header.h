/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : header.h														*/
/* �@�\         : �w�b�_�C���N���[�h�p											*/
/* �ύX����		: 2018.01.25 Axia Soft Design ���� ��	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/

#ifndef _HEADER_H_
#define _HEADER_H_

/* �W�� */
//#include <stdio.h>
//#include <limits.h>
//#include <stdlib.h>
//#include <ctype.h>
#include <string.h>
//#include <stdarg.h>
#include <math.h>

/* �V�X�e�����ʒ�` */
#include "sys.h"

/* �h���C�o */
#include "compiler.h"
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_iica.h"
#include "r_cg_port.h"
#include "r_cg_intp.h"
#include "r_cg_it.h"

/* �~�h�� */
#include "eep.h"
#include "cpu_com.h"
#include "calc_data.h"

/* �A�v���P�[�V���� */
#include "user_main.h"

#if FUNC_DEBUG_CALC_NON == OFF
#include "calc/pulseoximeter/calc_pulseoximeter.h"
#include "calc/apnea/calc_apnea.h"
#include "calc/apnea/calc_snore.h"
#endif

#endif /*_HEADER_H */
