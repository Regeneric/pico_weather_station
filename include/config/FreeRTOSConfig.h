#pragma once
#include <stdint.h>

/* Scheduler Related */
#define configUSE_PREEMPTION                            1
#if HPLATFORM_ARM
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION     0
#else
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION     1
#endif
#define configUSE_TICKLESS_IDLE                         0
#define configUSE_IDLE_HOOK                             0   // May be needed later
#define configUSE_TICK_HOOK                             0
#define configTICK_RATE_HZ                              ((TickType_t)1000)
#define configMAX_PRIORITIES                            32
#define configMINIMAL_STACK_SIZE                        (configSTACK_DEPTH_TYPE)128 
#define configUSE_16_BIT_TICKS                          0
#define configMAX_TASK_NAME_LEN                         32
#define configUSE_TASK_NOTIFICATIONS                    1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES           1   // May be changed later to ~5
#define configIDLE_SHOULD_YIELD                         1
#define configUSE_SB_COMPLETED_CALLBACK                 1

/* Synchronization Related */
#define configUSE_MUTEXES                               1
#define configUSE_RECURSIVE_MUTEXES                     1
#define configUSE_APPLICATION_TASK_TAG                  0   // May be needed later for debug
#define configUSE_COUNTING_SEMAPHORES                   1
#define configQUEUE_REGISTRY_SIZE                       8   // For debugging
#define configUSE_QUEUE_SETS                            1
#define configUSE_TIME_SLICING                          1
#define configUSE_NEWLIB_REENTRANT                      0

//  Need this for lwip FreeRTOS sys_arch to compile
#define configENABLE_BACKWARD_COMPATIBILITY             1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS         5
#define configUSE_ALTERNATIVE_API                       0
#define configUSE_MINI_LIST_ITEM                        1

/* System */
#define configSTACK_DEPTH_TYPE                          uint32_t
#define configMESSAGE_BUFFER_LENGTH_TYPE                size_t

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION                 0
#define configSUPPORT_DYNAMIC_ALLOCATION                1
#define configTOTAL_HEAP_SIZE                           (128*1024)  // May be changed later
#define configAPPLICATION_ALLOCATED_HEAP                0

/* Hook function related definitions. */
#define configCHECK_FOR_STACK_OVERFLOW                  0   // Will be needed later
#define configUSE_MALLOC_FAILED_HOOK                    1   // Will be needed later
#define configUSE_DAEMON_TASK_STARTUP_HOOK              0   // May be needed later

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS                   0   // WILL BE NEEDED LATER FOR DEBUG
#define configUSE_TRACE_FACILITY                        0   // WILL BE NEEDED LATER FOR DEBUG
#define configUSE_STATS_FORMATTING_FUNCTIONS            0   // WILL BE NEEDED LATER FOR DEBUG

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                           0
#define configMAX_CO_ROUTINE_PRIORITIES                 1

/* Software timer related definitions. */
#define configUSE_TIMERS                                1   // May be disabled later
#define configTIMER_TASK_PRIORITY                       (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH                        10
#define configTIMER_TASK_STACK_DEPTH                    1024

/* Interrupt nesting behaviour configuration. */
/*
#define configKERNEL_INTERRUPT_PRIORITY                 [dependent of processor]
#define configMAX_SYSCALL_INTERRUPT_PRIORITY            [dependent on processor and application]
#define configMAX_API_CALL_INTERRUPT_PRIORITY           [dependent on processor and application]
*/

#if FREE_RTOS_KERNEL_SMP 
/* SMP port only */
    #ifndef configNUMBER_OF_CORES
        #define configNUMBER_OF_CORES                   2
    #endif
    #define configNUM_CORES                             configNUMBER_OF_CORES
    #define configTICK_CORE                             0   // CPU core to run system ticks   
    #define configRUN_MULTIPLE_PRIORITIES               1
    #if configNUMBER_OF_CORES > 1
        #define configUSE_CORE_AFFINITY                 1
    #endif
    #define configUSE_PASSIVE_IDLE_HOOK                 0
#endif

/* RP2040 specific */
#define configSUPPORT_PICO_SYNC_INTEROP                 1
#define configSUPPORT_PICO_TIME_INTEROP                 1

#include <assert.h>
/* Define to trap errors during development. */
#define configASSERT(x)                                 assert(x)

#define vPortSVCHandler                                 isr_svcall
#define xPortPendSVHandler                              isr_pendsv
#define xPortSysTickHandler                             isr_systick

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                        1
#define INCLUDE_uxTaskPriorityGet                       1
#define INCLUDE_vTaskDelete                             1
#define INCLUDE_vTaskSuspend                            1
#define INCLUDE_vTaskDelayUntil                         1
#define INCLUDE_vTaskDelay                              1
#define INCLUDE_xTaskGetSchedulerState                  1
#define INCLUDE_xTaskGetCurrentTaskHandle               1
#define INCLUDE_uxTaskGetStackHighWaterMark             1
#define INCLUDE_xTaskGetIdleTaskHandle                  1
#define INCLUDE_pxTaskGetStackStart                     1
#define INCLUDE_eTaskGetState                           1
#define INCLUDE_xTimerPendFunctionCall                  1
#define INCLUDE_xTaskAbortDelay                         1
#define INCLUDE_xTaskGetHandle                          1
#define INCLUDE_xTaskResumeFromISR                      1
#define INCLUDE_xQueueGetMutexHolder                    1

#if PICO_RP2350
    #define configCPU_CLOCK_HZ                          150000000
    #define configENABLE_MPU                            0
    #define configENABLE_TRUSTZONE                      0
    #define configRUN_FREERTOS_SECURE_ONLY              1
    #define configENABLE_FPU                            1
    #define configMAX_SYSCALL_INTERRUPT_PRIORITY        16
#else
    #define configCPU_CLOCK_HZ                          133000000
#endif

#include <SEGGER_SYSVIEW_FreeRTOS.h>