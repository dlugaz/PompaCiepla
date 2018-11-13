/*

    Standard logic main application.

    Use the following defines:

    #define MODBUS_THREAD   // start modbus on another thread

*/

#define included_stdalgo

#include "mcxlib.lib"
#include "modbuslib.lib"


// declare can access variable
TCANopen Can;

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



// this is the initialization procedure, and will be called once, at startup of the application
void Algo_Init()  {

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

    App_Init();

    #ifdef MODBUS_THREAD
        nvm_thread_create(stdalgo_ModbusThread,0);
    #endif

};


// this is the control loop procedure, and will be called continuously
void Algo_MainLoop()  {

    // handle any incoming canopen request
    Can.Run();

    #ifdef MODBUS_THREAD
        // modbus is managed by a separate thread
    #else
        #ifdef NO_COM
        #else
            // handle any incoming modbus request
            Com1.Run();
        #endif
    #endif

    // run application main loop
    App_Run();
};
