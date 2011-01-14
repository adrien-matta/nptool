/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
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
#include "RootOutput.h"
#include "RootInput.h"

//  STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <cmath>
#include <stdlib.h>
using namespace std;
using namespace LOCAL;

//  ROOT
#include "TChain.h"

//  tranform an integer to a string
string itoa(int value)
{
   std::ostringstream o;
      
   if (!(o << value)) return "";

   return o.str();
}



ClassImp(TW1Physics)
///////////////////////////////////////////////////////////////////////////
TW1Physics::TW1Physics()
   : m_EventData(new TW1Data),
     m_PreTreatedData(new TW1Data),
     m_EventPhysics(this),
     m_MaximumStripMultiplicityAllowed(1),   // multiplidity 1
     m_StripEnergyMatchingTolerance(10),     // 10%
     m_FrontE_Raw_Threshold(0),
     m_BackE_Raw_Threshold(0),
     m_FrontE_Calib_Threshold(0),
     m_BackE_Calib_Threshold(0),
     m_NumberOfDetector(0),
     m_SiliconFace(49.6),  // mm
     m_NumberOfStrips(16)
{    
   m_StripPitch = m_SiliconFace / (double)m_NumberOfStrips;
}



///////////////////////////////////////////////////////////////////////////
TW1Physics::~TW1Physics()
{
   delete m_EventData;
   delete m_PreTreatedData;
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::Clear()
{
   fEventType.clear();
   fDetectorNumber.clear();
   fEnergy.clear();
   fTime.clear();
   fFrontStrip.clear();
   fBackStrip.clear();
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::ReadConfiguration(string Path) 
  {
   ifstream ConfigFile           ;
   ConfigFile.open(Path.c_str()) ;
   string LineBuffer          ;
   string DataBuffer          ;

   double TLX , BLX , BRX , TRX , TLY , BLY , BRY , TRY , TLZ , BLZ , BRZ , TRZ   ;
   double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0      ;
   bool check_A = false   ;
   bool check_B = false ;
   bool check_C = false   ;
   bool check_D = false ;

   bool check_Theta = false   ;
   bool check_Phi  = false  ;
   bool check_R     = false   ;
   bool check_beta = false  ;
   bool ReadingStatus = false ;

 while (!ConfigFile.eof()) 
   {
      
    getline(ConfigFile, LineBuffer);

    //  If line is a Start Up ThinSi bloc, Reading toggle to true      
        if (LineBuffer.compare(0, 6, "ThinSi") == 0) 
          {
            cout << "Detector found: " << endl   ;        
            ReadingStatus = true ;
          }

    //  Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    //  Reading Block
    while(ReadingStatus)
      {
          // Pickup Next Word 
        ConfigFile >> DataBuffer ;

        //  Comment Line 
        if (DataBuffer.compare(0, 1, "%") == 0) {  ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

          //  Finding another telescope (safety), toggle out
        else if (DataBuffer.compare(0, 6, "ThinSi") == 0) {
          cout << "WARNING: Another Telescope is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
          ReadingStatus = false ;
        }

           //Position method
             else if (DataBuffer.compare(0, 3, "A=") == 0) {
                check_A = true;
                ConfigFile >> DataBuffer ;
                TLX = atof(DataBuffer.c_str()) ;
                ConfigFile >> DataBuffer ;
                TLY = atof(DataBuffer.c_str()) ;
                ConfigFile >> DataBuffer ;
                TLZ = atof(DataBuffer.c_str()) ;

             }
        
        else if (DataBuffer.compare(0, 3, "B=") == 0) {
          check_B = true;
          ConfigFile >> DataBuffer ;
          BLX = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BLY = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BLZ = atof(DataBuffer.c_str()) ;

        }

        else if (DataBuffer.compare(0, 3, "C=") == 0) {
          check_C = true;
          ConfigFile >> DataBuffer ;
          BRX = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BRY = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BRZ = atof(DataBuffer.c_str()) ;

        }

        else if (DataBuffer.compare(0, 3, "D=") == 0) {
          check_D = true;
          ConfigFile >> DataBuffer ;
          TRX = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          TRY = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          TRZ = atof(DataBuffer.c_str()) ;

        }

                  
        //Angle method
        else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
          check_Theta = true;
          ConfigFile >> DataBuffer ;
          Theta = atof(DataBuffer.c_str()) ;
        }

        else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
          check_Phi = true;
          ConfigFile >> DataBuffer ;
          Phi = atof(DataBuffer.c_str()) ;
        }

        else if (DataBuffer.compare(0, 2, "R=") == 0) {
          check_R = true;
          ConfigFile >> DataBuffer ;
          R = atof(DataBuffer.c_str()) ;
        }


        else if (DataBuffer.compare(0, 5, "BETA=") == 0) {
          check_beta = true;
          ConfigFile >> DataBuffer ;
          beta_u = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          beta_v = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          beta_w = atof(DataBuffer.c_str()) ;
        }
          
               ///////////////////////////////////////////////////
          //  If no Detector Token and no comment, toggle out
             else 
               {ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
             
               /////////////////////////////////////////////////
               //  If All necessary information there, toggle out
             
             if ( (check_A && check_B && check_C && check_D) || (check_Theta && check_Phi && check_R && check_beta) ) 
               { 
                   ReadingStatus = false; 
                   
                   ///Add The previously define telescope
                   //With position method
                    if ((check_A && check_B && check_C && check_D) || !(check_Theta && check_Phi && check_R)) {
                           m_NumberOfDetector++;
                     }

                   //with angle method
                   else if ((check_Theta && check_Phi && check_R) || !(check_A && check_B && check_C && check_D)) {
                           m_NumberOfDetector++;
                     }
                     
                    //  Reinitialisation of Check Boolean 
                    
                check_A = false   ;
                check_B = false ;
                check_C = false   ;
                check_D = false ;

                check_Theta   = false   ;
                check_Phi     = false  ;
                check_R       = false   ;
                check_beta    = false  ;
                ReadingStatus = false ;
                       
               }
               
            }
          }
          
    InitializeStandardParameter() ;
    ReadAnalysisConfig()          ;
}


///////////////////////////////////////////////////////////////////////////
void TW1Physics::AddParameterToCalibrationManager()
{
   CalibrationManager* Cal = CalibrationManager::getInstance();
    
   for (int i = 0; i < m_NumberOfDetector; i++) {
      for (int j = 0; j < m_NumberOfStrips; j++) {
         // Energy
         Cal->AddParameter("W1", "Detector"+itoa(i+1)+"_Front_"+itoa(j+1)+"_E", "W1_DETECTOR"+itoa(i+1)+"_FRONT_"+itoa(j+1)+"_E");
         Cal->AddParameter("W1", "Detector"+itoa(i+1)+"_Back_"+itoa(j+1)+"_E",  "W1_DETECTOR"+itoa(i+1)+"_BACK_"+itoa(j+1)+"_E");  
         // Time
         Cal->AddParameter("W1", "Detector"+itoa(i+1)+"_Front_"+itoa(j+1)+"_T", "W1_DETECTOR"+itoa(i+1)+"_FRONT_"+itoa(j+1)+"_T");
         Cal->AddParameter("W1", "Detector"+itoa(i+1)+"_Back_"+itoa(j+1)+"_T",  "W1_DETECTOR"+itoa(i+1)+"_BACK_"+itoa(j+1)+"_T");  
      }
   }
}


  
///////////////////////////////////////////////////////////////////////////
void TW1Physics::InitializeRootInput()
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus("W1"   , true);
   inputChain->SetBranchStatus("fW1_*", true);
   inputChain->SetBranchAddress("W1"  , &m_EventData);
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
   m_NumberOfDetector++;

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
   m_NumberOfDetector++;

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

   // Begin treatement
   int evtType = EventType();

   if (evtType == 1) {  // case where multiplicity front = multiplicity back
      vector<TVector2> couple = Match_Front_Back();

      for (unsigned int i = 0; i < couple.size(); i++) { // loop on selected events
         int    DetecNbr    = m_PreTreatedData->GetW1FrontEDetectorNbr(couple[i].X());
         int    StripFront  = m_PreTreatedData->GetW1FrontEStripNbr(couple[i].X());
         int    StripBack   = m_PreTreatedData->GetW1BackEStripNbr(couple[i].Y());
         double EnergyFront = m_PreTreatedData->GetW1FrontEEnergy(couple[i].X());
         double EnergyBack  = m_PreTreatedData->GetW1BackEEnergy(couple[i].Y());
         EnergyBack *= 1;

         // Search for associate time
         // Front
         double TimeFront = -1000;
         for (unsigned int t = 0; t < m_PreTreatedData->GetW1FrontTMult(); t++) {
            if (m_PreTreatedData->GetW1FrontTStripNbr(couple[i].X()) == m_PreTreatedData->GetW1FrontTStripNbr(t) ||
                m_PreTreatedData->GetW1FrontTDetectorNbr(couple[i].X()) == m_PreTreatedData->GetW1FrontTDetectorNbr(t))
               TimeFront = m_PreTreatedData->GetW1FrontTTime(t);
         }
         // Back
         double TimeBack = -1000;
         for (unsigned int t = 0; t < m_PreTreatedData->GetW1BackTMult(); t++) {
            if (m_PreTreatedData->GetW1BackTStripNbr(couple[i].X()) == m_PreTreatedData->GetW1BackTStripNbr(t) ||
                m_PreTreatedData->GetW1BackTDetectorNbr(couple[i].X()) == m_PreTreatedData->GetW1BackTDetectorNbr(t))
               TimeBack = m_PreTreatedData->GetW1BackTTime(t);
         }

         // Fill TW1Physics private members
         fEventType.push_back(evtType);
         fDetectorNumber.push_back(DetecNbr);
         fEnergy.push_back(EnergyFront);
         fTime.push_back(TimeBack);
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
   for (int i = 0; i < m_EventData->GetW1FrontEMult(); i++) {
      if (IsValidChannel("Front", m_EventData->GetW1FrontEDetectorNbr(i), m_EventData->GetW1FrontEStripNbr(i)) &&
           m_EventData->GetW1FrontEEnergy(i) > m_FrontE_Raw_Threshold) {
         double E = fW1_Front_E(m_EventData , i);
         if (E > m_FrontE_Calib_Threshold)	{
            m_PreTreatedData->SetW1FrontEDetectorNbr(m_EventData->GetW1FrontEDetectorNbr(i));
            m_PreTreatedData->SetW1FrontEStripNbr(m_EventData->GetW1FrontEStripNbr(i));
            m_PreTreatedData->SetW1FrontEEnergy(E);
         }
      } 
   }
   // (Front, T)
   for (int i = 0; i < m_EventData->GetW1FrontTMult(); i++) {
      if (IsValidChannel("Front", m_EventData->GetW1FrontTDetectorNbr(i), m_EventData->GetW1FrontTStripNbr(i))) {
         double T = fW1_Front_T(m_EventData , i);
         m_PreTreatedData->SetW1FrontTDetectorNbr(m_EventData->GetW1FrontTDetectorNbr(i));
         m_PreTreatedData->SetW1FrontTStripNbr(m_EventData->GetW1FrontTStripNbr(i));
         m_PreTreatedData->SetW1FrontTTime(T);
      } 
   }

   // (Back, E)
   for (int i = 0; i < m_EventData->GetW1BackEMult(); i++) {
      if (IsValidChannel("Back", m_EventData->GetW1FrontEDetectorNbr(i), m_EventData->GetW1FrontEStripNbr(i)) &&
          m_EventData->GetW1BackEEnergy(i) > m_BackE_Raw_Threshold) {
         double E = fW1_Back_E(m_EventData , i);
         if (E > m_BackE_Calib_Threshold) {
            m_PreTreatedData->SetW1BackEDetectorNbr(m_EventData->GetW1BackEDetectorNbr(i));
            m_PreTreatedData->SetW1BackEStripNbr(m_EventData->GetW1BackEStripNbr(i));
            m_PreTreatedData->SetW1BackEEnergy(E);
         }
      } 
   }
   // (Back, T)
   for (int i = 0; i < m_EventData->GetW1BackTMult(); i++) {
      if (IsValidChannel("Back", m_EventData->GetW1FrontTDetectorNbr(i), m_EventData->GetW1FrontTStripNbr(i))) {
         double T = fW1_Back_T(m_EventData , i);
         m_PreTreatedData->SetW1BackTDetectorNbr(m_EventData->GetW1BackTDetectorNbr(i));
         m_PreTreatedData->SetW1BackTStripNbr(m_EventData->GetW1BackTStripNbr(i));
         m_PreTreatedData->SetW1BackTTime(T);
      } 
   }
}



int TW1Physics::EventType()
{
   // Same multiplicity on front and back side
   if (m_PreTreatedData->GetW1FrontEMult() == m_PreTreatedData->GetW1BackEMult()) {
      return 1;
   }
   // Possibly interstrip
   else if (m_PreTreatedData->GetW1FrontEMult() == m_PreTreatedData->GetW1BackEMult()+1 ||
            m_PreTreatedData->GetW1FrontEMult() == m_PreTreatedData->GetW1BackEMult()-1) {
      return 2;
   }
   // Rejected event
   else {
      return -1;
   }
}



vector<TVector2> TW1Physics::Match_Front_Back()
{
   vector<TVector2> ArrayOfGoodCouple;

   // Treat only allowd multiplicity events. If multiplicity is too 
   // high, then return "empty" vector
   if (m_PreTreatedData->GetW1FrontEMult() > m_MaximumStripMultiplicityAllowed || 
       m_PreTreatedData->GetW1BackEMult()  > m_MaximumStripMultiplicityAllowed)
      return ArrayOfGoodCouple;

   // Loop on front multiplicity
   for (int i = 0; i < m_PreTreatedData->GetW1FrontEMult(); i++) {
      // Loop on back multiplicity
      for (int j = 0; j < m_PreTreatedData->GetW1BackEMult(); j++) {
         // if same detector check energy
         if (m_PreTreatedData->GetW1FrontEDetectorNbr(i) == m_PreTreatedData->GetW1BackEDetectorNbr(j)) {
            // Look if energy match (within m_StripEnergyMatchingTolerance %)
            double de = abs(m_PreTreatedData->GetW1FrontEEnergy(i) - m_PreTreatedData->GetW1BackEEnergy(j));
            if (de / m_PreTreatedData->GetW1FrontEEnergy(i) < m_StripEnergyMatchingTolerance/100) {
               ArrayOfGoodCouple.push_back(TVector2(i,j));
            }  // end test energy
         }  // end test same detector
      }  // end loop back multiplicity
   }  // end loop front multiplicity

   // Prevent treating event with ambiguous matchin beetween X and Y
   if (ArrayOfGoodCouple.size() > m_PreTreatedData->GetW1FrontEMult()) ArrayOfGoodCouple.clear();

   return ArrayOfGoodCouple;
}



bool TW1Physics::IsValidChannel(string Type, int detector, int channel)
{
   vector<bool>::iterator it;
   if (Type == "Front")
      return *(m_FrontChannelStatus[detector].begin()+channel);

   else if (Type == "Back")
      return *(m_BackChannelStatus[detector].begin()+channel);

   else 
      return false;
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::InitializeStandardParameter()
{
   // Enable all channels
   vector<bool> ChannelStatus;
   m_FrontChannelStatus.clear();
   m_BackChannelStatus.clear();

   ChannelStatus.resize(m_NumberOfStrips, true);
   for (int i = 0; i < m_NumberOfDetector; i ++) {
      m_FrontChannelStatus[i+1] = ChannelStatus;
      m_BackChannelStatus[i+1]  = ChannelStatus;
   }
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::ReadAnalysisConfig()
{
   bool ReadingStatus = false;
   bool check_mult    = false;
   bool check_match   = false;

   // path to file
   string FileName = "./configs/ConfigW1.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << " No ConfigW1.dat found: Default parameter loaded for Analayis " << FileName << endl;
      return;
   }
   cout << " Loading user parameter for Analysis from ConfigW1.dat " << endl;

   // read analysis config file
   string LineBuffer,DataBuffer;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 8, "ConfigW1") == 0) ReadingStatus = true;

      // loop on tokens and data
      while (ReadingStatus) {
         AnalysisConfigFile >> DataBuffer;

         // Search for comment symbol (%)
         if (DataBuffer.compare(0, 1, "%") == 0) {
            AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
         }
         
         else if (DataBuffer.compare(0, 22, "MAX_STRIP_MULTIPLICITY") == 0) {
            check_mult = true;
            AnalysisConfigFile >> DataBuffer;
            m_MaximumStripMultiplicityAllowed = atoi(DataBuffer.c_str() );
            cout << "Maximun strip multiplicity= " << m_MaximumStripMultiplicityAllowed << endl;
         }

         else if (DataBuffer.compare(0, 31, "STRIP_ENERGY_MATCHING_TOLERANCE") == 0) {
            check_match = true;
            AnalysisConfigFile >> DataBuffer;
            m_StripEnergyMatchingTolerance = atoi(DataBuffer.c_str() );
            cout << "Strip energy matching tolerance= " << m_StripEnergyMatchingTolerance << endl;
         }
         
         else if (DataBuffer.compare(0, 2, "W1") == 0) {
            AnalysisConfigFile >> DataBuffer;
            string whatToDo = DataBuffer;
            if (whatToDo.compare(0, 11, "DISABLE_ALL") == 0) {
               AnalysisConfigFile >> DataBuffer;
               cout << whatToDo << "  " << DataBuffer << endl;
               int Detector = atoi(DataBuffer.substr(3,1).c_str());
               vector< bool > ChannelStatus;
               ChannelStatus.resize(m_NumberOfStrips,false);
               m_FrontChannelStatus[Detector] = ChannelStatus;
               m_BackChannelStatus[Detector]  = ChannelStatus;
            }
            
            else if (whatToDo.compare(0, 15, "DISABLE_CHANNEL") == 0) {
               AnalysisConfigFile >> DataBuffer;
               cout << whatToDo << "  " << DataBuffer << endl;
               int detector = atoi(DataBuffer.substr(3,1).c_str());
               int channel = -1;
               if (DataBuffer.compare(4,5,"FRONT") == 0) {
                  channel = atoi(DataBuffer.substr(11).c_str());
                  *(m_FrontChannelStatus[detector].begin()+channel) = false;
               }
               else if (DataBuffer.compare(4,4,"BACK") == 0) {
                  channel = atoi(DataBuffer.substr(10).c_str());
                  *(m_BackChannelStatus[detector].begin()+channel) = false;
               }
               
               else {
                  cout << "Warning: detector type for W1 unknown!" << endl;
               }
            }

            else if (whatToDo.compare(0, 21, "FRONT_E_RAW_THRESHOLD") == 0) {
               AnalysisConfigFile >> DataBuffer;
               m_FrontE_Raw_Threshold = atoi(DataBuffer.c_str());
               cout << whatToDo << " " << m_FrontE_Raw_Threshold << endl;
            }

            else if (whatToDo.compare(0, 20, "BACK_E_RAW_THRESHOLD") == 0) {
               AnalysisConfigFile >> DataBuffer;
               m_BackE_Raw_Threshold = atoi(DataBuffer.c_str());
               cout << whatToDo << " " << m_BackE_Raw_Threshold << endl;
            }

            else if (whatToDo.compare(0, 21, "FRONT_E_CAL_THRESHOLD") == 0) {
               AnalysisConfigFile >> DataBuffer;
               m_FrontE_Calib_Threshold = atoi(DataBuffer.c_str());
               cout << whatToDo << " " << m_FrontE_Calib_Threshold << endl;
            }

            else if (whatToDo.compare(0, 20, "BACK_E_CAL_THRESHOLD") == 0) {
               AnalysisConfigFile >> DataBuffer;
               m_BackE_Calib_Threshold = atoi(DataBuffer.c_str());
               cout << whatToDo << " " << m_BackE_Calib_Threshold << endl;
            }

            else {
               cout << "Warning: don't know what to do (lost in translation)" << endl;
            }
         }
         else {
            ReadingStatus = false;
         }
      }
   }
}	



///////////////////////////////////////////////////////////////////////////
double LOCAL::fW1_Front_E(TW1Data* m_EventData , int i)
{
   return CalibrationManager::getInstance()->ApplyCalibration("W1/Detector" + itoa(m_EventData->GetW1FrontEDetectorNbr(i)) + "_Front_" + itoa(m_EventData->GetW1FrontEStripNbr(i)) +"_E",  m_EventData->GetW1FrontEEnergy(i));
}
 
 

double LOCAL::fW1_Back_E(TW1Data* m_EventData , int i)
{
   return CalibrationManager::getInstance()->ApplyCalibration("W1/Detector" + itoa(m_EventData->GetW1BackEDetectorNbr(i)) + "_Back_" + itoa(m_EventData->GetW1BackEStripNbr(i)) +"_E",  m_EventData->GetW1BackEEnergy(i));
}
  
 

double LOCAL::fW1_Front_T(TW1Data* m_EventData , int i)
{
   return CalibrationManager::getInstance()->ApplyCalibration("W1/Detector" + itoa(m_EventData->GetW1FrontTDetectorNbr(i)) + "_Front_" + itoa(m_EventData->GetW1FrontTStripNbr(i)) +"_T",  m_EventData->GetW1FrontTTime(i));
}
 
 

double LOCAL::fW1_Back_T(TW1Data* m_EventData , int i)
{
   return CalibrationManager::getInstance()->ApplyCalibration("W1/Detector" + itoa(m_EventData->GetW1BackTDetectorNbr(i)) + "_Back_" + itoa(m_EventData->GetW1BackTStripNbr(i)) +"_T",  m_EventData->GetW1BackTTime(i));
}
