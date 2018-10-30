/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: valerian  contact address: girardalcindor@ganil.fr                        *
 *                                                                           *
 * Creation Date  : November 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold beam_dump Treated  data                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "Tbeam_dumpPhysics.h"

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

ClassImp(Tbeam_dumpPhysics)


    ///////////////////////////////////////////////////////////////////////////
Tbeam_dumpPhysics::Tbeam_dumpPhysics()
    : m_EventData(new Tbeam_dumpData),
    m_PreTreatedData(new Tbeam_dumpData),
    m_EventPhysics(this),
    m_Spectra(0),
    m_E_RAW_Threshold(0), // adc channels
    m_E_Threshold(0),     // MeV
    m_NumberOfDetectors(0) {
    }

///////////////////////////////////////////////////////////////////////////
/// A usefull method to bundle all operation to add a detector
///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::AddDetector(double R, double Theta, double Phi, double thickness){
    // Compute the TVector3 corresponding
    TVector3 Pos(R*sin(Theta)*cos(Phi),R*sin(Theta)*sin(Phi),R*cos(Theta));
    // Call the cartesian method
    m_NumberOfDetectors++;
} 

///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::BuildSimplePhysicalEvent() {
    BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::BuildPhysicalEvent() {
    // apply thresholds and calibration
    PreTreat();

    // match energy and time together
    unsigned int mysizeE = m_PreTreatedData->GetMultEnergy();
    unsigned int mysizeT = m_PreTreatedData->GetMultTime();
    for (UShort_t e = 0; e < mysizeE ; e++) {
        for (UShort_t t = 0; t < mysizeT ; t++) {
            if (m_PreTreatedData->GetE_DetectorNbr(e) == m_PreTreatedData->GetT_DetectorNbr(t)) {
                DetectorNumber.push_back(m_PreTreatedData->GetE_DetectorNbr(e));
                Energy.push_back(m_PreTreatedData->Get_Energy(e));
                Time.push_back(m_PreTreatedData->Get_Time(t));
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::PreTreat() {
    // This method typically applies thresholds and calibrations
    // Might test for disabled channels for more complex detector

    // clear pre-treated object
    ClearPreTreatedData();

    // instantiate CalibrationManager
    static CalibrationManager* Cal = CalibrationManager::getInstance();

    // Energy
    unsigned int mysize = m_EventData->GetMultEnergy();
    for (UShort_t i = 0; i < mysize ; ++i) {
        if (m_EventData->Get_Energy(i) > m_E_RAW_Threshold) {
            Double_t Energy = Cal->ApplyCalibration("beam_dump/ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
            if (Energy > m_E_Threshold) {
                m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i), Energy);
            }
        }
    }

    // Time 
    mysize = m_EventData->GetMultTime();
    for (UShort_t i = 0; i < mysize; ++i) {
        Double_t Time= Cal->ApplyCalibration("beam_dump/TIME"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)),m_EventData->Get_Time(i));
        m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i), Time);
    }
}



///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::ReadAnalysisConfig() {
    bool ReadingStatus = false;

    // path to file
    string FileName = "./configs/Configbeam_dump.dat";

    // open analysis config file
    ifstream AnalysisConfigFile;
    AnalysisConfigFile.open(FileName.c_str());

    if (!AnalysisConfigFile.is_open()) {
        cout << " No Configbeam_dump.dat found: Default parameter loaded for Analayis " << FileName << endl;
        return;
    }
    cout << " Loading user parameter for Analysis from Configbeam_dump.dat " << endl;

    // Save it in a TAsciiFile
    TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
    asciiConfig->AppendLine("%%% Configbeam_dump.dat %%%");
    asciiConfig->Append(FileName.c_str());
    asciiConfig->AppendLine("");
    // read analysis config file
    string LineBuffer,DataBuffer,whatToDo;
    while (!AnalysisConfigFile.eof()) {
        // Pick-up next line
        getline(AnalysisConfigFile, LineBuffer);

        // search for "header"
        string name = "Configbeam_dump";
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
void Tbeam_dumpPhysics::Clear() {
    DetectorNumber.clear();
    Energy.clear();
    Time.clear();
}



///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::ReadConfiguration(NPL::InputParser parser) {
    vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("beam_dump");
    if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << "//// " << blocks.size() << " detectors found " << endl; 

    vector<string> sphe = {"R","Theta","Phi","Thickness"};

    for(unsigned int i = 0 ; i < blocks.size() ; i++){
        if(blocks[i]->HasTokenList(sphe)){
            if(NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "////  beam_dump " << i+1 <<  endl;
            double R = blocks[i]->GetDouble("R","mm");
            double Theta = blocks[i]->GetDouble("Theta","deg");
            double Phi = blocks[i]->GetDouble("Phi","deg");
            double Thickness = blocks[i]->GetDouble("Thickness", "micrometer");
            AddDetector(R,Theta,Phi,Thickness);
        }
        else{
            cout << "ERROR: check your input file formatting " << endl;
            exit(1);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::InitSpectra() {
    m_Spectra = new Tbeam_dumpSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::FillSpectra() {
    m_Spectra -> FillRawSpectra(m_EventData);
    m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
    m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::CheckSpectra() {
    m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::ClearSpectra() {
    // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> Tbeam_dumpPhysics::GetSpectra() {
    if(m_Spectra)
        return m_Spectra->GetMapHisto();
    else{
        map< string , TH1*> empty;
        return empty;
    }
}

///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::WriteSpectra() {
    m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::AddParameterToCalibrationManager() {
    CalibrationManager* Cal = CalibrationManager::getInstance();
    for (int i = 0; i < m_NumberOfDetectors; ++i) {
        Cal->AddParameter("beam_dump", "D"+ NPL::itoa(i+1)+"_ENERGY","beam_dump_D"+ NPL::itoa(i+1)+"_ENERGY");
        Cal->AddParameter("beam_dump", "D"+ NPL::itoa(i+1)+"_TIME","beam_dump_D"+ NPL::itoa(i+1)+"_TIME");
    }
}



///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::InitializeRootInputRaw() {
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchStatus("beam_dump",  true );
    inputChain->SetBranchAddress("beam_dump", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::InitializeRootInputPhysics() {
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchAddress("beam_dump", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void Tbeam_dumpPhysics::InitializeRootOutput() {
    TTree* outputTree = RootOutput::getInstance()->GetTree();
    outputTree->Branch("beam_dump", "Tbeam_dumpPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* Tbeam_dumpPhysics::Construct() {
    return (NPL::VDetector*) new Tbeam_dumpPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_beam_dump{
    public:
        proxy_beam_dump(){
            NPL::DetectorFactory::getInstance()->AddToken("beam_dump","beam_dump");
            NPL::DetectorFactory::getInstance()->AddDetector("beam_dump",Tbeam_dumpPhysics::Construct);
        }
};

proxy_beam_dump p_beam_dump;
}

