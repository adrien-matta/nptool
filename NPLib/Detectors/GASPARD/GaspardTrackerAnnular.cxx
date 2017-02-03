/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:                  contact address:                        *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "GaspardTrackerAnnular.h"

// C++ headers
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <stdlib.h>

// Gaspard
#include "TGaspardTrackerPhysics.h"

using namespace std;


GaspardTrackerAnnular::GaspardTrackerAnnular(map<int, GaspardTrackerModule*> &Module,TGaspardTrackerPhysics* EventPhysics) 
  : m_ModuleTest(Module),
  m_EventPhysics(EventPhysics),
  m_EventData(0),
  m_PreTreatData(new TGaspardTrackerData),
  m_NumberOfModule(0),
  m_NumberOfStripsTheta(16),
  m_NumberOfStripsPhi(16)
{
}



GaspardTrackerAnnular::~GaspardTrackerAnnular()
{
  delete m_PreTreatData;
}



void GaspardTrackerAnnular::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("GaspardTracker");
  vector<string> tokenp= {"Z","RMIN","RMAX"};

  vector<string> token={"FIRSTSTAGE","SECONDSTAGE","THIRDSTAGE"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->GetMainValue() == "Annular" && blocks[i]->HasTokenList(token) ){
      bool first = blocks[i]->GetInt("FIRSTSTAGE");
      bool second = blocks[i]->GetInt("SECONDSTAGE");
      bool third = blocks[i]->GetInt("THIRDSTAGE");
      if(blocks[i]->HasTokenList(tokenp)){
        // Add module
        double Z    = blocks[i]->GetDouble("Z","mm");
        double Rmin = blocks[i]->GetDouble("Rmin","mm");
        double Rmax = blocks[i]->GetDouble("Rmax","mm");
        AddModule(Z, Rmin, Rmax);
        m_ModuleTest[m_index["Annular"] + m_NumberOfModule] = this;
      }
      else{
      cout << "ERROR: Check input formatting for Gaspard" << endl;
      exit(1);
      }
    }
  } 
}

void GaspardTrackerAnnular::PreTreat()
{
}



void GaspardTrackerAnnular::BuildPhysicalEvent()
{
  // Check flags
  //   bool Check_FirstStage  = false;
  bool Check_SecondStage = false; 
  bool Check_ThirdStage  = false;

  // Thresholds
  /*
     double FirstStage_Front_E_Threshold = 0; double FirstStage_Front_T_Threshold = 0;
     double FirstStage_Back_E_Threshold  = 0; double FirstStage_Back_T_Threshold  = 0;
     double SecondStage_E_Threshold      = 0; double SecondStage_T_Threshold      = 0;
     double ThirdStage_E_Threshold       = 0; double ThirdStage_T_Threshold       = 0;
     */
  // calculate multipicity in the first stage
  int multXE = m_EventData->GetGPDTrkFirstStageFrontEMult();
  int multYE = m_EventData->GetGPDTrkFirstStageBackEMult();
  int multXT = m_EventData->GetGPDTrkFirstStageFrontTMult();
  int multYT = m_EventData->GetGPDTrkFirstStageBackTMult();
  // calculate multiplicity of 2nd and third stages
  int mult2E = m_EventData->GetGPDTrkSecondStageEMult();
  int mult2T = m_EventData->GetGPDTrkSecondStageTMult();
  int mult3E = m_EventData->GetGPDTrkThirdStageEMult();
  int mult3T = m_EventData->GetGPDTrkThirdStageTMult();

  // Deal with multiplicity 1 for the first layer
  if (multXE==1 && multYE==1 && multXT==1 && multYT==1) {
    // calculate detector number
    int det_ref = m_EventData->GetGPDTrkFirstStageFrontEDetectorNbr(0);
    int detecXE = m_EventData->GetGPDTrkFirstStageFrontEDetectorNbr(0) / det_ref;
    int detecXT = m_EventData->GetGPDTrkFirstStageFrontTDetectorNbr(0) / det_ref;
    int detecYE = m_EventData->GetGPDTrkFirstStageBackEDetectorNbr(0) / det_ref;
    int detecYT = m_EventData->GetGPDTrkFirstStageBackTDetectorNbr(0) / det_ref;

    // case of same detector
    if (detecXE*detecXT*detecYE*detecYT == 1) {
      // store module number
      m_EventPhysics->SetModuleNumber(det_ref);
      // calculate strip number
      int stripXE = m_EventData->GetGPDTrkFirstStageFrontEStripNbr(0);
      int stripXT = m_EventData->GetGPDTrkFirstStageFrontTStripNbr(0);
      int stripYE = m_EventData->GetGPDTrkFirstStageBackEStripNbr(0);
      int stripYT = m_EventData->GetGPDTrkFirstStageBackTStripNbr(0);

      // case of same strips on X and Y
      if (stripXE == stripXT  &&  stripYE == stripYT) {        // here we have a good strip event
        // various
        //            Check_FirstStage = true;
        // store strip ID
        m_EventPhysics->SetFirstStageFrontPosition(stripXE);
        m_EventPhysics->SetFirstStageBackPosition(stripYE);
        // get energy from strips and store it
        double EnergyStripFront = m_EventData->GetGPDTrkFirstStageFrontEEnergy(0);
        m_EventPhysics->SetFirstStageEnergy(EnergyStripFront);
        double EnergyTot = EnergyStripFront;
        // get time from strips and store it
        double TimeStripBack  = m_EventData->GetGPDTrkFirstStageBackTTime(0);
        m_EventPhysics->SetFirstStageTime(TimeStripBack);

        // check if we have a 2nd stage event
        if (mult2E==1 && mult2T==1) {
          Check_SecondStage = true;
          double EnergySecond = m_EventData->GetGPDTrkSecondStageEEnergy(0);
          m_EventPhysics->SetSecondStageEnergy(EnergySecond);
          EnergyTot += EnergySecond;
        }
        else if (mult2E>1 || mult2T>1) {
          cout << "Warning: multiplicity in second stage greater than in firststage" << endl;
        }

        // check if we have a third stage event
        if (mult3E==1 && mult3T==1) {
          Check_ThirdStage = true;
          double EnergyThird = m_EventData->GetGPDTrkThirdStageEEnergy(0);
          m_EventPhysics->SetThirdStageEnergy(EnergyThird);
          EnergyTot += EnergyThird;
        }
        else if (mult3E>1 || mult3T>1) {
          cout << "Warning: multiplicity in third stage greater than in firststage" << endl;
        }

        // Fill total energy
        m_EventPhysics->SetTotalEnergy(EnergyTot);

        // Fill default values for second an third stages
        if (!Check_SecondStage) {
          m_EventPhysics->SetSecondStageEnergy(-1000);
          m_EventPhysics->SetSecondStageTime(-1000);
          m_EventPhysics->SetSecondStagePosition(-1000);
        }
        if (!Check_ThirdStage) {
          m_EventPhysics->SetThirdStageEnergy(-1000);
          m_EventPhysics->SetThirdStageTime(-1000);
          m_EventPhysics->SetThirdStagePosition(-1000);
        }
      }
      else {
        cout << "Not same strips" << endl;
      }
    }
    else {
      cout << "Not same detector" << endl;
    }
  }
  else {
    /*      cout << "Multiplicity is not one, it is: " << endl;
            cout << "\tmultXE: " << multXE << endl;
            cout << "\tmultXT: " << multXT << endl;
            cout << "\tmultYE: " << multYE << endl;
            cout << "\tmultYT: " << multYT << endl;*/
  }
}



void GaspardTrackerAnnular::BuildSimplePhysicalEvent()
{
}




void GaspardTrackerAnnular::AddModule(double zpos, double rmin, double rmax)
{
  m_NumberOfModule++;

  // Theta & phi strips pitch
  double thetaPitch = (rmax - rmin) / m_NumberOfStripsTheta;
  double phiPitch   = 2.*M_PI / m_NumberOfStripsPhi;

  // Buffer object to fill Position Array
  vector<double> lineX;
  vector<double> lineY;
  vector<double> lineZ;

  vector< vector< double > >   OneModuleStripPositionX;
  vector< vector< double > >   OneModuleStripPositionY;
  vector< vector< double > >   OneModuleStripPositionZ;

  // loop on theta strips
  for (int i = 0; i < m_NumberOfStripsTheta; i++) {
    lineX.clear();
    lineY.clear();
    lineZ.clear();
    double x = 0;
    double y = 0;
    // center of theta strip
    double r = rmin + thetaPitch*0.5 + thetaPitch*i ;

    // loop on phi strips
    for (int j = 0; j < m_NumberOfStripsPhi; j++) {
      // center of phi strips
      double phi = phiPitch*0.5 + phiPitch*j;
      // calculate x and y projections
      x = r * cos(phi);
      y = r * sin(phi);
      if (zpos < 0) 
        y *= -1;

      // fill lineX,Y,Z
      lineX.push_back(x);
      lineY.push_back(y);
      lineZ.push_back(zpos);
    }
    OneModuleStripPositionX.push_back(lineX);
    OneModuleStripPositionY.push_back(lineY);
    OneModuleStripPositionZ.push_back(lineZ);
  }
  m_StripPositionX.push_back( OneModuleStripPositionX );
  m_StripPositionY.push_back( OneModuleStripPositionY );
  m_StripPositionZ.push_back( OneModuleStripPositionZ );
}
