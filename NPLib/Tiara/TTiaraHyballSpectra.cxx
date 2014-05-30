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
 *  This class holds all the online spectra needed for Tiara/Hyball          *
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
#include "TTiaraHyballSpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

////////////////////////////////////////////////////////////////////////////////
TTiaraHyballSpectra::TTiaraHyballSpectra(){
  if (NPOptionManager::getInstance()->GetVerboseLevel() > 0) {
    cout << "************************************************" << endl;
    cout << "TTiaraHyballSpectra: Initalising control spectra" << endl; 
    cout << "************************************************" << endl;
  }

  fRingsNumber   = 16;
  fSectorsNumber =  8;
  fWedgesNumber  =  6;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TTiaraHyballSpectra::~TTiaraHyballSpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::InitRawSpectra(){
  string name;

  // HIT_RING_RAW
  name = "HYB_HIT_RING_RAW";
  AddHisto1D(name, name, fWedgesNumber*fRingsNumber, 1, fWedgesNumber*fRingsNumber+1, "TIARA/HYBALL/RAW/HIT");

  // HIT_SECTOR_RAW
  name = "HYB_HIT_SECT_RAW";
  AddHisto1D(name, name, fWedgesNumber*fSectorsNumber, 1, fWedgesNumber*fSectorsNumber+1, "TIARA/HYBALL/RAW/HIT");

  // RING_E_RAW
  name = "HYB_RING_E_RAW";
  AddHisto2D(name, name, fWedgesNumber*fRingsNumber, 1, fWedgesNumber*fRingsNumber+1, 512, 0, 16384, "TIARA/HYBALL/RAW/ENERGY");

  // SECTOR_E_RAW
  name = "HYB_SECT_E_RAW";
  AddHisto2D(name, name, fWedgesNumber*fSectorsNumber, 1, fWedgesNumber*fSectorsNumber+1, 512, 0, 16384, "TIARA/HYBALL/RAW/ENERGY");

  // MULT
  for (unsigned int i = 0; i < fWedgesNumber; ++i) {   // loop on number of wedges
    // RING_RAW_MULT
    name = "HYB_W"+TiaraHyball_LOCAL::itoa(i+1)+"_RING_E_RAW_MULT";
    AddHisto1D(name, name, fRingsNumber, 1, fRingsNumber+1, "TIARA/HYBALL/RAW/MULT");

    // SECTOR_RAW_MULT
    name = "HYB_W"+TiaraHyball_LOCAL::itoa(i+1)+"_SECT_E_RAW_MULT";
    AddHisto1D(name, name, fSectorsNumber, 1, fSectorsNumber+1, "TIARA/HYBALL/RAW/MULT");
  } // end loop on number of wedges
}



////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::InitPreTreatedSpectra(){
  string name;

  // HIT_RING_CAL
  name = "HYB_HIT_RING_CAL";
  AddHisto1D(name, name, fWedgesNumber*fRingsNumber, 1, fWedgesNumber*fRingsNumber+1, "TIARA/HYBALL/CAL/HIT");

  // HIT_SECTOR_CAL
  name = "HYB_HIT_SECT_CAL";
  AddHisto1D(name, name, fWedgesNumber*fSectorsNumber, 1, fWedgesNumber*fSectorsNumber+1, "TIARA/HYBALL/CAL/HIT");

  // RING_E_CAL
  name = "HYB_RING_E_CAL";
  AddHisto2D(name, name, fWedgesNumber*fRingsNumber, 1, fWedgesNumber*fRingsNumber+1, 500, 0, 50, "TIARA/HYBALL/CAL/ENERGY");

  // SECTOR_E_CAL
  name = "HYB_SECT_E_CAL";
  AddHisto2D(name, name, fWedgesNumber*fSectorsNumber, 1, fWedgesNumber*fSectorsNumber+1, 500, 0, 50, "TIARA/HYBALL/CAL/ENERGY");

  // MULT
  for (unsigned int i = 0; i < fWedgesNumber; ++i) {   // loop on number of wedges
    // RING_CAL_MULT
    name = "HYB_W"+TiaraHyball_LOCAL::itoa(i+1)+"_RING_E_CAL_MULT";
    AddHisto1D(name, name, fRingsNumber, 1, fRingsNumber+1, "TIARA/HYBALL/CAL/MULT");

    // SECTOR_CAL_MULT
    name = "HYB_W"+TiaraHyball_LOCAL::itoa(i+1)+"_SECT_E_CAL_MULT";
    AddHisto1D(name, name, fSectorsNumber, 1, fSectorsNumber+1, "TIARA/HYBALL/CAL/MULT");
  } // end loop on number of wedges
}



////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::InitPhysicsSpectra(){
  string name;
  // X-Y Impact Matrix
  name = "HYB_IMPACT_MATRIX";
  AddHisto2D(name, name, 500, -150, 150, 500, -150, 150, "TIARA/HYBALL/PHY");

  // Kinematic line
  name = "HYB_THETA_E";
  AddHisto2D(name, name, 360, 0, 180, 500, 0, 50, "TIARA/HYBALL/PHY");

  // Ring v.s. Sector Energy Correlation
  name = "HYB_XY_COR";
  AddHisto2D(name, name, 500, 0, 50, 500, 0, 50, "TIARA/HYBALL/PHY"); 
}



////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::FillRawSpectra(TTiaraHyballData* RawData){
  string name;
  string family;

  // HIT_RING_RAW
  family = "TIARA/HYBALL/RAW/HIT";
  name = "HYB_HIT_RING_RAW";
  for (unsigned int i = 0; i < RawData->GetRingEMult(); i++) {
    unsigned short channel = (RawData->GetRingEDetectorNbr(i)-1) * fRingsNumber + RawData->GetRingEStripNbr(i);
    GetHisto(family, name) -> Fill(channel);
  }

  // HIT_SECT_RAW
  family = "TIARA/HYBALL/RAW/HIT";
  name = "HYB_HIT_SECT_RAW";
  for (unsigned int i = 0; i < RawData->GetSectorEMult(); i++) {
    unsigned short channel = (RawData->GetSectorEDetectorNbr(i)-1) * fSectorsNumber + RawData->GetSectorEStripNbr(i);
    GetHisto(family, name) -> Fill(channel);
  }

  // RING_E_RAW
  family = "TIARA/HYBALL/RAW/ENERGY";
  name = "HYB_RING_E_RAW";
  for (unsigned int i = 0; i < RawData->GetRingEMult(); i++) {
    unsigned short channel = (RawData->GetRingEDetectorNbr(i)-1) * fRingsNumber + RawData->GetRingEStripNbr(i);
    GetHisto(family, name) -> Fill(channel, RawData->GetRingEEnergy(i));
  }

  // SECTOR_E_RAW
  family = "TIARA/HYBALL/RAW/ENERGY";
  name = "HYB_SECT_E_RAW";
  for (unsigned int i = 0; i < RawData->GetSectorEMult(); i++) {
    unsigned short channel = (RawData->GetSectorEDetectorNbr(i)-1) * fSectorsNumber + RawData->GetSectorEStripNbr(i);
    GetHisto(family, name) -> Fill(channel, RawData->GetSectorEEnergy(i));
}

  // RING_RAW_MULT
  int myMULT[fWedgesNumber];
  for (unsigned int i = 0; i < fWedgesNumber; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < RawData->GetRingEMult(); i++) myMULT[(RawData->GetRingEDetectorNbr(i)-1)] += 1;

  for (unsigned int i = 0; i < fWedgesNumber; i++) {
    name   = "HYB_W"+TiaraHyball_LOCAL::itoa(i+1)+"_RING_E_RAW_MULT";
    family = "TIARA/HYBALL/RAW/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }

  // Sector_RAW_MULT
  for (unsigned int i = 0; i < fWedgesNumber; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < RawData->GetSectorEMult(); i++) myMULT[(RawData->GetSectorEDetectorNbr(i)-1)] += 1;

  for (unsigned int i = 0; i < fWedgesNumber; i++) {
    name   = "HYB_W"+TiaraHyball_LOCAL::itoa(i+1)+"_SECT_E_RAW_MULT";
    family = "TIARA/HYBALL/RAW/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }
}



////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::FillPreTreatedSpectra(TTiaraHyballData* PreTreatedData){
  string name;
  string family;

  // HIT_RING_CAL
  family = "TIARA/HYBALL/CAL/HIT";
  name = "HYB_HIT_RING_CAL";
  for (unsigned int i = 0; i < PreTreatedData->GetRingEMult(); i++) {
    unsigned short channel = (PreTreatedData->GetRingEDetectorNbr(i)-1) * fRingsNumber + PreTreatedData->GetRingEStripNbr(i);
    GetHisto(family, name) -> Fill(channel);
  }

  // HIT_SECT_CAL
  family = "TIARA/HYBALL/CAL/HIT";
  name = "HYB_HIT_SECT_CAL";
  for (unsigned int i = 0; i < PreTreatedData->GetSectorEMult(); i++) {
    unsigned short channel = (PreTreatedData->GetSectorEDetectorNbr(i)-1) * fSectorsNumber + PreTreatedData->GetSectorEStripNbr(i);
    GetHisto(family, name) -> Fill(channel);
  }

  // RING_E_CAL
  family = "TIARA/HYBALL/CAL/ENERGY";
  name = "HYB_RING_E_CAL";
  for (unsigned int i = 0; i < PreTreatedData->GetRingEMult(); i++) {
    unsigned short channel = (PreTreatedData->GetRingEDetectorNbr(i)-1) * fRingsNumber + PreTreatedData->GetRingEStripNbr(i);
    GetHisto(family, name) -> Fill(channel, PreTreatedData->GetRingEEnergy(i));
  }

  // SECTOR_E_CAL
  family = "TIARA/HYBALL/CAL/ENERGY";
  name = "HYB_SECT_E_CAL";
  for (unsigned int i = 0; i < PreTreatedData->GetRingEMult(); i++) {
    unsigned short channel = (PreTreatedData->GetRingEDetectorNbr(i)-1) * fSectorsNumber + PreTreatedData->GetSectorEStripNbr(i);
    GetHisto(family, name) -> Fill(channel, PreTreatedData->GetRingEEnergy(i));
  }

  // RING_CAL_MULT
  int myMULT[fWedgesNumber];
  for (unsigned int i = 0; i < fWedgesNumber; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < PreTreatedData->GetRingEMult(); i++) myMULT[(PreTreatedData->GetRingEDetectorNbr(i)-1)] += 1;

  for (unsigned int i = 0; i < fWedgesNumber; i++) {
    name   = "HYB_W"+TiaraHyball_LOCAL::itoa(i+1)+"_RING_E_CAL_MULT";
    family = "TIARA/HYBALL/CAL/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }

  // RING_CAL_MULT
  for (unsigned int i = 0; i < fWedgesNumber; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < PreTreatedData->GetSectorEMult(); i++) myMULT[(PreTreatedData->GetSectorEDetectorNbr(i)-1)] += 1;

  for (unsigned int i = 0; i < fWedgesNumber; i++) {
    name   = "HYB_W"+TiaraHyball_LOCAL::itoa(i+1)+"_SECT_E_CAL_MULT";
    family = "TIARA/HYBALL/CAL/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }
}



////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::FillPhysicsSpectra(TTiaraHyballPhysics* Physics){
  string name;
  string family= "TIARA/HYBALL/PHY";
  // X-Y Impact Matrix

  for(unsigned int i = 0 ; i < Physics->Strip_E.size(); i++){
  name = "HYB_IMPACT_MATRIX";
  double x = Physics->GetPositionOfInteraction(i).x();
  double y = Physics->GetPositionOfInteraction(i).y();
  GetHisto(family,name)-> Fill(x,y);

  name = "HYB_THETA_E";
  double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));
  Theta = Theta/deg;
  GetHisto(family,name)-> Fill(Theta,Physics->Strip_E[i]);

  // STRX_E_CAL
  name = "HYB_XY_COR";
  GetHisto(family,name)-> Fill(Physics->StripRing_E[i],Physics->StripSector_E[i]);
  }

}

