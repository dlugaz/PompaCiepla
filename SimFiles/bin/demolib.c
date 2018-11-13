/*

    Demo library

    This is a usefull library to make simple demo application

    Use the following defines:

    #define MODBUS_THREAD   // start modbus on another thread

*/

#define included_demolib

#ifdef DrawValueCR_SINGLELINE
#else
#define DrawValueCR_SINGLELINE  // DrawValueCR does not work on multiple lines, but it will be faster
#endif

#include "mcxlib.lib"
#include "displaylib.lib"
#include "modbuslib.lib"


// declare display and keyboard interface object
TDisplayLed dsp;
TEditKbd kbd;

// declare can access variable
TCANopen Can;

#ifdef MMI_APP
    // on MMI application, we do not have Com and IO

    // on MMI_APP single task, we do not want LED support
    #define NO_LED_SUPPORT

#else

    #ifdef NO_COM
        // make sure we do not enable MODBUS_THREAD
        #undef MODBUS_THREAD
    #else
        // declare modbus serial communication port
        TModbus Com1;

        #ifdef MODBUS_THREAD
            #include "multitask.lib"

            void stdalgo_ModbusThread(long Param)  {
                while(1)  {
                    Suspend(5000,Suspend_WakeUpOnPacketCom1);
                    Com1.Run();
                };
            };
        #endif

    #endif

    #ifdef NO_FILTERED_IO
    #else
        // use filtered IO
        TFilteredIO IO;
    #endif

#endif


// this is the initialization procedure, and will be called once, at startup of the application
void Algo_Init()  {
    dsp.Init();
    kbd.Init();

    // clear display on startup
    dsp.ClearDisplay();
    dsp.UpdateDisplay();

#ifdef MMI_APP
#else
    // set export table (CANopen Object Dictionary)
    Can.SetExportTable(ExportTable,ModbusTable);

    // load default values
    //Can.NodeID = 1;
    //Can.Init(50000,0);

    #ifdef NO_COM
    #else
        Com1.Ident = Can.NodeID;

        if (Com1.Ident == 0)
            Com1.Ident = 1;
    #endif
#endif

    App_Init();
    UserI_Init();

    #ifdef MODBUS_THREAD
        nvm_thread_create(stdalgo_ModbusThread,0);
    #endif

};


// this is the control loop procedure, and will be called continuously
void Algo_MainLoop()  {
    // 22/11/06: automatic keyboard handling and display update
    kbd.GetKey();
    dsp.UpdateDisplay();

    // handle any incoming canopen request
    Can.Run();

#ifdef MMI_APP
#else
    #ifdef MODBUS_THREAD
        // modbus is managed by a separate thread
    #else
        #ifdef NO_COM
        #else
            // handle any incoming modbus request
            Com1.Run();
        #endif
    #endif
#endif

    // run application main loop
    App_Run();
    UserI_Run();
};


/**
    Perform default user interface application actions.
    Call this function when you need to stop your user interface application into a local loop.
*/
void App_InWaitLoop()  {
    // 22/11/06: automatic keyboard handling and display update
    kbd.GetKey();
    dsp.UpdateDisplay();

    // handle any incoming canopen request
    Can.Run();

#ifdef MMI_APP
#else
    #ifdef MODBUS_THREAD
        // modbus is managed by a separate thread
    #else
        #ifdef NO_COM
        #else
            // handle any incoming modbus request
            Com1.Run();
        #endif
    #endif
#endif
};


default_impl void UserI_Init()  {
};

default_impl void UserI_Run()  {
};
