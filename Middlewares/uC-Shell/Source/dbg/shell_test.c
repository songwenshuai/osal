/*
*********************************************************************************************************
*                                               uC/Shell
*                                            Shell utility
*
*                             (c) Copyright 2007-2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           SHELL TEST SUITE
*
* Filename      : shell_test.c
* Version       : V1.03.01
* Programmer(s) : SR
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    SHELL_TEST_MODULE
#include  <includes.h>
#include  <shell_test.h>
#include  <shell.h>

#if APP_FS_EN
#include  <fs_api.h>
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Shell_Test_Init();
static  CPU_BOOLEAN  Shell_Test_AddTbl();
static  CPU_BOOLEAN  Shell_Test_Exec();
static  CPU_BOOLEAN  Shell_Test_RemTbl();


static  CPU_INT16S  Shell_Test_Cmd (CPU_INT16U        argc,
                                    CPU_CHAR         *argv[],
                                    SHELL_OUT_FNCT    out_fnct,
                                    SHELL_CMD_PARAM  *pcmd_param);

static  CPU_INT16S  Shell_Test_Cmd_Err (CPU_INT16U        argc,
                                        CPU_CHAR         *argv[],
                                        SHELL_OUT_FNCT    out_fnct,
                                        SHELL_CMD_PARAM  *pcmd_param);

static  CPU_INT16S  Shell_Test_Out (CPU_CHAR         *pbuf,
                                    CPU_INT16U        buf_len,
                                    void             *popt);

static  CPU_INT16S  Shell_Test_Out_Err (CPU_CHAR         *pbuf,
                                        CPU_INT16U        buf_len,
                                        void             *popt);


static  CPU_INT16S  Shell_Test_GetPoolSize (SHELL_MODULE_CMD  *pool);


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  SHELL_TEST_EXT  SHELL_CMD    Shell_Test_CmdTbl_1[] = {
    {SHELL_TEST_CMD_1_1, Shell_Test_Cmd},
    {SHELL_TEST_CMD_1_2, Shell_Test_Cmd},
    {0,            0}
};

static  SHELL_TEST_EXT  SHELL_CMD    Shell_Test_CmdTbl_2[] = {
    {SHELL_TEST_CMD_2_1, Shell_Test_Cmd},
    {SHELL_TEST_CMD_2_2, Shell_Test_Cmd},
    {0,            0}
};

static  SHELL_TEST_EXT  SHELL_CMD    Shell_Test_CmdTbl_3[] = {
    {SHELL_TEST_CMD_3_1, Shell_Test_Cmd},
    {SHELL_TEST_CMD_3_2, Shell_Test_Cmd},
    {0,            0}
};

static  SHELL_TEST_EXT  SHELL_CMD    Shell_Test_CmdTbl_4[] = {
    {SHELL_TEST_CMD_4_1, Shell_Test_Cmd},
    {SHELL_TEST_CMD_4_2, Shell_Test_Cmd},
    {0,            0}
};

static  SHELL_TEST_EXT  SHELL_CMD    Shell_Test_CmdTbl_5[] = {
    {SHELL_TEST_CMD_5_1, Shell_Test_Cmd_Err},
    {SHELL_TEST_CMD_5_2, Shell_Test_Cmd_Err},
    {0,            0}
};

static  SHELL_TEST_EXT  SHELL_CMD    Shell_Test_CmdTbl_6[] = {
    {SHELL_TEST_CMD_6_1, Shell_Test_Cmd},
    {SHELL_TEST_CMD_6_2, Shell_Test_Cmd},
    {0,            0}
};

static  SHELL_TEST_EXT  SHELL_CMD    Shell_Test_CmdTbl_7[] = {
    {0,            0}
};


/*
*********************************************************************************************************
*                                           INITIALIZED DATA
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           Shell_Test()
*
* Description : Test Shell.
*
* Arguments   : none.
*
* Returns     : none.
*
* Caller(s)   : Your Product's Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  Shell_Test (void)
{
    CPU_BOOLEAN  ret_val;
  
  
    SHELL_TEST_TRACE_DEBUG(("Testing Shell... "));
  
    
    ret_val = Shell_Test_Init();
    if (ret_val != DEF_OK) {
        return;
    }
    
    ret_val = Shell_Test_AddTbl();
    if (ret_val != DEF_OK) {
        return;
    }
    
    ret_val = Shell_Test_Exec();
    if (ret_val != DEF_OK) {
        return;
    }
    
    ret_val = Shell_Test_RemTbl();
    if (ret_val != DEF_OK) {
        return;
    }    
    
    
    SHELL_TEST_TRACE_DEBUG(("done.\n\r"));    
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          Shell_Test_Init()
*
* Description : Test Shell - Init.
*
* Arguments   : none.
*
* Returns     : DEF_OK,   if no error.
*
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Shell_Test().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Shell_Test_Init ()
{
    CPU_BOOLEAN  ret_val;
    

    ret_val = Shell_Init();
    if (ret_val != DEF_OK) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Init (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }
    
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                        Shell_Test_AddTbl()
*
* Description : Test Shell - Add tables
*
* Arguments   : none.
*
* Returns     : DEF_OK,   if no error.
*
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Shell_Test().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Shell_Test_AddTbl()
{
    SHELL_ERR    err;
    CPU_INT16S   size;


    size = Shell_Test_GetPoolSize(Shell_ModuleCmdFreePoolPtr);
    if (size != 5) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdUsedPoolPtr);
    if (size != 0) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }
    
    
                                                                /* Adding NULL tbl.                                     */     
    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_1, (SHELL_CMD *)0, &err);
    if (err != SHELL_ERR_NULL_PTR) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }    
    
                                                                /* Adding tbl with too long name.                       */
    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_TOO_LONG, Shell_Test_CmdTbl_1, &err);
    if (err != SHELL_ERR_MODULE_CMD_NAME_TOO_LONG) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    } 

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdFreePoolPtr);
    if (size != 5) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdUsedPoolPtr);
    if (size != 0) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_1, Shell_Test_CmdTbl_1, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }
    
                                                                /* Trying to add existing tbl name.                     */
    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_1, Shell_Test_CmdTbl_2, &err);
    if (err != SHELL_ERR_MODULE_CMD_ALREADY_IN) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }
    
                                                                /* Trying to add existing tbl, new name.                */
    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_2, Shell_Test_CmdTbl_1, &err);
    if (err != SHELL_ERR_MODULE_CMD_ALREADY_IN) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }
    
                                                                /* Trying to add existing tbl and name.                 */
    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_1, Shell_Test_CmdTbl_1, &err);
    if (err != SHELL_ERR_MODULE_CMD_ALREADY_IN) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }   

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdFreePoolPtr);
    if (size != 4) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdUsedPoolPtr);
    if (size != 1) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Adding tbl without name.                             */
    Shell_CmdTblAdd((CPU_CHAR *)0, Shell_Test_CmdTbl_2, &err);    
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Adding tbl with name len of 0.                       */
    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_NO, Shell_Test_CmdTbl_3, &err);    
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Trying to add existing tbl and name.                 */
    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_3, Shell_Test_CmdTbl_3, &err);
    if (err != SHELL_ERR_MODULE_CMD_ALREADY_IN) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdFreePoolPtr);
    if (size != 2) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdUsedPoolPtr);
    if (size != 3) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_4, Shell_Test_CmdTbl_4, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Trying to add empty cmd tbl.                         */
    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_7, Shell_Test_CmdTbl_7, &err);
    if (err != SHELL_ERR_MODULE_CMD_EMPTY) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }
                                                                /* Trying to add empty cmd tbl with empty name.         */
    Shell_CmdTblAdd((CPU_CHAR *)0, Shell_Test_CmdTbl_7, &err);
    if (err != SHELL_ERR_MODULE_CMD_EMPTY) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }


    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_5, Shell_Test_CmdTbl_5, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdFreePoolPtr);
    if (size != 0) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdUsedPoolPtr);
    if (size != 5) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }
    
                                                                /* Trying to add a tbl when there is no entry left.     */
    Shell_CmdTblAdd(SHELL_TEST_CMD_TBL_NAME_6, Shell_Test_CmdTbl_6, &err);
    if (err != SHELL_ERR_MODULE_CMD_NONE_AVAIL) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    } 

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdFreePoolPtr);
    if (size != 0) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdUsedPoolPtr);
    if (size != 5) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }
    
    return (DEF_OK);    
}


/*
*********************************************************************************************************
*                                        Shell_Test_Exec()
*
* Description : Test Shell - Add tables
*
* Arguments   : none.
*
* Returns     : DEF_OK,   if no error.
*
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Shell_Test().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Shell_Test_Exec()
{
    SHELL_ERR        err;
    SHELL_CMD_PARAM  cmd_param;
    CPU_CHAR         cmd_line[SHELL_TEST_CMD_LINE_LENGTH_MAX];
    CPU_BOOLEAN      session_active;


    cmd_param.pcur_working_dir = (void *)        0;
    cmd_param.pout_opt         = (void *)        0;
    cmd_param.psession_active  = (CPU_BOOLEAN *)&session_active;
    

                                                                /* Passing cmd line NULL ptr.                           */
    Shell_Exec((CPU_CHAR *)0, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NULL_PTR) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Passing out funct NULL ptr.                          */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_1);
    Shell_Exec(cmd_line, (SHELL_OUT_FNCT)0, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Passing cmd param NULL ptr.                         */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_1);
    Shell_Exec(cmd_line, &Shell_Test_Out, (SHELL_CMD_PARAM *)0, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }
    
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_1);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_2);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Passing unknown cmd.                                 */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_3);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_CMD_NOT_FOUND) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Passing unknown cmd in unknown tbl.                  */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_4);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_CMD_NOT_FOUND) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_5);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Passing too many arguments.                          */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_6);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_ARG_TBL_FULL) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Cmd with err.                                        */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_7);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_CMD_EXEC) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Out fnct with err.                                   */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_2);
    Shell_Exec(cmd_line, &Shell_Test_Out_Err, &cmd_param, &err);
    if (err != SHELL_ERR_CMD_EXEC) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* No module cmd name.                                  */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_8);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_CMD_NOT_FOUND) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Module cmd name too long.                            */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_9);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_CMD_NOT_FOUND) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_10);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_11);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_12);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* String and too many arg.                             */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_13);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_ARG_TBL_FULL) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_14);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_15);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_16);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_17);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* String and too many arg.                             */
    Str_Copy(cmd_line, SHELL_TEST_CMD_LINE_18);
    Shell_Exec(cmd_line, &Shell_Test_Out, &cmd_param, &err);
    if (err != SHELL_ERR_ARG_TBL_FULL) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_Exec (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                        Shell_Test_RemTbl()
*
* Description : Test Shell - Add tables
*
* Arguments   : none.
*
* Returns     : DEF_OK,   if no error.
*
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Shell_Test().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Shell_Test_RemTbl()
{
    SHELL_ERR   err;
    CPU_INT16S  size;


    size = Shell_Test_GetPoolSize(Shell_ModuleCmdFreePoolPtr);
    if (size != 0) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdUsedPoolPtr);
    if (size != 5) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Shell_CmdTblRem(SHELL_TEST_CMD_TBL_NAME_1, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_RemTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Rem previously rem'd tbl.                            */
    Shell_CmdTblRem(SHELL_TEST_CMD_TBL_NAME_1, &err);
    if (err != SHELL_ERR_MODULE_CMD_NOT_FOUND) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_RemTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdFreePoolPtr);
    if (size != 1) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdUsedPoolPtr);
    if (size != 4) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Rem NULL tbl name.                                   */
    Shell_CmdTblRem((CPU_CHAR *)0, &err);
    if (err != SHELL_ERR_NULL_PTR) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_RemTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Shell_CmdTblRem(SHELL_TEST_CMD_TBL_NAME_2, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_RemTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Shell_CmdTblRem(SHELL_TEST_CMD_TBL_NAME_3, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_RemTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Shell_CmdTblRem(SHELL_TEST_CMD_TBL_NAME_4, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_RemTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    Shell_CmdTblRem(SHELL_TEST_CMD_TBL_NAME_5, &err);
    if (err != SHELL_ERR_NONE) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_RemTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdFreePoolPtr);
    if (size != 5) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdUsedPoolPtr);
    if (size != 0) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

                                                                /* Rem non inserted tbl.                                */
    Shell_CmdTblRem(SHELL_TEST_CMD_TBL_NAME_6, &err);
    if (err != SHELL_ERR_MODULE_CMD_NOT_FOUND) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_RemTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdFreePoolPtr);
    if (size != 5) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }

    size = Shell_Test_GetPoolSize(Shell_ModuleCmdUsedPoolPtr);
    if (size != 0) {
        SHELL_TEST_TRACE_DEBUG(("Error Shell_Test_AddTbl (line %d)\n\r", __LINE__));
        return (DEF_FAIL);
    }
    
    return (DEF_OK);    
}


/*
*********************************************************************************************************
*                                           Shell_Test_Cmd()
*
* Description : Test command.
*
* Arguments   : argc        Argument count supplied to the function via argv.
*               argv        Array of pointer to the strings which are those arguments.
*               out_fnct    Pointer to 'output' function.
*               pcmd_param  Pointer to command additional parameters.
*
* Returns     : Number of positive octets output, if no error.
*
*               SHELL_EXEC_ERR,                   otherwise.
*
* Caller(s)   : Shell_Exec().
*
* Note(s)     : (1) This function is called by its pointer from Shell_Exec().
*********************************************************************************************************
*/

CPU_INT16S  Shell_Test_Cmd (CPU_INT16U        argc,
                           CPU_CHAR         *argv[],
                           SHELL_OUT_FNCT    out_fnct,
                           SHELL_CMD_PARAM  *pcmd_param)
{
    CPU_INT16U  cmd_name_len;
    CPU_INT16S  output;
    CPU_INT16S  ret_val;


    cmd_name_len = Str_Len(argv[0]);
    
    if (out_fnct != (SHELL_OUT_FNCT)0) {
    
        output       = out_fnct(argv[0],
                                cmd_name_len,
                                pcmd_param->pout_opt);

        switch(output) {
            case SHELL_OUT_RTN_CODE_CONN_CLOSED:
            case SHELL_OUT_ERR:
                  ret_val = SHELL_EXEC_ERR;
                  break;

            default:
                 ret_val = output;
        }
    
    } else {
      ret_val = cmd_name_len;
    }
    
    return (ret_val);
}


/*
*********************************************************************************************************
*                                         Shell_Test_Cmd_Err()
*
* Description : Test command with error.
*
* Arguments   : argc        Argument count supplied to the function via argv.
*               argv        Array of pointer to the strings which are those arguments.
*               out_fnct    Pointer to 'output' function.
*               pcmd_param  Pointer to command additional parameters.
*
* Returns     : SHELL_EXEC_ERR
*
* Caller(s)   : Shell_Exec().
*
* Note(s)     : (1) This function is called by its pointer from Shell_Exec().
*********************************************************************************************************
*/

CPU_INT16S  Shell_Test_Cmd_Err (CPU_INT16U        argc,
                               CPU_CHAR         *argv[],
                               SHELL_OUT_FNCT    out_fnct,
                               SHELL_CMD_PARAM  *pcmd_param)
{
    return (SHELL_EXEC_ERR);
}


/*
*********************************************************************************************************
*                                             Shell_Test_Out()
*
* Description : Test output function.
*
* Arguments   : pbuf        Pointer to buffer to output.
*               buf_len     Length of buffer.
*               popt        Pointer to implementation specific additional parameter.
*
* Returns     : Length of pbuf (See Note #2).
*
* Caller(s)   : Shell_Test_Cmd().
*
* Note(s)     : (1) This function is called by its pointer from Shell_Test_Cmd().
*
*               (2) The return value of this function is implementation specific.
*********************************************************************************************************
*/

CPU_INT16S  Shell_Test_Out (CPU_CHAR   *pbuf,
                            CPU_INT16U  buf_len,
                            void       *popt)
{
    return (buf_len);
}


/*
*********************************************************************************************************
*                                           Shell_Test_Out_Err()
*
* Description : Test output function.
*
* Arguments   : pbuf        Pointer to buffer to output.
*               buf_len     Length of buffer.
*               popt        Pointer to implementation specific additional parameter.
*
* Returns     : SHELL_OUT_ERR.
*
* Caller(s)   : Shell_Test_Cmd().
*
* Note(s)     : (1) This function is called by its pointer from Shell_Test_Cmd().
*********************************************************************************************************
*/

CPU_INT16S  Shell_Test_Out_Err (CPU_CHAR   *pbuf,
                                CPU_INT16U  buf_len,
                                void       *popt)
{
    return (SHELL_OUT_ERR);
}


/*
*********************************************************************************************************
*                                         Shell_Test_GetPoolSize()
*
* Description : Get size of pool
*
* Arguments   : pool        Pointer to pool of interest.
*
* Returns     : Size of pool.
*
* Caller(s)   : Shell_Test_AddTbl(),
*               Shell_Test_RemTbl().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_INT16S  Shell_Test_GetPoolSize (SHELL_MODULE_CMD  *pool)
{
    CPU_INT16S         size;
    SHELL_MODULE_CMD  *cur_tbl;


    size    = 0;
    cur_tbl = pool;
    
    while (cur_tbl != (SHELL_MODULE_CMD *)0) {
        size ++;
        cur_tbl = cur_tbl->NextModuleCmdPtr;
    }

    return (size);
}


