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
 *  This class holds all the online spectra needed for Charissa              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TCharissaSpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif


// STL
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
TCharissaSpectra::TCharissaSpectra(){
  SetName("Charissa");
  fNumberOfTelescope =  0;
  fStripX            = 16;
  fStripY            = 16;
  fCrystalCsI        =  1;
}



////////////////////////////////////////////////////////////////////////////////
TCharissaSpectra::TCharissaSpectra(unsigned int NumberOfTelescope)
{
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TCharissaSpectra : Initalising control spectra for " 
      << NumberOfTelescope << " Telescopes" << endl
      << "************************************************" << endl ;

  SetName("Charissa");
  fNumberOfTelescope = NumberOfTelescope;
  fStripX            = 16;
  fStripY            = 16;
  fCrystalCsI        =  1;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}




////////////////////////////////////////////////////////////////////////////////
TCharissaSpectra::~TCharissaSpectra(){
}



////////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::InitRawSpectra()
{
   string name;
   int NbrBin = 512;
   int MinBin = 0;
   int MaxBin = 16384;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
      // DE_STRX_E_RAW
      name = Form("CHA%d_DE_IMPACT_RAW", i+1);
      AddHisto2D(name, name, fStripX, 1, fStripX+1, fStripY, 1, fStripY+1, "CHARISSA/RAW/IMPACT");

      // DE_STRX_E_RAW
      name = Form("CHA%d_DE_STRX_E_RAW", i+1);
      AddHisto2D(name, name, fStripX, 1, fStripX+1, NbrBin, MinBin, MaxBin, "CHARISSA/RAW/STRXE");

      // DE_STRY_E_RAW
      name = Form("CHA%d_DE_STRY_E_RAW", i+1);
      AddHisto2D(name, name, fStripY, 1, fStripY+1, NbrBin, MinBin, MaxBin, "CHARISSA/RAW/STRYE");

      // E_STRX_E_RAW
      name = Form("CHA%d_E_STRX_E_RAW", i+1);
      AddHisto2D(name, name, fStripX, 1, fStripX+1, NbrBin, MinBin, MaxBin, "CHARISSA/RAW/STRXE");

      // E_STRY_E_RAW
      name = Form("CHA%d_E_STRY_E_RAW", i+1);
      AddHisto2D(name, name, fStripY, 1, fStripY+1, NbrBin, MinBin, MaxBin, "CHARISSA/RAW/STRYE");

      // CSI_E_RAW
      name = Form("CHA%d_CSI_E_RAW", i+1);
      AddHisto2D(name, name, fCrystalCsI, 1, fCrystalCsI+1, NbrBin, MinBin, MaxBin, "CHARISSA/RAW/CSIE");

      // DE_STRX_RAW_MULT
      name = Form("CHA%d_DE_STRX_RAW_MULT", i+1);
      AddHisto1D(name, name, fStripX, 1, fStripX+1, "CHARISSA/RAW/MULT");

      // DE_STRY_RAW_MULT
      name = Form("CHA%d_DE_STRY_RAW_MULT", i+1);
      AddHisto1D(name, name, fStripX, 1, fStripX+1, "CHARISSA/RAW/MULT");

      // E_STRX_RAW_MULT
      name = Form("CHA%d_E_STRX_RAW_MULT", i+1);
      AddHisto1D(name, name, fStripX, 1, fStripX+1, "CHARISSA/RAW/MULT");

      // E_STRY_RAW_MULT
      name = Form("CHA%d_E_STRY_RAW_MULT", i+1);
      AddHisto1D(name, name, fStripX, 1, fStripX+1, "CHARISSA/RAW/MULT");

      // CSI_RAW_MULT
      name = Form("CHA%d_CSI_RAW_MULT", i+1);
      AddHisto1D(name, name, fCrystalCsI, 1, fCrystalCsI+1, "CHARISSA/RAW/MULT");
   } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::InitPreTreatedSpectra()
{
   string name;
   int NbrBin = 512;
   int MinBin = 0;
   int MaxBin = 1;
 
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
      // DE_STRX_E_CAL
      name = Form("CHA%d_DE_STRX_E_CAL", i+1);
      AddHisto2D(name, name, fStripX, 1, fStripX+1, NbrBin, MinBin, MaxBin, "CHARISSA/CAL/STRXE");

      // DE_STRY_E_CAL
      name = Form("CHA%d_DE_STRY_E_CAL", i+1);
      AddHisto2D(name, name, fStripY, 1, fStripY+1, NbrBin, MinBin, MaxBin, "CHARISSA/CAL/STRYE");

      // E_STRX_E_CAL
      name = Form("CHA%d_E_STRX_E_CAL", i+1);
      AddHisto2D(name, name, fStripX, 1, fStripX+1, NbrBin, MinBin, MaxBin, "CHARISSA/CAL/STRXE");

      // E_STRY_E_CAL
      name = Form("CHA%d_E_STRY_E_CAL", i+1);
      AddHisto2D(name, name, fStripY, 1, fStripY+1, NbrBin, MinBin, MaxBin, "CHARISSA/CAL/STRYE");


      // CSI_E_CAL
      name = Form("CHA%d_CSI_E_CAL", i+1);
      AddHisto2D(name, name, fCrystalCsI, 1, fCrystalCsI+1, MaxBin, MinBin, MaxBin, "CHARISSA/CAL/CSIE");

      // STRX_DE_CAL_MULT
      name = Form("CHA%d_DE_STRX_CAL_MULT", i+1);
      AddHisto1D(name, name, fStripX, 1, fStripX+1, "CHARISSA/CAL/MULT");

      // STRY_DE_CAL_MULT
      name = Form("CHA%d_DE_STRY_CAL_MULT", i+1);
      AddHisto1D(name, name, fStripX, 1, fStripX+1, "CHARISSA/CAL/MULT");

      // STRX_E_CAL_MULT
      name = Form("CHA%d_E_STRX_CAL_MULT", i+1);
      AddHisto1D(name, name, fStripX, 1, fStripX+1, "CHARISSA/CAL/MULT");

      // STRY_E_CAL_MULT
      name = Form("CHA%d_E_STRY_CAL_MULT", i+1);
      AddHisto1D(name, name, fStripX, 1, fStripX+1, "CHARISSA/CAL/MULT");

      // CSI_CAL_MULT
      name = Form("CHA%d_CSI_CAL_MULT", i+1);
      AddHisto1D(name, name, fCrystalCsI, 1, fCrystalCsI+1, "CHARISSA/CAL/MULT");
   }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::InitPhysicsSpectra()
{
   string name;

   // DE X-Y Impact Matrix
   name = "CHA_DE_IMPACT_MATRIX";
   AddHisto2D(name, name, 500, -150, 150, 500, -150, 150, "CHARISSA/PHY");

   // E X-Y Impact Matrix
   name = "CHA_E_IMPACT_MATRIX";
   AddHisto2D(name, name, 500, -150, 150, 500, -150, 150, "CHARISSA/PHY");

   // DE kinematic line
   name = "CHA_DE_THETA";
   AddHisto2D(name, name, 360, 0, 180, 500, 0, 500, "CHARISSA/PHY");

   int NbrBin = 1000;
   int MinBin = 0;
   int MaxBin = 1;
   int MaxBinCsI = 20000; 
   // X-Y Energy Correlation
   for (unsigned int i = 0 ; i < fNumberOfTelescope ; i++) { // loop on number of detectors
      // DE
      name = Form("CHA%d_DE_XY_COR", i+1);
      AddHisto2D(name, name, NbrBin, MinBin, MaxBin, NbrBin, MinBin, MaxBin, "CHARISSA/PHY"); 

      // En
      name = Form("CHA%d_E_XY_COR", i+1);
      AddHisto2D(name, name, NbrBin, MinBin, MaxBin, NbrBin, MinBin, MaxBin, "CHARISSA/PHY"); 
   }

   // ID plot detector by detector
   for (unsigned int i = MinBin; i < fNumberOfTelescope; i++) { // loop on number of detectors
      // E-TOF:
      name = "CHA_E_TOF";
      AddHisto2D(name, name,NbrBin,MinBin,MaxBin,NbrBin,MinBin,MaxBin,"CHARISSA/PHY");

      // DE-TOF:
      name = "CHA_DE_TOF";
      AddHisto2D(name, name,NbrBin,MinBin,MaxBin,NbrBin,MinBin,MaxBin,"CHARISSA/PHY");

      // E-CSIE:
      name = "CHA_E_CSIE";
      AddHisto2D(name, name,NbrBin,MinBin,MaxBinCsI,NbrBin,MinBin,MaxBin,"CHARISSA/PHY");
      
      // DE-CSIE:
      name = "CHA_DE_CSIE";
      AddHisto2D(name, name,NbrBin,MinBin,MaxBinCsI,NbrBin,MinBin,MaxBin,"CHARISSA/PHY");

      // DE-E:
      name = "CHA_DE_E";
      AddHisto2D(name, name,NbrBin,MinBin,MaxBin,NbrBin,MinBin,MaxBin,"CHARISSA/PHY");    
   }
}



////////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::FillRawSpectra(TCharissaData* RawData)
{
   string name;
   string family;
  
 for (unsigned int i = 0; i < RawData->GetCharissaLayer1StripXEMult(); i++) {
   for (unsigned int j = 0; j < RawData->GetCharissaLayer1StripYEMult(); j++) {
     if(RawData->GetCharissaLayer1StripXEEnergy(i)> 500 &&  RawData->GetCharissaLayer1StripYEEnergy(j)>500){
      name = Form("CHA%d_DE_IMPACT_RAW", RawData->GetCharissaLayer1StripXEDetectorNbr(i));
      family = "CHARISSA/RAW/IMPACT";
      FillSpectra(family,name ,RawData->GetCharissaLayer1StripXEStripNbr(i), RawData->GetCharissaLayer1StripYEStripNbr(j));
      }
    }
  }
   // DE_STRX_E
   for (unsigned int i = 0; i < RawData->GetCharissaLayer1StripXEMult(); i++) {
      name   = Form("CHA%d_DE_STRX_E_RAW", RawData->GetCharissaLayer1StripXEDetectorNbr(i));
      family = "CHARISSA/RAW/STRXE";
      FillSpectra(family,name ,RawData->GetCharissaLayer1StripXEStripNbr(i), RawData->GetCharissaLayer1StripXEEnergy(i));
   }

   // DE_STRY_E 
   for (unsigned int i = 0; i < RawData->GetCharissaLayer1StripYEMult(); i++) {
      name   = Form("CHA%d_DE_STRY_E_RAW", RawData->GetCharissaLayer1StripYEDetectorNbr(i));
      family = "CHARISSA/RAW/STRYE";
      FillSpectra(family,name ,RawData->GetCharissaLayer1StripYEStripNbr(i), RawData->GetCharissaLayer1StripYEEnergy(i));
   }

   // E_STRX_E 
   for (unsigned int i = 0; i < RawData->GetCharissaLayer2StripXEMult(); i++) {
      name   = Form("CHA%d_E_STRX_E_RAW", RawData->GetCharissaLayer2StripXEDetectorNbr(i));
      family = "CHARISSA/RAW/STRXE";
      FillSpectra(family,name ,RawData->GetCharissaLayer2StripXEStripNbr(i), RawData->GetCharissaLayer2StripXEEnergy(i));
   }

   // E_STRY_E 
   for (unsigned int i = 0; i < RawData->GetCharissaLayer2StripYEMult(); i++) {
      name   = Form("CHA%d_E_STRY_E_RAW", RawData->GetCharissaLayer2StripYEDetectorNbr(i));
      family = "CHARISSA/RAW/STRYE";
      FillSpectra(family,name ,RawData->GetCharissaLayer2StripYEStripNbr(i), RawData->GetCharissaLayer2StripYEEnergy(i));
   }

   // CSI_E
   for (unsigned int i = 0; i < RawData->GetCharissaCsIEMult(); i++) {
      name   = Form("CHA%d_CSI_E_RAW", RawData->GetCharissaCsIEDetectorNbr(i));
      family = "CHARISSA/RAW/CSIE";
      FillSpectra(family,name ,RawData->GetCharissaCsIECristalNbr(i), RawData->GetCharissaCsIEEnergy(i));
   }

   // DE_STRX MULT
   int myMULT[fNumberOfTelescope];
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetCharissaLayer1StripXEMult(); i++) myMULT[RawData->GetCharissaLayer1StripXEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_DE_STRX_RAW_MULT", i+1);
      family = "CHARISSA/RAW/MULT";
      FillSpectra(family,name ,myMULT[i]);
   }

   // DE_STRY MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetCharissaLayer1StripYEMult(); i++) myMULT[RawData->GetCharissaLayer1StripYEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_DE_STRY_RAW_MULT", i+1);
      family = "CHARISSA/RAW/MULT";
      FillSpectra(family,name ,myMULT[i]);
   }

   // E_STRX MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetCharissaLayer2StripXEMult(); i++) myMULT[RawData->GetCharissaLayer2StripXEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_E_STRX_RAW_MULT", i+1);
      family = "CHARISSA/RAW/MULT";
      FillSpectra(family,name ,myMULT[i]);
   }

   // E_STRY MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetCharissaLayer2StripYEMult(); i++) myMULT[RawData->GetCharissaLayer2StripYEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_E_STRY_RAW_MULT", i+1);
      family = "CHARISSA/RAW/MULT";
      FillSpectra(family,name ,myMULT[i]);
   }

   // CSI MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetCharissaCsIEMult();i++) myMULT[RawData->GetCharissaCsIEDetectorNbr(i)-1] += 1;  

   for( unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_CSI_RAW_MULT", i+1);
      family = "CHARISSA/RAW/MULT";
      FillSpectra(family,name ,myMULT[i]);
   }

}



////////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::FillPreTreatedSpectra(TCharissaData* PreTreatedData)
{
   string name;
   string family;
   // DE_STRX_E 
   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer1StripXEMult(); i++) {
      name   = Form("CHA%d_DE_STRX_E_CAL", PreTreatedData->GetCharissaLayer1StripXEDetectorNbr(i));
      family = "CHARISSA/CAL/STRXE";
      FillSpectra(family,name ,PreTreatedData->GetCharissaLayer1StripXEStripNbr(i), PreTreatedData->GetCharissaLayer1StripXEEnergy(i));
  
} 

   // DE_STRY_E 
   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer1StripYEMult(); i++) {
      name   = Form("CHA%d_DE_STRY_E_CAL", PreTreatedData->GetCharissaLayer1StripYEDetectorNbr(i));
      family = "CHARISSA/CAL/STRYE";
      FillSpectra(family,name ,PreTreatedData->GetCharissaLayer1StripYEStripNbr(i), PreTreatedData->GetCharissaLayer1StripYEEnergy(i));
   }

   // E_STRX_E 
   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer2StripXEMult(); i++) {
      name   = Form("CHA%d_E_STRX_E_CAL", PreTreatedData->GetCharissaLayer2StripXEDetectorNbr(i));
      family = "CHARISSA/CAL/STRXE";
      FillSpectra(family,name ,PreTreatedData->GetCharissaLayer2StripXEStripNbr(i), PreTreatedData->GetCharissaLayer2StripXEEnergy(i));
   }

   // E_STRY_E 
   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer2StripYEMult(); i++) {
      name   = Form("CHA%d_E_STRY_E_CAL", PreTreatedData->GetCharissaLayer2StripYEDetectorNbr(i));
      family = "CHARISSA/CAL/STRYE";
      FillSpectra(family,name ,PreTreatedData->GetCharissaLayer2StripYEStripNbr(i), PreTreatedData->GetCharissaLayer2StripYEEnergy(i));
   }

   // CSI_E
   for (unsigned int i = 0; i < PreTreatedData->GetCharissaCsIEMult(); i++) {
      name   = Form("CHA%d_CSI_E_CAL", PreTreatedData->GetCharissaCsIEDetectorNbr(i));
      family = "CHARISSA/CAL/CSIE";
      FillSpectra(family,name ,PreTreatedData->GetCharissaCsIECristalNbr(i), PreTreatedData->GetCharissaCsIEEnergy(i));
   }

   // DE_STRX MULT
   int myMULT[fNumberOfTelescope];
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer1StripXEMult(); i++) myMULT[PreTreatedData->GetCharissaLayer1StripXEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_DE_STRX_CAL_MULT", i+1);
      family = "CHARISSA/CAL/MULT";
      FillSpectra(family,name ,myMULT[i]);
   }

   // DE_STRY MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer1StripYEMult(); i++) myMULT[PreTreatedData->GetCharissaLayer1StripYEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_DE_STRY_CAL_MULT", i+1);
      family = "CHARISSA/CAL/MULT";
      FillSpectra(family,name ,myMULT[i]);
   }

   // E_STRX MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer2StripXEMult(); i++) myMULT[PreTreatedData->GetCharissaLayer2StripXEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_E_STRX_CAL_MULT", i+1);
      family = "CHARISSA/CAL/MULT";
      FillSpectra(family,name ,myMULT[i]);
   }

   // E_STRY MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer2StripYEMult(); i++) myMULT[PreTreatedData->GetCharissaLayer2StripYEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_E_STRY_CAL_MULT", i+1);
      family = "CHARISSA/CAL/MULT";
      FillSpectra(family,name ,myMULT[i]);
   }

   // CSI MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < PreTreatedData->GetCharissaCsIEMult();i++) myMULT[PreTreatedData->GetCharissaCsIEDetectorNbr(i)-1] += 1;  

   for( unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_CSI_CAL_MULT", i+1);
      family = "CHARISSA/CAL/MULT";
      FillSpectra(family,name ,myMULT[i]);
   }
}



////////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::FillPhysicsSpectra(TCharissaPhysics* Physics)
{
  string name;
  string family= "CHARISSA/PHY";
  for(unsigned int i = 0 ; i < Physics->EventMultiplicity; i++){
    double x = Physics->GetPositionOfInteraction(i).x();
    double y = Physics->GetPositionOfInteraction(i).y();
    
    if(Physics->Layer1_Si_E[i]>0){ 
      name = "CHA_DE_IMPACT_MATRIX";
      FillSpectra(family,name,x,y);
    }

    if(Physics->Layer2_Si_E[i]>0){ 
      name = "CHA_E_IMPACT_MATRIX";
      FillSpectra(family,name,x,y);
    }

    name = "CHA_DE_THETA";
    double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));
    Theta = Theta/deg;

    double TotalE = 0 ;
    if(Physics->Layer1_Si_E[i]>0) TotalE += Physics->Layer1_Si_E[i];
    if(Physics->Layer2_Si_E[i]>0) TotalE += Physics->Layer2_Si_E[i];
    if(Physics->CsI_E[i]>0) TotalE += Physics->CsI_E[i];
    
    FillSpectra(family,name,Theta,TotalE); 
    
    // DE-TOF:
    if(Physics->Layer1_Si_E[i]>0 && Physics->Layer2_Si_E[i]< 0 ){
      name = "CHA_DE_TOF";
      FillSpectra(family,name,Physics->Layer1_Si_E[i],Physics->Layer1_Si_T[i]);
    }
    // DE-E:
    if(Physics->Layer1_Si_E[i]>0 && Physics->Layer2_Si_E[i]> 0 ){
      name = "CHA_DE_E";
      FillSpectra(family,name,Physics->Layer2_Si_E[i],Physics->Layer1_Si_E[i]);
    }
  
    name = "CHA_DE_CSIE";
    FillSpectra(family,name,Physics->CsI_E[i],Physics->Layer1_Si_E[i]);

    name = "CHA_E_CSIE";
    FillSpectra(family,name,Physics->CsI_E[i],Physics->Layer2_Si_E[i]);
    
    // X-Y Corr
    name = Form("CHA%d_DE_XY_COR", Physics->TelescopeNumber[i]);
    FillSpectra(family,name,Physics->Layer1_Si_EX[i],Physics->Layer1_Si_EY[i]);
    
    // X-Y Corr
    name = Form("CHA%d_E_XY_COR", Physics->TelescopeNumber[i]);
    FillSpectra(family,name,Physics->Layer2_Si_EX[i],Physics->Layer2_Si_EY[i]);

  }
 
}

