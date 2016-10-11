/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre  contact address: lefebvrl@ipno.in2p3.fr     *
 *                                                                           *
 * Creation Date  : June 2011                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds SPEG Raw data                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TSpegDCData.h"


ClassImp(TSpegDCData)

TSpegDCData::TSpegDCData()
{
   // Default constructor
   Clear();
}



TSpegDCData::~TSpegDCData()
{
}



void TSpegDCData::Clear()
{
   // Strips from CDM 11
   fSpeg_DC_Strip11_DetNbr.clear();
   fSpeg_DC_Strip11_StripNbr.clear();
   fSpeg_DC_Strip11_Energy.clear();
   // Strips from CDM 12
   fSpeg_DC_Strip12_DetNbr.clear();
   fSpeg_DC_Strip12_StripNbr.clear();
   fSpeg_DC_Strip12_Energy.clear();
   // Strips from CDM 21
   fSpeg_DC_Strip21_DetNbr.clear();
   fSpeg_DC_Strip21_StripNbr.clear();
   fSpeg_DC_Strip21_Energy.clear();
   // Strips from CDM 22
   fSpeg_DC_Strip22_DetNbr.clear();
   fSpeg_DC_Strip22_StripNbr.clear();
   fSpeg_DC_Strip22_Energy.clear();
   // Efil11
   fSpeg_DC_Efil11_DetNbr.clear();
   fSpeg_DC_Efil11_Energy.clear();
   // Efil12
   fSpeg_DC_Efil12_DetNbr.clear();
   fSpeg_DC_Efil12_Energy.clear();
   // Efil21
   fSpeg_DC_Efil21_DetNbr.clear();
   fSpeg_DC_Efil21_Energy.clear();
   // Efil22
   fSpeg_DC_Efil22_DetNbr.clear();
   fSpeg_DC_Efil22_Energy.clear();

   // Tplfil11
   fSpeg_DC_Tplfil11_DetNbr.clear();
   fSpeg_DC_Tplfil11_Time.clear();
   // Tplfil12
   fSpeg_DC_Tplfil12_DetNbr.clear();
   fSpeg_DC_Tplfil12_Time.clear();
   // Tplfil21
   fSpeg_DC_Tplfil21_DetNbr.clear();
   fSpeg_DC_Tplfil21_Time.clear();
   // Tplfil22
   fSpeg_DC_Tplfil22_DetNbr.clear();
   fSpeg_DC_Tplfil22_Time.clear();

   // TEMPS fil11
   fSpeg_DC_Tfil11_DetNbr.clear();
   fSpeg_DC_Tfil11_Time.clear();
   // TEMPS fil12
   fSpeg_DC_Tfil12_DetNbr.clear();
   fSpeg_DC_Tfil12_Time.clear();
   // TEMPS fil21
   fSpeg_DC_Tfil21_DetNbr.clear();
   fSpeg_DC_Tfil21_Time.clear();
   // TEMPS fil22
   fSpeg_DC_Tfil22_DetNbr.clear();
   fSpeg_DC_Tfil22_Time.clear();
}



void TSpegDCData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // Strips from CDM 11
   cout << "SpegDC_Strip11_Mult = " << fSpeg_DC_Strip11_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Strip11_DetNbr.size(); i++)
      cout << "Det_Strip1: " << fSpeg_DC_Strip11_DetNbr[i] << " Strip_Strip1: " << fSpeg_DC_Strip11_StripNbr[i] << " Energy_Strip11: " << fSpeg_DC_Strip11_Energy[i] << endl;
   // Strips from CDM 12
   cout << "SpegDC_Strip12_Mult = " << fSpeg_DC_Strip12_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Strip12_DetNbr.size(); i++)
      cout << "Det_Strip12: " << fSpeg_DC_Strip12_DetNbr[i] << " Strip_Strip12: " << fSpeg_DC_Strip12_StripNbr[i] << " Energy_Strip12: " << fSpeg_DC_Strip12_Energy[i] << endl;
   // Strips from CDM 21
   cout << "SpegDC_Strip21_Mult = " << fSpeg_DC_Strip21_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Strip21_DetNbr.size(); i++)
      cout << "Det_Strip21: " << fSpeg_DC_Strip21_DetNbr[i] << " Strip_Strip21: " << fSpeg_DC_Strip21_StripNbr[i] << " Energy_Strip21: " << fSpeg_DC_Strip21_Energy[i] << endl;
   // Strips from CDM 22
   cout << "SpegDC_Strip22_Mult = " << fSpeg_DC_Strip22_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Strip22_DetNbr.size(); i++)
      cout << "Det_Strip22: " << fSpeg_DC_Strip22_DetNbr[i] << " Strip_Strip22: " << fSpeg_DC_Strip22_StripNbr[i] << " Energy_Strip22: " << fSpeg_DC_Strip22_Energy[i] << endl;

   // Efil11
   cout << "SpegDC_Efil11_Mult = " << fSpeg_DC_Efil11_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Efil11_DetNbr.size(); i++)
      cout << "DetNbr11: " << fSpeg_DC_Efil11_DetNbr[i] << " Energy11: " << fSpeg_DC_Efil11_Energy[i] << endl;
   // Efil12
   cout << "SpegDC_Efil12_Mult = " << fSpeg_DC_Efil12_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Efil12_DetNbr.size(); i++)
      cout << "DetNbr12: " << fSpeg_DC_Efil12_DetNbr[i] << " Energy12: " << fSpeg_DC_Efil12_Energy[i] << endl;
   // Efil21
   cout << "SpegDC_Efil21_Mult = " << fSpeg_DC_Efil21_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Efil21_DetNbr.size(); i++)
      cout << "DetNbr21: " << fSpeg_DC_Efil21_DetNbr[i] << " Energy21: " << fSpeg_DC_Efil21_Energy[i] << endl;
   // Efil22
   cout << "SpegDC_Efil22_Mult = " << fSpeg_DC_Efil22_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Efil22_DetNbr.size(); i++)
      cout << "DetNbr22: " << fSpeg_DC_Efil22_DetNbr[i] << " Energy22: " << fSpeg_DC_Efil22_Energy[i] << endl;

   // Tplfil11
   cout << "SpegDC_Tplfil11_Mult = " << fSpeg_DC_Tplfil11_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Tplfil11_DetNbr.size(); i++)
      cout << "DetNbr11: " << fSpeg_DC_Tplfil11_DetNbr[i] << " Time11: " << fSpeg_DC_Tplfil11_Time[i] << endl;
   // Tplfil12
   cout << "SpegDC_Tplfil12_Mult = " << fSpeg_DC_Tplfil12_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Tplfil12_DetNbr.size(); i++)
      cout << "DetNbr12: " << fSpeg_DC_Tplfil12_DetNbr[i] << " Time12: " << fSpeg_DC_Tplfil12_Time[i] << endl;
   // Tplfil21
   cout << "SpegDC_Tplfil21_Mult = " << fSpeg_DC_Tplfil21_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Tplfil21_DetNbr.size(); i++)
      cout << "DetNbr21: " << fSpeg_DC_Tplfil21_DetNbr[i] << " Time21: " << fSpeg_DC_Tplfil21_Time[i] << endl;
   // Tplfil22
   cout << "SpegDC_Tplfil22_Mult = " << fSpeg_DC_Tplfil22_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Tplfil22_DetNbr.size(); i++)
      cout << "DetNbr22: " << fSpeg_DC_Tplfil22_DetNbr[i] << " Time22: " << fSpeg_DC_Tplfil22_Time[i] << endl;

   // TEMPS fil11
   cout << "SpegDC_Tfil11_Mult = " << fSpeg_DC_Tfil11_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Tfil11_DetNbr.size(); i++)
      cout << "DetNbr11: " << fSpeg_DC_Tfil11_DetNbr[i] << " Time11: " << fSpeg_DC_Tfil11_Time[i] << endl;
   // TEMPS fil12
   cout << "SpegDC_Tfil12_Mult = " << fSpeg_DC_Tfil12_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Tfil12_DetNbr.size(); i++)
      cout << "DetNbr12: " << fSpeg_DC_Tfil12_DetNbr[i] << " Time12: " << fSpeg_DC_Tfil12_Time[i] << endl;
   // TEMPS fil21
   cout << "SpegDC_Tfil21_Mult = " << fSpeg_DC_Tfil21_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Tfil21_DetNbr.size(); i++)
      cout << "DetNbr21: " << fSpeg_DC_Tfil21_DetNbr[i] << " Time21: " << fSpeg_DC_Tfil21_Time[i] << endl;
   // TEMPS fil22
   cout << "SpegDC_Tfil22_Mult = " << fSpeg_DC_Tfil22_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fSpeg_DC_Tfil22_DetNbr.size(); i++)
      cout << "DetNbr22: " << fSpeg_DC_Tfil22_DetNbr[i] << " Time22: " << fSpeg_DC_Tfil22_Time[i] << endl;
}
