

#define APPSWDESCR "ZaTeChS Heat Pump"                 /* application description */
#define APPSWVER  120                       /* application version (2 decimals) */



#define MENU_INACTIVITY_TIMEOUT 20*60   /* inactivity timeout (sec) */

#define PASSWORD_MASK
#define PASSWORD_3DIGIT     // use 3-digit password


#ifdef DrawValueCR_SINGLELINE
#else
    #define DrawValueCR_SINGLELINE      /* DrawValueCR does not work on multiple lines, but it will be faster    */
#endif

#define IOValueBaseIndexCAN     1000    // base address for exported I/O (max 512)
#define PackedAlarmsBaseIndexCAN 1900   // base address for exported packed alarms (max 1000)
#define AlarmsBaseIndexCAN      2000    // base address for exported alarms (max 512-2)
#define ParamsBaseIndexCAN      3000    // base address for exported parameters (max 512)
#define VIOBaseIndexCAN         4000    // base address for exported Virtual I/O (max 1024)
#define MyVariablesCAN         5000     // base address for additional variables

#define UIAlarmNotificationID   AlarmsBaseIndexCAN
#define UIAlarmResetID          (AlarmsBaseIndexCAN+1)
#define AlarmsExBaseIndexCAN	(AlarmsBaseIndexCAN+2)
#define ParamsChangedFlagID     (ParamsBaseIndexCAN-1)
#define VDIBaseIndexCAN         (VIOBaseIndexCAN+0)
#define VDOBaseIndexCAN         (VIOBaseIndexCAN+200)
#define VAIBaseIndexCAN         (VIOBaseIndexCAN+400)
#define VAOBaseIndexCAN         (VIOBaseIndexCAN+600)

#define CountValue (MyVariablesCAN +1)
#define MySchedules (MyVariablesCAN+50)
#define Inverter_Power (MyVariablesCAN+25)

#define ON 1
#define OFF 0
#define CO 0
#define CWU 1
#define true 1
#define false 0
#define LongSchedules 9

#define CAN_WRITE_WAIT_TIME   500

#define FULLALCODE
#define SUPPORT_PACKED_ALARMS

#include "AGF_Define.c"

#define Params_AL_NextFree_ID   ALARM_ARRAY_SIZE

#define ParamsVal ParamVal  // to be compatible with some libraries modules

#define SUPPORT_RTC

#define EVENT_HISTORY_MAX_EVENTS  20
#define EVENT_HISTORY_SAVE_PARAMS_CHANGE

//#define T7dScheduledAction_MaxActions 10
#define T7DSCHEDULER_ENHANCED   // Enable day of month selection

//#define MULTILANGUAGE 3     // enable multilanguage support. define to the total number of languages supported (including default one)

#ifdef MULTILANGUAGE
    #ifdef STRINGTABLE
    #else
        #define STRINGTABLE
    #endif

    //#define DOUBLEHEIGHTFONT    (2 * (GetUILanguage() == 2))    /* support double height font in menu and set correct font */
#endif

//////////////////////////////////////////////////////////////
// My declarations
/////////////////////////////////////////////////////////////
#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))
#define MAXSCHEDULEPOINTS 100

    /*kazdy punkt ma taka strukture
    1. time 8 bits
    2. day 3 bits
    3. on/off 1 bit
    */
    /*//Writing point
    int time; char day; char on;
    time=143; day=6; on=0;
    SchedulePoints=time|day<<8|on<<11;
    //Reading point
    time = SchedulePoints&0xff;
    day = (SchedulePoints>>8)&127;
    on = (SchedulePoints>>15)&1;
   00010000 0 0010001 00010001
   | |       |        |
   ON   DAY     TIME    */
 struct SchedulePoint
 {
        long point;
        void WritePoint(unsigned char time,unsigned char day,unsigned char on,unsigned char schedule)
        {point=time|day<<8|on<<15|schedule<<16;};
        int ReadTime(){return point&0xff;};
        int ReadDay(){return (point>>8)&0x7f;};
        int ReadAction(){return (point>>15)&1;};
        int ReadScheduleNo(){return (point>>16&0xff)}
        int CheckDay(char cday)
        {return ReadDay()&(1<<cday);};
};
