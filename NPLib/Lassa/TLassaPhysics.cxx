/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu *
 *                                                                           *
 * Creation Date  : June 2015                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Hira  Physics                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TLassaPhysics.h"
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPCalibrationManager.h"
#include "TAsciiFile.h"

//   STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
using namespace std;

//   ROOT
#include "TChain.h"

//   tranform an integer to a string
string itoa(int value)
{
   char buffer [33];
   sprintf(buffer,"%d",value);
   return buffer;
}

ClassImp(TLassaPhysics)
///////////////////////////////////////////////////////////////////////////
TLassaPhysics::TLassaPhysics()
   {
       //EventMultiplicity    = 0 ;
       m_NumberOfTelescope  = 0 ;
       m_EventData          = new TLassaData ;
       m_PreTreatedData     = new TLassaData;
       m_EventPhysics       = this ;
   }
   
///////////////////////////////////////////////////////////////////////////
TLassaPhysics::~TLassaPhysics()
   {}
   
///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::Clear(){
    TelescopeNumber.clear();
    
/*    // Thin Si
    ThinSi_E.clear();
    ThinSi_T.clear();
    ThinSi_StripNumber.clear();*/
    
    //Thick Si
    ThickSi_E.clear();
    ThickSi_T.clear();
    ThickSi_EX.clear();
    ThickSi_EY.clear();
    ThickSi_StripNumberX.clear();
    ThickSi_StripNumberY.clear();
    
    // CsI
    CsI_E.clear();
    CsI_CrystalNumber.clear();
    
    
}

///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::ReadConfiguration(string Path){
    ifstream ConfigFile           ;
    ConfigFile.open(Path.c_str()) ;
    string LineBuffer          ;
    string DataBuffer          ;
    
    // A:X1_Y1     --> X:1    Y:1
    // B:X128_Y1   --> X:128  Y:1
    // C:X1_Y128   --> X:1    Y:128
    // D:X128_Y128    --> X:128  Y:128
    
    double Ax , Bx , Cx , Dx , Ay , By , Cy , Dy , Az , Bz , Cz , Dz          ;
    TVector3 A , B , C , D                                                 ;

    
    bool ReadingStatus = false ;
    
    bool check_A = false ;
    bool check_C = false ;
    bool check_B = false ;
    bool check_D = false ;
    
    /*bool check_Theta = false ;
     bool check_Phi   = false ;
     bool check_R     = false ;*/
    
    
    
    while (!ConfigFile.eof()) {
        getline(ConfigFile, LineBuffer);
        if (LineBuffer.compare(0, 14, "LassaTelescope") == 0) {
            cout << "///" << endl           ;
            cout << "Lassa element found: " << endl   ;
            ReadingStatus = true ;
        }
        
        while(ReadingStatus){
            
            ConfigFile >> DataBuffer;
            //   Comment Line
            if (DataBuffer.compare(0, 1, "%") == 0) {/*do nothing */;}
            
            // Position method
            else if (DataBuffer.compare(0, 2, "A=") == 0) {
                check_A = true;
                ConfigFile >> DataBuffer ;
                Ax = atof(DataBuffer.c_str()) ;
                Ax = Ax ;
                ConfigFile >> DataBuffer ;
                Ay = atof(DataBuffer.c_str()) ;
                Ay = Ay;
                ConfigFile >> DataBuffer ;
                Az = atof(DataBuffer.c_str()) ;
                Az = Az;
                
                A = TVector3(Ax, Ay, Az);
                cout << "A corner position : " << A.X() << ";" << A.Y() << ";" << A.Z() << endl;
            }
            
            else if (DataBuffer.compare(0, 2, "B=") == 0) {
                check_B = true;
                ConfigFile >> DataBuffer ;
                Bx = atof(DataBuffer.c_str()) ;
                Bx = Bx;
                ConfigFile >> DataBuffer ;
                By = atof(DataBuffer.c_str()) ;
                By = By;
                ConfigFile >> DataBuffer ;
                Bz = atof(DataBuffer.c_str()) ;
                Bz = Bz;
                
                B = TVector3(Bx, By, Bz);
                cout << "B corner position : " << B.X() << ";" << B.Y() << ";" << B.Z() << endl;
            }
            
            else if (DataBuffer.compare(0, 2, "C=") == 0) {
                check_C = true;
                ConfigFile >> DataBuffer ;
                Cx = atof(DataBuffer.c_str()) ;
                Cx = Cx;
                ConfigFile >> DataBuffer ;
                Cy = atof(DataBuffer.c_str()) ;
                Cy = Cy;
                ConfigFile >> DataBuffer ;
                Cz = atof(DataBuffer.c_str()) ;
                Cz = Cz;
                
                C = TVector3(Cx, Cy, Cz);
                cout << "C corner position : " << C.X() << ";" << C.Y() << ";" << C.Z() << endl;
            }
            
            else if (DataBuffer.compare(0, 2, "D=") == 0) {
                check_D = true;
                ConfigFile >> DataBuffer ;
                Dx = atof(DataBuffer.c_str()) ;
                Dx = Dx;
                ConfigFile >> DataBuffer ;
                Dy = atof(DataBuffer.c_str()) ;
                Dy = Dy;
                ConfigFile >> DataBuffer ;
                Dz = atof(DataBuffer.c_str()) ;
                Dz = Dz;
                
                D = TVector3(Dx, Dy, Dz);
                cout << "D corner position : " << D.X() << ";" << D.Y() << ";" << D.Z() << endl;
            }
            
            
            // Angle method
            /*   else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
             check_Theta = true;
             ConfigFile >> DataBuffer ;
             Theta = atof(DataBuffer.c_str()) ;
             Theta = Theta * deg;
             cout << "Theta:  " << Theta / deg << endl;
             }
             
             else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
             check_Phi = true;
             ConfigFile >> DataBuffer ;
             Phi = atof(DataBuffer.c_str()) ;
             Phi = Phi * deg;
             cout << "Phi:  " << Phi / deg << endl;
             }
             
             else if (DataBuffer.compare(0, 2, "R=") == 0) {
             check_R = true;
             ConfigFile >> DataBuffer ;
             R = atof(DataBuffer.c_str()) ;
             R = R * mm;
             cout << "R:  " << R / mm << endl;
             }
             
             else if (DataBuffer.compare(0, 5, "BETA=") == 0) {
             ConfigFile >> DataBuffer ;
             beta_u = atof(DataBuffer.c_str()) ;
             beta_u = beta_u * deg   ;
             ConfigFile >> DataBuffer ;
             beta_v = atof(DataBuffer.c_str()) ;
             beta_v = beta_v * deg   ;
             ConfigFile >> DataBuffer ;
             beta_w = atof(DataBuffer.c_str()) ;
             beta_w = beta_w * deg   ;
             cout << "Beta:  " << beta_u / deg << " " << beta_v / deg << " " << beta_w / deg << endl  ;
             }*/
            
            
            else cout << "WARNING: Wrong Token, GaspardTrackerRectangle: Rectangle Element not added" << endl;
            
            //Add The previously define telescope
            //With position method
            if (check_A && check_B && check_C && check_D) {
                
                ReadingStatus = false ;
                check_A = false ;
                check_C = false ;
                check_B = false ;
                check_D = false ;
                
                AddTelescope(A                ,
                             B                ,
                             C                ,
                             D);
            }
            
            //with angle method
            /*if ((check_Theta && check_Phi && check_R && check_FirstStage && check_SecondStage && check_ThirdStage && checkVis) && !(check_A && check_B && check_C && check_D)) {
             ReadingStatus = false ;
             check_Theta = false ;
             check_Phi   = false ;
             check_R     = false ;
             check_FirstStage = false ;
             check_SecondStage = false ;
             check_ThirdStage = false ;
             checkVis = false ;
             
             AddModule(R                ,
             Theta            ,
             Phi              ,
             beta_u           ,
             beta_v           ,
             beta_w           ,
             FIRSTSTAGE  == 1 ,
             SECONDSTAGE == 1 ,
             THIRDSTAGE  == 1);
             }*/
            
            
        }
    }
  ReadAnalysisConfig();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TLassaPhysics::AddTelescope(TVector3 Pos1, TVector3 Pos2, TVector3 Pos3, TVector3 Pos4){
    m_NumberOfTelescope++;
    TVector3 Pos= 0.25*(Pos1+Pos2+Pos3+Pos4);//centroid of the telescope
    TVector3 u = Pos4-Pos2;//Horizontal Vector->EF
    TVector3 w = Pos.Unit();//Telescope normal
    TVector3 v = w.Cross(u);//Vertical Vector->EB
    u = u.Unit(); v = v.Unit(); w = w.Unit();
    
    //   Geometry Parameter
    double Face = 50.; //64.; //mm
    double NumberOfStrip = 16;
    double StripPitch = Face/NumberOfStrip ; //mm
    double offset = 0.5874*25.4; //mm
    //   Buffer object to fill Position Array
    vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;
    
    vector< vector< double > >   OneTelescopeStripPositionX;
    vector< vector< double > >   OneTelescopeStripPositionY;
    vector< vector< double > >   OneTelescopeStripPositionZ;

    //   Position Vector of Strip Center
    TVector3 StripCenter = TVector3(0,0,0);
    //   Position Vector of X=1 Y=1 Strip
    TVector3 Strip_1_1;
    
    //   Moving StripCenter to 1.1 corner:
    Strip_1_1 = Pos + offset*w - (NumberOfStrip/2-0.5)*StripPitch*(u+v);
    
    for( int i = 0 ; i < 32 ; ++i ){
        lineX.clear();
        lineY.clear();
        lineZ.clear();
        
        for( int j = 0 ; j < 32 ; ++j ){
            StripCenter  = Strip_1_1 + StripPitch*( i*u + j*v  );
            lineX.push_back( StripCenter.X() );
            lineY.push_back( StripCenter.Y() );
            lineZ.push_back( StripCenter.Z() );
        }
        
        OneTelescopeStripPositionX.push_back(lineX);
        OneTelescopeStripPositionY.push_back(lineY);
        OneTelescopeStripPositionZ.push_back(lineZ);
        
    }
    
    m_StripPositionX.push_back( OneTelescopeStripPositionX ) ;
    m_StripPositionY.push_back( OneTelescopeStripPositionY ) ;
    m_StripPositionZ.push_back( OneTelescopeStripPositionZ ) ;

}


///////////////////////////////////////////////////////////////////////////
TVector3 TLassaPhysics::GetPositionOfInteraction(const int i) const{
    TVector3 Position = TVector3 (   GetStripPositionX( TelescopeNumber[i] , ThickSi_StripNumberX[i] , ThickSi_StripNumberY[i] ),
                                  GetStripPositionY( TelescopeNumber[i] , ThickSi_StripNumberX[i] , ThickSi_StripNumberY[i] ),
                                  GetStripPositionZ( TelescopeNumber[i] , ThickSi_StripNumberX[i] , ThickSi_StripNumberY[i] )) ;
    
    return(Position) ;
    
}

///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::AddParameterToCalibrationManager()
   {
      CalibrationManager* Cal = CalibrationManager::getInstance();
      
      for(int i = 0 ; i < m_NumberOfTelescope ; i++)
         {
            for( int j = 0 ; j < 16 ; j++)
               {
                  Cal->AddParameter("Lassa", "Detector"+itoa(i+1)+"_E","Lassa_Detector"+itoa(i+1)+"_E")   ;
                  Cal->AddParameter("Lassa", "Detector"+itoa(i+1)+"_T","Lassa_Detector"+itoa(i+1)+"_T")   ;   
               }
      
         }
   }
   
///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::InitializeRootInputRaw() 
   {
      TChain* inputChain = RootInput::getInstance()->GetChain()     ;
      inputChain->SetBranchStatus ( "Lassa"       , true )        ;
      inputChain->SetBranchStatus ( "fLassa_*"    , true )        ;
      inputChain->SetBranchAddress( "Lassa"       , &m_EventData )  ;
   }
///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::InitializeRootInputPhysics()
   {
      TChain* inputChain = RootInput::getInstance()->GetChain();
      inputChain->SetBranchStatus ( "Lassa", true );
      inputChain->SetBranchStatus ( "DetectorNumber", true );
      inputChain->SetBranchStatus ( "Energy", true );
      inputChain->SetBranchStatus ( "Time", true );
      inputChain->SetBranchAddress( "Lassa", &m_EventPhysics );
   }
///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::InitializeRootOutput()
   {
      TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
      outputTree->Branch( "Lassa" , "TLassaPhysics" , &m_EventPhysics ) ;
   }

///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::BuildPhysicalEvent(){
    BuildSimplePhysicalEvent()   ;
}

///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::BuildSimplePhysicalEvent(){
    PreTreat();
    //m_ThinSi_EMult      = m_PreTreatedData->GetLassaThinSiEMult();
    m_ThickSi_EXMult    = m_PreTreatedData->GetLassaStripXEMult();
    m_ThickSi_EYMult    = m_PreTreatedData->GetLassaStripYEMult();
    m_CsI_EMult         = m_PreTreatedData->GetLassaCsIEMult();
   
//    cout << "Si_X mult: " << m_ThickSi_EXMult << endl;
//    cout << "Si_Y mult: " << m_ThickSi_EYMult << endl;
//    cout << "CsI mult: " << m_CsI_EMult << endl;

 
    /*for(unsigned int i = 0 ; i < m_ThinSi_EMult ; ++i){
        TelescopeNumber.push_back(m_PreTreatedData->GetLassaThinSiStripEDetectorNbr(i));
        ThinSi_StripNumber.push_back(m_PreTreatedData->GetLassaThinSiStripEStripNbr(i));
        ThinSi_E.push_back(m_PreTreatedData->GetLassaThinSiStripEEnergy(i));
        ThinSi_T.push_back(m_PreTreatedData->GetLassaThinSiStripTTime(i));
    }*/

    if(CheckEvent(m_ThickSi_EXMult,m_ThickSi_EYMult) == 1){
    
      for(unsigned int i = 0 ; i < m_ThickSi_EXMult ; ++i){
	  TelescopeNumber.push_back(m_PreTreatedData->GetLassaStripXEDetectorNbr(i));
          ThickSi_StripNumberX.push_back(m_PreTreatedData->GetLassaStripXEStripNbr(i));
          ThickSi_EX.push_back(m_PreTreatedData->GetLassaStripXEEnergy(i));
          ThickSi_T.push_back(m_EventData->GetLassaStripXTTime(i));
          ThickSi_E.push_back(m_PreTreatedData->GetLassaStripXEEnergy(i));
      } 

      for(unsigned int i = 0 ; i < m_ThickSi_EXMult ; ++i){
          ThickSi_StripNumberY.push_back(m_PreTreatedData->GetLassaStripYEStripNbr(i));
          ThickSi_EY.push_back(m_PreTreatedData->GetLassaStripYEEnergy(i));
      }
    
      for(unsigned int i = 0 ; i < m_CsI_EMult ; ++i){
          CsI_CrystalNumber.push_back(m_PreTreatedData->GetLassaCsIECristalNbr(i));
          CsI_E.push_back(m_PreTreatedData->GetLassaCsIEEnergy(i));
      }
    }
    return;
}

///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::PreTreat(){
    ClearPreTreatedData();
    m_ThickSi_EXMult    = m_EventData->GetLassaStripXEMult();
    m_ThickSi_EYMult    = m_EventData->GetLassaStripYEMult();
    m_CsI_EMult         = m_EventData->GetLassaCsIEMult();

//    cout << "Si XMult: " << m_ThickSi_EXMult << endl;
//    cout << "Si YMult: " << m_ThickSi_EYMult << endl;
//    cout << "CsI Mult: " << m_CsI_EMult << endl;
    
    // Thin Si
    /*for(unsigned int i = 0 ; i < m_ThinSi_EMult ; ++i){
        m_PreTreatedData->SetLassaThinSiStripEDetectorNbr( m_EventData->GetLassaThinSiStripEDetectorNbr(i) );
        m_PreTreatedData->SetLassaThinSiStripEStripNbr( m_EventData->GetLassaThinSiStripEStripNbr(i) );
        m_PreTreatedData->SetLassaThinSiStripEEnergy( m_EventData->GetLassaThinSiStripEEnergy(i) );
        m_PreTreatedData->SetLassaThinSiStripTTime( m_EventData->GetLassaThinSiStripTTime(i));
    }*/

    bool IsValid_XStrip, IsValid_YStrip;

    IsValid_XStrip = true;
    IsValid_YStrip = true;


    // Thick Si
    // X
    for(unsigned int i = 0 ; i < m_ThickSi_EXMult ; ++i){
      if(IsValidChannel("X",m_EventData->GetLassaStripXEDetectorNbr(i),m_EventData->GetLassaStripXEStripNbr(i))){
        m_PreTreatedData->SetLassaThickSiStripXEDetectorNbr( m_EventData->GetLassaStripXEDetectorNbr(i) );
        m_PreTreatedData->SetLassaThickSiStripXEStripNbr( m_EventData->GetLassaStripXEStripNbr(i) );
        m_PreTreatedData->SetLassaThickSiStripXEEnergy( m_EventData->GetLassaStripXEEnergy(i) );
        m_PreTreatedData->SetLassaThickSiStripXTTime( m_EventData->GetLassaStripXTTime(i) );
        }
        else if (i == 0) IsValid_XStrip = false;
  }
    // Y
    for(unsigned int i = 0 ; i < m_ThickSi_EYMult ; ++i){
      if(IsValidChannel("Y",m_EventData->GetLassaStripYEDetectorNbr(i),m_EventData->GetLassaStripYEStripNbr(i))){ 
        m_PreTreatedData->SetLassaThickSiStripYEDetectorNbr( m_EventData->GetLassaStripYEDetectorNbr(i) );
        m_PreTreatedData->SetLassaThickSiStripYEStripNbr( m_EventData->GetLassaStripYEStripNbr(i) );
        m_PreTreatedData->SetLassaThickSiStripYEEnergy( m_EventData->GetLassaStripYEEnergy(i) );
        m_PreTreatedData->SetLassaThickSiStripYTTime( m_EventData->GetLassaStripYTTime(i) );
      }
      else if(i == 0) IsValid_YStrip = false;
    }

//  cout << "IsValid_XStrip: " << IsValid_XStrip << endl;
//  cout << "IsValid_YStrip: " << IsValid_YStrip << endl;
    
    // CsI
    for(unsigned int i = 0 ; i < m_CsI_EMult ; ++i){
      if(IsValid_XStrip&&IsValid_YStrip){
        m_PreTreatedData->SetLassaCsIEDetectorNbr( m_EventData->GetLassaCsIEDetectorNbr(i) );
        m_PreTreatedData->SetLassaCsIECristalNbr( m_EventData->GetLassaCsIECristalNbr(i) );
        m_PreTreatedData->SetLassaCsIEEnergy( m_EventData->GetLassaCsIEEnergy(i) );
      }
    }
    

    return;
}


////////////////////////////////////////////////////////////////////////////
bool TLassaPhysics :: IsValidChannel(const string DetectorType, const int telescope , const int channel){
  if(DetectorType == "CsI")
    *(m_CsIChannelStatus[telescope].begin()+channel); //return *(m_CsIChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "X")
    return *(m_XChannelStatus[telescope].begin()+channel); //return *(m_XChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "Y")
   return *(m_YChannelStatus[telescope].begin()+channel); //return *(m_YChannelStatus[telescope-1].begin()+channel-1);

  else return false;
}

////////////////////////////////////////////////////////////////////////////
int TLassaPhysics :: CheckEvent(const double XEMult, const double YEMult){
  //Check that the multiplicity in the X-strip is the same as the y-strip
  
  if( XEMult == YEMult ){
    return 1; //Regular Event
  }

  else 
    return -1; //Rejected Event
}

///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::ReadAnalysisConfig(){
  bool ReadingStatus = false;

   for(int i = 0; i < m_NumberOfTelescope; i++){ //for(int i = 0; i < 8; i++){
          for(int j = 0; j < 16; j++){
            m_XChannelStatus[i][j] = true;
            m_YChannelStatus[i][j] = true;
          }
          for(int k = 0; k < 4; k++){
            m_CsIChannelStatus[i][k] = true;
          }
  }

  // path to file
  string FileName = "./configs/ConfigLassaTest.dat"; //"./configs/ConfigLassa.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigLassa.dat found: Default parameters loaded for Analysis " << FileName << endl;
    return;
  }
  cout << " Loading user parameters for Analysis from ConfigLassa.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigLassa.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 11, "ConfigLassa") == 0) ReadingStatus = true;

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
        int telescope = atoi(DataBuffer.substr(2,1).c_str());
        vector< bool > ChannelStatus;
        ChannelStatus.resize(128,false);
        m_XChannelStatus[telescope] = ChannelStatus; //m_XChannelStatus[telescope-1] = ChannelStatus;
        m_YChannelStatus[telescope] = ChannelStatus; //m_YChannelStatus[telescope-1] = ChannelStatus;
        ChannelStatus.resize(16,false);
        m_CsIChannelStatus[telescope]  = ChannelStatus; //m_CsIChannelStatus[telescope-1]  = ChannelStatus;
      }

    else if (whatToDo == "DISABLE_CHANNEL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        int telescope = atoi(DataBuffer.substr(2,1).c_str());
        int channel = -1;
        if (DataBuffer.compare(3,4,"STRX") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_XChannelStatus[telescope].begin()+channel) = false; //*(m_XChannelStatus[telescope-1].begin()+channel-1) = false;
        }

        else if (DataBuffer.compare(3,4,"STRY") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_YChannelStatus[telescope].begin()+channel) = false; //*(m_YChannelStatus[telescope-1].begin()+channel-1) = false;
        }

        else if (DataBuffer.compare(3,3,"CSI") == 0) {
          channel = atoi(DataBuffer.substr(6).c_str());
          *(m_CsIChannelStatus[telescope].begin()+channel) = false; //*(m_CsIChannelStatus[telescope-1].begin()+channel-1) = false;
        }

        else cout << "Warning: detector type for Lassa unknown!" << endl;

      }

    //I want to print out the values of my map to see if they are filled correctly

      else {
        ReadingStatus = false;
      }
   }
  }
  

  //I want to print out the values of my map to see if they are filled correctly

        for(int i = 0; i < m_NumberOfTelescope; i++){
          for(int j = 0; j < 16; j++){
//          cout << "Hello!" << endl;
            cout << "Telescope: " << i << "  " << "XStrip: " << j << "  " <<  m_XChannelStatus[i][j] << "  " << "YStrip: " << j << "  " <<  m_YChannelStatus[i][j] << endl;
          }
          for(int k = 0; k < 4; k++){
            cout << "Telescope: " << i << "  " << "Crystal: " << k << "  " << m_CsIChannelStatus[i][k] << endl;
          }
        }
      

}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TLassaPhysics::Construct(){
    return (NPL::VDetector*) new TLassaPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
    class proxy_lassa{ //class proxy{
    public:
        proxy_lassa(){//proxy(){
            NPL::DetectorFactory::getInstance()->AddToken("LASSAArray","Lassa");
            NPL::DetectorFactory::getInstance()->AddDetector("LASSAArray",TLassaPhysics::Construct);
        }
    };
    
    proxy_lassa p_lassa; //proxy p;
}

