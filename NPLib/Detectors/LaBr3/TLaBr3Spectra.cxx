/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : dec 2013                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for LaBr3              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// STL
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>
using namespace std;

// NPL
#include "TLaBr3Spectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif



////////////////////////////////////////////////////////////////////////////////
TLaBr3Spectra::TLaBr3Spectra()
{
  fNumberDetector =  0;
}



////////////////////////////////////////////////////////////////////////////////
TLaBr3Spectra::TLaBr3Spectra(unsigned int NumberOfDetectors)
{
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TLaBr3Spectra : Initalising control spectra for " 
      << NumberOfDetectors << " detectorss" << endl
      << "************************************************" << endl ;

  fNumberDetector = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}




////////////////////////////////////////////////////////////////////////////////
TLaBr3Spectra::~TLaBr3Spectra(){
}



////////////////////////////////////////////////////////////////////////////////
void TLaBr3Spectra::InitRawSpectra()
{
   string name;
   int NbrBin = 512;
   int MinBin = 0;
   int MaxBin = 16384;
   for (unsigned int i = 0; i < fNumberDetector; i++) { // loop on number of detectors
      // LABR3_E_RAW
      name = Form("LABR3_%d_E_RAW", i+1);cout<<Form("LABR3_%d_E_RAW", i+1)<<endl;
      AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "LABR3/RAW/ENERGY");

      // LABR3_T_RAW
      name = Form("LABR3_%d_T_RAW", i+1);
      AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "LABR3/RAW/TIME");
   } // end loop on number of detectors

   // LABR3_E_RAW
   name = "LABR3_E_RAW";
   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBin, MaxBin, "LABR3/RAW/ENERGY");

   // LABR3_T_RAW
   name = "LABR3_T_RAW";
   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBin, MaxBin, "LABR3/RAW/TIME");

   // LABR3_HIT_E_RAW
   name = "LABR3_HITPATTERN_E_RAW";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "LABR3/RAW/ENERGY");

   // LABR3_HIT_T_RAW
   name = "LABR3_HITPATTERN_T_RAW";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "LABR3/RAW/TIME");

   // E_RAW_MULT
   name = "LABR3_E_RAW_MULT";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "LABR3/RAW/MULT");

   // T_RAW_MULT
   name = "LABR3_T_RAW_MULT";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "LABR3/RAW/MULT");
}



////////////////////////////////////////////////////////////////////////////////
void TLaBr3Spectra::InitPreTreatedSpectra()
{
   string name;
   int NbrBin  = 512;
   int MinBinE = 0;
   int MaxBinE = 10;    // MeV
   int MinBinT = 0;
   int MaxBinT = 1;     // us 
 
   for (unsigned int i = 0; i < fNumberDetector; i++) { // loop on number of detectors
      // LABR3_E_CAL
      name = Form("LABR3_%d_E_CAL", i+1);
      AddHisto1D(name, name, NbrBin, MinBinE, MaxBinE, "LABR3/CAL/ENERGY");

      // LABR3_T_CAL
      name = Form("LABR3_%d_T_CAL", i+1);
      AddHisto1D(name, name, NbrBin, MinBinT, MaxBinT, "LABR3/CAL/TIME");

   } // end loop on number of detectors

   // LABR3_E_CAL
   name = "LABR3_ESUM_CAL";
   AddHisto1D(name, name, NbrBin, MinBinE, MaxBinE, "LABR3/CAL/ENERGY");

   // LABR3_E_CAL
   name = "LABR3_E_CAL";
   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBinE, MaxBinE, "LABR3/CAL/ENERGY");

   // LABR3_T_CAL
   name = "LABR3_T_CAL";
   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBinT, MaxBinT, "LABR3/CAL/TIME");

   // LABR3_HIT_E_CAL
   name = "LABR3_HITPATTERN_E_CAL";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "LABR3/CAL/ENERGY");

   // LABR3_HIT_T_CAL
   name = "LABR3_HITPATTERN_T_CAL";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "LABR3/CAL/TIME");

   // E_CAL_MULT
   name = "LABR3_E_CAL_MULT";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "LABR3/CAL/MULT");

   // T_CAL_MULT
   name = "LABR3_T_CAL_MULT";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "LABR3/CAL/MULT");
}



////////////////////////////////////////////////////////////////////////////////
void TLaBr3Spectra::InitPhysicsSpectra()
{
   string name;

   int NbrBin  = 512;
   int MinBinE = 0;
   int MaxBinE = 10;
   int MinBinT = 0;
   int MaxBinT = 1;

   // Energy-Time Correlation
   for (unsigned int i = 0 ; i < fNumberDetector ; i++) { // loop on number of detectors
      name = Form("LABR3_%d_E_TOF", i+1);
      AddHisto2D(name, name, NbrBin, MinBinE, MaxBinE, NbrBin, MinBinT, MaxBinT, "LABR3/PHY"); 
   }

   // E-TOF:
   name = "LABR3_E_TOF";
   AddHisto2D(name, name, NbrBin, MinBinE, MaxBinE, NbrBin, MinBinT, MaxBinT, "LABR3/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TLaBr3Spectra::FillRawSpectra(TLaBr3Data* RawData)
{
   string name;
   string family;
  
   // E_RAW
   for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
      name   = Form("LABR3_%d_E_RAW", RawData->GetENumber(i));
      family = "LABR3/RAW/ENERGY";
      GetHisto(family,name) -> Fill(RawData->GetEEnergy(i));
   }

   // T_RAW
   for (unsigned int i = 0; i < RawData->GetTimeMult(); i++) {
      name   = Form("LABR3_%d_T_RAW", RawData->GetTNumber(i));
      family = "LABR3/RAW/TIME";
      GetHisto(family,name) -> Fill(RawData->GetTTime(i));
   }

   // LABR3_E_RAW
   name = "LABR3_E_RAW";
   family = "LABR3/RAW/ENERGY";
   for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
      GetHisto(family,name) -> Fill(RawData->GetENumber(i), RawData->GetEEnergy(i));
   }

   // LABR3_T_RAW
   name = "LABR3_T_RAW";
   family = "LABR3/RAW/TIME";
   for (unsigned int i = 0; i < RawData->GetTimeMult(); i++) {
      GetHisto(family,name) -> Fill(RawData->GetTNumber(i), RawData->GetTTime(i));
   }

   // LABR3_HIT_E_RAW
   name = "LABR3_HITPATTERN_E_RAW";
   family = "LABR3/RAW/ENERGY";
   for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
      GetHisto(family,name) -> Fill(RawData->GetENumber(i));
   }

   // LABR3_HIT_T_RAW
   name = "LABR3_HITPATTERN_T_RAW";
   family = "LABR3/RAW/TIME";
   for (unsigned int i = 0; i < RawData->GetTimeMult(); i++) {
      GetHisto(family,name) -> Fill(RawData->GetTNumber(i));
   }

   // E_RAW_MULT
   name = "LABR3_E_RAW_MULT";
   family = "LABR3/RAW/MULT";
   GetHisto(family,name) -> Fill(RawData->GetEnergyMult());

   // T_RAW_MULT
   name = "LABR3_T_RAW_MULT";
   family = "LABR3/RAW/MULT";
   GetHisto(family,name) -> Fill(RawData->GetTimeMult());
}



////////////////////////////////////////////////////////////////////////////////
void TLaBr3Spectra::FillPreTreatedSpectra(TLaBr3Data* PreTreatedData)
{
   string name;
   string family;
  
   // E_CAL
   for (unsigned int i = 0; i < PreTreatedData->GetEnergyMult(); i++) {
      name   = Form("LABR3_%d_E_CAL", PreTreatedData->GetENumber(i));
      family = "LABR3/CAL/ENERGY";
      GetHisto(family,name) -> Fill(PreTreatedData->GetEEnergy(i));
   }

   // T_CAL
   for (unsigned int i = 0; i < PreTreatedData->GetTimeMult(); i++) {
      name   = Form("LABR3_%d_T_CAL", PreTreatedData->GetTNumber(i));
      family = "LABR3/CAL/TIME";
      GetHisto(family,name) -> Fill(PreTreatedData->GetTTime(i));
   }

   // LABR3_E_CAL
   name = "LABR3_E_CAL";
   family = "LABR3/CAL/ENERGY";
   for (unsigned int i = 0; i < PreTreatedData->GetEnergyMult(); i++) {
      GetHisto(family,name) -> Fill(PreTreatedData->GetENumber(i), PreTreatedData->GetEEnergy(i));
   }

   // LABR3_T_CAL
   name = "LABR3_T_CAL";
   family = "LABR3/CAL/TIME";
   for (unsigned int i = 0; i < PreTreatedData->GetTimeMult(); i++) {
      GetHisto(family,name) -> Fill(PreTreatedData->GetTNumber(i), PreTreatedData->GetTTime(i));
   }

   // LABR3_HIT_E_CAL
   name = "LABR3_HITPATTERN_E_CAL";
   family = "LABR3/CAL/ENERGY";
   for (unsigned int i = 0; i < PreTreatedData->GetEnergyMult(); i++) {
      GetHisto(family,name) -> Fill(PreTreatedData->GetENumber(i));
   }

   // LABR3_HIT_T_CAL
   name = "LABR3_HITPATTERN_T_CAL";
   family = "LABR3/CAL/TIME";
   for (unsigned int i = 0; i < PreTreatedData->GetTimeMult(); i++) {
      GetHisto(family,name) -> Fill(PreTreatedData->GetTNumber(i));
   }

   // E_CAL_MULT
   name = "LABR3_E_CAL_MULT";
   family = "LABR3/CAL/MULT";
   GetHisto(family,name) -> Fill(PreTreatedData->GetEnergyMult());

   // T_CAL_MULT
   name = "LABR3_T_CAL_MULT";
   family = "LABR3/CAL/MULT";
   GetHisto(family,name) -> Fill(PreTreatedData->GetTimeMult());
}



////////////////////////////////////////////////////////////////////////////////
void TLaBr3Spectra::FillPhysicsSpectra(TLaBr3Physics* Physics)
{
   string name;
   string family = "LABR3/PHY";


   // Energy-Time Correlation
   for (unsigned int i = 0 ; i < Physics->DetectorNumber.size(); i++) { // loop on number of detectors
      name = Form("LABR3_%d_E_TOF", i+1);
      GetHisto(family,name) -> Fill(Physics->Time[i], Physics->Energy[i]);
   }

   // E-TOF:
   name = "LABR3_E_TOF";
   for (unsigned int i = 0 ; i < Physics->DetectorNumber.size(); i++) { // loop on number of detectors
      GetHisto(family,name) -> Fill(Physics->Time[i], Physics->Energy[i]);
   }
}

