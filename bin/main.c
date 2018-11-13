/** \file

    MCXApp logic main file


 \par Company:
 ZaTeChS S.C.

 \author Krzysztof D³u¿yñski

 ******************************************************************************

    This is the controller logic main file.

 ******************************************************************************
*/


#include "nvmcc_headers.h"
#include "stdalgo.c"
#include "AlgoLib.c"
#include "math.lib"
#include "ModbusMaster.lib"

#include "AGF_Digital_Input.c"
#include "AGF_Digital_Output.c"
#include "AGF_Analog_Input.c"
#include "AGF_Analog_Output.c"
#include "TStaticIOManager.lib"
#include "HVAClib.lib"


#ifdef SUPPORT_PACKED_ALARMS
    modbusdecl(ModbusTable, modbusR + modbusPackedBits32, PackedAlarmsBaseIndexCAN, AlarmsManager.PackedAlarmsStatus);
#endif


TStaticIOManager IOManager;


bit GlobalOn;
TToggleTimer SysOn;
//definicja czujnikow
float NTC_Temp_CO;
float NTC_Temp_Zewn;
float NTC_Temp_CWU;
float NTC_Temp_Glik;
//Modbus Master do po³¹czenia z inwerterem
TModbusMaster Com2;
// declare buffer for com1
byte Com2BufferRx[128];     // size must be multiple of 2
byte Com2BufferTx[128];      // size must be multiple of 2
const byte SlaveID=1;

//Zmienne pomocnicze
int bTaryfaON;  //sygnalizuje czy sa godziny II taryfy (harmonogram)
int bPcwuON;    //sygnalizuje czy sa godziny kiedy ma chodziæ pompa CWU(harmonogram)
int bPasteryzuj;//sygnalizuje czy jest wlaczony tryb pasteryzacji (harmonogram)
int bAL_CzP;    //sygnalizuje czy jest alarm przeplywu
int bHP1_isOn;  //sygnalizuje czy jest wlaczona pompa ciepla 1
int bHP2_isOn;  //sygnalizuje czy jest wlaczona pompa ciepla 2
int bHP3_isOn;  //sygnalizuje czy jest wlaczona pompa ciepla 3
int Hist_od_CWU;//zmienna do histerezy CWU
int Hist_od_CO; //zmienna do histerezy CO
int bCwuON;     // steruje wlaczaniem CWU
int bCoON;      //steruje wlaczaniem CO
int iHPselected; //pomocnicza zmienna - ktora sprezarka obecnie pracuje by nie zmienialy sie zbyt czesto
int bFirstRun; //zmienna do okreslenia pierwszego cyklu
int ActiveSchedules; //zmienna do transportu danych o harmonogramach miedzy GUI a MAIN
short Moc_inwertera;
//Timery
TTimerSec PG1Wait;       //oczekiwania na wlaczenie sie pompy glikolu 1
TTimerSec Comp_Min_On[3];  //odmierzajacy minimalny czas wlaczenia sprezarek
TTimerSec Comp_Min_Off[3]; //odmierzajacy minimalny czas wylaczenia sprezarek
TTimerSec SaveWH;       //do okresowego zapisywania czasow pracy urzadzen
TStoppableTimerSec PasterizationTime;//odmierzajacy czas pasteryzacji
TTimerSec CheckLongSchedule; //do okresowego sprawdzania harmonogramu (aby nie robi³ tego w kazdym cyklu)
TStoppableTimerSec PasterizationTimeOut;


//Countery
THourCounter WHCounters[10];
persistent long CounterValue[10];

//zmienna zapisana w EEPROM zapamietujaca harmonogramy
persistent SchedulePoint mainSchedulesPoints[MAXSCHEDULEPOINTS];

//moje deklaracje do przesylania danych do GUI
modbusdecl(ModbusTable, modbusRW + modbusArray,CountValue,CounterValue); //liczniki godzin pracy urzadzen

modbusdecl( ModbusTable, modbusRW+modbusArray, MySchedules, mainSchedulesPoints );   // export 5 modbus registers
modbusdecl(ModbusTable, modbusRW+modbusArray, Inverter_Power, Moc_inwertera ); //Moc inwertera

/** @fn int CheckSchedule
    Funkcja sprawdzajaca ktore z obecnie ustawionych harmonogramow powinny byc aktywne.
    Powinna byc okresowo wlaczana ze wzgledu na duze potrzeby obliczeniowe.
    Do u¿ytku z CANVar.Set(Schedules,Actions); do przesy³ania do main
    @return Actions - 16 bit gdzie ka¿dy bit oznacza jeden harmonogram
    **/
int CheckSchedule()
{
    char temporary[145];char latch;
    int Actions; //zmienna do przechowywania bitow obecnie dzialajacego harmonogramu
    TDateTime t;unsigned char RTCTime;char RTCDay;
    t.LoadRTC;//wczytanie obecnego czasu z RTC
    t.Explode;
    RTCDay=t.WDay;
    RTCTime=(60*t.Hours+t.Minutes)/10;
    int i=0; int j=0;
    /////////////////////////
    //1. Utworzenie tablicy 144 elementowej w ktorej robimy mapê kiedy dany harmonogram jest ON
    //2. Odczytujemy punkt odpowiadajacy obecnej godzinie i zapisujemy jako bit
    //3. Przechodzimy do kolejnego harmonogramu
    for (j=0;j<=2;j++)//TODO zmienna ilosc harmonogramow
    {

        for (i=0;i<=MAXSCHEDULEPOINTS-1;i++)
            {   //w tablicy 144 elementowej zapisujemy 1 w punktach z obecnego dnia w miejscu wystepowania danej akcji
                if(mainSchedulesPoints[i].CheckDay(RTCDay)>0&&mainSchedulesPoints[i].point!=0&&mainSchedulesPoints[i].ReadScheduleNo==j)
                temporary[mainSchedulesPoints[i].ReadTime()]=mainSchedulesPoints[i].ReadAction+1;
            }
        for (i=0;i<=ARRAY_LENGTH(temporary)-1;i++)
            {//dla wszystkich elementow tablicy przypisujemy 1 jesli jest tam akcja harmonogramu i 0 jesli nie ma
                if(temporary[i]==2)latch=1;
                if(temporary[i]==1)latch=0;
                temporary[i]=latch;
            }
            Actions=Actions|(temporary[RTCTime]<<j);
    }
    return Actions;

}

/** @fn int EraseSchedules
    Funkcja usuwa wszystkie zapisane punkty harmonogramu
**/
int EraseSchedules()
{
    char j;
    for (j=0;j< MAXSCHEDULEPOINTS;j++)
        { mainSchedulesPoints[j].point=0; }
}

/**
    @fn float RealBuildingTemperature() wyliczenie temperatury w budynku mieszkalnym
    na podstawie modelu inercyjnego budynku
    @return Temperatura wewn¹trz budynku (estymowana)
*/
float lastU;TTimer SampleTime;float Y;
float RealBuildingTemperature()
{
    float Tp;float T_CO;float Parameter;float T_zewn;float U;float K;float T;
    if(SampleTime.isClear)SampleTime.Start;
    if(SampleTime.Elapsedms>=1000)
    {
        Tp=SampleTime.Elapsedms/1000;
        T_CO=IntegerToFloat(AI_CZT1,-1);
        Parameter=IntegerToFloat(Params_Weather_Curve,-1);
        T_zewn=IntegerToFloat(AI_CZT3,-1);
        T=IntegerToFloat(Params_Time_Const)*3600;
        //T=10;
        K=1.0;

        U=(T_CO +Parameter*T_zewn)/(1.0+Parameter);
        Y=K*Tp/(2.0*T+Tp)*U+K*Tp/(2.0*T+Tp)*lastU-(Tp-2.0*T)/(2.0*T+Tp)*Y;
        lastU=U;
        SampleTime.Start;
    }
    return Y;
}

/**
    @fn float BuildingTemperature() wyliczenie temperatury w budynku na ,
    krzywej pogodowej,temperatury zewnetrznej i temperatury w zbiorniku CO.
    @return Temperatura wewn¹trz budynku (estymowana)
*/
float BuildingTemperature()
{
    return (NTC_Temp_CO +IntegerToFloat(Params_Weather_Curve,-1)*NTC_Temp_Zewn)/(IntegerToFloat(1)+IntegerToFloat(Params_Weather_Curve,-1));
}
/**
    @fn float COTemperature() wyliczenie temperatury w zbiorniku CO na podstawie ,
    krzywej pogodowej,temperatury zewnetrznej i temperatury w budynku.
    @return Temperatura wewn¹trz zbiornika CO
*/
float COTemperature()
{
    return (IntegerToFloat(Temp_BM)-IntegerToFloat(Params_Weather_Curve,-1)*(IntegerToFloat(NTC_Temp_Zewn)-IntegerToFloat(Temp_BM)));
}

//definicja funkcji kontrolujacych pompy
/**
    @fn void HP1_Turn(int bOnOff,int bCoCwu)
    funkcja kontroluje prace pompy ciepla ze sprezarka S1.
    Alarmy od CzP - czujnika przeplywu, presostatu PWN1 i CzT6- temperatury glikolu blokuja uruchomienie
    @param int bOnOff wlaczenie lub wylaczenie urzadzenia (1 = wlaczony)
    @param int bCoCwu przelaczenie miedzy grzaniem CO i CWU (1 = tryb CO)
*/
void HP1_Turn(int bOnOff,int bCoCwu)
{
    if(AL_CZP==0&&AL_CZT6==0&&AL_PWN1==0&&bOnOff==1&&DO_Z3D1==bCoCwu)//jesli wlaczymy pompe i nie ma zadnych alarmow i nie nastapi³a zmiana stanu CO<=>CWU
    {
        //zasygnalizuj wlaczenie PC
        bHP1_isOn=1;
        //Wlacz pompe glikolu PG1
        DO_PG1=1;
        // poczekaj na Czujnik przeplywu wg. parametru
        if(PG1Wait.isClear)PG1Wait.Start;
        if(PG1Wait.ElapsedSec>Params_WypPG1)    //minal okres wyprzedzenia PG1 lub zalaczyl sie CzP
        {
            //Sprawdz czy wystapil przeplyw glikolu
            if(DI_CZP==1)
            {
                if(bCoCwu==CO)DO_Z3D1=CO; //przelacz zawor trojdrozny w CO
                else DO_Z3D1=CWU;
                //Wlacz pompe cyrk. PC2
                DO_PC2=1;
                //Wlacz S1 w obiegu CO
                if (Comp_Min_Off[0].ElapsedSec>=Params_Comp_TimeMinOFF)DO_S1=1;
                return;
            }
            else
            {
                //zglos wystapienie alarmu o braku przeplywu glikolu
                bAL_CzP=1;
                return;
            }
        }else return;

    }
    else if(Comp_Min_On[0].ElapsedSec>=Params_Comp_TimeMinON||AL_CZP==1||AL_CZT6==1||AL_PWN1==1)  //sa alarmy, badz pompa ma sie wylaczyc
    {
        //Wyl PC W/W S1
        bHP1_isOn=0;
        DO_S1=0;    //Wyl sprezarki
        if(DO_Z3D1!=bCoCwu) DO_Z3D1=bCoCwu; //nastapila zmiana CO<=>CWU
        if(bOnOff==OFF) DO_Z3D1=0;  //Jeœli wy³¹czamy zupe³nie t¹ pompê ciep³a to zdejmij napiêcie z zaworu trojdroznego
        if (!bHP2_isOn){//jesli druga sprezarka (S2) nie dziala
            DO_PG1=0;   //Wyl pompy glikolu PG1
            DO_PC2=0;   //Wyl pompe cyrkulacyjna PC2
            PG1Wait.Clear; //Zresetuj timer
            bAL_CzP=0;
        }
        return;
    }

}
/**
    @fn int HP2_Turn(int bOnOff) funkcja kontroluje prace pompy ciepla ze sprezarka S2
    @param bool OnOff wlaczenie lub wylaczenie urzadzenia (1=wlaczony)
*/
int HP2_Turn(int bOnOff)
{
    if(AL_CZP==0&&AL_CZT6==0&&AL_PWN2==0&&bOnOff==1)//jesli wlaczymy pompe i nie ma zadnych alarmow
    {
        //zasygnalizuj wlaczenie PC
        bHP2_isOn=1;
        //Wlacz pompe glikolu PG1
        DO_PG1=1;
        // poczekaj na Czujnik przeplywu wg. parametru
        if(PG1Wait.isClear)PG1Wait.Start;
        if(PG1Wait.ElapsedSec>Params_WypPG1)    //minal okres wyprzedzenia PG1 lub zalaczyl sie CzP
        {
            //Sprawdz czy wystapil przeplyw glikolu
            if(DI_CZP==1)
            {
                //Wlacz pompe cyrk. PC2
                DO_PC2=1;
                //Wlacz zawor elektromagnetyczny ZE1
                DO_ZE1=1;
                //Wlacz S2 w obiegu CO
                if (Comp_Min_Off[1].ElapsedSec>=Params_Comp_TimeMinOFF)DO_S2=1;
                return;
            }
            else
            {
                //zglos wystapienie alarmu o braku przeplywu glikolu
                bAL_CzP=1;
                return;
            }
        }else return;

    }
    else if(Comp_Min_On[1].ElapsedSec>=Params_Comp_TimeMinON||AL_CZP==1||AL_CZT6==1||AL_PWN2==1)  //sa alarmy, badz pompa jest wylaczona
    {
        //Wyl PC W/W S2
        bHP2_isOn=0;
        DO_S2=0;    //Wyl sprezarki
        DO_ZE1=0;
        if(!bHP1_isOn){//jesli pierwsza pompa nie dziala
            DO_PG1=0;   //Wyl pompy glikolu PG1 //UWAGA moze byc kiepskie jesli druga pompa dziala
            DO_PC2=0;   //Wyl pompe cyrkulacyjna PC2
            PG1Wait.Clear; //Zresetuj timer
            bAL_CzP=0;  //Wyl trigger alarmu
        }
        return;
    }
}
/**
    @fn int HP3_Turn(int bOnOff,int bCoCwu) funkcja kontroluje prace pompy ciepla ze sprezarka S3
    @param int bOnOff wlaczenie lub wylaczenie urzadzenia (1 = wlaczony)
    @param int bCoCwu przelaczenie miedzy grzaniem CO i CWU
*/
int HP3_Turn(int bOnOff,int bCoCwu)
{
    if(AL_PWN3==0&&bOnOff==ON&&DO_Z3D2!=bCoCwu)//jesli wlaczymy pompe i nie ma zadnych alarmow i nie nast¹pi³a zmiana stanu CO<=>CWU
    {
        //zasygnalizuj wlaczenie PC
        bHP3_isOn=1;
        if(bCoCwu==CO)DO_Z3D2=1; //przelacz zawor trojdrozny w CO = 0
        else DO_Z3D2=0;
        //Wlacz pompe cyrk. PC1
        DO_PC1=1;
        //Wlacz S1
        if (Comp_Min_Off[2].ElapsedSec>=Params_Comp_TimeMinOFF)DO_S3=1;
        return;
    }
    else if(Comp_Min_On[2].ElapsedSec>=Params_Comp_TimeMinON||AL_PWN2==1)   //sa alarmy, badz pompa ma byc wylaczona lub nastapila zmiana statusu CO/CWU
    {
        //Wyl PC P/W S3
        bHP3_isOn=0;
        DO_S3=0;    //Wyl sprezarki
        DO_PC1=0;   //Wyl pompy cyrkulacyjnej PC1
        if(DO_Z3D2==bCoCwu) DO_Z3D2=!bCoCwu; //nastapila zmiana CO<=>CWU
        if(bOnOff==OFF) DO_Z3D2=0;  //Jeœli wy³¹czamy zupe³nie t¹ pompê ciep³a to zdejmij napiêcie z zaworu trojdroznego
        return;
    }
}

// this is the initialization procedure, and will be called once, at startup of the application
void App_Init()  {
    AlgoLib_Init();
    IOManager.Init();

    // initialize parameters array every time a new application is loaded into device
    if (ParVersion != nvmcc_CompileDate)  {
        Params_Sys_Default = 1;
        ParVersion = nvmcc_CompileDate;

        //wyczyszczenie ewentualnych smieci w harmonogramach
        EraseSchedules();

        #ifdef EVENT_HISTORY_MAX_EVENTS
            // reset event history
            UIResetHistory = 1;
        #endif
        // reset wartoœci licznikow pracy przy wgraniu nowego programu
        CounterValue[0]=0;CounterValue[1]=0;CounterValue[2]=0;CounterValue[3]=0;CounterValue[4]=0;CounterValue[5]=0;
        CounterValue[6]=0;CounterValue[7]=0;CounterValue[8]=0;CounterValue[9]=0;
    };

    //Inicjalizacja zmiennych

    //Wyczyszczenie Timerow
    PG1Wait.Clear;
    PasterizationTime.Clear;
    PasterizationTimeOut.Clear;
    //Modbus init speed 9600
    Com2.InitComEx(2,Com2BufferRx[0],ASize(Com2BufferRx),Com2BufferTx[0],ASize(Com2BufferTx));
    Com2.Init(9600,comSerFlag_8N1);

    Comp_Min_Off[0].EarlyStart(Params_Comp_TimeMinOFF*1000);//obejscie, aby od poczatku mo¿liwe by³o szybkie w³¹czenie sprezarek
    Comp_Min_Off[1].EarlyStart(Params_Comp_TimeMinOFF*1000);
    Comp_Min_Off[2].EarlyStart(Params_Comp_TimeMinOFF*1000);

    //Wczytanie CounterValue do licznikow godzin urzadzen
    short i;
    iHPselected=-1; //resetowanie wybranej sprê¿arki
    for (i=0;i<=9;i++)
    {
        WHCounters[i].Load(CounterValue[i]);
    }
};


// this is the control loop procedure, and will be called continuously
void App_Run()  {

   //Params_Sys_Default=0;//DEBUG

    if (Params_Sys_Default == 1)  {
        InitializeParamVal();
        IOManager.LoadDefault();

        #ifdef EVENT_HISTORY_MAX_EVENTS
            EventHistoryLog.ClearHistory();
        #endif

        #ifdef T7dScheduledAction_MaxActions
            Scheduler.Clear();
        #endif
        Params_Sys_Default = 0;

    };

    AlgoLib_Run();
    IOManager.Run();
    //MODBUS MASTER SECTION ON COM2
    Com2.Run();

    if (Com2.Status != ModbusM_Pending)
    {
        // response arrived, get it
        Moc_inwertera = Com2.ReadAnswerVarShort()/10;
        Com2.ReadRequest(SlaveID,Modbus_FnReadInputRegs, 12, 1);
    }
    //END MODBUS SECTION

    //Wyliczenie wartosci temperatur z czujnikow
    NTC_Temp_CO=IntegerToFloat(AI_CZT1,-1);
    NTC_Temp_CWU=IntegerToFloat(AI_CZT4,-1);
    NTC_Temp_Glik=IntegerToFloat(AI_CZT6,-1);
    NTC_Temp_Zewn=IntegerToFloat(AI_CZT3,-1);
    //Wyliczenie estymowanej temperatury w budynku z modelu inercyjnego
    if(bFirstRun<300){Y=BuildingTemperature;bFirstRun++;}; //przez pierwsze tysiac cykli programu wczytuj dane z czujnikow do modelu
    Params_Temp_RealBuilding=FloatToIntegerRound(RealBuildingTemperature)*10;

    //obsluga Harmonogramu
    if(bSchedule_override==0)//jesli nie ma obejscia harmonogramu
    {
        ActiveSchedules = CheckSchedule();
        bTaryfaON=ActiveSchedules&1;
        bPcwuON=ActiveSchedules&2;
        bPasteryzuj=ActiveSchedules&4;
    }
    else //jest obejscie harmonogramu
    {
        bTaryfaON= DI_HTF; //jesli wejscie wysoko to znaczy ze jest II taryfa
        //bPcwuON=DI_HPCWU;//jesli wejscie wysoko to znaczy ze jest czas pracy pompy PCWU
        if(DI_PAS==1)bPasteryzuj=1; //jesli wejscie wysoko to znaczy ze poczatek pasteryzacji

    }

    //Wyliczenie estymowanej temperatury w budynku na podstawie krzywej pogodowej
    if(Temp_Curve_Control==1)Temp_CO=FloatToIntegerRound(COTemperature);
    else Temp_BM=FloatToIntegerRound(BuildingTemperature);
    //Ograniczenie maksymalnej temperatury w zbiorniku CO i CWU
    if(Temp_CO>Params_Max_TempCO)Temp_CO=Params_Max_TempCO;
    if(Temp_CWU>Params_Max_TempCWU)Temp_CWU=Params_Max_TempCWU;
    //Zabezpieczenie aby czas trwania pasteryzacji ktos nie ustawil na wiekszy niz dopuszczalny czas trwania
    if(Params_Pasterization_Time>Params_PasterizationTimeOut)Params_PasterizationTimeOut=Params_Pasterization_Time;
    //Wlaczenie odmierzania timerow minimalnych czasow wl/wyl poszczegolnych sprezarek
    if(DO_S1==0){if(Comp_Min_Off[0].isClear)Comp_Min_Off[0].Start;Comp_Min_On[0].Clear;}
    else{if(Comp_Min_On[0].isClear)Comp_Min_On[0].Start;Comp_Min_Off[0].Clear;}
    if(DO_S2==0){if(Comp_Min_Off[1].isClear)Comp_Min_Off[1].Start;Comp_Min_On[1].Clear;}
    else{if(Comp_Min_On[1].isClear)Comp_Min_On[1].Start;Comp_Min_Off[1].Clear;}
    if(DO_S3==0){if(Comp_Min_Off[2].isClear)Comp_Min_Off[2].Start;Comp_Min_On[2].Clear;}
    else{if(Comp_Min_On[2].isClear)Comp_Min_On[2].Start;Comp_Min_Off[2].Clear;}
    //////////////////////////////////////////////////
    //GLOWNY ALGORYTM WLACZANIA POMP

    if (Params_taryfacwu==0
        ||bTaryfaON==1
        ||Moc_inwertera>=Params_moc_inwertera)//Zalaczamy grzanie CWU
    {
       if (NTC_Temp_CWU<Temp_CWU-Params_Hist_CWU||(NTC_Temp_CWU<Temp_CWU&&Hist_od_CWU==0)) bCwuON=1;
       else {bCwuON=0;}
       if(NTC_Temp_CWU>=Temp_CWU||(NTC_Temp_CWU>Temp_CWU-Params_Hist_CWU&&Hist_od_CWU==1))  Hist_od_CWU=1;
       else {Hist_od_CWU=0;}
    }else {bCwuON=0;Hist_od_CWU=0;}

    if ((Params_Taryfa==0||bTaryfaON==1)
        &&bTurnCO==1)//Zalaczamy grzanie CO
    {
        if (NTC_Temp_CO<Temp_CO-Params_Hist_CO||(NTC_Temp_CO<Temp_CO&&Hist_od_CO==0)) bCoON=1; //ponizej dolnej granicy histerezy lub ponizej nastawionej temp i histereza wylaczona
       else bCoON=0;
       if(NTC_Temp_CO>=Temp_CO||(NTC_Temp_CO>Temp_CO-Params_Hist_CO&&Hist_od_CO==1))  Hist_od_CO=1; //gdy przekroczy nastawiona temp lub dolna granice histerezy i jest w trybie histerezy
       else Hist_od_CO=0;
    }else {bCoON=0;Hist_od_CO=0;}

    if (bPasteryzuj==1&&PasterizationTime.ElapsedSec<=Params_Pasterization_Time*60&&AL_CZT4==0&&AL_PASTIME==0)
    /*jest ustawiona godzina pasteryzacji i nie minal jeszcze wyznaczony czas
    jesli nie ma alarmow od czujnika temperatury i od przekroczenia czasu pasteryzacji*/
    {
        if(PasterizationTimeOut.isClear)PasterizationTimeOut.Start;//wystartuj odliczanie timeoutu pasteryzacji
        if(NTC_Temp_CWU<Params_Max_TempCWU){bCwuON=1;DO_GRZ=0;bPcwuON=0;} //nagrzewanie CWU pompa ciepla do nastawionej temperatury
        else if(NTC_Temp_CWU <=Temp_Pasteryzacji-2)  //histereza 2 stopni
        {
            DO_GRZ=1;    //jesli temp. wieksza badz rowna nastawionej uruchom grzalki az nie osiagniemy temperatury pasteryzacji\
            bCwuON=0;   //wylacz pompe ciep³a
            bPcwuON=0; //wylacz pompe cyrkulacyjna wody
        }
        else if(NTC_Temp_CWU >=Temp_Pasteryzacji)
        {
            DO_GRZ=0;
            bCwuON=0;   //wylacz pompe ciep³a
            bPcwuON=0; //wylacz pompe cyrkulacyjna wody
            if(PasterizationTime.isClear)PasterizationTime.Start;//odmierzanie czasu pasteryzacji
        }else
        {
        bPcwuON=0; //wylacz pompe cyrkulacyjna wody
        }
    }else
    {
        bPasteryzuj=0;
        PasterizationTime.Start;
        PasterizationTime.Clear;
        PasterizationTimeOut.Start;
        PasterizationTimeOut.Clear;
        DO_GRZ=0;
        bPcwuON=1; //obejscie do czasu wlaczenia harmonogramu
    }

    //////////////////////////////////////////
    //ZALACZANIE POMP
    //Pompa Powietrze/Woda
    if (NTC_Temp_Zewn >=Params_Temp_PW
    ||(bHP3_isOn==1
        &&NTC_Temp_Zewn>=Params_Temp_PW-Params_Hist_PW/10) ) //temperatura pozwala na wlaczenie pompy (z histereza 1 stopnia)
    {
        HP1_Turn(OFF,CO);  //wy³ innych pomp
        HP2_Turn(OFF);
        if(bCwuON==1)
            HP3_Turn(ON,CWU); //Priorytet ma przygotowanie CWU
        else if(bCoON==1)
            HP3_Turn(ON,CO); //nie ma zapotrzebowania na CWU i jest na CO, grzej CO
        else
            HP3_Turn(OFF,CO); //nie ma ¿adnego zapotrzebowania
    }else
    {//////////////// Pompy woda/woda
        HP3_Turn(OFF,CO); //wyl pompe P/W
        if(bCwuON==1)
        {   //Priorytet ma przygotowanie CWU,
            HP1_Turn(ON,CWU); // zawsze tylko pierwsza pompa
            HP2_Turn(OFF);
        }
        else
        {
            if(bCoON==1)//jest zapotrzebowanie na CO
            {
                if(BuildingTemperature-NTC_Temp_Zewn>=Params_Temp_Diff)
                {
                    //przy zapotrzebowaniu mocy wiekszym niz moze dac jedna sprezarka, wlaczaj obie
                    //wg modelu przy roznicy temp. zewn i wewn. domu > 40 st. C
                    HP2_Turn(ON);
                    HP1_Turn(ON,CO);
                }
                else
                {
                    //wlaczaj rownomiernie obie sprezarki
                    //iHPselected po to aby zapobiec zbyt czestemu zmienianiu sprezarki
                    if(iHPselected==-1)//nie zosta³a jeszcze wyznaczona zadna sprezarka do pracy
                    {
                        if(WHCounters[0].WorkingHours > WHCounters[1].WorkingHours)iHPselected=1;//S1 przepracowala wiecej godzin niz S2
                        else iHPselected=0; //S2 przepracowala wiecej godzin niz S1

                    }
                    else if(iHPselected==1)//S1 przepracowala wiecej godzin niz S2
                    {
                        HP2_Turn(ON);
                        HP1_Turn(OFF,CO);
                    }
                    else if(iHPselected==0) //S2 przepracowala wiecej godzin niz S1
                    {
                        HP2_Turn(OFF);
                        HP1_Turn(ON,CO);
                    }
                }
            }
            else{ //nie ma ¿adnego zapotrzebowania
            iHPselected=-1;
            HP1_Turn(OFF,CO);
            HP2_Turn(OFF);
            }
        }
    }
    //Zalaczanie dodatkowych pomp
    //Pompa obiegowa cieplej wody uzytkowej
    if(bPcwuON==1)DO_PCWU=1;
    else DO_PCWU=0;
    //Pompa glikolu na poddasze
    if(Temp_Poddasze==1)DO_PG2=1;
    else DO_PG2=0;
    //Pompa obiegowa CO budynku mieszkalnego i gara¿owego
    if(bTurnCO==1&&NTC_Temp_CO>Temp_CO-Params_Temp_PompyObiegoweCO/10){DO_PCOBM=1;DO_PCOBG=1;}
    else {DO_PCOBM=0;DO_PCOBG=0;}
    // Zalaczanie szybkiego biegu wentylatora pompy powietrznej poni¿ej temperatury zewnetrznej
    if(NTC_Temp_Zewn <= Params_temp_zal_szybkiego_went){
        DO_WENT = 1;
    }
    else {
        DO_WENT = 0;
    }


    //KONIEC GLOWNEGO ALGORYTMU
    // unit on when both parameter and digital input are on
    GlobalOn = DI_ON && (Params_Sys_OnOff != 0);
    SysOn.SetStatus(GlobalOn);

    // check alarm condition
    AlarmsManager.SetAlarmCondition(AL_Gen_ID, DI_GA, ToChar(SysOn.GetStatus()), SysOn.tmr.ElapsedSec());
    //Alarm od przeplywu glikolu PG1 - CzP nie wykazuje przeplywu i minal czas ochronny
    AlarmsManager.SetAlarmCondition(AL_CZP_ID,bAL_CzP,false,false);
    //Alarm przeciwzamrozeniowy (AntiFreeze) dla temperatury glikolu
    AlarmsManager.SetAlarmCondition(AL_CZT6_ID,NTC_Temp_Glik<Params_Temp_AFR,false,false);
    //Alarm zbyt wysokiego lub zbyt niskiego cisnienia freonu PWN1
    AlarmsManager.SetAlarmCondition(AL_PWN1_ID,DI_PWN1,false,false);
    //Alarm zbyt wysokiego lub zbyt niskiego cisnienia freonu PWN1
    AlarmsManager.SetAlarmCondition(AL_PWN2_ID,DI_PWN2,false,false);
    //Alarm zbyt wysokiego lub zbyt niskiego cisnienia freonu PWN1
    AlarmsManager.SetAlarmCondition(AL_PWN3_ID,DI_PWN3,false,false);
    //Alarm zbyt wysokiej temperatury pasteryzacji
    AlarmsManager.SetAlarmCondition(AL_CZT4_ID,NTC_Temp_CWU>Temp_Pasteryzacji+10,false,false);
    //Alarm zbyt wysokiej temperatury w zbiorniku CO
    AlarmsManager.SetAlarmCondition(AL_CZT1_ID,NTC_Temp_CO>Params_Max_TempCO,false,false);
    //Alarm przekroczenia dopuszczalnego czasu pasteryzacji
    AlarmsManager.SetAlarmCondition(AL_PASTIME_ID,PasterizationTimeOut.ElapsedSec>=Params_PasterizationTimeOut*60,false,false);


    // check alarm status
    if (AlarmsManager.isAlarm(AL_Gen_ID))  {
        // handle alarm
        //Params_Sys_OnOff = 0;   // turn off unit
    };


    // turn on alarm relay if there are active alarms
    DO_AL = AlarmsManager.ActiveAlarms;
    DO_WR = AlarmsManager.ActiveWarnings;

    WHCounters[0].SetStatus(DO_S1);
    WHCounters[1].SetStatus(DO_S2);
    WHCounters[2].SetStatus(DO_S3);
    WHCounters[3].SetStatus(DO_PG1);
    WHCounters[4].SetStatus(DO_PG2);
    WHCounters[5].SetStatus(DO_PC1);
    WHCounters[6].SetStatus(DO_PC2);
    WHCounters[7].SetStatus(DO_PCOBM);
    WHCounters[8].SetStatus(DO_PCOBG);
    WHCounters[9].SetStatus(DO_PCWU);


    //Zliczanie godzin pracy urzadzen i co 10 min zapisywanie ich w EEpromie
    if ( SaveWH.ElapsedSec() >= 10*60 )  {
        // zapisz w eeprom wszystkie wartosci
        int i;
        for (i=0;i<=9;i++)
        {
            CounterValue[i]=WHCounters[i].Sec;
            //ParamsVal[Time_S1_ID+i]=WHCounters[i].WorkingHours;
        }
        // reset timer
        SaveWH.Start();
    };

};

