/* Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Sandra Giron  contact address: giron@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2010                                             *
 * Last update    : modification november 2011 by Pierre Morfouace			 *
 * Contact adress : morfouac@ipno.in2p3.fr									 *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold CATS treated data                                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include "TCATSPhysics.h"
using namespace CATS_LOCAL;

//	STL
#include <cmath>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <cstdlib>
using namespace std;
//	NPL
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "RootOutput.h"
#include "NPOptionManager.h"
//	ROOT
#include "TChain.h"
#include "TF1.h"
#include "TGraph.h"
ClassImp(TCATSPhysics)

  ///////////////////////////////////////////////////////////////////////////
  TCATSPhysics::TCATSPhysics(){
    m_EventData 				= new TCATSData	;
    m_PreTreatedData    = new TCATSData ;
    m_EventPhysics 			= this			    ;
    m_NumberOfCATS      = 0             ;
    m_Spectra           = NULL          ;
  }

///////////////////////////////////////////////////////////////////////////
TCATSPhysics::~TCATSPhysics(){
}

///////////////////////////////////////////////////////////////////////////
void TCATSPhysics::PreTreat(){
  ClearPreTreatedData();   
  gRandom->SetSeed(0);
  // X
  unsigned int sizeX = m_EventData->GetCATSMultX();
  for(unsigned int i = 0 ; i < sizeX ; i++){
    // Valid Channel X
    if(IsValidChannel("X", m_EventData->GetCATSDetX(i), m_EventData->GetCATSStripX(i)) ){
      if( fCATS_Threshold_X(m_EventData , i) ){
        double QX = fCATS_X_Q(m_EventData , i);
        m_PreTreatedData->SetCATSChargeX( QX );
        //Inversion X
        if( *(m_CATSXInversion[m_EventData->GetCATSDetX(i)-1].begin() + m_EventData->GetCATSStripX(i)-1) != m_EventData->GetCATSStripX(i) ){
          m_PreTreatedData->SetCATSStripX( *(m_CATSXInversion[m_EventData->GetCATSDetX(i)-1].begin() + m_EventData->GetCATSStripX(i)-1) );
        }
        else {
          m_PreTreatedData->SetCATSStripX( m_EventData->GetCATSStripX(i) );
        }
        m_PreTreatedData->SetCATSDetX( m_EventData->GetCATSDetX(i) );
      }
    }
  }

  // Y
  unsigned int sizeY = m_EventData->GetCATSMultY();
  for(unsigned int i = 0 ; i < sizeY ; i++){
    // Valid Channel Y
    if(IsValidChannel("Y", m_EventData->GetCATSDetY(i), m_EventData->GetCATSStripY(i))){
      if( fCATS_Threshold_Y(m_EventData , i) ){
        double QY = fCATS_Y_Q(m_EventData , i);
        m_PreTreatedData->SetCATSChargeY( QY );
        //Inversion Y
        if( *(m_CATSYInversion[m_EventData->GetCATSDetY(i)-1].begin() + m_EventData->GetCATSStripY(i)-1) != m_EventData->GetCATSStripY(i) ){
          m_PreTreatedData->SetCATSStripY( *(m_CATSYInversion[m_EventData->GetCATSDetY(i)-1].begin() + m_EventData->GetCATSStripY(i)-1) );
        }
        else {
          m_PreTreatedData->SetCATSStripY( m_EventData->GetCATSStripY(i) );
        }
        m_PreTreatedData->SetCATSDetY( m_EventData->GetCATSDetY(i) );
      }
    }
  }
  return;
}

/////////////////////////////////////////////////////////////////////////////
void TCATSPhysics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

//////////////////////////////////////////////////////////////////////////////		
void TCATSPhysics::BuildPhysicalEvent(){
  PreTreat();
  // Look how many CATS were fired
  // use a set to identify which detector has been hit
  set<int> DetectorHitX; // X only
  set<int> DetectorHit; // X and Y

  unsigned int sizeX = m_PreTreatedData->GetCATSMultX() ;
  for( unsigned short i = 0 ; i < m_PreTreatedData->GetCATSMultX() ; i++ ){ 
    // Insert detector number in the set, if the key already exist, do nothing
    DetectorHitX.insert(m_PreTreatedData->GetCATSDetX(i));
  }

  // Correspond to CATS with both X and Y
  unsigned int sizeY = m_PreTreatedData->GetCATSMultY() ;
  for( unsigned short i = 0 ; i < m_PreTreatedData->GetCATSMultY() ; i++ ){ 
    // Insert detector number in the set, if the key already exist, do nothing
    // Only if the detector was hit on X as well
    if(DetectorHitX.find(m_PreTreatedData->GetCATSDetY(i))!=DetectorHitX.end())
      DetectorHit.insert(m_PreTreatedData->GetCATSDetY(i));
  }
  // The number of CATS hit, i.e. the number of CATS that we are going to analyse
  unsigned int NumberOfCATSHit = DetectorHit.size();

  vector<double> ChargeArray;
  ChargeArray.resize(28,0);

  // INITIALISATION OF VECTORS : DIM = NumberOfCATSHit
  for(set<int>::iterator it=DetectorHit.begin(); it!=DetectorHit.end(); ++it){
    // Assign order for the detector number
    DetMaxX.push_back(*it);
    DetMaxY.push_back(*it);
    // X
    StripMaxX.push_back(1); 
    ChargeMaxX.push_back(1);
    QsumX.push_back(0);
    // Y
    StripMaxY.push_back(1); 
    ChargeMaxY.push_back(1);
    QsumY.push_back(0);

    Buffer_X_Q.push_back(ChargeArray);
    Buffer_Y_Q.push_back(ChargeArray);  
  }

  // Fill up the Charge And Max field for X
  for(unsigned int i = 0 ; i < sizeX ; i++ ){
    int StrX					         = m_PreTreatedData->GetCATSStripX(i);
    int NX						         = m_PreTreatedData->GetCATSDetX(i);
    double CATS_X_Q				     = m_PreTreatedData->GetCATSChargeX(i) ;
    ChargeX.push_back(CATS_X_Q);
    StripX.push_back(StrX);
    DetNumberX.push_back(NX);
    for(unsigned int j = 0 ; j < NumberOfCATSHit ; j++){
      if(NX == DetMaxX[j] ){
        Buffer_X_Q[j][StrX-1]= CATS_X_Q;
        QsumX[j]+= CATS_X_Q;	
        if(CATS_X_Q > Buffer_X_Q[j][StripMaxX[j] -1]){ 
          StripMaxX[j] = StrX ; 
          ChargeMaxX[j]= CATS_X_Q; 
        }
      }
    }
  }

  // Fill up the Charge And Max field for Y
  for(unsigned int i = 0 ; i < sizeY ; i++ ){
    int StrY					         = m_PreTreatedData->GetCATSStripY(i);
    int NY						         = m_PreTreatedData->GetCATSDetY(i);
    double CATS_Y_Q				     = m_PreTreatedData->GetCATSChargeY(i) ;
    ChargeY.push_back(CATS_Y_Q);
    StripY.push_back(StrY);
    DetNumberY.push_back(NY);

    for(unsigned int j = 0 ; j < NumberOfCATSHit ; j++){
      if(NY == DetMaxY[j] ){
        Buffer_Y_Q[j][StrY-1]= CATS_Y_Q;
        QsumY[j]+= CATS_Y_Q;	
        if(CATS_Y_Q > Buffer_Y_Q[j][StripMaxY[j] -1]){ 
          StripMaxY[j] = StrY ; 
          ChargeMaxY[j]= CATS_Y_Q; 
        }
      }
    }
  }

  //  double CalculatedStripX = 0, CalculatedStripY = 0;
  //  double posX = 0 , posY = 0;

  for(unsigned int i  = 0 ; i < NumberOfCATSHit ; i++ ){       
    // Return the position in strip unit
     // Convention: the collected charge is atrributed to the center of the strip
     // (histogram convention) so that a reconstructed position for a single strip
     // goes from strip index -0.5 to strip index +0.5
    double PosX =  ReconstructionFunctionX[DetMaxX[i]-1](Buffer_X_Q[i],StripMaxX[i]);
    double PosY =  ReconstructionFunctionY[DetMaxY[i]-1](Buffer_Y_Q[i],StripMaxY[i]);
    StripNumberX.push_back(PosX);
    StripNumberY.push_back(PosY);   

    // a shift - -1 is made to have PosX in between -0.5 and 27.5
    // for the following calculation of the position in the lab.
    PosX = PosX -1;
    PosY = PosY -1;

    // sx and sy are the X and Y strip number between which the PosX and PosY are
    int sx0 = (int) PosX;
    int sx1 = sx0+1; 
    int sy0 = (int) PosY;
    int sy1 = sy0+1; 

    if(PosX>-1000 && PosY>-1000 && sx0 > -1 && sx1 < 28 && sy0 > -1  && sy1 < 28){
      // px and py are the x and y coordinate of strip sx and sy 
      double px0 = StripPositionX[DetMaxX[i]-1][sx0][sy0];
      double px1 = StripPositionX[DetMaxX[i]-1][sx1][sy1];

      double py0 = StripPositionY[DetMaxY[i]-1][sx0][sy0];
      double py1 = StripPositionY[DetMaxY[i]-1][sx1][sy1];

      PositionX.push_back(px0+(px1-px0)*(PosX-sx0));  
      PositionY.push_back(py0+(py1-py0)*(PosY-sy0));  
      PositionZ.push_back(StripPositionZ[DetMaxX[i]-1]);
    }

  }

  // At least two CATS need to gave back position in order to reconstruct on Target 
  if(PositionX.size()>1){
    if(DetMaxX[0]<DetMaxX[1]){
      double t = -PositionZ[1]/(PositionZ[1]-PositionZ[0]);
      PositionOnTargetX= PositionX[1] + (PositionX[1]-PositionX[0])*t;
      PositionOnTargetY= PositionY[1] + (PositionY[1]-PositionY[0])*t; 
      BeamDirection = GetBeamDirection();
    }

    else{
      double t = -PositionZ[0]/(PositionZ[0]-PositionZ[1]);
      PositionOnTargetX= PositionX[0] + (PositionX[0]-PositionX[1])*t;
      PositionOnTargetY= PositionY[0] + (PositionY[0]-PositionY[1])*t; 
      BeamDirection = GetBeamDirection();
    }
  }

  // Does not meet the conditions for target position and beam direction 
  else{
    BeamDirection = TVector3 (1,0,0);
    PositionOnTargetX = -1000	;
    PositionOnTargetY = -1000	;
  }
  return;
}

///////////////////////////////////////////////////////////////////////////
//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void TCATSPhysics::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("CATSDetector");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"X1_Y1","X28_Y1","X1_Y28","X28_Y28"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      TVector3 A = blocks[i]->GetTVector3("X1_Y1","mm");
      TVector3 B = blocks[i]->GetTVector3("X28_Y1","mm");
      TVector3 C = blocks[i]->GetTVector3("X1_Y28","mm");
      TVector3 D = blocks[i]->GetTVector3("X28_Y28","mm");

      AddCATS(A,B,C,D);
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

  InitializeStandardParameter();
  ReadAnalysisConfig();
}

/////////////////////////////////////////////////////////////////////
//	Activated associated Branches and link it to the private member DetectorData address
//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void TCATSPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain()	;
  inputChain->SetBranchStatus( "CATS" , true )			;
  inputChain->SetBranchStatus( "fCATS_*" , true )		;
  inputChain->SetBranchAddress( "CATS" , &m_EventData )           ;
}

/////////////////////////////////////////////////////////////////////
//   Activated associated Branches and link it to the private member DetectorPhysics address
//   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
void TCATSPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "CATS" , true );
  inputChain->SetBranchStatus( "DetNumberX" , true );
  inputChain->SetBranchStatus( "StripX" , true );
  inputChain->SetBranchStatus( "ChargeX" , true );
  inputChain->SetBranchStatus( "StripMaxX" , true );
  inputChain->SetBranchStatus( "DetNumberY" , true );
  inputChain->SetBranchStatus( "StripY" , true );
  inputChain->SetBranchStatus( "ChargeY" , true );
  inputChain->SetBranchStatus( "StripMaxY" , true );
  inputChain->SetBranchStatus( "DetMaxX" , true );
  inputChain->SetBranchStatus( "DetMaxY" , true );
  inputChain->SetBranchStatus( "PositionX" , true );
  inputChain->SetBranchStatus( "PositionY" , true );
  inputChain->SetBranchStatus( "PositionZ" , true );
  inputChain->SetBranchStatus( "StripNumberX" , true );
  inputChain->SetBranchStatus( "StripNumberY" , true );
  inputChain->SetBranchStatus( "PositionOnTargetX" , true );
  inputChain->SetBranchStatus( "PositionOnTargetY" , true );
  inputChain->SetBranchStatus( "QsumX" , true );
  inputChain->SetBranchStatus( "QsumY" , true );
  inputChain->SetBranchAddress( "CATS" , &m_EventPhysics );

}

/////////////////////////////////////////////////////////////////////
//	Create associated branches and associated private member DetectorPhysics address
void TCATSPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree()		;
  outputTree->Branch( "CATS" , "TCATSPhysics" , &m_EventPhysics )	;
}

/////////////////////////////////////////////////////////////////////
void TCATSPhysics::AddCATS(TVector3 C_X1_Y1, TVector3 C_X28_Y1, TVector3 C_X1_Y28, TVector3 C_X28_Y28){
  m_NumberOfCATS++			;

  // remove warning
  C_X28_Y28 *= 1;

  //	Vector U on Telescope Face (paralelle to Y Strip) 
  TVector3 U = C_X28_Y1 - C_X1_Y1 				;	
  U = U.Unit()									;

  //	Vector V on Telescope Face (parallele to X Strip)
  TVector3 V = C_X1_Y28 - C_X1_Y1 				;
  V = V.Unit()									;

  //	Position Vector of Strip Center
  TVector3 StripCenter 					;
  //	Position Vector of X=1 Y=1 Strip 
  TVector3 Strip_1_1 					;		

  //	Geometry Parameters  
  double Face = 71.12 				; //mm
  double NumberOfStrip = 28 			;
  double StripPitch = Face / NumberOfStrip	; //mm

  //	Buffer object to fill Position Array
  vector<double> lineX ; 
  vector<double> lineY ; 

  vector< vector< double > >	OneDetectorStripPositionX	;
  vector< vector< double > >	OneDetectorStripPositionY	;
  double                 	OneDetectorStripPositionZ	;

  //	Moving StripCenter to 1.1 corner (strip center!) :
  Strip_1_1 = C_X1_Y1 + (U+V) * (StripPitch/2) 	;

  for( int i = 0 ; i < 28 ; i++ ){
    lineX.clear()	;
    lineY.clear()	;

    for( int j = 0 ; j < 28 ; j++ ){
      StripCenter  = Strip_1_1 + StripPitch*( i*U + j*V  )	;
      lineX.push_back( StripCenter.x() )	;
      lineY.push_back( StripCenter.y() )	;	
    }

    OneDetectorStripPositionX.push_back(lineX);
    OneDetectorStripPositionY.push_back(lineY);
  }

  OneDetectorStripPositionZ = C_X1_Y1.Z();

  StripPositionX.push_back(OneDetectorStripPositionX)	;
  StripPositionY.push_back(OneDetectorStripPositionY)	;
  StripPositionZ.push_back(OneDetectorStripPositionZ)	;

}

///////////////////////////////////////////////////////////////
void TCATSPhysics::Clear(){  
  DetNumberX.clear(); 
  StripX.clear();
  ChargeX.clear();  
  StripMaxX.clear();
  ChargeMaxX.clear();
  DetMaxX.clear();
  DetNumberY.clear(); 
  StripY.clear();
  ChargeY.clear(); 
  StripMaxY.clear();
  ChargeMaxY.clear();
  DetMaxY.clear();
  PositionX.clear();
  PositionY.clear();
  PositionZ.clear();
  StripNumberX.clear();
  StripNumberY.clear();
  QsumX.clear();
  QsumY.clear();

  Buffer_X_Q.clear();
  Buffer_Y_Q.clear();
}

////////////////////////////////////////////////////////////////////////////
bool TCATSPhysics :: IsValidChannel(const string DetectorType, const int Detector , const int channel) {
  if(DetectorType == "X")
    return *(m_XChannelStatus[Detector-1].begin()+channel-1);

  else if(DetectorType == "Y")
    return *(m_YChannelStatus[Detector-1].begin()+channel-1);

  else return false;
}


///////////////////////////////////////////////////////////////////////////////////
void TCATSPhysics::InitializeStandardParameter(){
  //   Enable all channel and no inversion
  vector< bool > ChannelStatus;
  vector< int > InversionStatus;
  m_XChannelStatus.clear()    ;
  m_YChannelStatus.clear()    ;
  m_CATSXInversion.clear()   ;
  m_CATSYInversion.clear()   ;

  ChannelStatus.resize(28,true);
  InversionStatus.resize(28);
  for(unsigned int j = 0 ; j < InversionStatus.size() ; j++){
    InversionStatus[j] = j+1;
  }

  for(int i = 0 ; i < m_NumberOfCATS ; ++i){
    m_XChannelStatus[i] = ChannelStatus;
    m_YChannelStatus[i] = ChannelStatus;
    m_CATSXInversion[i] = InversionStatus;
    m_CATSYInversion[i] = InversionStatus;
    SetReconstructionMethod(i+1, "X", "AGAUSS");
    SetReconstructionMethod(i+1, "Y", "AGAUSS");
  }

  return;
}   

///////////////////////////////////////////////////////////////////////////
void TCATSPhysics::ReadAnalysisConfig(){
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigCATS.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigCATS.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigCATS.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigCATS.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 10, "ConfigCATS") == 0) ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus ) {

      whatToDo="";
      AnalysisConfigFile >> whatToDo;

      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
      }

      else if (whatToDo == "DISABLE_CHANNEL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        int Detector = atoi(DataBuffer.substr(4,1).c_str());
        int channel = -1;
        if (DataBuffer.compare(5,4,"STRX") == 0) {
          channel = atoi(DataBuffer.substr(9).c_str());
          *(m_XChannelStatus[Detector-1].begin()+channel-1) = false;
        }

        else if (DataBuffer.compare(5,4,"STRY") == 0) {
          channel = atoi(DataBuffer.substr(9).c_str());
          *(m_YChannelStatus[Detector-1].begin()+channel-1) = false;
        }

        else cout << "Warning: detector type for CATS unknown!" << endl;

      }

      else if (whatToDo == "INVERSION") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer;
        int Detector = atoi(DataBuffer.substr(4,1).c_str());
        int channel1 = -1;
        int channel2 = -1;
        AnalysisConfigFile >> DataBuffer;
        cout << " " << DataBuffer;
        if (DataBuffer.compare(0,4,"STRX") == 0) {
          channel1 = atoi(DataBuffer.substr(4).c_str());
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          channel2 = atoi(DataBuffer.substr(4).c_str());
          *(m_CATSXInversion[Detector-1].begin()+channel1-1) = channel2;
          *(m_CATSXInversion[Detector-1].begin()+channel2-1) = channel1;
        }

        else if (DataBuffer.compare(0,4,"STRY") == 0) {
          channel1 = atoi(DataBuffer.substr(4).c_str());
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          channel2 = atoi(DataBuffer.substr(4).c_str());
          *(m_CATSYInversion[Detector-1].begin()+channel1-1) = channel2;
          *(m_CATSYInversion[Detector-1].begin()+channel2-1) = channel1;
        }
      }

      else if (whatToDo == "RECONSTRUCTION_METHOD") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer ;
        // DataBuffer is of form CATSNX 
        // Look for the CATS Number removing the first 4 letters and the trailling letter
        string Duplicate = DataBuffer.substr(4); // Duplicate is of form NX
        Duplicate.resize(Duplicate.size()-1); // Duplicate is of form
        unsigned int CATSNumber =  atoi(Duplicate.c_str());

        // Look for the X or Y part of the Detector, Basically the last character
        string XorY(string(1,DataBuffer[DataBuffer.size()-1])) ; 

        // Get the Reconstruction Methods Name
        AnalysisConfigFile >> DataBuffer;       
        cout << " " << DataBuffer << endl ;
        // Set the Reconstruction Methods using above information 
        SetReconstructionMethod(CATSNumber,XorY,DataBuffer);
      }

      else {ReadingStatus = false;}

    }
  }
} 
///////////////////////////////////////////////////////////////////////////
void TCATSPhysics::InitSpectra(){
  m_Spectra = new TCATSSpectra(m_NumberOfCATS);
}

///////////////////////////////////////////////////////////////////////////
void TCATSPhysics::FillSpectra(){
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TCATSPhysics::CheckSpectra(){
  // To be done
}
///////////////////////////////////////////////////////////////////////////
void TCATSPhysics::ClearSpectra(){
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TCATSPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();

  else{
    map< string , TH1*> empty; 
    return empty;
  }
}

///////////////////////////////////////////////////////////////////////////
void TCATSPhysics::WriteSpectra(){
  if(m_Spectra)
    m_Spectra->WriteSpectra();
}

/////////////////////////////////////////////////////////////////////
//	Add Parameter to the CalibrationManger
void TCATSPhysics::AddParameterToCalibrationManager()	{
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for(int i = 0 ; i < m_NumberOfCATS ; i++){
    for( int j = 0 ; j < 28 ; j++){
      Cal->AddParameter("CATS", "D"+NPL::itoa(i+1)+"_X"+NPL::itoa(j+1)+"_Q","CATS_D"+NPL::itoa(i+1)+"_X"+NPL::itoa(j+1)+"_Q")	;
      Cal->AddParameter("CATS", "D"+NPL::itoa(i+1)+"_Y"+NPL::itoa(j+1)+"_Q","CATS_D"+NPL::itoa(i+1)+"_Y"+NPL::itoa(j+1)+"_Q")	;
      Cal->AddParameter("CATS", "D"+NPL::itoa(i+1)+"_X"+NPL::itoa(j+1),"CATS_D"+NPL::itoa(i+1)+"_X"+NPL::itoa(j+1))	;
      Cal->AddParameter("CATS", "D"+NPL::itoa(i+1)+"_Y"+NPL::itoa(j+1),"CATS_D"+NPL::itoa(i+1)+"_Y"+NPL::itoa(j+1))	;
    } 
  }

  return;
}	

////////////////////////////////////////////////////////////////
void TCATSPhysics::SetReconstructionMethod(unsigned int CATSNumber, string XorY, string MethodName){
  if(XorY=="X"){
    if(ReconstructionFunctionX.size() < CATSNumber)
      ReconstructionFunctionX.resize(CATSNumber);

    if(MethodName=="ASECH") ReconstructionFunctionX[CATSNumber-1] = &(AnalyticHyperbolicSecant);
    else if(MethodName=="FSECH") ReconstructionFunctionX[CATSNumber-1] = &(FittedHyperbolicSecant);
    else if(MethodName=="AGAUSS") ReconstructionFunctionX[CATSNumber-1] = &(AnalyticGaussian);
    else if(MethodName=="CENTROIDE")  ReconstructionFunctionX[CATSNumber-1] = &(Centroide); 
    else cout <<"WARNING: Wrong name for reconsctuction Method, using default AGAUSS" << endl;
  }

  if(XorY=="Y"){
    if(ReconstructionFunctionY.size() < CATSNumber)
      ReconstructionFunctionY.resize(CATSNumber);

    if(MethodName=="ASECH") ReconstructionFunctionY[CATSNumber-1] = &(AnalyticHyperbolicSecant);
    else if(MethodName=="FSECH") ReconstructionFunctionY[CATSNumber-1] = &(FittedHyperbolicSecant);
    else if(MethodName=="AGAUSS") ReconstructionFunctionY[CATSNumber-1] = &(AnalyticGaussian);
    else if(MethodName=="CENTROIDE")  ReconstructionFunctionY[CATSNumber-1] = &(Centroide); 
    else cout <<"WARNING: Wrong name for reconsctuction Method, using default AGAUSS" << endl;
  }

}

///////////////////////////////////////////////////////////////
TVector3 TCATSPhysics::GetBeamDirection(){
  TVector3 Direction;
  if(PositionX.size() <2)return Direction;

  if(DetMaxX[0]<DetMaxX[1]){
    Direction = TVector3 (PositionX[1]-PositionX[0] ,
        PositionY[1]-PositionY[0] ,
        PositionZ[1]-PositionZ[0] );
    Direction.Unit();
  }

  else{
    Direction = TVector3 (PositionX[0]-PositionX[1] ,
        PositionY[0]-PositionY[1] ,
        PositionZ[0]-PositionZ[1] );
    Direction.Unit();
  }

  return(Direction) ;	
}

///////////////////////////////////////////////////////////////
TVector3 TCATSPhysics::GetPositionOnTarget(){
  double Pi = 3.14159265;
  TVector3 Position = TVector3 (GetPositionOnTargetX() 	,
      GetPositionOnTargetY() 	,
      GetPositionOnTargetX()*tan(m_TargetAngle*Pi/180)); 
  return(Position) ;	
}

////////////////////////////////////////////////////////////////////////
namespace CATS_LOCAL{
  ////////////////////////////////////////////////////////////////////
  double AnalyticGaussian(vector<double>& Buffer_Q,int& StripMax){
    double gauss = -1000;  
    double Q[3];
    double StripPos[3];
    for(int j = 0; j<3 ; j++){
      Q[j] = 0;
      StripPos[j] = 0;
    }

    if(StripMax> 3 && StripMax< 26){
      // central value taken using the Strip with Max charge
      Q[0] = Buffer_Q[StripMax-1] ;

      // Look at the next strip on the left
      if(Buffer_Q[StripMax-2]!=-1){
        Q[1] = Buffer_Q[StripMax-2];
        StripPos[1] = StripMax-2;
      }

      // Look at the next next strip on the left
      else if(Buffer_Q[StripMax-3]!=-1){
        Q[1] = Buffer_Q[StripMax-3];
        StripPos[1] = StripMax-3;
      }

      // Look at the next next next strip on the left
      else if(Buffer_Q[StripMax-4]!=-1){
        Q[1] = Buffer_Q[StripMax-4];
        StripPos[1] = StripMax-4;
      }

      // Look at the next strip on the right
      if(Buffer_Q[StripMax]!=-1){
        Q[2] = Buffer_Q[StripMax];
        StripPos[2] = StripMax;
      }

      // Look at the next next strip on the right
      else if(Buffer_Q[StripMax+1]!=-1){
        Q[2] = Buffer_Q[StripMax+1];
        StripPos[2] = StripMax+1;
      }

      // Look at the next next next strip on the right 
      else if(Buffer_Q[StripMax+2]!=-1){
        Q[2] = Buffer_Q[StripMax+2];
        StripPos[2] = StripMax+2;
      }

    }

    double Q0_Q1 = log(Q[0]/Q[1]);
    double Q0_Q2 = log(Q[0]/Q[2]);

    double num   = Q0_Q1 * (StripPos[2]*StripPos[2] - StripPos[0]*StripPos[0]) - Q0_Q2 * (StripPos[1]*StripPos[1] - StripPos[0]*StripPos[0])  ;
    double denom = Q0_Q1 * (StripPos[2] - StripPos[0]) - Q0_Q2 * (StripPos[1] - StripPos[0]) ;

    if(denom != 0){
      gauss = 0.5*num / denom;
    }

    else{
      gauss = -1000;
    }

    return gauss;

  }

  ///////////////////////////////////////////////////////////////
  double Centroide(vector<double>& Buffer_Q, int& StripMax){
    double Centroide = 0 ;
    double ChargeTotal = 0;

    unsigned int sizeQ = Buffer_Q.size(); 
    for(unsigned int i = 0 ; i < sizeQ ; i++){
      if(Buffer_Q[i]>0){ 
        Centroide += (i)*Buffer_Q[i-1] ;
        ChargeTotal+=Buffer_Q[i-1];
      }
    }

    if(ChargeTotal>0) Centroide = Centroide / ChargeTotal ;

    else {
      Centroide = -1000 ; 
    } 

    return Centroide ;
  }

  /////////////////////////////////////////////////////////////////////
  double AnalyticHyperbolicSecant(vector<double>& Buffer_Q, int& StripMax){
    double sech = -1000 ;

    if(StripMax > 2 && StripMax<27){	
      if(Buffer_Q[StripMax-1+1]==0||Buffer_Q[StripMax-1-1]==0)
        return sech;

      double vs1 = sqrt( Buffer_Q[StripMax-1]/Buffer_Q[StripMax-1+1] );
      double vs2 = sqrt( Buffer_Q[StripMax-1]/Buffer_Q[StripMax-1-1] );
      double vs3 = 0.5*( vs1 + vs2 );
      double vs4 = log( vs3 + sqrt(vs3*vs3-1.0) );
      double vs5 = (vs1 - vs2)/(2.0*sinh(vs4));	

      if(vs5<0) 
        vs5=-vs5 ;

      double vs6 = 0.5*log( (1.0+vs5)/(1.0-vs5) ) ;

      if ( Buffer_Q[StripMax-1+1]>Buffer_Q[StripMax-1-1] ) 
        sech = StripMax + vs6/vs4 ;


      else 
        sech = StripMax - vs6/vs4 ;

    }

    return sech ;
  }

  /////////////////////////////////////////////////////////////////////
  double FittedHyperbolicSecant(vector<double>& Buffer_Q, int& StripMax){
    // Warning: should not delete static variable
    static TF1* f = new TF1("sechs","[0]/(cosh(TMath::Pi()*(x-[1])/[2])*cosh(TMath::Pi()*(x-[1])/[2]))",1,28);

    // Help the fit by computing the position of the maximum by analytic method
    double StartingPoint = AnalyticHyperbolicSecant(Buffer_Q,StripMax);
    // if analytic method fails then the starting point in strip max
    if(StartingPoint==-1000) StartingPoint = StripMax; 

    // Maximum is close to charge max, Mean value is close to Analytic one, typical width is 3.8 strip
    f->SetParameters(Buffer_Q[StripMax-1],StartingPoint,3.8);

    static vector<double> y ;
    static vector<double> q ; 
    y.clear(); q.clear();
    double final_size = 0 ;
    unsigned int sizeQ = Buffer_Q.size(); 

    for(unsigned int i = 0 ; i < sizeQ ; i++){
      if(Buffer_Q[i] > Buffer_Q[StripMax-1]*0.2){
        q.push_back(Buffer_Q[i]);
        y.push_back(i+1);
        final_size++;
      }
    }

    // requiered at least 3 point to perfom a fit
    if(final_size<3){
      return -1000 ;
    }

    TGraph* g = new TGraph(q.size(),&y[0],&q[0]);
    g->Fit(f,"QN0");
    delete g;
    return f->GetParameter(1)  ;
  }



  ////////////////////////////////////////////////////////////////////////
  double fCATS_X_Q(const TCATSData* m_EventData , const int i){
    static string name; 
    name = "CATS/D" ;
    name+= NPL::itoa( m_EventData->GetCATSDetX(i) ) ;
    name+= "_X" ;
    name+= NPL::itoa( m_EventData->GetCATSStripX(i) ) ;
    name+= "_Q";
    return CalibrationManager::getInstance()->ApplyCalibration( name,   
        m_EventData->GetCATSChargeX(i) + gRandom->Rndm() - fCATS_Ped_X(m_EventData, i) );
  }
  ////////////////////////////////////////////////////////////////////////
  double fCATS_Y_Q(const TCATSData* m_EventData , const int i){
    static string name; 
    name = "CATS/D" ;
    name+= NPL::itoa( m_EventData->GetCATSDetY(i) ) ;
    name+= "_Y" ;
    name+= NPL::itoa( m_EventData->GetCATSStripY(i) ) ;
    name+= "_Q";
    return CalibrationManager::getInstance()->ApplyCalibration( name ,   
        m_EventData->GetCATSChargeY(i) + gRandom->Rndm() - fCATS_Ped_Y(m_EventData, i) );
  }
  ////////////////////////////////////////////////////////////////////////
  bool fCATS_Threshold_X(const TCATSData* m_EventData , const int i){
    static string name; 
    name = "CATS/D" ;
    name+= NPL::itoa( m_EventData->GetCATSDetX(i) ) ;
    name+= "_X" ;
    name+= NPL::itoa( m_EventData->GetCATSStripX(i) );
    return CalibrationManager::getInstance()->ApplyThreshold(name,
        m_EventData->GetCATSChargeX(i));
  }
  ////////////////////////////////////////////////////////////////////////
  bool fCATS_Threshold_Y(const TCATSData* m_EventData , const int i){
    static string name; 
    name ="CATS/D" ;
    name+= NPL::itoa( m_EventData->GetCATSDetY(i) ) ;
    name+= "_Y" ;
    name+= NPL::itoa( m_EventData->GetCATSStripY(i) );
    return CalibrationManager::getInstance()->ApplyThreshold( name,
        m_EventData->GetCATSChargeY(i));
  }
  ////////////////////////////////////////////////////////////////////////
  double fCATS_Ped_X(const TCATSData* m_EventData, const int i){
    static string name; 
    name =  "CATS/D" ;
    name+= NPL::itoa( m_EventData->GetCATSDetX(i) ) ;
    name+= "_X" ;
    name+= NPL::itoa( m_EventData->GetCATSStripX(i) ) ;
    return CalibrationManager::getInstance()->GetPedestal(name);
  }
  ////////////////////////////////////////////////////////////////////////
  double fCATS_Ped_Y(const TCATSData* m_EventData, const int i){
    static string name; 
    name = "CATS/D" ;
    name+= NPL::itoa( m_EventData->GetCATSDetY(i) ) ;
    name+= "_Y" ;
    name+= NPL::itoa( m_EventData->GetCATSStripY(i) );
    return CalibrationManager::getInstance()->GetPedestal( name );
  }
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TCATSPhysics::Construct(){
  return (NPL::VDetector*) new TCATSPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_cats{
  public:
    proxy_cats(){
      NPL::DetectorFactory::getInstance()->AddToken("CATSDetector","CATS");
      NPL::DetectorFactory::getInstance()->AddDetector("CATSDetector",TCATSPhysics::Construct);
    }
};

proxy_cats p;
}

