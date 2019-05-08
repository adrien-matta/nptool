/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Dustin Scriven  contact address: scrivend@tamu.edu                        *
 *                                                                           *
 * Creation Date  : July 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TNT Treated  data                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TTNTPhysics.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
#include <map>
#include <vector>
using namespace std;

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"

//   ROOT
#include "TChain.h"

ClassImp(TTNTPhysics)


///////////////////////////////////////////////////////////////////////////
TTNTPhysics::TTNTPhysics()
   : m_EventData(new TTNTData),
     m_PreTreatedData(new TTNTData),
     m_EventPhysics(this),
     m_Spectra(0),
     m_E_RAW_Threshold(0), // adc channels
     m_E_Threshold(0),     // MeV
     m_NumberOfDetectors(0) {
}

///////////////////////////////////////////////////////////////////////////
/// A usefull method to bundle all operation to add a detector
void TTNTPhysics::AddDetector(TVector3 , string){
  //cout << "check1" << endl;
  // In That simple case nothing is done
  // Typically for more complex detector one would calculate the relevant
  // positions (stripped silicon) or angles (gamma array)
  m_NumberOfDetectors++;
  //cout << "m_NumberofDetectors is: " << m_NumberOfDetectors << endl;
  //cout <<"test1" << endl;
}

///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::AddDetector(double R, double Theta, double Phi, string shape){
  // Compute the TVector3 corresponding
  cout << "check2" << endl;
  TVector3 Pos(R*sin(Theta)*cos(Phi),R*sin(Theta)*sin(Phi),R*cos(Theta));
  // Call the cartesian method
  AddDetector(Pos,shape);
}

///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();
  const double PI = 3.141592653589793;
  // match energy and time together
  unsigned int mysizeE = m_PreTreatedData->GetMultEnergy();
  unsigned int mysizeT = m_PreTreatedData->GetMultTime();
  for (ULong_t e = 0; e < mysizeE ; e++) {
    for (ULong_t t = 0; t < mysizeT ; t++) {
      if (m_PreTreatedData->GetE_DetectorNbr(e) == m_PreTreatedData->GetT_DetectorNbr(t)) {
        DetectorNumber.push_back(m_PreTreatedData->GetE_DetectorNbr(e));
        Energy.push_back(m_PreTreatedData->Get_Energy(e));
        Time.push_back(m_PreTreatedData->Get_Time(t));
        	// GAC -> Calculate positions
		// Here we are doing this event-by-event
		// The logic is:
		//   1. Read detector number from the Raw tree
		//   2. Calculate position from map of detector number -> {x,y,z}
		//   3. Store x,y,z positions in Xpos,Ypos,Zpos vectors
	int detnumber = m_PreTreatedData->GetE_DetectorNbr(e);
	auto info = DetectorInfo.find(detnumber);
	if(info == DetectorInfo.end()) { // detector number NOT FOUND
  	  std::cerr << "ERROR (fatal): coundn't find detector number: " << detnumber
	    << " in position map!\n";
	    exit(1);
	  }
	//assert(info->second.size() == 3); // error check that the position vector is correct size
	double xpos = info->second[0];
        double ypos = info->second[1];
        double zpos = info->second[2];

        Xpos.push_back(info->second[0]);
	Ypos.push_back(info->second[1]);
	Zpos.push_back(info->second[2]);

        Theta.push_back(atan(info->second[0]/info->second[2])*180/PI);
        Phi.push_back(atan(info->second[1]/info->second[2])*180/PI);
        Theta_Lab.push_back(atan((pow(pow(info->second[0],2)+pow(info->second[1],2),0.5))/info->second[2])*180/PI);
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // Energy
  ULong_t mysize = m_EventData->GetMultEnergy();
  for (ULong_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = m_EventData->Get_Energy(i);   //Cal->ApplyCalibration("TNT/ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i), Energy);
      }
    }
  }

  // Time
  mysize = m_EventData->GetMultTime();
  for (ULong_t i = 0; i < mysize; ++i) {
    Double_t Time= m_EventData->Get_Time(i); //Cal->ApplyCalibration("TNT/TIME"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)),m_EventData->Get_Time(i));
    m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i), Time);
  }
}



///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  cout << "check3" << endl;
  string FileName = "./configs/ConfigTNT.dat";
  cout << "test3" << endl;
  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigTNT.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigTNT.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigTNT.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  cout << "check4" << endl;
  string LineBuffer,DataBuffer,whatToDo;
  cout << "Test4" << endl;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    cout << "check5" << endl;
    string name = "ConfigTNT";
    cout << "Test5" << endl;
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
void TTNTPhysics::Clear() {
  DetectorNumber.clear();
  Energy.clear();
  Time.clear();
  Xpos.clear();
  Ypos.clear();
  Zpos.clear();
  Theta.clear();
  Phi.clear();
  Theta_Lab.clear();
}



///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("TNT");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl;
  cout << "check6" << endl;
  vector<string> cart = {"POS","VOXNUM","VOXSIZE","Shape"};
  vector<string> sphe = {"R","Theta","Phi","Shape"};
  cout << "test6" << endl;
  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  TNT " << i+1 <<  endl;

      TVector3 Pos = blocks[i]->GetTVector3("POS","mm");
      vector<int> vnumber = blocks[i]->GetVectorInt("VOXNUM");
      if((vnumber.size() != 3) || (vnumber[0]+vnumber[1]+vnumber[2] < 3)) {
        cout << "Check TNT.detector. Insure VOXNUM has three values >= 1" << endl;
        exit(1); // this exit method is not favored among C++ programmers!
        }
      TVector3 VoxNum(vnumber.at(0),vnumber.at(1),vnumber.at(2));
      TVector3 VoxWidth = blocks[i]->GetTVector3("VOXSIZE","mm");
      cout << "check7" << endl;
      string Shape = blocks[i]->GetString("Shape");
      cout << "test7" << endl;
      vector<int> WLS = blocks[i]->GetVectorInt("WLS");
      int WLSToggle = WLS[0];
      unsigned short wlsSize = WLS[1];

      double Widthx = VoxWidth[0]; // x-width of individual cell
      double Widthy = VoxWidth[1]; // y-width
      double Widthz = VoxWidth[2]; // z-width

      int VoxNumx = VoxNum[0];     // number of voxels placed in x-dim
      int VoxNumy = VoxNum[1];     // y-dim
      int VoxNumz = VoxNum[2];     // z-dim

      // Shift array off center to build geometry
      Pos[0] -= Widthx*VoxNumx/2 - Widthx/2;
      Pos[1] -= Widthy*VoxNumy/2 - Widthy/2;
      Pos[2] -= Widthz*VoxNumz/2 - Widthz/2;

      // Store original offset of detector in dummy vars
      int dummyy = Pos[1];
      int dummyz = Pos[2];
      int detnumber = 0;

      // Add WLS spacing
      if(WLSToggle == false) {
        wlsSize = 0;
        }

      // Build detector array
      for (int l=1; l<=VoxNumx; l++) {
	detnumber += 1;
        vector<double> detcoord = {Pos[0],Pos[1],Pos[2]};
        DetectorInfo[detnumber] = detcoord;
        AddDetector(Pos,Shape);
        for (int m=1; m<=VoxNumy; m++) {
          if (m != 1) {
	    detnumber += 1;
            Pos[1] += Widthy;
            vector<double> detcoord = {Pos[0],Pos[1],Pos[2]};
            DetectorInfo[detnumber] = detcoord;
            AddDetector(Pos,Shape);
            }
          for (int n=1; n<=VoxNumz; n++) {
            if (n != 1) {
	      detnumber += 1;
              Pos[2] += Widthz + wlsSize;
              vector<double> detcoord = {Pos[0],Pos[1],Pos[2]};
              DetectorInfo[detnumber] = detcoord;
              AddDetector(Pos,Shape);
              }
            Pos[2] = dummyz;
            }
          }
        Pos[1] = dummyy;
        Pos[0] += Widthx;
        }
      cout << "Detector number is: " << detnumber << endl;
      }

    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  TNT " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      cout << "check8" << endl;
      string Shape = blocks[i]->GetString("Shape");
      cout << "test8" << endl;
      AddDetector(R,Theta,Phi,Shape);
      }
    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::InitSpectra() {
  //m_Spectra = new TTNTSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::FillSpectra() {
  //m_Spectra -> FillRawSpectra(m_EventData);
  //m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  //m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::CheckSpectra() {
  //m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TTNTPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    cout << "check9" << endl;
    map< string , TH1*> empty;
    cout << "test9" << endl;
    return empty;
  }
}

///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::WriteSpectra() {
  //m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::AddParameterToCalibrationManager() {
//  CalibrationManager* Cal = CalibrationManager::getInstance();
//  for (int i = 0; i < m_NumberOfDetectors; ++i) {
//    Cal->AddParameter("TNT", "D"+ NPL::itoa(i+1)+"_ENERGY","TNT_D"+ NPL::itoa(i+1)+"_ENERGY");
//    Cal->AddParameter("TNT", "D"+ NPL::itoa(i+1)+"_TIME","TNT_D"+ NPL::itoa(i+1)+"_TIME");
//  }
}



///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("TNT",  true );
  inputChain->SetBranchAddress("TNT", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("TNT", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TTNTPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("TNT", "TTNTPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TTNTPhysics::Construct() {
  cout << "Hello" << endl;
  return (NPL::VDetector*) new TTNTPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_TNT{
  public:
    proxy_TNT(){
      NPL::DetectorFactory::getInstance()->AddToken("TNT","TNT");
      NPL::DetectorFactory::getInstance()->AddDetector("TNT",TTNTPhysics::Construct);
    }
};

proxy_TNT p_TNT;
}
