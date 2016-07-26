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
#include "THiraPhysics.h"
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPCalibrationManager.h"
#include "NPCalibrationManager.h"

//   STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
#include <cmath>
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

ClassImp(THiraPhysics)
///////////////////////////////////////////////////////////////////////////
THiraPhysics::THiraPhysics()
   {
       EventMultiplicity    = 0 ;
       m_NumberOfTelescope  = 0 ;
       m_EventData          = new THiraData ;
       m_PreTreatedData     = new THiraData;
       m_EventPhysics       = this ;
       
       m_NumberOfStrip = 32;
       m_NumberOfCrystal = 4;
       m_StripEnergyMatchingSigma = 0.020;
       m_StripEnergyMatchingNumberOfSigma = 5;
       m_Si_DE_E_RAW_Threshold = 0;
       m_Si_EF_E_RAW_Threshold = 0;
       m_Si_EB_E_RAW_Threshold = 0;
       m_Si_DE_E_Threshold = 0;
       m_Si_EF_E_Threshold = 0;
       m_Si_EB_E_Threshold = 0;
       m_CsI_E_RAW_Threshold = 0;
       m_CsI_E_Threshold = 0;

       m_Ignore_not_matching_CsI = true;
       
       m_CsI_Size = m_NumberOfStrip;
       m_CsI_MatchingX.resize(4,0);
       m_CsI_MatchingY.resize(4,0);
       for(int i=0; i<4; i++){
           m_CsI_MatchingX[0]=7;
           m_CsI_MatchingY[0]=7;
           
           m_CsI_MatchingX[1]=7;
           m_CsI_MatchingY[1]=23;

           m_CsI_MatchingX[2]=23;
           m_CsI_MatchingY[2]=7;

           m_CsI_MatchingX[3]=23;
           m_CsI_MatchingY[3]=23;
       }
   }
   
///////////////////////////////////////////////////////////////////////////
THiraPhysics::~THiraPhysics()
   {}
   
///////////////////////////////////////////////////////////////////////////
void THiraPhysics::Clear(){
    EventMultiplicity = 0;
    
    TelescopeNumber.clear();
    
    // Thin Si
    ThinSi_E.clear();
    ThinSi_T.clear();
    ThinSi_StripNumber.clear();
    
    //Thick Si
    ThickSi_E.clear();
    ThickSi_T.clear();
    ThickSi_EX.clear();
    ThickSi_EY.clear();
    ThickSi_TX.clear();
    ThickSi_TY.clear();
    ThickSi_StripNumberX.clear();
    ThickSi_StripNumberY.clear();
    
    // CsI
    CsI_E.clear();
    CsI_CrystalNumber.clear();
    
    
}

///////////////////////////////////////////////////////////////////////////
void THiraPhysics::ReadConfiguration(string Path){
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
    bool check_ThinSi = false;
    bool check_ThickSi = false;
    bool check_CsI = false;
    /*bool check_Theta = false ;
     bool check_Phi   = false ;
     bool check_R     = false ;*/
    
    
    
    while (!ConfigFile.eof()) {
        getline(ConfigFile, LineBuffer);
        if (LineBuffer.compare(0, 13, "HiraTelescope") == 0) {
            cout << "///" << endl           ;
            cout << "Hira element found: " << endl   ;
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
            
            else if (DataBuffer.compare(0, 10, "ThinSi_DE=") == 0) {
                check_ThinSi= true;
                ConfigFile >> DataBuffer ;
                m_build_ThinSi = atoi(DataBuffer.c_str()) ;
                cout << "Build ThinSi : " << m_build_ThinSi << endl;
            }
            
            else if (DataBuffer.compare(0, 10, "ThickSi_E=") == 0) {
                check_ThickSi= true;
                ConfigFile >> DataBuffer ;
                m_build_ThickSi = atoi(DataBuffer.c_str()) ;
                cout << "Build ThickSi : " << m_build_ThickSi << endl;
            }
            
            else if (DataBuffer.compare(0, 4, "CsI=") == 0) {
                check_CsI= true;
                ConfigFile >> DataBuffer ;
                m_build_CsI = atoi(DataBuffer.c_str()) ;
                cout << "Build CsI : " << m_build_CsI << endl;
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
            if (check_A && check_B && check_C && check_D && check_ThinSi && check_ThickSi && check_CsI) {
                
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
    
    InitializeStandardParameter();
    ReadAnalysisConfig();
    
    cout << endl << "/////////////////////////////" << endl << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void THiraPhysics::AddTelescope(TVector3 Pos1, TVector3 Pos2, TVector3 Pos3, TVector3 Pos4){
    m_NumberOfTelescope++;
    TVector3 Pos= 0.25*(Pos1+Pos2+Pos3+Pos4);//centroid of the telescope
    TVector3 u = Pos4-Pos2;//Horizontal Vector->EF
    TVector3 w = Pos.Unit();//Telescope normal
    TVector3 v = w.Cross(u);//Vertical Vector->EB
    u = u.Unit(); v = v.Unit(); w = w.Unit();
    
    //   Geometry Parameter
    double Face = 62.5; //mm
    double NumberOfStrip = m_NumberOfStrip;
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
    
    for( int i = 0 ; i < m_NumberOfStrip ; ++i ){
        lineX.clear();
        lineY.clear();
        lineZ.clear();
        
        for( int j = 0 ; j < m_NumberOfStrip ; ++j ){
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
TVector3 THiraPhysics::GetPositionOfInteraction(const int i) const{
    TVector3 Position = TVector3 (   GetStripPositionX( TelescopeNumber[i] , ThickSi_StripNumberX[i] , ThickSi_StripNumberY[i] ),
                                  GetStripPositionY( TelescopeNumber[i] , ThickSi_StripNumberX[i] , ThickSi_StripNumberY[i] ),
                                  GetStripPositionZ( TelescopeNumber[i] , ThickSi_StripNumberX[i] , ThickSi_StripNumberY[i] )) ;
    
    return(Position) ;
    
}

///////////////////////////////////////////////////////////////////////////
void THiraPhysics::AddParameterToCalibrationManager()
   {
      CalibrationManager* Cal = CalibrationManager::getInstance();
      
      for(int i = 0 ; i < m_NumberOfTelescope ; i++)
         {
            for( int j = 0 ; j < m_NumberOfStrip ; j++)
               {
                  Cal->AddParameter("Hira", "Detector"+itoa(i+1)+"_E","Hira_Detector"+itoa(i+1)+"_E")   ;
                  Cal->AddParameter("Hira", "Detector"+itoa(i+1)+"_T","Hira_Detector"+itoa(i+1)+"_T")   ;   
               }
      
         }
   }
   
///////////////////////////////////////////////////////////////////////////
void THiraPhysics::InitializeRootInputRaw() 
   {
      TChain* inputChain = RootInput::getInstance()->GetChain()     ;
      inputChain->SetBranchStatus ( "Hira"       , true )        ;
      inputChain->SetBranchStatus ( "fHira_*"    , true )        ;
      inputChain->SetBranchAddress( "Hira"       , &m_EventData )  ;
   }
///////////////////////////////////////////////////////////////////////////
void THiraPhysics::InitializeRootInputPhysics()
   {
      TChain* inputChain = RootInput::getInstance()->GetChain();
      inputChain->SetBranchStatus ( "Hira", true );
      //inputChain->SetBranchStatus ( "DetectorNumber", true );
      //inputChain->SetBranchStatus ( "Energy", true );
      //inputChain->SetBranchStatus ( "Time", true );
      inputChain->SetBranchAddress( "Hira", &m_EventPhysics );
   }
///////////////////////////////////////////////////////////////////////////
void THiraPhysics::InitializeRootOutput()
   {
      TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
      outputTree->Branch( "Hira" , "THiraPhysics" , &m_EventPhysics ) ;
   }

///////////////////////////////////////////////////////////////////////////
void THiraPhysics::BuildPhysicalEvent(){
    BuildSimplePhysicalEvent()   ;
}

///////////////////////////////////////////////////////////////////////////
void THiraPhysics::BuildSimplePhysicalEvent(){

    PreTreat();
    bool check_CSI = false;
    
    if(m_build_ThinSi){
        m_ThinSi_EMult      = m_PreTreatedData->GetHiraThinSiEMult();
    }
    
    m_ThickSi_EXMult    = m_PreTreatedData->GetHiraStripXEMult();
    m_ThickSi_EYMult    = m_PreTreatedData->GetHiraStripYEMult();
    m_ThickSi_TXMult    = m_PreTreatedData->GetHiraStripXTMult();
    m_ThickSi_TYMult    = m_PreTreatedData->GetHiraStripYTMult();
    
    m_CsI_EMult         = m_PreTreatedData->GetHiraCsIEMult();
    m_CsI_TMult         = m_PreTreatedData->GetHiraCsITMult();

    

    if(CheckEvent() == 1){
        
        vector<TVector2> couple = Match_EF_EB();
        EventMultiplicity = couple.size();
        
        for(unsigned int i=0; i<couple.size(); i++){
            check_CSI = false;
            
            int N = m_PreTreatedData->GetHiraStripXEDetectorNbr(couple[i].X());
            
            int X = m_PreTreatedData->GetHiraStripXEStripNbr(couple[i].X());
            int Y = m_PreTreatedData->GetHiraStripYEStripNbr(couple[i].Y());
            
            double Si_EF_E = m_PreTreatedData->GetHiraStripXEEnergy(couple[i].X());
            double Si_EB_E = m_PreTreatedData->GetHiraStripYEEnergy(couple[i].Y());
            
            //Search for associate Time
            /*double Si_EF_T = -100;
            for(unsigned int t = 0 ; t < m_ThickSi_TXMult ; ++t){
                if(   m_PreTreatedData->GetHiraStripXTStripNbr( couple[i].X() ) == m_PreTreatedData->GetHiraStripXTStripNbr(t)
                   && m_PreTreatedData->GetHiraStripXTDetectorNbr( couple[i].X() ) == m_PreTreatedData->GetHiraStripXTDetectorNbr(t)){
                    Si_EF_T = m_PreTreatedData->GetHiraStripXTTime(t);
                    break;
                }
            }
            
            double Si_EB_T = -100;
            for(unsigned int t = 0 ; t < m_ThickSi_TYMult ; ++t){
                if(   m_PreTreatedData->GetHiraStripYTStripNbr( couple[i].Y() ) == m_PreTreatedData->GetHiraStripYTStripNbr(t)
                && m_PreTreatedData->GetHiraStripYTDetectorNbr( couple[i].Y() ) == m_PreTreatedData->GetHiraStripYTDetectorNbr(t)){
                    Si_EB_T = m_PreTreatedData->GetHiraStripYTTime(t);
                    break;
                }
            }*/
            
            double Si_EF_T = m_PreTreatedData->GetHiraStripXTTime(couple[i].X());
            double Si_EB_T = m_PreTreatedData->GetHiraStripYTTime(couple[i].Y());
            
            TelescopeNumber.push_back(N);
            ThickSi_StripNumberX.push_back(X);
            ThickSi_StripNumberY.push_back(Y);
            ThickSi_EX.push_back(Si_EF_E);
            ThickSi_EY.push_back(Si_EB_E);
            ThickSi_TX.push_back(Si_EF_T);
            ThickSi_TY.push_back(Si_EB_T);
            
            ThickSi_E.push_back(Si_EF_E);
            ThickSi_T.push_back(Si_EF_T);
            
            // DE
            if(m_build_ThinSi){
                for(unsigned int d = 0 ; d < m_ThinSi_EMult ; ++d){
                    if(m_PreTreatedData->GetHiraThinSiStripEDetectorNbr(d)==N){
                        if(Match_Si_DE_E(X,m_PreTreatedData->GetHiraThinSiStripEStripNbr(d))){
                            ThinSi_StripNumber.push_back(m_PreTreatedData->GetHiraThinSiStripEStripNbr(d));
                            ThinSi_E.push_back(m_PreTreatedData->GetHiraThinSiStripEEnergy(d));
                            ThinSi_T.push_back(m_PreTreatedData->GetHiraThinSiStripTTime(d));
                        }
                    }
                }
            }
 
            // CsI
            for(unsigned int j=0; j<m_CsI_EMult; ++j){
                if(m_PreTreatedData->GetHiraCsIEDetectorNbr(j)==N){
                    if(Match_Si_CsI(X,Y,m_PreTreatedData->GetHiraCsIECristalNbr(j))){
                        CsI_CrystalNumber.push_back(m_PreTreatedData->GetHiraCsIECristalNbr(j));
                        CsI_E.push_back(m_PreTreatedData->GetHiraCsIEEnergy(j));

                        check_CSI = true;
                    }
                }
            }
            if(!check_CSI){
                CsI_CrystalNumber.push_back(-1);
                CsI_E.push_back(-100);
            }
        
        }
    }
    
    return;
}

///////////////////////////////////////////////////////////////////////////
void THiraPhysics::PreTreat(){
    ClearPreTreatedData();
    
    if(m_build_ThinSi){
        m_ThinSi_EMult      = m_EventData->GetHiraThinSiEMult();
    }
    
    m_ThickSi_EXMult    = m_EventData->GetHiraStripXEMult();
    m_ThickSi_EYMult    = m_EventData->GetHiraStripYEMult();
    m_ThickSi_TXMult    = m_EventData->GetHiraStripXTMult();
    m_ThickSi_TYMult    = m_EventData->GetHiraStripYTMult();
    
    m_CsI_EMult         = m_EventData->GetHiraCsIEMult();
    m_CsI_TMult         = m_EventData->GetHiraCsITMult();
    
    // Thin Si
    // DE
    if(m_build_ThinSi){
        for(unsigned int i = 0 ; i < m_ThinSi_EMult ; ++i){
            if(m_EventData->GetHiraThinSiStripEDetectorNbr(i)<m_NumberOfTelescope && m_EventData->GetHiraThinSiStripEStripNbr(i)<m_NumberOfStrip){
                if(m_EventData->GetHiraThinSiStripEEnergy(i)>m_Si_DE_E_RAW_Threshold && IsValidChannel("DE",m_EventData->GetHiraThinSiStripEDetectorNbr(i),m_EventData->GetHiraThinSiStripEStripNbr(i))){
                    m_PreTreatedData->SetHiraThinSiStripEDetectorNbr( m_EventData->GetHiraThinSiStripEDetectorNbr(i) );
                    m_PreTreatedData->SetHiraThinSiStripEStripNbr( m_EventData->GetHiraThinSiStripEStripNbr(i) );
                    m_PreTreatedData->SetHiraThinSiStripEEnergy( m_EventData->GetHiraThinSiStripEEnergy(i) );
                    m_PreTreatedData->SetHiraThinSiStripTTime( m_EventData->GetHiraThinSiStripTTime(i));
                }
            }
        }
    }
    // Thick Si
    // EF
    for(unsigned int i = 0 ; i < m_ThickSi_EXMult ; ++i){
        if(m_EventData->GetHiraStripXEDetectorNbr(i)<m_NumberOfTelescope && m_EventData->GetHiraStripXEStripNbr(i)<m_NumberOfStrip){
            if(m_EventData->GetHiraStripXEEnergy(i)>m_Si_EF_E_RAW_Threshold && IsValidChannel("EF",m_EventData->GetHiraStripXEDetectorNbr(i),m_EventData->GetHiraStripXEStripNbr(i))){
                m_PreTreatedData->SetHiraThickSiStripXEDetectorNbr( m_EventData->GetHiraStripXEDetectorNbr(i) );
                m_PreTreatedData->SetHiraThickSiStripXEStripNbr( m_EventData->GetHiraStripXEStripNbr(i) );
                m_PreTreatedData->SetHiraThickSiStripXEEnergy( m_EventData->GetHiraStripXEEnergy(i) );
                m_PreTreatedData->SetHiraThickSiStripXTTime( m_EventData->GetHiraStripXTTime(i) );
            }
        }
    }
    // EB
    for(unsigned int i = 0 ; i < m_ThickSi_EYMult ; ++i){
        if(m_EventData->GetHiraStripYEDetectorNbr(i)<m_NumberOfTelescope && m_EventData->GetHiraStripYEStripNbr(i)<m_NumberOfStrip){
            if(m_EventData->GetHiraStripYEEnergy(i)>m_Si_EB_E_RAW_Threshold && IsValidChannel("EB",m_EventData->GetHiraStripYEDetectorNbr(i),m_EventData->GetHiraStripYEStripNbr(i))){
                m_PreTreatedData->SetHiraThickSiStripYEDetectorNbr( m_EventData->GetHiraStripYEDetectorNbr(i) );
                m_PreTreatedData->SetHiraThickSiStripYEStripNbr( m_EventData->GetHiraStripYEStripNbr(i) );
                m_PreTreatedData->SetHiraThickSiStripYEEnergy( m_EventData->GetHiraStripYEEnergy(i) );
                m_PreTreatedData->SetHiraThickSiStripYTTime( m_EventData->GetHiraStripYTTime(i) );
            }
        }
    }
    
    // CsI
    for(unsigned int i = 0 ; i < m_CsI_EMult ; ++i){
        if(m_EventData->GetHiraCsIEDetectorNbr(i)<m_NumberOfTelescope && m_EventData->GetHiraCsIECristalNbr(i)<m_NumberOfCrystal){
            if(IsValidChannel("CsI",m_EventData->GetHiraCsIEDetectorNbr(i),m_EventData->GetHiraCsIECristalNbr(i))){
                m_PreTreatedData->SetHiraCsIEDetectorNbr( m_EventData->GetHiraCsIEDetectorNbr(i) );
                m_PreTreatedData->SetHiraCsIECristalNbr( m_EventData->GetHiraCsIECristalNbr(i) );
                m_PreTreatedData->SetHiraCsIEEnergy( m_EventData->GetHiraCsIEEnergy(i) );
            }
        }
    }
    
    

    return;
}

///////////////////////////////////////////////////////////////////////////
bool THiraPhysics::IsValidChannel(const string DetectorType, const int telescope , const int channel){
    if(DetectorType=="CsI")
        return *(m_CsIChannelStatus[telescope].begin()+channel);
    
    else if(DetectorType == "DE")
        return *(m_DEChannelStatus[telescope].begin()+channel);
    
    else if(DetectorType == "EF")
        return *(m_EFChannelStatus[telescope].begin()+channel);
    
    else if(DetectorType == "EB")
        return *(m_EBChannelStatus[telescope].begin()+channel);
    
    else return false;

}

///////////////////////////////////////////////////////////////////////////
void THiraPhysics::InitializeStandardParameter(){
    //Enable all channel
    vector< bool > ChannelStatus;
    m_DEChannelStatus.clear()    ;
    m_EFChannelStatus.clear()    ;
    m_EBChannelStatus.clear() ;
    m_CsIChannelStatus.clear()  ;
    
    ChannelStatus.resize(m_NumberOfStrip,true);
    for(int i = 0 ; i < m_NumberOfTelescope ; ++i){
        m_DEChannelStatus[i] = ChannelStatus;
        m_EFChannelStatus[i] = ChannelStatus;
        m_EBChannelStatus[i] = ChannelStatus;
    }
    
    ChannelStatus.resize(4,true);
    for(int i = 0 ; i < m_NumberOfTelescope ; ++i){
        m_CsIChannelStatus[i]  = ChannelStatus;
    }
    
    return;
}

///////////////////////////////////////////////////////////////////////////
void THiraPhysics::ReadAnalysisConfig(){
    bool ReadingStatus = false;
    
    // path to file
    string FileName = "./configs/ConfigHira.dat";
    
    // open analysis config file
    ifstream AnalysisConfigFile;
    AnalysisConfigFile.open(FileName.c_str());
    
    if (!AnalysisConfigFile.is_open()) {
        cout << " No ConfigMust2.dat found: Default parameters loaded for Analysis " << FileName << endl;
        return;
    }
    cout << endl;
    cout << "//////////////////////////////////////////////////////////////" << endl;
    cout << "// Loading user parameters for Analysis from ConfigHira.dat //" << endl;
    cout << "//////////////////////////////////////////////////////////////" << endl;
    
    // Save it in a TAsciiFile
    TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
    asciiConfig->AppendLine("%%% ConfigHira.dat %%%");
    asciiConfig->Append(FileName.c_str());
    asciiConfig->AppendLine("");
    // read analysis config file
    string LineBuffer,DataBuffer,whatToDo;
    while (!AnalysisConfigFile.eof()) {
        // Pick-up next line
        getline(AnalysisConfigFile, LineBuffer);
        
        // search for "header"
        if (LineBuffer.compare(0, 10, "ConfigHira") == 0) ReadingStatus = true;
        
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
                int telescope = atoi(DataBuffer.substr(4,1).c_str());
                vector< bool > ChannelStatus;
                ChannelStatus.resize(m_NumberOfStrip,false);
                m_EFChannelStatus[telescope] = ChannelStatus;
                m_EBChannelStatus[telescope] = ChannelStatus;
                m_DEChannelStatus[telescope] = ChannelStatus;
                ChannelStatus.resize(4,false);
                m_CsIChannelStatus[telescope]  = ChannelStatus;
            }
            
            else if (whatToDo == "DISABLE_CHANNEL") {
                AnalysisConfigFile >> DataBuffer;
                cout << whatToDo << "  " << DataBuffer << endl;
                int telescope = atoi(DataBuffer.substr(4,1).c_str());
                int channel = -1;
                if (DataBuffer.compare(5,5,"STRDE") == 0) {
                    channel = atoi(DataBuffer.substr(10).c_str());
                    *(m_DEChannelStatus[telescope].begin()+channel) = false;
                }
                
                else if (DataBuffer.compare(5,5,"STREF") == 0) {
                    channel = atoi(DataBuffer.substr(10).c_str());
                    *(m_EFChannelStatus[telescope].begin()+channel) = false;
                }
                
                else if (DataBuffer.compare(5,5,"STREB") == 0) {
                    channel = atoi(DataBuffer.substr(10).c_str());
                    *(m_EBChannelStatus[telescope].begin()+channel) = false;
                }
                
                else if (DataBuffer.compare(5,3,"CSI") == 0) {
                    channel = atoi(DataBuffer.substr(8).c_str());
                    *(m_CsIChannelStatus[telescope].begin()+channel) = false;
                }
                
                else cout << "Warning: detector type for Hira unknown!" << endl;
                
            }
            
            else if (whatToDo=="TAKE_E_EF") {
                m_Take_E_EF = true;
                cout << whatToDo << endl;
            }
            
            else if (whatToDo=="TAKE_E_EB") {
                m_Take_E_EB = false;
                cout << whatToDo << endl;
            }
            
            else if (whatToDo== "IGNORE_NOT_MATCHING_CSI") {
                m_Ignore_not_matching_CsI = true;
                cout << whatToDo << endl;
            }
            
            else if (whatToDo=="CSI_SIZE") {
                AnalysisConfigFile >> DataBuffer;
                m_CsI_Size = atoi(DataBuffer.c_str());
                cout << whatToDo << " " << m_CsI_Size << endl;
            }
            
            
            else if (whatToDo=="SI_DE_E_RAW_THRESHOLD") {
                AnalysisConfigFile >> DataBuffer;
                m_Si_DE_E_RAW_Threshold = atof(DataBuffer.c_str());
                cout << whatToDo << " " << m_Si_DE_E_RAW_Threshold << endl;
            }
            
            else if (whatToDo=="SI_EF_E_RAW_THRESHOLD") {
                AnalysisConfigFile >> DataBuffer;
                m_Si_EF_E_RAW_Threshold = atof(DataBuffer.c_str());
                cout << whatToDo << " " << m_Si_EF_E_RAW_Threshold << endl;
            }
            
            else if (whatToDo=="SI_EF_EB_RAW_THRESHOLD") {
                AnalysisConfigFile >> DataBuffer;
                m_Si_EB_E_RAW_Threshold = atof(DataBuffer.c_str());
                cout << whatToDo << " " << m_Si_EB_E_RAW_Threshold << endl;
            }
            
            else if (whatToDo== "CSI_E_RAW_THRESHOLD") {
                AnalysisConfigFile >> DataBuffer;
                m_CsI_E_RAW_Threshold = atof(DataBuffer.c_str());
                cout << whatToDo << " " << m_CsI_E_RAW_Threshold << endl;
            }
            
            else if (whatToDo=="SI_DE_E_THRESHOLD") {
                AnalysisConfigFile >> DataBuffer;
                m_Si_DE_E_Threshold = atof(DataBuffer.c_str());
                cout << whatToDo << " " << m_Si_DE_E_Threshold << endl;
            }
            
            else if (whatToDo== "SI_EF_E_THRESHOLD") {
                AnalysisConfigFile >> DataBuffer;
                m_Si_EF_E_Threshold = atof(DataBuffer.c_str());
                cout << whatToDo << " " << m_Si_EF_E_Threshold << endl;
            }
            
            else if (whatToDo== "SI_EB_E_THRESHOLD") {
                AnalysisConfigFile >> DataBuffer;
                m_Si_EB_E_Threshold = atof(DataBuffer.c_str());
                cout << whatToDo << " " << m_Si_EB_E_Threshold << endl;
            }
            
            else if (whatToDo=="CSI_E_THRESHOLD") {
                AnalysisConfigFile >> DataBuffer;
                m_CsI_E_Threshold = atof(DataBuffer.c_str());
                cout << whatToDo << " " << m_CsI_E_Threshold << endl;
            }
            
            else {
                ReadingStatus = false;
            }
            
        }
    }
}


///////////////////////////////////////////////////////////////////////////
vector <TVector2> THiraPhysics::Match_EF_EB(){
    vector<TVector2> ArrayOfGoodCouple;
    m_ThickSi_EXMult    = m_PreTreatedData->GetHiraStripXEMult();
    m_ThickSi_EYMult    = m_PreTreatedData->GetHiraStripYEMult();
    
    for(unsigned int i=0; i<m_ThickSi_EXMult; i++){
        for(unsigned int j=0; j<m_ThickSi_EYMult; j++){
            // if same detector check energy
            if(m_PreTreatedData->GetHiraStripXEDetectorNbr(i) == m_PreTreatedData->GetHiraStripYEDetectorNbr(j)){
                // Look if energy match
                if( fabs( (m_PreTreatedData->GetHiraStripXEEnergy(i) - m_PreTreatedData->GetHiraStripYEEnergy(j))/2. ) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma){
                    //Special Option, if the event is between two CsI crystal, it is rejected
                    if(m_Ignore_not_matching_CsI){
                        bool check_validity_csi=false;
                        for (unsigned int hh = 0 ; hh<4 ; ++hh ){
                            if( Match_Si_CsI(m_PreTreatedData->GetHiraStripXEStripNbr(i), m_PreTreatedData->GetHiraStripYEStripNbr(j) , hh) ){
                                check_validity_csi=true;
                            }
                        }
                        if(check_validity_csi)
                            ArrayOfGoodCouple.push_back ( TVector2(i,j) ) ;
                    }
                    // Regular case, keep the event
                    else ArrayOfGoodCouple.push_back (TVector2(i,j));
                }
            }
        }
    }
    
    return ArrayOfGoodCouple;
}

///////////////////////////////////////////////////////////////////////////
bool THiraPhysics::Match_Si_CsI(int EF, int EB, int CristalNbr){
    
    if( abs(m_CsI_MatchingX[CristalNbr] - EF) < m_CsI_Size/2.&&
       abs(m_CsI_MatchingY[CristalNbr] - EB) < m_CsI_Size/2.)
        return true ;
    
    else
        return false;
}

///////////////////////////////////////////////////////////////////////////
bool THiraPhysics::Match_Si_DE_E(int EF, int DEStipNumber){
    
    if( abs(EF - DEStipNumber) < 2)
        return true ;
    
    else
        return false;
}

///////////////////////////////////////////////////////////////////////////
int THiraPhysics::CheckEvent(){
    // Check the size of the different elements
    if( (m_PreTreatedData->GetHiraStripXEMult() == m_PreTreatedData->GetHiraStripYEMult()) )
        return 1 ; // Regular Event
    
    // INterstrip management is not coded, so waste of time to make this test
    /*  else if(   m_PreTreatedData->GetMMStripXEMult() == m_PreTreatedData->GetMMStripYEMult()+1
     || m_PreTreatedData->GetMMStripXEMult() == m_PreTreatedData->GetMMStripYEMult()-1  )
     return 2 ; // Pseudo Event, potentially interstrip*/
    
    else
        return -1 ; // Rejected Event
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* THiraPhysics::Construct(){
    return (NPL::VDetector*) new THiraPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
    class proxy_hira{
    public:
        proxy_hira(){
            NPL::DetectorFactory::getInstance()->AddToken("HIRAArray","Hira");
            NPL::DetectorFactory::getInstance()->AddDetector("HIRAArray",THiraPhysics::Construct);
        }
    };
    
    proxy_hira p;
}

