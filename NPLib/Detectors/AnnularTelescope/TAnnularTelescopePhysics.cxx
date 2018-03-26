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
#include "TRandom3.h"

ClassImp(TAnnularTelescopePhysics)


///////////////////////////////////////////////////////////////////////////
TAnnularTelescopePhysics::TAnnularTelescopePhysics()
   : m_EventData(new TAnnularTelescopeData),
     m_PreTreatedData(new TAnnularTelescopeData),
     m_EventPhysics(this),
     m_Spectra(0)
{
	;
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::AddDetector(const AnnularTelescope_Utils::Geometry& g){
	m_WedgeGeometry.push_back(g);
} 
  
///////////////////////////////////////////////////////////////////////////
TVector3 TAnnularTelescopePhysics::GetPositionOfInteraction(int i) const {
	try {
		const AnnularTelescope_Utils::Geometry& geo = m_WedgeGeometry.at(i);
		Int_t detector = SiHit.Detector.at(i);
		Int_t theta_n  = SiHit.ThetaStrip.at(i);
		Int_t phi_n    = SiHit.PhiStrip.at(i);

		TVector3 position;
		// set position on the wafer
		position.SetPtThetaPhi(geo.Si_Strip_Theta_Radius.at(theta_n), 0,
													 geo.Si_Strip_Phi_Angle.at(phi_n) );
		// then translate to the Z-position (from target)
		position.SetZ(geo.Z);
		return position;
	} catch(std::exception& e) {
		cerr << "ERROR: GetPositionOfInteration: invalid index " << i
				 << "Returning (0,0,0)";
		return TVector3(0,0,0);
	}
}

///////////////////////////////////////////////////////////////////////////
TVector3 TAnnularTelescopePhysics::GetRandomisedPositionOfInteraction(int i) const {
	try {
		TVector3 pos = GetPositionOfInteraction(i);
		double R = pos.Pt();
		double Phi = pos.Phi();

		double R_pitch = m_WedgeGeometry.at(i).Si_Theta_Radius_Pitch;
		R += gRandom->Uniform(-R_pitch/2., +R_pitch/2.);

		double Phi_pitch = m_WedgeGeometry.at(i).Si_Phi_Angle_Pitch;
		double Arc_len = R*Phi_pitch;
		double Arc_pos = R*Phi;
		Arc_pos += gRandom->Uniform(-Arc_len/2., +Arc_len/2.);
		Phi = Arc_pos / R;

		TVector3 position;
		// set position on the wafer
		position.SetPtThetaPhi(R, 0, Phi);
		// then translate to the Z-position (from target)
		position.SetZ(pos.Z());
		return position;
	} catch(std::exception& e) {
		cerr << "ERROR: GetRandomisedPositionOfInteration: invalid index " << i
				 << "Returning (0,0,0)";
		return TVector3(0,0,0);
	}
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();
	// build CsI event
	BuildCsIEvent();
	// build Si event
	BuildSiEvent();
	// build total (Si + CsI event)
	BuildTotalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::BuildCsIEvent() {
	// loop over energies
  size_t mysizeE = m_PreTreatedData->GetCsIMultEnergy();
  for (size_t i = 0; i < mysizeE ; ++i) {
		CsIHit.Detector.push_back(
			m_PreTreatedData->GetCsIE_DetectorNbr(i) );
		CsIHit.Wedge.push_back(
			m_PreTreatedData->GetCsIE_WedgeNbr(i) );
		CsIHit.Energy.push_back(
			m_PreTreatedData->GetCsIEnergy(i) );

		// now look for matching times
		double time = -1000;
		size_t mysizeT = m_PreTreatedData->GetCsIMultTime();
		for(size_t j=0; j< mysizeT; ++j) {
			size_t detT = m_PreTreatedData->GetCsIT_DetectorNbr(j);
			size_t wedgeT = m_PreTreatedData->GetCsIT_WedgeNbr(j);
			if(CsIHit.Detector.at(i) == detT &&
				 CsIHit.Wedge.at(i) == wedgeT) {
				// Match - set time
				time = m_PreTreatedData->GetCsITime(j);
				break;
			}
		}
		// add (real or dummy) time
		CsIHit.Time.push_back(time);
	}
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::BuildSiEvent() {

#ifdef DUMMY_
	SiHit
	std::vector<Int_t>    Detector;
	std::vector<Int_t>    ThetaStrip;
	std::vector<Int_t>    PhiStrip;
	std::vector<Double_t> ThetaStripEnergy;
	std::vector<Double_t> PhiStripEnergy;
	std::vector<Double_t> ThetaStripTime;
	std::vector<Double_t> PhiStripTime;
#endif
	
	// loop over theta strip energies
	const size_t mysizeE = m_PreTreatedData->GetSiThetaE_Mult();
	for (size_t iThetaE = 0; iThetaE < mysizeE; ++iThetaE) {
		//
		// Add theta strip energy info
		SiHit.Detector.push_back(
			m_PreTreatedData->GetSiThetaE_DetectorNbr(iThetaE) );
		SiHit.ThetaStrip.push_back(
			m_PreTreatedData->GetSiThetaE_StripNbr(iThetaE) );
		SiHit.ThetaStripEnergy.push_back(
			m_PreTreatedData->GetSiThetaE_Energy(iThetaE) );
		//
		// Check for corresponding time
		Double_t timeTheta = -1000;
		const size_t mysizeT = m_PreTreatedData->GetSiThetaT_Mult();
		for (size_t iThetaT = 0; iThetaT < mysizeT; ++iThetaT) {
			size_t detT = m_PreTreatedData->GetSiThetaT_DetectorNbr(iThetaT);
			size_t thT = m_PreTreatedData->GetSiThetaT_StripNbr(iThetaT);
			if(SiHit.Detector.at(iThetaE) == detT &&
				 SiHit.ThetaStrip.at(iThetaE) == thT) {
				// Match - set time
				timeTheta = m_PreTreatedData->GetSiThetaT_Time(iThetaT);
				break;
			}
		}
		// add (real or dummy) time
		SiHit.ThetaStripTime.push_back(timeTheta);
		//
		// Now check for phi match
		Int_t phiStrip = -1000;
		Double_t phiEnergy = -1000;
		Double_t timePhi = -1000;
		// Check for match in energy window
		const size_t mysizePhiE = m_PreTreatedData->GetSiPhiE_Mult();
		for (size_t iPhiE = 0; iPhiE < mysizePhiE; ++iPhiE) {
			size_t det = m_PreTreatedData->GetSiPhiE_DetectorNbr(iPhiE);
			Double_t e = m_PreTreatedData->GetSiPhiE_Energy(iPhiE);
			Int_t N_SIGMA = 5;
			Double_t SIGMA = 0.0149;
			if(SiHit.Detector.at(iThetaE) == det &&
				 fabs(SiHit.ThetaStripEnergy.at(iThetaE) - e) < SIGMA*N_SIGMA) {
				// Match!
				phiStrip = m_PreTreatedData->GetSiPhiE_StripNbr(iPhiE);
				phiEnergy = m_PreTreatedData->GetSiPhiE_Energy(iPhiE);
				//
				// Now see if we have a time
				const size_t mysizePhiT = m_PreTreatedData->GetSiPhiT_Mult();
				for (size_t iPhiT = 0; iPhiT < mysizePhiT; ++iPhiT) {
					size_t det = m_PreTreatedData->GetSiPhiT_DetectorNbr(iPhiT);
					size_t phT = m_PreTreatedData->GetSiPhiT_StripNbr(iPhiT);
					if(SiHit.Detector.at(iPhiE) == det &&
						 SiHit.ThetaStrip.at(iPhiE) == phT) {
						// Match - set time
						timePhi = m_PreTreatedData->GetSiPhiT_Time(iPhiT);
						break;
					}
				}
				break; // Done - phi energy match found
			}
		}
		// add (real or dummy) phi info
		SiHit.PhiStrip.push_back(phiStrip);
		SiHit.PhiStripEnergy.push_back(phiEnergy);
		SiHit.PhiStripTime.push_back(timePhi);
	}
}


///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::BuildTotalEvent(){
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::PreTreat() {
/********************
 TODO:: NEED TO CALIBRATE AND APPLY THRESHOLDS
 (BUT NOT FOR SIMULATION).
 Here is some example code for calibration

  // instantiate CalibrationManager
  static CalibrationManager* Cal = 
		CalibrationManager::getInstance();
		
   Double_t Energy =
			Cal->ApplyCalibration(
				"AnnularTelescope/CSI_ENERGY"+NPL::itoa(
					m_EventData->GetCsIE_DetectorNbr(i)  ),
				m_EventData->GetCsIEnergy(i) );
*********************/

	
	// clear pre-treated object
	ClearPreTreatedData();

	// CsI energy
	for(size_t i=0; i< m_EventData->GetCsIMultEnergy(); ++i){
		m_PreTreatedData->SetCsIEnergy(
			m_PreTreatedData->GetCsIE_DetectorNbr(i),
			m_PreTreatedData->GetCsIE_WedgeNbr(i),
			m_PreTreatedData->GetCsIEnergy(i) );
	}

	// CsI time
	for(size_t i=0; i< m_EventData->GetCsIMultTime(); ++i){
		m_PreTreatedData->SetCsITime(
			m_PreTreatedData->GetCsIT_DetectorNbr(i),
			m_PreTreatedData->GetCsIT_WedgeNbr(i),
			m_PreTreatedData->GetCsITime(i) );
	}

	// Si energy (theta strips)
	for(size_t i=0; i< m_EventData->GetSiThetaE_Mult(); ++i){
		m_PreTreatedData->SetSiThetaEnergy(
			m_PreTreatedData->GetSiThetaE_DetectorNbr(i),
			m_PreTreatedData->GetSiThetaE_StripNbr(i),
			m_PreTreatedData->GetSiThetaE_Energy(i) );
	}

	// Si time (theta strips)
	for(size_t i=0; i< m_EventData->GetSiThetaT_Mult(); ++i){
		m_PreTreatedData->SetSiThetaTime(
			m_PreTreatedData->GetSiThetaT_DetectorNbr(i),
			m_PreTreatedData->GetSiThetaT_StripNbr(i),
			m_PreTreatedData->GetSiThetaT_Time(i) );
	}

	// Si energy (phi strips)
	for(size_t i=0; i< m_EventData->GetSiPhiE_Mult(); ++i){
		m_PreTreatedData->SetSiPhiEnergy(
			m_PreTreatedData->GetSiPhiE_DetectorNbr(i),
			m_PreTreatedData->GetSiPhiE_StripNbr(i),
			m_PreTreatedData->GetSiPhiE_Energy(i) );
	}

	// Si time (phi strips)
	for(size_t i=0; i< m_EventData->GetSiPhiT_Mult(); ++i){
		m_PreTreatedData->SetSiPhiTime(
			m_PreTreatedData->GetSiPhiT_DetectorNbr(i),
			m_PreTreatedData->GetSiPhiT_StripNbr(i),
			m_PreTreatedData->GetSiPhiT_Time(i) );
	}
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::ReadAnalysisConfig() {
	// TODO: Implement!
/*********************
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
**************************/
}

// ///////////////////////////////////////////////////////////////////////////
// int TAnnularTelescopePhysics::MatchToSi(const TVector3& SiPosition) const {
// 	// return detector number of detector that spans the Si position
// 	// for when CsI backs an Si detector
// 	// returns -1 if no detector found

// 	// todo account for offset between detectors
// 	int hitno = 0;
// 	for(const auto& detno : DetectorNumber){
// 		const AnnularTelescope_Utils::Geometry& g = m_WedgeGeometry.at(detno - 1);
// 		if(SiPosition.Phi() >= g.Phi_min && SiPosition.Phi() < g.Phi_max &&
// 			 SiPosition.Pt()  >= g.R_min   && SiPosition.Pt()  < g.R_max   &&
// 			 fabs(SiPosition.Z()/NPUNITS::mm - g.Z/NPUNITS::mm) < 10) {
// 			//
// 			// Match!
// 			return hitno;
// 		}
// 		++hitno;
// 	}
// 	// no match
// 	return -1;
// }


///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::Clear() {

	CsIHit.Detector.clear();
	CsIHit.Wedge.clear();
	CsIHit.Energy.clear();
	CsIHit.Time.clear();

	SiHit.Detector.clear();
	SiHit.ThetaStrip.clear();
	SiHit.PhiStrip.clear();
	SiHit.ThetaStripEnergy.clear();
	SiHit.PhiStripEnergy.clear();
	SiHit.ThetaStripTime.clear();
	SiHit.PhiStripTime.clear();

	Hit.Si_Energy.clear();
	Hit.Si_Time.clear();
	Hit.CsI_Energy.clear();
	Hit.CsI_Time.clear();
	Hit.Position.clear();
}



///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::ReadConfiguration(NPL::InputParser parser) {
	vector<AnnularTelescope_Utils::Geometry> geometries =
		AnnularTelescope_Utils::ReadConfiguration(parser);
	for(const auto& geo : geometries){ 	AddDetector(geo);  }
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

