/** \file
 ******************************************************************************
    Standard user interface main application.

    Use the following defines:
    - define MODBUS_THREAD     // start modbus on another thread
    - define NO_LCD_SUPPORT    // disable LCD support
    - define NO_LED_SUPPORT    // disable LED support
    - define LCX_SUPPORT       // enable LCX support
    - define LDS_SUPPORT       // enable LDS support


 *
 *  \par CVS:
 *  \verbatim
 @(#) $Author: DANFOSS\U241621 $
 @(#) $Date: 2012/11/14 13:56:13 $
 @(#) $Source: /CVS/NECOS/swa/nvmlib/stdui.c,v $
 @(#) $Revision: 1.7 $\endverbatim
 ******************************************************************************
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

#ifdef LDS_SUPPORT
#include "mmilds.lib"
TMMILDS lds;    // one instance for every display
#endif

// this is the initialization procedure, and will be called once, at startup of the application
void Algo_Init()  {

    // initialize CAN
    Can.ComPort = 11;
    Can.NodeID = CANgetNodeID();


    dsp.Init();
    kbd.Init();

#ifdef LDS_SUPPORT
    if (isLocalApp()) {
        if (dsp.DisplayType == -1)
            dsp.DisplayType = TDisplay_TypeLCX;
        lds.Init();
    }
#endif

    // clear display on startup
    dsp.ClearDisplay();
    dsp.UpdateDisplay();

    App_Init();
};


//TTimer LastRefresh;

// this is the control loop procedure, and will be called continuously
void Algo_MainLoop()  {

    App_InWaitLoop();

    // run application main loop
    App_Run();
};


/**
    Perform default user interface application actions.
    Call this function when you need to stop your user interface application into a local loop.
*/
void App_InWaitLoop()  {
    kbd.GetKey();

    // Remote keyboard management
    #ifdef LDS_SUPPORT
    if(isLocalApp())
    {
        lds.GetKey();                   // read remote keyboard
        lds.CopyRemoteKeyToLocal(kbd);  // merge remote keyboard with local keyboard
    }
    #endif

    // Adjust for LCX display
    #ifdef LCX_SUPPORT
    if (dsp.DisplayType == 2)
    {
        dsp.AdjustLCX();
    }
    #endif

    dsp.UpdateDisplay();

    // Remote display management
    #ifdef LDS_SUPPORT
    if (isLocalApp())
    {
        lds.CopyDisplayMCXtoLCX(dsp);   // copy local LED display to the remote display
        lds.AdjustLCX();                // adjust local LED display for the MMILDS display
        lds.UpdateRemoteMMILDS();       // update remote display
    }
    #endif

    // handle any incoming canopen request
    Can.Run();
};
