/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu     *
 *                                                                           *
 * Creation Date  : March 2018                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularCsI Treated  data                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TAnnularCsIPhysics.h"

//   STL
#include <cassert>
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

ClassImp(TAnnularCsIPhysics)


///////////////////////////////////////////////////////////////////////////
TAnnularCsIPhysics::TAnnularCsIPhysics()
   : m_EventData(new TAnnularCsIData),
     m_PreTreatedData(new TAnnularCsIData),
     m_EventPhysics(this),
     m_Spectra(0),
     m_E_RAW_Threshold(0), // adc channels
     m_E_Threshold(0)      // MeV
{
	;
}

///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::AddDetector(double R_min, double R_max,
																		 double Phi_min, double Phi_max,
																		 double Z) {
  // Add wedge spanning R_min -> R_max and Phi_min -> Phi_max
	double R = (R_max - R_min) / 2;
	double Phi = ((Phi_min - Phi_max) / 2);
	
	m_WedgePosition.push_back( TVector3(R*cos(Phi), R*sin(Phi), Z) );

	AnnularCsI_Utils::Geometry geo =
		{ R_min, R_max, R_max-R_min, Phi_min, Phi_max, Phi_max-Phi_min, Z };
	m_WedgeGeometry.push_back(geo);
} 
  
///////////////////////////////////////////////////////////////////////////
TVector3 TAnnularCsIPhysics::GetPositionOfInteraction(int i) const {
	return m_WedgePosition.at(DetectorNumber[i] - 1);
}

///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();

	// loop over energies
  unsigned int mysizeE = m_PreTreatedData->GetMultEnergy();
  for (UShort_t e = 0; e < mysizeE ; e++) {
		DetectorNumber.push_back(m_PreTreatedData->GetE_DetectorNbr(e));
		Energy.push_back(m_PreTreatedData->Get_Energy(e));
	}
}

///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::PreTreat() {
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
      Double_t Energy = Cal->ApplyCalibration("AnnularCsI/ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i), Energy);
      }
    }
  }

  // Time 
  mysize = m_EventData->GetMultTime();
  for (UShort_t i = 0; i < mysize; ++i) {
    Double_t Time= Cal->ApplyCalibration("AnnularCsI/TIME"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)),m_EventData->Get_Time(i));
    m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i), Time);
  }
}



///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigAnnularCsI.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigAnnularCsI.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigAnnularCsI.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigAnnularCsI.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigAnnularCsI";
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
int TAnnularCsIPhysics::MatchToSi(const TVector3& SiPosition) const {
	// return detector number of detector that spans the Si position
	// for when CsI backs an Si detector
	// returns -1 if no detector found

	// todo account for offset between detectors
	int i = 0;
	for (const auto& g : m_WedgeGeometry){
		if(SiPosition.Phi() >= g.Phi_min && SiPosition.Phi() <= g.Phi_max &&
			 SiPosition.Pt()  >= g.R_min   && SiPosition.Pt()  <= g.R_max) {
			// match!
			return i;
		}
		++i;
	}
	// no match
	return -1;
}


///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::Clear() {
  DetectorNumber.clear();
  Energy.clear();
  Time.clear();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::ReadConfiguration(NPL::InputParser parser) {
	vector<AnnularCsI_Utils::Geometry> detectors =
		AnnularCsI_Utils::ReadConfiguration(parser);
	for(const auto& d : detectors){
		AddDetector(d.R_min,d.R_max,d.Phi_min,d.Phi_max,d.Z);
	}
}

/*************************************************************************
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("AnnularCsI");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> wedge = {"R_MIN", "R_MAX", "PHI_MIN","PHI_MAX", "Z"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(wedge)){
      if(NPOptionManager::getInstance()->GetVerboseLevel()){
        cout << endl << "////  AnnularCsI " << i+1 <<  endl;
			}
			double R_min   = blocks[i]->GetDouble("R_MIN",   "mm");
			double R_max   = blocks[i]->GetDouble("R_MIN",   "mm");
			double Phi_min = blocks[i]->GetDouble("PHI_MIN", "deg");
			double Phi_max = blocks[i]->GetDouble("PHI_MAX", "deg");	
      double Z       = blocks[i]->GetDouble("Z",       "mm");
			
      AddDetector(R_min,R_max,Phi_min,Phi_max,Z);
    }
    else{
      cout << "ERROR (TAnnularCsIPhysics.cxx): "
					 << "check your input file formatting " << endl;
      exit(1);
    }
  }
}
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::InitSpectra() {
  m_Spectra = new TAnnularCsISpectra(GetNumberOfDetectors());
}



///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TAnnularCsIPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}

///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < GetNumberOfDetectors(); ++i) {
    Cal->AddParameter("AnnularCsI", "D"+ NPL::itoa(i+1)+"_ENERGY","AnnularCsI_D"+ NPL::itoa(i+1)+"_ENERGY");
    Cal->AddParameter("AnnularCsI", "D"+ NPL::itoa(i+1)+"_TIME","AnnularCsI_D"+ NPL::itoa(i+1)+"_TIME");
  }
}



///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("AnnularCsI",  true );
  inputChain->SetBranchAddress("AnnularCsI", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("AnnularCsI", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TAnnularCsIPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("AnnularCsI", "TAnnularCsIPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TAnnularCsIPhysics::Construct() {
  return (NPL::VDetector*) new TAnnularCsIPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_AnnularCsI{
  public:
    proxy_AnnularCsI(){
      NPL::DetectorFactory::getInstance()->AddToken("AnnularCsI","AnnularCsI");
      NPL::DetectorFactory::getInstance()->AddDetector("AnnularCsI",TAnnularCsIPhysics::Construct);
    }
};

proxy_AnnularCsI p_AnnularCsI;
}

