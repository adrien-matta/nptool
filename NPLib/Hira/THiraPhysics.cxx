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

ClassImp(THiraPhysics)
///////////////////////////////////////////////////////////////////////////
THiraPhysics::THiraPhysics()
   {
       //EventMultiplicity    = 0 ;
       m_NumberOfTelescope  = 0 ;
       m_EventData          = new THiraData ;
       m_PreTreatedData     = new THiraData;
       m_EventPhysics       = this ;
   }
   
///////////////////////////////////////////////////////////////////////////
THiraPhysics::~THiraPhysics()
   {}
   
///////////////////////////////////////////////////////////////////////////
void THiraPhysics::Clear(){
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
    double Face = 64.; //mm
    double NumberOfStrip = 32;
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
            for( int j = 0 ; j < 16 ; j++)
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
      inputChain->SetBranchStatus ( "DetectorNumber", true );
      inputChain->SetBranchStatus ( "Energy", true );
      inputChain->SetBranchStatus ( "Time", true );
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
    m_ThinSi_EMult      = m_PreTreatedData->GetHiraThinSiEMult();
    m_ThickSi_EXMult    = m_PreTreatedData->GetHiraStripXEMult();
    m_ThickSi_EYMult    = m_PreTreatedData->GetHiraStripYEMult();
    m_CsI_EMult         = m_PreTreatedData->GetHiraCsIEMult();
    
    
    for(unsigned int i = 0 ; i < m_ThinSi_EMult ; ++i){
        TelescopeNumber.push_back(m_PreTreatedData->GetHiraThinSiStripEDetectorNbr(i));
        ThinSi_StripNumber.push_back(m_PreTreatedData->GetHiraThinSiStripEStripNbr(i));
        ThinSi_E.push_back(m_PreTreatedData->GetHiraThinSiStripEEnergy(i));
        ThinSi_T.push_back(m_PreTreatedData->GetHiraThinSiStripTTime(i));
    }
    
    for(unsigned int i = 0 ; i < m_ThickSi_EXMult ; ++i){
        ThickSi_StripNumberX.push_back(m_PreTreatedData->GetHiraStripXEStripNbr(i));
        ThickSi_EX.push_back(m_PreTreatedData->GetHiraStripXEEnergy(i));
        ThickSi_T.push_back(m_EventData->GetHiraStripXTTime(i));
        ThickSi_E.push_back(m_PreTreatedData->GetHiraStripXEEnergy(i));
    }
    
    for(unsigned int i = 0 ; i < m_ThickSi_EXMult ; ++i){
        ThickSi_StripNumberY.push_back(m_PreTreatedData->GetHiraStripYEStripNbr(i));
        ThickSi_EY.push_back(m_PreTreatedData->GetHiraStripYEEnergy(i));
    }
    
    for(unsigned int i = 0 ; i < m_CsI_EMult ; ++i){
        CsI_CrystalNumber.push_back(m_PreTreatedData->GetHiraCsIECristalNbr(i));
        CsI_E.push_back(m_PreTreatedData->GetHiraCsIEEnergy(i));
    }
    
    return;
}

///////////////////////////////////////////////////////////////////////////
void THiraPhysics::PreTreat(){
    ClearPreTreatedData();
    m_ThinSi_EMult      = m_EventData->GetHiraThinSiEMult();
    m_ThickSi_EXMult    = m_EventData->GetHiraStripXEMult();
    m_ThickSi_EYMult    = m_EventData->GetHiraStripYEMult();
    m_CsI_EMult         = m_EventData->GetHiraCsIEMult();
    
    
    // Thin Si
    for(unsigned int i = 0 ; i < m_ThinSi_EMult ; ++i){
        m_PreTreatedData->SetHiraThinSiStripEDetectorNbr( m_EventData->GetHiraThinSiStripEDetectorNbr(i) );
        m_PreTreatedData->SetHiraThinSiStripEStripNbr( m_EventData->GetHiraThinSiStripEStripNbr(i) );
        m_PreTreatedData->SetHiraThinSiStripEEnergy( m_EventData->GetHiraThinSiStripEEnergy(i) );
        m_PreTreatedData->SetHiraThinSiStripTTime( m_EventData->GetHiraThinSiStripTTime(i));
    }
    
    // Thick Si
    // X
    for(unsigned int i = 0 ; i < m_ThickSi_EXMult ; ++i){
        m_PreTreatedData->SetHiraThickSiStripXEDetectorNbr( m_EventData->GetHiraStripXEDetectorNbr(i) );
        m_PreTreatedData->SetHiraThickSiStripXEStripNbr( m_EventData->GetHiraStripXEStripNbr(i) );
        m_PreTreatedData->SetHiraThickSiStripXEEnergy( m_EventData->GetHiraStripXEEnergy(i) );
        m_PreTreatedData->SetHiraThickSiStripXTTime( m_EventData->GetHiraStripXTTime(i) );
    }
    // Y
    for(unsigned int i = 0 ; i < m_ThickSi_EYMult ; ++i){
        m_PreTreatedData->SetHiraThickSiStripYEDetectorNbr( m_EventData->GetHiraStripYEDetectorNbr(i) );
        m_PreTreatedData->SetHiraThickSiStripYEStripNbr( m_EventData->GetHiraStripYEStripNbr(i) );
        m_PreTreatedData->SetHiraThickSiStripYEEnergy( m_EventData->GetHiraStripYEEnergy(i) );
        m_PreTreatedData->SetHiraThickSiStripYTTime( m_EventData->GetHiraStripYTTime(i) );
    }
    
    // CsI
    for(unsigned int i = 0 ; i < m_CsI_EMult ; ++i){
        m_PreTreatedData->SetHiraCsIEDetectorNbr( m_EventData->GetHiraCsIEDetectorNbr(i) );
        m_PreTreatedData->SetHiraCsIECristalNbr( m_EventData->GetHiraCsIECristalNbr(i) );
        m_PreTreatedData->SetHiraCsIEEnergy( m_EventData->GetHiraCsIEEnergy(i) );
    }
    
    

    return;
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
    class proxy{
    public:
        proxy(){
            NPL::DetectorFactory::getInstance()->AddToken("HIRAArray","Hira");
            NPL::DetectorFactory::getInstance()->AddDetector("HIRAArray",THiraPhysics::Construct);
        }
    };
    
    proxy p;
}

