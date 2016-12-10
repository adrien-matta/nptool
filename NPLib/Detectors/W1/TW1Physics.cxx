/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : november 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *    This class hold W1  Physics                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
//  NPL
#include "TW1Physics.h"
using namespace W1_LOCAL;

//  STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <cmath>
#include <stdlib.h>
using namespace std;

// NPTOOL
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"

//  ROOT
#include "TChain.h"


ClassImp(TW1Physics)


///////////////////////////////////////////////////////////////////////////
TW1Physics::TW1Physics()
   : m_EventData(new TW1Data),
     m_PreTreatedData(new TW1Data),
     m_EventPhysics(this),
     m_Spectra(0),
     m_nCounter(10),
     m_MaximumStripMultiplicityAllowed(1),   // multiplidity 1
     m_StripEnergyMatchingSigma(0.060),      // MeV
     m_StripEnergyMatchingNumberOfSigma(5),  // MeV
     m_FrontE_Raw_Threshold(0),     // adc channels
     m_BackE_Raw_Threshold(0),      // adc channels
     m_FrontE_Calib_Threshold(0),   // MeV
     m_BackE_Calib_Threshold(0),    // MeV
     m_NumberOfDetectors(0),
     m_SiliconFace(49.6),  // mm
     m_NumberOfStrips(16)
{    
   m_StripPitch = m_SiliconFace / (double)m_NumberOfStrips;

   for (Int_t i = 0; i < m_nCounter; ++i) {   // loop on counters
      m_Counter[i] = 0;
   } // end loop on counters
}



///////////////////////////////////////////////////////////////////////////
//TW1Physics::~TW1Physics()
//{
//   delete m_EventData;
//   delete m_PreTreatedData;
//}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::Clear()
{
   fEventType.clear();
   fDetectorNumber.clear();
   fFrontEnergy.clear();
   fBackEnergy.clear();
   fHalfEnergy.clear();
   fFrontTime.clear();
   fBackTime.clear();
   fFrontStrip.clear();
   fBackStrip.clear();

   for (Int_t i = 0; i < m_nCounter; ++i) {   // loop on counters
      m_Counter[i] = 0;
   } // end loop on counters
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("W1");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 
  for(unsigned int i  = 0 ; i < blocks.size() ; i++){
    // Cartesian Case
    vector<string> cart = {"X1_Y1","X1_Y16","X16_Y1","X16_Y16"};
    // Spherical Case
    vector<string> sphe= {"R","THETA","PHI","BETA"};

    if(blocks[i]->HasTokenList(cart)){
      cout << endl << "////  W1 " << i+1 << endl;
      TVector3 A = blocks[i]->GetTVector3("X1_Y1","mm");
      TVector3 B = blocks[i]->GetTVector3("X16_Y1","mm");
      TVector3 C = blocks[i]->GetTVector3("X1_Y16","mm");
      TVector3 D = blocks[i]->GetTVector3("X16_Y16","mm");
      AddDetector(A,B,C,D) ;
    }

    else if(blocks[i]->HasTokenList(sphe)){
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi= blocks[i]->GetDouble("PHI","deg");
      double R = blocks[i]->GetDouble("R","mm");
      vector<double> beta = blocks[i]->GetVectorDouble("BETA","deg");
      AddDetector(Theta,Phi,R,beta[0],beta[1],beta[2]);
    }

    else{
      cout << "ERROR: Missing token for W1 blocks, check your input file" << endl;
      exit(1);
    }

  }

  InitializeStandardParameters();
  ReadAnalysisConfig();
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::AddParameterToCalibrationManager()
{
   CalibrationManager* Cal = CalibrationManager::getInstance();
    
   for (Int_t i = 0; i < m_NumberOfDetectors; i++) {
      for (Int_t j = 0; j < m_NumberOfStrips; j++) {
         // Energy
         Cal->AddParameter("W1", "D"+ NPL::itoa(i+1)+ "_Front" + NPL::itoa(j) + "_E", "W1_D"+ NPL::itoa(i+1) + "_FRONT" + NPL::itoa(j) + "_E");
         Cal->AddParameter("W1", "D"+ NPL::itoa(i+1)+ "_Back"  + NPL::itoa(j) + "_E", "W1_D"+ NPL::itoa(i+1) + "_BACK"  + NPL::itoa(j) + "_E");  
         // Time
         Cal->AddParameter("W1", "D"+ NPL::itoa(i+1)+ "_Front" + NPL::itoa(j) + "_T", "W1_D"+ NPL::itoa(i+1) + "_FRONT" + NPL::itoa(j) + "_T");
         Cal->AddParameter("W1", "D"+ NPL::itoa(i+1)+ "_Back"  + NPL::itoa(j) + "_T", "W1_D"+ NPL::itoa(i+1) + "_BACK"  + NPL::itoa(j) + "_T");  
      }
   }
}



///////////////////////////////////////////////////////////////////////////
void  TW1Physics::InitializeRootInputRaw() 
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus("W1"   , true);
   inputChain->SetBranchStatus("fW1_*", true);
   inputChain->SetBranchAddress("W1"  , &m_EventData);
}



///////////////////////////////////////////////////////////////////////////
void  TW1Physics::InitializeRootInputPhysics() 
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus("W1"   , true);
   inputChain->SetBranchStatus("fEventType", true);
   inputChain->SetBranchStatus("fDetectorNumber", true);
   inputChain->SetBranchStatus("fEnergy", true);
   inputChain->SetBranchStatus("fTime", true);
   inputChain->SetBranchStatus("fFrontStrip", true);
   inputChain->SetBranchStatus("fBackStrip", true);
   inputChain->SetBranchAddress("W1"  , &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::InitializeRootOutput()
{
   TTree* outputTree = RootOutput::getInstance()->GetTree();
   outputTree->Branch("W1", "TW1Physics", &m_EventPhysics);
}



void TW1Physics::AddDetector(TVector3 C_X1_Y1,  TVector3 C_X16_Y1,
                             TVector3 C_X1_Y16, TVector3 C_X16_Y16)
{
   m_NumberOfDetectors++;

   // remove warning using C_X16_Y16
   C_X16_Y16.Unit();

   // Vector U on Module Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
   TVector3 U = C_X16_Y1 - C_X1_Y1;
   U = U.Unit();

   // Vector V on Module Face (parallele to X Strip)
   TVector3 V = C_X1_Y16 - C_X1_Y1;
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
   Strip_1_1 = C_X1_Y1 + (U+V) * (m_StripPitch/2.);

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



void TW1Physics::AddDetector(double theta, double phi, double distance,
                             double beta_u, double beta_v, double beta_w)
{
   m_NumberOfDetectors++;

   // convert from degree to radian:
   theta *= M_PI/180;
   phi   *= M_PI/180;

   // Vector U on Module Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
   TVector3 U;
   // Vector V on Module Face (parallele to X Strip)
   TVector3 V;
   // Vector W normal to Module Face (pointing CsI)
   TVector3 W;
   // Vector position of Module Face center
   TVector3 C;

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

   U.Rotate(beta_u * M_PI/180, U);
   V.Rotate(beta_u * M_PI/180, U);

   U.Rotate(beta_v * M_PI/180, V);
   V.Rotate(beta_v * M_PI/180, V);

   U.Rotate(beta_w * M_PI/180, W);
   V.Rotate(beta_w * M_PI/180, W);

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
   Strip_1_1 = C - 0.5 * (m_SiliconFace - m_StripPitch) * (U + V);

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



TVector3 TW1Physics::GetPositionOfInteraction(int i)
{
   TVector3 Position = TVector3(GetStripPositionX(fDetectorNumber[i], fFrontStrip[i], fBackStrip[i]),
                                GetStripPositionY(fDetectorNumber[i], fFrontStrip[i], fBackStrip[i]),
                                GetStripPositionZ(fDetectorNumber[i], fFrontStrip[i], fBackStrip[i]));

   return Position;
}



TVector3 TW1Physics::GetDetectorNormal(int i)
{
   TVector3 U = TVector3(GetStripPositionX(fDetectorNumber[i], m_NumberOfStrips, 1),
                         GetStripPositionY(fDetectorNumber[i], m_NumberOfStrips, 1),
                         GetStripPositionZ(fDetectorNumber[i], m_NumberOfStrips, 1))

              - TVector3(GetStripPositionX(fDetectorNumber[i], 1, 1),
                         GetStripPositionY(fDetectorNumber[i], 1, 1),
                         GetStripPositionZ(fDetectorNumber[i], 1, 1));


   TVector3 V = TVector3(GetStripPositionX(fDetectorNumber[i], m_NumberOfStrips, m_NumberOfStrips),
                         GetStripPositionY(fDetectorNumber[i], m_NumberOfStrips, m_NumberOfStrips),
                         GetStripPositionZ(fDetectorNumber[i], m_NumberOfStrips, m_NumberOfStrips))

              - TVector3(GetStripPositionX(fDetectorNumber[i], m_NumberOfStrips, 1),
                         GetStripPositionY(fDetectorNumber[i], m_NumberOfStrips, 1),
                         GetStripPositionZ(fDetectorNumber[i], m_NumberOfStrips, 1));

   TVector3 Normal = U.Cross(V);

   return Normal.Unit();
}



void TW1Physics::DumpStrippingScheme(Int_t detecNumber)
{
   cout << endl << "TW1Physics::DumpStrippingScheme()" << endl;
   cout << "Detector number " << detecNumber << endl;

   for (int i = 1; i < m_NumberOfStrips+1; i++) {   // front part
      for (int j = 1; j < m_NumberOfStrips+1; j++) {   // back part
         cout << "strips Front, Back: " << i << "  " << j << "\t--->\t (X,Y,Z) mm: "
            << GetStripPositionX(detecNumber, i, j) << "\t"
            << GetStripPositionY(detecNumber, i, j) << "\t"
            << GetStripPositionZ(detecNumber, i, j) << endl;
      }
   }
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::BuildPhysicalEvent()
{
   BuildSimplePhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::BuildSimplePhysicalEvent()
{
   // Select active channels and apply thresholds
   PreTreat();
   m_Counter[0] = 1;

   // Begin treatement
   Int_t evtType = EventType();

   if (evtType == 1) {  // case where multiplicity front = multiplicity back
      m_Counter[1] = 1;
      vector<TVector2> couple = Match_Front_Back();

      for (UShort_t i = 0; i < couple.size(); i++) { // loop on selected events
         Int_t    DetecNbr    = m_PreTreatedData->GetFrontEDetectorNbr(couple[i].X());
         Int_t    StripFront  = m_PreTreatedData->GetFrontEStripNbr(couple[i].X());
         Int_t    StripBack   = m_PreTreatedData->GetBackEStripNbr(couple[i].Y());
         Double_t EnergyFront = m_PreTreatedData->GetFrontEEnergy(couple[i].X());
         Double_t EnergyBack  = m_PreTreatedData->GetBackEEnergy(couple[i].Y());

         // Search for associate time
         // Front
         Double_t TimeFront = -1000;
         for (UShort_t t = 0; t < m_PreTreatedData->GetFrontTMult(); t++) {
            if (m_PreTreatedData->GetFrontTStripNbr(couple[i].X()) == m_PreTreatedData->GetFrontTStripNbr(t) &&
                m_PreTreatedData->GetFrontTDetectorNbr(couple[i].X()) == m_PreTreatedData->GetFrontTDetectorNbr(t)) {
               TimeFront = m_PreTreatedData->GetFrontTTime(t);
               m_Counter[4] = 1;
            }
         }
         // Back
         Double_t TimeBack = -1000;
         for (UShort_t t = 0; t < m_PreTreatedData->GetBackTMult(); t++) {
            if (m_PreTreatedData->GetBackTStripNbr(couple[i].Y()) == m_PreTreatedData->GetBackTStripNbr(t) &&
                m_PreTreatedData->GetBackTDetectorNbr(couple[i].Y()) == m_PreTreatedData->GetBackTDetectorNbr(t))
               TimeBack = m_PreTreatedData->GetBackTTime(t);
         }

         // Fill TW1Physics private members
         fEventType.push_back(evtType);
         fDetectorNumber.push_back(DetecNbr);
         fFrontEnergy.push_back(EnergyFront);
         fBackEnergy.push_back(EnergyBack);
         fHalfEnergy.push_back((EnergyFront+EnergyBack)/2);
         fFrontTime.push_back(TimeFront);
         fBackTime.push_back(TimeBack);
         fFrontStrip.push_back(StripFront);
         fBackStrip.push_back(StripBack);
      }
   }
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::PreTreat()
{
   // Clear pre treated object
   ClearPreTreatedData();
      
   // (Front, E)
   for (UShort_t i = 0; i < m_EventData->GetFrontEMult(); i++) {
      if (IsValidChannel("Front", m_EventData->GetFrontEDetectorNbr(i), m_EventData->GetFrontEStripNbr(i)) &&
          m_EventData->GetFrontEEnergy(i) > m_FrontE_Raw_Threshold) {
         Double_t E = fW1_Front_E(m_EventData, i);
         if (E > m_FrontE_Calib_Threshold) {
            m_PreTreatedData->SetFrontEDetectorNbr(m_EventData->GetFrontEDetectorNbr(i));
            m_PreTreatedData->SetFrontEStripNbr(m_EventData->GetFrontEStripNbr(i));
            m_PreTreatedData->SetFrontEEnergy(E);
         }
      } 
   }
   // (Front, T)
   for (UShort_t i = 0; i < m_EventData->GetFrontTMult(); i++) {
      if (IsValidChannel("Front", m_EventData->GetFrontTDetectorNbr(i), m_EventData->GetFrontTStripNbr(i))) {
         Double_t T = fW1_Front_T(m_EventData, i);
         m_PreTreatedData->SetFrontTDetectorNbr(m_EventData->GetFrontTDetectorNbr(i));
         m_PreTreatedData->SetFrontTStripNbr(m_EventData->GetFrontTStripNbr(i));
         m_PreTreatedData->SetFrontTTime(T);
      } 
   }

   // (Back, E)
   for (UShort_t i = 0; i < m_EventData->GetBackEMult(); i++) {
      if (IsValidChannel("Back", m_EventData->GetBackEDetectorNbr(i), m_EventData->GetBackEStripNbr(i)) &&
          m_EventData->GetBackEEnergy(i) > m_BackE_Raw_Threshold) {
         Double_t E = fW1_Back_E(m_EventData, i);
         if (E > m_BackE_Calib_Threshold) {
            m_PreTreatedData->SetBackEDetectorNbr(m_EventData->GetBackEDetectorNbr(i));
            m_PreTreatedData->SetBackEStripNbr(m_EventData->GetBackEStripNbr(i));
            m_PreTreatedData->SetBackEEnergy(E);
         }
      } 
   }
   // (Back, T)
   for (UShort_t i = 0; i < m_EventData->GetBackTMult(); i++) {
      if (IsValidChannel("Back", m_EventData->GetBackTDetectorNbr(i), m_EventData->GetBackTStripNbr(i))) {
         Double_t T = fW1_Back_T(m_EventData, i);
         m_PreTreatedData->SetBackTDetectorNbr(m_EventData->GetBackTDetectorNbr(i));
         m_PreTreatedData->SetBackTStripNbr(m_EventData->GetBackTStripNbr(i));
         m_PreTreatedData->SetBackTTime(T);
      } 
   }
}



///////////////////////////////////////////////////////////////////////////
Int_t TW1Physics::EventType()
{
   // Same multiplicity on front and back side
   if (m_PreTreatedData->GetFrontEMult() == m_PreTreatedData->GetBackEMult()) {
      return 1;
   }
   // Possibly interstrip
   else if (m_PreTreatedData->GetFrontEMult() == m_PreTreatedData->GetBackEMult()+1 ||
            m_PreTreatedData->GetFrontEMult() == m_PreTreatedData->GetBackEMult()-1) {
      return 2;
   }
   // Rejected event
   else {
      return -1;
   }
}



///////////////////////////////////////////////////////////////////////////
vector<TVector2> TW1Physics::Match_Front_Back()
{
   vector<TVector2> ArrayOfGoodCouple;

   // Select allowed multiplicity events. If multiplicity is too 
   // high, then return "empty" vector
   if (m_PreTreatedData->GetFrontEMult() > m_MaximumStripMultiplicityAllowed || 
       m_PreTreatedData->GetBackEMult()  > m_MaximumStripMultiplicityAllowed)
      return ArrayOfGoodCouple;

   // Loop on front multiplicity
   for (UShort_t i = 0; i < m_PreTreatedData->GetFrontEMult(); i++) {
      // Loop on back multiplicity
      for (UShort_t j = 0; j < m_PreTreatedData->GetBackEMult(); j++) {
         // if same detector check energy
         if (m_PreTreatedData->GetFrontEDetectorNbr(i) == m_PreTreatedData->GetBackEDetectorNbr(j)) {
            m_Counter[2] = 1;
            // Equal energy
            if (abs((m_PreTreatedData->GetFrontEEnergy(i)-m_PreTreatedData->GetBackEEnergy(j))/2.) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma) {
               ArrayOfGoodCouple.push_back(TVector2(i,j));
               m_Counter[3] = 1;
            }  // end test energy
         }  // end test same detector
      }  // end loop back multiplicity
   }  // end loop front multiplicity

   // Prevent treating event with ambiguous matchin beetween X and Y
   if (ArrayOfGoodCouple.size() > m_PreTreatedData->GetFrontEMult()) ArrayOfGoodCouple.clear();

   return ArrayOfGoodCouple;
}



///////////////////////////////////////////////////////////////////////////
bool TW1Physics::IsValidChannel(string Type, int detector, int channel)
{
   if (Type == "Front") {
      return *(m_FrontChannelStatus[detector-1].begin()+channel);
   }
   else if (Type == "Back")
      return *(m_BackChannelStatus[detector-1].begin()+channel);

   else 
      return false;
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::InitializeStandardParameters()
{
   // Enable all channels
   vector<bool> ChannelStatus;
   m_FrontChannelStatus.clear();
   m_BackChannelStatus.clear();

   ChannelStatus.resize(m_NumberOfStrips, true);
   for (Int_t i = 0; i < m_NumberOfDetectors; i++) {
      m_FrontChannelStatus[i] = ChannelStatus;
      m_BackChannelStatus[i]  = ChannelStatus;
   }
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::ReadAnalysisConfig()
{
   bool ReadingStatus = false;

   cout << "\t/////////// Reading ConfigW1.dat file ///////////" << endl;

   // path to file
   string FileName = "./configs/ConfigW1.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << "\tNo ConfigW1.dat found: default parameters loaded for Analysis " << FileName << endl;
      return;
   }
   cout << "\tLoading user parameters from ConfigW1.dat " << endl;

   // storing config file in the ROOT output file
   TAsciiFile *asciiFile = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
   asciiFile->AppendLine("%% ConfigW1.dat %%");
   asciiFile->Append(FileName.c_str());
   asciiFile->AppendLine("");

   // read analysis config file
   string LineBuffer, DataBuffer, whatToDo;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 8, "ConfigW1") == 0) ReadingStatus = true;

      // loop on tokens and data
      while (ReadingStatus) {
         whatToDo = "";
         AnalysisConfigFile >> whatToDo;

         // Search for comment symbol (%)
         if (whatToDo.compare(0, 1, "%") == 0) {
            AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
         }
         
         else if (whatToDo == "MAX_STRIP_MULTIPLICITY") {
            AnalysisConfigFile >> DataBuffer;
            m_MaximumStripMultiplicityAllowed = atoi(DataBuffer.c_str());
            cout << "\t" << whatToDo << "\t" << m_MaximumStripMultiplicityAllowed << endl;
         }

         else if (whatToDo == "FRONT_BACK_ENERGY_MATCHING_SIGMA") {
            AnalysisConfigFile >> DataBuffer;
            m_StripEnergyMatchingSigma = atof(DataBuffer.c_str() );
            cout << "\t" << whatToDo << "\t" << m_StripEnergyMatchingSigma << endl;
         }

         else if (whatToDo == "FRONT_BACK_ENERGY_MATCHING_NUMBER_OF_SIGMA") {
            AnalysisConfigFile >> DataBuffer;
            m_StripEnergyMatchingNumberOfSigma = atoi(DataBuffer.c_str());
            cout << "\t" << whatToDo << "\t" << m_StripEnergyMatchingNumberOfSigma << endl;
         }

         else if (whatToDo=="FRONT_E_RAW_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_FrontE_Raw_Threshold = atoi(DataBuffer.c_str());
            cout << "\t" << whatToDo << "\t" << m_FrontE_Raw_Threshold << endl;
         }

         else if (whatToDo=="BACK_E_RAW_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_BackE_Raw_Threshold = atoi(DataBuffer.c_str());
            cout << "\t" << whatToDo << "\t" << m_BackE_Raw_Threshold << endl;
         }

         else if (whatToDo=="FRONT_E_CAL_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_FrontE_Calib_Threshold = atoi(DataBuffer.c_str());
            cout << "\t" << whatToDo << "\t" << m_FrontE_Calib_Threshold << endl;
         }

         else if (whatToDo=="BACK_E_CAL_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_BackE_Calib_Threshold = atoi(DataBuffer.c_str());
            cout << "\t" << whatToDo << "\t" << m_BackE_Calib_Threshold << endl;
         }

         else if (whatToDo == "DISABLE_ALL") {
            AnalysisConfigFile >> DataBuffer;
            cout << "\t" << whatToDo << "\t" << DataBuffer << endl;
            Int_t Detector = atoi(DataBuffer.substr(4,1).c_str());
            vector< bool > ChannelStatus;
            ChannelStatus.resize(m_NumberOfStrips, false);
            m_FrontChannelStatus[Detector-1] = ChannelStatus;
            m_BackChannelStatus[Detector-1]  = ChannelStatus;
         }

         else if (whatToDo == "DISABLE_CHANNEL") {
            AnalysisConfigFile >> DataBuffer;
            cout << "\t" << whatToDo << "\t" << DataBuffer << endl;
            int detector = atoi(DataBuffer.substr(4,1).c_str());
            int channel = -1;
            if (DataBuffer.compare(6,5,"FRONT") == 0) {
               channel = atoi(DataBuffer.substr(12).c_str());
               *(m_FrontChannelStatus[detector-1].begin()+channel) = false;
            }
            else if (DataBuffer.compare(6,4,"BACK") == 0) {
               channel = atoi(DataBuffer.substr(11).c_str());
               *(m_BackChannelStatus[detector-1].begin()+channel) = false;
            }
         }
         else {
            ReadingStatus = false;
         }
      }
   }
   cout << "\t/////////////////////////////////////////////////" << endl;
}   



///////////////////////////////////////////////////////////////////////////
void TW1Physics::InitSpectra()
{
   m_Spectra = new TW1Spectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::FillSpectra()
{
   m_Spectra->FillRawSpectra(m_EventData);
   m_Spectra->FillPreTreatedSpectra(m_PreTreatedData);
   m_Spectra->FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::CheckSpectra()
{
   m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::ClearSpectra()
{
   // To be done
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::WriteSpectra()
{
     m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
map<string, TH1*> TW1Physics::GetSpectra()
{
   if (m_Spectra)
      return m_Spectra->GetMapHisto();
   else {
      map<string, TH1*> empty;
      return empty;
   }
}



///////////////////////////////////////////////////////////////////////////
Double_t W1_LOCAL::fW1_Front_E(TW1Data* m_EventData , Int_t i)
{
   return CalibrationManager::getInstance()->ApplyCalibration("W1/D" + NPL::itoa(m_EventData->GetFrontEDetectorNbr(i)) + "_Front" + NPL::itoa(m_EventData->GetFrontEStripNbr(i)) + "_E",  m_EventData->GetFrontEEnergy(i));
}



Double_t W1_LOCAL::fW1_Back_E(TW1Data* m_EventData , Int_t i)
{
   return CalibrationManager::getInstance()->ApplyCalibration("W1/D" + NPL::itoa(m_EventData->GetBackEDetectorNbr(i)) + "_Back" + NPL::itoa(m_EventData->GetBackEStripNbr(i)) + "_E",  m_EventData->GetBackEEnergy(i));
}
  
 

Double_t W1_LOCAL::fW1_Front_T(TW1Data* m_EventData , Int_t i)
{
   return CalibrationManager::getInstance()->ApplyCalibration("W1/D" + NPL::itoa(m_EventData->GetFrontTDetectorNbr(i)) + "_Front" + NPL::itoa(m_EventData->GetFrontTStripNbr(i)) + "_T",  m_EventData->GetFrontTTime(i));
}
 
 

Double_t W1_LOCAL::fW1_Back_T(TW1Data* m_EventData , Int_t i)
{
   return CalibrationManager::getInstance()->ApplyCalibration("W1/D" + NPL::itoa(m_EventData->GetBackTDetectorNbr(i)) + "_Back" + NPL::itoa(m_EventData->GetBackTStripNbr(i)) + "_T",  m_EventData->GetBackTTime(i));
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TW1Physics::Construct(){
  return (NPL::VDetector*) new TW1Physics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_w1{
  public:
    proxy_w1(){
      NPL::DetectorFactory::getInstance()->AddToken("W1", "W1");
      NPL::DetectorFactory::getInstance()->AddDetector("W1", TW1Physics::Construct);
    }
};

proxy_w1 p;
}

