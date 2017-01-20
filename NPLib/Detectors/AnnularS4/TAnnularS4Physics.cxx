/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : January 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularS4 Treated  data                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TAnnularS4Physics.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"

//   ROOT
#include "TChain.h"

ClassImp(TAnnularS4Physics)


///////////////////////////////////////////////////////////////////////////
TAnnularS4Physics::TAnnularS4Physics()
: m_EventData(new TAnnularS4Data),
m_PreTreatedData(new TAnnularS4Data),
m_EventPhysics(this),
m_Spectra(0),
m_E_RAW_Threshold(0), // adc channels
m_E_Threshold(0),     // MeV
m_NumberOfDetectors(0) {
}

///////////////////////////////////////////////////////////////////////////
/// A usefull method to bundle all operation to add a detector
void TAnnularS4Physics::AddDetector(TVector3 Position){
    // In That simple case nothing is done
    // Typically for more complex detector one would calculate the relevant
    // positions (stripped silicon) or angles (gamma array)
    
    double R_Min = 5;
    double R_Max = 65;
    
    double Phi_Min = 0  ;
    double Phi_Max = 360;
    
    int NumberOfRingStrip = 128 ;
    int NumberOfSector = 128 ;
    int Ring_NumberOfQuadrant = 1 ;
    
    double StripPitchSector = (Phi_Max-Phi_Min)/NumberOfSector ; //radial strip spacing in deg
    double StripPitchRing = (R_Max-R_Min)/NumberOfRingStrip  ; // ring strip spacing in mm
    
    
    m_NumberOfDetectors++;
    
    double X = Position.X();
    double Y = Position.Y();
    double Z = Position.Z();
    TVector3 Strip_1_1 = TVector3(0,0,Z);
    TVector3 StripCenter;
    
    vector<double> v_Theta;
    v_Theta.clear();
    for(int i=0; i<NumberOfRingStrip; i++){
        StripCenter = TVector3(R_Min+(i+0.5)*StripPitchRing,0,Z);
        v_Theta.push_back(StripCenter.Theta());
    }
    m_ThetaAngleOfRingStrip.push_back(v_Theta);
    for(int s=0; s<NumberOfSector; s++){
        double PhiAngle = s*StripPitchSector;
        m_PhiAngleOfSector.push_back(PhiAngle*M_PI/180);
        //cout << PhiAngle << endl;
    }

    return;
}

///////////////////////////////////////////////////////////////////////////
TVector3 TAnnularS4Physics::GetPositionOfInteraction(const int i) const{
    TVector3 Position;
    
    double R = GetR(DetectorNumber[i]);
    double Theta = GetTheta(DetectorNumber[i], StripNbr[i]);
    double Phi = GetPhi(SectorNbr[i]);
    
    Position = TVector3(R*tan(Theta)*cos(Phi),R*tan(Theta)*sin(Phi),R);
    
    return(Position) ;
    
}

///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::AddDetector(double R, double Theta, double Phi){
    // Compute the TVector3 corresponding
    TVector3 Pos(R*sin(Theta)*cos(Phi),R*sin(Theta)*sin(Phi),R*cos(Theta));
    m_R.push_back(R);
    AddDetector(Pos);
}

///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::BuildSimplePhysicalEvent() {
    BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::BuildPhysicalEvent() {
    // apply thresholds and calibration
    PreTreat();
    
    // match energy and time together
    unsigned int mysizeE = m_PreTreatedData->GetS4EMult();
    unsigned int mysizeT = m_PreTreatedData->GetS4TMult();
    for (UShort_t e = 0; e < mysizeE ; e++) {
        for (UShort_t t = 0; t < mysizeT ; t++) {
            if (m_PreTreatedData->GetS4_E_DetectorNbr(e) == m_PreTreatedData->GetS4_T_DetectorNbr(t)) {
                DetectorNumber.push_back(m_PreTreatedData->GetS4_E_DetectorNbr(e));
                Energy.push_back(m_PreTreatedData->GetS4_E_Energy(e));
                StripNbr.push_back(m_PreTreatedData->GetS4_E_StripNbr(e));
                SectorNbr.push_back(m_PreTreatedData->GetS4_E_SectorNbr(e));
                Time.push_back(m_PreTreatedData->GetS4_T_Time(t));
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::PreTreat() {
    // This method typically applies thresholds and calibrations
    // Might test for disabled channels for more complex detector
    
    // clear pre-treated object
    ClearPreTreatedData();
    
    // instantiate CalibrationManager
    static CalibrationManager* Cal = CalibrationManager::getInstance();
    
    // Energy
    unsigned int mysize = m_EventData->GetS4EMult();
    for (UShort_t i = 0; i < mysize ; ++i) {
        if (m_EventData->GetS4_E_Energy(i) > m_E_RAW_Threshold) {
            Double_t Energy = Cal->ApplyCalibration("AnnularS4/ENERGY"+NPL::itoa(m_EventData->GetS4_E_DetectorNbr(i)),m_EventData->GetS4_E_Energy(i));
            if (Energy > m_E_Threshold) {
                m_PreTreatedData->SetS4_E_Energy(Energy);
                m_PreTreatedData->SetS4_E_StripNbr(m_EventData->GetS4_E_StripNbr(i));
                m_PreTreatedData->SetS4_E_DetectorNbr(m_EventData->GetS4_E_DetectorNbr(i));
                m_PreTreatedData->SetS4_E_SectorNbr(m_EventData->GetS4_E_SectorNbr(i));
            }
        }
    }
    
    // Time
    mysize = m_EventData->GetS4TMult();
    for (UShort_t i = 0; i < mysize; ++i) {
        Double_t Time= Cal->ApplyCalibration("AnnularS4/TIME"+NPL::itoa(m_EventData->GetS4_T_DetectorNbr(i)),m_EventData->GetS4_T_Time(i));
        m_PreTreatedData->SetS4_T_Time(Time);
        m_PreTreatedData->SetS4_T_StripNbr(m_EventData->GetS4_T_StripNbr(i));
        m_PreTreatedData->SetS4_T_DetectorNbr(m_EventData->GetS4_T_DetectorNbr(i));
        m_PreTreatedData->SetS4_T_SectorNbr(m_EventData->GetS4_T_SectorNbr(i));
    }
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::ReadAnalysisConfig() {
    bool ReadingStatus = false;
    
    // path to file
    string FileName = "./configs/ConfigAnnularS4.dat";
    
    // open analysis config file
    ifstream AnalysisConfigFile;
    AnalysisConfigFile.open(FileName.c_str());
    
    if (!AnalysisConfigFile.is_open()) {
        cout << " No ConfigAnnularS4.dat found: Default parameter loaded for Analayis " << FileName << endl;
        return;
    }
    cout << " Loading user parameter for Analysis from ConfigAnnularS4.dat " << endl;
    
    // Save it in a TAsciiFile
    TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
    asciiConfig->AppendLine("%%% ConfigAnnularS4.dat %%%");
    asciiConfig->Append(FileName.c_str());
    asciiConfig->AppendLine("");
    // read analysis config file
    string LineBuffer,DataBuffer,whatToDo;
    while (!AnalysisConfigFile.eof()) {
        // Pick-up next line
        getline(AnalysisConfigFile, LineBuffer);
        
        // search for "header"
        string name = "ConfigAnnularS4";
        if (LineBuffer.compare(0, name.length(), name) == 0)
            ReadingStatus = true;
        
        // loop on tokens and data
        while (ReadingStatus ) {
            whatToDo="";
            AnalysisConfigFile >> whatToDo;
            
            // Search for comment symbol (%)
            if (whatToDo.compare(0, 1, "%") == 0) {
                AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
            }
            
            else if (whatToDo=="E_RAW_THRESHOLD") {
                AnalysisConfigFile >> DataBuffer;
                m_E_RAW_Threshold = atof(DataBuffer.c_str());
                cout << whatToDo << " " << m_E_RAW_Threshold << endl;
            }
            
            else if (whatToDo=="E_THRESHOLD") {
                AnalysisConfigFile >> DataBuffer;
                m_E_Threshold = atof(DataBuffer.c_str());
                cout << whatToDo << " " << m_E_Threshold << endl;
            }
            
            else {
                ReadingStatus = false;
            }
        }
    }
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::Clear() {
    DetectorNumber.clear();
    Energy.clear();
    Time.clear();
    StripNbr.clear();
    SectorNbr.clear();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::ReadConfiguration(NPL::InputParser parser) {
    vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("AnnularS4");
    if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << "//// " << blocks.size() << " detectors found " << endl;
    
    vector<string> cart = {"POS"};
    vector<string> sphe = {"R","Theta","Phi"};
    
    for(unsigned int i = 0 ; i < blocks.size() ; i++){
        if(blocks[i]->HasTokenList(cart)){
            if(NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "////  AnnularS4 " << i+1 <<  endl;
            
            TVector3 Pos = blocks[i]->GetTVector3("POS","mm");
            AddDetector(Pos);
        }
        else if(blocks[i]->HasTokenList(sphe)){
            if(NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "////  AnnularS4 " << i+1 <<  endl;
            double R = blocks[i]->GetDouble("R","mm");
            double Theta = blocks[i]->GetDouble("Theta","deg");
            double Phi = blocks[i]->GetDouble("Phi","deg");
            AddDetector(R,Theta,Phi);
        }
        else{
            cout << "ERROR: check your input file formatting " << endl;
            exit(1);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::InitSpectra() {
    m_Spectra = new TAnnularS4Spectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::FillSpectra() {
    m_Spectra -> FillRawSpectra(m_EventData);
    m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
    m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::CheckSpectra() {
    m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::ClearSpectra() {
    // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TAnnularS4Physics::GetSpectra() {
    if(m_Spectra)
        return m_Spectra->GetMapHisto();
    else{
        map< string , TH1*> empty;
        return empty;
    }
}



////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TAnnularS4Physics::GetCanvas() {
    if(m_Spectra)
        return m_Spectra->GetCanvas();
    else{
        vector<TCanvas*> empty;
        return empty;
    }
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::WriteSpectra() {
    m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::AddParameterToCalibrationManager() {
    CalibrationManager* Cal = CalibrationManager::getInstance();
    for (int i = 0; i < m_NumberOfDetectors; ++i) {
        Cal->AddParameter("AnnularS4", "D"+ NPL::itoa(i+1)+"_ENERGY","AnnularS4_D"+ NPL::itoa(i+1)+"_ENERGY");
        Cal->AddParameter("AnnularS4", "D"+ NPL::itoa(i+1)+"_TIME","AnnularS4_D"+ NPL::itoa(i+1)+"_TIME");
    }
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::InitializeRootInputRaw() {
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchStatus("AnnularS4",  true );
    inputChain->SetBranchAddress("AnnularS4", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::InitializeRootInputPhysics() {
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchAddress("AnnularS4", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TAnnularS4Physics::InitializeRootOutput() {
    TTree* outputTree = RootOutput::getInstance()->GetTree();
    outputTree->Branch("AnnularS4", "TAnnularS4Physics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TAnnularS4Physics::Construct() {
    return (NPL::VDetector*) new TAnnularS4Physics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
    class proxy_AnnularS4{
    public:
        proxy_AnnularS4(){
            NPL::DetectorFactory::getInstance()->AddToken("AnnularS4","AnnularS4");
            NPL::DetectorFactory::getInstance()->AddDetector("AnnularS4",TAnnularS4Physics::Construct);
        }
    };
    
    proxy_AnnularS4 p_AnnularS4;
}

