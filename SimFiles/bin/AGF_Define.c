﻿//************************************************************************
// File automatically generated by MCXShape v.2.01.00
// -------------------------------------------------
//
// !!Warning:!!
// The amendment of the Code can affect the performance of the application
// ***********************************************************************

//General define


#define Ver_MCX15

#define SELECTED_LANGUAGE        "EN"
#define EN_LANGUAGE
#define LANGUAGE_EN        2

#define APP_VERSION                     "1.00"


#define T7dScheduledAction_MaxActions   10             //Scheduler



// Index params list
#define Comp_TimeMinOFF_ID                                0
#define Comp_TimeMinON_ID                                 1
#define taryfa_ID                                         2
#define Hist_CWU_ID                                       3
#define Hist_CO_ID                                        4
#define Temp_Antifreeze_ID                                5
#define wypPG1_ID                                         6
#define Password1_ID                                      7
#define Password2_ID                                      8
#define Password3_ID                                      9
#define Sys_OnOff_ID                                      10
#define Sys_Default_ID                                    11
#define Sys_NodeID_ID                                     12
#define Sys_SerialBaud_ID                                 13
#define Sys_SerialSet_ID                                  14
#define Al_Buzz_ID                                        15
#define Al_Relay_Delay_ID                                 16
#define Al_Relay_ActInStdby_ID                            17
#define tempCwu_ID                                        18
#define tempPaster_ID                                     19
#define tempCO_ID                                         20
#define tempCooling_ID                                    21

// Value params list
#define Params_Comp_TimeMinOFF                            ParamVal[Comp_TimeMinOFF_ID]
#define Params_Comp_TimeMinON                             ParamVal[Comp_TimeMinON_ID]
#define Params_taryfa                                     ParamVal[taryfa_ID]
#define Params_Hist_CWU                                   ParamVal[Hist_CWU_ID]
#define Params_Hist_CO                                    ParamVal[Hist_CO_ID]
#define Params_Temp_Antifreeze                            ParamVal[Temp_Antifreeze_ID]
#define Params_wypPG1                                     ParamVal[wypPG1_ID]
#define Params_Password1                                  ParamVal[Password1_ID]
#define Params_Password2                                  ParamVal[Password2_ID]
#define Params_Password3                                  300
#define Params_Sys_OnOff                                  ParamVal[Sys_OnOff_ID]
#define Params_Sys_Default                                ParamVal[Sys_Default_ID]
#define Params_Sys_NodeID                                 ParamVal[Sys_NodeID_ID]
#define Params_Sys_SerialBaud                             ParamVal[Sys_SerialBaud_ID]
#define Params_Sys_SerialSet                              ParamVal[Sys_SerialSet_ID]
#define Params_Al_Buzz                                    ParamVal[Al_Buzz_ID]
#define Params_Al_Relay_Delay                             ParamVal[Al_Relay_Delay_ID]
#define Params_Al_Relay_ActInStdby                        ParamVal[Al_Relay_ActInStdby_ID]
#define tempCwu                                           ParamVal[tempCwu_ID]
#define tempPaster                                        ParamVal[tempPaster_ID]
#define tempCO                                            ParamVal[tempCO_ID]
#define tempCooling                                       ParamVal[tempCooling_ID]

#ifdef MMI_APP
    #define PARAMS_DESCRLEN    44
#else
    #define PARAMS_DESCRLEN 1
#endif
#define PARAMS_NUMParams    22


// Index alarms list
#define AL_Gen_ID                                         0
#define AL_Probe1_ID                                      1
#define AL_Probe2_ID                                      2
#define AL_Probe3_ID                                      3
#define AL_Probe4_ID                                      4
#define AL_Probe5_ID                                      5
#define AL_Probe6_ID                                      6
#define AL_Probe7_ID                                      7
#define AL_Probe8_ID                                      8
#define AL_Probe9_ID                                      9
#define AL_Probe10_ID                                     10
#define AL_Probe11_ID                                     11
#define AL_Probe12_ID                                     12
#define AL_Probe13_ID                                     13
#define AL_Probe14_ID                                     14
#define AL_Probe15_ID                                     15
#define AL_Probe16_ID                                     16

// Value alarms list
#define AL_Gen                                            AlarmsManager.isAlarm(AL_Gen_ID)
#define AL_Probe1                                         AlarmsManager.isAlarm(AL_Probe1_ID)
#define AL_Probe2                                         AlarmsManager.isAlarm(AL_Probe2_ID)
#define AL_Probe3                                         AlarmsManager.isAlarm(AL_Probe3_ID)
#define AL_Probe4                                         AlarmsManager.isAlarm(AL_Probe4_ID)
#define AL_Probe5                                         AlarmsManager.isAlarm(AL_Probe5_ID)
#define AL_Probe6                                         AlarmsManager.isAlarm(AL_Probe6_ID)
#define AL_Probe7                                         AlarmsManager.isAlarm(AL_Probe7_ID)
#define AL_Probe8                                         AlarmsManager.isAlarm(AL_Probe8_ID)
#define AL_Probe9                                         AlarmsManager.isAlarm(AL_Probe9_ID)
#define AL_Probe10                                        AlarmsManager.isAlarm(AL_Probe10_ID)
#define AL_Probe11                                        AlarmsManager.isAlarm(AL_Probe11_ID)
#define AL_Probe12                                        AlarmsManager.isAlarm(AL_Probe12_ID)
#define AL_Probe13                                        AlarmsManager.isAlarm(AL_Probe13_ID)
#define AL_Probe14                                        AlarmsManager.isAlarm(AL_Probe14_ID)
#define AL_Probe15                                        AlarmsManager.isAlarm(AL_Probe15_ID)
#define AL_Probe16                                        AlarmsManager.isAlarm(AL_Probe16_ID)

// Optimize resource alarm array
#ifdef MMI_APP
    #define ALARM_DESCRLEN    15
#else
    #define ALARM_DESCRLEN 1
#endif
#define ALARM_ARRAY_SIZE    17
#define ALARM_BUFFER_SIZE    1


// IO Manager defines
#define IOMNG_AI_NO                       0
#define IOMNG_AI_NTC_10K                  1
#define IOMNG_AI_0_5V                     2
#define IOMNG_AI_0_1V                     3
#define IOMNG_AI_0_10V                    4
#define IOMNG_AI_0_20mA                   5
#define IOMNG_AI_4_20mA                   6
#define IOMNG_AI_PT1000                   7
#define IOMNG_AI_ONOFF                    8
#define IOMNG_AI_NTC_100K                 9
#define IOMNG_AI_NI1000TK5                10
#define IOMNG_AI_0_5V_NOP                 11
#define IOMNG_AI_NTC_2K                   12

#define IOMNG_AO_NO                       0
#define IOMNG_AO_0_1V                     1
#define IOMNG_AO_0_5V                     2
#define IOMNG_AO_0_10V                    3
#define IOMNG_AO_PPM                      4
#define IOMNG_AO_PWM                      5
#define IOMNG_AO_FREQ                     6
#define IOMNG_AO_DIGOUT                   7
#define IOMNG_AO_EXV                      8

