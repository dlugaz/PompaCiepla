/** \file

    Controller user interface


 \par Company:
 ZaTeChS S.C.

 \author Krzysztof D³u¿yñski

 ******************************************************************************

    This is the controller user interface.


 ******************************************************************************
*/


#include "nvmcc_headers.h"
#include "stdui.c"
#include "UILib.c"
#include "math.lib"



#include "AGF_Digital_Input.c"
#include "AGF_Digital_Output.c"
#include "AGF_Analog_Input.c"
#include "AGF_Analog_Output.c"
#include "AGF_resources.c"



#define CMD_TURNOFF         50
#define CMD_TURNON          51
#define CMD_DEFAULTPAR      89
#define RESET_TIMES        100



TTimerSec PeriodicSchedulesCheck;

const String Devices[] = {"Sprezarka S1","Sprezarka S2","Sprezarka S3","Pompa glikolowa PG1","Pompa glikolowa PG2","Pompa cyrk. PC1",
                        "Pompa cyrk. PC2","Pompa C.O. budynku mieszkalnego","Pompa C.O. budynku gospodarczego","Pompa C.W.U."};

/** Funkcje do wysy³ania i odbierania ca³ych tablic z harmonogramem\
**/
void ModbusSendSchedules()
{
    int i;
    //SchedulePoint table[MAXSCHEDULEPOINTS];
    for (i=0;i<=MAXSCHEDULEPOINTS&&i<ARRAY_LENGTH(SchedulePoints);i++)
    {
        CANVar.Set(MySchedules+2*i,SchedulePoints[i].point);
    }
}

void ModbusGetSchedules()
{
    //Get Schedules from main
    int i;
    for (i=0;i<=MAXSCHEDULEPOINTS&&i<ARRAY_LENGTH(SchedulePoints);i++)
    {
        SchedulePoints[i].point=CANVar.Get(MySchedules+2*i);
    }
}

/**
    @fn void DrawFormResetTimes() Obsluga interfejsu przegladania czasow pracy urzadzen i ich resetowania
*/
void DrawFormResetTimes()
{
    int iSelected;
    int iResetCounter;
    if (UI.NewForm)  {
        UI.NewForm = 0;
        kbd.Val=0;
        iSelected=0;
        iResetCounter=0;
        dsp.ClearDisplay();
        }
    dsp.ClearDisplay();
    //aby przeniesc w jednej zmiennej dwie informacje
    iResetCounter=kbd.Val/100;// pierwsza cyfra to licznik wcisniec OK
    iSelected=kbd.Val-iResetCounter*100;    //trzecia cyfra to wybor ekranu
    //wyswietlanie ilosci przepracowanych godzin
    dsp.DrawMultilineTextCR(0,0,Devices[iSelected]);
    //dsp.DrawTextCenter(0,Devices[iSelected]);
    dsp.SetFont(TDisplay_BigFont);// /3600
    dsp.DrawValueCR(3,3,"",CANVar.Get(CountValue+2*iSelected)/3600,6,0); //long uzywa dwoch rejestrow MODBUS dlatego 2*iSelected
    dsp.DrawTextCR(16,3,"h");
    dsp.SetFont(TDisplay_StdFont);
    dsp.DrawTextCenter(7,"3xOK aby zresetowac");
    dsp.DrawValueCR(15,8,"",kbd.Rep,2,0);
    //resetujemy licznik
    if (iResetCounter>=3)
    {kbd.Val-=iResetCounter*100;
    CANVar.Set(CountValue+2*iSelected,0);//reset wartosci w liczniku sekund
    }
    if (kbd.Key == KEY_DOWN&&iSelected<9){kbd.Val+=1;kbd.Val-=iResetCounter*100;}   //przechodzimy do nastepnego urzadzenia
    if (kbd.Key == KEY_UP&&iSelected>0){kbd.Val-=1;kbd.Val-=iResetCounter*100;}  //do poprzedniego urzadzenia
    if (kbd.Key == KEY_ESC)UI.PrevForm();   //wychodzimy
    if (kbd.Key == KEY_OK)kbd.Val+=100; //zliczamy reset

}

/** Funkcja listuj¹ca wszystkie zapisane harmonogramy**/
int ListPosition;
SchedulePoint SchedulePoints[MAXSCHEDULEPOINTS];
String ScheduleName[SCHEDULESNUMBER]={"II Taryfa","Pompa CWU","Pasteryzacja"};
void DrawFormScheduleList()
{
    if (UI.NewForm)  {
        UI.NewForm = 0;
        kbd.Val=0;
        dsp.ClearDisplay();
        }
    char i;char looplimit;
    char ScheduleNameSize;
    ScheduleNameSize=sizeof(ScheduleName)/sizeof(String)-1;
    if(ScheduleNameSize>6)looplimit=6;else looplimit=ScheduleNameSize;
    dsp.ClearDisplay;
    dsp.DrawTextCenter(0,"Harmonogramy :");
    if(kbd.Val>ScheduleNameSize)kbd.Val=ScheduleNameSize;//ograniczenie od góry
    if(kbd.Val<0)kbd.Val=0; //ograniczenie od do³u
    if(kbd.Val>ListPosition+6)ListPosition++; //przesuwanie listy w dó³ jesli sie nie miesci
    if(kbd.Val<ListPosition)ListPosition--; //przesuwanie w górê
    for(i=0;i<=looplimit;i++)
    {
        if(ListPosition+i==kbd.Val)dsp.SetColor(0,1);
        else dsp.SetColor(1,0);
        dsp.DrawTextCR(0,i+1,ScheduleName[ListPosition+i]);
    }

    if(kbd.Key == KEY_DOWN)kbd.Val++;
    if(kbd.Key == KEY_UP) kbd.Val--;
    if (kbd.Key == KEY_ESC) UI.PrevForm();   //wychodzimy
    if(kbd.Key == KEY_ENTER) UI.ShowFormAndKeepHistory(FORM_SCHEDULER_SHOW);
}


/** Funkcja do przegl¹dania i edycji harmonogramów **/
char PositionVert; char PositionHoriz;char SelectedDay;int SelectedPoint;int SelectedSchedule;char RefreshSorted;
String WeekDays[7]={"Pn","Wt","Sr","Cz","Pt","Sb","Nd"};
int sortedlength;
int sorted[MAXSCHEDULEPOINTS];
void DrawFormScheduleShow()
{//w kbd.val numer schedule ktory wyswietlamy
    if (UI.NewForm)  {
        UI.NewForm = 0;
        //kbd.Val=0;
        //SelectedDay=0;
        SelectedPoint=0;
        PositionVert=0;
        RefreshSorted=1;
       }
    dsp.ClearDisplay();
    dsp.DrawTextCenter(0,ScheduleName[kbd.Val]);
    int i;
    for(i=0;i<ARRAY_LENGTH(WeekDays);i++)// Wypisanie wszystkich dni tygodnia
    {
        if(i==SelectedDay)dsp.SetColor(0,1);
        else dsp.SetColor(1,0);
        dsp.DrawTextCR(1+3*i,1,WeekDays[i]);
    }
    dsp.SetColor(1,0);
    // Wyrysowanie linii
    dsp.DrawText(6,17,"0");dsp.DrawText(32,17,"6");dsp.DrawText(56,17,"12");dsp.DrawText(82,17,"18");dsp.DrawText(109,17,"24");
    dsp.DrawLine(9,30,106,1);//linia horyzontalna
    dsp.DrawLine(9,25,1,5);dsp.DrawLine(35,25,1,5);dsp.DrawLine(62,25,1,5);dsp.DrawLine(88,25,1,5);dsp.DrawLine(115,25,1,5);//linie wertykalne co 26.5 px

    int j=0;
    int test=0;
    if(RefreshSorted==1)
    {
        for(i=0;i<ARRAY_LENGTH(sorted);i++)sorted[i]=0;
        ///////////////////////
        //Przygotowanie tablicy do odczytu punktow (sortowanie i wydzielanie pkt dla danego dnia)
        int temp[145];
        sortedlength=0;
        for (i=0;i<=MAXSCHEDULEPOINTS-1;i++)
        {

            test = SchedulePoints[i].CheckDay(SelectedDay);
            //zaznacz w tablicy temp punkty w ktorych cos sie dzieje
            if(SchedulePoints[i].CheckDay(SelectedDay)>0&&SchedulePoints[i].point!=0&&SchedulePoints[i].ReadScheduleNo==kbd.Val)
            {
                temp[SchedulePoints[i].ReadTime]=i+1;//zapisujemy miejsce w schedulepoints posortowane, +1 aby dobrze odczytywalo 0
            }
        }

        for(i=0;i<=ARRAY_LENGTH(temp)-1;i++) //kondensowanie tablicy
        {
            if(temp[i]!=0){sorted[sortedlength]=temp[i]-1;sortedlength++;} //sortedlength = ilosc niezerowych elementow w tablicy
        }

        RefreshSorted=0;
    }
    ////////////////////////////
    // Rysowanie diagramu
    for (i=0;i<=ARRAY_LENGTH(sorted)-1;i++) //wyrysuj na diagramie
    {
        if(SchedulePoints[sorted[i]].ReadAction==1)// znajdujemy pierwszy punkt, kiedy ON
        {
            for (j=1;(j+i)<=ARRAY_LENGTH(sorted)-1;j++) //w nastêpnych punktach szukamy punktu kiedy OFF (0)
            {if (SchedulePoints[sorted[i+j]].ReadAction==0&&
                SchedulePoints[sorted[i+j]].point!=0&&
                SchedulePoints[sorted[i+j]].ReadScheduleNo==kbd.Val)break;}// jeœli znajdziemy to przerwij pêtle
            if(i+j>ARRAY_LENGTH(sorted)-1)break;
            dsp.DrawFillRect(9+FloatToIntegerRound(IntegerToFloat(SchedulePoints[sorted[i]].ReadTime)*0.736),28,
            FloatToIntegerRound(IntegerToFloat(SchedulePoints[sorted[i+j]].ReadTime-SchedulePoints[sorted[i]].ReadTime)*0.736),4); //narysuj prostokat od i do i+j
        }
    }
    ////////////////////////////////
    //Wyswietlenie konkretnych punktów jako tekst i na diagramie oraz ich przewijanie
    String DspHour; //zmienna pomocnicza do wyswietlania godziny
    if(SelectedPoint>=sortedlength) DspHour="Nowy";
    else{
    DspHour.AddFormatNumBase(SchedulePoints[sorted[SelectedPoint]].ReadTime/6,2,'0',10);//wysw godzine
    DspHour+=":";
    DspHour.AddFormatNumBase((SchedulePoints[sorted[SelectedPoint]].ReadTime%6)*10,2,'0',10);//wysw minuty
    }
    //wyswietlenie punktu - godziny i zal/wyl oraz tryb edycji
    if(SelectedPoint>0)dsp.DrawTextCR(5,5,"<-");
    if(PositionVert==1)dsp.SetColor(0,1);
    dsp.DrawTextCenter(5,DspHour);
    dsp.SetColor(1,0);
    if(SelectedPoint<=sortedlength-1)dsp.DrawTextCR(14,5,"->");
    if(SchedulePoints[sorted[SelectedPoint]].ReadAction==1)dsp.DrawTextCenter(6,"Zalacz");
    if(SchedulePoints[sorted[SelectedPoint]].ReadAction==0)dsp.DrawTextCenter(6,"Wylacz");
    // Wskaznik na diagram na punkt wyswietlany
    dsp.DrawText(6+FloatToIntegerRound(IntegerToFloat(SchedulePoints[sorted[SelectedPoint]].ReadTime)*0.736),32,"^");

    ///////////////////////////////////////
    //Obsluga klawiszy
    if(kbd.Key == KEY_DOWN)PositionVert=1;
    if(kbd.Key == KEY_UP)PositionVert=0;
    if(PositionVert==0){
        SelectedPoint=0; //przy zmianie dnia zerujemy wybrany punkt
        if(kbd.Key == KEY_LEFT){SelectedDay--;RefreshSorted=1;}
        if(kbd.Key == KEY_RIGHT){SelectedDay++;RefreshSorted=1;}
        }//wybór dnia
    if(SelectedDay>6)SelectedDay=6;if(SelectedDay<0)SelectedDay=0;
    if(PositionVert==1){
        if(kbd.Key == KEY_LEFT)SelectedPoint--;
        if(kbd.Key == KEY_RIGHT)SelectedPoint++;
        if(kbd.Key == KEY_ENTER){
            if(SelectedPoint>sortedlength-1){//jesli chcemy utworzyc nowy punkt
                for(SelectedSchedule=0;SelectedSchedule<MAXSCHEDULEPOINTS-1;SelectedSchedule++)//przeszukaj wszystkie punkty w poszukiwaniu wolnego
                {if(SchedulePoints[SelectedSchedule].point==0)break;}
            }else SelectedSchedule=sorted[SelectedPoint];//jesli edytujemy to przekaz obecny punkt
            UI.ShowFormAndKeepHistory(FORM_SCHEDULER_EDIT);
            }
        }//wybór godziny
    if(SelectedPoint>sortedlength)SelectedPoint=sortedlength;if(SelectedPoint<0)SelectedPoint=0;//przewijanie SelectedPoint
    if (kbd.Key == KEY_ESC) UI.PrevForm();   //wychodzimy
}
struct TUILibParams :public TStdParamsCompile  {
    // TODO: Add here your application parameters
};

/** Funkcja do edycji poszczególnego punktu w harmonogramie**/
SchedulePoint TempPoint; char CursorPosition;
void DrawFormScheduleEdit()
{
    unsigned char status;unsigned char time;unsigned char day;unsigned char ScheduleNo  // zmienne pomocnicze do przechowywania wybranych wartosci
        if (UI.NewForm)  {
        UI.NewForm = 0;
        CursorPosition = 0;
        TempPoint.point=SchedulePoints[SelectedSchedule].point;
        }

    status=TempPoint.ReadAction;
    time=TempPoint.ReadTime;
    day=TempPoint.ReadDay;
    ScheduleNo = kbd.Val;
    dsp.ClearDisplay;
    dsp.DrawTextCenter(0,"Edycja Punktu");
    String DspHour; //zmienna pomocnicza do wyswietlania godziny
    DspHour.AddFormatNumBase(time/6,2,'0',10);//wysw godzine
    DspHour+=":";
    DspHour.AddFormatNumBase((time%6)*10,2,'0',10);//wysw minuty
    //wyswietlenie punktu - godziny i zal/wyl oraz tryb edycji
    dsp.DrawTextCenter(2,DspHour);
    if(TempPoint.ReadAction==1)dsp.DrawTextCenter(3,"Zalacz");
    if(TempPoint.ReadAction==0)dsp.DrawTextCenter(3,"Wylacz");
    char i;
    for(i=0;i<ARRAY_LENGTH(WeekDays);i++)// Wypisanie wszystkich dni tygodnia wraz z zaznaczeniem ktore sa aktywne
    {
        if(TempPoint.CheckDay(i))dsp.SetColor(0,1);
        else dsp.SetColor(1,0);
        dsp.DrawTextCR(1+3*i,5,WeekDays[i]);
    }
    dsp.SetColor(1,0);
    dsp.DrawTextCR(2,6,"Usun");
    dsp.DrawTextCR(14,6,"Zapisz");


    //Obsluga kursora - jest pod polem które edytujemy
    if (CursorPosition > 10) CursorPosition =0;
    if (CursorPosition <0) CursorPosition =16;
    //1 - Godzina
    if (CursorPosition ==0){
        dsp.DrawTextCR(6,2,">");
        if (kbd.Key == KEY_RIGHT) time++;
        if (kbd.Key == KEY_LEFT) time--;
        if(time>144)time=0;
        if(time<0)time=144;
    }
    // 2 - Zal/Wyl
    if (CursorPosition ==1){
        dsp.DrawTextCR(6,3,">");
        if (kbd.Key == KEY_RIGHT || kbd.Key == KEY_OK || kbd.Key == KEY_LEFT) status= !status;
    }
    // Dzien tygodnia
    if (CursorPosition >=2 && CursorPosition <=8){
        dsp.DrawTextCR(CursorPosition*3-6,5,">");
        if (kbd.Key == KEY_RIGHT || kbd.Key == KEY_OK || kbd.Key == KEY_LEFT)day = day ^ 1<<CursorPosition -2;
    }
    //Zapisanie tymczasowo wybranych wartosci
    TempPoint.WritePoint(time,day,status,ScheduleNo);
    // Usun
    if (CursorPosition == 9){
        dsp.DrawTextCR(1,6,">");
        if (kbd.Key == KEY_OK)
        {
            SchedulePoints[SelectedSchedule].point=0;//usuwamy punkt (wpisujemy 0), przesylamy do main i wychodzimy
            ModbusSendSchedules;
            UI.PrevForm();
        }
    }
    // Zapisz
    if (CursorPosition == 10){
        dsp.DrawTextCR(13,6,">");
        if (kbd.Key == KEY_OK)
        {
            SchedulePoints[SelectedSchedule].point=TempPoint.point;
            ModbusSendSchedules;
            UI.PrevForm();
        }//zapisujemy punkt i wychodzimy

    }
    if (kbd.Key == KEY_UP) CursorPosition--;
    if (kbd.Key == KEY_DOWN) CursorPosition++;


}
persistent TUILibParams UILibParams;



// this is the initialization procedure, and will be called once, at startup of the application
void App_Init()  {
    UILib_Init();
    if (UILibParams.VerifyParams())  {
        // default params have been reloaded
        UILibParams.ValidateParams();

        #ifdef MULTILANGUAGE
            // reset default language
            // NOTE: on MMI this will also reset the application, so we need to validate parameters before executing this command
            SetUILanguage(0);
        #endif

    };
    ModbusGetSchedules();
    //EraseSchedules();
    //Ustaw podstawowe czasy w schedulerze - pocz. i kon. II taryfy, czas pasteryzacji, godziny pracy CWU:
    //SchedulePoints[0][0].WritePoint(0,0b0011111,0);
    /*SchedulePoints[0][1].WritePoint(42,0b0011111,0);
    SchedulePoints[0][2].WritePoint(78,0b0011111,1);
    SchedulePoints[0][3].WritePoint(90,0b0011111,0);
    SchedulePoints[0][4].WritePoint(132,0b0011111,1);
    SchedulePoints[0][5].WritePoint(144,0b0011111,0);*/

};


// this is the user interface main loop procedure, and will be called continuously
void App_Run()  {

    // sleep user interface to allow main application to run faster
    //if (kbd.ActiveKey == 0 && UI.NewForm == 0)
    //    Suspend(500,Suspend_WakeUpOnKey);
    if (UILib_Run() == 0)  {

        if (UI.CurrFormID == FORM_NOCONNECTION)  {
            DrawFormMain(); // stay on main form
        }

        // TODO: Add here your custom forms...
        //Wyswietlanie i resetowanie czasow pracy maszyn
        else if (UI.CurrFormID == RESET_TIMES) {
            DrawFormResetTimes();
        }

        else if (UI.CurrFormID == CMD_DEFAULTPAR)  {
            // reset default parameters
            CANVar.Set(ParamsBaseIndexCAN + Sys_Default_ID,1);
            UI.ShowForm(FORM_OK_DONE);
        }

        else if (UI.CurrFormID == CMD_TURNON)  {
            // turn on unit
            CANVar.Set(ParamsBaseIndexCAN + Sys_OnOff_ID,1);
            UI.ShowForm(FORM_OK_DONE);
        }

        else if (UI.CurrFormID == CMD_TURNOFF)  {
            // turn off unit
            CANVar.Set(ParamsBaseIndexCAN + Sys_OnOff_ID,0);
            UI.ShowForm(FORM_OK_DONE);
        }

    }

    if (UI.CurrFormID != FORM_MAIN && kbd.Key == KEY_ESC)  {
        UI.PrevForm();
    };
    /*////////////////////////////
    // Okresowe sprawdzanie harmonogramu
    if(PeriodicSchedulesCheck.ElapsedSec>=60*2||PeriodicSchedulesCheck.isClear)
    {
        CANVar.Set(MySchedules,CheckSchedule());
        PeriodicSchedulesCheck.Start;
    }*/


};



void DrawFormMain()  {
    // check KEY to enter menu
    if (kbd.Key == KEY_OK)  {
        UI.ShowMenu(FORM_ROOTMENU);
        return;
    };

    if ( CheckNoConnection(ParamsBaseIndexCAN) == IO_NAValue )
        return;

    // TODO: Add here your main form custom drawings

    if (UI.NewForm)  {
        dsp.ClearDisplay();
        UI.NewForm = 0;
    };


    if (kbd.ActiveKey == KEY_RIGHT && kbd.ActiveTime > 2000)  {
        CANVar.Set(ParamsBaseIndexCAN + Sys_OnOff_ID, !Params_Sys_OnOff);
        kbd.Clear();
    };


    if (dsp.isLed())  {
        dsp.ClearDisplay();

        dsp.SetLed(TDisplayLed_Alarm, ToChar(AlarmNotification > 0));
        dsp.SetValB(Temp_CO, 1);
        dsp.SetValA(AI_CZT1, 1);
        dsp.SetUM(TMeasurementUnit_DegC);
    }
    else  {

        #ifdef SUPPORT_RTC
            // read the RTC from the main board
            RTC.RemoteUpdate();
            // show date/time
            String s(20);
            RTC.Explode();
            RTC.AddFormatDate(s,"DD-MM-YY hh:mm");
            dsp.DrawTextCR(0,0,s);
        #endif

        //wyswietlanie ikonek gdy sa alarmy badz wlaczone urzadzenia
        dsp.DrawImageOnOffCR(15,0, imgALARM16, ToChar(AlarmNotification > 0));
        dsp.DrawImageOnOffCR(15,2,imgPUMP16,DO_PG1);
        dsp.DrawImageOnOffCR(15,4,imgCOMPRESSOR16,DO_S1||DO_S2);
        dsp.DrawImageOnOffCR(18,4,imgFAN16,DO_S3);
        dsp.DrawImageOnOffCR(18,2,imgHEATER16,DO_GRZ);
        dsp.SetFont(TDisplay_BigFont);
        if((DO_Z3D1==0&&DO_S1==1)||(DO_S3==1&&DO_Z3D2==1)||DO_S2==1)dsp.DrawTextCR(15,6," CO");
        else if((DO_Z3D1==1&&DO_S1==1)||(DO_S3==1&&DO_Z3D2==0))dsp.DrawTextCR(15,6,"CWU");
       /* if((DO_Z3D1==0||DO_Z3D2==1))dsp.DrawTextCR(15,6," CO");
        else if((DO_Z3D1==1||DO_Z3D2==0))dsp.DrawTextCR(15,6,"CWU");*/
        dsp.SetFont(TDisplay_StdFont);
        dsp.DrawLine(6,12,75,1);
        //wyswietlanie mierzonych temperatur
        dsp.DrawValueCR(1, 2, "C.O.   :", AI_CZT1, 4, 1);dsp.DrawTextCR(13,2,"°C");
        dsp.DrawValueCR(1, 3, "C.W.U. :", AI_CZT4, 4, 1);dsp.DrawTextCR(13,3,"°C");
        //dsp.DrawValueCR(0, 4, "Budynek:", Temp_BM, 3, 0);dsp.DrawTextCR(12,4,"°C");
        //Params_Temp_RealBuilding=FloatToIntegerRound(RealBuildingTemperature()*IntegerToFloat(10,));
        dsp.DrawValueCR(0, 4, "Moc inw :", CANVar.Get(Inverter_Power), 5, 0);dsp.DrawTextCR(14,4,"W");
        dsp.DrawValueCR(1, 5, "Glikol :", AI_CZT6, 4, 1);dsp.DrawTextCR(13,5,"°C");
        dsp.DrawValueCR(1, 6, "Zewn.  :", AI_CZT3, 4, 1);dsp.DrawTextCR(13,6,"°C");
        //dsp.DrawTextCR(0,7,MessageFromMain);

        //CheckSchedule();


    };

};

