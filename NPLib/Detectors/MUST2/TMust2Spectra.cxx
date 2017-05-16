/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
 *  This class holds all the online spectra needed for Must2                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TMust2Spectra.h"
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
TMust2Spectra::TMust2Spectra(){
  SetName("Must2");
  fNumberOfTelescope = 0;
  fStripX=128;
  fStripY=128;
  fPadSili=16;
  fCrystalCsI=16;
}

////////////////////////////////////////////////////////////////////////////////
TMust2Spectra::TMust2Spectra(unsigned int NumberOfTelescope){
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TMust2Spectra : Initalising control spectra for " 
      << NumberOfTelescope << " Telescopes" << endl
      << "************************************************" << endl ;

  SetName("Must2");
  fNumberOfTelescope = NumberOfTelescope;
  fStripX=128;
  fStripY=128;
  fPadSili=16;
  fCrystalCsI=16;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}

////////////////////////////////////////////////////////////////////////////////
TMust2Spectra::~TMust2Spectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::InitRawSpectra(){
  string name;
  for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
    TString nbr = NPL::itoa(i+1);
    
    // STRX_E_RAW
    name = "MM"+NPL::itoa(i+1)+"_STRX_E_RAW";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 512, 0, 8192, "MUST2/RAW/STRXE")->Draw("colz");
    
    // STRY_E_RAW
    name = "MM"+NPL::itoa(i+1)+"_STRY_E_RAW";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 512, 0, 8192, "MUST2/RAW/STRYE")->Draw("colz");

    // STRX_T_RAW
    name = "MM"+NPL::itoa(i+1)+"_STRX_T_RAW";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 512, 0, 8192, "MUST2/RAW/STRXT")->Draw("colz");

    // STRY_T_RAW
    name = "MM"+NPL::itoa(i+1)+"_STRY_T_RAW";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 512, 0, 8192, "MUST2/RAW/STRYT")->Draw("colz");
    
    // SILI_E_RAW
    name = "MM"+NPL::itoa(i+1)+"_SILI_E_RAW";
    AddHisto2D(name, name, fPadSili, 1, fPadSili+1, 512, 0, 8192, "MUST2/RAW/SILIE");

    // SILI_T_RAW
    name = "MM"+NPL::itoa(i+1)+"_SILI_T_RAW";
    AddHisto2D(name, name, fPadSili, 1, fPadSili+1, 512, 0, 8192, "MUST2/RAW/SILIT");

    // CSI_E_RAW
    name = "MM"+NPL::itoa(i+1)+"_CSI_E_RAW";
    AddHisto2D(name, name, fCrystalCsI, 1, fCrystalCsI+1, 512, 0, 16384, "MUST2/RAW/CSIE");

    // CSI_T_RAW
    name = "MM"+NPL::itoa(i+1)+"_CSI_T_RAW";
    AddHisto2D(name, name, fCrystalCsI, 1, fCrystalCsI+1, 512, 0, 8192, "MUST2/RAW/CSIT");

    // STRX_RAW_MULT
    name = "MM"+NPL::itoa(i+1)+"_STRX_RAW_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "MUST2/RAW/MULT");

    // STRY_RAW_MULT
    name = "MM"+NPL::itoa(i+1)+"_STRY_RAW_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "MUST2/RAW/MULT");

    // SILI_RAW_MULT
    name = "MM"+NPL::itoa(i+1)+"_SILI_RAW_MULT";
    AddHisto1D(name, name, fPadSili, 1, fPadSili+1, "MUST2/RAW/MULT");

    // CSI_RAW_MULT
    name = "MM"+NPL::itoa(i+1)+"_CSI_RAW_MULT";
    AddHisto1D(name, name, fCrystalCsI, 1, fCrystalCsI+1, "MUST2/RAW/MULT");
  } // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::InitPreTreatedSpectra()
{
  string name;
  for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
    // STRX_E_CAL
    name = "MM"+NPL::itoa(i+1)+"_STRX_E_CAL";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 500, 0, 50, "MUST2/CAL/STRXE");

    // STRY_E_CAL
    name = "MM"+NPL::itoa(i+1)+"_STRY_E_CAL";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 500, 0, 50, "MUST2/CAL/STRYE");

    // STRX_T_CAL
    name = "MM"+NPL::itoa(i+1)+"_STRX_T_CAL";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 500, 0, 500, "MUST2/CAL/STRXT");

    // STRY_T_CAL
    name = "MM"+NPL::itoa(i+1)+"_STRY_T_CAL";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 500, 0, 500, "MUST2/CAL/STRYT");

    // SILI_E_CAL
    name = "MM"+NPL::itoa(i+1)+"_SILI_E_CAL";
    AddHisto2D(name, name, fPadSili, 1, fPadSili+1, 500, 0, 50, "MUST2/CAL/SILIE");

    // SILI_T_CAL
    name = "MM"+NPL::itoa(i+1)+"_SILI_T_CAL";
    AddHisto2D(name, name, fPadSili, 1, fPadSili+1, 500, 0, 50, "MUST2/CAL/SILIT");

    // CSI_E_CAL
    name = "MM"+NPL::itoa(i+1)+"_CSI_E_CAL";
    AddHisto2D(name, name, fCrystalCsI, 1, fCrystalCsI+1, 500, 0, 500, "MUST2/CAL/CSIE");

    // CSI_T_CAL
    name = "MM"+NPL::itoa(i+1)+"_CSI_T_CAL";
    AddHisto2D(name, name, fCrystalCsI, 1, fCrystalCsI+1, 500, 0, 50, "MUST2/CAL/CSIT");

    // STRX_CAL_MULT
    name = "MM"+NPL::itoa(i+1)+"_STRX_CAL_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "MUST2/CAL/MULT");

    // STRY_CAL_MULT
    name = "MM"+NPL::itoa(i+1)+"_STRY_CAL_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "MUST2/CAL/MULT");

    // SILI_CAL_MULT
    name = "MM"+NPL::itoa(i+1)+"_SILI_CAL_MULT";
    AddHisto1D(name, name, fPadSili, 1, fPadSili+1, "MUST2/CAL/MULT");

    // CSI_CAL_MULT
    name = "MM"+NPL::itoa(i+1)+"_CSI_CAL_MULT";
    AddHisto1D(name, name, fCrystalCsI, 1, fCrystalCsI+1, "MUST2/CAL/MULT");
   
    // CSI_CAL_ID 
    for(unsigned int j = 0 ; j < fCrystalCsI ; j++){
     name = "MM"+NPL::itoa(i+1)+"_CSI"+NPL::itoa(j+1)+"_CAL_ID";
     AddHisto2D(name, name,1024,0,16384,500,0,50, "MUST2/CAL/ID");
    }
  }  // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::InitPhysicsSpectra(){
  string name;
  // X-Y Impact Matrix
  name = "MM_IMPACT_MATRIX";
  AddHisto2D(name, name,500,-150,150,500,-150,150, "MUST2/PHY");

  // X-Y Impact Matrix
  name = "MM_THETA_E";
  AddHisto2D(name, name,360,0,180,500,0,50,"MUST2/PHY");

  // X-Y Energy Correlation
  for (unsigned int i = 0 ; i < fNumberOfTelescope ; i++) { // loop on number of detectors
    name = "MM"+NPL::itoa(i+1)+"_XY_COR";
    AddHisto2D(name, name,500,0,50,500,0,50, "MUST2/PHY"); 
  }

  // ID Plot
  // E-TOF:
  name = "MM_E_TOF";
  AddHisto2D(name, name,500,0,50,500,200,1200,"MUST2/PHY");

  // SILIE-DE:
  name = "MM_SILIE_E";
  AddHisto2D(name, name,500,0,200,500,0,50,"MUST2/PHY");

  // CSI-DE:
  name = "MM_CSIE_E";
  AddHisto2D(name, name,500,0,500,500,0,50,"MUST2/PHY");

  // Etot-DE:
  name = "MM_Etot_E";
  AddHisto2D(name, name,500,0,500,500,0,50,"MUST2/PHY");
  

  // ID plot detector by detector
  for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
    // E-TOF:
    name = "MM"+NPL::itoa(i+1)+"_E_TOF";
    AddHisto2D(name, name,500,0,50,500,200,1200,"MUST2/PHY");

    // SILIE-DE:
    name = "MM"+NPL::itoa(i+1)+"_SILIE_E";
    AddHisto2D(name, name,500,0,200,500,0,50,"MUST2/PHY");

    // CSI-DE:
    name = "MM"+NPL::itoa(i+1)+"_CSIE_E";
    AddHisto2D(name, name,500,0,500,500,0,50,"MUST2/PHY");

    // Etot-DE:
    name = "MM"+NPL::itoa(i+1)+"_Etot_E";
    AddHisto2D(name, name,500,0,500,500,0,50,"MUST2/PHY");
  }
}



////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::FillRawSpectra(TMust2Data* RawData){
  string name;
  string family;

  // STRX_E 
  for (unsigned int i = 0; i < RawData->GetMMStripXEMult(); i++) {
    name = "MM"+NPL::itoa(RawData->GetMMStripXEDetectorNbr(i))+"_STRX_E_RAW";
    family = "MUST2/RAW/STRXE";

    FillSpectra(family,name
      ,RawData->GetMMStripXEStripNbr(i), 
          RawData->GetMMStripXEEnergy(i) - 8192);
  }

  // STRY_E
  for (unsigned int i = 0; i < RawData->GetMMStripYEMult(); i++) {
    name = "MM"+NPL::itoa( RawData->GetMMStripYEDetectorNbr(i) )+"_STRY_E_RAW";
    family = "MUST2/RAW/STRYE";

    FillSpectra(family,name
      ,RawData->GetMMStripYEStripNbr(i),
          8192 - RawData->GetMMStripYEEnergy(i));
  }

  // STRX_T
  for (unsigned int i = 0; i < RawData->GetMMStripXTMult(); i++) {
    name = "MM"+NPL::itoa(RawData->GetMMStripXTDetectorNbr(i))+"_STRX_T_RAW";
    family = "MUST2/RAW/STRXT";

    FillSpectra(family,name
      ,RawData->GetMMStripXTStripNbr(i),
          8192 - RawData->GetMMStripXTTime(i));
  }
  // STRY_T
  for (unsigned int i = 0; i < RawData->GetMMStripYTMult(); i++) {
    name = "MM"+NPL::itoa(RawData->GetMMStripYTDetectorNbr(i))+"_STRY_T_RAW";
    family = "MUST2/RAW/STRYT";

    FillSpectra(family,name
      ,RawData->GetMMStripYTStripNbr(i),
          RawData->GetMMStripYTTime(i) - 8192);
  }

  // SILI_E
  for (unsigned int i = 0; i < RawData->GetMMSiLiEMult(); i++) {
    name = "MM"+NPL::itoa( RawData->GetMMSiLiEDetectorNbr(i))+"_SILI_E_RAW";
    family = "MUST2/RAW/SILIE";

    FillSpectra(family,name
      ,RawData->GetMMSiLiEPadNbr(i),
          RawData->GetMMSiLiEEnergy(i));
  }

  // SILI_T
  for (unsigned int i = 0; i < RawData->GetMMSiLiTMult(); i++) {
    name = "MM"+NPL::itoa(RawData->GetMMSiLiTDetectorNbr(i))+"_SILI_T_RAW";
    family = "MUST2/RAW/SILIT";

    FillSpectra(family,name
      ,RawData->GetMMSiLiTPadNbr(i),
          RawData->GetMMSiLiTTime(i));
  }

  // CSI_E
  for (unsigned int i = 0; i < RawData->GetMMCsIEMult(); i++) {
    name = "MM"+NPL::itoa(RawData->GetMMCsIEDetectorNbr(i))+"_CSI_E_RAW";
    family = "MUST2/RAW/CSIE";

    FillSpectra(family,name
      ,RawData->GetMMCsIECristalNbr(i), 
          RawData->GetMMCsIEEnergy(i));
  }

  // CSI_T
  for (unsigned int i = 0; i < RawData->GetMMCsITMult(); i++) {
    name = "MM"+NPL::itoa( RawData->GetMMCsITDetectorNbr(i))+"_CSI_T_RAW";
    family = "MUST2/RAW/CSIT";

    FillSpectra(family,name
      ,RawData->GetMMCsITCristalNbr(i), 
          RawData->GetMMCsITTime(i));
  }

  // STRX MULT
  int myMULT[fNumberOfTelescope];
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < RawData->GetMMStripXEMult();i++){
    myMULT[RawData->GetMMStripXEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){

    name = "MM"+NPL::itoa(i+1)+"_STRX_RAW_MULT";
    family= "MUST2/RAW/MULT";
    FillSpectra(family,name
      ,myMULT[i]);
  }

  // STRY MULT
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < RawData->GetMMStripYEMult();i++){
    myMULT[RawData->GetMMStripYEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){
    name = "MM"+NPL::itoa(i+1)+"_STRY_RAW_MULT";
    family= "MUST2/RAW/MULT";
    FillSpectra(family,name
      ,myMULT[i]);
  }

  // SILI MULT
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < RawData->GetMMSiLiEMult();i++){
    myMULT[RawData->GetMMSiLiEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){
    name = "MM"+NPL::itoa(i+1)+"_SILI_RAW_MULT";
    family= "MUST2/RAW/MULT";
    FillSpectra(family,name
      ,myMULT[i]);
  }

  // CSI MULT
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < RawData->GetMMCsIEMult();i++){
    myMULT[RawData->GetMMCsIEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){
    name = "MM"+NPL::itoa(i+1)+"_CSI_RAW_MULT";
    family= "MUST2/RAW/MULT";
    FillSpectra(family,name
      ,myMULT[i]);
  }

}

////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::FillPreTreatedSpectra(TMust2Data* PreTreatedData){
  string name ;
  string family;
  // STRX_E
  for (unsigned int i = 0; i < PreTreatedData->GetMMStripXEMult(); i++) {
    name = "MM"+NPL::itoa(PreTreatedData->GetMMStripXEDetectorNbr(i))+"_STRX_E_CAL";
    family = "MUST2/CAL/STRXE";

    FillSpectra(family,name
      ,PreTreatedData->GetMMStripXEStripNbr(i), 
          PreTreatedData->GetMMStripXEEnergy(i));
  }
  // STRY_E
  for (unsigned int i = 0; i < PreTreatedData->GetMMStripYEMult(); i++) {
    name = "MM"+NPL::itoa( PreTreatedData->GetMMStripYEDetectorNbr(i))+"_STRY_E_CAL";
    family = "MUST2/CAL/STRYE";

    FillSpectra(family,name
      ,PreTreatedData->GetMMStripYEStripNbr(i), 
          PreTreatedData->GetMMStripYEEnergy(i));
  }
  // STRX_T
  for (unsigned int i = 0; i < PreTreatedData->GetMMStripXTMult(); i++) {
    name = "MM"+NPL::itoa(PreTreatedData->GetMMStripXTDetectorNbr(i))+"_STRX_T_CAL";
    family = "MUST2/CAL/STRXT";

    FillSpectra(family,name
      ,PreTreatedData->GetMMStripXTStripNbr(i), 
          PreTreatedData->GetMMStripXTTime(i));
  }
  // STRY_T
  for (unsigned int i = 0; i < PreTreatedData->GetMMStripYTMult(); i++) {
    name = "MM"+NPL::itoa(PreTreatedData->GetMMStripYTDetectorNbr(i))+"_STRY_T_CAL";
    family = "MUST2/CAL/STRYT";

    FillSpectra(family,name
      ,PreTreatedData->GetMMStripYTStripNbr(i), 
          PreTreatedData->GetMMStripYTTime(i));
  }
  // SILI_E
  for (unsigned int i = 0; i < PreTreatedData->GetMMSiLiEMult(); i++) {
    name = "MM"+NPL::itoa(PreTreatedData->GetMMSiLiEDetectorNbr(i) )+"_SILI_E_CAL";
    family = "MUST2/CAL/SILIE";

    FillSpectra(family,name
      ,PreTreatedData->GetMMSiLiEPadNbr(i), 
          PreTreatedData->GetMMSiLiEEnergy(i));
  }
  // SILI_T
  for (unsigned int i = 0; i < PreTreatedData->GetMMSiLiTMult(); i++) {
    name = "MM"+NPL::itoa(PreTreatedData->GetMMSiLiTDetectorNbr(i))+"_SILI_T_CAL";
    family = "MUST2/CAL/SILIT";

    FillSpectra(family,name
      ,PreTreatedData->GetMMSiLiTPadNbr(i), 
          PreTreatedData->GetMMSiLiTTime(i));
  }
  // CSI_E
  for (unsigned int i = 0; i < PreTreatedData->GetMMCsIEMult(); i++) {
    name = "MM"+NPL::itoa(PreTreatedData->GetMMCsIEDetectorNbr(i))+"_CSI_E_CAL";
    family = "MUST2/CAL/CSIE";

    FillSpectra(family,name
      ,PreTreatedData->GetMMCsIECristalNbr(i),   
          PreTreatedData->GetMMCsIEEnergy(i));
  }
  
  // CSI_T
  for (unsigned int i = 0; i < PreTreatedData->GetMMCsITMult(); i++) {
    name = "MM"+NPL::itoa(PreTreatedData->GetMMCsITDetectorNbr(i))+"_CSI_T_CAL";
    family = "MUST2/CAL/CSIT";

    FillSpectra(family,name
      ,PreTreatedData->GetMMCsITCristalNbr(i), 
          PreTreatedData->GetMMCsITTime(i));
  }

  // STRX MULT
  int myMULT[fNumberOfTelescope];
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetMMStripXEMult();i++){
    myMULT[PreTreatedData->GetMMStripXEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){

    name = "MM"+NPL::itoa(i+1)+"_STRX_CAL_MULT";
    family= "MUST2/CAL/MULT";
    FillSpectra(family,name
      ,myMULT[i]);
  }

  // STRY MULT
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetMMStripYEMult();i++){
    myMULT[PreTreatedData->GetMMStripYEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){
    name = "MM"+NPL::itoa(i+1)+"_STRY_CAL_MULT";
    family= "MUST2/CAL/MULT";
    FillSpectra(family,name
      ,myMULT[i]);
  }

  // SILI MULT
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetMMSiLiEMult();i++){
    myMULT[PreTreatedData->GetMMSiLiEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){
    name = "MM"+NPL::itoa(i+1)+"_SILI_CAL_MULT";
    family= "MUST2/CAL/MULT";
    FillSpectra(family,name
      ,myMULT[i]);
  }

  // CSI MULT
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetMMCsIEMult();i++){
    myMULT[PreTreatedData->GetMMCsIEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){
    name = "MM"+NPL::itoa(i+1)+"_CSI_CAL_MULT";
    family= "MUST2/CAL/MULT";
    FillSpectra(family,name
      ,myMULT[i]);
  }

  //E-CSI ID
  family = "MUST2/CAL/ID";
  for (unsigned int i = 0; i < PreTreatedData->GetMMStripXEMult(); i++) {
   for (unsigned int j = 0; j < PreTreatedData->GetMMCsIEMult(); j++) {
    
    if(PreTreatedData->GetMMStripXEDetectorNbr(i) == PreTreatedData->GetMMCsIEDetectorNbr(j)){ 
      name = "MM"+NPL::itoa(PreTreatedData->GetMMStripXEDetectorNbr(i))+"_CSI"+NPL::itoa(PreTreatedData->GetMMCsIECristalNbr(j))+"_CAL_ID";
    
      FillSpectra(family,name
        ,PreTreatedData->GetMMCsIEEnergy(j), 
          PreTreatedData->GetMMStripXEEnergy(i));
      }
    }
  }


}

////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::FillPhysicsSpectra(TMust2Physics* Physics){
  string name;
  string family= "MUST2/PHY";
  // X-Y Impact Matrix

  for(unsigned int i = 0 ; i < Physics->Si_E.size(); i++){
    name = "MM_IMPACT_MATRIX";
    double x = Physics->GetPositionOfInteraction(i).x();
    double y = Physics->GetPositionOfInteraction(i).y();
    FillSpectra(family,name,x,y);

    name = "MM_THETA_E";
    double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));
    Theta = Theta/deg;
    FillSpectra(family,name,Theta,Physics->Si_E[i]);

    // STRX_E_CAL
    name = "MM"+NPL::itoa( Physics->TelescopeNumber[i])+"_XY_COR";
    FillSpectra(family,name,Physics->Si_EX[i],Physics->Si_EY[i]);


    // Fill only for particle stopped in the first stage
    if(Physics->SiLi_E[i]<0 && Physics->CsI_E[i]<0){
      // E-TOF:
      name = "MM_E_TOF";
      FillSpectra(family,name,Physics->Si_E[i],Physics->Si_T[i]);

      name = "MM"+NPL::itoa(Physics->TelescopeNumber[i])+"_E_TOF";
      FillSpectra(family,name,Physics->Si_E[i],Physics->Si_T[i]);
    }

    double Etot=0;
    if(Physics->SiLi_E[i]>0){
      name = "MM_SILIE_E";
      Etot = Physics->SiLi_E[i];
      FillSpectra(family,name,Physics->SiLi_E[i],Physics->Si_E[i]);

      name = "MM"+NPL::itoa(Physics->TelescopeNumber[i])+"_SILIE_E";
      FillSpectra(family,name,Physics->SiLi_E[i],Physics->Si_E[i]);
    }

    if(Physics->CsI_E[i]>0){
      name = "MM_CSIE_E";
      Etot += Physics->CsI_E[i];
      FillSpectra(family,name,Physics->CsI_E[i],Physics->Si_E[i]);
      name = "MM"+NPL::itoa(Physics->TelescopeNumber[i])+"_CSIE_E";
      FillSpectra(family,name,Physics->CsI_E[i],Physics->Si_E[i]);

    }

    if(Etot>0){
      name = "MM_Etot_E";
      FillSpectra(family,name,Etot,Physics->Si_E[i]);
      name = "MM"+NPL::itoa(Physics->TelescopeNumber[i])+"_Etot_E";
      FillSpectra(family,name,Etot,Physics->Si_E[i]);
    }

  }

}

