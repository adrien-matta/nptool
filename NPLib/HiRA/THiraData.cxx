/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address:                       *
 *                                                                           *
 * Creation Date  : febuary 2015                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Hira Raw data                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * CoHiRAent:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "THiraData.h"

ClassImp(THiraData)

THiraData::THiraData()
{
    // Default constructor

    // SSSD
    // Energy
    fHiRA_ThinSi_StripE_DetectorNbr.clear();
    fHiRA_ThinSi_StripE_StripNbr.clear();
    fHiRA_ThinSi_StripE_Energy.clear();
    // Time
    fHiRA_ThinSi_StripT_DetectorNbr.clear();
    fHiRA_ThinSi_StripT_StripNbr.clear();
    fHiRA_ThinSi_StripT_Time.clear();
    
    // DSSD
    // (X,E)
    fHiRA_ThickSi_StripXE_DetectorNbr.clear();
    fHiRA_ThickSi_StripXE_StripNbr.clear();
    fHiRA_ThickSi_StripXE_Energy.clear();
    // (X,T)
    fHiRA_ThickSi_StripXT_DetectorNbr.clear();
    fHiRA_ThickSi_StripXT_StripNbr.clear();
    fHiRA_ThickSi_StripXT_Time.clear();
    // (Y,E)
    fHiRA_ThickSi_StripYE_DetectorNbr.clear();
    fHiRA_ThickSi_StripYE_StripNbr.clear();
    fHiRA_ThickSi_StripYE_Energy.clear();
    // (Y,T)
    fHiRA_ThickSi_StripYT_DetectorNbr.clear();
    fHiRA_ThickSi_StripYT_StripNbr.clear();
    fHiRA_ThickSi_StripYT_Time.clear();

    // CsI E
    fHiRA_CsIE_DetectorNbr.clear();
    fHiRA_CsIE_CristalNbr.clear() ;
    fHiRA_CsIE_Energy.clear();
    // CsI T
    fHiRA_CsIT_DetectorNbr.clear();
    fHiRA_CsIT_CristalNbr.clear();
    fHiRA_CsIT_Time.clear();
}

THiraData::~THiraData()
{}

void THiraData::Clear()
{
    // SSSD
    // Energy
    fHiRA_ThinSi_StripE_DetectorNbr.clear();
    fHiRA_ThinSi_StripE_StripNbr.clear();
    fHiRA_ThinSi_StripE_Energy.clear();
    // Time
    fHiRA_ThinSi_StripT_DetectorNbr.clear();
    fHiRA_ThinSi_StripT_StripNbr.clear();
    fHiRA_ThinSi_StripT_Time.clear();
    
    // DSSD
    // (X,E)
    fHiRA_ThickSi_StripXE_DetectorNbr.clear();
    fHiRA_ThickSi_StripXE_StripNbr.clear();
    fHiRA_ThickSi_StripXE_Energy.clear();
    // (X,T)
    fHiRA_ThickSi_StripXT_DetectorNbr.clear();
    fHiRA_ThickSi_StripXT_StripNbr.clear();
    fHiRA_ThickSi_StripXT_Time.clear();
    // (Y,E)
    fHiRA_ThickSi_StripYE_DetectorNbr.clear();
    fHiRA_ThickSi_StripYE_StripNbr.clear();
    fHiRA_ThickSi_StripYE_Energy.clear();
    // (Y,T)
    fHiRA_ThickSi_StripYT_DetectorNbr.clear();
    fHiRA_ThickSi_StripYT_StripNbr.clear();
    fHiRA_ThickSi_StripYT_Time.clear();
    
    // CsI E
    fHiRA_CsIE_DetectorNbr.clear();
    fHiRA_CsIE_CristalNbr.clear() ;
    fHiRA_CsIE_Energy.clear();
    // CsI T
    fHiRA_CsIT_DetectorNbr.clear();
    fHiRA_CsIT_CristalNbr.clear();
    fHiRA_CsIT_Time.clear();

}



void THiraData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
    
    // SSSD
    cout << "HiRA_ThinSi_StripXE_Mult = " << fHiRA_ThinSi_StripE_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fHiRA_ThinSi_StripE_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fHiRA_ThinSi_StripE_DetectorNbr[i] << " Strip: " << fHiRA_ThinSi_StripE_StripNbr[i] << " Energy: " << fHiRA_ThinSi_StripE_Energy[i] << endl;
    // (X,T)
    cout << "HiRA_ThinSi_StripT_Mult = " << fHiRA_ThinSi_StripT_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fHiRA_ThinSi_StripT_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fHiRA_ThinSi_StripT_DetectorNbr[i] << " Strip: " << fHiRA_ThinSi_StripT_StripNbr[i] << " Time: " << fHiRA_ThinSi_StripT_Time[i] << endl;

    // DSSD
    // (X,E)
    cout << "HiRA_ThickSi_StripXE_Mult = " << fHiRA_ThickSi_StripXE_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fHiRA_ThickSi_StripXE_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fHiRA_ThickSi_StripXE_DetectorNbr[i] << " Strip: " << fHiRA_ThickSi_StripXE_StripNbr[i] << " Energy: " << fHiRA_ThickSi_StripXE_Energy[i] << endl;
    // (X,T)
    cout << "HiRA_ThickSi_StripXT_Mult = " << fHiRA_ThickSi_StripXT_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fHiRA_ThickSi_StripXT_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fHiRA_ThickSi_StripXT_DetectorNbr[i] << " Strip: " << fHiRA_ThickSi_StripXT_StripNbr[i] << " Time: " << fHiRA_ThickSi_StripXT_Time[i] << endl;
    // (Y,E)
    cout << "HiRA_ThickSi_StripYE_Mult = " << fHiRA_ThickSi_StripYE_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fHiRA_ThickSi_StripYE_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fHiRA_ThickSi_StripYE_DetectorNbr[i] << " Strip: " << fHiRA_ThickSi_StripYE_StripNbr[i] << " Energy: " << fHiRA_ThickSi_StripYE_Energy[i] << endl;
    // (Y,T)
    cout << "HiRA_ThickSi_StripYT_Mult = " << fHiRA_ThickSi_StripYT_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fHiRA_ThickSi_StripYT_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fHiRA_ThickSi_StripYT_DetectorNbr[i] << " Strip: " << fHiRA_ThickSi_StripYT_StripNbr[i] << " Time: " << fHiRA_ThickSi_StripYT_Time[i] << endl;

    // CsI
    // Energy
    cout << "HiRA_CsIE_Mult = " << fHiRA_CsIE_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fHiRA_CsIE_DetectorNbr.size(); i++)
        cout << "Det: " << fHiRA_CsIE_DetectorNbr[i] << " Pad: " << fHiRA_CsIE_CristalNbr[i] << " Energy: " << fHiRA_CsIE_Energy[i] << endl;
    // Time
    cout << "HiRA_CsIT_Mult = " << fHiRA_CsIT_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fHiRA_CsIT_DetectorNbr.size(); i++)
        cout << "Det: " << fHiRA_CsIT_DetectorNbr[i] << " Pad: " << fHiRA_CsIT_CristalNbr[i] << " Time: " << fHiRA_CsIT_Time[i] << endl;
}
