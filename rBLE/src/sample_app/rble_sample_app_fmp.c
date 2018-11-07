/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_fmp.c
 *
 * @brief   rBLE Sample Application
 *
 * Copyright(C) 2016 Renesas Electronics Corporation
 *
 * $Rev: 9242 $
 *
 ****************************************************************************************
 */

/**
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwble_config.h"
#include "arch.h"

#ifdef CFG_SAMPLE
#ifdef CONFIG_EMBEDDED

#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>

#include    "rble.h"
#include    "rble_app.h"
#include    "menu_sel.h"
#include    "console.h"

#include    "prf_sel.h"

#if (PRF_SEL_FMPL || PRF_SEL_FMPT)
/**
 * DEFINES
 ****************************************************************************************
 */


/**
 * TYPE DEFINES
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/* --------------------------- Find Me Profile ---------------------------------------*/
#if (PRF_SEL_FMPT)
static void RBLE_APP_FMPT_CallBack( RBLE_FMPT_EVENT *event );                               /* FMP(Target) Call Back */
static BOOL RBLE_FMP_Target_Enable_Test( void );                                            /* A FMP_Target_Enable command is executed. */
static BOOL RBLE_FMP_Target_Disable_Test( void );                                           /* A FMP_Target_Disable command is executed. */
#endif

#if (PRF_SEL_FMPL)
static void RBLE_APP_FMPL_CallBack( RBLE_FMPL_EVENT *event );                               /* FMP(Locator) Call Back */
static BOOL RBLE_FMP_Locator_Enable_Test( void );                                           /* A FMP_Locator_Enable command is executed. */
static BOOL RBLE_FMP_Locator_Disable_Test( void );                                          /* A FMP_Locator_Disable command is executed. */
static BOOL RBLE_FMP_Locator_Set_Alert_Test( void );                                        /* A FMP_Locator_Set_Alert command is executed. */
#endif

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Find Me Profile Select Menu */
static const MENU_INFO  Test_Prog_FMP_Menu[] = {
#if (PRF_SEL_FMPT)
    {  1,   RBLE_FMP_Target_Enable_Test,        NULL,       "1.FMP Target_Enable\n",        },
    {  2,   RBLE_FMP_Target_Disable_Test,       NULL,       "2.FMP Target_Disable\n",       },
#endif
#if (PRF_SEL_FMPL)
    {  3,   RBLE_FMP_Locator_Enable_Test,       NULL,       "3.FMP Locator_Enable\n",       },
    {  4,   RBLE_FMP_Locator_Disable_Test,      NULL,       "4.FMP Locator_Disable\n",      },
    {  5,   RBLE_FMP_Locator_Set_Alert_Test,    NULL,       "5.FMP Locator_Set_Alert\n",    },
#endif
    {  ESC_KEY, Menu_Exit,                      NULL,       "ESC Key: Menu exit\n",         },
    {  0,   NULL,                               NULL,       NULL,                           }
};

const SELECT_MENU   Test_Prog_FMP_Select_Menu = {
    "\n-- BLE Sample Program Find Me Profile Test Menu --",
    Test_Prog_FMP_Menu
};

#if (PRF_SEL_FMPL)
static RBLE_FMP_IAS_CONTENT g_fmp_ias = {
    0x0000,                         /* Start Handle */
    0x0000,                         /* End Handle */
    0x0000,                         /* Alert Level Characteristic handle */
    0x0000,                         /* Alert Level Characteristic value handle */
    0x00,                           /* Alert Level Characteristic properties */
    0x00,                           /* Reserved */
};
#endif

/******************************* Program Area *****************************************/

#if (PRF_SEL_FMPT)
/******************************************************************************************/
/**
 *  @brief rBLE FMP(Target) Call Back
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *event          event parameter
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_APP_FMPT_CallBack( RBLE_FMPT_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_FMP_EVENT_TARGET_ENABLE_COMP:
            printf( "\nrBLE FMP EVENT (TARGET_ENABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.target_enable.status ) );
            Connection_Handle_Disp( event->param.target_enable.conhdl );
            break;
        case RBLE_FMP_EVENT_TARGET_DISABLE_COMP:
            printf( "\nrBLE FMP EVENT (TARGET_DISABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.target_disable.status ) );
            Connection_Handle_Disp( event->param.target_disable.conhdl );
            break;
        case RBLE_FMP_EVENT_TARGET_ALERT_IND:
            printf( "\nrBLE FMP EVENT (TARGET_ALERT_IND)\n" );
            Connection_Handle_Disp( event->param.target_alert_ind.conhdl );
            printf( "alert_lvl = %d\n", event->param.target_alert_ind.alert_lvl );
            break;
        case RBLE_FMP_EVENT_TARGET_COMMAND_DISALLOWED_IND:
            printf( "\nrBLE FMP EVENT (COMMAND_DISALLOWED_IND) Status(%s)\n", Get_Status_Str( event->param.cmd_disallowed_ind.status ) );
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A FMP_Target_Enable command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_FMP_Target_Enable_Test( void )
{
    /* security level */
    uint8_t sec_lvl[] = {
        RBLE_SVC_SEC_NONE,
        RBLE_SVC_SEC_UNAUTH,
        RBLE_SVC_SEC_UNAUTH|RBLE_SVC_SEC_ENC,
    };
    uint8_t     select = 0;
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ((0 != RBLE_Parameter_Cnt) && (2 >= RBLE_Parameter_Arg[ 0 ]))
        {
            select = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_FMP_Target_Enable( Connection_Handle, sec_lvl[select], RBLE_APP_FMPT_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A FMP_Target_Disable command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_FMP_Target_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_FMP_Target_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_FMPT)


#if (PRF_SEL_FMPL)
/******************************************************************************************/
/**
 *  @brief rBLE FMP(Locator) Call Back
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *event          event parameter
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_APP_FMPL_CallBack( RBLE_FMPL_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_FMP_EVENT_LOCATOR_ENABLE_COMP:
            printf( "\nrBLE FMP EVENT (LOCATOR_ENABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.locator_enable.status ) );
            Connection_Handle_Disp( event->param.locator_enable.conhdl );
            printf( "* Immediate Alert service\n");
            printf( " Start Handle    = 0x%04X\n", event->param.locator_enable.ias.shdl );
            printf( " End Handle      = 0x%04X\n\n", event->param.locator_enable.ias.ehdl );
            printf( " alert_char_hdl  = 0x%04X\n", event->param.locator_enable.ias.alert_char_hdl);
            printf( " alert_val_hdl   = 0x%04X\n", event->param.locator_enable.ias.alert_val_hdl);
            printf( " alert_char_prop = 0x%02X\n\n", event->param.locator_enable.ias.alert_char_prop);

            g_fmp_ias = event->param.locator_enable.ias;
            break;
        case RBLE_FMP_EVENT_LOCATOR_DISABLE_COMP:
            printf( "\nrBLE FMP EVENT (LOCATOR_DISABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.locator_disable.status ) );
            Connection_Handle_Disp( event->param.locator_disable.conhdl );
            break;
        case RBLE_FMP_EVENT_LOCATOR_ERROR_IND:
            printf( "\nrBLE FMP EVENT (LOCATOR_ERROR_IND) Status(%s)\n", Get_Status_Str( event->param.locator_error_ind.status ) );
            Connection_Handle_Disp( event->param.locator_error_ind.conhdl );
            break;
        case RBLE_FMP_EVENT_LOCATOR_COMMAND_DISALLOWED_IND:
            printf( "\nrBLE FMP EVENT (COMMAND_DISALLOWED_IND) Status(%s)\n", Get_Status_Str( event->param.cmd_disallowed_ind.status ) );
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A FMP_Locator_Enable command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_FMP_Locator_Enable_Test( void )
{
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;    /* Discovery type */
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if (0x0000 != g_fmp_ias.shdl) {
            con_type = RBLE_PRF_CON_NORMAL;
        }

        /* API Call */
        Ret_Status = RBLE_FMP_Locator_Enable( Connection_Handle, con_type, &g_fmp_ias, RBLE_APP_FMPL_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A FMP_Locator_Disable command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_FMP_Locator_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_FMP_Locator_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A FMP_Locator_Set_Alert command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_FMP_Locator_Set_Alert_Test( void )
{
    int32_t         Parameter_No;
    RBLE_STATUS     Ret_Status;
    uint8_t         alert_lvl;


    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter_No = ILLEGAL_NO;
        }

        switch (Parameter_No)
        {
            case 1:
                alert_lvl = RBLE_SVC_ALERT_MILD;
                break;
            case 2:
                alert_lvl = RBLE_SVC_ALERT_HIGH;
                break;
            case 0:
            default:
                alert_lvl = RBLE_SVC_ALERT_NONE;
                break;
        }

        /* API Call */
        Ret_Status = RBLE_FMP_Locator_Set_Alert(Connection_Handle, alert_lvl);

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_FMPL)


#endif  ///#if (PRF_SEL_FMPL || PRF_SEL_FMPT)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
