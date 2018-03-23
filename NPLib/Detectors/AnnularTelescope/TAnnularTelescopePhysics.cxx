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
 *  This class hold AnnularTelescope Treated  data                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TAnnularTelescopePhysics.h"

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
#include "NPSystemOfUnits.h"

//   ROOT
#include "TChain.h"

ClassImp(TAnnularTelescopePhysics)


///////////////////////////////////////////////////////////////////////////
TAnnularTelescopePhysics::TAnnularTelescopePhysics()
   : m_EventData(new TAnnularTelescopeData),
     m_PreTreatedData(new TAnnularTelescopeData),
     m_EventPhysics(this),
     m_Spectra(0),
     m_E_RAW_Threshold(0), // adc channels
     m_E_Threshold(0)      // MeV
{
	;
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::AddDetector(double R_min, double R_max,
																		 double Phi_min, double Phi_max,
																		 double Z) {
  // Add wedge spanning R_min -> R_max and Phi_min -> Phi_max
	double R = (R_max - R_min) / 2;
	double Phi = ((Phi_min - Phi_max) / 2);
	
	m_WedgePosition.push_back( TVector3(R*cos(Phi), R*sin(Phi), Z) );

	AnnularTelescope_Utils::Geometry geo =
		{ R_min, R_max, R_max-R_min, Phi_min, Phi_max, Phi_max-Phi_min, Z };
	m_WedgeGeometry.push_back(geo);
} 
  
///////////////////////////////////////////////////////////////////////////
TVector3 TAnnularTelescopePhysics::GetPositionOfInteraction(int i) const {
	return m_WedgePosition.at(DetectorNumber[i] - 1);
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::BuildPhysicalEvent() {
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
void TAnnularTelescopePhysics::PreTreat() {
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
      Double_t Energy = Cal->ApplyCalibration("AnnularTelescope/ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i), Energy);
      }
    }
  }

  // Time 
  mysize = m_EventData->GetMultTime();
  for (UShort_t i = 0; i < mysize; ++i) {
    Double_t Time= Cal->ApplyCalibration("AnnularTelescope/TIME"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)),m_EventData->Get_Time(i));
    m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i), Time);
  }
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigAnnularTelescope.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigAnnularTelescope.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigAnnularTelescope.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigAnnularTelescope.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigAnnularTelescope";
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
int TAnnularTelescopePhysics::MatchToSi(const TVector3& SiPosition) const {
	// return detector number of detector that spans the Si position
	// for when CsI backs an Si detector
	// returns -1 if no detector found

	// todo account for offset between detectors
	int hitno = 0;
	for(const auto& detno : DetectorNumber){
		const AnnularTelescope_Utils::Geometry& g = m_WedgeGeometry.at(detno - 1);
		if(SiPosition.Phi() >= g.Phi_min && SiPosition.Phi() < g.Phi_max &&
			 SiPosition.Pt()  >= g.R_min   && SiPosition.Pt()  < g.R_max   &&
			 fabs(SiPosition.Z()/NPUNITS::mm - g.Z/NPUNITS::mm) < 10) {
			//
			// Match!
			return hitno;
		}
		++hitno;
	}
	// no match
	return -1;
}


///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::Clear() {
  DetectorNumber.clear();
  Energy.clear();
  Time.clear();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::ReadConfiguration(NPL::InputParser parser) {
	vector<AnnularTelescope_Utils::Geometry> detectors =
		AnnularTelescope_Utils::ReadConfiguration(parser);
	for(const auto& d : detectors){
		AddDetector(d.R_min,d.R_max,d.Phi_min,d.Phi_max,d.Z);
	}
}


///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::InitSpectra() {
  m_Spectra = new TAnnularTelescopeSpectra(GetNumberOfDetectors());
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TAnnularTelescopePhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < GetNumberOfDetectors(); ++i) {
    Cal->AddParameter("AnnularTelescope", "D"+ NPL::itoa(i+1)+"_ENERGY","AnnularTelescope_D"+ NPL::itoa(i+1)+"_ENERGY");
    Cal->AddParameter("AnnularTelescope", "D"+ NPL::itoa(i+1)+"_TIME","AnnularTelescope_D"+ NPL::itoa(i+1)+"_TIME");
  }
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("AnnularTelescope",  true );
  inputChain->SetBranchAddress("AnnularTelescope", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("AnnularTelescope", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("AnnularTelescope", "TAnnularTelescopePhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TAnnularTelescopePhysics::Construct() {
  return (NPL::VDetector*) new TAnnularTelescopePhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_AnnularTelescope{
  public:
    proxy_AnnularTelescope(){
      NPL::DetectorFactory::getInstance()->AddToken("AnnularTelescope","AnnularTelescope");
      NPL::DetectorFactory::getInstance()->AddDetector("AnnularTelescope",TAnnularTelescopePhysics::Construct);
    }
};

proxy_AnnularTelescope p_AnnularTelescope;
}

