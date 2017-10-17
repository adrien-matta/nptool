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
 *  This class holds all the online spectra needed for Tiara/Hyball          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TCATSSpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

// ROOT
#include "TH2.h"

//STL
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>
using namespace std;



////////////////////////////////////////////////////////////////////////////////
TCATSSpectra::TCATSSpectra(){
  SetName("CATS");
  fNumberOfCats = 0;
  fStripsNumber = 28;

  // An histo of size fEventLoopSize is reset every fEventLoopSize to monitor the
  fEventLoopSize = 5000;
  fEventLoopStep = 100;
  fEventLoopIndex = 0;
}

////////////////////////////////////////////////////////////////////////////////
TCATSSpectra::TCATSSpectra(unsigned int NumberOfCats){
  if (NPOptionManager::getInstance()->GetVerboseLevel() > 0) {
    cout << "************************************************" << endl;
    cout << "TCATSSpectra: Initalising control spectra for " << NumberOfCats << " detectors" << endl; 
    cout << "************************************************" << endl;
  }
  SetName("CATS");
  fNumberOfCats = NumberOfCats;
  fStripsNumber = 28;
  // An histo of size fEventLoopSize is reset every fEventLoopSize to monitor the
  fEventLoopSize = 10000;
  fEventLoopStep = 1;
  fEventLoopIndex = 0;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}

////////////////////////////////////////////////////////////////////////////////
TCATSSpectra::~TCATSSpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::InitRawSpectra(){
  string name;

  for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
    // CATS_STRX_Q_RAW
    name = "CATS"+NPL::itoa(i+1)+"_STRX_Q_RAW";
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, "CATS/RAW/STRQ");

    // CATS_STRY_Q_RAW
    name = "CATS"+NPL::itoa(i+1)+"_STRY_Q_RAW";
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, "CATS/RAW/STRQ");

    // STRX_MULT
    name = "CATS"+NPL::itoa(i+1)+"_STRX_MULT";
    AddHisto1D(name, name, fStripsNumber, 1, fStripsNumber+1, "CATS/RAW/MULT");

    // STRY_MULT
    name = "CATS"+NPL::itoa(i+1)+"_STRY_MULT";
    AddHisto1D(name, name, fStripsNumber, 1, fStripsNumber+1, "CATS/RAW/MULT");
  } // end loop on number of cats
}

////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::InitPreTreatedSpectra(){
  string family = "CATS/CAL/STRQ" ;
  string name;

  for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
    // CATS_STRX_Q_CAL
    name = "CATS"+NPL::itoa(i+1)+"_STRX_Q_CAL";
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, family);

    // CATS_STRY_Q_CAL
    name = "CATS"+NPL::itoa(i+1)+"_STRY_Q_CAL";
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, family);
    // end loop on number of cats

    // STRX_MULT
    name = "CATS"+NPL::itoa(i+1)+"_STRX_CAL_MULT";
    AddHisto1D(name, name, fStripsNumber, 1, fStripsNumber+1, "CATS/CAL/MULT");

    // STRY_MULT
    name = "CATS"+NPL::itoa(i+1)+"_STRY_CAL_MULT";
    AddHisto1D(name, name, fStripsNumber, 1, fStripsNumber+1, "CATS/CAL/MULT");
  } // end loop on number of cats
}

////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::InitPhysicsSpectra(){
  string family = "CATS/PHY/QSUM" ;
  string name;

  for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
    name = "CATS"+NPL::itoa(i+1)+"_QSUM_STRMAX_X_CAL";
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, family);

    name = "CATS"+NPL::itoa(i+1)+"_QSUM_STRMAX_Y_CAL";
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, family);
  }

  family = "CATS/PHY/CTRL";
  for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
    name = "CATS"+NPL::itoa(i+1)+"_QMEAN_TIME";
    AddHisto1D(name, name, fEventLoopSize,0,fEventLoopSize,family); 
    fEventLoopQSum.push_back(0);
  }

  family = "CATS/PHY/POS";
  for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
    name = "CATS"+NPL::itoa(i+1)+"_POS";
    AddHisto2D(name, name,120,-40,40,120,-40,40,family);
    name = "CATS_STRIP_"+NPL::itoa(i+1)+"_POS";
    AddHisto2D(name, name,120,1,28,120,1,28,family);

  } 

  name = "TARGET_POS";
  AddHisto2D(name, name,320,-40,40,320,-40,40,family);
  name = "TRAJECTORY_XZ";
  AddHisto2D(name, name,500,-700,500,200,-200,400,family); 

  name = "TRAJECTORY_YZ";
  AddHisto2D(name, name,500,-700,500,200,-200,400,family); 

}

////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::FillRawSpectra(TCATSData* RawData){
  string name;
  string family;

  // CATS_STRX_Q_RAW
  for (unsigned int i = 0; i < RawData->GetCATSMultX(); ++i) {   // loop on vector
    family = "CATS/RAW/STRQ";
    name   = "CATS"+NPL::itoa(RawData->GetCATSDetX(i))+"_STRX_Q_RAW";
    FillSpectra(family, name ,RawData->GetCATSStripX(i), RawData->GetCATSChargeX(i)); 
  } // end loop on vector

  // CATS_STRY_Q_RAW
  for (unsigned int i = 0; i < RawData->GetCATSMultY(); ++i) {   // loop on vector
    family = "CATS/RAW/STRQ";
    name   = "CATS"+NPL::itoa(RawData->GetCATSDetY(i))+"_STRY_Q_RAW";
    FillSpectra(family, name ,RawData->GetCATSStripY(i), RawData->GetCATSChargeY(i)); 
  } // end loop on vector

  // STRX_MULT
  int myMULT[fStripsNumber];
  for (unsigned int i = 0; i < fNumberOfCats; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < RawData->GetCATSMultX(); i++) myMULT[RawData->GetCATSDetX(i)-1] += 1;

  for (unsigned int i = 0; i < fNumberOfCats; i++) {
    name   = "CATS"+NPL::itoa(i+1)+"_STRX_MULT";
    family = "CATS/RAW/MULT";
    FillSpectra(family,name ,myMULT[i]);
  }

  // STRY_MULT
  for (unsigned int i = 0; i < fNumberOfCats; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < RawData->GetCATSMultY(); i++) myMULT[RawData->GetCATSDetY(i)-1] += 1;

  for (unsigned int i = 0; i < fNumberOfCats; i++) {
    name   = "CATS"+NPL::itoa(i+1)+"_STRY_MULT";
    family = "CATS/RAW/MULT";
    FillSpectra(family,name ,myMULT[i]);
  }
}

////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::FillPreTreatedSpectra(TCATSData* PreTreatedData){
  string name;
  string family;

  // CATS_STRX_Q_CAL
  for (unsigned int i = 0; i < PreTreatedData->GetCATSMultX(); ++i) {   // loop on vector
    family = "CATS/CAL/STRQ";
    name   = "CATS"+NPL::itoa(PreTreatedData->GetCATSDetX(i))+"_STRX_Q_CAL";
    FillSpectra(family,name ,PreTreatedData->GetCATSStripX(i), PreTreatedData->GetCATSChargeX(i)); 
  } // end loop on vector

  // CATS_STRY_Q_CAL
  for (unsigned int i = 0; i < PreTreatedData->GetCATSMultY(); ++i) {   // loop on vector
    family = "CATS/CAL/STRQ";
    name   = "CATS"+NPL::itoa(PreTreatedData->GetCATSDetY(i))+"_STRY_Q_CAL";
    FillSpectra(family,name ,PreTreatedData->GetCATSStripY(i), PreTreatedData->GetCATSChargeY(i)); 
  } // end loop on vector

  // STRX_MULT
  int myMULT[fStripsNumber];
  for (unsigned int i = 0; i < fNumberOfCats; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < PreTreatedData->GetCATSMultX(); i++) myMULT[PreTreatedData->GetCATSDetX(i)-1] += 1;

  for (unsigned int i = 0; i < fNumberOfCats; i++) {
    name   = "CATS"+NPL::itoa(i+1)+"_STRX_CAL_MULT";
    family = "CATS/CAL/MULT";
    FillSpectra(family,name ,myMULT[i]);
  }

  // STRY_MULT
  for (unsigned int i = 0; i < fNumberOfCats; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < PreTreatedData->GetCATSMultY(); i++) myMULT[PreTreatedData->GetCATSDetY(i)-1] += 1;

  for (unsigned int i = 0; i < fNumberOfCats; i++) {
    name   = "CATS"+NPL::itoa(i+1)+"_STRY_CAL_MULT";
    family = "CATS/CAL/MULT";
    FillSpectra(family,name ,myMULT[i]);
  }

}

////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::FillPhysicsSpectra(TCATSPhysics* Physics){
  string name,family;
  // CATS_STRX_Q_CAL
  for (unsigned int i = 0; i < Physics->DetNumberX.size() ; ++i) {   // loop on vector
    family="CATS/PHY/QSUM";
    name   = "CATS"+NPL::itoa(Physics->DetNumberX[i])+"_QSUM_STRMAX_X_CAL";
    FillSpectra(family,name ,Physics->StripMaxX[i],Physics->QsumX[i]); 
    name   = "CATS"+NPL::itoa(Physics->DetNumberX[i])+"_QSUM_STRMAX_Y_CAL";
    FillSpectra(family,name ,Physics->StripMaxY[i],Physics->QsumY[i]); 
  }
  // An histo of size fEventLoopSize is reset every fEventLoopSize to monitor the
  // Keep Track of how many event elapsed
  if(Physics->StripMaxX.size()==fNumberOfCats){
    fEventLoopIndex++;
    family = "CATS/PHY/CTRL";

    for (unsigned int i = 0; i < Physics->StripMaxX.size(); ++i) {
      fEventLoopQSum[i]+=Physics->QsumX[i]/1000000.;
      name = "CATS"+NPL::itoa(i+1)+"_QMEAN_TIME";

      GetSpectra(family,name) ->SetBinContent(fEventLoopIndex/fEventLoopIndex,fEventLoopQSum[i]/fEventLoopIndex);
    }
  }

  // Reset the mean every bin 
  if(fEventLoopIndex%fEventLoopStep>fEventLoopStep)
    for (unsigned int i = 0; i < fNumberOfCats; ++i)  
      fEventLoopQSum[i]=0;

  // Restart histo
  if(fEventLoopIndex > fEventLoopSize)
    fEventLoopIndex = 0 ;

  for (unsigned int i = 0; i < Physics->PositionX.size(); ++i) {
    family = "CATS/PHY/POS";
    name = "CATS"+NPL::itoa(Physics->DetMaxX[i])+"_POS";
    FillSpectra(family,name ,Physics->PositionX[i],Physics->PositionY[i]);

    name = "CATS_STRIP_"+NPL::itoa(Physics->DetMaxX[i])+"_POS";
    FillSpectra(family,name ,Physics->StripNumberX[i],Physics->StripNumberY[i]);
  }

  if(Physics->PositionOnTargetX > -1000 && Physics->PositionOnTargetY > -1000){
    name = "TARGET_POS";
    FillSpectra(family,name, Physics->PositionOnTargetX,Physics->PositionOnTargetY);

    name = "TRAJECTORY_XZ";
    TH1* histo1 = GetSpectra(family,name);

    name = "TRAJECTORY_YZ";
    TH1* histo2 = GetSpectra(family,name); 



    for(int i = 0 ; i < histo1->GetNbinsX() ; i++){ 
      double z = histo1->GetXaxis()->GetBinCenter(i);
      double PositionOnTargetX;
      double PositionOnTargetY;
      double PositionZ0 = Physics->PositionZ[0];
      double PositionZ1 = Physics->PositionZ[1];
      if(Physics->DetMaxX[0]<Physics->DetMaxX[2]){
        double t = -(PositionZ1-z)/(PositionZ1-PositionZ0);
        PositionOnTargetX= Physics->PositionX[1] + (Physics->PositionX[1]-Physics->PositionX[0])*t;
        PositionOnTargetY= Physics->PositionY[1] + (Physics->PositionY[1]-Physics->PositionY[0])*t; 
      }

      else{
        double t = -(PositionZ0-z)/(PositionZ0-PositionZ1);
        PositionOnTargetX= Physics->PositionX[0] + (Physics->PositionX[0]-Physics->PositionX[1])*t;
        PositionOnTargetY= Physics->PositionY[0] + (Physics->PositionY[0]-Physics->PositionY[1])*t; 
      }
    name = "TRAJECTORY_XZ";
    FillSpectra(family,name,z,PositionOnTargetX);

    name = "TRAJECTORY_YZ";
    FillSpectra(family,name,z,PositionOnTargetY); 

    }
  }
}

