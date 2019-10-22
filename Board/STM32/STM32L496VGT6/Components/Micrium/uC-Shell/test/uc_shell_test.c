#include <shell.h>
#include <sh_shell.h>
#include <terminal.h>



/*
*********************************************************************************************************
*                                          UCOS Shell Init
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static CPU_BOOLEAN UCOS_Shell_Init (void)
{
    CPU_BOOLEAN err_shell, terminal;

    printf(("Initializing uC/Shell.\r\n"));

    err_shell = Shell_Init();

    if (err_shell != DEF_OK) {
        printf(("Error initializing uC/Shell.\r\n"));
        return (DEF_FAIL);
    }
    err_shell = ShShell_Init();
    if (err_shell != DEF_OK) {
        printf(("Error initializing uC/Shell.\r\n"));
        return (DEF_FAIL);
    }

    terminal = Terminal_Init();

    if (terminal != DEF_OK) {
        printf(("Error initializing uC/Terminal.\r\n"));
        return (DEF_FAIL);
    }

    return (DEF_OK);
}
