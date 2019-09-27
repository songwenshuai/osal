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
* Filename      : shell_test.h
* Version       : V1.03.01
* Programmer(s) : SR
*********************************************************************************************************
* 
*
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SHELL_TEST present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  SHELL_TEST_PRESENT                                     /* See Note #1.                                         */
#define  SHELL_TEST_PRESENT


/*
*********************************************************************************************************
*                                          SHELL VERSION NUMBER
*
* Note(s) : (1) (a) The Shell software version is denoted as follows :
*
*                       Vx.yy
*
*                           where
*                                  V     denotes 'Version' label
*                                  x     denotes major software version revision number
*                                  yy    denotes minor software version revision number
*
*               (b) The Shell software version label #define is formatted as follows :
*
*                       ver = x.yy * 100
*
*                           where
*                                  ver   denotes software version number scaled as 
*                                        an integer value
*                                  x.yy  denotes software version number
*********************************************************************************************************
*/

#define  SHELL_TEST_VERSION             100u                    /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   SHELL_TEST_MODULE
#define  SHELL_TEST_EXT
#else
#define  SHELL_TEST_EXT  extern
#endif


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <shell.h>


/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/

#define  SHELL_TEST_CMD_LINE_LENGTH_MAX        100

/*
*********************************************************************************************************
*                                           TBL NAME DEFINES
*********************************************************************************************************
*/

#define  SHELL_TEST_CMD_TBL_NAME_1          "Test1"
#define  SHELL_TEST_CMD_TBL_NAME_2          "Test2"
#define  SHELL_TEST_CMD_TBL_NAME_3          "Test3"
#define  SHELL_TEST_CMD_TBL_NAME_4          "Test4"
#define  SHELL_TEST_CMD_TBL_NAME_5          "Test5"
#define  SHELL_TEST_CMD_TBL_NAME_6          "Test6"
#define  SHELL_TEST_CMD_TBL_NAME_7          "Test7"
#define  SHELL_TEST_CMD_TBL_NAME_NO         ""
#define  SHELL_TEST_CMD_TBL_NAME_TOO_LONG   "NameTooLong"


/*
*********************************************************************************************************
*                                           CMD NAME DEFINES
*********************************************************************************************************
*/

#define  SHELL_TEST_CMD_1_1                 "Test1_1"
#define  SHELL_TEST_CMD_1_2                 "Test1_2"
#define  SHELL_TEST_CMD_2_1                 "Test2_1"
#define  SHELL_TEST_CMD_2_2                 "Test2_2"
#define  SHELL_TEST_CMD_3_1                 "Test3_1"
#define  SHELL_TEST_CMD_3_2                 "Test3_2"
#define  SHELL_TEST_CMD_4_1                 "Test4_1"
#define  SHELL_TEST_CMD_4_2                 "Test4_2"
#define  SHELL_TEST_CMD_5_1                 "Test5_1"
#define  SHELL_TEST_CMD_5_2                 "Test5_2"
#define  SHELL_TEST_CMD_6_1                 "Test6_1"
#define  SHELL_TEST_CMD_6_2                 "Test6_2"


/*
*********************************************************************************************************
*                                           CMD LINE DEFINES
*********************************************************************************************************
*/

#define  SHELL_TEST_CMD_LINE_1              "Test1_1"
#define  SHELL_TEST_CMD_LINE_2              "Test2_2"
#define  SHELL_TEST_CMD_LINE_3              "Test2_3"
#define  SHELL_TEST_CMD_LINE_4              "Test7_3"
#define  SHELL_TEST_CMD_LINE_5              "Test3_1 -a -b -c -d"
#define  SHELL_TEST_CMD_LINE_6              "Test3_1 -a -b -c -d -e"
#define  SHELL_TEST_CMD_LINE_7              "Test5_1 -a"
#define  SHELL_TEST_CMD_LINE_8              "1"
#define  SHELL_TEST_CMD_LINE_9              "TestTestTest"
#define  SHELL_TEST_CMD_LINE_10             "Test1_1 \"a string\""
#define  SHELL_TEST_CMD_LINE_11             "Test1_1 \"a string with many words\""
#define  SHELL_TEST_CMD_LINE_12             "Test1_1 \"a string with many words\" -b -c -d"
#define  SHELL_TEST_CMD_LINE_13             "Test1_1 \"a string with many words\" -b -c -d -e"
#define  SHELL_TEST_CMD_LINE_14             "Test1_1 \"a string with many words\" \"another string\" -c -d"
#define  SHELL_TEST_CMD_LINE_15             "Test1_1 \"a string with many words\" \"another string -c -d"
#define  SHELL_TEST_CMD_LINE_16             "Test1_1 \"a string\"concanated"
#define  SHELL_TEST_CMD_LINE_17             "Test1_1 \"a string\"\"a\"\"b\"\"c\""
#define  SHELL_TEST_CMD_LINE_18             "Test1_1 \"a string\"\"a\"\"b\"\"c\"\"d\""


/*
*********************************************************************************************************
*                                           GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  Shell_Test (void);


/*
*********************************************************************************************************
*                                              TRACING
*********************************************************************************************************
*/

                                                                /* Trace level, default to TRACE_LEVEL_OFF              */
#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                                 0
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                                1
#endif

#ifndef  TRACE_LEVEL_DEBUG
#define  TRACE_LEVEL_DEBUG                               2
#endif

#ifndef  SHELL_TEST_TRACE_LEVEL
#define  SHELL_TEST_TRACE_LEVEL          TRACE_LEVEL_DEBUG
#endif

#ifndef  SHELL_TEST_TRACE
#define  SHELL_TEST_TRACE                           printf
#endif

#define  SHELL_TEST_TRACE_INFO(x)        ((SHELL_TEST_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(SHELL_TEST_TRACE x) : (void)0)
#define  SHELL_TEST_TRACE_DEBUG(x)       ((SHELL_TEST_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(SHELL_TEST_TRACE x) : (void)0)


/*
*********************************************************************************************************
*                                         CONFIGURATION ERRORS
*
* Note(s) : The configuration have to be set to some particular values in order for the tests to be
*           effective.  Hence, some error checking is performed here to enforce the use of those values.
*********************************************************************************************************
*/

#if    ((SHELL_CFG_CMD_TBL_SIZE             !=                  5))
#error  "SHELL_CFG_CMD_TBL_SIZE                 illegally #define'd in 'app_cfg.h'"
#error  "                                       [MUST be ==     5]                "
#endif


#if    ((SHELL_CFG_CMD_ARG_NBR_MAX          !=                  5))
#error  "SHELL_CFG_CMD_ARG_NBR_MAX              illegally #define'd in 'app_cfg.h'"
#error  "                                       [MUST be ==     5]                "
#endif


#if    ((SHELL_CFG_MODULE_CMD_NAME_LEN_MAX  !=                  6))
#error  "SHELL_CFG_MODULE_CMD_NAME_LEN_MAX      illegally #define'd in 'app_cfg.h'"
#error  "                                       [MUST be ==     6]                "
#endif


/*
*********************************************************************************************************
*                                              MODULE END
*
* Note(s) : See 'MODULE  Note #1'.
*********************************************************************************************************
*/

#endif                                                          /* End of SHELL_TEST module include (see Note #1).      */
