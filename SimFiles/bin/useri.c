//===============================================================================
// User Interface
//===============================================================================

#define MMI_APP

#define MAX_COMPRNUM    2
#define MAX_EXT_COIL    1
#define MAX_INT_COIL    1
#define MAX_CIRCNUM     1


// must be here to let others modules know our parameters definitions (#define Params_*)
#define Params_Sys_KeyLock_Val             0    // this keyboard is always enabled
#define TParamForm_SupportUM                    // enable support of Unit conversion on TParamForm.

#include "AGF_Define.c"

#ifdef Ver_MCX06C_EXC
#define Ver_MCX06
#define EXC_SUPPORT
#endif

#ifdef Ver_MCX06C
#define Ver_MCX06
#endif

#ifdef Ver_MCX06D
#define Ver_MCX06
#endif

#include "AGF_Digital_Input.c"
#include "AGF_Digital_Output.c"
#include "AGF_Analog_Input.c"
#include "AGF_Analog_Output.c"
#define MAX_AINUM       NUMBER_AI
#define MAX_AONUM       NUMBER_AO
#define MAX_DINUM       NUMBER_DI
#define MAX_DONUM       NUMBER_DO

#ifdef Ver_MCX06
// MCX06C e MCX06D
#define IO_SMALL_VER
#endif

#include "AppDefine.lib"
#include "nvmlib/mcxlib.lib"
#include "nvmlib/sysdef.lib"
#include "nvmlib/stdui.c"
#include "nvmlib/multitask.lib"


#include "AGF_resources.c"
#include "AGF_Define.c"

#include "AGF_Alarm.c"
#include "ch_export.lib"
#include "IOSetupUI.lib"

#ifdef ALARM_HISTORY_ENABLE
    #include "nvmlib_additional/TEventHistoryLog.lib"
#endif // ALARM_HISTORY_ENABLE

#ifdef ENABLE_SOLAR_PLANT
#include "_TSPM/TSPM.lib"
TSPMForm SPMForm;
#endif // ENABLE_SOLAR_PLANT

#include "version.lib"

#ifdef USE_CANRWSCHEDULER
#include "CanRWScheduler_interface.lib"
#endif //USE_CANRWSCHEDULER

#include "_UserInterfaceUtils/UserInterfaceUtils.lib"
#include "UserInterfaceUser.lib"
struct TUILibParams :public TStdParamsCompile  {
    // TODO: Add here your application parameters
};

persistent TUILibParams UILibParams;

struct TParamsRequester  {
    word BaseAddr;

    long geti(long iIndex)  {
        return CANVar.Get(BaseAddr + iIndex);
    };
};


#ifdef USE_CANRWSCHEDULER
// defines and vars declared in CanRWScheduler_interface.c
#else //USE_CANRWSCHEDULER
long S32_UISysOnOffStatus       = IO_NAValue;
long S32_UIComprStatus_C1       = IO_NAValue;
long S32_UIComprStatus_C2       = IO_NAValue;
long S32_UIComprGlobalStatus    = IO_NAValue;
long S32_DIOnOff                = IO_NAValue;
long S32_UIComprInv             = IO_NAValue;
long S32_UIPump                 = IO_NAValue;
long S32_UISFan                 = IO_NAValue;
long S32_UISPump                = IO_NAValue;
long S32_UIHeater               = IO_NAValue;
long S32_ConfigError            = 0;
long S32_CurrSetpoint           = IO_NAValue;
long S32_Treg                   = IO_NAValue;
long S32_DefrostOn              = IO_NAValue;
long S32_MainLoopTime           = 0;
long S32_ch_sta                 = IO_NAValue;
long S32_Man_Set                = IO_NAValue;
long S32_DHW_Man_Set            = IO_NAValue;
long S32_LowBrightness          = 30;
long S32_HiBrightness           = 100;
long S32_CompStage              = 0;
long S32_StatusSTH              = 0;
#endif //USE_CANRWSCHEDULER
unsigned char UIBlock;
struct TChillerRequester  {
    char UICommandReq;     // 0=no command, 1=reset buzzer, 2=reset alarms, 3=reload parameters, 4=switch on/off status, 5=reload default normal, 6=reload default full
    word UITrg;
    char group;

    void Run(){
        UITrg = CANVar.GetUITarget();
        UIBlock++;
        if (UIBlock == 1) {
            HandleReqMBVar(UITrg , ChillerTreg                   ,S32_Treg );
            HandleReqMBVar(UITrg , ChillerCurrSetpointID         ,S32_CurrSetpoint );
            HandleReqMBVar(UITrg , ChillerUISysOnOffStatusID     ,S32_UISysOnOffStatus );
            HandleReqMBVar(UITrg , ChillerUIComprStatusID + 0    ,S32_UIComprStatus_C1 );
            HandleReqMBVar(UITrg , ChillerUIComprStatusID + 1    ,S32_UIComprStatus_C2 );
        }
        else if (UIBlock == 2) {
            HandleReqMBVar(UITrg , ChillerUIComprGlobalStatusID  ,S32_UIComprGlobalStatus );
            HandleReqMBVar(UITrg , ChillerDIOnOffID              ,S32_DIOnOff );
            HandleReqMBVar(UITrg , ChillerUIPumpID               ,S32_UIPump );
            HandleReqMBVar(UITrg , ChillerUISFanID               ,S32_UISFan );
            HandleReqMBVar(UITrg , ChillerStatusSTH_M_ID         ,S32_StatusSTH );
        }
        else if (UIBlock == 3) {
            HandleReqMBVar(UITrg , ChillerUISPumpID              ,S32_UISPump );
            HandleReqMBVar(UITrg , ChillerUIHeaterID             ,S32_UIHeater );
            HandleReqMBVar(UITrg , ChillerConfigErrorID          ,S32_ConfigError );
            HandleReqMBVar(UITrg , ChillerDefrostOnID            ,S32_DefrostOn );
            HandleReqMBVar(UITrg , AddressMainLoopTimeID         ,S32_MainLoopTime );
        }
        else if (UIBlock == 4) {
            HandleReqMBVar(UITrg , Chillerch_staID               ,S32_ch_sta );
            HandleReqMBVar(UITrg , Circ_Man_Set_1_M_ID           ,S32_Man_Set );
            HandleReqMBVar(UITrg , DHW_Man_Set_1_M_ID                           ,S32_DHW_Man_Set );
            HandleReqMBVar(UITrg , ChillerStageON_M_ID                          ,S32_CompStage );
            HandleReqMBVar(UITrg , ParamsBaseIndexCAN + Sys_LowBrightness_ID    ,S32_LowBrightness );
            HandleReqMBVar(UITrg , ParamsBaseIndexCAN + Sys_HiBrightness_ID     ,S32_HiBrightness );
            UIBlock = 0;
        }
    };

    short UIAlarmNotification()  {
        return CANVar.Get(ChillerUIAlarmNotificationID)
    };
    char UISysOnOffStatus()  {
        return S32_UISysOnOffStatus;
    };
    short UIComprStatus(char Compr)  {
        if      (Compr == 0)
            return S32_UIComprStatus_C1;
        else if (Compr == 1)
            return S32_UIComprStatus_C2;
        else
            return IO_NAValue;
    };
    short UIComprGlobalStatus()  {
        return S32_UIComprGlobalStatus;
    };
    short DI_OnOff()  {
        return S32_DIOnOff;
    };
    short UIPump()  {
        return S32_UIPump;
    };
    short UISFan()  {
        return S32_UISFan;
    };
    short UISPump()  {
        return S32_UISPump;
    };
    short UIHeater()  {
        return S32_UIHeater;
    };
    short ConfigError()  {
        return S32_ConfigError;
    };
    short CurrSetpoint()  {
        return S32_CurrSetpoint;
    };
    short Treg()  {
        return S32_Treg;
    };
    short DefrostOn()  {
        return S32_DefrostOn;
    };
    long SysTime()  {
        RetVal = CANVar.Get(ChillerSysTimeID);
        // try again
        // this is because when we turn device ON, the next response could be quite slow (because device is initilizing chiller)
        // so we could loose answer. Then it's better to try again before to give an error.
        if (RetVal == IO_NAValue)
            RetVal = CANVar.Get(ChillerSysTimeID);
    };
};

struct TParams :public TStdParams  {
};

persistent  TParams Params;

TChillerRequester uich;

TTimer UserLoopTimer;
short UserLoopValue;

void App_Init()  {

    // ensure persistent parameters are initialized
    Params.InitParams();

    // set buzzer (keep bios settings)
    SetBuzzerVolume(Can.RequestAndWaitVar(Can.NodeID , 0x5002 , 3 ));

    ParamsVal.BaseAddr = ParamsBaseIndexCAN;


    if (UILibParams.VerifyParams())  {
        // default params have been reloaded
        UILibParams.ValidateParams();
#ifdef MULTILANGUAGE
            // reset default language
            // NOTE: on MMI this will also reset the application, so we need to validate parameters before executing this command
       SetUILanguage(0);
#endif
    }

    // init display
    Display_Init();
};

void App_Run()  {
    if (uich.UICommandReq)  {
        CANVar.Set(ChillerUICommandReqID,uich.UICommandReq);
        uich.UICommandReq = 0;
    };

    Display_Run();

    UserLoopValue = UserLoopTimer.Elapsedms();
    UserLoopTimer.Start();
};
