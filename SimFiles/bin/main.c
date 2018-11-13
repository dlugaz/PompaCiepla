//======================================================
// uich application
//======================================================

#define IO_AIFILTER_ACTIVE 1        // 1=no filter, 2=50% filter, 8=default MCX06, 16=ok MCX08

#define nvmcc_ApplicationID 9001    // SWACH v1.00

#include "AGF_Digital_Input.c"
#include "AGF_Digital_Output.c"
#include "AGF_Analog_Input.c"
#include "AGF_Analog_Output.c"
#define MAX_AINUM       NUMBER_AI
#define MAX_AONUM       NUMBER_AO
#define MAX_DINUM       NUMBER_DI
#define MAX_DONUM       NUMBER_DO


#include "swa_ch.lib"
#include "nvmlib/math.lib"
#include "version.lib"


TChillerApp MyChiller;

//**************  RHP - RESIDENTIAL HEAT PUMP MODULES  ************************
#ifdef HANDLE_RHP
TDHWgen DHWgen;
TRoomCircuit RoomCircuit[RHP_MAX_CIRCUIT];
short RHPStatus;
persistent short Prev_Circ_Man_Set;
persistent short Prev_DHW_Man_Set;
#endif

#ifdef SUPPORT_BUFFER_TANK
#include "TBufferTanklib.lib"
TBufferTankManager BufferTankManager;
#endif //#ifdef SUPPORT_BUFFER_TANK

short MainLoopValue;
void App_Init() {


#ifdef SCHEDULER_ENABLE
    // initialize the scheduler
    Scheduler.Init();
#endif

    // init default parameters
    Params.InitParams();

    // initialize parameters array every time a new application is loaded into device
    if (ParVersion != nvmcc_CompileDate) {
        InitializeParamVal();

#ifdef SCHEDULER_ENABLE
        // clear all scheduled actions
        Scheduler.Clear();

        // set some default schedule (action is implicit in the schedule index array)
        T7dScheduledAction schedule;
        // default RHP sheduler
        /**
        Set schedule fields.
        @param iWeekDays is a bitmapped integer, where each bit is 1 to enable one week day. Bit(0)=Mon, Bit(6)=Sun.
        @param iHour is the hour when the action must be performed. -1=any hour.
        @param iMin is the minute when the action must be performed. Minutes will be truncated to 10 minutes (e.g. 17 become 10).
        @param iMonth is the month when the action must be performed. -1=any month.
        @param iDay is the day when the action must be performed. -1=any day. Depending on the month, it can go from 1 up to 31. 32 means last day,
                33 means last 7 days, 34 means first 7 days. Combining 33 and 34 with a weekday you can select last (or first) weekday of the month.
        @param iAction is the action to be performed.
        void SetSchedule(char iHour, char iMin, char iMonth, char iDay, char iWeekDays, byte iAction = 0)  {
        */
        // default of circuit 1
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 1);
        ScheduledAction[0] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 1);
        ScheduledAction[1] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 1);
        ScheduledAction[2] = schedule.Pack();
         // default of circuit 2
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 2);
        ScheduledAction[0] = schedule.Pack();
         // default of circuit 3
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 3);
        ScheduledAction[0] = schedule.Pack();
         // default of dhw
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 3 , 4);
        ScheduledAction[0] = schedule.Pack();

        // default of circuit 2
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 2);
        ScheduledAction[3] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 2);
        ScheduledAction[4] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 2);
        ScheduledAction[5] = schedule.Pack();
        // default of circuit 3
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 3);
        ScheduledAction[6] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 3);
        ScheduledAction[7] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 3);
        ScheduledAction[8] = schedule.Pack();
#ifdef Params_RHP_Circuit_6_Description
        // default of circuit 4
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 4);
        ScheduledAction[9] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 4);
        ScheduledAction[10] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 4);
        ScheduledAction[11] = schedule.Pack();
        // default of circuit 5
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 5);
        ScheduledAction[12] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 5);
        ScheduledAction[13] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 5);
        ScheduledAction[14] = schedule.Pack();
        // default of circuit 6
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 6);
        ScheduledAction[15] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 6);
        ScheduledAction[16] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 6);
        ScheduledAction[17] = schedule.Pack();
#ifdef Params_RHP_Circuit_10_Description
        // default of circuit 7
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 7);
        ScheduledAction[18] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 7);
        ScheduledAction[19] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 7);
        ScheduledAction[20] = schedule.Pack();
        // default of circuit 8
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 8);
        ScheduledAction[21] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 8);
        ScheduledAction[22] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 8);
        ScheduledAction[23] = schedule.Pack();
        // default of circuit 9
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 9);
        ScheduledAction[24] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 9);
        ScheduledAction[25] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 9);
        ScheduledAction[26] = schedule.Pack();
        // default of circuit 10
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 10);
        ScheduledAction[27] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 10);
        ScheduledAction[28] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 10);
        ScheduledAction[29] = schedule.Pack();
        // default of circuit 11 (DHW)
        schedule.SetSchedule(0, 0, -1, -1,      0b01111111, 2 , 11);
        ScheduledAction[30] = schedule.Pack();
        schedule.SetSchedule(6, 30, -1, -1,     0b01111111, 3 , 11);
        ScheduledAction[31] = schedule.Pack();
        schedule.SetSchedule(20, 30, -1, -1,    0b01111111, 2 , 11);
        ScheduledAction[32] = schedule.Pack();
#endif // #ifdef Params_RHP_Circuit_10_Description
#endif // #ifdef Params_RHP_Circuit_6_Description
#endif // #ifdef SCHEDULER_ENABLE

        // inizialize default mode of unit to AUTO
        Prev_Circ_Man_Set = UNI_AUT;

        // inizialize default mode of unit to AUTO
        Prev_DHW_Man_Set = DHW_AUT;

        char c;
        for (c=0 ; c < ASize( ComprSecondsCounter ) ; c++ ) {
        	ComprSecondsCounter[c] = 0;
        }

        for (c=0 ; c < ASize( SinkPumpSecCounter ) ; c++ ) {
        	SinkPumpSecCounter[c] = 0;
        }

        for (c=0 ; c < ASize( SourcePumpSecCounter ) ; c++ ){
        	SourcePumpSecCounter[c] = 0;
        }

        // store nvmcc_CompileDate
        ParVersion = nvmcc_CompileDate;
    }

    // if device does not have user interface, disable buzzer
    if (!dsp.HaveDisplay()) {
        SetBuzzerVolume(0);
    };

    MyChiller.Init();

    DHWgen.Init();
    int i;
    for ( i=0; i < RHP_MAX_CIRCUIT; i++ ) {
        RoomCircuit[i].ID = (i);
        RoomCircuit[i].Init();
    };

    Exp_RunningIntoSimulation = RunningIntoSimulation;
};


// this is the control loop procedure, and will be called continuously
void App_Run() {
    char i;
    RTC_Run();
#ifdef ALARM_HISTORY_ENABLE
    EventHistoryLog.Init(RTC);
#endif

    Can.SetSuspend(0);              // Unlock Object Dictionary

    DHWgen.Run();                   // DHW Preparation

    #ifdef DO_Main_Circuit_Pump_ID
    char CircuitIsRun;
    CircuitIsRun = 0;
    #endif

    for (i=0 ; i< RHP_MAX_CIRCUIT ; i++ ) {
        RoomCircuit[i].SysPumpStatus = ch.SinkPump.isOn();
        RoomCircuit[i].Run();       // Rooms Circuits
        #ifdef DO_Main_Circuit_Pump_ID
        CircuitIsRun = CircuitIsRun  == 1 || RoomCircuit[i].PumpStatus  == 1;
        #endif
    };

    #ifdef DO_Main_Circuit_Pump_ID
    DO_Main_Circuit_Pump = CircuitIsRun;
    #endif


    #ifdef SUPPORT_BUFFER_TANK
    BufferTankManager.Run( AI_Temp_BT_1_ID ,DO_ChargeValve_BT1_ID, DO_DischargeValve_BT1_ID,  ToShort( ch.Setpoint.Setp ),RTC );
    //BufferTankManager.Run( AI_Temp_BT_1_ID ,DO_ChargeValve_BT1_ID, DO_DischargeValve_BT1_ID,  ToShort( ch.Treg ) );
    #endif //#ifdef SUPPORT_BUFFER_TANK

    MyChiller.Run();

    MainLoopValue = baseTimer.Elapsedms;
    baseTimer.Start();

    // enable watchdog after 20 sec.
    SetWatchdog(20);
};

