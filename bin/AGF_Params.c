﻿//************************************************************************
// File automatically generated by MCXShape v.2.01.00
// -------------------------------------------------
//
// !!Warning:!!
// The amendment of the Code can affect the performance of the application
// ***********************************************************************


// Visibility condition method
long menu_CheckIsAvailableParam(long iCondition)
{
    iCondition = iCondition & 0x3F;
    if (iCondition == 0) return 1;
    return 1;
};


// Params fieds table
const TParamsField ParamField[] = 
{
//  {Ind,Code ,Description ,Min ,Max ,Default ,UnitM ,Dec1 ,Dec2 ,RW ,VMin ,VMax ,Mask, Level ,Visibility
    {bTurnCO_ID, "WC","Wlacz centralne ogrzewanie", 0,1,0,0,0,0,0,0,0,1,0,0},
    {Temp_CO_ID, "CO","Temperatura w zbiorniku C.O.", 0,100,35,1,0,0,0,0,0,0,0,0},
    {Temp_CWU_ID, "CWU","Temperatura w zbiorniku C.W.U.", 0,100,38,1,0,0,0,0,0,0,0,0},
    {Temp_Poddasze_ID, "CBM","Zalaczenie PG2 - chlodzenie poddasza", 0,1,0,0,0,0,0,0,0,1,0,0},
    {Temp_Pasteryzacji_ID, "PAS","Temperatura pasteryzacji", 0,100,70,1,0,0,0,0,0,0,0,0},
    {Temp_BM_ID, "BM","Estymowana temperatura w budynku mieszkalnym", -20,100,25,1,0,0,0,0,0,0,0,0},
    {Temp_Curve_Control_ID, "KPO","Sterowanie temp C.O. przy pomocy krzywej pogodowej", 0,1,0,0,0,0,0,0,0,1,0,0},
    {Weather_Curve_ID, "POG","Nachylenie krzywej pogodowej", 0,20,6,0,1,0,0,0,0,0,0,0},
    {Taryfa_ID, "TA1","Grzanie C.O. tylko w czasie II taryfy", 0,1,1,0,0,0,0,0,0,1,0,0},
    {taryfacwu_ID, "TA2","Grzanie C.W.U. tylko w czasie II taryfy", 0,1,0,0,0,0,0,0,0,1,0,0},
    {Hist_CWU_ID, "HI1","Szerokosc histerezy temp. w C.W.U.", 0,100,2,1,0,0,0,0,0,0,0,0},
    {Hist_CO_ID, "HI2","Szerokosc histerezy temp. C.O.", 0,100,2,1,0,0,0,0,0,0,0,0},
    {Temp_AFR_ID, "AFR","Temperatura dla blokady przeciwzamrozeniowej", -40,50,-10,1,0,0,0,0,0,0,0,0},
    {Temp_PW_ID, "POW","Temperatura przelaczania na pompe P/W", -30,50,5,1,0,0,0,0,0,0,0,0},
    {Hist_PW_ID, "HPW","Histereza przelaczania pompy P/W", -100,100,30,1,1,0,0,0,0,0,0,0},
    {WypPG1_ID, "WYP","Czas wyprzedzenia PG1 przed S1 i S2", 0,999,30,8,0,0,0,0,0,0,0,0},
    {Comp_TimeMinON_ID, "CC7","Minimalny czas ON sprezarek", 0,999,30,8,0,0,0,0,0,0,0,0},
    {Comp_TimeMinOFF_ID, "CC6","Minimalny czas OFF sprezarek", 0,999,180,8,0,0,0,0,0,0,0,0},
    {PasterizationTimeOut_ID, "PTO","Dopuszczalny maksymalny czas pasteryzacji", 0,1000,180,9,0,0,0,0,0,0,0,0},
    {Pasterization_Time_ID, "TPA","Czas pasteryzacji", 0,100,30,9,0,0,0,0,0,0,0,0},
    {Temp_Diff_ID, "ROZ","Roznica temp.zewn. i BM przy ktorej dzialaja dwie sprezarki", 0,100,40,1,0,0,0,0,0,0,0,0},
    {Time_Const_ID, "TBM","Stala czasowa budynku", 0,1000,90,10,0,0,0,0,0,0,0,0},
    {Max_TempCO_ID, "MCO","Maksymalna temperatura w zbiorniku CO", 0,100,38,1,0,0,0,0,0,0,0,0},
    {Max_TempCWU_ID, "MCW","Maksymalna temperatura w zbiorniku CWU", 0,100,50,1,0,0,0,0,0,0,0,0},
    {Temp_PompyObiegoweCO_ID, "DPO","Temperatura do zalaczanie pomp obiegowych CO", -300,300,160,1,1,0,0,0,0,0,0,0},
    {bSchedule_override_ID, "SCO","Obejscie wewnetrznego harmonogramu", 0,1,1,0,0,0,0,0,0,1,0,0},
    {Temp_RealBuilding_ID, "TRB","Temperatura z modelu inercyjnego", -1000,1000,0,1,1,0,0,0,0,0,0,0},
    {temp_zal_szybkiego_went_ID, "TWE","Temperatura zalaczania szybkiego biegu wentylatora", -300,1000,100,1,1,0,0,0,0,0,0,0},
    {moc_inwertera_ID, "MOC","Moc Inwertera powyzej ktorej mozemy zalaczyc grzanie", 0,60,30,11,1,0,0,0,0,0,0,0},
    {Sys_NodeID_ID, "SEr","Serial address (Modbus and CAN)", 1,100,2,0,0,0,0,0,0,0,0,0},
    {Sys_SerialBaud_ID, "bAU","Serial baudrate (Modbus)", 2,10,4,0,0,0,0,0,0,1,0,0},
    {Sys_SerialSet_ID, "COM","Serial settings (Modbus)", 11,13,0,0,0,0,0,0,0,1,0,0},
    {Al_Buzz_ID, "BUZ","Buzzer czas aktywacji", 0,15,1,9,0,0,0,0,0,0,0,0},
    {Al_Relay_Delay_ID, "AdL","Opoznienie aktywacji stycznika alarmu", 0,999,0,8,0,0,0,0,0,0,0,0},
    {Al_Relay_ActInStdby_ID, "AOF","Stycznik alarmu aktywny gdy jednostka WYL ?", 0,1,1,0,0,0,0,0,0,1,0,0},
    {Sys_OnOff_ID, "y01","ON/OFF", 14,15,1,0,0,0,0,0,0,1,0,0},
    {Sys_Default_ID, "y07","Przywroc ustawienia standardowe", 0,1,0,0,0,0,0,0,0,1,0,0},
    {Password1_ID, "L01","Haslo poziom 1", 0,999,100,0,0,0,0,0,0,0,0,0},
    {Password2_ID, "L02","Haslo poziom 2", 0,999,200,0,0,0,0,0,0,0,0,0},
};


// Mask Table
const TMask MaskParams[] =
{
    {"NO"},
    {"YES"},
    {"0"},
    {"12"},
    {"24"},
    {"48"},
    {"96"},
    {"144"},
    {"192"},
    {"288"},
    {"384"},
    {"8N1"},
    {"8E1"},
    {"8N2"},
    {"OFF"},
    {"ON"},
};


// measurement unit table
const TUnit UnitTab[] =
{
    {""},
    {"°C"},
    {"K"},
    {"°F"},
    {"bar"},
    {"%"},
    {"Time"},
    {"Psi"},
    {"s"},
    {"min"},
    {"h"},
    {"kW"},
};


#ifdef MMI_APP
#else
// Params export CAN-BUS table
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + bTurnCO_ID, bTurnCO);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_CO_ID, Temp_CO);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_CWU_ID, Temp_CWU);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_Poddasze_ID, Temp_Poddasze);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_Pasteryzacji_ID, Temp_Pasteryzacji);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_BM_ID, Temp_BM);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_Curve_Control_ID, Temp_Curve_Control);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Weather_Curve_ID, Params_Weather_Curve);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Taryfa_ID, Params_Taryfa);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + taryfacwu_ID, Params_taryfacwu);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Hist_CWU_ID, Params_Hist_CWU);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Hist_CO_ID, Params_Hist_CO);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_AFR_ID, Params_Temp_AFR);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_PW_ID, Params_Temp_PW);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Hist_PW_ID, Params_Hist_PW);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + WypPG1_ID, Params_WypPG1);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Comp_TimeMinON_ID, Params_Comp_TimeMinON);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Comp_TimeMinOFF_ID, Params_Comp_TimeMinOFF);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + PasterizationTimeOut_ID, Params_PasterizationTimeOut);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Pasterization_Time_ID, Params_Pasterization_Time);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_Diff_ID, Params_Temp_Diff);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Time_Const_ID, Params_Time_Const);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Max_TempCO_ID, Params_Max_TempCO);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Max_TempCWU_ID, Params_Max_TempCWU);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_PompyObiegoweCO_ID, Params_Temp_PompyObiegoweCO);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + bSchedule_override_ID, bSchedule_override);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Temp_RealBuilding_ID, Params_Temp_RealBuilding);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + temp_zal_szybkiego_went_ID, Params_temp_zal_szybkiego_went);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + moc_inwertera_ID, Params_moc_inwertera);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Sys_NodeID_ID, Params_Sys_NodeID);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Sys_SerialBaud_ID, Params_Sys_SerialBaud);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Sys_SerialSet_ID, Params_Sys_SerialSet);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Al_Buzz_ID, Params_Al_Buzz);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Al_Relay_Delay_ID, Params_Al_Relay_Delay);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Al_Relay_ActInStdby_ID, Params_Al_Relay_ActInStdby);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Sys_OnOff_ID, Params_Sys_OnOff);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Sys_Default_ID, Params_Sys_Default);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Password1_ID, Params_Password1);
modbusdecl(ModbusTable, modbusRW, ParamsBaseIndexCAN + Password2_ID, Params_Password2);
modbusdecl(ModbusTable, modbusR, ParamsBaseIndexCAN + Password3_ID, Params_Password3);
#endif


