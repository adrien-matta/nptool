/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Cyril Lenain  contact address: lenain@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : octobre 2018                                             *
 * Last update    : 09/01/2019                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Vamos Treated  data                                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TVamosPhysics.h"
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
#include "NPDetectorManager.h"
#include "NPOptionManager.h"
#include "NPSystemOfUnits.h"

//   ROOT
#include "TChain.h"

ClassImp(TVamosPhysics)

    using namespace NPUNITS;

    ///////////////////////////////////////////////////////////////////////////
TVamosPhysics::TVamosPhysics()
    : m_EventData(new TVamosData),
    m_EventPhysics(this),
    m_NumberOfDetectors(0) {
    }

///////////////////////////////////////////////////////////////////////////
/// A usefull method to bundle all operation to add a detector
///////////////////////////////////////////////////////////////////////////
void TVamosPhysics::BuildSimplePhysicalEvent() {
    BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TVamosPhysics::BuildPhysicalEvent() {

    double DriftSpeed = 1*centimeter/microsecond;
    double Z_t = m_TargetThickness*0.5 ;     
    double E = 0 ;
    double DriftTime =0;
    double Raw_Y = 0;
    double t_para =0; 
    int DetectorNumber = 0;

    // match energy and time together
    unsigned int mysizeE = m_EventData->GetMultEnergy();
    unsigned int mysizeT = m_EventData->GetMultTime();
    for (UShort_t e = 0; e < mysizeE ; e++) {
        /* for (UShort_t t = 0; t < mysizeT ; t++) { */
        /* if (m_EventData->GetE_DetectorNbr(e) == m_EventData->GetT_DetectorNbr(t)) { */
        if (e!= (mysizeE-1)){
            E+=m_EventData->Get_Energy(e);

        }
        else {
            /* DetectorNumber.push_back(m_EventData->GetE_DetectorNbr(e)); */
            E+=m_EventData->Get_Energy(e);
            Energy.push_back(E);

            /* Time.push_back(m_EventData->Get_Time(t)); */

            /* } */
            /* } */
    }
    }

    // extract X and Y from DriftChamber 
    unsigned int sizeDrift = m_EventData->GetMultDrift(); 
    vector<double> X_Event, Y_Event;

    for (UShort_t d=0; d<sizeDrift ; d++){
        DetectorNumber = m_EventData->GetDrift_DetectorNbr(d);
        DriftDetectorNumber.push_back(DetectorNumber);
        Drift_X.push_back(m_EventData->Get_X(d));
        X_Event.push_back(m_EventData->Get_X(d));

        DriftTime = m_EventData->Get_DriftTime(d);
        
        /* cout<<7.5*centimeter/DriftSpeed<<endl; */

        // 7.5 centimeter is half times the height of Chamber 
        // to separate negative Y from positive Y
        if (DriftTime > 7.5*centimeter/DriftSpeed) {
            Raw_Y = (DriftTime-7.5*centimeter/DriftSpeed)*DriftSpeed;    
            Drift_Y.push_back(Raw_Y);
            Y_Event.push_back(Raw_Y);
        }

        else {
            Raw_Y = (7.5*centimeter/DriftSpeed-DriftTime)*DriftSpeed;
            Drift_Y.push_back(-Raw_Y);
            Y_Event.push_back(-Raw_Y);
        }

    }

    // calculate  
    if (sizeDrift >=2){

        double X1, X2, Y1, Y2, t, r; 
        double Y_t_Event, X_t_Event, Phi_t_Event, Theta_t_Event;

        X1=X_Event[0];
        Y1=Y_Event[0];
        X2=X_Event[1];
        Y2=Y_Event[1];

        // Calculate X,Y on the target
        t =(Z_t-Z1)/(Z2-Z1);
        X_t_Event = t*(X2-X1)+X1;
        Y_t_Event = t*(Y2-Y1)+Y1;

        // Theta and Phi out of the target
        r = sqrt(pow(X2-X1,2) + pow(Y2-Y1,2) + pow(Z2-Z1,2)); 
        Phi_t_Event = acos((Y2-Y1)/r);
        Theta_t_Event = acos((X2-X1)/(r*sin(Phi_t_Event)));

        X_target.push_back(X_t_Event);
        Y_target.push_back(Y_t_Event);

        Theta_target.push_back((Theta_t_Event*180)/3.14159);
        Phi_target.push_back((Phi_t_Event*180)/3.14159);

        // in anticipation to use macro
        TargetThickness.push_back(m_TargetThickness);

    }

    Y_Event.clear();
    X_Event.clear();

}

///////////////////////////////////////////////////////////////////////////
void TVamosPhysics::ReadAnalysisConfig() {
    bool ReadingStatus = false;

    // path to file
    string FileName = "./configs/ConfigVamos.dat";

    // open analysis config file
    ifstream AnalysisConfigFile;
    AnalysisConfigFile.open(FileName.c_str());

    if (!AnalysisConfigFile.is_open()) {
        cout << " No ConfigVamos.dat found: Default parameter loaded for Analayis " << FileName << endl;
        return;  
    }
    cout << " Loading user parameter for Analysis from ConfigVamos.dat " << endl;

    // Save it in a TAsciiFile
    TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
    asciiConfig->AppendLine("%%% ConfigVamos.dat %%%");
    asciiConfig->Append(FileName.c_str());
    asciiConfig->AppendLine("");
    // read analysis config file
    string LineBuffer,DataBuffer,whatToDo;
    while (!AnalysisConfigFile.eof()) {
        // Pick-up next line
        getline(AnalysisConfigFile, LineBuffer);

        // search for "header"
        string name = "ConfigVamos";
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
void TVamosPhysics::Clear() {
    DetectorNumber.clear();
    Energy.clear();
    Time.clear();
    DriftDetectorNumber.clear();
    Drift_X.clear();
    Drift_Y.clear();
    X_target.clear();
    Y_target.clear();
    Theta_target.clear();
    Phi_target.clear();
    TargetThickness.clear();
}

///////////////////////////////////////////////////////////////////////////
void TVamosPhysics::ReadConfiguration(NPL::InputParser parser) {

    vector<NPL::InputBlock*> blocks1= parser.GetAllBlocksWithToken("Target");
    if (NPOptionManager::getInstance()->GetVerboseLevel())

        for (unsigned int i=0; i< blocks1.size(); i++) {
            vector<string> TokenTarget = {"THICKNESS", "RADIUS","MATERIAL","ANGLE","X","Y","Z"}; 
            if (blocks1[i]->HasTokenList(TokenTarget)) {
                if (NPOptionManager::getInstance()->GetVerboseLevel())
                    cout << endl << "////  Target " << i + 1 << endl;
                m_TargetThickness   = blocks1[i]->GetDouble("THICKNESS", "micrometer");
            }
        }

    vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Vamos");
    if (NPOptionManager::getInstance()->GetVerboseLevel())
        cout << "//// " << blocks.size() << " detectors found " << endl;

    vector<string> TokenBeamCatcher
        = {"Material", "Width", "Length", "Thickness", "Pos"};
    vector<string> sphe        = {"R", "Theta"};
    vector<string> TokenMWPPAC = {"Z", "Gas", "Pressure", "Temperature"};
    vector<string> TokenDC     = {"Z", "Gas", "Pressure", "Temperature"};
    vector<string> TokenIC = {"Z", "Thickness", "Gas", "Pressure", "Temperature"};

    for (unsigned int i = 0; i < blocks.size(); i++) {

        if (blocks[i]->HasTokenList(sphe)) {
            if (NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "////  Vamos " << i + 1 << endl;
            double R     = blocks[i]->GetDouble("R", "mm");
            double Theta = blocks[i]->GetDouble("Theta", "deg");
            m_NumberOfDetectors++;
            DistanceTargetVamos = R;
        }

        else if (blocks[i]->GetMainValue() == "BeamCatcher"
                && blocks[i]->HasTokenList(TokenBeamCatcher)) {
            if (NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "//// BeamCatcher" << i + 1 << endl;
            string        Material  = blocks[i]->GetString("Material");
            double      Width     = blocks[i]->GetDouble("Width", "mm");
            double      Length    = blocks[i]->GetDouble("Length", "mm");
            double      Thickness = blocks[i]->GetDouble("Thickness", "mm");
            m_NumberOfDetectors++;
        }

        else if (blocks[i]->GetMainValue() == "MWPPAC"
                && blocks[i]->HasTokenList(TokenMWPPAC)) {
            if (NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "//// MWPPAC" << i + 1 << endl;
            double Z           = blocks[i]->GetDouble("Z", "mm");
            string   Gas         = blocks[i]->GetString("Gas");
            double Pressure    = blocks[i]->GetDouble("Pressure", "bar");
            double Temperature = blocks[i]->GetDouble("Temperature", "kelvin");
            m_NumberOfDetectors++;
        }

        else if (blocks[i]->GetMainValue() == "DC"
                && blocks[i]->HasTokenList(TokenDC)) {
            if (NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "//// DC" << i + 1 << endl;
            double Z           = blocks[i]->GetDouble("Z", "mm"); 
            string   Gas         = blocks[i]->GetString("Gas");
            double Pressure    = blocks[i]->GetDouble("Pressure", "bar");
            double Temperature = blocks[i]->GetDouble("Temperature", "kelvin");
            Z_DriftChambers.push_back(Z);
            m_NumberOfDetectors++;
        }

        else if (blocks[i]->GetMainValue() == "IC"
                && blocks[i]->HasTokenList(TokenIC)) {
            if (NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "//// IC" << i + 1 << endl;
            double Z           = blocks[i]->GetDouble("Z", "mm");
            double Thickness   = blocks[i]->GetDouble("Thickness", "mm");
            string   Gas         = blocks[i]->GetString("Gas");
            double Pressure    = blocks[i]->GetDouble("Pressure", "bar");
            double Temperature = blocks[i]->GetDouble("Temperature", "kelvin");
            m_NumberOfDetectors++;
        }

        else {
            cout << "ERROR: check your input file formatting " << endl;
            exit(1);
        }
    }

    Z1 = Z_DriftChambers[0]+ DistanceTargetVamos;
    Z2 = Z_DriftChambers[1]+ DistanceTargetVamos;
    /* OriginalBeamEnergy = myReaction.GetBeamEnergy(); */

    /* cout<<OriginalBeamEnergy <<endl; */

}

///////////////////////////////////////////////////////////////////////////
void TVamosPhysics::AddParameterToCalibrationManager() {
    CalibrationManager* Cal = CalibrationManager::getInstance();
    for (int i = 0; i < m_NumberOfDetectors; ++i) {
        Cal->AddParameter("Vamos", "D"+ NPL::itoa(i+1)+"_ENERGY","Vamos_D"+ NPL::itoa(i+1)+"_ENERGY");
        Cal->AddParameter("Vamos", "D"+ NPL::itoa(i+1)+"_TIME","Vamos_D"+ NPL::itoa(i+1)+"_TIME");
    }
}

///////////////////////////////////////////////////////////////////////////
void TVamosPhysics::InitializeRootInputRaw() {
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchStatus("Vamos",  true );
    inputChain->SetBranchAddress("Vamos", &m_EventData );
}

///////////////////////////////////////////////////////////////////////////
void TVamosPhysics::InitializeRootInputPhysics() {
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchAddress("Vamos", &m_EventPhysics);
}

///////////////////////////////////////////////////////////////////////////
void TVamosPhysics::InitializeRootOutput() {
    TTree* outputTree = RootOutput::getInstance()->GetTree();
    outputTree->Branch("Vamos", "TVamosPhysics", &m_EventPhysics);
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TVamosPhysics::Construct() {
    return (NPL::VDetector*) new TVamosPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
    class proxy_Vamos{
        public:
            proxy_Vamos(){
                NPL::DetectorFactory::getInstance()->AddToken("Vamos","Vamos");
                NPL::DetectorFactory::getInstance()->AddDetector("Vamos",TVamosPhysics::Construct);
            }
    };

    proxy_Vamos p_Vamos;
}

