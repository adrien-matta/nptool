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
    // FRONT_E_RAW
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_FRONT_E_RAW";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 512, 0, 8192, "COMPTONTELESCOPE/RAW/FRONTE");

    // BACK_E_RAW
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_BACK_E_RAW";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 512, 0, 8192, "COMPTONTELESCOPE/RAW/BACKE");

    // FRONT_T_RAW
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_FRONT_T_RAW";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 512, 0, 8192, "COMPTONTELESCOPE/RAW/FRONTT");

    // BACK_T_RAW
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_BACK_T_RAW";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 512, 0, 8192, "COMPTONTELESCOPE/RAW/BACKT");

    // FRONT_RAW_MULT
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_FRONT_RAW_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "COMPTONTELESCOPE/RAW/MULT");

    // BACK_RAW_MULT
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_BACK_RAW_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "COMPTONTELESCOPE/RAW/MULT");
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TComptonTelescopeSpectra::InitPreTreatedSpectra()
{
  string name;
  for (unsigned int i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
    // FRONT_E_CAL
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_FRONT_E_CAL";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 500, 0, 50, "COMPTONTELESCOPE/CAL/FRONTE");

    // BACK_E_CAL
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_BACK_E_CAL";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 500, 0, 50, "COMPTONTELESCOPE/CAL/BACKE");

    // FRONT_T_CAL
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_FRONT_T_CAL";
    AddHisto2D(name, name, fStripX, 1, fStripX+1, 500, 0, 500, "COMPTONTELESCOPE/CAL/FRONTT");

    // BACK_T_CAL
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_BACK_T_CAL";
    AddHisto2D(name, name, fStripY, 1, fStripY+1, 500, 0, 500, "COMPTONTELESCOPE/CAL/BACKT");

    // FRONT_CAL_MULT
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_FRONT_CAL_MULT";
    AddHisto1D(name, name, fStripX, 1, fStripX+1, "COMPTONTELESCOPE/CAL/MULT");

    // BACK_CAL_MULT
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_BACK_CAL_MULT";
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

  // X-Y Energy Correlation
  for (unsigned int i = 0 ; i < fNumberOfTelescope ; i++) { // loop on number of detectors
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_XY_COR";
    AddHisto2D(name, name,500,0,50,500,0,50, "COMPTONTELESCOPE/PHY"); 
  }
}



////////////////////////////////////////////////////////////////////////////////
void TComptonTelescopeSpectra::FillRawSpectra(TComptonTelescopeData* RawData)
{
  string name;
  string family;

  // FRONT_E 
  for (unsigned int i = 0; i < RawData->GetCTTrackerFrontEMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(RawData->GetCTTrackerFrontEDetectorNbr(i))+"_FRONT_E_RAW";
    family = "COMPTONTELESCOPE/RAW/FRONTE";

    GetHisto(family,name)
      -> Fill(RawData->GetCTTrackerFrontEStripNbr(i), 
          RawData->GetCTTrackerFrontEEnergy(i));
  }

  // BACK_E
  for (unsigned int i = 0; i < RawData->GetCTTrackerBackEMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(RawData->GetCTTrackerBackEDetectorNbr(i))+"_BACK_E_RAW";
    family = "COMPTONTELESCOPE/RAW/BACKE";

    GetHisto(family,name)
      -> Fill(RawData->GetCTTrackerBackEStripNbr(i),
          RawData->GetCTTrackerBackEEnergy(i));
  }

  // FRONT_T
  for (unsigned int i = 0; i < RawData->GetCTTrackerFrontTMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(RawData->GetCTTrackerFrontTDetectorNbr(i))+"_FRONT_T_RAW";
    family = "COMPTONTELESCOPE/RAW/FRONTT";

    GetHisto(family,name)
      -> Fill(RawData->GetCTTrackerFrontTStripNbr(i),
          RawData->GetCTTrackerFrontTTime(i));
  }
  // BACK_T
  for (unsigned int i = 0; i < RawData->GetCTTrackerBackTMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(RawData->GetCTTrackerBackEDetectorNbr(i))+"_BACK_T_RAW";
    family = "COMPTONTELESCOPE/RAW/BACKT";

    GetHisto(family,name)
      -> Fill(RawData->GetCTTrackerBackTStripNbr(i),
          RawData->GetCTTrackerBackTTime(i));
  }

  // FRONT MULT
  int myMULT[fNumberOfTelescope];
  for (unsigned int i = 0; i < fNumberOfTelescope; i++)
     myMULT[i] = 0 ; 

  for (unsigned int i = 0 ; i < RawData->GetCTTrackerFrontEMult(); i++) { 
     myMULT[RawData->GetCTTrackerFrontEDetectorNbr(i)-1] += 1;  
  }

  for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
     name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_FRONT_RAW_MULT";
     family= "COMPTONTELESCOPE/RAW/MULT";
     GetHisto(family,name)
        -> Fill(myMULT[i]);
  }

  // BACK MULT
  for (unsigned int i = 0; i < fNumberOfTelescope; i++)
     myMULT[i] = 0 ; 

  for (unsigned int i = 0 ; i < RawData->GetCTTrackerBackEMult(); i++) {
     myMULT[RawData->GetCTTrackerBackEDetectorNbr(i)-1] += 1;  
  }

  for (unsigned int i = 0; i < fNumberOfTelescope; i++) {
     name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_BACK_RAW_MULT";
     family= "COMPTONTELESCOPE/RAW/MULT";
     GetHisto(family,name)
        -> Fill(myMULT[i]);
  }
}

////////////////////////////////////////////////////////////////////////////////
void TComptonTelescopeSpectra::FillPreTreatedSpectra(TComptonTelescopeData* PreTreatedData)
{
  string name ;
  string family;
  // FRONT_E
  for (unsigned int i = 0; i < PreTreatedData->GetCTTrackerFrontEMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTTrackerFrontEDetectorNbr(i))+"_FRONT_E_CAL";
    family = "COMPTONTELESCOPE/CAL/FRONTE";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTTrackerFrontEStripNbr(i), 
          PreTreatedData->GetCTTrackerFrontEEnergy(i));
  }
  // BACK_E
  for (unsigned int i = 0; i < PreTreatedData->GetCTTrackerBackEMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa( PreTreatedData->GetCTTrackerBackEDetectorNbr(i))+"_BACK_E_CAL";
    family = "COMPTONTELESCOPE/CAL/BACKE";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTTrackerBackEStripNbr(i), 
          PreTreatedData->GetCTTrackerBackEEnergy(i));
  }
  // FRONT_T
  for (unsigned int i = 0; i < PreTreatedData->GetCTTrackerFrontTMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTTrackerFrontTDetectorNbr(i))+"_FRONT_T_CAL";
    family = "COMPTONTELESCOPE/CAL/FRONTT";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTTrackerFrontTStripNbr(i), 
          PreTreatedData->GetCTTrackerFrontTTime(i));
  }
  // BACK_T
  for (unsigned int i = 0; i < PreTreatedData->GetCTTrackerBackTMult(); i++) {
    name = "CT"+ComptonTelescope_LOCAL::itoa(PreTreatedData->GetCTTrackerBackTDetectorNbr(i))+"_BACK_T_CAL";
    family = "COMPTONTELESCOPE/CAL/BACKT";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetCTTrackerBackTStripNbr(i), 
          PreTreatedData->GetCTTrackerBackTTime(i));
  }

  // FRONT MULT
  int myMULT[fNumberOfTelescope];
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetCTTrackerFrontEMult();i++){
    myMULT[PreTreatedData->GetCTTrackerFrontEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){

    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_FRONT_CAL_MULT";
    family= "COMPTONTELESCOPE/CAL/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

  // BACK MULT
  for( unsigned int i = 0; i < fNumberOfTelescope; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetCTTrackerBackEMult();i++){
    myMULT[PreTreatedData->GetCTTrackerBackEDetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfTelescope; i++){
    name = "CT"+ComptonTelescope_LOCAL::itoa(i+1)+"_BACK_CAL_MULT";
    family= "COMPTONTELESCOPE/CAL/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }
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

    // FRONT_E_CAL
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

