/******************************************************************************
  Filename:     OSAL_Debug.h
  Revised:      $Date: 2014-06-30 16:38:56 -0700 (Mon, 30 Jun 2014) $
  Revision:     $Revision: 39297 $

  Description:  Debug Macro
******************************************************************************/

#ifndef _OSAL_DEBUG_H
#define _OSAL_DEBUG_H

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//Trace level definitions
#define TRACE_LEVEL_OFF      0
#define TRACE_LEVEL_FATAL    1
#define TRACE_LEVEL_ERROR    2
#define TRACE_LEVEL_WARNING  3
#define TRACE_LEVEL_INFO     4
#define TRACE_LEVEL_DEBUG    5

//Default trace level
#ifndef TRACE_LEVEL
   #define TRACE_LEVEL TRACE_LEVEL_INFO
#endif

//Trace output redirection
#ifndef TRACE_PRINTF
   #define TRACE_PRINTF(...) printf(__VA_ARGS__)
#endif

//Debugging macros
#if (TRACE_LEVEL >= TRACE_LEVEL_FATAL)
   #define TRACE_FATAL(...) TRACE_PRINTF("[FATAL ] "), TRACE_PRINTF(__VA_ARGS__)
#else
   #define TRACE_FATAL(...)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_ERROR)
   #define TRACE_ERROR(...) TRACE_PRINTF("[ERR  ] "), TRACE_PRINTF(__VA_ARGS__)
#else
   #define TRACE_ERROR(...)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_WARNING)
   #define TRACE_WARNING(...) TRACE_PRINTF("[WARN ] "), TRACE_PRINTF(__VA_ARGS__)
#else
   #define TRACE_WARNING(...)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_INFO)
   #define TRACE_INFO(...) TRACE_PRINTF("[INFO ] "),TRACE_PRINTF(__VA_ARGS__)
#else
   #define TRACE_INFO(...)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_DEBUG)
   #define TRACE_DEBUG(...) TRACE_PRINTF("[DEBUG  ] "), TRACE_PRINTF(__VA_ARGS__)
#else
   #define TRACE_DEBUG(...)
#endif

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
