/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 ****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                  B. Le Crom                        lecrom@ipno.in2p3.fr   *
 * Creation Date  : April 2014                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for ComptonTelescope                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TComptonTelescopeSpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

// STL
#include <stdexcept>
#include <iostream>  
#include <cstdlib>
#include <string>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
TComptonTelescopeSpectra::TComptonTelescopeSpectra(){
  fNumberOfTelescope = 0;
  fStripX=32;
  fStripY=32;
  fPadSili=16;
  fCrystalCsI=16;
}

////////////////////////////////////////////////////////////////////////////////
TComptonTelescopeSpectra::TComptonTelescopeSpectra(unsigned int NumberOfTelescope)
{
   if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
      cout << "************************************************" << endl
         << "TComptonTelescopeSpectra : Initalising control spectra for " 
         << NumberOfTelescope << " Telescopes" << endl
         << "************************************************" << endl ;

   fNumberOfTelescope = NumberOfTelescope;
   fStripX=32;
   fStripY=32;
   fPadSili=16;
   fCrystalCsI=16;

   InitRawSpectra();
   InitPreTreatedSpectra();
   InitPhysicsSpectra();
}

////////////////////////////////////////////////////////////////////////////////
TComptonTelescopeSpectra::~TComptonTelescopeSpectra()
{
}

////////////////////////////////////////////////////////////////////////////////
void TComptonTelescopeSpectra::InitRawSpectra()
{
  string name;
  for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
    // STRX_E_RAW
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRX_E_RAW";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 512, 0, 8192, "COMPTONTELESCOPE/RAW/STRXE");

    // STRY_E_RAW
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRY_E_RAW";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 512, 0, 8192, "COMPTONTELESCOPE/RAW/STRYE");

    // STRX_T_RAW
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRX_T_RAW";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 512, 0, 8192, "COMPTONTELESCOPE/RAW/STRXT");

    // STRY_T_RAW
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRY_T_RAW";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 512, 0, 8192, "COMPTONTELESCOPE/RAW/STRYT");

    // STRX_RAW_MULT
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRX_RAW_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "COMPTONTELESCOPE/RAW/MULT");

    // STRY_RAW_MULT
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRY_RAW_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "COMPTONTELESCOPE/RAW/MULT");
  } // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TComptonTelescopeSpectra::InitPreTreatedSpectra()
{
  string name;
  for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
    // STRX_E_CAL
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRX_E_CAL";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 500, 0, 50, "COMPTONTELESCOPE/CAL/STRXE");

    // STRY_E_CAL
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRY_E_CAL";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 500, 0, 50, "COMPTONTELESCOPE/CAL/STRYE");

    // STRX_T_CAL
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRX_T_CAL";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 500, 0, 500, "COMPTONTELESCOPE/CAL/STRXT");

    // STRY_T_CAL
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRY_T_CAL";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 500, 0, 500, "COMPTONTELESCOPE/CAL/STRYT");

    // STRX_CAL_MULT
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRX_CAL_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "COMPTONTELESCOPE/CAL/MULT");

    // STRY_CAL_MULT
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRY_CAL_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "COMPTONTELESCOPE/CAL/MULT");
  }  // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TComptonTelescopeSpectra::InitPhysicsSpectra()
{
  string name;
  // X-Y Impact Matrix
  name = "CT_IMPACT_MATRIX";
  AddHisto2D(name, name,500,-150,150,500,-150,150, "COMPTONTELESCOPE/PHY");

  // X-Y Impact Matrix
  name = "CT_THETA_E";
  AddHisto2D(name, name,360,0,180,500,0,50,"COMPTONTELESCOPE/PHY");

  // X-Y Energy Correlation
  for (unsigned int i = 0 ; i < fNumberOfTelescope ; i++) { // loop on number of detectors
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_XY_COR";
    AddHisto2D(name, name,500,0,50,500,0,50, "COMPTONTELESCOPE/PHY"); 
  }

  // ID Plot
  // E-TOF:
  name = "CT_E_TOF";
  AddHisto2D(name, name,500,0,50,500,200,1200,"COMPTONTELESCOPE/PHY");

  // SILIE-DE:
  name = "CT_SILIE_E";
  AddHisto2D(name, name,500,0,200,500,0,50,"COMPTONTELESCOPE/PHY");

  // CSI-DE:
  name = "CT_CSIE_E";
  AddHisto2D(name, name,500,0,500,500,0,50,"COMPTONTELESCOPE/PHY");

  // Etot-DE:
  name = "CT_Etot_E";
  AddHisto2D(name, name,500,0,500,500,0,50,"COMPTONTELESCOPE/PHY");
  

  // ID plot detector by detector
  for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
    // E-TOF:
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_E_TOF";
    AddHisto2D(name, name,500,0,50,500,200,1200,"COMPTONTELESCOPE/PHY");

    // SILIE-DE:
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_SILIE_E";
    AddHisto2D(name, name,500,0,200,500,0,50,"COMPTONTELESCOPE/PHY");

    // CSI-DE:
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_CSIE_E";
    AddHisto2D(name, name,500,0,500,500,0,50,"COMPTONTELESCOPE/PHY");

    // Etot-DE:
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_Etot_E";
    AddHisto2D(name, name,500,0,500,500,0,50,"COMPTONTELESCOPE/PHY");
  }
}



////////////////////////////////////////////////////////////////////////////////
void TComptonTelescopeSpectra::FillRawSpectra(TComptonTelescopeData* RawData)
{
  string name;
  string family;

  // STRX_E 
  for (unsigned int i = 0; i < RawData->GetCTTrackerFrontEMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(RawData->GetCTTrackerFrontEDetectorNbr(i))+"_STRX_E_RAW";
    family = "COMPTONTELESCOPE/RAW/STRXE";

    GetHisto(family,name)
      -> Fill(RawData->GetCTTrackerFrontEStripNbr(i), 
          RawData->GetCTTrackerFrontEEnergy(i));
  }

  // STRY_E
  for (unsigned int i = 0; i < RawData->GetCTTrackerBackEMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(RawData->GetCTTrackerBackEDetectorNbr(i))+"_STRY_E_RAW";
    family = "COMPTONTELESCOPE/RAW/STRYE";

    GetHisto(family,name)
      -> Fill(RawData->GetCTTrackerBackEStripNbr(i),
          RawData->GetCTTrackerBackEEnergy(i));
  }

  // STRX_T
  for (unsigned int i = 0; i < RawData->GetCTTrackerFrontTMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(RawData->GetCTTrackerFrontTDetectorNbr(i))+"_STRX_T_RAW";
    family = "COMPTONTELESCOPE/RAW/STRXT";

    GetHisto(family,name)
      -> Fill(RawData->GetCTTrackerFrontTStripNbr(i),
          RawData->GetCTTrackerFrontTTime(i));
  }
  // STRY_T
  for (unsigned int i = 0; i < RawData->GetCTTrackerBackTMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(RawData->GetCTTrackerBackEDetectorNbr(i))+"_STRY_T_RAW";
    family = "COMPTONTELESCOPE/RAW/STRYT";

    GetHisto(family,name)
      -> Fill(RawData->GetCTTrackerBackTStripNbr(i),
          RawData->GetCTTrackerBackTTime(i));
  }

  // STRX MULT
  int myMULT[fNumberOfTelescope];
  for (unsigned int i = 0; i < fNumberOfTelescope; i++)
     myMULT[i] = 0 ; 

  for (unsigned int i = 0 ; i < RawData->GetCTTrackerFrontEMult(); i++) { 
     myMULT[RawData->GetCTTrackerFrontEDetectorNbr(i)-1] += 1;  
  }

  for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
     name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRX_RAW_MULT";
     family= "COMPTONTELESCOPE/RAW/MULT";
     GetHisto(family,name)
        -> Fill(myMULT[i]);
  }

  // STRY MULT
  for (unsigned int i = 0; i < fNumberOfTelescope; i++)
     myMULT[i] = 0 ; 

  for (unsigned int i = 0 ; i < RawData->GetCTTrackerBackEMult(); i++) {
     myMULT[RawData->GetCTTrackerBackEDetectorNbr(i)-1] += 1;  
  }

  for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
     name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRY_RAW_MULT";
     family= "COMPTONTELESCOPE/RAW/MULT";
     GetHisto(family,name)
        -> Fill(myMULT[i]);
  }
}

////////////////////////////////////////////////////////////////////////////////
void TComptonTelescopeSpectra::FillPreTreatedSpectra(TComptonTelescopeData* PreTreatedData)
{
/*  string name ;
  string family;
  // STRX_E
  for (unsigned int i = 0; i < PreTreatedData->GetCTStripXEMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTStripXEDetectorNbr(i))+"_STRX_E_CAL";
    family = "COMPTONTELESCOPE/CAL/STRXE";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTStripXEStripNbr(i), 
          PreTreatedData->GetCTStripXEEnergy(i));
  }
  // STRY_E
  for (unsigned int i = 0; i < PreTreatedData->GetCTStripYEMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa( PreTreatedData->GetCTStripYEDetectorNbr(i))+"_STRY_E_CAL";
    family = "COMPTONTELESCOPE/CAL/STRYE";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTStripYEStripNbr(i), 
          PreTreatedData->GetCTStripYEEnergy(i));
  }
  // STRX_T
  for (unsigned int i = 0; i < PreTreatedData->GetCTStripXTMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTStripXTDetectorNbr(i))+"_STRX_T_CAL";
    family = "COMPTONTELESCOPE/CAL/STRXT";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTStripXTStripNbr(i), 
          PreTreatedData->GetCTStripXTTime(i));
  }
  // STRY_T
  for (unsigned int i = 0; i < PreTreatedData->GetCTStripYTMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTStripYTDetectorNbr(i))+"_STRY_T_CAL";
    family = "COMPTONTELESCOPE/CAL/STRYT";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTStripYTStripNbr(i), 
          PreTreatedData->GetCTStripYTTime(i));
  }
  // SILI_E
  for (unsigned int i = 0; i < PreTreatedData->GetCTSiLiEMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTSiLiEDetectorNbr(i) )+"_SILI_E_CAL";
    family = "COMPTONTELESCOPE/CAL/SILIE";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTSiLiEPadNbr(i), 
          PreTreatedData->GetCTSiLiEEnergy(i));
  }
  // SILI_T
  for (unsigned int i = 0; i < PreTreatedData->GetCTSiLiTMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTSiLiTDetectorNbr(i))+"_SILI_T_CAL";
    family = "COMPTONTELESCOPE/CAL/SILIT";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTSiLiTPadNbr(i), 
          PreTreatedData->GetCTSiLiTTime(i));
  }
  // CSI_E
  for (unsigned int i = 0; i < PreTreatedData->GetCTCsIEMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTCsIEDetectorNbr(i))+"_CSI_E_CAL";
    family = "COMPTONTELESCOPE/CAL/CSIE";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTCsIECristalNbr(i),   
          PreTreatedData->GetCTCsIEEnergy(i));
  }
  
  // CSI_T
  for (unsigned int i = 0; i < PreTreatedData->GetCTCsITMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTCsITDetectorNbr(i))+"_CSI_T_CAL";
    family = "COMPTONTELESCOPE/CAL/CSIT";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTCsITCristalNbr(i), 
          PreTreatedData->GetCTCsITTime(i));
  }

  // STRX MULT
  int myMULT[fNumberOfTelescope];
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetCTStripXEMult();i++){
    myMULT[PreTreatedData->GetCTStripXEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){

    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRX_CAL_MULT";
    family= "COMPTONTELESCOPE/CAL/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

  // STRY MULT
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetCTStripYEMult();i++){
    myMULT[PreTreatedData->GetCTStripYEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_STRY_CAL_MULT";
    family= "COMPTONTELESCOPE/CAL/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

  // SILI MULT
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetCTSiLiEMult();i++){
    myMULT[PreTreatedData->GetCTSiLiEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_SILI_CAL_MULT";
    family= "COMPTONTELESCOPE/CAL/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

  // CSI MULT
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetCTCsIEMult();i++){
    myMULT[PreTreatedData->GetCTCsIEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_CSI_CAL_MULT";
    family= "COMPTONTELESCOPE/CAL/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

  //E-CSI ID
  family = "COMPTONTELESCOPE/CAL/ID";
  for (unsigned int i = 0; i < PreTreatedData->GetCTStripXEMult(); i++) {
   for (unsigned int j = 0; j < PreTreatedData->GetCTCsIEMult(); j++) {
    
    if(PreTreatedData->GetCTStripXEDetectorNbr(i) == PreTreatedData->GetCTCsIEDetectorNbr(j)){ 
      name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTStripXEDetectorNbr(i))+"_CSI"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTCsIECristalNbr(j))+"_CAL_ID";
    
      GetHisto(family,name)
        -> Fill(PreTreatedData->GetCTCsIEEnergy(j), 
          PreTreatedData->GetCTStripXEEnergy(i));
      }
    }
  }

*/
}

////////////////////////////////////////////////////////////////////////////////
void TComptonTelescopeSpectra::FillPhysicsSpectra(TComptonTelescopePhysics* Physics){
/*  string name;
  string family= "COMPTONTELESCOPE/PHY";
  // X-Y Impact Matrix

  for(unsigned int i = 0 ; i < Physics->Si_E.size(); i++){
    name = "CT_IMPACT_MATRIX";
    double x = Physics->GetPositionOfInteraction(i).x();
    double y = Physics->GetPositionOfInteraction(i).y();
    GetHisto(family,name)-> Fill(x,y);

    name = "CT_THETA_E";
    double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));
    Theta = Theta/deg;
    GetHisto(family,name)-> Fill(Theta,Physics->Si_E[i]);

    // STRX_E_CAL
    name = "CT"+ComptonTelescope_LOCAL::itoa( Physics->TelescopeNumber[i])+"_XY_COR";
    GetHisto(family,name)-> Fill(Physics->Si_EX[i],Physics->Si_EY[i]);


    // Fill only for particle stopped in the first stage
    if(Physics->SiLi_E[i]<0 && Physics->CsI_E[i]<0){
      // E-TOF:
      name = "CT_E_TOF";
      GetHisto(family,name)->Fill(Physics->Si_E[i],Physics->Si_T[i]);

      name = "CT"+ComptonTelescope_LOCAL::itoa(Physics->TelescopeNumber[i])+"_E_TOF";
      GetHisto(family,name)->Fill(Physics->Si_E[i],Physics->Si_T[i]);
    }

    double Etot=0;
    if(Physics->SiLi_E[i]>0){
      name = "CT_SILIE_E";
      Etot = Physics->SiLi_E[i];
      GetHisto(family,name)->Fill(Physics->SiLi_E[i],Physics->Si_E[i]);

      name = "CT"+ComptonTelescope_LOCAL::itoa(Physics->TelescopeNumber[i])+"_SILIE_E";
      GetHisto(family,name)->Fill(Physics->SiLi_E[i],Physics->Si_E[i]);
    }

    if(Physics->CsI_E[i]>0){
      name = "CT_CSIE_E";
      Etot += Physics->CsI_E[i];
      GetHisto(family,name)->Fill(Physics->CsI_E[i],Physics->Si_E[i]);
      name = "CT"+ComptonTelescope_LOCAL::itoa(Physics->TelescopeNumber[i])+"_CSIE_E";
      GetHisto(family,name)->Fill(Physics->CsI_E[i],Physics->Si_E[i]);

    }

    if(Etot>0){
      name = "CT_Etot_E";
      GetHisto(family,name)->Fill(Etot,Physics->Si_E[i]);
      name = "CT"+ComptonTelescope_LOCAL::itoa(Physics->TelescopeNumber[i])+"_Etot_E";
      GetHisto(family,name)->Fill(Etot,Physics->Si_E[i]);
    }

  }
*/
}

