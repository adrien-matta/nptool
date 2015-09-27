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

#include "TLassaData.h"

ClassImp(TLassaData)

TLassaData::TLassaData()
{
    // Default constructor

    // SSSD
    // Energy
    /*fLassa_ThinSi_StripE_DetectorNbr.clear();
    fLassa_ThinSi_StripE_StripNbr.clear();
    fLassa_ThinSi_StripE_Energy.clear();
    // Time
    fLassa_ThinSi_StripT_DetectorNbr.clear();
    fLassa_ThinSi_StripT_StripNbr.clear();
    fLassa_ThinSi_StripT_Time.clear();*/
    
    // DSSD
    // (X,E)
    fLassa_ThickSi_StripXE_DetectorNbr.clear();
    fLassa_ThickSi_StripXE_StripNbr.clear();
    fLassa_ThickSi_StripXE_Energy.clear();
    // (X,T)
    fLassa_ThickSi_StripXT_DetectorNbr.clear();
    fLassa_ThickSi_StripXT_StripNbr.clear();
    fLassa_ThickSi_StripXT_Time.clear();
    // (Y,E)
    fLassa_ThickSi_StripYE_DetectorNbr.clear();
    fLassa_ThickSi_StripYE_StripNbr.clear();
    fLassa_ThickSi_StripYE_Energy.clear();
    // (Y,T)
    fLassa_ThickSi_StripYT_DetectorNbr.clear();
    fLassa_ThickSi_StripYT_StripNbr.clear();
    fLassa_ThickSi_StripYT_Time.clear();

    // CsI E
    fLassa_CsIE_DetectorNbr.clear();
    fLassa_CsIE_CristalNbr.clear() ;
    fLassa_CsIE_Energy.clear();
    // CsI T
    fLassa_CsIT_DetectorNbr.clear();
    fLassa_CsIT_CristalNbr.clear();
    fLassa_CsIT_Time.clear();
}

TLassaData::~TLassaData()
{}

void TLassaData::Clear()
{
    // SSSD
    // Energy
   /* fLassa_ThinSi_StripE_DetectorNbr.clear();
    fLassa_ThinSi_StripE_StripNbr.clear();
    fLassa_ThinSi_StripE_Energy.clear();
    // Time
    fLassa_ThinSi_StripT_DetectorNbr.clear();
    fLassa_ThinSi_StripT_StripNbr.clear();
    fLassa_ThinSi_StripT_Time.clear();*/
    
    // DSSD
    // (X,E)
    fLassa_ThickSi_StripXE_DetectorNbr.clear();
    fLassa_ThickSi_StripXE_StripNbr.clear();
    fLassa_ThickSi_StripXE_Energy.clear();
    // (X,T)
    fLassa_ThickSi_StripXT_DetectorNbr.clear();
    fLassa_ThickSi_StripXT_StripNbr.clear();
    fLassa_ThickSi_StripXT_Time.clear();
    // (Y,E)
    fLassa_ThickSi_StripYE_DetectorNbr.clear();
    fLassa_ThickSi_StripYE_StripNbr.clear();
    fLassa_ThickSi_StripYE_Energy.clear();
    // (Y,T)
    fLassa_ThickSi_StripYT_DetectorNbr.clear();
    fLassa_ThickSi_StripYT_StripNbr.clear();
    fLassa_ThickSi_StripYT_Time.clear();
    
    // CsI E
    fLassa_CsIE_DetectorNbr.clear();
    fLassa_CsIE_CristalNbr.clear() ;
    fLassa_CsIE_Energy.clear();
    // CsI T
    fLassa_CsIT_DetectorNbr.clear();
    fLassa_CsIT_CristalNbr.clear();
    fLassa_CsIT_Time.clear();

}



void TLassaData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
    
    // SSSD
    /*cout << "Lassa_ThinSi_StripXE_Mult = " << fLassa_ThinSi_StripE_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fLassa_ThinSi_StripE_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fLassa_ThinSi_StripE_DetectorNbr[i] << " Strip: " << fLassa_ThinSi_StripE_StripNbr[i] << " Energy: " << fLassa_ThinSi_StripE_Energy[i] << endl;
    // (X,T)
    cout << "Lassa_ThinSi_StripT_Mult = " << fLassa_ThinSi_StripT_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fLassa_ThinSi_StripT_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fLassa_ThinSi_StripT_DetectorNbr[i] << " Strip: " << fLassa_ThinSi_StripT_StripNbr[i] << " Time: " << fLassa_ThinSi_StripT_Time[i] << endl;
*/
    // DSSD
    // (X,E)
    cout << "Lassa_ThickSi_StripXE_Mult = " << fLassa_ThickSi_StripXE_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fLassa_ThickSi_StripXE_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fLassa_ThickSi_StripXE_DetectorNbr[i] << " Strip: " << fLassa_ThickSi_StripXE_StripNbr[i] << " Energy: " << fLassa_ThickSi_StripXE_Energy[i] << endl;
    // (X,T)
    cout << "Lassa_ThickSi_StripXT_Mult = " << fLassa_ThickSi_StripXT_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fLassa_ThickSi_StripXT_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fLassa_ThickSi_StripXT_DetectorNbr[i] << " Strip: " << fLassa_ThickSi_StripXT_StripNbr[i] << " Time: " << fLassa_ThickSi_StripXT_Time[i] << endl;
    // (Y,E)
    cout << "Lassa_ThickSi_StripYE_Mult = " << fLassa_ThickSi_StripYE_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fLassa_ThickSi_StripYE_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fLassa_ThickSi_StripYE_DetectorNbr[i] << " Strip: " << fLassa_ThickSi_StripYE_StripNbr[i] << " Energy: " << fLassa_ThickSi_StripYE_Energy[i] << endl;
    // (Y,T)
    cout << "Lassa_ThickSi_StripYT_Mult = " << fLassa_ThickSi_StripYT_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fLassa_ThickSi_StripYT_DetectorNbr.size(); i++)
        cout << "DetNbr: " << fLassa_ThickSi_StripYT_DetectorNbr[i] << " Strip: " << fLassa_ThickSi_StripYT_StripNbr[i] << " Time: " << fLassa_ThickSi_StripYT_Time[i] << endl;

    // CsI
    // Energy
    cout << "Lassa_CsIE_Mult = " << fLassa_CsIE_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fLassa_CsIE_DetectorNbr.size(); i++)
        cout << "Det: " << fLassa_CsIE_DetectorNbr[i] << " Pad: " << fLassa_CsIE_CristalNbr[i] << " Energy: " << fLassa_CsIE_Energy[i] << endl;
    // Time
    cout << "Lassa_CsIT_Mult = " << fLassa_CsIT_DetectorNbr.size() << endl;
    for (UShort_t i = 0; i < fLassa_CsIT_DetectorNbr.size(); i++)
        cout << "Det: " << fLassa_CsIT_DetectorNbr[i] << " Pad: " << fLassa_CsIT_CristalNbr[i] << " Time: " << fLassa_CsIT_Time[i] << endl;
}
