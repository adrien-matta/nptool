/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold must2 Raw data                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TCharissaData.h"


ClassImp(TCharissaData)

TCharissaData::TCharissaData()
{
   // Default constructor
   Clear();
}



TCharissaData::~TCharissaData()
{
}



void TCharissaData::Clear()
{
   // DSSSD 1st layer
   // (X,E)
   fCharissa_Layer1_StripXE_DetectorNbr.clear();
   fCharissa_Layer1_StripXE_StripNbr.clear();
   fCharissa_Layer1_StripXE_Energy.clear();
   // (X,T)
   fCharissa_Layer1_StripXT_DetectorNbr.clear();
   fCharissa_Layer1_StripXT_StripNbr.clear();
   fCharissa_Layer1_StripXT_Time.clear();
   // (Y,E)
   fCharissa_Layer1_StripYE_DetectorNbr.clear();
   fCharissa_Layer1_StripYE_StripNbr.clear();
   fCharissa_Layer1_StripYE_Energy.clear();
   // (Y,T)
   fCharissa_Layer1_StripYT_DetectorNbr.clear();
   fCharissa_Layer1_StripYT_StripNbr.clear();
   fCharissa_Layer1_StripYT_Time.clear();

   // DSSSD 2nd layer
   // (X,E)
   fCharissa_Layer2_StripXE_DetectorNbr.clear();
   fCharissa_Layer2_StripXE_StripNbr.clear();
   fCharissa_Layer2_StripXE_Energy.clear();
   // (X,T)
   fCharissa_Layer2_StripXT_DetectorNbr.clear();
   fCharissa_Layer2_StripXT_StripNbr.clear();
   fCharissa_Layer2_StripXT_Time.clear();
   // (Y,E)
   fCharissa_Layer2_StripYE_DetectorNbr.clear();
   fCharissa_Layer2_StripYE_StripNbr.clear();
   fCharissa_Layer2_StripYE_Energy.clear();
   // (Y,T)
   fCharissa_Layer2_StripYT_DetectorNbr.clear();
   fCharissa_Layer2_StripYT_StripNbr.clear();
   fCharissa_Layer2_StripYT_Time.clear();

   // CsI E
   fCharissa_CsIE_DetectorNbr.clear();
   fCharissa_CsIE_CristalNbr.clear();
   fCharissa_CsIE_Energy.clear();
   // CsI T
   fCharissa_CsIT_DetectorNbr.clear();
   fCharissa_CsIT_CristalNbr.clear();
   fCharissa_CsIT_Time.clear();
}



void TCharissaData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX TCharissaData: New Event XXXXXXXXXXXXXXXXX" << endl;

   // DSSSD 1st layer
   // (X,E)
   cout << "Charissa_Layer1_StripXE_Mult = " << fCharissa_Layer1_StripXE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCharissa_Layer1_StripXE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fCharissa_Layer1_StripXE_DetectorNbr[i] << " Strip: " << fCharissa_Layer1_StripXE_StripNbr[i] << " Energy: " << fCharissa_Layer1_StripXE_Energy[i] << endl;
   // (X,T)
   cout << "Charissa_Layer1 StripXT_Mult = " << fCharissa_Layer1_StripXT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCharissa_Layer1_StripXT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fCharissa_Layer1_StripXT_DetectorNbr[i] << " Strip: " << fCharissa_Layer1_StripXT_StripNbr[i] << " Time: " << fCharissa_Layer1_StripXT_Time[i] << endl;
   // (Y,E)
   cout << "Charissa_Layer1 StripYE_Mult = " << fCharissa_Layer1_StripYE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCharissa_Layer1_StripYE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fCharissa_Layer1_StripYE_DetectorNbr[i] << " Strip: " << fCharissa_Layer1_StripYE_StripNbr[i] << " Energy: " << fCharissa_Layer1_StripYE_Energy[i] << endl;
   // (Y,T)
   cout << "Charissa_Layer1 StripYT_Mult = " << fCharissa_Layer1_StripYT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCharissa_Layer1_StripYT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fCharissa_Layer1_StripYT_DetectorNbr[i] << " Strip: " << fCharissa_Layer1_StripYT_StripNbr[i] << " Time: " << fCharissa_Layer1_StripYT_Time[i] << endl;

   // DSSSD 2nd layer
   // (X,E)
   cout << "Charissa_Layer2_StripXE_Mult = " << fCharissa_Layer2_StripXE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCharissa_Layer2_StripXE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fCharissa_Layer2_StripXE_DetectorNbr[i] << " Strip: " << fCharissa_Layer2_StripXE_StripNbr[i] << " Energy: " << fCharissa_Layer2_StripXE_Energy[i] << endl;
   // (X,T)
   cout << "Charissa_Layer2 StripXT_Mult = " << fCharissa_Layer2_StripXT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCharissa_Layer2_StripXT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fCharissa_Layer2_StripXT_DetectorNbr[i] << " Strip: " << fCharissa_Layer2_StripXT_StripNbr[i] << " Time: " << fCharissa_Layer2_StripXT_Time[i] << endl;
   // (Y,E)
   cout << "Charissa_Layer2 StripYE_Mult = " << fCharissa_Layer2_StripYE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCharissa_Layer2_StripYE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fCharissa_Layer2_StripYE_DetectorNbr[i] << " Strip: " << fCharissa_Layer2_StripYE_StripNbr[i] << " Energy: " << fCharissa_Layer2_StripYE_Energy[i] << endl;
   // (Y,T)
   cout << "Charissa_Layer2 StripYT_Mult = " << fCharissa_Layer2_StripYT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCharissa_Layer2_StripYT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fCharissa_Layer2_StripYT_DetectorNbr[i] << " Strip: " << fCharissa_Layer2_StripYT_StripNbr[i] << " Time: " << fCharissa_Layer2_StripYT_Time[i] << endl;

   // CsI
   // Energy
   cout << "Charissa_CsIE_Mult = " << fCharissa_CsIE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCharissa_CsIE_DetectorNbr.size(); i++)
      cout << "Det: " << fCharissa_CsIE_DetectorNbr[i] << " Pad: " << fCharissa_CsIE_CristalNbr[i] << " Energy: " << fCharissa_CsIE_Energy[i] << endl;
   // Time
   cout << "Charissa_CsIT_Mult = " << fCharissa_CsIT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCharissa_CsIT_DetectorNbr.size(); i++)
      cout << "Det: " << fCharissa_CsIT_DetectorNbr[i] << " Pad: " << fCharissa_CsIT_CristalNbr[i] << " Time: " << fCharissa_CsIT_Time[i] << endl;
}
