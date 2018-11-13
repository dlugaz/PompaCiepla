/** \file

    Standard user interface framework.

 \par Company:
 Danfoss Electronics s.p.a.

 \author Sebastiano Fiorindo

 ******************************************************************************

    This user interface have the following standard features:

    - Application information screen (also shown at startup)
    - Parameters editing
    - Password login
    - IO display
    - Alarms management


    TODO:
    - Application loop timer
    - Sys on (with timer)
    - IO configuration

 ******************************************************************************
*/

#include "AGF_Define.c"
#include "AppDefine.c"

#ifdef MULTILANGUAGE
    #ifdef STRINGTABLE
    #else
        #define STRINGTABLE
    #endif
#endif

#include "AGF_Strings.c"
#include "AGF_MenuDef.c"
#include "AGF_Params.c"

#include "AGF_Alarm.c"
#include "alarms.lib"

#include "CLogin.lib"
#include "multitask.lib"

#include "TIOValue.lib"


#ifdef EVENT_HISTORY_MAX_EVENTS
    #ifdef SUPPORT_RTC
    #else
        #define SUPPORT_RTC
    #endif

    #include "TEventHistoryLog.lib"
#endif

#ifdef T7dScheduledAction_MaxActions
    #ifdef SUPPORT_RTC
    #else
        #define SUPPORT_RTC
    #endif

    #include "Scheduler7d.lib"

    // define the description for each allowed action
    const String T7dScheduledAction_ActionNames(3)[] = {"OFF", "ON "};
#endif


#ifdef SUPPORT_RTC
    #include "RTC_UI.lib"

    TRemoteRTC RTC;
#endif


#include "UserInterface.lib"


TMenuInterface UI;
TLogin Login;
TParamForm ParamForm;

TIOValue IOValuesForm;

// declare global variable and forms
TCANVarValMngMB CANVar;     // this will be used to access parameters in both local and remote version


#ifdef included_demolib
#else

    TCANVarRequester VDIValue;
    TCANVarRequester VDOValue;
    TCANVarRequester VAIValue;
    TCANVarRequester VAOValue;

    struct TCANVarRequester  {
        word BaseAddr;

        void Init(word iBaseAddr)  {
            BaseAddr = iBaseAddr;
        };

        long geti(long iIndex)  {
            return CANVar.Get(BaseAddr + iIndex);
        };
    };

    TCANVarRequester ParamsVal;


    struct TUIAlarmManager : public TCANVarRequester  {
        long isAlarm(long iIndex)  {
            return CANVar.Get(BaseAddr + iIndex);
        };
    };

    TUIAlarmManager AlarmsManager;

#endif


#define FORM_SWVER          1
#define FORM_NOCONNECTION   90
#define FORM_ALLALARMS      91
#define CMD_RESETALR        92
#define FORM_EVENT_HISTORY  93
#define CMD_RESETHIS        94
#define FORM_OK_DONE        95
#define FORM_SCHEDULER      96
#define FORM_RTC            97
#define FORM_SCHEDULER_EDIT 98
#define FORM_SCHEDULER_SHOW 99
#define CMD_CHANGELANG     150  // reserved id until CMD_CHANGELANG+MULTILANGUAGE (last excluded)




char AlarmNotification;






/*
    Initialization of user interface library.
    Call this function at initialization of your application user interface.
*/
void UILib_Init()  {

    // Beep at startup
    SetBuzzer(1,100);

    CANVar.Init();

    #ifdef MMI_APP
        ParamsVal.Init(ParamsBaseIndexCAN);
        AlarmsManager.Init(AlarmsExBaseIndexCAN);
        VDIValue.Init(VDIBaseIndexCAN);
        VDOValue.Init(VDOBaseIndexCAN);
        VAIValue.Init(VAIBaseIndexCAN);
        VAOValue.Init(VAOBaseIndexCAN);
    #else
    #endif

    // initialize user interface
    UI.Init();
    //Show logo of company
    dsp.DrawImage(0,0,38);
    dsp.UpdateDisplay();
    Sleep(2000);

    // if there is no display, wait forever (so the UI will never be started)
    while (!dsp.HaveDisplay() && kbd.Key == 0)  {
        Suspend(5000, Suspend_WakeUpOnKey);
    };

    Sleep(2000);

    #ifdef SUPPORT_RTC
        RTC.Init();
    #endif

    UI.ShowForm(FORM_MAIN);
}


/*
    User interface library main loop.
    Call this function on main user interface loop.
    This function will handle standard forms.
    If current form is not standard, the function will return 0, and the application must handle it.
*/
char UILib_Run()  {

    #ifdef Sys_NodeID_ID
        if (CANVar.UITargetID == 0 && CANgetNodeID() != Can.NodeID)  {
            // I'm running in local and the node ID have been changed
            Can.NodeID = CANgetNodeID();
            CANVar.Init();
        };
    #endif

    AlarmNotification = CANVar.Get(UIAlarmNotificationID);
    SetBuzzer(AlarmNotification > 1);

    // mute alarms if a key is pressed
    if (kbd.Key != 0 && AlarmNotification > 1)
        CANVar.Set(UIAlarmResetID,1);


    // handle user interface
    UI.Run();

    // handle standard forms
    if (UI.CurrFormID == FORM_MAIN)  {
        UI.AccessLevel = 0;     // reset access level, when we move back to main form
        DrawFormMain();
    }
    else if (UI.CurrFormID == FORM_LOGIN)  {
        if ( Login.Run(ToWord(Params_Password1),ToWord(Params_Password2),ToWord(Params_Password3)) )  {
            UI.PrevForm();
            UI.AccessLevel = Login.Level;
        }
    }

#ifdef DEBUG
    else if (UI.CurrFormID == FORM_DEBUG)
        DebugForm.Run();
#endif

#ifdef NO_LCD_SUPPORT
#else
    else if (UI.CurrFormID == FORM_DEVINFO)  {
        if (UI.NewForm)  {
            dsp.ClearDisplay();
            UI.NewForm = 0;
            DrawMaskInfo();
        };
        if (kbd.Key == KEY_ESC)
            UI.PrevForm();
        else if (kbd.Key == KEY_DOWN)
            UI.ShowFormAndKeepHistory(FORM_VMINFO);
    }
    else if (UI.CurrFormID == FORM_VMINFO)  {
        if (UI.NewForm)  {
            dsp.ClearDisplay();
            UI.NewForm = 0;
            DrawFormVMInfo();
        };
        if (kbd.Key == KEY_ESC)
            UI.PrevForm();
    }
#endif


    else if ((UI.CurrFormID & 0xF000) == FORM_MENU)  {
        short root;
        root = UI.CurrFormID & 0x0FFF;
        UI.DrawMenu(root, MenuTable[root].Start, MenuTable[root].Stop);
    }
    else if ((UI.CurrFormID & 0xF000) == FORM_MENUSINGLE)  {
        short root;
        root = UI.CurrFormID & 0x0FFF;
        ParamForm.DrawParam(MenuTable[root].Start, MenuTable[root].Stop);
    }

#ifdef UI_SETUP_IO
    // support for configurable I/O
    else if (UI.CurrFormID == FORM_CONFIGDI)  {
        // DI Setup
        TypeSetupIO = 2;
        if (IOSetup.Run())
            UI.PrevForm();
    }
    else if (UI.CurrFormID == FORM_CONFIGDO)  {
        // DO Setup
        TypeSetupIO = 4;
        if (IOSetup.Run())
            UI.PrevForm();
    }
    else if (UI.CurrFormID == FORM_CONFIGAI)  {
        // AI Setup
        TypeSetupIO = 1;
        if (IOSetup.Run())
            UI.PrevForm();
    }
    else if (UI.CurrFormID == FORM_CONFIGAO)  {
        // AO Setup
        TypeSetupIO = 3;
        if (IOSetup.Run())
            UI.PrevForm();
    }
#endif

    else if (UI.CurrFormID == FORM_SWVER)  {
        DrawFormSwVer();
    }

    else if (UI.CurrFormID == FORM_IOVALUES)  {
        // display values of current probes
        if (IOValuesForm.Run())
            UI.PrevForm();
    }

    else if (UI.CurrFormID == FORM_OK_DONE)  {
        DrawFormOkDone();
    }

    else if (UI.CurrFormID == FORM_ALLALARMS)  {
        DrawFormAlarms();
    }

    else if (UI.CurrFormID == CMD_RESETALR)  {
        // reset alarms
        CANVar.Set(UIAlarmResetID,2);
        UI.ShowForm(FORM_OK_DONE);
    }

    #ifdef SUPPORT_RTC
        else if (UI.CurrFormID == FORM_RTC)  {
            DrawFormRTC(1);
        }
    #endif

    #ifdef EVENT_HISTORY_MAX_EVENTS
        else if (UI.CurrFormID == FORM_EVENT_HISTORY)  {
            DrawFormEventHistoryLog();
        }
        else if (UI.CurrFormID == CMD_RESETHIS)  {
            // reset history log
            CANVar.Set(EVENT_HISTORY_EXPORT_BASE-1,1);
            UI.ShowForm(FORM_OK_DONE);
        }
    #endif

    #ifdef T7dScheduledAction_MaxActions
        else if (UI.CurrFormID == FORM_SCHEDULER)  {
            //DrawFormScheduler(1);
            DrawFormScheduleList();
        }
        else if (UI.CurrFormID == FORM_SCHEDULER_SHOW)  {
            //DrawFormScheduler(1);
            DrawFormScheduleShow();
        }else if (UI.CurrFormID == FORM_SCHEDULER_EDIT)  {
            DrawFormScheduleEdit();
        }
    #endif

    #ifdef MULTILANGUAGE
        else if (UI.CurrFormID >= CMD_CHANGELANG && UI.CurrFormID < CMD_CHANGELANG+MULTILANGUAGE)  {
            // change to selected language. NOTE: on MMI this will also reset the application.
            SetUILanguage(UI.CurrFormID - CMD_CHANGELANG + 1);
            UI.ShowForm(FORM_OK_DONE);
        }
    #endif

    else
        // unknown form
        return 0;

    // form was processed
    return 1;

}


//
// display alarms form for led and lcd version
//
void DrawFormAlarms()  {
    String Code(12);
    long   n;
    long   iDir;

    if (UI.NewForm)  {
        UI.NewForm = 0;
        kbd.Val = -1;       // current alarm index to be displayed
        iDir = +1;          // first time need to move forward
        dsp.ClearDisplay();
    };


    // now display active alarms, if any
    if (AlarmNotification > 0)  {
        long chk = -1;  // -1 = first time

        if (kbd.Key == KEY_DOWN)
            iDir = +1;
        else if (kbd.Key == KEY_UP)
            iDir = -1;

        // clear display when changing alarm and when coming from "no alarms" screen
        if (iDir != 0 || kbd.Val == -1)
            dsp.ClearDisplay();

        // scan for next active alarm/warning
        #ifdef SUPPORT_PACKED_ALARMS
            long LastAlrPack = 0;
        #endif
        long numAlarm = ASize(AlarmsConfig);
        while (numAlarm >= 0)  {
            numAlarm--;

            kbd.Val += iDir;
            if (kbd.Val >= ASize(AlarmsConfig))
                kbd.Val = 0;
            else if (kbd.Val < 0)
                kbd.Val = ASize(AlarmsConfig)-1;


            #ifdef SUPPORT_PACKED_ALARMS
                // read alarm bit
                if ((kbd.Val % 32) == 0 || chk == -1)
                    // read 32 alarm bits in one shot, NOTE: we need to skip odd offset used only by modbus
                    LastAlrPack = CANVar.Get(PackedAlarmsBaseIndexCAN + ((kbd.Val / 32)*2));
                if (LastAlrPack.LBit(kbd.Val % 32))
                    chk = 2;
                else
                    chk = 0;
            #else
                chk = CANVar.Get(AlarmsExBaseIndexCAN + kbd.Val);
            #endif

            if (chk)  {
                if (dsp.isLed())  {
                    // led display version
                    dsp.DigA = AlarmsConfig[kbd.Val].Code;
                    dsp.DigB = "    ";
                }
                else  {
                    //-------------- dsp.DrawImageOnOff(1, 0, CUSTOM_ICON_ALARM24, 0);

                    dsp.DrawTextCR(1,5,AlarmsConfig[kbd.Val].Code);

                    #ifdef DOUBLEHEIGHTFONT
                        char PrevFont;
                        PrevFont = dsp.GetFont();
                        dsp.SetFont(ToChar(DOUBLEHEIGHTFONT));
                    #endif

                    // show active alarm description
                    #ifdef STRINGTABLE
                        String Descr(ALARM_DESCRLEN);
                        LoadFromStringTable(Descr, AlarmsConfig[kbd.Val].DescrID);
                        dsp.DrawMultilineTextCR(0,0,Descr);
                    #else
                        dsp.DrawMultilineTextCR(0,0,AlarmsConfig[kbd.Val].Descr);
                    #endif

                    #ifdef DOUBLEHEIGHTFONT
                        dsp.SetFont(PrevFont);
                    #endif

                    if (chk > 1)
                        // write if alarm is active
                        dsp.DrawTextCR(10,5,"ACTIVE");
                    else
                        dsp.DrawTextCR(10,5,"      ");
                };

                break;
            }
            else if (iDir == 0)
                iDir = 1;   // to prevent deadlock
        };
    }
    else  {
        // no alarms

        // if we were showing an alarm, clear display
        if (kbd.Val != -1)  {
            UI.NewForm = 1;     // this will reset display and others on next loop
        }
        else  {

            if (dsp.isLed())  {
                dsp.DigB = "----";
                dsp.DigA = "A--";
            }
            else  {
                // write in row 0, so if an alarm appear, it will overwrite the text
                dsp.DrawTextCenter(0,"No alarms");
                //dsp.DrawImage(1, 0, imgALARM24);
                //dsp.DrawTextCenter(0,"---------");
            };

            // after some seconds, go back to main form
            //if (tmrForm.ElapsedSec() >= 2)
            //if (kbd.ActiveKey == 0 && kbd.ActiveTime >= 2000)
            //    UI.ShowForm(FORM_MAIN);
        };
    };

    // if we keep ESC pressed for more than 2 seconds, reset alarms
    if (kbd.ActiveKey == KEY_ESC && kbd.ActiveTime >= 2000)  {
        // reset alarms
        CANVar.Set(UIAlarmResetID,2);
    }
    else if (kbd.ReleasedKey == KEY_ESC)  {
        UI.PrevForm();
    };

};



/**
    Check if connection with UITarget is active.
    If no connection, show a NO CONNECTION message.
    If connection is active, go back to previous form.

    Just call this function in your main form.

    @param iCheckVarID is one variable index to be used to check connection.
    @return IO_NAValue if connection is not active. The value of the iCheckVarID variable.

    @code
        void DrawFormMain()  {

            // check KEY to enter menu
            if (kbd.Key == KEY_OK)  {
                UI.ShowMenu(FORM_ROOTMENU);
                return;
            };

            if ( CheckNoConnection(10) == IO_NAValue )
                return;

            if (UI.NewForm)  {
                dsp.ClearDisplay();
                UI.NewForm = 0;
            };

            dsp.DrawTextCR(0,0,"Main form");

        };
    @endcode
*/
long CheckNoConnection(word iCheckVarID)  {
    RetVal = CANVar.Get(iCheckVarID);

    if (UI.CurrFormID != FORM_NOCONNECTION)  {
        // check if connection is active
        if (RetVal == IO_NAValue)
            UI.ShowForm(FORM_NOCONNECTION);
    }
    else  {

        if (UI.NewForm)  {
            dsp.ClearDisplay();
            UI.NewForm = 0;
        };

        dsp.DrawTextCenter(3,"NO CONNECTION");

        if (kbd.Key == KEY_ESC || RetVal != IO_NAValue)  {  // check if connection have resumed or ESC key have been pressed
            // switch to previous form
            UI.PrevForm();

            // reconnect and update UI if required
            MMIConnectToDevice(-1);
        };

    };

};



/**
    Show a confirm form, sound a beep and wait user to press ESC.
*/
void DrawFormOkDone()  {

    if (UI.NewForm)  {
        dsp.ClearDisplay();
        UI.NewForm = 0;
        SetBuzzer(1,200);   // beep

        if (dsp.isLed())  {
            dsp.DigB = " -1-";  // TODO: adjust with correct character
            //dsp.DigB[2] = 0x88;
        }
        else  {
            dsp.DrawTextCenter(3,"! DONE !");
            dsp.DrawTextCenter(5,"ESC to go back");
        };
    };

    if (kbd.Key == KEY_ESC)  {
        // switch to previous form
        UI.PrevForm();
    };

};


