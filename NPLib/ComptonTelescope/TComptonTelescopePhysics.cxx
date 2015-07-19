/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold ComptonTelescope treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TComptonTelescopePhysics.h"
using namespace ComptonTelescope_LOCAL;

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPOptionManager.h"
#include "NPDetectorFactory.h"

//   ROOT
#include "TChain.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

///////////////////////////////////////////////////////////////////////////

ClassImp(TComptonTelescopePhysics)
///////////////////////////////////////////////////////////////////////////
TComptonTelescopePhysics::TComptonTelescopePhysics()
{
   EventMultiplicity   = 0;
   m_EventData         = new TComptonTelescopeData();
   m_PreTreatedData    = new TComptonTelescopeData();
   m_EventPhysics      = this;
   m_NumberOfDetector  = 0;
   m_MaximumStripMultiplicityAllowed  = 10;
   m_StripEnergyMatchingSigma         = 0.060;
   //m_StripEnergyMatchingSigma = 10    ;
   m_StripEnergyMatchingNumberOfSigma = 3;

   // Threshold
   m_StripFront_E_RAW_Threshold  = 0;
   m_StripFront_E_Threshold      = 0;
   m_StripBack_E_RAW_Threshold   = 0;
   m_StripBack_E_Threshold       = 0;
   m_Take_E_Front                = true;     // p-side
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::BuildSimplePhysicalEvent()
{
   BuildPhysicalEvent();
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::BuildPhysicalEvent()
{
   PreTreat();

   if (CheckEvent() == 1) {
      vector< TVector2 > couple = Match_Front_Back() ;
      EventMultiplicity = couple.size();

      unsigned int size = couple.size();
      for (unsigned int i = 0; i < size; ++i) {

         int N = m_PreTreatedData->GetCTTrackerFrontEDetectorNbr(couple[i].X());

         int Front = m_PreTreatedData->GetCTTrackerFrontEStripNbr(couple[i].X());
         int Back  = m_PreTreatedData->GetCTTrackerBackEStripNbr(couple[i].Y());

         double Front_E = m_PreTreatedData->GetCTTrackerFrontEEnergy(couple[i].X());
         double Back_E  = m_PreTreatedData->GetCTTrackerBackEEnergy(couple[i].Y());

         DetectorNumber.push_back(N);
         StripFront_E.push_back(Front_E);
         StripBack_E.push_back(Back_E) ;

         if(m_Take_E_Front)
            Strip_E.push_back(Front_E) ;
         else
            Strip_E.push_back(Back_E) ;

         Strip_Front.push_back(Front) ;
         Strip_Back.push_back(Back) ;
      }
   }

   if (DetectorNumber.size() == 1) return;
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::PreTreat()
{
   ClearPreTreatedData();

//   cout << "TComptonTelescopePhysics::PreTreat() Front" << endl;
//   m_EventData->Dump();
   //   Front
   unsigned int sizeFront = m_EventData->GetCTTrackerFrontEMult();
//   cout << "sizeFront = " << sizeFront << endl;
   for (unsigned int i = 0; i < sizeFront; ++i) {
//      cout << "loop on sizeFront " << i << endl;
//      cout << "energy / raw threshold " << m_EventData->GetCTTrackerFrontEEnergy(i) << "\t" << m_StripFront_E_RAW_Threshold << endl;
      if (m_EventData->GetCTTrackerFrontEEnergy(i) > m_StripFront_E_RAW_Threshold && IsValidChannel("Front", m_EventData->GetCTTrackerFrontEDetectorNbr(i), m_EventData->GetCTTrackerFrontEStripNbr(i))) {
//         cout << "valid channel & raw threshold applied" << endl;
         double Front_E = fStrip_Front_E(m_EventData, i);
//         cout << "calib E = " << Front_E << endl;
         if (Front_E > m_StripFront_E_Threshold) {
//            cout << "physical threshold applied" << endl;
            if (m_EventData->GetCTTrackerFrontEStripNbr(i) == 0) cout << m_EventData->GetCTTrackerFrontEStripNbr(i) << endl;
            m_PreTreatedData->SetCTTrackerFrontEDetectorNbr(m_EventData->GetCTTrackerFrontEDetectorNbr(i));
            m_PreTreatedData->SetCTTrackerFrontEStripNbr(m_EventData->GetCTTrackerFrontEStripNbr(i));
            m_PreTreatedData->SetCTTrackerFrontEEnergy(Front_E);

         }
      }
   }

   //  Back
   unsigned int sizeBack = m_EventData->GetCTTrackerBackEMult();
   for (unsigned int i = 0; i < sizeBack; ++i) {
      if (m_EventData->GetCTTrackerBackEEnergy(i) > m_StripBack_E_RAW_Threshold && IsValidChannel("Back", m_EventData->GetCTTrackerBackEDetectorNbr(i), m_EventData->GetCTTrackerBackEStripNbr(i))) {
         double Back_E = fStrip_Back_E(m_EventData, i);
         if (Back_E > m_StripBack_E_Threshold) {
            m_PreTreatedData->SetCTTrackerBackEDetectorNbr( m_EventData->GetCTTrackerBackEDetectorNbr(i));
            m_PreTreatedData->SetCTTrackerBackEStripNbr( m_EventData->GetCTTrackerBackEStripNbr(i));
            m_PreTreatedData->SetCTTrackerBackEEnergy( Back_E);
         }
      }
   }

   return;
}


///////////////////////////////////////////////////////////////////////////
int TComptonTelescopePhysics::CheckEvent()
{
  // Check the size of the different elements
  if (m_PreTreatedData->GetCTTrackerBackEMult() == m_PreTreatedData->GetCTTrackerFrontEMult())
    return 1 ; // Regular Event
  
  else
    return -1 ; // Rejected Event
}


///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TComptonTelescopePhysics::Match_Front_Back()
{
   vector < TVector2 > ArrayOfGoodCouple ;

   // Prevent code from treating very high multiplicity Event
   // Those event are not physical anyway and that improve speed.
   if (m_PreTreatedData->GetCTTrackerFrontEMult() > m_MaximumStripMultiplicityAllowed || m_PreTreatedData->GetCTTrackerBackEMult() > m_MaximumStripMultiplicityAllowed)
      return ArrayOfGoodCouple;

   for (unsigned int i = 0; i < m_PreTreatedData->GetCTTrackerFrontEMult(); i++) {
      for (unsigned int j = 0; j < m_PreTreatedData->GetCTTrackerBackEMult(); j++) {
         //   if same detector check energy
         if (m_PreTreatedData->GetCTTrackerFrontEDetectorNbr(i) == m_PreTreatedData->GetCTTrackerBackEDetectorNbr(j)) {
            //   Look if energy match
            if (abs((m_PreTreatedData->GetCTTrackerFrontEEnergy(i) - m_PreTreatedData->GetCTTrackerBackEEnergy(j))/2.) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma)
               ArrayOfGoodCouple.push_back(TVector2(i,j));
         }
      }
   }

   //   Prevent to treat event with ambiguous matchin beetween X and Y
   if (ArrayOfGoodCouple.size() > m_PreTreatedData->GetCTTrackerFrontEMult()) ArrayOfGoodCouple.clear();
   return ArrayOfGoodCouple;
}


////////////////////////////////////////////////////////////////////////////
bool TComptonTelescopePhysics::IsValidChannel(const string DetectorType, const int telescope, const int channel)
{
   if (DetectorType == "Front")
      return *(m_FrontChannelStatus[telescope-1].begin()+channel-1);

   else if (DetectorType == "Back")
      return *(m_BackChannelStatus[telescope-1].begin()+channel-1);

   else return false;
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::ReadAnalysisConfig()
{
   bool ReadingStatus = false;

   // path to file
   string FileName = "./configs/ConfigComptonTelescope.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << " No ConfigComptonTelescope.dat found: Default parameter loaded for Analayis " << FileName << endl;
      return;
   }
   cout << " Loading user parameter for Analysis from ConfigComptonTelescope.dat " << endl;

   // Save it in a TAsciiFile
   TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
   asciiConfig->AppendLine("%%% ConfigComptonTelescope.dat %%%");
   asciiConfig->Append(FileName.c_str());
   asciiConfig->AppendLine("");
   // read analysis config file
   string LineBuffer,DataBuffer,whatToDo;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 22, "ConfigComptonTelescope") == 0) ReadingStatus = true;

      // loop on tokens and data
      while (ReadingStatus ) {

         whatToDo="";
         AnalysisConfigFile >> whatToDo;

         // Search for comment symbol (%)
         if (whatToDo.compare(0, 1, "%") == 0) {
            AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
         }

         else if (whatToDo=="MAX_STRIP_MULTIPLICITY") {
            AnalysisConfigFile >> DataBuffer;
            m_MaximumStripMultiplicityAllowed = atoi(DataBuffer.c_str() );
            cout << "MAXIMUN STRIP MULTIPLICITY " << m_MaximumStripMultiplicityAllowed << endl;
         }

         else if (whatToDo=="STRIP_ENERGY_MATCHING_SIGMA") {
            AnalysisConfigFile >> DataBuffer;
            m_StripEnergyMatchingSigma = atof(DataBuffer.c_str() );
            cout << "STRIP ENERGY MATCHING SIGMA " << m_StripEnergyMatchingSigma <<endl;
         }

         else if (whatToDo=="STRIP_ENERGY_MATCHING_NUMBER_OF_SIGMA") {
            AnalysisConfigFile >> DataBuffer;
            m_StripEnergyMatchingNumberOfSigma = atoi(DataBuffer.c_str() );
            cout << "STRIP ENERGY MATCHING NUMBER OF SIGMA " << m_StripEnergyMatchingNumberOfSigma << endl;
         }

         else if (whatToDo== "DISABLE_ALL") {
            AnalysisConfigFile >> DataBuffer;
            cout << whatToDo << "  " << DataBuffer << endl;
            int Detector = atoi(DataBuffer.substr(2,1).c_str());
            vector< bool > ChannelStatus;
            ChannelStatus.resize(24,false);
            m_FrontChannelStatus[Detector-1] = ChannelStatus;
            ChannelStatus.resize(48,false);
            m_BackChannelStatus[Detector-1] = ChannelStatus;
            ChannelStatus.resize(1,false);
            m_PADChannelStatus[Detector-1] = ChannelStatus;
         }

         else if (whatToDo == "DISABLE_CHANNEL") {
            AnalysisConfigFile >> DataBuffer;
            cout << whatToDo << "  " << DataBuffer << endl;
            int Detector = atoi(DataBuffer.substr(2,1).c_str());
            int channel = -1;
            if (DataBuffer.compare(3,4,"STRF") == 0) {
               channel = atoi(DataBuffer.substr(7).c_str());
               *(m_FrontChannelStatus[Detector-1].begin()+channel-1) = false;
            }

            else if (DataBuffer.compare(3,4,"STRB") == 0) {
               channel = atoi(DataBuffer.substr(7).c_str());
               *(m_BackChannelStatus[Detector-1].begin()+channel-1) = false;
            }

            else cout << "Warning: detector type for ComptonTelescope unknown!" << endl;

         }

         else if (whatToDo=="TAKE_E_FRONT") {
            m_Take_E_Front = true;
            cout << whatToDo << endl;
         }

         else if (whatToDo=="TAKE_E_BACK") {
            m_Take_E_Front = false;
            cout << whatToDo << endl;
         }

         else if (whatToDo=="STRIP_FRONT_E_RAW_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_StripFront_E_RAW_Threshold = atoi(DataBuffer.c_str());
            cout << whatToDo << " " << m_StripFront_E_RAW_Threshold << endl;
         }

         else if (whatToDo=="STRIP_BACK_E_RAW_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_StripBack_E_RAW_Threshold = atoi(DataBuffer.c_str());
            cout << whatToDo << " " << m_StripBack_E_RAW_Threshold << endl;
         }

         else if (whatToDo=="STRIP_FRONT_E_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_StripFront_E_Threshold = atoi(DataBuffer.c_str());
            cout << whatToDo << " " << m_StripFront_E_Threshold << endl;
         }

         else if (whatToDo=="STRIP_BACK_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_StripBack_E_Threshold = atoi(DataBuffer.c_str());
            cout << whatToDo << " " << m_StripBack_E_Threshold << endl;
         }

         else {
            ReadingStatus = false;
         }
      }
   }
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::Clear()
{
   EventMultiplicity = 0;

   //   Provide a Classification of Event
   EventType.clear() ;

   // Detector
   DetectorNumber.clear() ;

   //   DSSD
   Strip_E.clear() ;
   Strip_T.clear() ;
   StripFront_E.clear() ;
   StripFront_T.clear();
   StripBack_E.clear() ;
   StripBack_T.clear() ;
   Strip_Front.clear() ;
   Strip_Back.clear() ;
}


///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::ReadConfiguration(string Path){
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;
  
  double R,Phi,Z;
  R = 0 ; Phi = 0 ; Z = 0;
  TVector3 Pos;
  bool check_R   = false ;
  bool check_Phi = false ;
  bool check_Z   = false ;
  
  bool ReadingStatusQQQ = false ;
  bool ReadingStatusBOX = false ;
  bool ReadingStatus    = false ;
  
  bool VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel(); ;
  
  while (!ConfigFile.eof()){
    
    getline(ConfigFile, LineBuffer);
    // cout << LineBuffer << endl;
    if (LineBuffer.compare(0, 16, "ComptonTelescope") == 0)
      ReadingStatus = true;
    
    while (ReadingStatus && !ConfigFile.eof()) {
      ConfigFile >> DataBuffer ;
      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
      
      //   CD case
      if (DataBuffer=="ComptonTelescopeQQQ"){
       if(VerboseLevel) cout << "///" << endl           ;
        if(VerboseLevel) cout << "QQQ Quadrant found: " << endl   ;
        ReadingStatusQQQ = true ;
      }
      
      //  Box case
      else if (DataBuffer=="ComptonTelescopeBOX"){
        if(VerboseLevel) cout << "///" << endl           ;
        if(VerboseLevel) cout << "Box Detector found: " << endl   ;
        ReadingStatusBOX = true ;
      }
      
      //   Reading Block
      while(ReadingStatusQQQ){
        // Pickup Next Word
        ConfigFile >> DataBuffer ;
        
        //   Comment Line
        if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
        
        //Position method
        else if (DataBuffer == "Z=") {
          check_Z = true;
          ConfigFile >> DataBuffer ;
          Z= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  Z= " << Z << "mm" << endl;
        }
        
        else if (DataBuffer == "R=") {
          check_R = true;
          ConfigFile >> DataBuffer ;
          R= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  R= " << R << "mm" << endl;
        }
        
        else if (DataBuffer == "Phi=") {
          check_Phi = true;
          ConfigFile >> DataBuffer ;
          Phi= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  Phi= " << Phi << "deg" << endl;
        }
        
        else if (DataBuffer == "ThicknessDector=") {
          /*ignore that*/
        }
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusQQQ = false;
          cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_R && check_Phi && check_Z){
          
          ReadingStatusQQQ = false;
//          AddQQQDetector(R,Phi,Z);
          //   Reinitialisation of Check Boolean
          check_R   = false ;
          check_Phi = false ;
        }
        
      }
      
      while(ReadingStatusBOX){
        // Pickup Next Word
        ConfigFile >> DataBuffer ;
        
        //   Comment Line
        if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
        
        //Position method
        else if (DataBuffer == "Z=") {
          check_Z = true;
          ConfigFile >> DataBuffer ;
          Z= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  Z= " << Z << "mm" << endl;
        }
        
        else if (DataBuffer == "ThicknessDector1=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessDector2=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessDector3=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessDector4=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessPAD1=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessPAD2=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessPAD3=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessPAD4=") {
          /*ignore this */
        }
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusBOX = false;
          cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_Z){
          ReadingStatusBOX = false;
//          AddBoxDetector(Z);
          //   Reinitialisation of Check Boolean
          check_R = false ;
          check_Phi = false ;
          check_Z = false ;
          
        }
      }
    }
  }

  AddComptonTelescope(15);
  
  InitializeStandardParameter();
  ReadAnalysisConfig();
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::AddParameterToCalibrationManager()
{
   CalibrationManager* Cal = CalibrationManager::getInstance();

   for (int i = 0; i < m_NumberOfDetector; ++i) {
      for (int j = 0; j < 32; ++j) {
         Cal->AddParameter("COMPTONTELESCOPE", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_E", "COMPTONTELESCOPE_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_E");
         Cal->AddParameter("COMPTONTELESCOPE", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_T", "COMPTONTELESCOPE_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_T");
      }
      for (int j = 0; j < 32; ++j) {
         Cal->AddParameter("COMPTONTELESCOPE", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_E",  "COMPTONTELESCOPE_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_E");
         Cal->AddParameter("COMPTONTELESCOPE", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_T",  "COMPTONTELESCOPE_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_T");
      }
   }

   return;  
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::InitializeRootInputRaw()
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus("ComptonTelescope",    true);
   inputChain->SetBranchStatus("fComptonTelescope_*", true);
   inputChain->SetBranchAddress("ComptonTelescope",   &m_EventData);
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::InitializeRootInputPhysics()
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus("EventMultiplicity", true);
   inputChain->SetBranchStatus("EventType",         true);
   inputChain->SetBranchStatus("DetectorNumber",    true);
   inputChain->SetBranchStatus("Strip_E",           true);
   inputChain->SetBranchStatus("Strip_T",           true);
   inputChain->SetBranchStatus("StripFront_E",      true);
   inputChain->SetBranchStatus("StripBack_T",       true);
   inputChain->SetBranchStatus("StripFront_E",      true);
   inputChain->SetBranchStatus("StripBack_T",       true);
   inputChain->SetBranchStatus("Strip_Front",       true);
   inputChain->SetBranchStatus("Strip_Back",        true);
}


///////////////////////////////////////////////////////////////////////////
void TComptonTelescopePhysics::InitializeRootOutput()
{
   TTree* outputTree = RootOutput::getInstance()->GetTree();
   outputTree->Branch("ComptonTelescope", "TComptonTelescopePhysics", &m_EventPhysics);
}


/////   Specific to ComptonTelescopeArray   ////

void TComptonTelescopePhysics::AddComptonTelescope(double Z)
{
   m_NumberOfDetector++;
   // empty at the moment
   // needed if solid angle analysis are needed
}



TVector3 TComptonTelescopePhysics::GetDetectorNormal( const int i) const{
  /*  TVector3 U =    TVector3 ( GetStripPositionX( DetectorNumber[i] , 24 , 1 ) ,
   GetStripPositionY( DetectorNumber[i] , 24 , 1 ) ,
   GetStripPositionZ( DetectorNumber[i] , 24 , 1 ) )
   
   -TVector3 ( GetStripPositionX( DetectorNumber[i] , 1 , 1 ) ,
   GetStripPositionY( DetectorNumber[i] , 1 , 1 ) ,
   GetStripPositionZ( DetectorNumber[i] , 1 , 1 ) );
   
   TVector3 V =    TVector3 ( GetStripPositionX( DetectorNumber[i] , 24 , 48 ) ,
   GetStripPositionY( DetectorNumber[i] , 24 , 48 ) ,
   GetStripPositionZ( DetectorNumber[i] , 24 , 48 ) )
   
   -TVector3 ( GetStripPositionX( DetectorNumber[i] , 24 , 1 ) ,
   GetStripPositionY( DetectorNumber[i] , 24 , 1 ) ,
   GetStripPositionZ( DetectorNumber[i] , 24 , 1 ) );
   
   TVector3 Normal = U.Cross(V);
   
   return(Normal.Unit()) ;*/
  
  return (TVector3(0,0,i));
  
}

TVector3 TComptonTelescopePhysics::GetPositionOfInteraction(const int i) const{
  TVector3    Position = TVector3 (  GetStripPositionX( DetectorNumber[i] , Strip_Front[i] , Strip_Back[i] )    ,
                                  GetStripPositionY( DetectorNumber[i] , Strip_Front[i] , Strip_Back[i] )    ,
                                  GetStripPositionZ( DetectorNumber[i] , Strip_Front[i] , Strip_Back[i] )    ) ;
  return(Position) ;
  
}

void TComptonTelescopePhysics::InitializeStandardParameter()
{
   //   Enable all channel
   vector< bool > ChannelStatus;
   m_FrontChannelStatus.clear()    ;
   m_BackChannelStatus.clear()    ;
   m_PADChannelStatus.clear() ;

   ChannelStatus.resize(32,true);
   for(int i = 0; i < m_NumberOfDetector; ++i) {
      m_FrontChannelStatus[i] = ChannelStatus;
   }

   ChannelStatus.resize(32,true);
   for(int i = 0; i < m_NumberOfDetector; ++i) {
      m_BackChannelStatus[i] = ChannelStatus;
   }

   m_MaximumStripMultiplicityAllowed = m_NumberOfDetector;
  
  return;
}


void TComptonTelescopePhysics::InitSpectra()
{
   m_Spectra = new TComptonTelescopeSpectra(m_NumberOfDetector);
}


void TComptonTelescopePhysics::FillSpectra()
{
   m_Spectra->FillRawSpectra(m_EventData);
   m_Spectra->FillPreTreatedSpectra(m_PreTreatedData);
   m_Spectra->FillPhysicsSpectra(m_EventPhysics);
}


void TComptonTelescopePhysics::CheckSpectra()
{
   m_Spectra->CheckSpectra();
}


void TComptonTelescopePhysics::ClearSpectra()
{
   // To be done
}


void TComptonTelescopePhysics::WriteSpectra()
{
   m_Spectra->WriteSpectra();
}


map< vector<string> , TH1*> TComptonTelescopePhysics::GetSpectra() 
{
   if(m_Spectra)
      return m_Spectra->GetMapHisto();
   else{
      map< vector<string> , TH1*> empty;
      return empty;
   }
}

///////////////////////////////////////////////////////////////////////////
namespace ComptonTelescope_LOCAL
{
   // tranform an integer to a string
   string itoa(unsigned int value) 
   {
      char buffer [33];
      sprintf(buffer,"%d",value);
      return buffer;
   }

   // DSSD
   // Front
   double fStrip_Front_E(const TComptonTelescopeData* m_EventData , const int i)
   {
      return CalibrationManager::getInstance()->ApplyCalibration("COMPTONTELESCOPE/D" + NPL::itoa(m_EventData->GetCTTrackerFrontEDetectorNbr(i)) + "_STRIP_FRONT" + NPL::itoa(m_EventData->GetCTTrackerFrontEStripNbr(i)) + "_E", m_EventData->GetCTTrackerFrontEEnergy(i));
   }

   // Back
   double fStrip_Back_E(const TComptonTelescopeData* m_EventData , const int i)
   {
      return CalibrationManager::getInstance()->ApplyCalibration("COMPTONTELESCOPE/D" + NPL::itoa(m_EventData->GetCTTrackerBackEDetectorNbr(i)) + "_STRIP_BACK" + NPL::itoa(m_EventData->GetCTTrackerBackEStripNbr(i)) + "_E", m_EventData->GetCTTrackerBackEEnergy(i));
   }
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TComptonTelescopePhysics::Construct(){
  return (NPL::VDetector*) new TComptonTelescopePhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_comptontelescope{
  public:
    proxy_comptontelescope(){
      NPL::DetectorFactory::getInstance()->AddToken("ComptonTelescope","ComptonTelescope");
      NPL::DetectorFactory::getInstance()->AddDetector("ComptonTelescope",TComptonTelescopePhysics::Construct);
    }
};

proxy_comptontelescope p;
}

