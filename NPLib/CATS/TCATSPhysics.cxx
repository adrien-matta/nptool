/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
using namespace LOCAL_CATS;

//	STL
#include <cmath>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
//	NPL
#include "RootInput.h"
#include "RootOutput.h"
//	ROOT
#include "TChain.h"
#include "TRandom.h"

ClassImp(TCATSPhysics)

  ///////////////////////////////////////////////////////////////////////////
TCATSPhysics::TCATSPhysics()
{
  m_EventData 				= new TCATSData	;
  m_PreTreatedData      = new TCATSData ;
  m_EventPhysics 			= this			;
  m_Spectra             = 0;
  m_NumberOfCATS        = 0             ;
}

///////////////////////////////////////////////////////////////////////////
TCATSPhysics::~TCATSPhysics()
{
}

///////////////////////////////////////////////////////////////////////////
void TCATSPhysics::PreTreat()
{ 
  ClearPreTreatedData();   
  gRandom->SetSeed(0);
  // X
  for(int i = 0 ; i < m_EventData->GetCATSMultX() ; i++)
  {
    // Valid Channel X
    if(IsValidChannel("X", m_EventData->GetCATSDetX(i), m_EventData->GetCATSStripX(i)) )
    {
      if( fCATS_Threshold_X(m_EventData , i) )
      {
        double QX = fCATS_X_Q(m_EventData , i);
        m_PreTreatedData->SetCATSChargeX( QX );
        //Inversion X
        if( *(m_CATSXInversion[m_EventData->GetCATSDetX(i)-1].begin() + m_EventData->GetCATSStripX(i)-1) != m_EventData->GetCATSStripX(i) )
        {
          m_PreTreatedData->SetCATSStripX( *(m_CATSXInversion[m_EventData->GetCATSDetX(i)-1].begin() + m_EventData->GetCATSStripX(i)-1) );
        }
        else {m_PreTreatedData->SetCATSStripX( m_EventData->GetCATSStripX(i) );}
        m_PreTreatedData->SetCATSDetX( m_EventData->GetCATSDetX(i) );
      }
    }
  }

  // Y
  for(int i = 0 ; i < m_EventData->GetCATSMultY() ; i++)
  {
    // Valid Channel Y
    if(IsValidChannel("Y", m_EventData->GetCATSDetY(i), m_EventData->GetCATSStripY(i)))
    {
      if( fCATS_Threshold_Y(m_EventData , i) )
      {
        double QY = fCATS_Y_Q(m_EventData , i);
        m_PreTreatedData->SetCATSChargeY( QY );
        //Inversion Y
        if( *(m_CATSYInversion[m_EventData->GetCATSDetY(i)-1].begin() + m_EventData->GetCATSStripY(i)-1) != m_EventData->GetCATSStripY(i) )
        {
          m_PreTreatedData->SetCATSStripY( *(m_CATSYInversion[m_EventData->GetCATSDetY(i)-1].begin() + m_EventData->GetCATSStripY(i)-1) );
        }
        else {m_PreTreatedData->SetCATSStripY( m_EventData->GetCATSStripY(i) );}
        m_PreTreatedData->SetCATSDetY( m_EventData->GetCATSDetY(i) );
      }
    }
  }
  

  return;
}

/////////////////////////////////////////////////////////////////////////////
void TCATSPhysics::BuildSimplePhysicalEvent()				
{
  BuildPhysicalEvent();
}

//////////////////////////////////////////////////////////////////////////////		
void TCATSPhysics::BuildPhysicalEvent(){
  PreTreat();
  double Pi = 3.14159265;
  //	How many CATS?
  int NumberOfCATSHit = 0 ;
  int DetectorID = -1;
  double SumChargeX[2];
  double SumChargeY[2];
  for( unsigned short i = 0 ; i < m_PreTreatedData->GetCATSMultX() ; i++ ){ 
    if( m_PreTreatedData->GetCATSDetX(i) != DetectorID)  {
      NumberOfCATSHit++;
    }
    if(NumberOfCATSHit == m_NumberOfCATS) break;	
  }


  // INITIALISATION OF VECTORS : DIM = NUMBER OF CATS
  for(int k = 0 ; k < NumberOfCATSHit ; k++ ){
    // X
    StripMaxX.push_back(-1); 
    ReconstructionMethodX.push_back(NO);
    SumChargeX[k] = 0;

    // Y
    StripMaxY.push_back(-1); 
    ReconstructionMethodY.push_back(NO);
    SumChargeY[k] = 0;
  }
  for(int p = 0 ; p < m_NumberOfCATS ; p++){
    for(int z=0; z<28; z++) {
      Buffer_X_Q[z][p] = -1;
      Buffer_Y_Q[z][p] = -1;
    }
  }
  
  for(unsigned int i = 0 ; i < m_PreTreatedData->GetCATSMultX() ; i++ ){
    int StrX					= m_PreTreatedData->GetCATSStripX(i);
    int NX						= m_PreTreatedData->GetCATSDetX(i);
    double CATS_X_Q		= m_PreTreatedData->GetCATSChargeX(i) ;            

    Buffer_X_Q[StrX-1][NX-1]	= CATS_X_Q;
    SumChargeX[NX-1]			+= CATS_X_Q;
    ChargeX.push_back(CATS_X_Q);
    StripX.push_back(StrX);
    DetNumberX.push_back(NX);
    HitX++;
    if(HitX==1) StripMaxX[NX-1] = StrX; 
    else if(ChargeX[HitX-1] > Buffer_X_Q[StripMaxX[NX-1] -1][NX-1] ) StripMaxX[NX-1] = StrX ;  
}
  for(unsigned int j = 0 ; j < m_PreTreatedData->GetCATSMultY() ; j++ ){
    int StrY					= m_PreTreatedData->GetCATSStripY(j);
    int NY						= m_PreTreatedData->GetCATSDetY(j);
    double CATS_Y_Q		= m_PreTreatedData->GetCATSChargeY(j) ;

    Buffer_Y_Q[StrY-1][NY-1]	= CATS_Y_Q;
    SumChargeY[NY-1]			+= CATS_Y_Q;

    ChargeY.push_back(CATS_Y_Q);
    StripY.push_back(StrY);
    DetNumberY.push_back(NY);
    HitY++;
    if(HitY==1) StripMaxY[NY-1] = StrY; 
    else if(ChargeY[HitY-1] > Buffer_Y_Q[StripMaxY[NY-1] -1][NY-1] ) StripMaxY[NY-1] = StrY ; 
  }
  
  double CalculatedStripX = 0, CalculatedStripY = 0;
  double posX = 0 , posY = 0;

  for(ff = 0 ; ff < NumberOfCATSHit ; ff++ ){       
    CalculatedStripX = AnalyseX(ff);         
    CalculatedStripY = AnalyseY(ff); 

    posX = CalculatePositionX(CalculatedStripX, NOcor);   
    posY = CalculatePositionY(CalculatedStripY, NOcor);  

    DetNumberX_Position.push_back(ff+1);
    DetNumberY_Position.push_back(ff+1);
    DetNumberZ_Position.push_back(ff+1);

    PositionX.push_back(posX) ;                  
    PositionY.push_back(posY) ;                  
    PositionZ.push_back(StripPositionZ[ff]) ;

    QsumX.push_back(SumChargeX[ff]);
    QsumY.push_back(SumChargeY[ff]);
  }

  if(NumberOfCATSHit > 1){
    if(PositionX[0] != -1000 && PositionY[0] != -1000 && PositionX[1] != -1000 && PositionY[1] != -1000) {
      double PositionOnTargetX_1;
      double PositionOnTargetY_1;
      double l = sqrt((PositionZ[0]-PositionZ[1])*(PositionZ[0]-PositionZ[1]));
      double L = - PositionZ[1];
      double t = (l+L) / l;

      PositionOnTargetX_1 = PositionX[0] + (PositionX[1] - PositionX[0]) * t ;
      PositionOnTargetY_1 = PositionY[0] + (PositionY[1] - PositionY[0]) * t ;

      if(m_TargetAngle != 0){
        double a = (PositionZ[1]-PositionZ[0])/(PositionX[1]-PositionX[0]);
        double b = PositionZ[0] - a*PositionX[0];
        PositionOnTargetX = b/(tan(m_TargetAngle*Pi/180.) - a);

        double t_new = (l + L + PositionOnTargetX*tan(m_TargetAngle*Pi/180.)) / l;
        PositionOnTargetY = PositionY[0] + (PositionY[1] - PositionY[0]) * t_new ;
      }


      else{
        PositionOnTargetX = PositionOnTargetX_1;
        PositionOnTargetY = PositionOnTargetY_1;
      }
    }

    else{
      BeamDirection = TVector3 (1,0,0);

      PositionOnTargetX = -1000	;
      PositionOnTargetY = -1000	;
    }
  }

  else if(NumberOfCATSHit == 1){		
    BeamDirection = TVector3 (1,0,0);
    PositionOnTargetX = -1000	;
    PositionOnTargetY = -1000	;
  }
  return;

}

///////////////////////////////////////////////////////////////////////////
//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void TCATSPhysics::ReadConfiguration(string Path) 				
{
  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());
  string LineBuffer          		;
  string DataBuffer          		;

  double Ax , Bx , Cx , Dx , Ay , By , Cy , Dy , Az , Bz , Cz , Dz    	;
  TVector3 A , B , C , D                                          	;

  bool check_A = false 	;
  bool check_B = false  	;
  bool check_C = false 	;
  bool check_D = false 	;

  bool ReadingStatus = false ;


  while (!ConfigFile.eof()) 
  {
    getline(ConfigFile, LineBuffer);

    //If line is a Start Up CATS bloc, Reading toggle to true      
    if (LineBuffer.compare(0, 12, "CATSDetector") == 0) 
    {
      cout << "CATS Detector found: " << endl   ;  
      ReadingStatus = true 	       ;
    }

    //	Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    //	Reading Block
    while(ReadingStatus)
    {
      ConfigFile >> DataBuffer ;
      //	Comment Line 
      if(DataBuffer.compare(0, 1, "%") == 0) {
        ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
      }

      //	Finding another telescope (safety), toggle out
      else if (DataBuffer.compare(0, 12, "CATSDetector") == 0) {
        cout << "Warning: Another CATS is found before standard sequence of Token, Error may have occurred in CATS definition" << endl ;
        ReadingStatus = false ;
      }

      //	Corner Position method

      else if (DataBuffer.compare(0, 6, "X1_Y1=") == 0) {
        check_A = true;
        ConfigFile >> DataBuffer ;
        Ax = atof(DataBuffer.c_str()) ;
        Ax = Ax  ;
        ConfigFile >> DataBuffer ;
        Ay = atof(DataBuffer.c_str()) ;
        Ay = Ay  ;
        ConfigFile >> DataBuffer ;
        Az = atof(DataBuffer.c_str()) ;
        Az = Az  ;

        A = TVector3(Ax, Ay, Az);
        cout << " X1 Y1 corner position : (" << A.X() << ";" << A.Y() << ";" << A.Z() << ")" << endl;
      }

      else if (DataBuffer.compare(0, 7, "X28_Y1=") == 0) {
        check_B = true;
        ConfigFile >> DataBuffer ;
        Bx = atof(DataBuffer.c_str()) ;
        Bx = Bx  ;
        ConfigFile >> DataBuffer ;
        By = atof(DataBuffer.c_str()) ;
        By = By  ;
        ConfigFile >> DataBuffer ;
        Bz = atof(DataBuffer.c_str()) ;
        Bz = Bz  ;

        B = TVector3(Bx, By, Bz);
        cout << " X28 Y1 corner position : (" << B.X() << ";" << B.Y() << ";" << B.Z() << ")" << endl;
      }

      else if (DataBuffer.compare(0, 7, "X1_Y28=") == 0) {
        check_C = true;
        ConfigFile >> DataBuffer ;
        Cx = atof(DataBuffer.c_str()) ;
        Cx = Cx  ;
        ConfigFile >> DataBuffer ;
        Cy = atof(DataBuffer.c_str()) ;
        Cy = Cy  ;
        ConfigFile >> DataBuffer ;
        Cz = atof(DataBuffer.c_str()) ;
        Cz = Cz  ;

        C = TVector3(Cx, Cy, Cz);
        cout << " X1 Y28 corner position : (" << C.X() << ";" << C.Y() << ";" << C.Z() << ")" << endl;
      }

      else if (DataBuffer.compare(0, 8, "X28_Y28=") == 0) {
        check_D = true;
        ConfigFile >> DataBuffer ;
        Dx = atof(DataBuffer.c_str()) ;
        Dx = Dx  ;
        ConfigFile >> DataBuffer ;
        Dy = atof(DataBuffer.c_str()) ;
        Dy = Dy  ;
        ConfigFile >> DataBuffer ;
        Dz = atof(DataBuffer.c_str()) ;
        Dz = Dz  ;

        D = TVector3(Dx, Dy, Dz);
        cout << " X28 Y28 corner position : (" << D.X() << ";" << D.Y() << ";" << D.Z() << ")" << endl;

      }

      //	End Corner Position Method

      /////////////////////////////////////////////////
      //	If All necessary information there, toggle out
      if (check_A && check_B && check_C && check_D)  
      { 
        ReadingStatus = false; 

        ///Add The previously define telescope

        AddCATS(	A   ,
            B   ,
            C   ,
            D   );

        check_A = false;
        check_B = false;
        check_C = false;
        check_D = false;
      }
    }  

  }
  InitializeStandardParameter();
  ReadAnalysisConfig();
  m_method_CATS1X = StringToEnum(m_reconstruction_CATS1X);
  m_method_CATS1Y = StringToEnum(m_reconstruction_CATS1Y);
  m_method_CATS2X = StringToEnum(m_reconstruction_CATS2X);
  m_method_CATS2Y = StringToEnum(m_reconstruction_CATS2Y);
}

/////////////////////////////////////////////////////////////////////
//	Activated associated Branches and link it to the private member DetectorData address
//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void TCATSPhysics::InitializeRootInputRaw() 
{
  TChain* inputChain = RootInput::getInstance()->GetChain()	;
  inputChain->SetBranchStatus( "CATS" , true )			;
  inputChain->SetBranchStatus( "fCATS_*" , true )		;
  inputChain->SetBranchAddress( "CATS" , &m_EventData )           ;
}

/////////////////////////////////////////////////////////////////////
//   Activated associated Branches and link it to the private member DetectorPhysics address
//   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
void TCATSPhysics::InitializeRootInputPhysics() 
{
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "CATS" , true );
  inputChain->SetBranchStatus( "ff" , true );
  inputChain->SetBranchStatus( "DetNumberX" , true );
  inputChain->SetBranchStatus( "StripX" , true );
  inputChain->SetBranchStatus( "ChargeX" , true );
  inputChain->SetBranchStatus( "StripMaxX" , true );
  inputChain->SetBranchStatus( "DetNumberY" , true );
  inputChain->SetBranchStatus( "StripY" , true );
  inputChain->SetBranchStatus( "ChargeY" , true );
  inputChain->SetBranchStatus( "StripMaxY" , true );
  inputChain->SetBranchStatus( "DetNumberX_Position" , true );
  inputChain->SetBranchStatus( "DetNumberY_Position" , true );
  inputChain->SetBranchStatus( "DetNumberZ_Position" , true );
  inputChain->SetBranchStatus( "PositionX" , true );
  inputChain->SetBranchStatus( "PositionY" , true );
  inputChain->SetBranchStatus( "PositionZ" , true );
  inputChain->SetBranchStatus( "PositionOnTargetX" , true );
  inputChain->SetBranchStatus( "PositionOnTargetY" , true );
  inputChain->SetBranchStatus( "QsumX" , true );
  inputChain->SetBranchStatus( "QsumY" , true );
  inputChain->SetBranchAddress( "CATS" , &m_EventPhysics );

}

/////////////////////////////////////////////////////////////////////
//	Create associated branches and associated private member DetectorPhysics address
void TCATSPhysics::InitializeRootOutput() 		 		
{
  TTree* outputTree = RootOutput::getInstance()->GetTree()		;
  outputTree->Branch( "CATS" , "TCATSPhysics" , &m_EventPhysics )	;
}

/////////////////////////////////////////////////////////////////////
void TCATSPhysics::AddCATS(TVector3 C_X1_Y1, TVector3 C_X28_Y1, TVector3 C_X1_Y28, TVector3 C_X28_Y28)
{
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


  for( int i = 0 ; i < 28 ; i++ )
  {
    lineX.clear()	;
    lineY.clear()	;

    for( int j = 0 ; j < 28 ; j++ )
    {
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
void TCATSPhysics::Clear()
{  
  DetNumberX.clear()				; 
  StripX.clear()				;
  ChargeX.clear()                		;  
  StripMaxX.clear()				;
  DetNumberY.clear()				; 
  StripY.clear()				;
  ChargeY.clear()				; 
  StripMaxY.clear() 				;
  DetNumberX_Position.clear()                   ;
  DetNumberY_Position.clear()                   ;
  DetNumberZ_Position.clear()                   ;
  PositionX.clear() 				;
  PositionY.clear() 				;
  PositionZ.clear() 				;
  QsumX.clear();
  QsumY.clear();
  ReconstructionMethodX.clear()                 ;
  ReconstructionMethodY.clear()                 ;


  ff = 0;
  HitX = 0;
  HitY = 0;  
}


////////////////////////////////////////////////////////////////////////////
bool TCATSPhysics :: IsValidChannel(const string DetectorType, const int Detector , const int channel) 
{
  if(DetectorType == "X")
    return *(m_XChannelStatus[Detector-1].begin()+channel-1);

  else if(DetectorType == "Y")
    return *(m_YChannelStatus[Detector-1].begin()+channel-1);

  else return false;
}


///////////////////////////////////////////////////////////////////////////////////
void TCATSPhysics::InitializeStandardParameter()
{
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

  for(int i = 0 ; i < m_NumberOfCATS ; ++i)      {
    m_XChannelStatus[i] = ChannelStatus;
    m_YChannelStatus[i] = ChannelStatus;
    m_CATSXInversion[i] = InversionStatus;
    m_CATSYInversion[i] = InversionStatus;
  }

  return;
}   

///////////////////////////////////////////////////////////////////////////
void TCATSPhysics::ReadAnalysisConfig()
{
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
        cout << whatToDo << "  " << DataBuffer;
        if (DataBuffer.compare(0,6,"CATS1X") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_reconstruction_CATS1X = DataBuffer;
        }

        else if (DataBuffer.compare(0,6,"CATS1Y") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_reconstruction_CATS1Y = DataBuffer;
        }

        else if (DataBuffer.compare(0,6,"CATS2X") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_reconstruction_CATS2X = DataBuffer;
        }

        else if (DataBuffer.compare(0,6,"CATS2Y") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_reconstruction_CATS2Y = DataBuffer;
        }
      }

      else if (whatToDo == "CORRECTION_METHOD") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer;
        if (DataBuffer.compare(0,6,"CATS1X") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_correction_CATS1X = DataBuffer;
        }

        else if (DataBuffer.compare(0,6,"CATS1Y") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_correction_CATS1Y = DataBuffer;
        }

        else if (DataBuffer.compare(0,6,"CATS2X") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_correction_CATS2X = DataBuffer;
        }

        else if (DataBuffer.compare(0,6,"CATS2Y") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_correction_CATS2Y = DataBuffer;
        }
      }
      /*else if (whatToDo == "CORRECTION_METHOD") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        m_correction = DataBuffer;
        }*/

      else if (whatToDo == "CORRECTION_COEF") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer;
        if (DataBuffer.compare(0,6,"CATS1X") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_CorrectionCoef_CATS1X = atof(DataBuffer.c_str());
        }

        else if (DataBuffer.compare(0,6,"CATS1Y") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_CorrectionCoef_CATS1Y = atof(DataBuffer.c_str());
        }

        else if (DataBuffer.compare(0,6,"CATS2X") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_CorrectionCoef_CATS2X = atof(DataBuffer.c_str());
        }

        else if (DataBuffer.compare(0,6,"CATS2Y") == 0) {
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          m_CorrectionCoef_CATS2Y = atof(DataBuffer.c_str());
        }
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
map< vector<TString> , TH1*> TCATSPhysics::GetSpectra() {
  return m_Spectra->GetMapHisto();
}

/////////////////////////////////////////////////////////////////////
//	Add Parameter to the CalibrationManger
void TCATSPhysics::AddParameterToCalibrationManager()	
{
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for(int i = 0 ; i < m_NumberOfCATS ; i++)
  {

    for( int j = 0 ; j < 28 ; j++)
    {
      Cal->AddParameter("CATS", "D"+itoa(i+1)+"_X"+itoa(j+1)+"_Q","CATS_D"+itoa(i+1)+"_X"+itoa(j+1)+"_Q")	;
      Cal->AddParameter("CATS", "D"+itoa(i+1)+"_Y"+itoa(j+1)+"_Q","CATS_D"+itoa(i+1)+"_Y"+itoa(j+1)+"_Q")	;
      Cal->AddParameter("CATS", "D"+itoa(i+1)+"_X"+itoa(j+1),"CATS_D"+itoa(i+1)+"_X"+itoa(j+1))	;
      Cal->AddParameter("CATS", "D"+itoa(i+1)+"_Y"+itoa(j+1),"CATS_D"+itoa(i+1)+"_Y"+itoa(j+1))	;
    } 
  }

  return;
}	

////////////////////////////////////////////////////////////////
double TCATSPhysics::AnalyseX(int ff)
{
  double CalculatedStripX=0;  

  ReconstructionMethodX[ff] = ChooseReconstruction(ff,"X");

  if(ReconstructionMethodX[ff] == SECHS)CalculatedStripX = HyperbolicSequentMethodX();
  if(ReconstructionMethodX[ff] == GAUSS)CalculatedStripX = GaussianMethodX();
  if(ReconstructionMethodX[ff] == BAR3) CalculatedStripX = Barycentric3MethodX(); 
  if(ReconstructionMethodX[ff] == BAR4) CalculatedStripX = Barycentric4MethodX(); 
  if(ReconstructionMethodX[ff] == BAR5) CalculatedStripX = Barycentric5MethodX(); 

  return(CalculatedStripX);
}

////////////////////////////////////////////////////////////////
double TCATSPhysics::AnalyseY(int ff)
{
  double CalculatedStripY=0;

  ReconstructionMethodY[ff] = ChooseReconstruction(ff,"Y");

  if(ReconstructionMethodY[ff] == SECHS)CalculatedStripY = HyperbolicSequentMethodY();
  if(ReconstructionMethodY[ff] == GAUSS)CalculatedStripY = GaussianMethodY();
  if(ReconstructionMethodY[ff] == BAR3) CalculatedStripY = Barycentric3MethodY(); 
  if(ReconstructionMethodY[ff] == BAR4) CalculatedStripY = Barycentric4MethodY(); 
  if(ReconstructionMethodY[ff] == BAR5) CalculatedStripY = Barycentric5MethodY(); 


  return(CalculatedStripY);
}

////////////////////////////////////////////////////////////////////////
reconstruction TCATSPhysics::ChooseReconstruction(int ff, TString type)
{
  reconstruction method = NO;
  if(ff==0){
    if(type=="X"){method = m_method_CATS1X;}
    else if(type=="Y"){method = m_method_CATS1Y;}
  }

  if(ff==1){
    if(type=="X"){method = m_method_CATS2X;}
    else if(type=="Y"){method = m_method_CATS2Y;}
  }

  return(method);
} 

/////////////////////////////////////////////////////////////////////////
reconstruction TCATSPhysics::StringToEnum(string type)
{
  reconstruction method = NO;
  if(type=="GAUSS"){method = GAUSS;}
  if(type=="SECHS"){method = SECHS;}
  if(type=="BAR3"){method = BAR3;}
  if(type=="BAR4"){method = BAR4;}
  if(type=="BAR5"){method = BAR5;}

  return(method);
}

//////////////////////////////////////////////////////////////////////
double  TCATSPhysics::CalculatePositionX(double CalculatedStripX, correction method)
{
  double positionX=-1000;
  int IStripX = 0;

  if(ReconstructionMethodX[ff] == GAUSS){positionX = CalculatedStripX;}   // already in mm -> see gaussian method

  else{
    IStripX = (int) CalculatedStripX ;   

    // Decimal Part
    double DStripX = CalculatedStripX-IStripX ;

    if( DStripX > 0.5) {IStripX++; DStripX = DStripX-1 ;}     else {DStripX = DStripX;} 

    // Calculate Geometrical Position 
    if(IStripX > 0 &&  IStripX < 29){
      if(ff==0){    //CATS1
        // Warning : DStrip sign has to be determined carefully depending on the X direction

        positionX = (DStripX)*2.54 + StripPositionX[ff][IStripX-1][0] ;  //init avec le moins

        if(method == NOcor) positionX = positionX;
        else if(method == cor){
          if(ReconstructionMethodX[ff] == BAR3) positionX = CorrectedPositionX3(positionX, 0.60);
          if(ReconstructionMethodX[ff] == BAR4) positionX = CorrectedPositionX4(positionX, 0.77);
        }
      }

      else if(ff==1){    //CATS2
        // Warning : DStrip sign has to be determined carefully depending on the X direction

        positionX = -(DStripX)*2.54 + StripPositionX[ff][IStripX-1][0] ;   

        if(method == NOcor) positionX = positionX;
        else if(method == cor){
          if(ReconstructionMethodX[ff] == BAR3) positionX = CorrectedPositionX3(positionX, 0.30);            
          if(ReconstructionMethodX[ff] == BAR4) positionX = CorrectedPositionX4(positionX, 0.67);
        }
      }
      else  cout << "only 2CATS!! ff = " << ff << endl;
    }	

    else {positionX = -1000;} 
  }


  if(ff==0 && CalculatedStripX != -1000){
    if(m_correction_CATS1X == "Correction3Points"){
      positionX = Corrected3PointsX(positionX,m_CorrectionCoef_CATS1X);
    }
    if(m_correction_CATS1X == "Correction4Points"){
      positionX = Corrected4PointsX(positionX,m_CorrectionCoef_CATS1X);
    }
  }

  if(ff==1 && CalculatedStripX != -1000){
    if(m_correction_CATS2X == "Correction3Points"){
      positionX = Corrected3PointsX(positionX,m_CorrectionCoef_CATS2X);
    }
    if(m_correction_CATS2X == "Correction4Points"){
      positionX = Corrected4PointsX(positionX,m_CorrectionCoef_CATS2X);
    }
  }


  return positionX;
}

/////////////////////////////////////////////////////////////////////////
double  TCATSPhysics::CalculatePositionY(double CalculatedStripY, correction method)
{
  double positionY = -1000;

  if(ReconstructionMethodY[ff] == GAUSS){positionY = CalculatedStripY;}  // already in mm -> see gaussian method

  else{     
    //  Integer part
    int IStripY = (int) CalculatedStripY ; 

    // Decimal Part
    double DStripY = CalculatedStripY-IStripY ;

    if(DStripY > 0.5) {IStripY++; DStripY = DStripY-1;}
    else {DStripY = DStripY; } 

    // Calculate Geometrical Position 
    if(IStripY > 0  && IStripY < 29 ){
      positionY = (DStripY)*2.54 + StripPositionY[ff][0][IStripY-1] ;  // conversion en mm initiale
      if(ff ==0){
        if(method == NOcor) positionY = positionY;
        else if(method == cor) {
          if(ReconstructionMethodY[ff] == BAR3) positionY = CorrectedPositionY3(positionY, 0.6);
          if(ReconstructionMethodY[ff] == BAR4) positionY = CorrectedPositionY4(positionY, 0.75);
        }
      }

      else if(ff ==1){
        if(method == NOcor) positionY = positionY;
        else if(method == cor){
          if(ReconstructionMethodY[ff] == BAR3) positionY = CorrectedPositionY3(positionY, 0.45);
          if(ReconstructionMethodY[ff] == BAR4) positionY = CorrectedPositionY4(positionY, 0.7);
        }
      }

      else cout << "only 2CATS!! ff = " << ff << endl;
    }

    else  {positionY = -1000;} 
  }


  if(ff==0 && CalculatedStripY != -1000){
    if(m_correction_CATS1Y == "Correction3Points"){
      positionY = Corrected3PointsY(positionY,m_CorrectionCoef_CATS1Y);
    }
    if(m_correction_CATS1Y == "Correction4Points"){
      positionY = Corrected4PointsY(positionY,m_CorrectionCoef_CATS1Y);
    }
  }

  if(ff==1 && CalculatedStripY != -1000){
    if(m_correction_CATS2Y == "Correction3Points"){
      positionY = Corrected3PointsY(positionY,m_CorrectionCoef_CATS2Y);
    }
    if(m_correction_CATS2Y == "Correction4Points"){
      positionY = Corrected4PointsY(positionY,m_CorrectionCoef_CATS2Y);
    }
  }

  return positionY;

}

////////////////////////////////////////////////////////////////////
double TCATSPhysics:: GaussianMethodX()
{
  int StripMax = StripMaxX[ff];
  double gauss = -1000;
  double Q[3];
  double StripPos[3];

  for(int j = 0; j<3 ; j++)
  {
    Q[j] = 0;
    StripPos[j] = 0;
  }


  if(StripMaxX[ff]> 3 && StripMaxX[ff]< 26)
  {
    Q[0] = Buffer_X_Q[StripMax-1][ff] ;
    StripPos[0] = StripPositionX[ff][StripMax-1][0];

    if(Buffer_X_Q[StripMax-2][ff]!=-1){
      Q[1] = Buffer_X_Q[StripMax-2][ff];
      StripPos[1] = StripPositionX[ff][StripMax-2][0];
    }

    else {
      if(Buffer_X_Q[StripMax-3][ff]!=-1){
        Q[1] = Buffer_X_Q[StripMax-3][ff];
        StripPos[1] = StripPositionX[ff][StripMax-3][0];
      }
      else {
        if(Buffer_X_Q[StripMax-4][ff]!=-1){
          Q[1] = Buffer_X_Q[StripMax-4][ff];
          StripPos[1] = StripPositionX[ff][StripMax-4][0];
        }
      }	
    }

    if(Buffer_X_Q[StripMax][ff]!=-1){
      Q[2] = Buffer_X_Q[StripMax][ff];
      StripPos[2] = StripPositionX[ff][StripMax][0];
    }
    else {
      if(Buffer_X_Q[StripMax+1][ff]!=-1){
        Q[2] = Buffer_X_Q[StripMax+1][ff];
        StripPos[2] = StripPositionX[ff][StripMax+1][0];
      }
      else {
        if(Buffer_X_Q[StripMax+2][ff]!=-1){
          Q[2] = Buffer_X_Q[StripMax+2][ff];
          StripPos[2] = StripPositionX[ff][StripMax+2][0];
        }
      }
    }

    double Q0_Q1 = log(Q[0]/Q[1]);
    double Q0_Q2 = log(Q[0]/Q[2]);

    double num   = Q0_Q1 * (StripPos[2]*StripPos[2] - StripPos[0]*StripPos[0]) - Q0_Q2 * (StripPos[1]*StripPos[1] - StripPos[0]*StripPos[0])  ;
    double denom = Q0_Q1 * (StripPos[2] - StripPos[0]) - Q0_Q2 * (StripPos[1] - StripPos[0]) ;
    if(denom != 0){
      gauss = 0.5*num / denom;
    }
    else{gauss = -1000;}

  }

  else {
    gauss = -1000;
  }

  return gauss;

}

/////////////////////////////////////////////////////////////////////////
double TCATSPhysics::Corrected3PointsX(double Position, double c) 
{
  double Corrected_Position = 0;
  int StripMax_ = StripMaxX[ff] -1;
  double xmax = StripPositionX[ff][StripMax_][0] ;

  Corrected_Position = (Position - xmax) / c + xmax;

  return Corrected_Position;
}

/////////////////////////////////////////////////////////////////////////
double TCATSPhysics::Corrected4PointsX(double Position, double d) 
{
  double Corrected_Position = 0;
  double xmax = 0;
  int StripMax_ = StripMaxX[ff] -1;

  if(Buffer_X_Q[StripMax_+1][ff] > Buffer_X_Q[StripMax_-1][ff]) {
    if(ff==0)     xmax = StripPositionX[ff][StripMax_][0] - 1.27;
    else  xmax = StripPositionX[ff][StripMax_][0] + 1.27;
  }

  else{
    if(ff==0)     xmax = StripPositionX[ff][StripMax_-1][0] - 1.27;
    else  xmax = StripPositionX[ff][StripMax_-1][0]  + 1.27;
  }

  Corrected_Position = (Position - xmax) / d + xmax;

  return Corrected_Position;
}

////////////////////////////////////////////////////////////////////////////
double TCATSPhysics:: GaussianMethodY()
{
  double Q[3];
  double StripPos[3];
  double Q0_Q1, Q0_Q2;
  double num, denom;

  for(int j = 0; j<3 ; j++)
  {
    Q[j] = 0;
    StripPos[j] = 0;
  }

  int StripMax = StripMaxY[ff];

  double gauss = -1000;

  if(StripMaxY[ff] > 2 && StripMaxY[ff]<27)
  {
    Q[0] = Buffer_Y_Q[StripMax-1][ff] ;
    StripPos[0] = StripPositionY[ff][0][StripMax-1];

    if(Buffer_Y_Q[StripMax-2][ff]!=-1){
      Q[1] = Buffer_Y_Q[StripMax-2][ff];
      StripPos[1] = StripPositionY[ff][0][StripMax-2];
    }

    else {
      if(Buffer_Y_Q[StripMax-3][ff]!=-1){
        Q[1] = Buffer_Y_Q[StripMax-3][ff];
        StripPos[1] = StripPositionY[ff][0][StripMax-3] ;
      }
      else {
        if(Buffer_Y_Q[StripMax-4][ff]!=-1){
          Q[1] = Buffer_Y_Q[StripMax-4][ff];
          StripPos[1] = StripPositionY[ff][0][StripMax-4] ;
        }
      }
    }

    if(Buffer_Y_Q[StripMax][ff]!=-1){
      Q[2] = Buffer_Y_Q[StripMax][ff];
      StripPos[2] = StripPositionY[ff][0][StripMax];
    }

    else {
      if(Buffer_Y_Q[StripMax+1][ff]!=-1){
        Q[2] = Buffer_Y_Q[StripMax+1][ff];
        StripPos[2] = StripPositionY[ff][0][StripMax+1] ;
      }

      else {
        if(Buffer_Y_Q[StripMax+2][ff]!=-1){
          Q[2] = Buffer_Y_Q[StripMax+2][ff];
          StripPos[2] = StripPositionY[ff][0][StripMax+2] ;
        }
      }
    }	



    Q0_Q1 = log(Q[0]/Q[1]);
    Q0_Q2 = log(Q[0]/Q[2]);

    num   = Q0_Q1 * (StripPos[2]*StripPos[2] - StripPos[0]*StripPos[0]) - Q0_Q2 * (StripPos[1]*StripPos[1] - StripPos[0]*StripPos[0])  ;
    denom = Q0_Q1 * (StripPos[2] - StripPos[0]) - Q0_Q2 * (StripPos[1] - StripPos[0]) ;
    if(denom != 0){
      gauss = 0.5*num / denom;
    }

  }

  else {
    gauss = -1000;
  }

  return gauss;

}

/////////////////////////////////////////////////////////////////////////
double TCATSPhysics::Corrected3PointsY(double Position, double c) 
{
  double Corrected_Position = 0;
  int StripMax_ = StripMaxY[ff] -1;
  double ymax = StripPositionY[ff][0][StripMax_] ;

  Corrected_Position = (Position - ymax) / c + ymax;

  return Corrected_Position;
}

/////////////////////////////////////////////////////////////////////////
double TCATSPhysics::Corrected4PointsY(double Position, double d) 
{
  double Corrected_Position = 0;
  double ymax = 0;
  int StripMax_ = StripMaxY[ff] -1;

  if(Buffer_Y_Q[StripMax_+1][ff] > Buffer_Y_Q[StripMax_-1][ff]) {
    ymax = StripPositionY[ff][0][StripMax_] + 1.27 ;
  }

  else{
    ymax = StripPositionY[ff][0][StripMax_-1] + 1.27;
  }

  Corrected_Position = (Position - ymax) / d + ymax;

  return Corrected_Position;
}

///////////////////////////////////////////////////////////////
double TCATSPhysics:: Barycentric5MethodX() 
{
  double Barycenter = 0 ;

  if(StripMaxX[ff] > 2 && StripMaxX[ff] < 27)
  {
    int StripMax_ = StripMaxX[ff] -1 ; 
    double NumberOfPoint = 0 ;
    double ChargeTotal =0;


    for(int i = -2 ; i < 3 ; i++)
    {
      if(Buffer_X_Q[StripMax_+i][ff]!=-1)	          
      { 
        Barycenter += (StripMaxX[ff]+i)*Buffer_X_Q[StripMax_+i][ff] ;
        NumberOfPoint++;
        ChargeTotal+=Buffer_X_Q[StripMax_+i][ff];
      }
    }

    if(ChargeTotal>0) Barycenter = Barycenter / ChargeTotal ;
    else {Barycenter = -1000 ; } 

  }
  else
  {
    Barycenter = -1000 ;
  }

  return Barycenter ;
}

///////////////////////////////////////////////////////////////
double TCATSPhysics:: Barycentric5MethodY() 
{
  double Barycenter = 0 ;

  if(StripMaxY[ff] > 2 && StripMaxY[ff] < 27)
  {
    int StripMax_ = StripMaxY[ff] -1 ; 
    double NumberOfPoint = 0 ;
    double ChargeTotal =0;


    for(int i = -2 ; i < 3 ; i++)
    {
      if(Buffer_Y_Q[StripMax_+i][ff]!=-1)	          
      { 
        Barycenter += (StripMaxY[ff]+i)*Buffer_Y_Q[StripMax_+i][ff] ;
        NumberOfPoint++;
        ChargeTotal+=Buffer_Y_Q[StripMax_+i][ff];
      }
    }

    if(ChargeTotal>0) Barycenter = Barycenter / ChargeTotal ;
    else {Barycenter = -1000 ; } // cout << "Yo" << endl ;}

}
else
{
  Barycenter = -1000 ;
}

return Barycenter ;
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
double TCATSPhysics:: Barycentric3MethodX() 
{
  double Barycenter = 0 ;

  if(StripMaxX[ff] > 2 && StripMaxX[ff] < 27)
  {
    int StripMax_ = StripMaxX[ff] -1; 
    double NumberOfPoint = 0 ;
    double ChargeTotal =0;

    for(int i = -1 ; i < 2 ; i++)
    {
      if(Buffer_X_Q[StripMax_+i][ff]!=-1)	// Charge initialized to -1
      { 
        Barycenter += (StripMaxX[ff]+i)*Buffer_X_Q[StripMax_+i][ff] ;
        NumberOfPoint++;
        ChargeTotal+=Buffer_X_Q[StripMax_+i][ff];
      }
    }

    if(ChargeTotal>0) Barycenter = Barycenter / ChargeTotal ;
    else {Barycenter = -1000 ;}  // cout << "Yo" << endl ;}
}


else
{
  Barycenter = -1000 ;
}

return Barycenter ;
}

///////////////////////////////////////////////////////////////
double TCATSPhysics:: Barycentric3MethodY() 
{
  double Barycenter = 0 ;

  if(StripMaxY[ff] > 2 && StripMaxY[ff] < 27)
  {
    int StripMax_ = StripMaxY[ff] -1 ; // Use because numerotation of array start at 0 ;
    double NumberOfPoint = 0 ;
    double ChargeTotal =0;

    for(int i = -1 ; i < 2 ; i++)
    {
      if(Buffer_Y_Q[StripMax_+i][ff]!=-1)	// Charge initialized to -1
      { 
        Barycenter += (StripMaxY[ff]+i)*Buffer_Y_Q[StripMax_+i][ff] ;
        NumberOfPoint++;
        ChargeTotal+=Buffer_Y_Q[StripMax_+i][ff];
      }
    }

    if(ChargeTotal>0) Barycenter = Barycenter / ChargeTotal ;
    else {Barycenter = -1000 ;}  // cout << "Yo" << endl ;}
}

else
{
  Barycenter = -1000 ;
  //        cout << "Strip max " << StripMax << endl;
}

return Barycenter ;
}

///////////////////////////////////////////////////////////////
double TCATSPhysics:: Barycentric4MethodX()
{
  double Barycenter = 0 ;

  if(StripMaxX[ff] > 2 && StripMaxX[ff] < 27)     {

    int StripMax_ = StripMaxX[ff] -1 ; // Use because numerotation of array start at 0 ;
    double NumberOfPoint = 0 ;
    double ChargeTotal =0;

    if(Buffer_X_Q[StripMax_+1][ff] > Buffer_X_Q[StripMax_-1][ff]) {

      //	cout << "Barycentre droit" << endl;
      for(int i = -1 ; i < 3 ; i++)  
      {
        if(Buffer_X_Q[StripMax_+i][ff]!=-1)   {                            // Charge initialized to -1
          Barycenter += (StripMaxX[ff]+i)*Buffer_X_Q[StripMax_+i][ff] ;
          NumberOfPoint++;
          ChargeTotal+=Buffer_X_Q[StripMax_+i][ff];
        }
      }
    }

    else {
      //	cout << "Barycentre gauche" << endl;	
      for(int i = -2 ; i < 2 ; i++)
      {
        if(Buffer_X_Q[StripMax_+i][ff]!=-1)	{                                 // Charge initialized to -1
          Barycenter += (StripMaxX[ff]+i)*Buffer_X_Q[StripMax_+i][ff] ;
          NumberOfPoint++;
          ChargeTotal+=Buffer_X_Q[StripMax_+i][ff];
        }
      }
    }

    if(ChargeTotal>0) {
      Barycenter = Barycenter / ChargeTotal ;
    }

  }

  else
  {
    Barycenter = -1000 ;
    //        cout << "Strip max " << StripMax << endl;
  }

  return Barycenter ;
}

///////////////////////////////////////////////////////////////
double TCATSPhysics:: Barycentric4MethodY() 
{
  double Barycenter = 0 ;

  if(StripMaxY[ff] > 2 && StripMaxY[ff] < 27)     {

    int StripMax_ = StripMaxY[ff] -1 ; // Use because numerotation of array start at 0 ;
    double NumberOfPoint = 0 ;
    double ChargeTotal =0;

    if(Buffer_Y_Q[StripMax_+1][ff] > Buffer_Y_Q[StripMax_-1][ff]) {

      //	cout << "Barycentre droit" << endl;
      for(int i = -1 ; i < 3 ; i++)  
      {
        if(Buffer_Y_Q[StripMax_+i][ff]!=-1)   {                            // Charge initialized to -1
          Barycenter += (StripMaxY[ff]+i)*Buffer_Y_Q[StripMax_+i][ff] ;
          NumberOfPoint++;
          ChargeTotal+=Buffer_Y_Q[StripMax_+i][ff];
        }
      }
    }

    else {
      //	cout << "Barycentre gauche" << endl;	
      for(int i = -2 ; i < 2 ; i++)
      {
        if(Buffer_Y_Q[StripMax_+i][ff]!=-1)	{                                 // Charge initialized to -1
          Barycenter += (StripMaxY[ff]+i)*Buffer_Y_Q[StripMax_+i][ff] ;
          NumberOfPoint++;
          ChargeTotal+=Buffer_Y_Q[StripMax_+i][ff];
        }
      }
    }

    if(ChargeTotal>0) {
      Barycenter = Barycenter / ChargeTotal ;
    }

  }

  else
  {
    Barycenter = -1000 ;
    //        cout << "Strip max " << StripMax << endl;
  }

  return Barycenter ;
}

/////////////////////////////////////////////////////////////////////
double TCATSPhysics:: HyperbolicSequentMethodX()
{
  double sechs = -1000 ;

  if(StripMaxX[ff] > 2 && StripMaxX[ff]<27)
  {		
    double vs1 = sqrt( Buffer_X_Q[StripMaxX[ff]-1][ff]/Buffer_X_Q[StripMaxX[ff]-1+1][ff] ) 	;
    double vs2 = sqrt( Buffer_X_Q[StripMaxX[ff]-1][ff]/Buffer_X_Q[StripMaxX[ff]-1-1][ff] ) 	;
    double vs3 = 0.5*( vs1 + vs2 ) 					;
    double vs4 = log( vs3 + sqrt(vs3*vs3-1.0) ) 			;
    double vs5 = (vs1 - vs2)/(2.0*sinh(vs4)) 				;	

    if(vs5<0) vs5=-vs5 ;

    double vs6 = 0.5*log( (1.0+vs5)/(1.0-vs5) ) ;

    if ( Buffer_X_Q[StripMaxX[ff]-1+1][ff]>Buffer_X_Q[StripMaxX[ff]-1-1][ff] )
    { sechs = StripMaxX[ff] + vs6/vs4 ;}

    else
    { sechs = StripMaxX[ff] - vs6/vs4 ;}	

  }

  else { 
    sechs = -1000; 
  }

  return sechs ;
}

//////////////////////////////////////////////////////////////////
double TCATSPhysics:: HyperbolicSequentMethodY()
{
  double sechs = -1000 ;

  if(StripMaxY[ff] > 2 && StripMaxY[ff]<27)
  {
    double vs1 = sqrt( Buffer_Y_Q[StripMaxY[ff]-1][ff]/Buffer_Y_Q[StripMaxY[ff]-1+1][ff] ) 	;
    double vs2 = sqrt( Buffer_Y_Q[StripMaxY[ff]-1][ff]/Buffer_Y_Q[StripMaxY[ff]-1-1][ff] ) 	;
    double vs3 = 0.5*( vs1 + vs2 ) 					;
    double vs4 = log( vs3 + sqrt(vs3*vs3-1.0) ) 			;
    double vs5 = (vs1 - vs2)/(2.0*sinh(vs4)) 				;	

    if(vs5<0) vs5=-vs5 ;

    double vs6 = 0.5*log( (1.0+vs5)/(1.0-vs5) ) ;

    if ( Buffer_Y_Q[StripMaxY[ff]-1+1][ff]>Buffer_Y_Q[StripMaxY[ff]-1-1][ff] )
    { sechs = StripMaxY[ff] + vs6/vs4 ;}

    else
    { sechs = StripMaxY[ff] - vs6/vs4 ;}	

  }

  else { 
    sechs = -1000; 
  }

  return sechs ;
}



///////////////////////////////////////////////////////////////
double TCATSPhysics::CorrectedPositionX3(double Position, double a) 
{
  double Corrected_Position = 0;
  int StripMax_ = StripMaxX[ff] -1;
  double xmax = StripPositionX[ff][StripMax_][0] ;

  Corrected_Position = (Position - xmax) / a + xmax;

  return Corrected_Position;
}

///////////////////////////////////////////////////////////////
double TCATSPhysics::CorrectedPositionY3(double Position, double a) 
{
  double Corrected_Position = 0;
  int StripMax_ = StripMaxY[ff] -1;
  double xmax = StripPositionY[ff][0][StripMax_];

  Corrected_Position = (Position - xmax) / a + xmax;

  return Corrected_Position;
}

///////////////////////////////////////////////////////////////
double TCATSPhysics::CorrectedPositionX4(double Position, double b) 
{
  double Corrected_Position = 0;
  double xmax = 0;
  int StripMax_ = StripMaxX[ff] -1;

  if(Buffer_X_Q[StripMax_+1][ff] > Buffer_X_Q[StripMax_-1][ff]) {
    if(ff ==0)     xmax = StripPositionX[ff][StripMax_][0] - 1.27;
    else  xmax = StripPositionX[ff][StripMax_][0] + 1.27;
  }

  else{
    if(ff ==0)     xmax = StripPositionX[ff][StripMax_-1][0] - 1.27;
    else  xmax = StripPositionX[ff][StripMax_-1][0]  + 1.27;
  }

  Corrected_Position = (Position - xmax) / b + xmax;

  return Corrected_Position;
}

///////////////////////////////////////////////////////////////
double TCATSPhysics::CorrectedPositionY4(double Position, double b) 
{
  double Corrected_Position = 0;
  double xmax = 0;
  int StripMax_ = StripMaxY[ff] -1;

  if(Buffer_Y_Q[StripMax_+1][ff] > Buffer_Y_Q[StripMax_-1][ff]) {
    xmax = StripPositionY[ff][0][StripMax_] + 1.27 ;
  }

  else{
    xmax = StripPositionY[ff][0][StripMax_-1] + 1.27;
  }

  Corrected_Position = (Position - xmax) / b + xmax;

  return Corrected_Position;
}


///////////////////////////////////////////////////////////////
TVector3 TCATSPhysics::GetBeamDirection()
{
  TVector3 Position = TVector3 (PositionX[1]-PositionX[0] ,
      PositionY[1]-PositionY[0] ,
      PositionZ[1]-PositionZ[0] );
  Position.Unit();
  return(Position) ;	
}

///////////////////////////////////////////////////////////////
TVector3 TCATSPhysics::GetPositionOnTarget()
{
  double Pi = 3.14159265;
  TVector3 Position = TVector3 (GetPositionOnTargetX() 	,
      GetPositionOnTargetY() 	,
      GetPositionOnTargetX()*tan(m_TargetAngle*Pi/180)); 
  Position.Unit();
  return(Position) ;	
}

////////////////////////////////////////////////////////////////////////
namespace LOCAL_CATS
{
  //	transform an integer to a string
  string itoa(int value)
  {
    std::ostringstream o;

    if (!(o << value))
      return ""	;

    return o.str();
  }

  double fCATS_X_Q(const TCATSData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration( "CATS/D" + itoa( m_EventData->GetCATSDetX(i) ) + "_X" + itoa( m_EventData->GetCATSStripX(i) ) + "_Q",   
        m_EventData->GetCATSChargeX(i) + gRandom->Rndm() - fCATS_Ped_X(m_EventData, i) );
  }

  double fCATS_Y_Q(const TCATSData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration( "CATS/D" + itoa( m_EventData->GetCATSDetY(i) ) + "_Y" + itoa( m_EventData->GetCATSStripY(i) ) + "_Q",   
        m_EventData->GetCATSChargeY(i) + gRandom->Rndm() - fCATS_Ped_Y(m_EventData, i) );
  }

  bool fCATS_Threshold_X(const TCATSData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyThreshold( "CATS/D" + itoa( m_EventData->GetCATSDetX(i) ) + "_X" + itoa( m_EventData->GetCATSStripX(i) ),
        m_EventData->GetCATSChargeX(i));
  }

  bool fCATS_Threshold_Y(const TCATSData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyThreshold( "CATS/D" + itoa( m_EventData->GetCATSDetY(i) ) + "_Y" + itoa( m_EventData->GetCATSStripY(i) ),
        m_EventData->GetCATSChargeY(i));
  }

  double fCATS_Ped_X(const TCATSData* m_EventData, const int i)
  {
    return CalibrationManager::getInstance()->GetPedestal( "CATS/D" + itoa( m_EventData->GetCATSDetX(i) ) + "_X" + itoa( m_EventData->GetCATSStripX(i) ) );
  }

  double fCATS_Ped_Y(const TCATSData* m_EventData, const int i)
  {
    return CalibrationManager::getInstance()->GetPedestal( "CATS/D" + itoa( m_EventData->GetCATSDetY(i) ) + "_Y" + itoa( m_EventData->GetCATSStripY(i) ) );
  }


}










