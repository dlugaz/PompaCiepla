
#define PASSWORD_MASK

#ifdef DrawValueCR_SINGLELINE
#else
    #define DrawValueCR_SINGLELINE  //DrawValueCR does not work on multiple lines, but it will be faster
#endif

#define ParamsBaseIndexCAN	    0x2000
#define DIBaseIndexCAN          0x2200
#define DIPOLBaseIndexCAN       0x2220
#define DOBaseIndexCAN          0x2400
#define DOPOLBaseIndexCAN       0x2420
#define AITypeBaseIndexCAN      0x2600
#define AIBaseIndexCAN          0x2800
#define AICALBaseIndexCAN       0x3000
#define AIMINBaseIndexCAN       0x3200
#define AIMAXBaseIndexCAN       0x3400
#define AIPERCENTBaseIndexCAN   0x3500
#define AIERRFLAGBaseIndexCAN   0x3550
#define AOTypeBaseIndexCAN      0x3600
#define AOBaseIndexCAN          0x3800
#define ChillerStatusBaseIndex   0x4000  // base address for exported chiller status variables
#define ChillerAlarmsBaseIndex   0x4300  // base address for exported alarms
#define IOValueBaseIndexCAN      0x4500  // base address for exported chiller I/O
#define EXVValveBaseIndexCAN     0x5500  // base address for exported EEV Value

#define SendReloadAIConfig			SICommandCAN.Bit(0)		// Invio richiesta di riconfigurazione ingressi analogici
#define SendReloadAOConfig          SICommandCAN.Bit(1)     // Invio richiesta di riconfigurazione uscita analogica
#define SendReloadDIConfig			SICommandCAN.Bit(0)		// Invio richiesta di riconfigurazione ingressi digitali (stessa che per gli ingressi analogici)
#define SendReloadDOConfig          SICommandCAN.Bit(1)     // Invio richiesta di riconfigurazione uscita digitali (stessa che per le uscite analogiche)
#define SendReadRTC                 SICommandCAN.Bit(2)     // Invio richiesta di lettura data e ora
#define SendWriteRTC				SICommandCAN.Bit(3)		// Invio richiesta di scrittura data e ora
#define SendONStandby               SICommandCAN.Bit(4)     // Invio richiesta di Attivazione modalità Standby
#define SendOFFStandby              SICommandCAN.Bit(5)     // Invio richiesta di Disattivazione modalità Standby
#define SendAckAlarms               SICommandCAN.Bit(6)     // Invio riconoscimento allarmi
#define SendParamsChanged           SICommandCAN.Bit(7)     // Invio notifica avvenuta modifica parametri

#define ReceiveReloadAIConfig		RICommandCAN.Bit(0)		// Richiesta di riconfigurazione ingressi analogici
#define ReceiveReloadAOConfig		RICommandCAN.Bit(1)		// Richiesta di riconfigurazione uscita analogica
#define ReceiveReloadDIConfig		RICommandCAN.Bit(0)		// Richiesta di riconfigurazione ingressi analogici
#define ReceiveReloadDOConfig		RICommandCAN.Bit(1)		// Richiesta di riconfigurazione uscita analogica
#define ReceiveReadRTC              RICommandCAN.Bit(2)     // Richiesta lettura valori data e ora
#define ReceiveWriteRTC				RICommandCAN.Bit(3)		// Richiesta scrittura valori data e ora
#define ReceiveONStandby            RICommandCAN.Bit(4)     // Richiesta Attivazione modalità Standby
#define ReceiveOFFStandBy           RICommandCAN.Bit(5)     // Richiesta Disattivazione modalità Standby
#define ReceiveAckAlarms            RICommandCAN.Bit(6)     // Richiesta riconoscimento allarmi
#define ReceiveParamsChanged        RICommandCAN.Bit(7)     // Ricezione notifica modifica parametri

#define CAN_WRITE_WAIT_TIME         2400
#define EXD_CAN_WRITE_WAIT_TIME     500

#define FULLALCODE

#define Params_AL_NextFree_ID   ALARM_ARRAY_SIZE

#define Kbr kbd
#define Dsp dsp
#define ParamsVal ParamVal
#define ParamFields ParamField

//**************   SUPPORT FOR PACKET ALARMS   *********** T 2/02/2011 *****
    #define SUPPORT_PACKED_ALARMS
    #ifdef SUPPORT_PACKED_ALARMS
        #define PackedAlarmsBaseIndexCAN 0x5000
    #endif
//**************  CONDENSER SPECIAL FUNCTION   *********** T 17/02/2011 ***
     //#define COND_SPECIAL_FUNCTION   // Add secon setpoint for fan condenser

//**************  MODBUS THREAD  *******************************************
     #define MODBUS_THREAD          // Run Modbus run into dedicated thread

//********************  PERFORMER VSD SUPPORT  *** SC April 2011 *******************
    /// #define PERFORMER_VSD_SUPPORT

    #ifdef PERFORMER_VSD_SUPPORT
        #define MB_MASTER_THREAD                // used to set a dedicated thread to update the PVSD structure

        #define ENABLE_POLLIONG_OPTIMIZATION    // used to set hi priority to the analog input reading and writing operation

        #ifdef MB_MASTER
        #else
            #define MB_MASTER               // modbus master thread
            #define MODBUSMASTER_LAST_CHAR_RECEIVED 5   // used as delay time check between two chars, default value 100 ms.
            #define Params_ModBus_Master_SerialSet 19200 // it should be get from params
            #define MODBUS_ERROR_ALARM_THERSOLD 5
        #endif

        #ifdef MODBUSMASTER_COM_ID
        #else
            #define MODBUSMASTER_COM_ID 1   // modbus master com port id: 1 or 2
        #endif

        // comment out this macro if modbus master is bound on com 2
        #define MB_MASTER_ID_1            // modbus master on first (or only) RS-485 port

        #ifdef MMI_APP
        #else
            #ifdef MB_MASTER_ID_1
                #ifdef NO_COM
                #else
                    #define NO_COM        // com 1 port free
                #endif
            #endif
        #endif //MMI_APP
    #endif //PERFORMER_VSD_SUPPORT


//********************  EIM316 EEV drive support *** SC April 2011 *******************
    /// #define EIM316

    #ifdef EIM316
        #ifdef MB_MASTER
        #else
            #define MB_MASTER               // modbus master thread
            #define MODBUSMASTER_LAST_CHAR_RECEIVED 5   // found 30  // used as delay time check between two chars, default value 100 ms.
            #define Params_ModBus_Master_SerialSet 19200 // it should be get from params
            #define MODBUS_ERROR_ALARM_THERSOLD 5
        #endif

        #ifdef MODBUSMASTER_COM_ID
        #else
            #define MODBUSMASTER_COM_ID 1   // modbus master com port id: 1 or 2
        #endif

        // comment out this macro if modbus master is bound on com 2
        #ifdef MB_MASTER_ID_1
        #else
            #define MB_MASTER_ID_1          // modbus master on first (or only) RS-485 port
        #endif

        #ifdef MMI_APP
        #else
            #ifdef MB_MASTER_ID_1
                #ifdef NO_COM
                #else
                    #define NO_COM              // com 1 port free
                #endif
            #endif
        #endif //MMI_APP

    #endif //EIM316

//**************  REGOLATION SET BY ANALOG INPUT     ***** T 23/09/2010 *****
     //#define ABSOLUTE_SET_ENABLE    // Define to enable this feature

//**************  ENABLE FILTER ON ANALOG INPUT      ***** T 23/09/2010 *****
     //#define FILTER_0-10_ENABLE   // Define to enable this feature on voltage current input
     //#define FILTER_NTC_ENABLE    // Define to enable this feature on resistor probe input
     #ifdef FILTER_NTC_ENABLE
         #define MCX06_Filter 3
         #define MCXXX_Filter 6
     #else
         #define MCX06_Filter 1
         #define MCXXX_Filter 1
     #endif

//**************   DISCHARGE-SUCTION PROBE POSITION  ***** T 22/09/2010 *****
     #define PROBE_CHANGE_IN_HP     // Define if probe are on coil
                                    // undefine if probe are on compressor (before reverse valve)
     #define DischargeP_ID 0        // ProbeConfiguration bit pointer
     #define SucttionP_ID  1        // ProbeConfiguration bit pointer

//********************   DISABLE MCX06C USER FORMS       ***** T 14/09/2010 **
    //#define NO_LED_SUPPORT                  // disable led support
        #ifdef NO_LED_SUPPORT               //
        #endif

//**********  COMPRESSOR - PUMP HOUR COUNTER UNIT  ******** T 24/08/2010 *****
    // set hour counter unit
    #define HOUR_COUNTER_UNIT_LED          36000 //(36000 sec = 10 ore !!!)
    #define HOUR_COUNTER_UNIT_LCD          3600  //(3600  sec = 1 ora !!!)
    #define HOUR_COUNTER_UNIT              3600  // per edit
    #define EVAP_HOURLIMIT_UNIT            1000  // allarm thersold unit of Params_SinkPump_HourLimit
    #define COMPRESSOR_HOURLIMIT_UNIT      1000  // allarm thersold unit of Params_SinkPump_HourLimit
    #define EDITHOUR_ENABLE

//**********************  USER INTERFACE           ******** T 24/05/2010 *****
    #define MENU_INACTIVITY_TIMEOUT 3600    // inactivity timeout 60 min.
    #define WELCOME_FORM_TIME       5       // Time to show the welcome form.

//********************    SCHEDULER  *********************** T 08/05/2011 *****
    /// #define SCHEDULER_ENABLE
        #ifdef SCHEDULER_ENABLE
			#ifdef Params_RHP_Circuit_10_Description
				#define T7dScheduledAction_MaxActions 55    // max number of program
			#else
				#define T7dScheduledAction_MaxActions 20    // max number of program
			#endif
            #define T7DSCHEDULER_HANDLE_GROUP           // handle group
            #define GROUP_BIT 4                         // max group number = 2^GROUP_BIT

            // graph
            #define OX 0            // offset x
            #define OY 56           // offset y (only mulies of 8)
            #define LenBar 97       // lenght of time bar
            #define StdStepY 8      // Pixel offset for each action
            #define SmallStepY 4    // Pixel offset for each action
            #define MaxAction 2     //
            #define ShowActualHour  // Show blinking cursor of actual hour
        #else
            #define OY 56           // offset y (only mulies of 8)
            #define LenBar 0        // without scheduler move left
        #endif

//********************   ALARM HISTORY  ******************** C 08/05/2010 *****
    /// #define ALARM_HISTORY_ENABLE
        #ifdef ALARM_HISTORY_ENABLE
            #ifdef Params_RHP_Circuit_10_Description
                #define EVENT_HISTORY_MAX_EVENTS  1             // max 2 allarms
            #else
                #ifdef Params_RHP_Circuit_6_Description
                    #define EVENT_HISTORY_MAX_EVENTS  3         // max 5 allarms
                #else
                    #define EVENT_HISTORY_MAX_EVENTS  6         // max 10 allarms
                #endif
            #endif
        #endif

//********************    PICTURE MANAGER     *********************************
    #define SHOW_START_LOGO_DX              // Enable the DX start logo.
        #ifdef SHOW_START_LOGO_DX
            #define OFFSET_X_LOGO1_DX  0    // Offset in pixel of the DX start logo from the ceter of LCD
            #define OFFSET_Y_LOGO1_DX  0    //
            #define OFFSET_X_LOGO2_DX  0    //
            #define OFFSET_Y_LOGO2_DX  0    //
            #define OFFSET_X_LOGO3_DX  0    //
            #define OFFSET_Y_LOGO3_DX  0    //
        #endif                              //

//*******************************   Main Screen ****************************
    #define SHOW_CLOCK              // Show clock in the main form.

//********************    DEBUG FORM         *********************************
    //#define DEBUG_FORM                              // ENABLE FORM FOR A DEEP ANALYSIS
        #ifdef DEBUG_FORM
            #define SHOW_LOOP_TIME_VALUE              // show user interface appication time and main application time
            //#define SET_DEBUG_MATRIX                  // buzz on TX queue overflow
      #endif

//********************  RHP RESIDENTIAL HEAT PUMP *****************************
    #define HANDLE_RHP                              // enable RHP manager
    #ifdef HANDLE_RHP
        // Chiller status bitmap
        #define sta_ShComprStatus   0
        #define sta_P_W             1
        #define sta_F_W             2
        #define sta_Dis             3
        #define sta_DHW_HOLD        4
        #define sta_BT_REQ_HP_OFF   5
        #define sta_P_OFF           6 // Burst off time
        #define sta_P_PR            7 // Burst pre-run

        // icons status
        #define st_wOn      0
        #define st_wOf      1
        #define st_On1      2
        #define st_On2      3
        #define st_On3      4
         #define st_Ondhwp  4
        #define st_On4      5
         #define st_OndhwR  5
        #define st_On5      6
        #define st_On6      7
        #define st_On7      8
        #define st_On8      9
        #define st_Alr      10
        #define st_War      11
        #define st_free1    12
        #define st_free2    13
        #define st_free3    14
        #define st_free4    15

        //Compressor status
        #define cst_P03      0
        #define cst_F03      1
        #define cst_ct0      2
        #define cst_ct1      3
        #define cst_ct2      4
        #define cst_ct3      5
        #define cst_ct4      6
        #define cst_c06      7
        #define cst_on       8
        #define cst_Won      9
        #define cst_Alr      10
        #define cst_War      11
        #define cst_Dis      12
        #define cst_Inv_WOn  13
        #define cst_Inv_WOff 14
        #define cst_free8    15

        // time costant
        #define secD    86400
        #define secH    3600

        #ifdef Params_RHP_Circuit_10_Description
            #define RHP_MAX_CIRCUIT                 10
        #else
            #ifdef Params_RHP_Circuit_6_Description
                #define RHP_MAX_CIRCUIT             6
            #else
                #define RHP_MAX_CIRCUIT             3
            #endif
        #endif
        //#define SHOW_LOOPtIME

        // TODO rimuovere parametri non usati
        #ifdef Params_HP_Setpoint_Set
        #else
            #define Params_HP_Setpoint_Set          200
            #define Params_HP_Setpoint_Min          -999
            #define Params_HP_Setpoint_Max          999
            #define Params_SetpointS_Set            200
            #define Params_SetpointS_Min            -999
            #define Params_SetpointS_Max            999
            #define Params_HP_Setpoint2_SetOffset   0
            #define Params_Setpoint2_SetOffset      0
            #define Params_Setpoint_CompensEn       0
            #define Params_HP_Setpoint_TCompensStart 0
            #define Params_HP_Setpoint_TCompensMax  0
            #define Params_HP_Setpoint_CompensMax   0
            #define Params_SetpointS_TCompensStart  0
            #define Params_SetpointS_TCompensMax    0
            #define Params_SetpointS_CompensMax     0
        #endif
        // TODO end rimuovere parametri non usati


        #define STANBY_ACTIVE_RESET_TIME
        #define T_MAIN                          0
        #define T_CIRC                          1
        #define T_DHW                           2
        #define T_ROOM                          3
        #define T_SUN                           4
        #define Params_Conf_HeatPumpType        1   //0=no, 1=refrigerant changeover, 2=water changeover

        #define Params_Sys_Icons                1
        #define Params_Conf_EvapNum             1
        #define Params_Conf_CircNumPerEvap      1
        #define Params_Conf_EvapAirorWater      1
        #define Params_Conf_EvapForPNum         1
        #define Params_Conf_RNumPerEvap         1
        #define Params_Conf_CpNum               0
        #define Params_Conf_CondOneFanCircuit   1
        #define Params_Conf_DO_HBoiler          0
        #define Params_Setpoint2_DiffOffset     10
        #define Params_Comp_ParOnMode           0
        #define Params_Comp_ParOffMode          0
        #define Params_Comp_ParStartDelay       1

        #ifdef Params_RegPIS_Diff
        #else
            #define Params_RegPIS_Diff          30
        #endif
        #ifdef Params_HP_RegPI_Diff
        #else
            #define Params_HP_RegPI_Diff        30
        #endif
        #ifdef Params_RegPI_Time
        #else
            #define Params_RegPI_Time           600
        #endif
        #define Params_RegS_DeadZone            00
        #define Params_HP_Reg_DeadZone          00
        #define Params_RegDZ_DiffAct            20
        #define Params_RegDZ_DiffDeact          20
        #define Params_RegDZ_TMinOff            50
        #define Params_HP_RegDZ_TMaxOff         470
        #define Params_Comp_TimeMaxAct          120
        #define Params_Comp_TimeMinAct          100
        #define Params_Comp_TimeMaxDeact        120
        #define Params_Comp_TimeMinDeact        100
        #define Params_PumpDown_MaxTime         0
        #define Params_PumpDown_StartDelay      0
        #define Params_PumpDown_StartupPressureSet  140
        #define Params_PumpDown_StopPressureSet     70
        #define Params_Comp_C3En                0
        #define Params_Comp_C4En                0
        #define Params_Comp_C5En                0
        #define Params_Comp_C6En                0
        #define Params_Comp_C7En                0
        #define Params_Comp_C8En                0
        #define Params_Res_C                    1
        #define Params_Res_Set                  1
        #define Params_Res_Diff                 1
        #define Params_HP_Res_Set               1
        #define Params_HP_Res_Diff              1
        #define Params_SinkPump_Pump2Rot            2
        #define Params_SinkPump_Pump2RotHourLimit   24
        #define Params_Al_Relay_Delay           0
        #define Params_Al_Relay_ActInStdby      1
        #define Params_Al_BoilerHighTempSet     500
        #define Params_Al_BoilerHighTempDiff    20
        #define Params_HP_Def_HeatersON         0
        #define Params_HP_Def_ManualEn          0
        #define Params_HP_Def_DifferentCircuitMinDelay  0
        #define Params_HP_Def_MulticircuitStartType     0
        #define Params_HP_Def_MulticircuitEndType       0
    #endif


//********************  NTC 100K PROBE        *********************************
    #define NTC_100K_ENABLE
        #ifdef NTC_100K_ENABLE
            #ifdef STEINHART_HART_CALCULATION
            #else
                #define STEINHART_HART_CALCULATION
            #endif
            // *********  NTC 100 K #25°C **************
                #define A_CONST_NTC100K    0.0008019542049643450
                #define B_CONST_NTC100K    0.0002116010720441770
                #define C_CONST_NTC100K    0.0000000759594885749
            // *****************************************
        #endif

//********************  NTC 10K PROBE        *********************************
    //#define NTC_10K_ENABLE
        #ifdef NTC_10K_ENABLE
            #ifdef STEINHART_HART_CALCULATION
            #else
                #define STEINHART_HART_CALCULATION
            #endif
            // *********  NTC 100 K #25°C **************
                #define A_CONST_NTC10K  0.00088713844547330900000
                #define B_CONST_NTC10K  0.00025129507961602700000
                #define C_CONST_NTC10K  0.00000019501028923490600
            // *****************************************
        #endif

//********************  NTC 2K PROBE        *********************************
    #define NTC_2K_ENABLE
        #ifdef NTC_2K_ENABLE
            #ifdef STEINHART_HART_CALCULATION
            #else
                #define STEINHART_HART_CALCULATION
            #endif
            // *********  NTC 2 K #25°C **************
                #define A_CONST_NTC2K  0.00220161345528768000000
                #define B_CONST_NTC2K  0.00009963244584966990000
                #define C_CONST_NTC2K  0.00000092078669653274200
        #endif
//********************  HANDLE ioType0_5V_NOP ********************************
    #define ioType0_5V_NOP_ENABLE

//********************  EXD ELECTRONIC VALVE  *********************************
/** These define to enable EXV manager
  */
    ///#define EXD_ENABLE                                 //enable EXD manager
    #ifdef EXD_ENABLE
        #define NUMBER_OF_EXD       1              //change the EXD need chiller export updete
        #define EXD316_1ST_VALVE
        #define MAX_EXD_PAR 13
    #endif

//********************  MCX1V-2V VALVE INTEGRATION ***************************
    #define NUMBER_OF_EXV   0
    #ifdef INTERNAL_VALVES_ENABLE // depends on EXD_ENABLE
        #ifdef Ver_MCX061V
            #undef NUMBER_OF_EXV
            #define NUMBER_OF_EXV   1
            #define EXV_1ST_VALVE
            #define MCX_INT_VALVE_INTEGRATION
        #endif

        #ifdef Ver_MCX152V
            #undef NUMBER_OF_EXV
            #define NUMBER_OF_EXV   2
            #define EXV_2ND_VALVE
            #define MCX_INT_VALVE_INTEGRATION
        #endif

        #ifdef EXV_1ST_VALVE
            #ifdef EXD316_1ST_VALVE
                #undef EXD316_1ST_VALVE
            #endif
        #endif

        #ifdef EXV_2ST_VALVE
            #ifdef EXD316_2ST_VALVE
                #undef EXD316_2ST_VALVE
            #endif
        #endif
    #endif


//********************    SUPPORT_BUFFER_TANK    *****************************
/** These define to support the buffer tank manager
  */
    ///#define SUPPORT_BUFFER_TANK
    #ifdef SUPPORT_BUFFER_TANK
        #define NUMBER_OF_BUFFER_TANK   3
        #define PARAMSFORM_SUPPORT_HMS_UNIT
    #endif //#ifdef SUPPORT_BUFFER_TANK

//********************    TSIMPLEINVERTER_SUPPORT_OFF_TIME    ****************
/** These define to handle the minimum off time
  * in the inverter library of the file  HVAClib.lib
  */
    #define TSIMPLEINVERTER_SUPPORT_OFF_TIME
    #define Params_Cond_Min     10
    #define Params_Cond_Max     70
    #define Params_Cond_MinTm   25
    #define Params_Cond_DimTm   45

//********************    TSIMPLEINVERTER_SUPPORT_OFF_TIME    ****************
/** These define to handle the minimum off time
  * in the inverter library of the file  HVAClib.lib
  */
    #define FIX_FOR_COMP_WORKINGSECONDS

//********************    HANDLE_DIMMERED_BRHIGHTNESS         ****************
/** Use these fonction to change the value of brightness fron a HI
  * to a LOW value of intensity.
  */
    #define HANDLE_DIMMERED_BRHIGHTNESS

//********************    ENABLE IO CONFIGURATION             ****************
/** Use this define to set the Virtual IO
  *
  */
    #ifdef ENABLE_IO_OVERRIDE
    #else
        //#define ENABLE_IO_OVERRIDE
    #endif

    #ifdef ENABLE_IO_OVERRIDE
        #define ENABLE_AI_OVERRIDE
        #define ENABLE_DI_OVERRIDE
        #define ENABLE_AO_OVERRIDE
        #define ENABLE_DO_OVERRIDE

        #define AO_OVERRIDE_ID    0X3900
        #define AI_OVERRIDE_ID    0X3920
        #define DO_OVERRIDE_ID    0X3940
        #define DI_OVERRIDE_ID    0X3960
    #endif // ENABLE_IO_OVERRIDE

//********************    ENABLE DEBUG FORM            ******************
/** Use this define to show the commissioning form and the remainin time
  * to start compressor
  *
  */
    /// #define ENABLE_DEBUG_FORM

//********************    SHOW_WAINTING_TIME            ******************
/** Use this define to show the waiting time to start the next function.
  */
    #define SHOW_WAINTING_TIME  // Use this define to show the remainin time

//********************    NO BUZZER ON WARNINGS          ****************
/** define this to avoid the buzzer sounds on warning Allarm
  *
  */
    /// #define ENABLE_BUZZER_ON_WARNINGS
    #ifdef ENABLE_BUZZER_ON_WARNINGS
    #else
        #define NO_BUZZER_ON_WARNINGS
    #endif

//********************    RHP WAIT CIRCUIT REQ         ******************
/** The off time of the bust function end if there is a circuit request
  *
  */
    /// #define RHP_WAIT_CIRCUIT_REQ



/**
 ******************************************************************************
 * Set Enable Solar Plant <br>
 ******************************************************************************
 **/
/// #define ENABLE_SOLAR_PLANT
    #ifdef ENABLE_SOLAR_PLANT
        #define ENABLE_SOLAR_PLANT_UI_FORM
        #define ENABLE_SOLAR_PLANT_KBD
        #define SPECIAL_LOAD_DEFINITION
        #define EXPORT_SOLAR_PLANT_VAR
    #endif // ENABLE_SOLAR_PLANT

/**
 ******************************************************************************
 * Enable the User Interface asyncronous can request with priority optimization
 ******************************************************************************
 **/
#define USE_CANRWSCHEDULER



/**
 ******************************************************************************
 * Multilanguage related compile options
 ******************************************************************************
 **/
/// MULTILANGUAGE is defined in MakePackage.bat
///#define MULTILANGUAGE 2       // Enable multilanguage support.
                                 // Define to the total number of languages supported (including default one)
#ifdef MULTILANGUAGE
    #define STRINGTABLE
    //#define DOUBLEHEIGHTFONT    (2 * (GetUILanguage() == 2))    /* support double height font in menu and set correct font */
#endif

// define EXTENDED REFERENCE SOUBGROUPS to extend the maximum parameter number from 256 to 65535
#define EXTENDED_REFERENCE_SOUBGROUPS

//********************   ENABLE MCX06C USER FORMS  ************************
///#define LED_SUPPORT                       // disable led support //
    #ifdef LED_SUPPORT
    #else
        #define NO_LED_SUPPORT
    #endif

//********************   ENABLE LCD USER FORMS   **************************
///#define LCD_SUPPORT                        // disable lcd support //
    #ifdef LCD_SUPPORT
    #else
        #define NO_LCD_SUPPORT
    #endif

//********************   ENABLE LCX USER FORMS   *************************
///#define LCX_SUPPORT                        // disable lcd support //


#ifdef SCHEDULER_ENABLE
    #define OPT_1       0x00000001
#else
    #define OPT_1       0
#endif

#ifdef ALARM_HISTORY_ENABLE
    #define OPT_2       0x00000002
#else
    #define OPT_2       0
#endif

#ifdef EXD_ENABLE
    #define OPT_3       0x00000004
#else
    #define OPT_3       0
#endif

#ifdef EIM316
    #define OPT_4       0x00000008
#else
    #define OPT_4       0
#endif

#ifdef PERFORMER_VSD_SUPPORT
    #define OPT_5       0x00000010
#else
    #define OPT_5       0
#endif

#ifdef MCX_INT_VALVE_INTEGRATION
    #define OPT_6       0x00000020
#else
    #define OPT_6       0
#endif

#ifdef ENABLE_IO_OVERRIDE
    #define OPT_7       0x00000040
#else
    #define OPT_7       0
#endif

#ifdef ENABLE_BUZZER_ON_WARNINGS
    #define OPT_8       0x00000080
#else
    #define OPT_8       0
#endif

#ifdef ENABLE_SOLAR_PLANT
    #define OPT_9       0x00000100
#else
    #define OPT_9       0
#endif

#ifdef ENABLE_CFG_IO
    #define OPT_10      0x00000200
#else
    #define OPT_10      0
#endif


#ifdef LED_SUPPORT
    #define OPT_11      0x00000400
#else
    #define OPT_11      0
#endif


#ifdef LCD_SUPPORT
    #define OPT_12      0x00000800
#else
    #define OPT_12      0
#endif


#ifdef LCX_SUPPORT
    #define OPT_13      0x00001000
#else
    #define OPT_13      0
#endif

#ifdef MMI_APP
#else
const long VERSION_BUILD_OPTIONS = OPT_1
                                 + OPT_2
                                 + OPT_3
                                 + OPT_4
                                 + OPT_5
                                 + OPT_6
                                 + OPT_7
                                 + OPT_8
                                 + OPT_9
                                 + OPT_10
                                 + OPT_11
                                 + OPT_12
                                 + OPT_13;
#endif //MMI_APP

#define VERSION_CUSTOMER_ID      1              // Customer ID
#ifdef Params_CfgVer
	#define VERSION_CFG_VER      Params_CfgVer  // Shape parameter set from shape
#else
	#define VERSION_CFG_VER      0              // Shape parameter set from appdefine
#endif
#define CUS_ID_STR              "XX"            //
