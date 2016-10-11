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
 *  This class holds all the online spectra needed for SiRes              *
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
#include "TSiResSpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif


////////////////////////////////////////////////////////////////////////////////
TSiResSpectra::TSiResSpectra()
{
  fNumberDetector =  0;
}



////////////////////////////////////////////////////////////////////////////////
TSiResSpectra::TSiResSpectra(unsigned int NumberOfDetectors)
{
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TSiResSpectra : Initalising control spectra for " 
      << NumberOfDetectors << " detectorss" << endl
      << "************************************************" << endl ;

  fNumberDetector = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}




////////////////////////////////////////////////////////////////////////////////
TSiResSpectra::~TSiResSpectra(){
}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::InitRawSpectra()
{
   string name;
   int NbrBin = 512;
   int MinBin = 0;
   int MaxBin = 16384;
   for(unsigned int j=0; j<fNumberDetector; j++)
   {
	   for (unsigned int i = 0; i < 4; i++) { // loop on channels
	      // SiRes_E_RAW
	      name = Form("SiRes_%d_%d_E_RAW", j+1, i+1);
	      AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SiRes/RAW/ENERGY");
	   } // end loop on number of detectors

	   //SiRes_E_RAW
	   name = Form("SiRes_%d_E_RAW",j+1);
	   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBin, MaxBin, "SiRes/RAW/ENERGY");

	   // SiRes_E_RAW
	   name = Form("SiRes_%d_EBack_RAW", j+1);
	   AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SiRes/RAW/ENERGY");

	   // SiRes_T_RAW
	   name = Form("SiRes_%d_T_RAW",j+1);
	   AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SiRes/RAW/TIME");

	   // E_RAW_MULT
	   name = Form("SiRes_%d_E_RAW_MULT", j+1);
	   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SiRes/RAW/MULT");

   }
}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::InitPreTreatedSpectra()
{
   string name;
   int NbrBin  = 512;
   int MinBin = 0;
   int MaxBin = 10;    // MeV
  for(unsigned int j=0; j<fNumberDetector; j++)
   {
	   for (unsigned int i = 0; i < 4; i++) { // loop on channels
	      // SiRes_E_RAW
	      name = Form("SiRes_%d_%d_E_CAL", j+1, i+1);
	      AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SiRes/CAL/ENERGY");
	   } // end loop on number of detectors

	   //SiRes_E_RAW
	   name = Form("SiRes_%d_E_CAL",j+1);
	   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBin, MaxBin, "SiRes/CAL/ENERGY");

	   // SiRes_E_RAW
	   name = Form("SiRes_%d_EBack_CAL", j+1);
	   AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SiRes/CAL/ENERGY");

	   // SiRes_T_RAW
	   name = Form("SiRes_%d_T_CAL",j+1);
	   AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SiRes/CAL/TIME");

	   // E_RAW_MULT
	   name = Form("SiRes_%d_E_CAL_MULT", j+1);
	   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SiRes/CAL/MULT");

   }

}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::InitPhysicsSpectra()
{
   string name;

   int NbrBin  = 100;
   int MinBin = 0;
   int MaxBin = 10;
   int MinBinT = 0;
   int MaxBinT = 1;     // us 
   for (unsigned int j = 0; j < fNumberDetector; j++) 
   { 
	   name = Form("SiRes_%d_XY", j+1);
	   AddHisto2D(name, name, NbrBin, MinBin, MaxBin, NbrBin, MinBin, MaxBin, "SiRes/PHY");
	   // EBack-TOF:
	   name = Form("SiRes_%d_EBack_TOF", j+1);
	   AddHisto2D(name, name, NbrBin, MinBin, MaxBin, NbrBin, MinBinT, MaxBinT, "SiRes/PHY");
   }
}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::FillRawSpectra(TSiResData* RawData)
{
   string name;
   string family;
	for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
	      name   = Form("SiRes_%d_%d_E_RAW", RawData->GetEDetectorNumber(i), RawData->GetEChannelNumber(i));
	      family = "SiRes/RAW/ENERGY";
	      GetHisto(family,name) -> Fill(RawData->GetEEnergy(i));

	      name   = Form("SiRes_%d_E_RAW", RawData->GetEDetectorNumber(i));
	      family = "SiRes/RAW/ENERGY";
	      GetHisto(family,name) -> Fill(RawData->GetEChannelNumber(i),RawData->GetEEnergy(i));

	   }
// SiRes_E_RAW
	for (unsigned int i = 0; i < RawData->GetEEnergyBackMult(); i++) {
	   name = Form("SiRes_%d_EBack_RAW",RawData->GetEEnergyBackDetectorNumber(i));
	   family = "SiRes/RAW/ENERGY";
	      GetHisto(family,name) -> Fill(RawData->GetEEnergyBack(i));
	   }

	for (unsigned int i = 0; i < RawData->GetTimeMult(); i++) {
	   name = Form("SiRes_%d_T_RAW",RawData->GetTDetectorNumber(i));
	   family = "SiRes/RAW/TIME";
	      GetHisto(family,name) -> Fill(RawData->GetTTime(i));
	   }
	
	for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
	      name   = Form("SiRes_%d_E_RAW_MULT", RawData->GetEDetectorNumber(i));
	      family = "SiRes/RAW/MULT";
	      GetHisto(family,name) -> Fill(RawData->GetEnergyMult());

	   }

}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::FillPreTreatedSpectra(TSiResData* PreTreatedData)
{
   string name;
   string family;
 
	for (unsigned int i = 0; i < PreTreatedData->GetEnergyMult(); i++) {
	      name   = Form("SiRes_%d_%d_E_CAL", PreTreatedData->GetEDetectorNumber(i), PreTreatedData->GetEChannelNumber(i));
	      family = "SiRes/CAL/ENERGY";
	      GetHisto(family,name) -> Fill(PreTreatedData->GetEEnergy(i));

	      name   = Form("SiRes_%d_E_CAL", PreTreatedData->GetEDetectorNumber(i));
	      family = "SiRes/CAL/ENERGY";
	      GetHisto(family,name) -> Fill(PreTreatedData->GetEChannelNumber(i),PreTreatedData->GetEEnergy(i));

	   }
// SiRes_E_RAW
	for (unsigned int i = 0; i < PreTreatedData->GetEEnergyBackMult(); i++) {
	   name = Form("SiRes_%d_EBack_CAL",PreTreatedData->GetEEnergyBackDetectorNumber(i));
	   family = "SiRes/CAL/ENERGY";
	      GetHisto(family,name) -> Fill(PreTreatedData->GetEEnergyBack(i));
	   }

	for (unsigned int i = 0; i < PreTreatedData->GetTimeMult(); i++) {
	   name = Form("SiRes_%d_T_CAL",PreTreatedData->GetTDetectorNumber(i));
	   family = "SiRes/CAL/TIME";
	      GetHisto(family,name) -> Fill(PreTreatedData->GetTTime(i));
	   }
	
	for (unsigned int i = 0; i < PreTreatedData->GetEnergyMult(); i++) {
	      name   = Form("SiRes_%d_E_CAL_MULT", PreTreatedData->GetEDetectorNumber(i));
	      family = "SiRes/CAL/MULT";
	      GetHisto(family,name) -> Fill(PreTreatedData->GetEnergyMult());

	   }   // E_CAL

}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::FillPhysicsSpectra(TSiResPhysics* Physics)
{
   string name;
   string family = "SiRes/PHY";
	
	for (unsigned int i = 0; i < Physics->x.size(); i++) {
	   name = Form("SiRes_%d_XY",Physics->DetectorNumber[i]);
	      GetHisto(family,name) -> Fill(Physics->x[i],Physics->y[i]);
	   }

	for (unsigned int i = 0; i < Physics->EnergyBack.size(); i++) {
	   name = Form("SiRes_%d_EBack_TOF",Physics->DetectorNumber[i]);
	      GetHisto(family,name) -> Fill(Physics->EnergyBack[i],Physics->Time[i]);
	   }
	

}

