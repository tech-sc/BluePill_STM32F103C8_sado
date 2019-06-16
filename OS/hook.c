#include "os.h"

/************************************************************************************//**
 * @brief   Tick hook function
 * @param   None
 * @retval  None
 * @note    Maybe this function will be called from the Tick handler.
 */
#if( configUSE_TICK_HOOK ==1 )
void vApplicationTickHook( void )
{
}
#endif

/************************************************************************************//**
 * @brief   Tick hook function
 * @param   None
 * @retval  None
 * @note    Maybe this function will be called from the Tick handler.
 */
#if( ( configCHECK_FOR_STACK_OVERFLOW > 0 ) && ( portSTACK_GROWTH < 0 ) )
void vApplicationStackOverflowHook( xTaskHandle xTask, signed char *pcTaskName )
{
}
#endif
