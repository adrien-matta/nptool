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
#include "GaspardTrackerSquare.h"

// C++ headers
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <stdlib.h>

// Gaspard
#include "TGaspardTrackerPhysics.h"


GaspardTrackerSquare::GaspardTrackerSquare(map<int, GaspardTrackerModule*> &Module,
                                           TGaspardTrackerPhysics* EventPhysics) 
   : m_ModuleTest(Module),
     m_EventPhysics(EventPhysics),
     m_EventData(0),
     m_PreTreatData(new TGaspardTrackerData),
     m_NumberOfModule(0),
     m_FirstStageFace(98), // mm
     m_NumberOfStrips(128)
{
   m_StripPitch = m_FirstStageFace / (double)m_NumberOfStrips;
}



GaspardTrackerSquare::~GaspardTrackerSquare(){
   delete m_PreTreatData;
}



void GaspardTrackerSquare::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("GaspardTracker");
  vector<string> token_cart= {"X1_Y1","X128_Y1","X1_Y128","X128_Y128"};
  vector<string> token_sphe= {"R","THETA","PHI","BETA"};

  vector<string> token={"FIRSTSTAGE","SECONDSTAGE","THIRDSTAGE"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->GetMainValue() == "Square" && blocks[i]->HasTokenList(token) ){
      if(blocks[i]->HasTokenList(token_cart)){
        // Add module
        TVector3 A = blocks[i]->GetTVector3("X1_Y1","mm");
        TVector3 B = blocks[i]->GetTVector3("X128_Y1","mm");
        TVector3 C = blocks[i]->GetTVector3("X1_Y128","mm");
        TVector3 D = blocks[i]->GetTVector3("X128_Y128","mm");
      
        AddModule(A,B,C,D);
        m_ModuleTest[m_index["Square"] + m_NumberOfModule] = this;
      }
     else if(blocks[i]->HasTokenList(token_sphe)){
        // Add module
        double R = blocks[i]->GetDouble("R","mm");
        double Theta = blocks[i]->GetDouble("THETA","deg");
        double Phi = blocks[i]->GetDouble("PHI","deg");
        vector<double> beta = blocks[i]->GetVectorDouble("BETA","deg");
      
        AddModule(R,Theta,Phi,beta[0],beta[1],beta[2]);
        m_ModuleTest[m_index["Square"] + m_NumberOfModule] = this;
      }
    }
  } 
}





void GaspardTrackerSquare::PreTreat(){
}



void GaspardTrackerSquare::BuildPhysicalEvent(){
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
            double TimeStripBack  = m_EventData->GetGPDTrkFirstStageBackEEnergy(0);
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

            // check strip position
/*            cout << "strips X, Y : " << stripXE << "  " << stripYE << endl;
            cout << "position : " << endl;
            cout << "\t X: " << GetStripPositionX(det_ref + m_index["Square"], stripXE, stripYE) << endl;
            cout << "\t Y: " << GetStripPositionY(det_ref + m_index["Square"], stripXE, stripYE) << endl;
            cout << "\t Z: " << GetStripPositionZ(det_ref + m_index["Square"], stripXE, stripYE) << endl;
*/
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



void GaspardTrackerSquare::BuildSimplePhysicalEvent(){
}



void GaspardTrackerSquare::AddModule(TVector3 C_X1_Y1,
                                     TVector3 C_X128_Y1,
                                     TVector3 C_X1_Y128,
                                     TVector3 C_X128_Y128)
{
   m_NumberOfModule++;

   // remove warning using C_X128_Y128
   C_X128_Y128.Unit();

   // Vector U on Module Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
   TVector3 U = C_X128_Y1 - C_X1_Y1;
   U = U.Unit();
   // Vector V on Module Face (parallele to X Strip)
   TVector3 V = C_X1_Y128 - C_X1_Y1;
   V = V.Unit();
  
   
   // Position Vector of Strip Center
   TVector3 StripCenter = TVector3(0,0,0);
   // Position Vector of X=1 Y=1 Strip 
   TVector3 Strip_1_1;

   // Buffer object to fill Position Array
   vector<double> lineX;
   vector<double> lineY;
   vector<double> lineZ;

   vector< vector< double > >   OneModuleStripPositionX;
   vector< vector< double > >   OneModuleStripPositionY;
   vector< vector< double > >   OneModuleStripPositionZ;

   // Moving StripCenter to 1.1 corner:
   Strip_1_1 = C_X1_Y1 + (U+V) * m_StripPitch*0.5;
   for (int i = 0; i < m_NumberOfStrips; i++) {
      lineX.clear();
      lineY.clear();
      lineZ.clear();

      for (int j = 0; j < m_NumberOfStrips; j++) {
         StripCenter  = Strip_1_1 + m_StripPitch*(i*U + j*V);

         lineX.push_back( StripCenter.X() );
         lineY.push_back( StripCenter.Y() );
         lineZ.push_back( StripCenter.Z() );
      }

      OneModuleStripPositionX.push_back(lineX);
      OneModuleStripPositionY.push_back(lineY);
      OneModuleStripPositionZ.push_back(lineZ);
   }

   m_StripPositionX.push_back( OneModuleStripPositionX );
   m_StripPositionY.push_back( OneModuleStripPositionY );
   m_StripPositionZ.push_back( OneModuleStripPositionZ );
}



void GaspardTrackerSquare::AddModule(double theta,
                                     double phi,
                                     double distance,
                                     double beta_u,
                                     double beta_v,
                                     double beta_w)
{
  m_NumberOfModule++;

   // convert from degree to radian:
   theta *= M_PI/180;
   phi   *= M_PI/180;

   // Vector U on Module Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
   TVector3 U ;
   // Vector V on Module Face (parallele to X Strip)
   TVector3 V ;
   // Vector W normal to Module Face (pointing CsI)
   TVector3 W ;
   // Vector position of Module Face center
   TVector3 C ;

   C = TVector3(distance * sin(theta) * cos(phi),
                distance * sin(theta) * sin(phi),
                distance * cos(theta));

  TVector3 YperpW = TVector3( cos(theta) * cos(phi),
                              cos(theta) * sin(phi),
                             -sin(theta));

   W = C.Unit();
   U = W.Cross(YperpW);
   V = W.Cross(U);

   U = U.Unit();
   V = V.Unit();

   U.Rotate( beta_u * M_PI/180. , U ) ;
   V.Rotate( beta_u * M_PI/180. , U ) ;

   U.Rotate( beta_v * M_PI/180. , V ) ;
   V.Rotate( beta_v * M_PI/180. , V ) ;

   U.Rotate( beta_w * M_PI/180. , W ) ;
   V.Rotate( beta_w * M_PI/180. , W ) ;

   // Position Vector of Strip Center
   TVector3 StripCenter = TVector3(0,0,0);
   // Position Vector of X=1 Y=1 Strip 
   TVector3 Strip_1_1;

   vector<double> lineX;
   vector<double> lineY;
   vector<double> lineZ;

   vector< vector< double > >   OneModuleStripPositionX;
   vector< vector< double > >   OneModuleStripPositionY;
   vector< vector< double > >   OneModuleStripPositionZ;

   // Moving C to the 1.1 corner:
   Strip_1_1 = C - 0.5 * (m_FirstStageFace - m_StripPitch) * (U + V);

   for (int i = 0; i < m_NumberOfStrips; i++) {
      lineX.clear();
      lineY.clear();
      lineZ.clear();

      for (int j = 0; j < m_NumberOfStrips; j++) {
         StripCenter = Strip_1_1 + m_StripPitch * (i*U + j*V);

         lineX.push_back(StripCenter.X());
         lineY.push_back(StripCenter.Y());
         lineZ.push_back(StripCenter.Z());
      }

      OneModuleStripPositionX.push_back(lineX);
      OneModuleStripPositionY.push_back(lineY);
      OneModuleStripPositionZ.push_back(lineZ);
   }

   m_StripPositionX.push_back( OneModuleStripPositionX );
   m_StripPositionY.push_back( OneModuleStripPositionY );
   m_StripPositionZ.push_back( OneModuleStripPositionZ );
}
