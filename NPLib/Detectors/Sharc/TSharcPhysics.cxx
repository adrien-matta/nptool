/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Sharc treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TSharcPhysics.h"
using namespace Sharc_LOCAL;

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPOptionManager.h"
#include "NPDetectorFactory.h"
//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TSharcPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TSharcPhysics::TSharcPhysics(){
    m_Rand= new TRandom3();
    EventMultiplicity   = 0 ;
    m_EventData         = new TSharcData ;
    m_PreTreatedData    = new TSharcData ;
    
    m_EventPhysics      = this ;
    m_Spectra           = NULL;
    m_NumberOfDetector = 0 ;
    m_MaximumStripMultiplicityAllowed = 10;
    m_StripEnergyMatchingSigma = 0.060    ;
    m_StripEnergyMatchingNumberOfSigma = 5;

    // Threshold
    m_StripFront_E_RAW_Threshold = 0 ;
    m_StripFront_E_Threshold = 0 ;

    m_StripBack_E_RAW_Threshold = 0 ;
    m_StripBack_E_Threshold = 0 ;

    m_PAD_E_RAW_Threshold=0;
    m_PAD_E_Threshold=0;
    
    m_Take_E_Front=true;
    m_Take_T_Back=true;
  }

///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::BuildPhysicalEvent(){
  PreTreat();
  bool check_PAD = false ;

  vector< TVector2 > couple = Match_Front_Back() ;
  EventMultiplicity = couple.size();

  unsigned int size = couple.size();
  for(unsigned int i = 0 ; i < size ; ++i){
    
    int N = m_PreTreatedData->GetFront_DetectorNbr(couple[i].X()) ;

    int Front = m_PreTreatedData->GetFront_StripNbr(couple[i].X()) ;
    int Back  = m_PreTreatedData->GetBack_StripNbr(couple[i].Y()) ;

    double Front_E = m_PreTreatedData->GetFront_Energy( couple[i].X() ) ;
    double Back_E  = m_PreTreatedData->GetBack_Energy( couple[i].Y() ) ;

    double Front_T = m_PreTreatedData->GetFront_TimeCFD( couple[i].X() ) ;
    double Back_T  = m_PreTreatedData->GetBack_TimeCFD ( couple[i].Y() ) ;

    DetectorNumber.push_back(N);
    StripFront_E.push_back(Front_E);
    StripFront_T.push_back(Front_T) ;
    StripBack_E.push_back(Back_E) ;
    StripBack_T.push_back(Back_T) ;

    // Try to obtain Pixel Calibration
    static CalibrationManager* Cal = CalibrationManager::getInstance();
    static string name;
    name = "SHARC/D"+ NPL::itoa(N)+"_STRIP_FRONT"+ NPL::itoa(Front)+"_BACK"+ NPL::itoa(Back)+"_E";
    double Pixel_E = Cal->ApplyCalibration(name,StripFront_OriginalE[couple[i].X()] );
    Strip_Front_RawE.push_back(StripFront_OriginalE[couple[i].X()]);
    if(Pixel_E != StripFront_OriginalE[couple[i].X()]){
      Strip_E.push_back(Pixel_E);
      name = "SHARC/D"+ NPL::itoa(N)+"_STRIP_FRONT"+ NPL::itoa(Front)+"_BACK"+ NPL::itoa(Back)+"_DEADLAYER";
      DeadLayer.push_back(Cal->GetValue(name,0));
    }

    // Fall Back option, take the Strip Calibration
    else if(m_Take_E_Front){
      Strip_E.push_back(Front_E) ;
      name = "SHARC/D"+ NPL::itoa(N)+"_STRIP_FRONT"+ NPL::itoa(Front)+"_DEADLAYER";
      DeadLayer.push_back(Cal->GetValue(name,0));
    }

    
    else{
      Strip_E.push_back(Back_E) ;
      name = "SHARC/D"+ NPL::itoa(N)+"_BACK"+ NPL::itoa(Back)+"_DEADLAYER";
      DeadLayer.push_back(Cal->GetValue(name,0));
    }

    if(m_Take_T_Back)
      Strip_T.push_back(Back_T) ;
    else
      Strip_T.push_back(Front_T) ;

    Strip_Front.push_back(Front) ;
    Strip_Back.push_back(Back) ;

    // Search for associate PAD
    check_PAD = false ;
    unsigned int sizePAD = m_PreTreatedData-> GetMultiplicityPAD();
    for(unsigned int j = 0 ; j < sizePAD ; ++j){
      if(m_PreTreatedData->GetPAD_DetectorNbr(j)==N){
        PAD_E.push_back( m_PreTreatedData-> GetPAD_Energy(j)) ;
        PAD_T.push_back( m_PreTreatedData-> GetPAD_TimeCFD(j)  ) ;
        check_PAD = true ;
      }

    }

    if(!check_PAD){
      PAD_E.push_back(-1000)   ;
      PAD_T.push_back(-1000)   ;
    }
  }

  if(DetectorNumber.size()==1)
    return;
}

///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::PreTreat(){
  ClearPreTreatedData();
  //   Front
  unsigned int sizeFront = m_EventData->GetMultiplicityFront();
  for(unsigned int i = 0 ; i < sizeFront ; i++){
    if( m_EventData->GetFront_Energy(i)>m_StripFront_E_RAW_Threshold && IsValidChannel("Front", m_EventData->GetFront_DetectorNbr(i), m_EventData->GetFront_StripNbr(i)) ){
      double Front_E = fStrip_Front_E(m_EventData , i);
      if( Front_E > m_StripFront_E_Threshold ){
        m_PreTreatedData->SetFront( m_EventData->GetFront_DetectorNbr(i),
            m_EventData->GetFront_StripNbr(i),
            Front_E,
            m_EventData->GetFront_TimeCFD(i),
            m_EventData->GetFront_TimeLED(i));

        StripFront_OriginalE.push_back( m_EventData->GetFront_Energy(i));
      }
    }
  }

  //  Back
  unsigned int sizeBack = m_EventData->GetMultiplicityBack() ;
  for(unsigned int i = 0 ; i < sizeBack ; i++){
    if( m_EventData->GetBack_Energy(i)>m_StripBack_E_RAW_Threshold && IsValidChannel("Back", m_EventData->GetBack_DetectorNbr(i), m_EventData->GetBack_StripNbr(i)) ){
      double Back_E = fStrip_Back_E(m_EventData , i);
      if( Back_E > m_StripBack_E_Threshold ){
        m_PreTreatedData->SetBack( m_EventData->GetBack_DetectorNbr(i),
            m_EventData->GetBack_StripNbr(i),
            Back_E,
            m_EventData->GetBack_TimeCFD(i),
            m_EventData->GetBack_TimeLED(i) );
      }
    }
  }

  //  PAD
  unsigned int sizePAD = m_EventData->GetMultiplicityPAD();
  for(unsigned int i = 0 ; i < sizePAD ; i++){
    if( m_EventData->GetPAD_Energy(i)>m_PAD_E_RAW_Threshold && IsValidChannel("PAD", m_EventData->GetPAD_DetectorNbr(i),1) ){
      double PAD_E = fPAD_E(m_EventData , i);
      if( PAD_E > m_PAD_E_Threshold ){
        m_PreTreatedData->SetPAD( m_EventData->GetPAD_DetectorNbr(i),
            PAD_E,
            m_EventData->GetPAD_TimeCFD(i),
            m_EventData->GetPAD_TimeLED(i));
      }
    }
  }

  return;
}


///////////////////////////////////////////////////////////////////////////
int TSharcPhysics :: CheckEvent(){
  return 1 ; // Regular Event
}

///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TSharcPhysics :: Match_Front_Back(){
  vector < TVector2 > ArrayOfGoodCouple ;
  // Prevent code from treating very high multiplicity Event
  // Those event are not physical anyway and that improve speed.
  if( m_PreTreatedData->GetMultiplicityFront() > m_MaximumStripMultiplicityAllowed || m_PreTreatedData->GetMultiplicityBack() > m_MaximumStripMultiplicityAllowed )
    return ArrayOfGoodCouple;

  unsigned int mysizeF =  m_PreTreatedData->GetMultiplicityFront();
  unsigned int mysizeB =  m_PreTreatedData->GetMultiplicityBack();

  for(unsigned int i = 0 ; i < mysizeF ; i++) {
    for(unsigned int j = 0 ; j < mysizeB ; j++){
      //   if same detector check energy
      if ( m_PreTreatedData->GetFront_DetectorNbr(i) == m_PreTreatedData->GetBack_DetectorNbr(j) ){
        //   Look if energy match
        if( abs( (m_PreTreatedData->GetFront_Energy(i)-m_PreTreatedData->GetBack_Energy(j))/2. ) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma )
          ArrayOfGoodCouple.push_back ( TVector2(i,j) ) ;
      }
    }
  }

  //  Prevent to treat event with ambiguous matching beetween Front and Back
  if( ArrayOfGoodCouple.size() > m_PreTreatedData->GetMultiplicityFront() ) 
    ArrayOfGoodCouple.clear() ;

  return ArrayOfGoodCouple;
}


////////////////////////////////////////////////////////////////////////////
bool TSharcPhysics :: IsValidChannel(const string& DetectorType, const int& telescope , const int& channel){

  if(DetectorType == "Front")
    return *(m_FrontChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "Back")
    return *(m_BackChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "PAD")
    return *(m_PADChannelStatus[telescope-1].begin()+channel-1);

  else return false;
}

///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::ReadAnalysisConfig(){
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigSharc.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigSharc.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigSharc.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigSharc.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 11, "ConfigSharc") == 0) ReadingStatus = true;

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
        int Detector = atoi(DataBuffer.substr(2,2).c_str());
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
        int Detector = atoi(DataBuffer.substr(2,2).c_str());
        int channel = -1;
        if (DataBuffer.find("STRF") != string::npos) {
          channel = atoi(DataBuffer.substr(8).c_str());
          *(m_FrontChannelStatus[Detector-1].begin()+channel-1) = false;
          cout << "DISABLE DETECTOR " << Detector << " STRIP FRONT " << channel << endl;
        }

        else if (DataBuffer.find("STRB")!=string::npos) {
          channel = atoi(DataBuffer.substr(8).c_str());
          *(m_BackChannelStatus[Detector-1].begin()+channel-1) = false;
          cout << "DISABLE DETECTOR " << Detector << " STRIP BACK " << channel << endl;

        }

        else if (DataBuffer.find("PAD") != string::npos) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_PADChannelStatus[Detector-1].begin()+channel-1) = false;
          cout << "DISABLE PAD ON DETECTOR " << Detector << endl;

        }

        else cout << "Warning: detector type for Sharc unknown!" << endl;

      }

      else if (whatToDo=="TAKE_E_FRONT") {
        m_Take_E_Front = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_E_BACK") {
        m_Take_E_Front = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_T_FRONT") {
        m_Take_T_Back = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_T_BACK") {
        m_Take_T_Back = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="STRIP_FRONT_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripFront_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripFront_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="STRIP_BACK_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripBack_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripBack_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="STRIP_FRONT_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripFront_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripFront_E_Threshold << endl;
      }

      else if (whatToDo=="STRIP_BACK_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripBack_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripBack_E_Threshold << endl;
      }

      else if (whatToDo=="PAD_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_PAD_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_PAD_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="PAD_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_PAD_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_PAD_E_Threshold << endl;
      }

      else {
        ReadingStatus = false;
      }

    }
  }

}


///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::Clear(){
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
  StripFront_OriginalE.clear();
  DeadLayer.clear();
  Strip_Front_RawE.clear(); 
  // PAD
  PAD_E.clear() ;
  PAD_T.clear() ;
}
///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Sharc");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

 vector<string> tokenQQQ = {"Z","R","Phi","ThicknessDetector"};
 vector<string> tokenBOX = {"Z","ThicknessDetector1","ThicknessDetector2","ThicknessDetector3","ThicknessDetector4","ThicknessPAD1","ThicknessPAD2","ThicknessPAD3","ThicknessPAD4"};
 
  for(unsigned int i = 0 ; i < blocks.size() ; i++){

    if(blocks[i]->GetMainValue()=="QQQ" && blocks[i]->HasTokenList(tokenQQQ)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Sharc QQQ " << i+1 <<  endl;
        double Z = blocks[i]->GetDouble("Z","mm");
        double R = blocks[i]->GetDouble("R","mm");
        double Phi = blocks[i]->GetDouble("Phi","deg");
        double Thickness= blocks[i]->GetDouble("ThicknessDetector","micrometer");
        AddQQQDetector(R,Phi,Z);
    }
    else if(blocks[i]->GetMainValue()=="BOX" && blocks[i]->HasTokenList(tokenBOX)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Sharc Box " << i+1 <<  endl;
        double Z = blocks[i]->GetDouble("Z","mm");
        double Thickness1= blocks[i]->GetDouble("ThicknessDetector1","micrometer");
        double Thickness2= blocks[i]->GetDouble("ThicknessDetector2","micrometer");
        double Thickness3= blocks[i]->GetDouble("ThicknessDetector3","micrometer");
        double Thickness4= blocks[i]->GetDouble("ThicknessDetector4","micrometer");
        double ThicknessPAD1 = blocks[i]->GetDouble("ThicknessPAD1","micrometer");
        double ThicknessPAD2 = blocks[i]->GetDouble("ThicknessPAD2","micrometer");
        double ThicknessPAD3 = blocks[i]->GetDouble("ThicknessPAD3","micrometer");
        double ThicknessPAD4 = blocks[i]->GetDouble("ThicknessPAD4","micrometer");
        AddBoxDetector(Z);
    }

    else{
      cout << "Warning: check your input file formatting " << endl;
    }
  }
  InitializeStandardParameter();
  ReadAnalysisConfig();
}
///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::InitSpectra(){  
  m_Spectra = new TSharcSpectra(m_NumberOfDetector);
}

///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::FillSpectra(){  
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::CheckSpectra(){  
  m_Spectra->CheckSpectra();  
}
///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TSharcPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
} 
////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TSharcPhysics::GetCanvas() {
  if(m_Spectra)
    return m_Spectra->GetCanvas();
  else{
    vector<TCanvas*> empty;
    return empty;
  }
} 

///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::WriteSpectra(){
  m_Spectra->WriteSpectra();
}
///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();

  for(int i = 0 ; i < m_NumberOfDetector ; ++i){

    for( int j = 0 ; j < 24 ; ++j){
      // Front Strip Calibration
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_E","SHARC_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_DEADLAYER",
                        "SHARC_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_DEADLAYER")   ;
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_T","SHARC_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_T")   ;

      // Pixel Calibration
      for( int k = 0 ; k < 48 ; ++k){
        Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_BACK"+ NPL::itoa(k+1)+"_E","SHARC_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_BACK"+ NPL::itoa(k+1)+"_E")   ;
        Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_BACK"+ NPL::itoa(k+1)+"_DEADLAYER",
                          "SHARC_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_BACK"+ NPL::itoa(k+1)+"_DEADLAYER")   ;

      }
    }

    for( int j = 0 ; j < 48 ; ++j){
      // Back strip Calibration
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_E","SHARC_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_DEADLAYER",
                        "SHARC_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_DEADLAYER")   ;

      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_T","SHARC_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_T")   ;
    }

    for( int j = 0 ; j < 1 ; ++j){
      // Pad Calibration
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_PAD_E","SHARC_D"+ NPL::itoa(i+1)+"_PAD_E")   ;
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_PAD_T","SHARC_D"+ NPL::itoa(i+1)+"_PAD_T")   ;
    }
  }

  return;

}

///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain()   ;
  inputChain->SetBranchStatus( "Sharc" , true );
  // The following line is necessary only for system were the tree is splitted
  // (older root version). The found argument silenced the Branches not found
  // warning for non splitted tree.
  if(inputChain->FindBranch("fSharc_*"))
    inputChain->SetBranchStatus( "fSharc_*",true);
  inputChain->SetBranchAddress( "Sharc" , &m_EventData );

}

///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "EventMultiplicity" , true );
  inputChain->SetBranchStatus( "EventType" , true );
  inputChain->SetBranchStatus( "DetectorNumber" , true );
  inputChain->SetBranchStatus( "Strip_E" , true );
  inputChain->SetBranchStatus( "Strip_T" , true );
  inputChain->SetBranchStatus( "StripFront_E" , true );
  inputChain->SetBranchStatus( "StripBack_T" , true );
  inputChain->SetBranchStatus( "StripFront_E" , true );
  inputChain->SetBranchStatus( "StripBack_T" , true );
  inputChain->SetBranchStatus( "Strip_Front" , true );
  inputChain->SetBranchStatus( "Strip_Back" , true );
  inputChain->SetBranchStatus( "PAD_E" , true );
  inputChain->SetBranchStatus( "PAD_T" , true );
  inputChain->SetBranchAddress( "Sharc" , &m_EventPhysics )      ;
}

///////////////////////////////////////////////////////////////////////////
void TSharcPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "Sharc" , "TSharcPhysics" , &m_EventPhysics );
}

////////////////////////////////////////////////////////////////////////////////
/////   Specific to SharcArray   ////
void TSharcPhysics::AddBoxDetector(double Z){
  // BOX //
  double BOX_PCB_Width  = 61.10;
  double BOX_PCB_Length = 104.00;
  double BOX_PCB_Thickness = 3.4;
  double BOX_PCB_Border_LongSide = 1;
  double BOX_PCB_Border_ShortSide = 2;

  // Single stage box case (DSSD only)
  double BOX_PCB_Slot_Width1 = BOX_PCB_Thickness;
  double BOX_PCB_Slot_Border1 = 4;
  double BOX_PCB_Slot_Deepness1 = BOX_PCB_Border_ShortSide;

  // BOX Wafer
  double BOX_ActiveWafer_Width  = 48;
  double BOX_ActiveWafer_Length = 72;
  double BOX_Wafer_Width  = 52.20;
  double BOX_Wafer_Length = 76.20;  
 
  int    BOX_Wafer_Front_NumberOfStrip = 24 ;
  int    BOX_Wafer_Back_NumberOfStrip = 48 ;

  // Compute
  double BOX_LeftOver1 =  BOX_PCB_Length - BOX_PCB_Border_ShortSide - BOX_Wafer_Length - BOX_PCB_Slot_Border1 - BOX_PCB_Slot_Width1 ;
  double BOX_Exposed_Length1 = BOX_Wafer_Length + BOX_PCB_Slot_Border1 ;

  double BOX_CenterOffset1 = - 0.5 * BOX_PCB_Length+BOX_PCB_Border_ShortSide+0.5*BOX_Exposed_Length1;
  double BOX_DetectorSpacing1 = 0.5*BOX_Exposed_Length1+0.5*BOX_PCB_Slot_Width1;

  double BOX_Wafer_Width_Offset1 = -0.5*BOX_PCB_Width + BOX_PCB_Border_LongSide + 0.5*BOX_Wafer_Width;
  double BOX_Wafer_Length_Offset1 = -0.5*BOX_PCB_Length + BOX_PCB_Border_ShortSide + 0.5*BOX_Wafer_Length;

  double BOX_PCB_Slot_Position1 = 0.5*BOX_PCB_Length-BOX_LeftOver1 - 0.5*BOX_PCB_Slot_Width1;

  double StripPitchFront = BOX_ActiveWafer_Length/BOX_Wafer_Front_NumberOfStrip ; //mm
  double StripPitchBack  = BOX_ActiveWafer_Width/BOX_Wafer_Back_NumberOfStrip ; //mm
  m_BoxPitchBack = StripPitchBack;
  m_BoxPitchFront = StripPitchFront;
  
  // Double stage box case (DSSD+PAD) (the wafer is the same but the slot is different to accomodate the additional PAD)
  double PAD_PCB_Thickness = 3.4;

  double BOX_PCB_Slot_Width2 = BOX_PCB_Thickness + PAD_PCB_Thickness ;
  double BOX_PCB_Slot_Border2 = 2.7;
  double BOX_PCB_Slot_Deepness2 = BOX_PCB_Border_ShortSide;

  double BOX_LeftOver2 =  BOX_PCB_Length - BOX_PCB_Border_ShortSide - BOX_Wafer_Length - BOX_PCB_Slot_Border2 - BOX_PCB_Slot_Width2;
  double BOX_Exposed_Length2 = BOX_Wafer_Length + BOX_PCB_Slot_Border2 ;

  double BOX_CenterOffset2 = - 0.5*BOX_PCB_Length+BOX_PCB_Border_ShortSide + 0.5*BOX_Exposed_Length2;
  double BOX_DetectorSpacing2 = 0.5*BOX_Exposed_Length2 + 0.5*BOX_PCB_Thickness;

  double BOX_Wafer_Width_Offset2 = - 0.5*BOX_PCB_Width + BOX_PCB_Border_LongSide + 0.5*BOX_Wafer_Width;
  double BOX_Wafer_Length_Offset2 = - 0.5*BOX_PCB_Length + BOX_PCB_Border_ShortSide + 0.5*BOX_Wafer_Length;

  double BOX_PCB_Slot_Position2 = 0.5*BOX_PCB_Length-BOX_LeftOver2 - 0.5*BOX_PCB_Slot_Width2;

  
  double A1 = BOX_Exposed_Length1*0.5 -BOX_PCB_Slot_Border1- 0.5*StripPitchFront ; 
  double B1 = BOX_DetectorSpacing1 - 0.5*BOX_PCB_Thickness;
  double Z1 = Z - BOX_Wafer_Width*0.5 + StripPitchBack*0.5 ;

  double A2 = BOX_Exposed_Length2*0.5 -BOX_PCB_Slot_Border2- 0.5*StripPitchFront ; 
  double B2 = BOX_DetectorSpacing2 - 0.5*BOX_PCB_Thickness;
  double Z2 = Z + BOX_Wafer_Width*0.5 - StripPitchBack*0.5 ;

  TVector3 U; TVector3 V;TVector3 Strip_1_1;
  
  for(int i = 0 ; i < 4 ; i++){
    m_NumberOfDetector++;
    if(Z<0){// Up Stream
      if(i==0)      {U=TVector3(1,0,0);V=TVector3(0,0,1);  Strip_1_1=TVector3( -A1 , B1  ,Z1); m_DetectorNormal.push_back(TVector3(0,-1,0));}
      else if(i==1) {U=TVector3(0,1,0);V=TVector3(0,0,1);  Strip_1_1=TVector3( -B1 , -A1 ,Z1); m_DetectorNormal.push_back(TVector3(1,0,0)) ;}
      else if(i==2) {U=TVector3(-1,0,0);V=TVector3(0,0,1); Strip_1_1=TVector3( A1  , -B1 ,Z1); m_DetectorNormal.push_back(TVector3(0,1,0)) ;}
      else if(i==3) {U=TVector3(0,-1,0);V=TVector3(0,0,1); Strip_1_1=TVector3( B1  , A1  ,Z1); m_DetectorNormal.push_back(TVector3(-1,0,0));}
    }

    else if(Z>0){//Down Stream
      if(i==0)      {U=TVector3(-1,0,0);V=TVector3(0,0,-1); Strip_1_1=TVector3( A2  ,B2  ,Z2); m_DetectorNormal.push_back(TVector3(0,-1,0));}
      else if(i==1) {U=TVector3(0,-1,0);V=TVector3(0,0,-1); Strip_1_1=TVector3( -B2 ,A2  ,Z2); m_DetectorNormal.push_back(TVector3(1,0,0)) ;}
      else if(i==2) {U=TVector3(1,0,0);V=TVector3(0,0,-1);  Strip_1_1=TVector3( -A2 ,-B2 ,Z2); m_DetectorNormal.push_back(TVector3(0,1,0)) ;}
      else if(i==3) {U=TVector3(0,1,0);V=TVector3(0,0,-1);  Strip_1_1=TVector3( B2  ,-A2 ,Z2); m_DetectorNormal.push_back(TVector3(-1,0,0));}
    }

    m_U.push_back(U);
    m_V.push_back(V);

    //   Buffer object to fill Position Array
    vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

    vector< vector< double > >   OneBoxStripPositionX   ;
    vector< vector< double > >   OneBoxStripPositionY   ;
    vector< vector< double > >   OneBoxStripPositionZ   ;

    TVector3 StripCenter = Strip_1_1;
    for(int f = 0 ; f < BOX_Wafer_Front_NumberOfStrip ; f++){
      lineX.clear()   ;
      lineY.clear()   ;
      lineZ.clear()   ;

      for(int b = 0 ; b < BOX_Wafer_Back_NumberOfStrip ; b++){
        StripCenter = Strip_1_1 + ( StripPitchFront*f*U + StripPitchBack*b*V  );

        lineX.push_back( StripCenter.X() );
        lineY.push_back( StripCenter.Y() );
        lineZ.push_back( StripCenter.Z() );
      }

      OneBoxStripPositionX.push_back(lineX);
      OneBoxStripPositionY.push_back(lineY);
      OneBoxStripPositionZ.push_back(lineZ);
    }
    m_StripPositionX.push_back( OneBoxStripPositionX ) ;
    m_StripPositionY.push_back( OneBoxStripPositionY ) ;
    m_StripPositionZ.push_back( OneBoxStripPositionZ ) ;
  }
}
////////////////////////////////////////////////////////////////////////////////
void TSharcPhysics::AddQQQDetector( double R,double Phi,double Z){

  if(Z>0)
    m_DetectorNormal.push_back(TVector3(0,0,-1));
  else
    m_DetectorNormal.push_back(TVector3(0,0,1));

  m_U.push_back(TVector3(0,0,0));
  m_V.push_back(TVector3(0,0,0));

  double QQQ_R_Min = 9.+R;
  double QQQ_R_Max = 41.0+R;

  double QQQ_Phi_Min = 2.0*M_PI/180.  ;
  double QQQ_Phi_Max = 83.6*M_PI/180. ;
  Phi= Phi*M_PI/180.;

  int    QQQ_Radial_NumberOfStrip = 16 ;
  int    QQQ_Sector_NumberOfStrip = 24 ;

  double StripPitchSector = (QQQ_Phi_Max-QQQ_Phi_Min)/QQQ_Sector_NumberOfStrip ; //radial strip spacing in rad
  double StripPitchRadial = (QQQ_R_Max-QQQ_R_Min)/QQQ_Radial_NumberOfStrip  ; // ring strip spacing in mm
  m_QQQPitchBack = StripPitchSector;
  m_QQQPitchFront = StripPitchRadial;
  TVector3 Strip_1_1;

  m_NumberOfDetector++;
  Strip_1_1=TVector3(0,0,Z);

  //   Buffer object to fill Position Array
  vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

  vector< vector< double > >   OneQQQStripPositionX   ;
  vector< vector< double > >   OneQQQStripPositionY   ;
  vector< vector< double > >   OneQQQStripPositionZ   ;

  TVector3 StripCenter = Strip_1_1;
  for(int f = 0 ; f < QQQ_Radial_NumberOfStrip ; f++){
    lineX.clear()   ;
    lineY.clear()   ;
    lineZ.clear()   ;

    for(int b = 0 ; b < QQQ_Sector_NumberOfStrip ; b++){
      StripCenter = Strip_1_1;
      StripCenter.SetY(QQQ_R_Max-f*StripPitchRadial);
      StripCenter.SetZ(Z);
      StripCenter.RotateZ(Phi+QQQ_Phi_Min+b*StripPitchSector);
      lineX.push_back( StripCenter.X() );
      lineY.push_back( StripCenter.Y() );
      lineZ.push_back( StripCenter.Z() );
    }

    OneQQQStripPositionX.push_back(lineX);
    OneQQQStripPositionY.push_back(lineY);
    OneQQQStripPositionZ.push_back(lineZ);
  }
  m_StripPositionX.push_back( OneQQQStripPositionX ) ;
  m_StripPositionY.push_back( OneQQQStripPositionY ) ;
  m_StripPositionZ.push_back( OneQQQStripPositionZ ) ;

  return;
}
////////////////////////////////////////////////////////////////////////////////
TVector3 TSharcPhysics::GetDetectorNormal( const int& i) const{
  return (m_DetectorNormal[DetectorNumber[i]-1]);

}
////////////////////////////////////////////////////////////////////////////////
TVector3 TSharcPhysics::GetPositionOfInteraction(const int& i,bool random) const{
  static TVector3 Position ;

  Position = TVector3 (  GetStripPositionX( DetectorNumber[i], Strip_Front[i], Strip_Back[i] ),
      GetStripPositionY( DetectorNumber[i] , Strip_Front[i], Strip_Back[i] ),
      GetStripPositionZ( DetectorNumber[i] , Strip_Front[i], Strip_Back[i] )) ;
  
  if(random){
    // Box Detector
    if(m_U[ DetectorNumber[i]-1].Mag()!=0){
      Position += m_V[ DetectorNumber[i]-1]*m_Rand->Uniform(-1,1)*m_BoxPitchBack*0.5;
      Position += m_U[ DetectorNumber[i]-1]*m_Rand->Uniform(-1,1)*m_BoxPitchFront*0.5;
    }

    // QQQ Detector
    else{
      Position.SetPerp( Position.Perp() + m_Rand->Uniform(-1,1)*m_QQQPitchFront*0.5);
      Position.RotateZ(m_Rand->Uniform(-1,1)*m_QQQPitchBack*0.5);
    }
  }
  
  return Position ;

}
////////////////////////////////////////////////////////////////////////////////
double TSharcPhysics::GetDeadLayer(const int& i ) const{
  return DeadLayer[i];
}
////////////////////////////////////////////////////////////////////////////////
void TSharcPhysics::InitializeStandardParameter()
{
  //   Enable all channel
  vector< bool > ChannelStatus;
  m_FrontChannelStatus.clear()    ;
  m_BackChannelStatus.clear()    ;
  m_PADChannelStatus.clear() ;

  ChannelStatus.resize(24,true);
  for(int i = 0 ; i < m_NumberOfDetector ; i++){
    m_FrontChannelStatus[i] = ChannelStatus;
  }

  ChannelStatus.resize(48,true);
  for(int i = 0 ; i < m_NumberOfDetector ; i++){
    m_BackChannelStatus[i] = ChannelStatus;
  }

  ChannelStatus.resize(1,true);
  for(int i = 0 ; i < m_NumberOfDetector ; i++){
    m_PADChannelStatus[i] = ChannelStatus;
  }

  m_MaximumStripMultiplicityAllowed = m_NumberOfDetector   ;

  return;
}


///////////////////////////////////////////////////////////////////////////
namespace Sharc_LOCAL{
  //   DSSD
  //   Front
  double fStrip_Front_E(const TSharcData* m_EventData , const int& i){
    static CalibrationManager* Cal = CalibrationManager::getInstance();
    static string name ;
    name = "SHARC/D" + NPL::itoa( m_EventData->GetFront_DetectorNbr(i) ) + "_STRIP_FRONT" + NPL::itoa( m_EventData->GetFront_StripNbr(i) ) + "_E";
    return Cal->ApplyCalibration(name,m_EventData->GetFront_Energy(i) );
  }

  double fStrip_Front_T(const TSharcData* m_EventData , const int& i){
    static CalibrationManager* Cal = CalibrationManager::getInstance();
    static string name ;
    name ="SHARC/D" + NPL::itoa( m_EventData->GetFront_DetectorNbr(i) ) + "_STRIP_FRONT" + NPL::itoa( m_EventData->GetFront_StripNbr(i) ) +"_T"; 

    return Cal->ApplyCalibration(name, m_EventData->GetFront_TimeCFD(i) );
  }

  //   Back
  double fStrip_Back_E(const TSharcData* m_EventData , const int& i){
    static CalibrationManager* Cal = CalibrationManager::getInstance();
    static string name ;
    name =  "SHARC/D" + NPL::itoa( m_EventData->GetBack_DetectorNbr(i) ) + "_STRIP_BACK" + NPL::itoa( m_EventData->GetBack_StripNbr(i)) +"_E";

    return Cal->ApplyCalibration(name, m_EventData->GetBack_Energy(i) );
  }

  double fStrip_Back_T(const TSharcData* m_EventData , const int& i){
    static CalibrationManager* Cal = CalibrationManager::getInstance();
    static string name ;
    name = "SHARC/D" + NPL::itoa( m_EventData->GetBack_DetectorNbr(i) ) + "_STRIP_BACK" + NPL::itoa( m_EventData->GetBack_StripNbr(i) ) +"_T"; 

    return Cal->ApplyCalibration(name, m_EventData->GetFront_TimeCFD(i));
  }

  //   PAD
  double fPAD_E(const TSharcData* m_EventData , const int& i){
    static CalibrationManager* Cal = CalibrationManager::getInstance();
    static string name ;
    name = "SHARC/D" + NPL::itoa( m_EventData->GetPAD_DetectorNbr(i) ) + "_PAD_E";

    return Cal->ApplyCalibration(name,m_EventData->GetPAD_Energy(i) );
  }

  double fPAD_T(const TSharcData* m_EventData , const int& i){
    static CalibrationManager* Cal = CalibrationManager::getInstance();
    static string name ;
    name = "SHARC/D" + NPL::itoa( m_EventData->GetPAD_DetectorNbr(i) ) + "_PAD_T";

    return Cal->ApplyCalibration(name,m_EventData->GetPAD_TimeCFD(i) );
  }

}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TSharcPhysics::Construct(){
  return (NPL::VDetector*) new TSharcPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_sharc{
  public:
    proxy_sharc(){
      NPL::DetectorFactory::getInstance()->AddToken("Sharc","Sharc");
      NPL::DetectorFactory::getInstance()->AddDetector("Sharc",TSharcPhysics::Construct);
    }
};

proxy_sharc p_sharc;
}

