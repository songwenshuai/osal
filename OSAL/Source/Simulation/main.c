#include "OSAL.h"

extern void SysTickSetup(void);

/**
 * @brief main
 **/

int main(int argc, char **argv)
{
    // Turn off interrupts
    osal_int_disable(INTS_ALL);

    // Initialize the operating system
    osal_init_system();

    // Allow interrupts
    osal_int_enable(INTS_ALL);

    // Tick Init
    SysTickSetup();

    // No Return from here
    osal_start_system();

    return 0;
}
