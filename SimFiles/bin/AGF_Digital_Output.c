﻿//************************************************************************
// File automatically generated by MCXShape v.2.01.00
// -------------------------------------------------
//
// !!Warning:!!
// The amendment of the Code can affect the performance of the application
// ***********************************************************************

//Digital defines list
#define DO_Not_Used                                       VDOValue[0]
#define DO_AL                                             VDOValue[1]
#define DO_WR                                             VDOValue[2]
#define DO_S1                                             VDOValue[3]
#define DO_S2                                             VDOValue[4]
#define DO_S3                                             VDOValue[5]
#define DO_PG1                                            VDOValue[6]
#define DO_PG2                                            VDOValue[7]
#define DO_PC1                                            VDOValue[8]
#define DO_PC2                                            VDOValue[9]
#define DO_PcoBM                                          VDOValue[10]
#define DO_PcoBG                                          VDOValue[11]
#define DO_Pwu                                            VDOValue[12]
#define DO_GRZ                                            VDOValue[13]
#define DO_ZE1                                            VDOValue[14]
#define DO_Z3D1                                           VDOValue[15]
#define DO_Z3D2                                           VDOValue[16]
#define DO_Not_Used_ID                                    0
#define DO_AL_ID                                          1
#define DO_WR_ID                                          2
#define DO_S1_ID                                          3
#define DO_S2_ID                                          4
#define DO_S3_ID                                          5
#define DO_PG1_ID                                         6
#define DO_PG2_ID                                         7
#define DO_PC1_ID                                         8
#define DO_PC2_ID                                         9
#define DO_PcoBM_ID                                       10
#define DO_PcoBG_ID                                       11
#define DO_Pwu_ID                                         12
#define DO_GRZ_ID                                         13
#define DO_ZE1_ID                                         14
#define DO_Z3D1_ID                                        15
#define DO_Z3D2_ID                                        16


#define NUMBER_DO 20
#define NUMBER_USED_DO 15
#define DO_FUNCTION_NUMBER 17


//Digital Output function Table
#ifdef DO_TABLE
const TVDOTable VDOTable[] =
{
            {"NO","----------------"},
            {"AL","Alarm"},
            {"WR","Warning"},
            {"S1","Sprezarka S1 W/W"},
            {"S2","Sprezarka S2 W/W"},
            {"S3","Sprezarka S3 P/W"},
            {"PG1","Pompa Glikolu PG1"},
            {"PG2","Pompa Glikolu PG2"},
            {"PC1","Pompa Obiegowa PC1"},
            {"PC2","Pompa Obiegowa PC2"},
            {"PcoBM","Pompa C.O. Budynek Mieszkalny"},
            {"PcoBG","Pompa C.O. Budynek Gospodarczy"},
            {"Pcwu","Pompa C.W.U."},
            {"GRZ","Grzalki w zbiorniku c.w.u."},
            {"ZE1","Zawor Elektromagnetyczny ZE1"},
            {"Z3D1","Zawor Trojdrozny Z3D1"},
            {"Z3D2","Zawor Trojdrozny Z3D2"},
};


//Digital Output polarity Table
const TVDOPolarity VDOPolTable[]=
{
        {"N.O."},
        {"N.C."},
};
#endif


#ifdef MMI_APP
#else
//Digital Output default Table
const TDODefault DODefault[] =
{
                {0,3},
                {0,4},
                {0,5},
                {0,6},
                {0,7},
                {0,8},
                {0,9},
                {0,15},
                {0,16},
                {0,10},
                {0,11},
                {0,12},
                {0,14},
                {0,13},
                {0,0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
};
#endif
