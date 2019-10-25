
#include "clk.h"

static void Clock_Test(void);

#define SBP_CLOCK_EVT         0x0008  
#define SBP_CLOCK_EVT_DELAY     1000


if (events & SBP_CLOCK_EVT)
{
    // Restart timer
    if ( SBP_CLOCK_EVT_DELAY )
    {
        osal_start_timerEx(App_TaskID, SBP_CLOCK_EVT, SBP_CLOCK_EVT_DELAY);
    }
    
    // Perform clock task
    Clk_TaskHandler();

    return (events ^ SBP_CLOCK_EVT);
}


void uc_clk_test( void ) {
    CLK_ERR err;

    Clk_Init(&err);
    
    /* initialize EasyFlash and EasyLogger */
    if ( err == CLK_ERR_NONE) {
        Clock_Test();
    }
}

/*********************************************************************
 * @fn      Clock_Test
 *
 * @brief   void
 *
 * @param   void
 *
 * @return  none
 */
static void Clock_Test(void)
{
    static CLK_DATE_TIME   date_time;
	static   CPU_BOOLEAN       valid;
	static    CLK_TZ_SEC      tz_sec;
	static    CLK_TS_SEC      ts_sec;
	static      CPU_CHAR     str[128];
	static    CLK_TS_SEC  ts_unix_sec;
    static      CPU_CHAR     CLK_FLAG = 0;

	tz_sec = 0;
	
    if(CLK_FLAG == 0){
    	valid = Clk_DateTimeMake(&date_time, 2013, 8, 11, 01, 11, 11, tz_sec);
    	if (valid != DEF_OK) {
    		printf("Clock make date/time failed\r\n");
    	}

    	valid = Clk_SetDateTime(&date_time);
    
    	if (valid != DEF_OK) {
    		printf("Clock set date/time failed\r\n");
    	}

    	Clk_DateTimeToTS(&ts_sec, &date_time);
    
    	if (valid == DEF_OK) {
    		printf("Clock timestamp = %u\r\n", ts_sec);
    	} else {
    		printf("Clock date/time to timestamp failed\r\n");
    	}

    	tz_sec = (-5 * 60 * 60);
    	valid = Clk_SetTZ(tz_sec);
    
    	if (valid != DEF_OK) {
        
    		printf("Clock set timezone unix failed\r\n");
    		return;
    	}

    	ts_sec = Clk_GetTS(); 
    
    	valid = Clk_TS_ToDateTime(ts_sec, 0, &date_time); 
    	if (valid != DEF_OK) {
    		printf("Clock convert timestamp to date/time failed\r\n");
    
    	}
    	valid = Clk_GetTS_Unix(&ts_unix_sec); 
    
    	if (valid != DEF_OK) {
    		printf("Clock get timestamp unix failed\r\n");
    
    	}
    
    	valid = Clk_TS_UnixToDateTime(ts_unix_sec, tz_sec, &date_time); 
    	if (valid != DEF_OK) {
    		printf("Clock timestamp unix to date/time failed\r\n");
    
    	}
    
    	valid = Clk_DateTimeToStr(&date_time, CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS, str, 128);
    	if (valid == DEF_OK) {
    		printf("Current Date/time :%s\n\r", str);
    	} else {
    		printf("Clock date/time to string failed\r\n");

    	}
    	ts_unix_sec = 126316799uL;
    
    	valid = Clk_TS_UnixToDateTime(ts_unix_sec, tz_sec, &date_time);
    	if (valid != DEF_OK) {
    		printf("Clock set date/time failed\r\n");
    
    	}

    	valid = Clk_DateTimeToStr(&date_time, CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY, str, 128);
    	if (valid == DEF_OK) {
    		printf("Unix timestamp = %s\r\n", str);
    	} else {
    		printf("Clock date/time to string failed\r\n");
    
    	}
        CLK_FLAG = 1;
	}
	Clk_GetDateTime(&date_time);

	valid = Clk_DateTimeToStr(&date_time, CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS,str, 128);
	
	if (valid == DEF_OK) {
		printf("Current Date/time :%s\n\r", str);
	} else {
		printf("Clock date/time to string failed\r\n");
	}
}

// To be used with
typedef struct
{
  uint8 seconds;  // 0-59
  uint8 minutes;  // 0-59
  uint8 hour;     // 0-23
  uint8 day;      // 0-30
  uint8 month;    // 0-11
  uint16 year;    // 2000+
} UTCTimeStruct;

#define IsLeapYear(yr)  (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))
#define    BEGYEAR  2000     //  UTC started at 00:00:00 January 1, 2000

#define    DAY      86400UL  // 24 hours * 60 minutes * 60 seconds
                                  
#define    YearLength(yr)    ((uint16)(IsLeapYear(yr) ? 366 : 365))

// number of seconds since 0 hrs, 0 minutes, 0 seconds, on the
// 1st of January 2000 UTC
typedef uint32 CLK_TS_SEC;

static uint8 monthLength( uint8 lpyr, uint8 mon );


  /*
   * Converts CLK_TS_SEC to UTCTimeStruct
   *
   * secTime - number of seconds since 0 hrs, 0 minutes,
   *          0 seconds, on the 1st of January 2000 UTC
   * tm - pointer to breakdown struct
   */
  extern void osal_ConvertUTCTime( UTCTimeStruct *tm, CLK_TS_SEC secTime );

  /*
   * Converts UTCTimeStruct to CLK_TS_SEC (seconds since 00:00:00 01/01/2000)
   *
   * tm - pointer to UTC time struct
   */
  extern CLK_TS_SEC osal_ConvertUTCSecs( UTCTimeStruct *tm );

/*********************************************************************
 * @fn      osal_ConvertUTCTime
 *
 * @brief   Converts CLK_TS_SEC to UTCTimeStruct
 *
 * @param   tm - pointer to breakdown struct
 *
 * @param   secTime - number of seconds since 0 hrs, 0 minutes,
 *          0 seconds, on the 1st of January 2000 UTC
 *
 * @return  none
 */
void osal_ConvertUTCTime( UTCTimeStruct *tm, CLK_TS_SEC secTime )
{
  // calculate the time less than a day - hours, minutes, seconds
  {
    uint32 day = secTime % DAY;
    tm->seconds = day % 60UL;
    tm->minutes = (uint8)((day % 3600UL) / 60UL);
    tm->hour = (uint8)(day / 3600UL);
  }

  // Fill in the calendar - day, month, year
  {
    uint16 numDays = (uint16)(secTime / DAY);
    tm->year = BEGYEAR;
    while ( numDays >= YearLength( tm->year ) )
    {
      numDays -= YearLength( tm->year );
      tm->year++;
    }

    tm->month = 0;
    while ( numDays >= monthLength( IsLeapYear( tm->year ), tm->month ) )
    {
      numDays -= monthLength( IsLeapYear( tm->year ), tm->month );
      tm->month++;
    }

    tm->day = (uint8)numDays;
  }
}

/*********************************************************************
 * @fn      monthLength
 *
 * @param   lpyr - 1 for leap year, 0 if not
 *
 * @param   mon - 0 - 11 (jan - dec)
 *
 * @return  number of days in specified month
 */
static uint8 monthLength( uint8 lpyr, uint8 mon )
{
  uint8 days = 31;

  if ( mon == 1 ) // feb
  {
    days = ( 28 + lpyr );
  }
  else
  {
    if ( mon > 6 ) // aug-dec
    {
      mon--;
    }

    if ( mon & 1 )
    {
      days = 30;
    }
  }

  return ( days );
}

/*********************************************************************
 * @fn      osal_ConvertUTCSecs
 *
 * @brief   Converts a UTCTimeStruct to CLK_TS_SEC
 *
 * @param   tm - pointer to provided struct
 *
 * @return  number of seconds since 00:00:00 on 01/01/2000 (UTC)
 */
CLK_TS_SEC osal_ConvertUTCSecs( UTCTimeStruct *tm )
{
  uint32 seconds;

  /* Seconds for the partial day */
  seconds = (((tm->hour * 60UL) + tm->minutes) * 60UL) + tm->seconds;

  /* Account for previous complete days */
  {
    /* Start with complete days in current month */
    uint16 days = tm->day;

    /* Next, complete months in current year */
    {
      int8 month = tm->month;
      while ( --month >= 0 )
      {
        days += monthLength( IsLeapYear( tm->year ), month );
      }
    }

    /* Next, complete years before current year */
    {
      uint16 year = tm->year;
      while ( --year >= BEGYEAR )
      {
        days += YearLength( year );
      }
    }

    /* Add total seconds before partial day */
    seconds += (days * DAY);
  }

  return ( seconds );
}

  /*
   * Set the new time.  This will only set the seconds portion
   * of time and doesn't change the factional second counter.
   *     newTime - number of seconds since 0 hrs, 0 minutes,
   *               0 seconds, on the 1st of January 2000 UTC
   */
  extern void osal_setClock( CLK_TS_SEC newTime );

  /*
   * Gets the current time.  This will only return the seconds
   * portion of time and doesn't include the factional second counter.
   *     returns: number of seconds since 0 hrs, 0 minutes,
   *              0 seconds, on the 1st of January 2000 UTC
   */
  extern CLK_TS_SEC osal_getClock( void );

/*********************************************************************
 * @fn      osal_setClock
 *
 * @brief   Set the new time.  This will only set the seconds portion
 *          of time and doesn't change the factional second counter.
 *
 * @param   newTime - number of seconds since 0 hrs, 0 minutes,
 *                    0 seconds, on the 1st of January 2000 UTC
 *
 * @return  none
 */
void osal_setClock( CLK_TS_SEC newTime )
{
  HAL_CRITICAL_STATEMENT(Clk_TS_UTC_sec = newTime);
}

/*********************************************************************
 * @fn      osal_getClock
 *
 * @brief   Gets the current time.  This will only return the seconds
 *          portion of time and doesn't include the factional second
 *          counter.
 *
 * @param   none
 *
 * @return  number of seconds since 0 hrs, 0 minutes, 0 seconds,
 *          on the 1st of January 2000 UTC
 */
CLK_TS_SEC osal_getClock( void )
{
  return ( Clk_TS_UTC_sec );
}
