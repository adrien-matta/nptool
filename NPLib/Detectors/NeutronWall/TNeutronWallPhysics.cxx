/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : June 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold NeutronWall Treated  data                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TNeutronWallPhysics.h"

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

//   ROOT
#include "TChain.h"

ClassImp(TNeutronWallPhysics)


///////////////////////////////////////////////////////////////////////////
TNeutronWallPhysics::TNeutronWallPhysics()
   : m_EventData(new TNeutronWallData),
     m_PreTreatedData(new TNeutronWallData),
     m_EventPhysics(this),
     m_Spectra(0),
     m_E_RAW_Threshold(0), // adc channels
     m_E_Threshold(0),     // MeV
     m_NumberOfDetectors(0) {
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();

  // Neutron Wall
  for (UShort_t e = 0; e < m_PreTreatedData->GetMultEnergy(); e++) {
      if (m_PreTreatedData->GetE_DetectorNbr(e) == m_PreTreatedData->GetT_DetectorNbr(e)) {
        DetectorNumber.push_back(m_PreTreatedData->GetE_DetectorNbr(e));
        NW_PadNumber.push_back(m_PreTreatedData->GetE_PadNbr(e));
        NW_Energy.push_back(m_PreTreatedData->Get_Energy(e));
        NW_Time.push_back(m_PreTreatedData->Get_Time(e));
      }
  }


 // Veto Wall
  for (UShort_t e = 0; e < m_PreTreatedData->GetVetoMultEnergy(); e++) {
      if (m_PreTreatedData->GetE_VetoDetectorNbr(e) == m_PreTreatedData->GetT_VetoDetectorNbr(e)) {
	VW_PadNumber.push_back(m_PreTreatedData->GetE_VetoPadNbr(e));
        VW_Energy.push_back(m_PreTreatedData->Get_VetoEnergy(e));
        VW_Time.push_back(m_PreTreatedData->Get_VetoTime(e));
       }
  }

}

///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

	// Neutron Wall
  	// Energy
  	for (UShort_t i = 0; i < m_EventData->GetMultEnergy(); ++i) {
    		if (m_EventData->Get_Energy(i) > m_E_RAW_Threshold) {
      			Double_t EnergyNW = Cal->ApplyCalibration("NeutronWall/ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
      			if (EnergyNW > m_E_Threshold) {
        			m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i),m_EventData->GetE_PadNbr(i), EnergyNW);
      			} 
    		}
  	}
  	// Time 
  	for (UShort_t i = 0; i < m_EventData->GetMultTime(); ++i) {
    		Double_t TimeNW= Cal->ApplyCalibration("NeutronWall/TIME"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)),m_EventData->Get_Time(i));
    		m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i),m_EventData->GetT_PadNbr(i), TimeNW);
  	}


	// Veto Wall
	// Energy
	for (UShort_t i = 0; i < m_EventData->GetVetoMultEnergy(); ++i) {
    		if (m_EventData->Get_VetoEnergy(i) > m_E_RAW_Threshold) {
			Double_t EnergyVW = Cal->ApplyCalibration("VetoWall/ENERGY"+NPL::itoa(m_EventData->GetE_VetoPadNbr(i)),m_EventData->Get_VetoEnergy(i));
			
			if (EnergyVW > m_E_Threshold) {
        			m_PreTreatedData->SetVetoEnergy(m_EventData->GetE_VetoDetectorNbr(i),m_EventData->GetE_VetoPadNbr(i), EnergyVW);
      			}
		}	
	}
	
	//Time
	for (UShort_t i = 0; i < m_EventData->GetVetoMultTime(); ++i) {
    		Double_t TimeVW= Cal->ApplyCalibration("VetoWall/TIME"+NPL::itoa(m_EventData->GetT_VetoPadNbr(i)),m_EventData->Get_VetoTime(i));
		m_PreTreatedData->SetVetoTime(m_EventData->GetT_VetoDetectorNbr(i),m_EventData->GetT_VetoPadNbr(i), TimeVW);
  	}






}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::ReadConfiguration(string Path) {
  ifstream ConfigFile           ;
    ConfigFile.open(Path.c_str()) ;
    string LineBuffer          ;
    string DataBuffer          ;
    
    double Theta = 0 , Phi = 0 , R = 0 ;
    double X = 0 , Y = 0 , Z = 0 ;
    double Rot =0;
    int Bars = 0;
    double VWDistance = 0.0;
    int VetoWall = 0;
    double Overlap = 3;
    string NWMaterial;
    string VWMaterial;
    bool check_Theta = false ;
    bool check_Phi = false ;
    bool check_R = false ;
    bool check_rotation = false ;
    bool check_X = false ;
    bool check_Y = false ;
    bool check_Z = false ;
    bool ReadingStatus = false ;
    bool check_Bars = false ;
    bool check_NWMaterial = false ;
    
    while (!ConfigFile.eof()) {
        getline(ConfigFile, LineBuffer);
        
        //   If line is a Start Up NeutronWall bloc, Reading toggle to true
        string name = "NeutronWall";
        
        if (LineBuffer.compare(0, name.length(), name) == 0) {
            cout << "///" << endl           ;
            cout << "NeutronWall found: " << endl   ;
            ReadingStatus = true ;
        }
        
        //   Else don't toggle to Reading Block Status
        else ReadingStatus = false ;
        
        //   Reading Block
        while(ReadingStatus){
            // Pickup Next Word
            ConfigFile >> DataBuffer ;
            
            //   Comment Line
            if (DataBuffer.compare(0, 1, "%") == 0) {
                ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
            }
            
            //   Finding another telescope (safety), toggle out
            else if (DataBuffer.compare(0, name.length(),name) == 0) {
                cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
                ReadingStatus = false ;
            }
            
            //Angle method
            else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
                check_Theta = true;
                ConfigFile >> DataBuffer ;
                Theta = atof(DataBuffer.c_str()) ;
                Theta = Theta ;
                cout << "Theta:  " << Theta << endl;
            }
            
            else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
                check_Phi = true;
                ConfigFile >> DataBuffer ;
                Phi = atof(DataBuffer.c_str()) ;
                Phi = Phi;
                cout << "Phi:  " << Phi << endl;
            }
            
            else if (DataBuffer.compare(0, 2, "R=") == 0) {
                check_R = true;
                ConfigFile >> DataBuffer ;
                R = atof(DataBuffer.c_str()) ;
                R = R ;
                cout << "R:  " << R << endl;
            }
            
            //Position method
            else if (DataBuffer.compare(0, 2, "X=") == 0) {
                check_X = true;
                ConfigFile >> DataBuffer ;
                X = atof(DataBuffer.c_str()) ;
                X = X ;
                cout << "X:  " << X  << endl;
            }
            
            else if (DataBuffer.compare(0, 2, "Y=") == 0) {
                check_Y = true;
                ConfigFile >> DataBuffer ;
                Y = atof(DataBuffer.c_str()) ;
                Y = Y ;
                cout << "Y:  " << Y  << endl;
            }
            
            else if (DataBuffer.compare(0, 2, "Z=") == 0) {
                check_Z = true;
                ConfigFile >> DataBuffer ;
                Z = atof(DataBuffer.c_str()) ;
                Z = Z ;
                cout << "Z:  " << Z  << endl;
            }
            
            
            //General
            else if (DataBuffer.compare(0, 4, "Rot=") == 0) {
                check_rotation = true;
                ConfigFile >> DataBuffer ;
                Rot = atof(DataBuffer.c_str());
                Rot = Rot ;
                cout << "Rotation:  " << Rot << endl;
            }
            
            //Bar number
            else if (DataBuffer.compare(0, 5, "BARS=") == 0){
                check_Bars = true;
                ConfigFile >> DataBuffer ;
                Bars = atoi(DataBuffer.c_str()) ;
                cout << "Bars:  " << Bars << endl;
            }
            
            
            //Material type
            else if (DataBuffer.compare(0, 11, "NWMATERIAL=") == 0){
                check_NWMaterial = true;
                ConfigFile >> DataBuffer ;
                NWMaterial = DataBuffer;
                cout << "NWMaterials:  " << NWMaterial << endl;
            }
            
            //Distance
            else if (DataBuffer.compare(0, 11, "VWDISTANCE=") == 0){
                //check_VWDistance = true;
                ConfigFile >> DataBuffer ;
                VWDistance = atof(DataBuffer.c_str());
                VWDistance = VWDistance;
                cout << "VWDistance: " << VWDistance << endl;
            }
            
            //Decide whether to add the vetowall or not, 1 means yes, 0 means no
            else if (DataBuffer.compare(0, 9, "VETOWALL=") == 0){
                //check_VetoWall = true;
                ConfigFile >> DataBuffer ;
                VetoWall = atoi(DataBuffer.c_str());
                cout << "VetoWall:  " << VetoWall << endl;
            }
            
            //VetoWall Material
            else if (DataBuffer.compare(0, 11, "VWMATERIAL=") == 0){
                //check_VWMaterial = true;
                ConfigFile >> DataBuffer ;
                VWMaterial = DataBuffer ;
                cout << "VWMaterial:  " << VWMaterial << endl;
            }
            
            //Overlap
            else if (DataBuffer.compare(0, 8, "OVERLAP=") == 0){
                ConfigFile >> DataBuffer ;
                Overlap = atof(DataBuffer.c_str());
                Overlap = Overlap;
                cout << "Overlap:  " << Overlap << endl;
            }
            
            ///////////////////////////////////////////////////
            //   If no Detector Token and no comment, toggle out
            else{
                ReadingStatus = false;
                cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;
            }
            
            /////////////////////////////////////////////////
            //   If All necessary information there, toggle out
            
            if (( check_Theta && check_Phi && check_R && check_Bars && check_NWMaterial)
                ||
                ( check_X && check_Y && check_Z && check_rotation && check_Bars && check_NWMaterial)){
                
                
                // Convert Cartesian to Spherical (detector always face the target)
                if (check_X){
                    R = sqrt (X*X+Y*Y+Z*Z);
                    Theta = acos(Z / (R) );
                    Phi = atan2(Y,X);
                }
                
       
                
                //   Reinitialisation of Check Boolean
                check_Theta = false ;
                check_Phi = false ;
                check_R = false ;
                check_rotation = false ;
                check_X = false ;
                check_Y = false ;
                check_Z = false ;
                ReadingStatus = false ;
                check_Bars = false ;
                check_NWMaterial = false ;
                cout << "///"<< endl ;
            }
        }
    }

}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::Clear() {
  DetectorNumber.clear();
  NW_PadNumber.clear();
  NW_Energy.clear();
  NW_Time.clear();
  VW_PadNumber.clear();
  VW_Energy.clear();
  VW_Time.clear();

}


///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigNeutronWall.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigNeutronWall.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigNeutronWall.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigNeutronWall.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigNeutronWall";
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
void TNeutronWallPhysics::InitSpectra() {
  m_Spectra = new TNeutronWallSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TNeutronWallPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}



////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TNeutronWallPhysics::GetCanvas() {
  if(m_Spectra)
    return m_Spectra->GetCanvas();
  else{
    vector<TCanvas*> empty;
    return empty;
  }
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    Cal->AddParameter("NeutronWall", "D"+ NPL::itoa(i+1)+"_ENERGY","NeutronWall_D"+ NPL::itoa(i+1)+"_ENERGY");
    Cal->AddParameter("NeutronWall", "D"+ NPL::itoa(i+1)+"_TIME","NeutronWall_D"+ NPL::itoa(i+1)+"_TIME");
  }
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("NeutronWall",  true );
  inputChain->SetBranchAddress("NeutronWall", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("NeutronWall", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("NeutronWall", "TNeutronWallPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TNeutronWallPhysics::Construct() {
  return (NPL::VDetector*) new TNeutronWallPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_NeutronWall{
  public:
    proxy_NeutronWall(){
      NPL::DetectorFactory::getInstance()->AddToken("NeutronWall","NeutronWall");
      NPL::DetectorFactory::getInstance()->AddDetector("NeutronWall",TNeutronWallPhysics::Construct);
    }
};

proxy_NeutronWall p_NeutronWall;
}

