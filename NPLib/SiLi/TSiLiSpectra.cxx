/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
 *  This class holds all the online spectra needed for SiLi              *
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

// NPL
#include "TSiLiSpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif


// ROOT
#include "string.h"
#include "TDirectory.h"
#include "TFile.h"


////////////////////////////////////////////////////////////////////////////////
TSiLiSpectra::TSiLiSpectra()
{
  fNumberDetector =  0;
}



////////////////////////////////////////////////////////////////////////////////
TSiLiSpectra::TSiLiSpectra(unsigned int NumberOfDetectors)
{
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TSiLiSpectra : Initalising control spectra for " 
      << NumberOfDetectors << " detectorss" << endl
      << "************************************************" << endl ;

  fNumberDetector = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}




////////////////////////////////////////////////////////////////////////////////
TSiLiSpectra::~TSiLiSpectra(){
}



////////////////////////////////////////////////////////////////////////////////
void TSiLiSpectra::InitRawSpectra()
{
   string name;
   int NbrBin = 512;
   int MinBin = 0;
   int MaxBin = 16384;

   for (unsigned int i = 0; i < fNumberDetector; i++) { // loop on number of detectors
      // SILI_E_RAW
      name = Form("SILI_%d_E_RAW", i+1);
      AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SILI/RAW/ENERGY");

      // SILI_T_RAW
      name = Form("SILI_%d_T_RAW", i+1);
      AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SILI/RAW/TIME");
   } // end loop on number of detectors

   // SILI_E_RAW
   name = "SILI_E_RAW";
   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBin, MaxBin, "SILI/RAW/ENERGY");

   // SILI_T_RAW
   name = "SILI_T_RAW";
   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBin, MaxBin, "SILI/RAW/TIME");

   // SILI_HIT_E_RAW
   name = "SILI_HITPATTERN_E_RAW";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SILI/RAW/ENERGY");

   // SILI_HIT_T_RAW
   name = "SILI_HITPATTERN_T_RAW";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SILI/RAW/TIME");

   // E_RAW_MULT
   name = "SILI_E_RAW_MULT";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SILI/RAW/MULT");

   // T_RAW_MULT
   name = "SILI_T_RAW_MULT";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SILI/RAW/MULT");
}



////////////////////////////////////////////////////////////////////////////////
void TSiLiSpectra::InitPreTreatedSpectra()
{
   string name;
   int NbrBin  = 512;
   int MinBinE = 0;
   int MaxBinE = 10;    // MeV
   int MinBinT = 0;
   int MaxBinT = 1;     // us 
 
   for (unsigned int i = 0; i < fNumberDetector; i++) { // loop on number of detectors
      // SILI_E_CAL
      name = Form("SILI_%d_E_CAL", i+1);
      AddHisto1D(name, name, NbrBin, MinBinE, MaxBinE, "SILI/CAL/ENERGY");

      // SILI_T_CAL
      name = Form("SILI_%d_T_CAL", i+1);
      AddHisto1D(name, name, NbrBin, MinBinT, MaxBinT, "SILI/CAL/TIME");

   } // end loop on number of detectors

   // SILI_E_CAL
   name = "SILI_ESUM_CAL";
   AddHisto1D(name, name, NbrBin, MinBinE, MaxBinE, "SILI/CAL/ENERGY");

   // SILI_E_CAL
   name = "SILI_E_CAL";
   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBinE, MaxBinE, "SILI/CAL/ENERGY");

   // SILI_T_CAL
   name = "SILI_T_CAL";
   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBinT, MaxBinT, "SILI/CAL/TIME");

   // SILI_HIT_E_CAL
   name = "SILI_HITPATTERN_E_CAL";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SILI/CAL/ENERGY");

   // SILI_HIT_T_CAL
   name = "SILI_HITPATTERN_T_CAL";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SILI/CAL/TIME");

   // E_CAL_MULT
   name = "SILI_E_CAL_MULT";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SILI/CAL/MULT");

   // T_CAL_MULT
   name = "SILI_T_CAL_MULT";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SILI/CAL/MULT");
}



////////////////////////////////////////////////////////////////////////////////
void TSiLiSpectra::InitPhysicsSpectra()
{
   string name;

   int NbrBin  = 512;
   int MinBinE = 0;
   int MaxBinE = 10;
   int MinBinT = 0;
   int MaxBinT = 1;

   // Energy-Time Correlation
   for (unsigned int i = 0 ; i < fNumberDetector ; i++) { // loop on number of detectors
      name = Form("SILI_%d_E_TOF", i+1);
      AddHisto2D(name, name, NbrBin, MinBinE, MaxBinE, NbrBin, MinBinT, MaxBinT, "SILI/PHY"); 
   }

   // E-TOF:
   name = "SILI_E_TOF";
   AddHisto2D(name, name, NbrBin, MinBinE, MaxBinE, NbrBin, MinBinT, MaxBinT, "SILI/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TSiLiSpectra::FillRawSpectra(TSiLiData* RawData)
{
   string name;
   string family;
  
   // E_RAW
   for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
      name   = Form("SILI_%d_E_RAW", RawData->GetENumber(i));
      family = "SILI/RAW/ENERGY";
      GetHisto(family,name) -> Fill(RawData->GetEEnergy(i));
   }

   // T_RAW
   for (unsigned int i = 0; i < RawData->GetTimeMult(); i++) {
      name   = Form("SILI_%d_T_RAW", RawData->GetTNumber(i));
      family = "SILI/RAW/TIME";
      GetHisto(family,name) -> Fill(RawData->GetTTime(i));
   }

   // SILI_E_RAW
   name = "SILI_E_RAW";
   family = "SILI/RAW/ENERGY";
   for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
      GetHisto(family,name) -> Fill(RawData->GetENumber(i), RawData->GetEEnergy(i));
   }

   // SILI_T_RAW
   name = "SILI_T_RAW";
   family = "SILI/RAW/TIME";
   for (unsigned int i = 0; i < RawData->GetTimeMult(); i++) {
      GetHisto(family,name) -> Fill(RawData->GetTNumber(i), RawData->GetTTime(i));
   }

   // SILI_HIT_E_RAW
   name = "SILI_HITPATTERN_E_RAW";
   family = "SILI/RAW/ENERGY";
   for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
      GetHisto(family,name) -> Fill(RawData->GetENumber(i));
   }

   // SILI_HIT_T_RAW
   name = "SILI_HITPATTERN_T_RAW";
   family = "SILI/RAW/TIME";
   for (unsigned int i = 0; i < RawData->GetTimeMult(); i++) {
      GetHisto(family,name) -> Fill(RawData->GetTNumber(i));
   }

   // E_RAW_MULT
   name = "SILI_E_RAW_MULT";
   family = "SILI/RAW/MULT";
   GetHisto(family,name) -> Fill(RawData->GetEnergyMult());

   // T_RAW_MULT
   name = "SILI_T_RAW_MULT";
   family = "SILI/RAW/MULT";
   GetHisto(family,name) -> Fill(RawData->GetTimeMult());
}



////////////////////////////////////////////////////////////////////////////////
void TSiLiSpectra::FillPreTreatedSpectra(TSiLiData* PreTreatedData)
{
   string name;
   string family;
  
   // E_CAL
   for (unsigned int i = 0; i < PreTreatedData->GetEnergyMult(); i++) {
      name   = Form("SILI_%d_E_CAL", PreTreatedData->GetENumber(i));
      family = "SILI/CAL/ENERGY";
      GetHisto(family,name) -> Fill(PreTreatedData->GetEEnergy(i));
   }

   // T_CAL
   for (unsigned int i = 0; i < PreTreatedData->GetTimeMult(); i++) {
      name   = Form("SILI_%d_T_CAL", PreTreatedData->GetTNumber(i));
      family = "SILI/CAL/TIME";
      GetHisto(family,name) -> Fill(PreTreatedData->GetTTime(i));
   }

   // SILI_E_CAL
   name = "SILI_E_CAL";
   family = "SILI/CAL/ENERGY";
   for (unsigned int i = 0; i < PreTreatedData->GetEnergyMult(); i++) {
      GetHisto(family,name) -> Fill(PreTreatedData->GetENumber(i), PreTreatedData->GetEEnergy(i));
   }

   // SILI_T_CAL
   name = "SILI_T_CAL";
   family = "SILI/CAL/TIME";
   for (unsigned int i = 0; i < PreTreatedData->GetTimeMult(); i++) {
      GetHisto(family,name) -> Fill(PreTreatedData->GetTNumber(i), PreTreatedData->GetTTime(i));
   }

   // SILI_HIT_E_CAL
   name = "SILI_HITPATTERN_E_CAL";
   family = "SILI/CAL/ENERGY";
   for (unsigned int i = 0; i < PreTreatedData->GetEnergyMult(); i++) {
      GetHisto(family,name) -> Fill(PreTreatedData->GetENumber(i));
   }

   // SILI_HIT_T_CAL
   name = "SILI_HITPATTERN_T_CAL";
   family = "SILI/CAL/TIME";
   for (unsigned int i = 0; i < PreTreatedData->GetTimeMult(); i++) {
      GetHisto(family,name) -> Fill(PreTreatedData->GetTNumber(i));
   }

   // E_CAL_MULT
   name = "SILI_E_CAL_MULT";
   family = "SILI/CAL/MULT";
   GetHisto(family,name) -> Fill(PreTreatedData->GetEnergyMult());

   // T_CAL_MULT
   name = "SILI_T_CAL_MULT";
   family = "SILI/CAL/MULT";
   GetHisto(family,name) -> Fill(PreTreatedData->GetTimeMult());
}



////////////////////////////////////////////////////////////////////////////////
void TSiLiSpectra::FillPhysicsSpectra(TSiLiPhysics* Physics)
{
   string name;
   string family = "SILI/PHY";


   // Energy-Time Correlation
   for (unsigned int i = 0 ; i < Physics->DetectorNumber.size(); i++) { // loop on number of detectors
      name = Form("SILI_%d_E_TOF", i+1);
      GetHisto(family,name) -> Fill(Physics->Time[i], Physics->Energy[i]);
   }

   // E-TOF:
   name = "SILI_E_TOF";
   for (unsigned int i = 0 ; i < Physics->DetectorNumber.size(); i++) { // loop on number of detectors
      GetHisto(family,name) -> Fill(Physics->Time[i], Physics->Energy[i]);
   }
}

