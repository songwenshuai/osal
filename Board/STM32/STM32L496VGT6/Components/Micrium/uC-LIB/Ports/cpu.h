/*
*********************************************************************************************************
*                                                uC/CPU
*                                    CPU CONFIGURATION & PORT LAYER
*
*                          (c) Copyright 2004-2015; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/CPU is provided in source form to registered licensees ONLY.  It is 
*               illegal to distribute this source code to any third party unless you receive 
*               written permission by an authorized Micrium representative.  Knowledge of 
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest 
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            CPU PORT FILE
*
*                                            ARM-Cortex-M4
*                                            IAR C Compiler
*
* Filename      : cpu.h
* Version       : V1.30.02.00
* Programmer(s) : JJL
*                 BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This CPU header file is protected from multiple pre-processor inclusion through use of 
*               the  CPU module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  CPU_MODULE_PRESENT                                     /* See Note #1.                                         */
#define  CPU_MODULE_PRESENT


/*
*********************************************************************************************************
*                                          CPU INCLUDE FILES
*
* Note(s) : (1) The following CPU files are located in the following directories :
*
*               (a) \<Your Product Application>\cpu_cfg.h
*
*               (b) (1) \<CPU-Compiler Directory>\cpu_def.h
*                   (2) \<CPU-Compiler Directory>\<cpu>\<compiler>\cpu*.*
*
*                       where
*                               <Your Product Application>      directory path for Your Product's Application
*                               <CPU-Compiler Directory>        directory path for common   CPU-compiler software
*                               <cpu>                           directory name for specific CPU
*                               <compiler>                      directory name for specific compiler
*
*           (2) Compiler MUST be configured to include as additional include path directories :
*
*               (a) '\<Your Product Application>\' directory                            See Note #1a
*
*               (b) (1) '\<CPU-Compiler Directory>\'                  directory         See Note #1b1
*                   (2) '\<CPU-Compiler Directory>\<cpu>\<compiler>\' directory         See Note #1b2
*
*           (3) Since NO custom library modules are included, 'cpu.h' may ONLY use configurations from
*               CPU configuration file 'cpu_cfg.h' that do NOT reference any custom library definitions.
*
*               In other words, 'cpu.h' may use 'cpu_cfg.h' configurations that are #define'd to numeric
*               constants or to NULL (i.e. NULL-valued #define's); but may NOT use configurations to
*               custom library #define's (e.g. DEF_DISABLED or DEF_ENABLED).
*********************************************************************************************************
*/

#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */

#include  <OSAL_Comdef.h>


#ifdef __cplusplus
extern  "C" {
#endif

/*
*********************************************************************************************************
*                                   CORE CPU MODULE VERSION NUMBER
*
* Note(s) : (1) (a) The core CPU module software version is denoted as follows :
*
*                       Vx.yy.zz
*
*                           where
*                                   V               denotes 'Version' label
*                                   x               denotes     major software version revision number
*                                   yy              denotes     minor software version revision number
*                                   zz              denotes sub-minor software version revision number
*
*               (b) The software version label #define is formatted as follows :
*
*                       ver = x.yyzz * 100 * 100
*
*                           where
*                                   ver             denotes software version number scaled as an integer value
*                                   x.yyzz          denotes software version number, where the unscaled integer 
*                                                       portion denotes the major version number & the unscaled 
*                                                       fractional portion denotes the (concatenated) minor 
*                                                       version numbers
*********************************************************************************************************
*/

#define  CPU_CORE_VERSION                              13002u   /* See Note #1.                                         */

/*
*********************************************************************************************************
*                                    CONFIGURE STANDARD DATA TYPES
*
* Note(s) : (1) Configure standard data types according to CPU-/compiler-specifications.
*
*           (2) (a) (1) 'CPU_FNCT_VOID' data type defined to replace the commonly-used function pointer
*                       data type of a pointer to a function which returns void & has no arguments.
*
*                   (2) Example function pointer usage :
*
*                           CPU_FNCT_VOID  FnctName;
*
*                           FnctName();
*
*               (b) (1) 'CPU_FNCT_PTR'  data type defined to replace the commonly-used function pointer
*                       data type of a pointer to a function which returns void & has a single void
*                       pointer argument.
*
*                   (2) Example function pointer usage :
*
*                           CPU_FNCT_PTR   FnctName;
*                           void          *p_obj
*
*                           FnctName(p_obj);
*********************************************************************************************************
*/

typedef            void        CPU_VOID;
typedef            char        CPU_CHAR;                        /*  8-bit character                                     */
typedef  unsigned  char        CPU_BOOLEAN;                     /*  8-bit boolean or logical                            */
typedef  unsigned  char        CPU_INT08U;                      /*  8-bit unsigned integer                              */
typedef    signed  char        CPU_INT08S;                      /*  8-bit   signed integer                              */
typedef  unsigned  short       CPU_INT16U;                      /* 16-bit unsigned integer                              */
typedef    signed  short       CPU_INT16S;                      /* 16-bit   signed integer                              */
typedef  unsigned  int         CPU_INT32U;                      /* 32-bit unsigned integer                              */
typedef    signed  int         CPU_INT32S;                      /* 32-bit   signed integer                              */
typedef  unsigned  long  long  CPU_INT64U;                      /* 64-bit unsigned integer                              */
typedef    signed  long  long  CPU_INT64S;                      /* 64-bit   signed integer                              */

typedef            float       CPU_FP32;                        /* 32-bit floating point                                */
typedef            double      CPU_FP64;                        /* 64-bit floating point                                */


typedef  volatile  CPU_INT08U  CPU_REG08;                       /*  8-bit register                                      */
typedef  volatile  CPU_INT16U  CPU_REG16;                       /* 16-bit register                                      */
typedef  volatile  CPU_INT32U  CPU_REG32;                       /* 32-bit register                                      */
typedef  volatile  CPU_INT64U  CPU_REG64;                       /* 64-bit register                                      */


typedef            void      (*CPU_FNCT_VOID)(void);            /* See Note #2a.                                        */
typedef            void      (*CPU_FNCT_PTR )(void *p_obj);     /* See Note #2b.                                        */

/*
*********************************************************************************************************
*                                         CPU_SW_Exception()
*
* Description : Trap unrecoverable software exception.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) CPU_SW_Exception() deadlocks the current code execution -- whether multi-tasked/
*                   -processed/-threaded or single-threaded -- when the current code execution cannot
*                   gracefully recover or report a fault or exception condition.
*
*                   See also 'cpu_core.h  CPU_SW_EXCEPTION()  Note #1'.
*********************************************************************************************************
*/

__STATIC_INLINE void CPU_SW_Exception (void)
{
    for (;;) {
        ;
    }
}


/*
*********************************************************************************************************
*                                         CPU_SW_EXCEPTION()
*
* Description : Trap unrecoverable software exception.
*
* Argument(s) : err_rtn_val     Error type &/or value of the calling function to return (see Note #2b).
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
* Note(s)     : (1) CPU_SW_EXCEPTION() deadlocks the current code execution -- whether multi-tasked/
*                   -processed/-threaded or single-threaded -- when the current code execution cannot 
*                   gracefully recover or report a fault or exception condition.
*
*                   Example CPU_SW_EXCEPTION() call :
*
*                       void  Fnct (CPU_ERR  *p_err)
*                       {
*                           :
*
*                           if (p_err == (CPU_ERR *)0) {        If 'p_err' NULL, cannot return error ...
*                               CPU_SW_EXCEPTION(;);            ... so trap invalid argument exception.
*                           }
*
*                           :
*                       }
*
*                   See also 'cpu_core.c  CPU_SW_Exception()  Note #1'.
*
*               (2) (a) CPU_SW_EXCEPTION()  MAY be developer-implemented to output &/or handle any error or 
*                       exception conditions; but since CPU_SW_EXCEPTION() is intended to trap unrecoverable 
*                       software  conditions, it is recommended that developer-implemented versions prevent 
*                       execution of any code following calls to CPU_SW_EXCEPTION() by deadlocking the code 
*                       (see Note #1).
*
*                           Example CPU_SW_EXCEPTION() :
*
*                               #define  CPU_SW_EXCEPTION(err_rtn_val)      do {                         \
*                                                                               Log(__FILE__, __LINE__); \
*                                                                               CPU_SW_Exception();      \
*                                                                           } while (0)
*
*                   (b) (1) However, if execution of code following calls to CPU_SW_EXCEPTION() is required 
*                           (e.g. for automated testing); it is recommended that the last statement in 
*                           developer-implemented versions be to return from the current function to prevent 
*                           possible software exception(s) in the current function from triggering CPU &/or 
*                           hardware exception(s).
*
*                           Example CPU_SW_EXCEPTION() :
*
*                               #define  CPU_SW_EXCEPTION(err_rtn_val)      do {                         \
*                                                                               Log(__FILE__, __LINE__); \
*                                                                               return  err_rtn_val;     \
*                                                                           } while (0)
*
*                           (A) Note that 'err_rtn_val' in the return statement MUST NOT be enclosed in 
*                               parentheses.  This allows CPU_SW_EXCEPTION() to return from functions that 
*                               return 'void', i.e. NO return type or value (see also Note #2b2A).
*
*                       (2) In order for CPU_SW_EXCEPTION() to return from functions with various return 
*                           types/values, each caller function MUST pass an appropriate error return type 
*                           & value to CPU_SW_EXCEPTION().
*
*                           (A) Note that CPU_SW_EXCEPTION()  MUST NOT be passed any return type or value 
*                               for functions that return 'void', i.e. NO return type or value; but SHOULD 
*                               instead be passed a single semicolon.  This prevents possible compiler 
*                               warnings that CPU_SW_EXCEPTION() is passed too few arguments.  However, 
*                               the compiler may warn that CPU_SW_EXCEPTION() does NOT prevent creating 
*                               null statements on lines with NO other code statements.
*
*                           Example CPU_SW_EXCEPTION() calls :
*
*                               void  Fnct (CPU_ERR  *p_err)
*                               {
*                                   :
*
*                                   if (p_err == (CPU_ERR *)0) {
*                                       CPU_SW_EXCEPTION(;);            Exception macro returns NO value
*                                   }                                       (see Note #2b2A)
*
*                                   :
*                               }
*
*                               CPU_BOOLEAN  Fnct (CPU_ERR  *p_err)
*                               {
*                                   :
*
*                                   if (p_err == (CPU_ERR *)0) {
*                                       CPU_SW_EXCEPTION(DEF_FAIL);     Exception macro returns 'DEF_FAIL'
*                                   }
*
*                                   :
*                               }
*
*                               OBJ  *Fnct (CPU_ERR  *p_err)
*                               {
*                                   :
*
*                                   if (p_err == (CPU_ERR *)0) {
*                                       CPU_SW_EXCEPTION((OBJ *)0);     Exception macro returns NULL 'OBJ *'
*                                   }
*
*                                   :
*                               }
*
*********************************************************************************************************
*/

#ifndef  CPU_SW_EXCEPTION                                                       /* See Note #2.                         */
#define  CPU_SW_EXCEPTION(err_rtn_val)              do {                    \
                                                        CPU_SW_Exception(); \
                                                    } while (0)
#endif

/*
*********************************************************************************************************
*                                   CRITICAL SECTION CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_CRITICAL_METHOD with CPU's/compiler's critical section method :
*
*                                                       Enter/Exit critical sections by ...
*
*                   CPU_CRITICAL_METHOD_INT_DIS_EN      Disable/Enable interrupts
*                   CPU_CRITICAL_METHOD_STATUS_STK      Push/Pop       interrupt status onto stack
*                   CPU_CRITICAL_METHOD_STATUS_LOCAL    Save/Restore   interrupt status to local variable
*
*               (a) CPU_CRITICAL_METHOD_INT_DIS_EN  is NOT a preferred method since it does NOT support
*                   multiple levels of interrupts.  However, with some CPUs/compilers, this is the only
*                   available method.
*
*               (b) CPU_CRITICAL_METHOD_STATUS_STK    is one preferred method since it supports multiple
*                   levels of interrupts.  However, this method assumes that the compiler provides C-level
*                   &/or assembly-level functionality for the following :
*
*                     ENTER CRITICAL SECTION :
*                       (1) Push/save   interrupt status onto a local stack
*                       (2) Disable     interrupts
*
*                     EXIT  CRITICAL SECTION :
*                       (3) Pop/restore interrupt status from a local stack
*
*               (c) CPU_CRITICAL_METHOD_STATUS_LOCAL  is one preferred method since it supports multiple
*                   levels of interrupts.  However, this method assumes that the compiler provides C-level
*                   &/or assembly-level functionality for the following :
*
*                     ENTER CRITICAL SECTION :
*                       (1) Save    interrupt status into a local variable
*                       (2) Disable interrupts
*
*                     EXIT  CRITICAL SECTION :
*                       (3) Restore interrupt status from a local variable
*
*           (2) Critical section macro's most likely require inline assembly.  If the compiler does NOT
*               allow inline assembly in C source files, critical section macro's MUST call an assembly
*               subroutine defined in a 'cpu_a.asm' file located in the following software directory :
*
*                   \<CPU-Compiler Directory>\<cpu>\<compiler>\
*
*                       where
*                               <CPU-Compiler Directory>    directory path for common   CPU-compiler software
*                               <cpu>                       directory name for specific CPU
*                               <compiler>                  directory name for specific compiler
*
*           (3) (a) To save/restore interrupt status, a local variable 'cpu_sr' of type 'CPU_SR' MAY need 
*                   to be declared (e.g. if 'CPU_CRITICAL_METHOD_STATUS_LOCAL' method is configured).
*
*                   (1) 'cpu_sr' local variable SHOULD be declared via the CPU_SR_ALLOC() macro which, 
*                        if used, MUST be declared following ALL other local variables (see any 'cpu.h  
*                        CRITICAL SECTION CONFIGURATION  Note #3a1').
*
*                        Example :
*
*                           void  Fnct (void)
*                           {
*                               CPU_INT08U  val_08;
*                               CPU_INT16U  val_16;
*                               CPU_INT32U  val_32;
*                               CPU_SR_ALLOC();         MUST be declared after ALL other local variables
*                                   :
*                                   :
*                           }
*
*               (b) Configure 'CPU_SR' data type with the appropriate-sized CPU data type large enough to 
*                   completely store the CPU's/compiler's status word.
*********************************************************************************************************
*/

                                                        /* --------------- CPU CRITICAL SECTION METHODS --------------- */
#define  CPU_CRITICAL_METHOD_NONE                  0u   /*                                                              */
#define  CPU_CRITICAL_METHOD_INT_DIS_EN            1u   /* DIS/EN       ints                    (see Note #1a).         */
#define  CPU_CRITICAL_METHOD_STATUS_STK            2u   /* Push/Pop     int status onto stk     (see Note #1b).         */
#define  CPU_CRITICAL_METHOD_STATUS_LOCAL          3u   /* Save/Restore int status to local var (see Note #1c).         */


/*
*********************************************************************************************************
*                                   CRITICAL SECTION CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_CRITICAL_METHOD with CPU's/compiler's critical section method :
*
*                                                       Enter/Exit critical sections by ...
*
*                   CPU_CRITICAL_METHOD_INT_DIS_EN      Disable/Enable interrupts
*                   CPU_CRITICAL_METHOD_STATUS_STK      Push/Pop       interrupt status onto stack
*                   CPU_CRITICAL_METHOD_STATUS_LOCAL    Save/Restore   interrupt status to local variable
*
*               (a) CPU_CRITICAL_METHOD_INT_DIS_EN  is NOT a preferred method since it does NOT support
*                   multiple levels of interrupts.  However, with some CPUs/compilers, this is the only
*                   available method.
*
*               (b) CPU_CRITICAL_METHOD_STATUS_STK    is one preferred method since it supports multiple
*                   levels of interrupts.  However, this method assumes that the compiler provides C-level
*                   &/or assembly-level functionality for the following :
*
*                     ENTER CRITICAL SECTION :
*                       (1) Push/save   interrupt status onto a local stack
*                       (2) Disable     interrupts
*
*                     EXIT  CRITICAL SECTION :
*                       (3) Pop/restore interrupt status from a local stack
*
*               (c) CPU_CRITICAL_METHOD_STATUS_LOCAL  is one preferred method since it supports multiple
*                   levels of interrupts.  However, this method assumes that the compiler provides C-level
*                   &/or assembly-level functionality for the following :
*
*                     ENTER CRITICAL SECTION :
*                       (1) Save    interrupt status into a local variable
*                       (2) Disable interrupts
*
*                     EXIT  CRITICAL SECTION :
*                       (3) Restore interrupt status from a local variable
*
*           (2) Critical section macro's most likely require inline assembly.  If the compiler does NOT
*               allow inline assembly in C source files, critical section macro's MUST call an assembly
*               subroutine defined in a 'cpu_a.asm' file located in the following software directory :
*
*                   \<CPU-Compiler Directory>\<cpu>\<compiler>\
*
*                       where
*                               <CPU-Compiler Directory>    directory path for common   CPU-compiler software
*                               <cpu>                       directory name for specific CPU
*                               <compiler>                  directory name for specific compiler
*
*           (3) (a) To save/restore interrupt status, a local variable 'cpu_sr' of type 'CPU_SR' MAY need
*                   to be declared (e.g. if 'CPU_CRITICAL_METHOD_STATUS_LOCAL' method is configured).
*
*                   (1) 'cpu_sr' local variable SHOULD be declared via the CPU_SR_ALLOC() macro which, if 
*                        used, MUST be declared following ALL other local variables.
*
*                        Example :
*
*                           void  Fnct (void)
*                           {
*                               CPU_INT08U  val_08;
*                               CPU_INT16U  val_16;
*                               CPU_INT32U  val_32;
*                               CPU_SR_ALLOC();         MUST be declared after ALL other local variables
*                                   :
*                                   :
*                           }
*
*               (b) Configure 'CPU_SR' data type with the appropriate-sized CPU data type large enough to
*                   completely store the CPU's/compiler's status word.
*********************************************************************************************************
*/
                                                                /* Configure CPU critical method      (see Note #1) :   */
#define  CPU_CFG_CRITICAL_METHOD    CPU_CRITICAL_METHOD_STATUS_LOCAL

typedef  CPU_INT32U                 CPU_SR;                     /* Defines   CPU status register size (see Note #3b).   */

                                                                /* Allocates CPU status register word (see Note #3a).   */
#if     (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
#define  CPU_SR_ALLOC()             CPU_SR  cpu_sr = (CPU_SR)0
#else
#define  CPU_SR_ALLOC()
#endif



#define  CPU_INT_DIS()         do { cpu_sr = __get_PRIMASK(); __disable_irq(); } while (0)
#define  CPU_INT_EN()          do { __set_PRIMASK(cpu_sr); } while (0) /* Restore CPU status word.                     */


#define  CPU_CRITICAL_ENTER()  do { CPU_INT_DIS(); } while (0)          /* Disable   interrupts.                        */
#define  CPU_CRITICAL_EXIT()   do { CPU_INT_EN();  } while (0)          /* Re-enable interrupts.                        */

/*
*********************************************************************************************************
*                                       CPU WORD CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_ADDR_SIZE & CPU_CFG_DATA_SIZE in 'cpu.h' with CPU's word sizes :
*
*                   CPU_WORD_SIZE_08             8-bit word size
*                   CPU_WORD_SIZE_16            16-bit word size
*                   CPU_WORD_SIZE_32            32-bit word size
*                   CPU_WORD_SIZE_64            64-bit word size
*
*           (2) Configure CPU_CFG_ENDIAN_TYPE in 'cpu.h' with CPU's data-word-memory order :
*
*               (a) CPU_ENDIAN_TYPE_BIG         Big-   endian word order (CPU words' most  significant
*                                                                         octet @ lowest memory address)
*               (b) CPU_ENDIAN_TYPE_LITTLE      Little-endian word order (CPU words' least significant
*                                                                         octet @ lowest memory address)
*********************************************************************************************************
*/

                                                        /* ---------------------- CPU WORD SIZE ----------------------- */
#define  CPU_WORD_SIZE_08                          1u   /*  8-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_16                          2u   /* 16-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_32                          4u   /* 32-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_64                          8u   /* 64-bit word size (in octets).                                */


                                                        /* ------------------ CPU WORD-ENDIAN ORDER ------------------- */
#define  CPU_ENDIAN_TYPE_NONE                      0u
#define  CPU_ENDIAN_TYPE_BIG                       1u   /* Big-   endian word order (see Note #1a).                     */
#define  CPU_ENDIAN_TYPE_LITTLE                    2u   /* Little-endian word order (see Note #1b).                     */


/*
*********************************************************************************************************
*                                       CPU WORD CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_ADDR_SIZE, CPU_CFG_DATA_SIZE, & CPU_CFG_DATA_SIZE_MAX with CPU's &/or
*               compiler's word sizes :
*
*                   CPU_WORD_SIZE_08             8-bit word size
*                   CPU_WORD_SIZE_16            16-bit word size
*                   CPU_WORD_SIZE_32            32-bit word size
*                   CPU_WORD_SIZE_64            64-bit word size
*
*           (2) Configure CPU_CFG_ENDIAN_TYPE with CPU's data-word-memory order :
*
*               (a) CPU_ENDIAN_TYPE_BIG         Big-   endian word order (CPU words' most  significant
*                                                                         octet @ lowest memory address)
*               (b) CPU_ENDIAN_TYPE_LITTLE      Little-endian word order (CPU words' least significant
*                                                                         octet @ lowest memory address)
*********************************************************************************************************
*/

                                                                /* Define  CPU         word sizes (see Note #1) :       */
#define  CPU_CFG_ADDR_SIZE              CPU_WORD_SIZE_32        /* Defines CPU address word size  (in octets).          */
#define  CPU_CFG_DATA_SIZE              CPU_WORD_SIZE_32        /* Defines CPU data    word size  (in octets).          */
#define  CPU_CFG_DATA_SIZE_MAX          CPU_WORD_SIZE_64        /* Defines CPU maximum word size  (in octets).          */

#define  CPU_CFG_ENDIAN_TYPE            CPU_ENDIAN_TYPE_LITTLE  /* Defines CPU data    word-memory order (see Note #2). */


/*
*********************************************************************************************************
*                                 CONFIGURE CPU ADDRESS & DATA TYPES
*********************************************************************************************************
*/

                                                                /* CPU address type based on address bus size.          */
#if     (CPU_CFG_ADDR_SIZE == CPU_WORD_SIZE_32)
typedef  CPU_INT32U  CPU_ADDR;
#elif   (CPU_CFG_ADDR_SIZE == CPU_WORD_SIZE_16)
typedef  CPU_INT16U  CPU_ADDR;
#else
typedef  CPU_INT08U  CPU_ADDR;
#endif

                                                                /* CPU data    type based on data    bus size.          */
#if     (CPU_CFG_DATA_SIZE == CPU_WORD_SIZE_32)
typedef  CPU_INT32U  CPU_DATA;
#elif   (CPU_CFG_DATA_SIZE == CPU_WORD_SIZE_16)
typedef  CPU_INT16U  CPU_DATA;
#else
typedef  CPU_INT08U  CPU_DATA;
#endif


typedef  CPU_DATA    CPU_ALIGN;                                 /* Defines CPU data-word-alignment size.                */
typedef  CPU_ADDR    CPU_SIZE_T;                                /* Defines CPU standard 'size_t'   size.                */


/*
*********************************************************************************************************
*                                             MODULE END
*
* Note(s) : (1) See 'cpu.h  MODULE'.
*********************************************************************************************************
*/

#ifdef __cplusplus
}
#endif

#endif                                                          /* End of CPU module include.                           */

