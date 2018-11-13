/** \file

    Standard application framework


 \par Company:
 ZaTeChS S.C.

 \author Krzysztof D³u¿yñski

 ******************************************************************************

    This algo framework have the following features:

    - Application information screen (also shown at startup)
    - Parameters editing
    - Password login
    - IO display
    - Alarms management

 ******************************************************************************
*/


#include "AGF_Define.c"
#include "AppDefine.c"
#include "AGF_Params.c"

#ifdef Params_Al_Relay_Delay
#else
    #define Params_Al_Relay_Delay 0
#endif

#ifdef Params_Al_Buzz
#else
    #define Params_Al_Buzz 1
#endif

#include "UserInterface.lib"
#include "AGF_Alarm.c"
#include "alarms.lib"
#include "TIOValue.lib"


#ifdef EVENT_HISTORY_MAX_EVENTS
    #ifdef SUPPORT_RTC
    #else
        #define SUPPORT_RTC
    #endif

    #include "TEventHistoryLog.lib"

    bit UIResetHistory;
    modbusdecl(ModbusTable, modbusRW, EVENT_HISTORY_EXPORT_BASE-1, UIResetHistory);
#endif

#ifdef T7dScheduledAction_MaxActions
    #ifdef SUPPORT_RTC
    #else
        #define SUPPORT_RTC
    #endif

    #include "Scheduler7d.lib"

    modbusdecl(ModbusTable, modbusRW, SchedulerEntries, ScheduledAction);

    // define a storage to keep the schedules
    persistent long ScheduledAction[T7dScheduledAction_MaxActions];

    // declare an instance of the scheduler
    T7dScheduler Scheduler;
#endif


#ifdef SUPPORT_RTC
    #include "RTC_UI.lib"
#endif



persistent long  ParVersion;    // this will keep the date of the application, to make sure parameters are initialized
persistent short ParamVal[PARAMS_NUMParams];

bit UIParamsChanged;    // 1=eeprom parameters have been changed

#ifdef Sys_NodeID_ID
    short LastNodeID;
#endif


modbusdecl(ModbusTable, modbusRW, ParamsChangedFlagID, UIParamsChanged);


// export I/O manager virtual functions
modbusdecl(ModbusTable, modbusRW + modbusArray, VDIBaseIndexCAN, VDIValue);
modbusdecl(ModbusTable, modbusRW + modbusArray, VDOBaseIndexCAN, VDOValue);
modbusdecl(ModbusTable, modbusRW + modbusArray, VAIBaseIndexCAN, VAIValue);
modbusdecl(ModbusTable, modbusRW + modbusArray, VAOBaseIndexCAN, VAOValue);



// this function will initialize parameters array
void InitializeParamVal()  {
    long i;

    for(; i<ASize(ParamField); i++)  {
        // do not reset the Params_Sys_Default flag until operation have been completed
        if (ParamField[i].VarPtr != Sys_Default_ID)
            ParamVal[ ParamField[i].VarPtr ] = ParamField[i].Default;
    };

    UIParamsChanged = 1;

    #ifdef Sys_NodeID_ID
        // keep CANopen node ID when resetting default parameters
        Params_Sys_NodeID = Can.NodeID;
    #endif

}




void IOManager_Init()  {
};

void IOManager_Run()  {
    long tmp;
    long i;
    char typ;

    // read analog inputs
    for (; i<MAX_AINUM; i++)  {
        typ = IO.GetAIType(ToChar(i+1));
        if (typ == ioTypeDIGIN)  {
            tmp = IO.GetFilteredAI(ToChar(i+1));
        }
        else if (typ == ioTypeNTC || typ == ioTypePT1000)  {
            tmp = IO.GetFilteredAI(ToChar(i+1));
        }
        else  {
            tmp = IO.GetAI(ToChar(i+1));

            long TempDelta;

            TempDelta = AIDefault[i].PercentVAI;

            #ifdef ACTUALIOINSIMULATION
            #else
            if (!RunningIntoSimulation)
            #endif

            tmp = AIDefault[i].IndexVAIMIN + ((tmp - TempDelta) * (AIDefault[i].IndexVAIMAX-AIDefault[i].IndexVAIMIN))/(1000 - 2 * TempDelta);
        };

        #ifdef AIEn1_ID
            // enable read of AI from phisical input. When disabled, you can use AICal to manually set input value.
            if (ParamVal[AIEn1_ID + i] == 0)
                tmp = 0;
        #endif

        #ifdef AICal1_ID
            // add calibration
            tmp = tmp + ParamVal[AICal1_ID + i];

            // if input is configured as digital input, make sure we only return 0 or 1.
            // in this condition, calibration can be 0 or 1, if 1 it will reverse the input logic.
            if (typ == ioTypeDIGIN)
                tmp = tmp & 0x01;
        #endif

        // check for errors
        if (AIDefault[i].ErrorOnVAI)  {
            if (tmp < AIDefault[i].IndexVAIMIN || tmp > AIDefault[i].IndexVAIMAX)
                tmp = IO_NAValue;
        };

        AIValue[i] = tmp;
    };


    // read digital inputs
    for (i=0; i<MAX_DINUM; i++)  {
        DIValue.LBit(i) = IO.GetDI(ToChar(i+1));
    };


    // write analog outputs
    for(i=0; i<MAX_AONUM; i++)  {
        IO.SetAO( ToChar(i+1), AOValue[i] );
    };


    // write digital outputs
    for (i=0; i<MAX_DONUM; i++)  {
        IO.SetDO( ToChar(i+1), DOValue.LBit(i) );
    };

};



TAlarmsManager  AlarmsManager;

char UIAlarmNotification;   // 0=no alarms, 1=alarms active, buzzer off, 2=buzzer on
char UIAlarmReset;          // 1=mute, 2=reset inactive alarms
long UIActiveAlarms[4];     // keep status of active alarms

modbusdecl(ModbusTable, modbusR , UIAlarmNotificationID, UIAlarmNotification);
modbusdecl(ModbusTable, modbusRW, UIAlarmResetID, UIAlarmReset);
//modbusdecl(ModbusTable, modbusR , AlarmsBaseIndexCAN + 10, UIActiveAlarms[0]);
//modbusdecl(ModbusTable, modbusR , AlarmsBaseIndexCAN + 12, UIActiveAlarms[1]);
//modbusdecl(ModbusTable, modbusR , AlarmsBaseIndexCAN + 14, UIActiveAlarms[2]);
//modbusdecl(ModbusTable, modbusR , AlarmsBaseIndexCAN + 16, UIActiveAlarms[3]);


void Alarms_Init()  {
    AlarmsManager.Init();
};

void Alarms_Run()  {
    long i;

    AlarmsManager.Run();

    if (AlarmsManager.BuzzerReq)
        UIAlarmNotification = 2;
    //else if (AlarmsManager.NackAlarms)
    else if (AlarmsManager.ActiveAlarms)
        UIAlarmNotification = 1;
    else
        UIAlarmNotification = 0;

    // handle alarms on-board notification
    IO.SetLed(2,AlarmsManager.ActiveWarnings != 0 || AlarmsManager.NackAlarms != 0);  // warning led when there are active warnings or inactive alarms
    IO.SetLed(3,AlarmsManager.ActiveAlarms);

    // collect all alarms
    for(i=0; i<ASize(UIActiveAlarms); i++)  {
        UIActiveAlarms[i] = 0;
    };
    for(i=0; i<ASize(AlarmsConfig); i++)  {
        if (AlarmsManager.AlarmsStatus[i].AlarmActive)
            UIActiveAlarms[ i / 32 ].LBit(i % 32) = 1;
    };

    // reset alarm command
    if (UIAlarmReset)  {
        AlarmsManager.Mute();
        if (UIAlarmReset > 1)
            AlarmsManager.ResetAlarms(1);
        UIAlarmReset = 0;
    };

    #ifdef EVENT_HISTORY_MAX_EVENTS
        #ifdef EVENT_HISTORY_SAVE_PARAMS_CHANGE
            if (EventHistoryLog_AddEvent)  {
                EventHistoryLog.WriteEvent(EventHistoryLog_AddEvent, EventHistoryLog_AddEventData);
                EventHistoryLog_AddEvent = 0;
            };
        #endif
    #endif

};


/**
    Set or clear alarm condition.
    Similar to SetAlarmCondition(), this function adds another parameter to specify a different reset condition.
    It can be usefull when you have conditions based on analog values thresold
    (e.g. high pressure alarm set if pressure > high limit, reset if pressure < high limit - delta).
    If you do not do like this, an auto-reset alarm could be triggered many times when pressure is close to high limit (due
    to minimum variation of the input).

    @param iIndex is the alarm index (to access alarm information)
    @param iSetCondition is the condition to set alarm (1 if alarm must be set)
    @param iResetCondition is the condition to reset alarm (1 if alarm must be reset)
    @param iDevActive is the status of the device connected to the alarm: (0=device is off, 1=device is on). This is
                        to handle the StandbyActive flag, that define if the alarm is active when the corresponding device is off.
                        NOTE: If alarm was activated, with StandbyActive = 0, then device will turn off, the alarm will remain on.
    @param iStartElapsedTime is the time since the device connected to the alarm is on. Use -1 to disable alarm delay (alarm will be immediatly activated).

    e.g.: SetOrResetAlarmCondition(AL_FlowCond_ID, IO.GetDI(DI_AlFlow), !IO.GetDI(DI_AlFlow), PumpManager.isOn(), PumpManager.LastPumpTurnOn.ElapsedSec())
*/
void SetOrResetAlarmCondition(long iIndex, long iSetCondition, long iResetCondition, char iDevActive = 0, long iStartElapsedTime = 0)  {
    if (AlarmsManager.isAlarm(iIndex))
        AlarmsManager.SetAlarmCondition(iIndex, !iResetCondition, iDevActive, iStartElapsedTime);
    else
        AlarmsManager.SetAlarmCondition(iIndex, iSetCondition, iDevActive, iStartElapsedTime);
};





// this is the initialization procedure, and will be called once, at startup of the application
void AlgoLib_Init()  {
    // TODO: Add here initialization code (this will be executed only once after power-on)

    #ifdef Sys_NodeID_ID
        // keep current CANopen NodeID
        Params_Sys_NodeID = Can.NodeID;
        LastNodeID = Can.NodeID;
    #endif

    // load the NodeID and COM configuration
    Com1.Ident = Can.NodeID;

    char flags;
    if (Params_Sys_SerialSet == 0)
        flags = comSerFlag_8N1;
    else if (Params_Sys_SerialSet == 1)
        flags = comSerFlag_8E1;
    else
        flags = comSerFlag_8N2;
    Com1.Init(Params_Sys_SerialBaud,flags);

    IOManager_Init();
    Alarms_Init();
    UIParamsChanged = 1;

    #ifdef SUPPORT_RTC
        RTC_Run();
    #endif

    #ifdef EVENT_HISTORY_MAX_EVENTS
        EventHistoryLog.Init(RTC);
    #endif

    #ifdef T7dScheduledAction_MaxActions
        Scheduler.Init();
    #endif

};


// this is the control loop procedure, and will be called continuously
void AlgoLib_Run()  {
    // TODO: Add here your algorithm

    #ifdef Sys_NodeID_ID
        if (CANgetNodeID() != Can.NodeID)  {
            // the node ID have been changed by network (LSS)
            Can.NodeID = CANgetNodeID();
            Params_Sys_NodeID = Can.NodeID;
            LastNodeID = Can.NodeID;
        };

        //if (UIParamsChanged)  {
            if (LastNodeID != Params_Sys_NodeID)  {
                // eeprom param have been changed, reset CANopen
                Can.NodeID = Params_Sys_NodeID;
                LastNodeID = Can.NodeID;
                Can.Init(Can.CurrSpeed(),0);
                Com1.Ident = Can.NodeID;
            };
        //};
    #endif

    IOManager_Run();

    #ifdef AL_Probe1_ID
        // check Analog inputs alarms
        long i;
        for(i=0; i<ASize(AIDefault); i++)  {
            if (AIDefault[i].ErrorOnVAI != 0 && AIDefault[i].IndexAIType != 0)
                AlarmsManager.SetAlarmCondition(AL_Probe1_ID + i, AIValue[i] == IO_NAValue, ToChar(SysOn.GetStatus()), SysOn.tmr.ElapsedSec());
        };
    #endif

    Alarms_Run();

    #ifdef SUPPORT_RTC
        RTC_Run();
    #endif

    #ifdef EVENT_HISTORY_MAX_EVENTS
        EventHistoryLog.Run(RTC);

        if (UIResetHistory)  {
            // reset history log
            UIResetHistory = 0;
            EventHistoryLog.ClearHistory();
        };
    #endif

    // update onboard led (toggle on every application main loop)
    IO.SetLed(1, !IO.GetLed(1) );

};
