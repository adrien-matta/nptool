/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
#include "NPOptionManager.h"
//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
#include <fstream>
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
    EventMultiplicity    = 0 ;
    m_NumberOfTelescope  = 0 ;
    m_EventData          = new TLassaData ;
    m_PreTreatedData     = new TLassaData;
    m_EventPhysics       = this ;
    
    m_NumberOfStrip = 16;
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
    
    m_CsI_Size = 8;
    m_CsI_MatchingX.resize(4,0);
    m_CsI_MatchingY.resize(4,0);
    for(int i=0; i<4; i++){
        m_CsI_MatchingX[0]=3;
        m_CsI_MatchingY[0]=3;
        
        m_CsI_MatchingX[1]=3;
        m_CsI_MatchingY[1]=11;
        
        m_CsI_MatchingX[2]=11;
        m_CsI_MatchingY[2]=3;
        
        m_CsI_MatchingX[3]=11;
        m_CsI_MatchingY[3]=11;
    }

}

///////////////////////////////////////////////////////////////////////////
TLassaPhysics::~TLassaPhysics()
{}

///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::Clear(){
    EventMultiplicity = 0;
    
    TelescopeNumber.clear();
    
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
void TLassaPhysics::ReadConfiguration(NPL::InputParser parser){


  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("LassaTelescope");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " Telescope found " << endl; 

  // Cartesian Case
  vector<string> cart = {"A","B","C","D"};
  for(unsigned int i  = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Lassa Telescope " << i+1 <<  endl;
      TVector3 A = blocks[i]->GetTVector3("A","mm");
      TVector3 B = blocks[i]->GetTVector3("B","mm");
      TVector3 C = blocks[i]->GetTVector3("C","mm");
      TVector3 D = blocks[i]->GetTVector3("D","mm");
      AddTelescope(A,B,C,D) ;
    }

    else{
      cout << "ERROR: Missing token for M2Telescope blocks, check your input file" << endl;
      exit(1);
    }

  }

  InitializeStandardParameter();
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
        for( int j = 0 ; j < m_NumberOfStrip ; j++)
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
    bool check_CSI = false;

    m_ThickSi_EXMult    = m_PreTreatedData->GetLassaStripXEMult();
    m_ThickSi_EYMult    = m_PreTreatedData->GetLassaStripYEMult();
    
    m_CsI_EMult         = m_PreTreatedData->GetLassaCsIEMult();
    
    if(CheckEvent() == 1){
        
        vector<TVector2> couple = Match_EF_EB();
        EventMultiplicity = couple.size();
        
        for(unsigned int i=0; i<couple.size(); i++){
            check_CSI = false;
            
            int N = m_PreTreatedData->GetLassaStripXEDetectorNbr(couple[i].X());
            
            int X = m_PreTreatedData->GetLassaStripXEStripNbr(couple[i].X());
            int Y = m_PreTreatedData->GetLassaStripYEStripNbr(couple[i].Y());
            
            double Si_EF_E = m_PreTreatedData->GetLassaStripXEEnergy(couple[i].X());
            double Si_EB_E = m_PreTreatedData->GetLassaStripYEEnergy(couple[i].Y());
            
            double Si_EF_T = m_PreTreatedData->GetLassaStripXTTime(couple[i].X());
            double Si_EB_T = m_PreTreatedData->GetLassaStripYTTime(couple[i].Y());
            
            TelescopeNumber.push_back(N);
            ThickSi_StripNumberX.push_back(X);
            ThickSi_StripNumberY.push_back(Y);
            ThickSi_EX.push_back(Si_EF_E);
            ThickSi_EY.push_back(Si_EB_E);
            
            ThickSi_E.push_back(Si_EF_E);
            ThickSi_T.push_back(Si_EF_T);
            
            // CsI
            for(unsigned int j=0; j<m_CsI_EMult; ++j){
                if(m_PreTreatedData->GetLassaCsIEDetectorNbr(j)==N){
                    if(Match_Si_CsI(X,Y,m_PreTreatedData->GetLassaCsIECristalNbr(j))){
                        CsI_CrystalNumber.push_back(m_PreTreatedData->GetLassaCsIECristalNbr(j));
                        CsI_E.push_back(m_PreTreatedData->GetLassaCsIEEnergy(j));
                        
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
void TLassaPhysics::PreTreat(){
    ClearPreTreatedData();
    
    m_ThickSi_EXMult    = m_EventData->GetLassaStripXEMult();
    m_ThickSi_EYMult    = m_EventData->GetLassaStripYEMult();
    
    m_CsI_EMult         = m_EventData->GetLassaCsIEMult();
    
    
    // CsI
    for(unsigned int i = 0 ; i < m_CsI_EMult ; ++i){
        if(m_EventData->GetLassaCsIEDetectorNbr(i)<m_NumberOfTelescope && m_EventData->GetLassaCsIECristalNbr(i)<m_NumberOfCrystal){
            if(IsValidChannel("CsI",m_EventData->GetLassaCsIEDetectorNbr(i),m_EventData->GetLassaCsIECristalNbr(i))){
                m_PreTreatedData->SetLassaCsIEDetectorNbr( m_EventData->GetLassaCsIEDetectorNbr(i) );
                m_PreTreatedData->SetLassaCsIECristalNbr( m_EventData->GetLassaCsIECristalNbr(i) );
                m_PreTreatedData->SetLassaCsIEEnergy( m_EventData->GetLassaCsIEEnergy(i) );
            }
        }
    }
                
    // X->EF
    for(unsigned int i = 0 ; i < m_ThickSi_EXMult ; ++i){
        if(m_EventData->GetLassaStripXEDetectorNbr(i)<m_NumberOfTelescope && m_EventData->GetLassaStripXEStripNbr(i)<m_NumberOfStrip){
            if(IsValidChannel("X",m_EventData->GetLassaStripXEDetectorNbr(i),m_EventData->GetLassaStripXEStripNbr(i))){
                m_PreTreatedData->SetLassaThickSiStripXEDetectorNbr( m_EventData->GetLassaStripXEDetectorNbr(i) );
                m_PreTreatedData->SetLassaThickSiStripXEStripNbr( m_EventData->GetLassaStripXEStripNbr(i) );
                m_PreTreatedData->SetLassaThickSiStripXEEnergy( m_EventData->GetLassaStripXEEnergy(i) );
                m_PreTreatedData->SetLassaThickSiStripXTTime( m_EventData->GetLassaStripXTTime(i) );
            }
        }
    }
    // Y->EB
    for(unsigned int i = 0 ; i < m_ThickSi_EYMult ; ++i){
        if(m_EventData->GetLassaStripYEDetectorNbr(i)<m_NumberOfTelescope && m_EventData->GetLassaStripYEStripNbr(i)<m_NumberOfStrip){
            if(IsValidChannel("Y",m_EventData->GetLassaStripYEDetectorNbr(i),m_EventData->GetLassaStripYEStripNbr(i))){
                m_PreTreatedData->SetLassaThickSiStripYEDetectorNbr( m_EventData->GetLassaStripYEDetectorNbr(i) );
                m_PreTreatedData->SetLassaThickSiStripYEStripNbr( m_EventData->GetLassaStripYEStripNbr(i) );
                m_PreTreatedData->SetLassaThickSiStripYEEnergy( m_EventData->GetLassaStripYEEnergy(i) );
                m_PreTreatedData->SetLassaThickSiStripYTTime( m_EventData->GetLassaStripYTTime(i) );
            }
        }
    }
    

    
    return;
}


////////////////////////////////////////////////////////////////////////////
bool TLassaPhysics :: IsValidChannel(const string DetectorType, const int telescope , const int channel){
    if(DetectorType == "CsI")
        return *(m_CsIChannelStatus[telescope].begin()+channel);
    
    else if(DetectorType == "X")
        return *(m_XChannelStatus[telescope].begin()+channel);
    
    else if(DetectorType == "Y")
        return *(m_YChannelStatus[telescope].begin()+channel);
    
    else return false;
    
}

////////////////////////////////////////////////////////////////////////////
int TLassaPhysics :: CheckEvent(){
    
    if( m_PreTreatedData->GetLassaStripXEMult() == m_PreTreatedData->GetLassaStripYEMult() )
        return 1 ; // Regular Event
    
    else
        return -1; //Rejected Event
}

///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::ReadAnalysisConfig(){
    bool ReadingStatus = false;
    
    // path to file
    string FileName = "./configs/ConfigLassa.dat";
    
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
                ChannelStatus.resize(m_NumberOfStrip,false);
                m_XChannelStatus[telescope] = ChannelStatus;
                m_YChannelStatus[telescope] = ChannelStatus;
                ChannelStatus.resize(4,false);
                m_CsIChannelStatus[telescope]  = ChannelStatus;
            }
            
            else if (whatToDo == "DISABLE_CHANNEL") {
                AnalysisConfigFile >> DataBuffer;
                cout << whatToDo << "  " << DataBuffer << endl;
                int telescope = atoi(DataBuffer.substr(2,1).c_str());
                int channel = -1;
                if (DataBuffer.compare(3,4,"STRX") == 0) {
                    channel = atoi(DataBuffer.substr(7).c_str());
                    *(m_XChannelStatus[telescope].begin()+channel) = false;
                }
                
                else if (DataBuffer.compare(3,4,"STRY") == 0) {
                    channel = atoi(DataBuffer.substr(7).c_str());
                    *(m_YChannelStatus[telescope].begin()+channel) = false;
                }
                
                else if (DataBuffer.compare(3,3,"CSI") == 0) {
                    channel = atoi(DataBuffer.substr(6).c_str());
                    *(m_CsIChannelStatus[telescope].begin()+channel) = false;
                }
                
                else cout << "Warning: detector type for Lassa unknown!" << endl;
                
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
vector <TVector2> TLassaPhysics::Match_EF_EB(){
    vector<TVector2> ArrayOfGoodCouple;
    m_ThickSi_EXMult    = m_PreTreatedData->GetLassaStripXEMult();
    m_ThickSi_EYMult    = m_PreTreatedData->GetLassaStripYEMult();
    
    for(unsigned int i=0; i<m_ThickSi_EXMult; i++){
        for(unsigned int j=0; j<m_ThickSi_EYMult; j++){
            // if same detector check energy
            if(m_PreTreatedData->GetLassaStripXEDetectorNbr(i) == m_PreTreatedData->GetLassaStripYEDetectorNbr(j)){
                // Look if energy match
                if( fabs( (m_PreTreatedData->GetLassaStripXEEnergy(i) - m_PreTreatedData->GetLassaStripYEEnergy(j))/2. ) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma){
                    //Special Option, if the event is between two CsI crystal, it is rejected
                    if(m_Ignore_not_matching_CsI){
                        bool check_validity=false;
                        for (unsigned int hh = 0 ; hh<4 ; ++hh ){
                            if(IsValidChannel("CsI",m_PreTreatedData->GetLassaStripXEDetectorNbr(i),hh) && Match_Si_CsI(m_PreTreatedData->GetLassaStripXEStripNbr(i), m_PreTreatedData->GetLassaStripYEStripNbr(j) , hh) ){
                                check_validity=true;
                            }
                        }
                        if(check_validity)
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
bool TLassaPhysics::Match_Si_CsI(int EF, int EB, int CristalNbr){
    
    if( abs(m_CsI_MatchingX[CristalNbr] - EF) < m_CsI_Size/2.&&
       abs(m_CsI_MatchingY[CristalNbr] - EB) < m_CsI_Size/2.)
        return true ;
    
    else
        return false;
}


///////////////////////////////////////////////////////////////////////////
void TLassaPhysics::InitializeStandardParameter(){
    //   Enable all channel
    vector< bool > ChannelStatus;
    m_XChannelStatus.clear()    ;
    m_YChannelStatus.clear()    ;
    m_CsIChannelStatus.clear()  ;
    
    ChannelStatus.resize(m_NumberOfStrip,true);
    for(int i = 0 ; i < m_NumberOfTelescope ; ++i){
        m_XChannelStatus[i] = ChannelStatus;
        m_YChannelStatus[i] = ChannelStatus;
    }
    
    ChannelStatus.resize(4,true);
    for(int i = 0 ; i < m_NumberOfTelescope ; ++i){
        m_CsIChannelStatus[i]  = ChannelStatus;
    }
    
    return;
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

