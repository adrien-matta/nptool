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


// ROOT
#include "TString.h"
#include "TDirectory.h"
#include "TFile.h"


////////////////////////////////////////////////////////////////////////////////
TCharissaSpectra::TCharissaSpectra()
{
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
   TString name;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
      // DE_STRX_E_RAW
      name = Form("CHA%d_DE_IMPACT_RAW", i+1);
      AddHisto2D(name, name, fStripX, 1, fStripX+1, fStripY, 1, fStripY+1, "CHARISSA/RAW/IMPACT");

      // DE_STRX_E_RAW
      name = Form("CHA%d_DE_STRX_E_RAW", i+1);
      AddHisto2D(name, name, fStripX, 1, fStripX+1, 512, 0, 16384, "CHARISSA/RAW/STRXE");

      // DE_STRY_E_RAW
      name = Form("CHA%d_DE_STRY_E_RAW", i+1);
      AddHisto2D(name, name, fStripY, 1, fStripY+1, 512, 0, 16384, "CHARISSA/RAW/STRYE");

      // E_STRX_E_RAW
      name = Form("CHA%d_E_STRX_E_RAW", i+1);
      AddHisto2D(name, name, fStripX, 1, fStripX+1, 512, 0, 16384, "CHARISSA/RAW/STRXE");

      // E_STRY_E_RAW
      name = Form("CHA%d_E_STRY_E_RAW", i+1);
      AddHisto2D(name, name, fStripY, 1, fStripY+1, 512, 0, 16384, "CHARISSA/RAW/STRYE");

      // CSI_E_RAW
      name = Form("CHA%d_CSI_E_RAW", i+1);
      AddHisto2D(name, name, fCrystalCsI, 1, fCrystalCsI+1, 512, 0, 16384, "CHARISSA/RAW/CSIE");

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
   TString name;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
      // DE_STRX_E_CAL
      name = Form("CHA%d_DE_STRX_E_CAL", i+1);
      AddHisto2D(name, name, fStripX, 1, fStripX+1, 1000, 0, 16000, "CHARISSA/CAL/STRXE");

      // DE_STRY_E_CAL
      name = Form("CHA%d_DE_STRY_E_CAL", i+1);
      AddHisto2D(name, name, fStripY, 1, fStripY+1, 1000, 0, 16000, "CHARISSA/CAL/STRYE");

      // E_STRX_E_CAL
      name = Form("CHA%d_E_STRX_E_CAL", i+1);
      AddHisto2D(name, name, fStripX, 1, fStripX+1, 1000, 0, 16000, "CHARISSA/CAL/STRXE");

      // E_STRY_E_CAL
      name = Form("CHA%d_E_STRY_E_CAL", i+1);
      AddHisto2D(name, name, fStripY, 1, fStripY+1, 1000, 0, 16000, "CHARISSA/CAL/STRYE");


      // CSI_E_CAL
      name = Form("CHA%d_CSI_E_CAL", i+1);
      AddHisto2D(name, name, fCrystalCsI, 1, fCrystalCsI+1, 500, 0, 50, "CHARISSA/CAL/CSIE");

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
   TString name;

   // DE X-Y Impact Matrix
   name = "CHA_DE_IMPACT_MATRIX";
   AddHisto2D(name, name, 500, -150, 150, 500, -150, 150, "CHARISSA/PHY");

   // E X-Y Impact Matrix
   name = "CHA_E_IMPACT_MATRIX";
   AddHisto2D(name, name, 500, -150, 150, 500, -150, 150, "CHARISSA/PHY");

   // DE kinematic line
   name = "CHA_DE_THETA";
   AddHisto2D(name, name, 360, 0, 180, 500, 0, 500, "CHARISSA/PHY");


   // X-Y Energy Correlation
   for (unsigned int i = 0 ; i < fNumberOfTelescope ; i++) { // loop on number of detectors
      // DE
      name = Form("CHA%d_DE_XY_COR", i+1);
      AddHisto2D(name, name, 500, 0, 500, 500, 0, 500, "CHARISSA/PHY"); 

      // En
      name = Form("CHA%d_E_XY_COR", i+1);
      AddHisto2D(name, name, 500, 0, 500, 500, 0, 500, "CHARISSA/PHY"); 
   }

  

   // ID plot detector by detector
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
      // E-TOF:
      name = "CHA_E_TOF";
      //AddHisto2D(name, name,500,0,50,1000,200,1200,"CHARISSA/PHY"); 
      AddHisto2D(name, name,500,0,500,500,0,500,"CHARISSA/PHY");

      // DE-TOF:
      name = "CHA_DE_TOF";
      AddHisto2D(name, name,500,0,500,500,0,500,"CHARISSA/PHY");

      // E-CSIE:
      name = "CHA_E_CSIE";
      AddHisto2D(name, name,500,0,20000,500,0,500,"CHARISSA/PHY");
      
      // DE-CSIE:
      name = "CHA_DE_CSIE";
      AddHisto2D(name, name,500,0,20000,500,0,500,"CHARISSA/PHY");

      // DE-E:
      name = "CHA_DE_E";
      AddHisto2D(name, name,500,0,500,500,0,500,"CHARISSA/PHY");    
   }
}



////////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::FillRawSpectra(TCharissaData* RawData)
{
   TString name;
   TString family;
  
 for (unsigned int i = 0; i < RawData->GetCharissaLayer1StripXEMult(); i++) {
   for (unsigned int j = 0; j < RawData->GetCharissaLayer1StripYEMult(); j++) {
     if(RawData->GetCharissaLayer1StripXEEnergy(i)> 500 &&  RawData->GetCharissaLayer1StripYEEnergy(j)>500){
      name = Form("CHA%d_DE_IMPACT_RAW", RawData->GetCharissaLayer1StripXEDetectorNbr(i));
      family = "CHARISSA/RAW/IMPACT";
      GetHisto(family,name) -> Fill(RawData->GetCharissaLayer1StripXEStripNbr(i), RawData->GetCharissaLayer1StripYEStripNbr(j));
      }
    }
  }
   // DE_STRX_E
   for (unsigned int i = 0; i < RawData->GetCharissaLayer1StripXEMult(); i++) {
      name   = Form("CHA%d_DE_STRX_E_RAW", RawData->GetCharissaLayer1StripXEDetectorNbr(i));
      family = "CHARISSA/RAW/STRXE";
      GetHisto(family,name) -> Fill(RawData->GetCharissaLayer1StripXEStripNbr(i), RawData->GetCharissaLayer1StripXEEnergy(i));
   }

   // DE_STRY_E 
   for (unsigned int i = 0; i < RawData->GetCharissaLayer1StripYEMult(); i++) {
      name   = Form("CHA%d_DE_STRY_E_RAW", RawData->GetCharissaLayer1StripYEDetectorNbr(i));
      family = "CHARISSA/RAW/STRYE";
      GetHisto(family,name) -> Fill(RawData->GetCharissaLayer1StripYEStripNbr(i), RawData->GetCharissaLayer1StripYEEnergy(i));
   }

   // E_STRX_E 
   for (unsigned int i = 0; i < RawData->GetCharissaLayer2StripXEMult(); i++) {
      name   = Form("CHA%d_E_STRX_E_RAW", RawData->GetCharissaLayer2StripXEDetectorNbr(i));
      family = "CHARISSA/RAW/STRXE";
      GetHisto(family,name) -> Fill(RawData->GetCharissaLayer2StripXEStripNbr(i), RawData->GetCharissaLayer2StripXEEnergy(i));
   }

   // E_STRY_E 
   for (unsigned int i = 0; i < RawData->GetCharissaLayer2StripYEMult(); i++) {
      name   = Form("CHA%d_E_STRY_E_RAW", RawData->GetCharissaLayer2StripYEDetectorNbr(i));
      family = "CHARISSA/RAW/STRYE";
      GetHisto(family,name) -> Fill(RawData->GetCharissaLayer2StripYEStripNbr(i), RawData->GetCharissaLayer2StripYEEnergy(i));
   }

   // CSI_E
   for (unsigned int i = 0; i < RawData->GetCharissaCsIEMult(); i++) {
      name   = Form("CHA%d_CSI_E_RAW", RawData->GetCharissaCsIEDetectorNbr(i));
      family = "CHARISSA/RAW/CSIE";
      GetHisto(family,name) -> Fill(RawData->GetCharissaCsIECristalNbr(i), RawData->GetCharissaCsIEEnergy(i));
   }

   // DE_STRX MULT
   int myMULT[fNumberOfTelescope];
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetCharissaLayer1StripXEMult(); i++) myMULT[RawData->GetCharissaLayer1StripXEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_DE_STRX_RAW_MULT", i+1);
      family = "CHARISSA/RAW/MULT";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }

   // DE_STRY MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetCharissaLayer1StripYEMult(); i++) myMULT[RawData->GetCharissaLayer1StripYEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_DE_STRY_RAW_MULT", i+1);
      family = "CHARISSA/RAW/MULT";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }

   // E_STRX MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetCharissaLayer2StripXEMult(); i++) myMULT[RawData->GetCharissaLayer2StripXEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_E_STRX_RAW_MULT", i+1);
      family = "CHARISSA/RAW/MULT";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }

   // E_STRY MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetCharissaLayer2StripYEMult(); i++) myMULT[RawData->GetCharissaLayer2StripYEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_E_STRY_RAW_MULT", i+1);
      family = "CHARISSA/RAW/MULT";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }

   // CSI MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < RawData->GetCharissaCsIEMult();i++) myMULT[RawData->GetCharissaCsIEDetectorNbr(i)-1] += 1;  

   for( unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_CSI_RAW_MULT", i+1);
      family = "CHARISSA/RAW/MULT";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }

}



////////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::FillPreTreatedSpectra(TCharissaData* PreTreatedData)
{
   TString name;
   TString family;
   // DE_STRX_E 
   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer1StripXEMult(); i++) {
      name   = Form("CHA%d_DE_STRX_E_CAL", PreTreatedData->GetCharissaLayer1StripXEDetectorNbr(i));
      family = "CHARISSA/CAL/STRXE";
      GetHisto(family,name) -> Fill(PreTreatedData->GetCharissaLayer1StripXEStripNbr(i), PreTreatedData->GetCharissaLayer1StripXEEnergy(i));
  
} 

   // DE_STRY_E 
   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer1StripYEMult(); i++) {
      name   = Form("CHA%d_DE_STRY_E_CAL", PreTreatedData->GetCharissaLayer1StripYEDetectorNbr(i));
      family = "CHARISSA/CAL/STRYE";
      GetHisto(family,name) -> Fill(PreTreatedData->GetCharissaLayer1StripYEStripNbr(i), PreTreatedData->GetCharissaLayer1StripYEEnergy(i));
   }

   // E_STRX_E 
   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer2StripXEMult(); i++) {
      name   = Form("CHA%d_E_STRX_E_CAL", PreTreatedData->GetCharissaLayer2StripXEDetectorNbr(i));
      family = "CHARISSA/CAL/STRXE";
      GetHisto(family,name) -> Fill(PreTreatedData->GetCharissaLayer2StripXEStripNbr(i), PreTreatedData->GetCharissaLayer2StripXEEnergy(i));
   }

   // E_STRY_E 
   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer2StripYEMult(); i++) {
      name   = Form("CHA%d_E_STRY_E_CAL", PreTreatedData->GetCharissaLayer2StripYEDetectorNbr(i));
      family = "CHARISSA/CAL/STRYE";
      GetHisto(family,name) -> Fill(PreTreatedData->GetCharissaLayer2StripYEStripNbr(i), PreTreatedData->GetCharissaLayer2StripYEEnergy(i));
   }

   // CSI_E
   for (unsigned int i = 0; i < PreTreatedData->GetCharissaCsIEMult(); i++) {
      name   = Form("CHA%d_CSI_E_CAL", PreTreatedData->GetCharissaCsIEDetectorNbr(i));
      family = "CHARISSA/CAL/CSIE";
      GetHisto(family,name) -> Fill(PreTreatedData->GetCharissaCsIECristalNbr(i), PreTreatedData->GetCharissaCsIEEnergy(i));
   }

   // DE_STRX MULT
   int myMULT[fNumberOfTelescope];
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer1StripXEMult(); i++) myMULT[PreTreatedData->GetCharissaLayer1StripXEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_DE_STRX_CAL_MULT", i+1);
      family = "CHARISSA/CAL/MULT";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }

   // DE_STRY MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer1StripYEMult(); i++) myMULT[PreTreatedData->GetCharissaLayer1StripYEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_DE_STRY_CAL_MULT", i+1);
      family = "CHARISSA/CAL/MULT";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }

   // E_STRX MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer2StripXEMult(); i++) myMULT[PreTreatedData->GetCharissaLayer2StripXEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_E_STRX_CAL_MULT", i+1);
      family = "CHARISSA/CAL/MULT";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }

   // E_STRY MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < PreTreatedData->GetCharissaLayer2StripYEMult(); i++) myMULT[PreTreatedData->GetCharissaLayer2StripYEDetectorNbr(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_E_STRY_CAL_MULT", i+1);
      family = "CHARISSA/CAL/MULT";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }

   // CSI MULT
   for (unsigned int i = 0; i < fNumberOfTelescope; i++) myMULT[i] = 0;

   for (unsigned int i = 0; i < PreTreatedData->GetCharissaCsIEMult();i++) myMULT[PreTreatedData->GetCharissaCsIEDetectorNbr(i)-1] += 1;  

   for( unsigned int i = 0; i < fNumberOfTelescope; i++) {
      name   = Form("CHA%d_CSI_CAL_MULT", i+1);
      family = "CHARISSA/CAL/MULT";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }
}



////////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::FillPhysicsSpectra(TCharissaPhysics* Physics)
{
  TString name;
  TString family= "CHARISSA/PHY";
  for(unsigned int i = 0 ; i < Physics->EventMultiplicity; i++){
    double x = Physics->GetPositionOfInteraction(i).x();
    double y = Physics->GetPositionOfInteraction(i).y();
    
    if(Physics->Layer1_Si_E[i]>0){ 
      name = "CHA_DE_IMPACT_MATRIX";
      GetHisto(family,name)-> Fill(x,y);
    }

    if(Physics->Layer2_Si_E[i]>0){ 
      name = "CHA_E_IMPACT_MATRIX";
      GetHisto(family,name)-> Fill(x,y);
    }

    name = "CHA_DE_THETA";
    double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));
    Theta = Theta/deg;

    double TotalE = 0 ;
    if(Physics->Layer1_Si_E[i]>0) TotalE += Physics->Layer1_Si_E[i];
    if(Physics->Layer2_Si_E[i]>0) TotalE += Physics->Layer2_Si_E[i];
    if(Physics->CsI_E[i]>0) TotalE += Physics->CsI_E[i];
    
    GetHisto(family,name)-> Fill(Theta,TotalE); 
    
    // DE-TOF:
    if(Physics->Layer1_Si_E[i]>0 && Physics->Layer2_Si_E[i]< 0 ){
      name = "CHA_DE_TOF";
      GetHisto(family,name)->Fill(Physics->Layer1_Si_E[i],Physics->Layer1_Si_T[i]);
    }
    // DE-E:
    if(Physics->Layer1_Si_E[i]>0 && Physics->Layer2_Si_E[i]> 0 ){
      name = "CHA_DE_E";
      GetHisto(family,name)->Fill(Physics->Layer1_Si_E[i],Physics->Layer2_Si_E[i]);
    }
  
    name = "CHA_DE_CSIE";
    GetHisto(family,name)->Fill(Physics->Layer1_Si_E[i],Physics->CsI_E[i]);

    name = "CHA_E_CSIE";
    GetHisto(family,name)->Fill(Physics->Layer2_Si_E[i],Physics->CsI_E[i]);
    
    // X-Y Corr
    name = Form("CHA%d_DE_XY_COR", Physics->TelescopeNumber[i]);
    GetHisto(family,name)->Fill(Physics->Layer1_Si_EX[i],Physics->Layer1_Si_EY[i]);
    
    // X-Y Corr
    name = Form("CHA%d_E_XY_COR", Physics->TelescopeNumber[i]);
    GetHisto(family,name)->Fill(Physics->Layer2_Si_EX[i],Physics->Layer2_Si_EY[i]);

  }
 
}



////////////////////////////////////////////////////////////////////////////////
TH1* TCharissaSpectra::AddHisto1D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString family){
  // create histo
  TH1 *hist = new TH1D(name, title, nbinsx, xlow, xup);

  vector<TString> index ;
  index.push_back(family);
  index.push_back(name);

  // fill map
  fMapHisto[index]=hist;

  return hist;
}



////////////////////////////////////////////////////////////////////////////////
TH1* TCharissaSpectra::AddHisto2D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString family){
  // create histo
  TH1 *hist = new TH2D(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup);

  vector<TString> index ;
  index.push_back(family);
  index.push_back(name);

  // fill map
  fMapHisto[index]=hist;

  return hist;
}



////////////////////////////////////////////////////////////////////////////////
TH1* TCharissaSpectra::GetHisto(TString family, TString name){
  vector<TString> index;
  index.push_back(family);
  index.push_back(name);
  TH1* histo ; 
  
  try{
    histo = fMapHisto.at(index); 
  }

  catch(const std::out_of_range& oor){
  cout << "ERROR : the folowing Histo has been requested by TCharissaSpectra and does not exist: family:" << family << " name: "  << name << endl ;
  exit(1);
  }

  return histo;
}



////////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::WriteHisto(TString filename)
{
   TFile *f = NULL; 

   if (filename != "VOID") {
      f = new TFile(filename, "RECREATE");
   }

   map< vector<TString>, TH1* >::iterator it;
   for (it=fMapHisto.begin(); it!=fMapHisto.end(); ++it) {
      it->second->Write();
   }

   if (filename != "VOID") {
      f->Close();
      delete f;
   }
}



///////////////////////////////////////////////////////////////////////////////
void TCharissaSpectra::CheckSpectra()
{
   map< vector<TString>, TH1* >::iterator it;
   Color_t ok_color      = kTeal+9;
   Color_t warning_color = kOrange+8;  warning_color *= 1;
   Color_t bad_color     = kRed;       bad_color     *= 1;

   for (it=fMapHisto.begin(); it!=fMapHisto.end(); ++it)
   {
      it->second->SetLineColor(ok_color);
      it->second->SetFillStyle(1001);
      it->second->SetFillColor(ok_color);
   }
}
