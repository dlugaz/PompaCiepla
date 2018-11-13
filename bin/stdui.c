/*

    Standard user interface main application.

*/

#define included_stdui

#define NO_COM  // this is necessary, to avoid double initialization of COM with MT
#define NO_CAN  // this is necessary, to avoid double initialization of CAN with MT

#ifdef DrawValueCR_SINGLELINE
#else
#define DrawValueCR_SINGLELINE  // DrawValueCR does not work on multiple lines, but it will be faster
#endif


#ifdef MMI_APP
#else
#define MMI_APP
#endif


#include "mcxlib.lib"
#include "displaylib.lib"


// declare display and keyboard interface object
TDisplayLed dsp;
TEditKbd kbd;

// declare can access variable
TCANopen Can;


// this is the initialization procedure, and will be called once, at startup of the application
void Algo_Init()  {

    // initialize CAN
    Can.ComPort = 11;
    Can.NodeID = CANgetNodeID();


    dsp.Init();
    kbd.Init();

    // clear display on startup
    dsp.ClearDisplay();
    dsp.UpdateDisplay();

    App_Init();
};


//TTimer LastRefresh;

// this is the control loop procedure, and will be called continuously
void Algo_MainLoop()  {
    kbd.GetKey();

/*
    if (kbd.Key == 0 && UI.CurrFormID == 0 && LastRefresh.Elapsedms() <= 2000)  {
        //Sleep0;   // Release control to another task.
        Suspend(100);
        return;
    };
    LastRefresh.Start();
*/
    //Suspend(100);

    dsp.UpdateDisplay();

    // handle any incoming canopen request
    Can.Run();

    // run application main loop
    App_Run();
};


/**
    Perform default user interface application actions.
    Call this function when you need to stop your user interface application into a local loop.
*/
void App_InWaitLoop()  {
    kbd.GetKey();

    dsp.UpdateDisplay();

    // handle any incoming canopen request
    Can.Run();
};
