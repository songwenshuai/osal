
#include "clk.h"

void task_rtc(void)
{
    CLK_DATE_TIME date_time;
    CLK_DATE_TIME date_time2;
	CPU_BOOLEAN   valid;
	CLK_TZ_SEC    tz_sec;
	CLK_TS_SEC    ts_sec;
	CPU_CHAR      str[128];
	CLK_TS_SEC    ts_unix_sec;
	
	tz_sec = 0;
	
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
		printf("Unix timestamp = %s", str);
	} else {
		printf("Clock date/time to string failed\r\n");
	
	}
	
	Clk_GetDateTime(&date_time);
	printf("seconds is %d\n", date_time.Sec);
	valid = Clk_DateTimeToStr(&date_time, CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS,str, 128);
	
	if (valid == DEF_OK) {
		printf("Current Date/time :%s\n\r", str);
	} else {
		printf("Clock date/time to string failed\r\n");
	
	}
}
