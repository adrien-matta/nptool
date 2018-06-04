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
using namespace NPUNITS;

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
	m_EnergyMatch.NSigma = 5;
	m_EnergyMatch.Sigma = 0.0149;
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::AddDetector(const AnnularTelescope_Utils::Geometry& g){
	m_WedgeGeometry.push_back(g);
} 
  
///////////////////////////////////////////////////////////////////////////
TVector3 TAnnularTelescopePhysics::GetPositionOfInteraction(int i) const {
	try {
		const Int_t detector = m_SiHits.at(i).Detector;
		const Int_t theta_n  = m_SiHits.at(i).ThetaStrip;
		const Int_t phi_n    = m_SiHits.at(i).PhiStrip;
		const AnnularTelescope_Utils::Geometry& geo =
		 	m_WedgeGeometry.at(detector - 1);

		TVector3 position(0,0,0);
		if(theta_n > 0 && phi_n > 0) { // check we have both strips
			// set position on the wafer
			position.SetPtThetaPhi(geo.Si_Strip_Theta_Radius.at(theta_n - 1), 0,
														 geo.Si_Strip_Phi_Angle.at(phi_n - 1) );
			// then translate to the Z-position (from target)
			position.SetZ(geo.Z);
		}
		return position;
	} catch(std::exception& e) {
		std::cerr	<< "RANGE ERROR: GetPositionOfInteraction\n";
		exit(1);
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

		TVector3 position(R*cos(Phi), R*sin(Phi), pos.Z());
		if(pos.Z() == 0) { position.SetXYZ(0,0,0); } // no phi strip
		return position;
	} catch(std::exception& e) {
		cerr << "RANGE ERROR: GetRandomisedPositionOfInteration\n";
		exit(1);
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
	// // build total (Si + CsI event)
	BuildTotalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::BuildCsIEvent() {
	// Loop over raw hits
  size_t mysizeE = m_PreTreatedData->GetCsIMultEnergy();
  for (size_t i = 0; i < mysizeE ; ++i) {
		// Read detector, wedge no, energy
		const Int_t det = m_PreTreatedData->GetCsIE_DetectorNbr(i);
		const Int_t wedge = m_PreTreatedData->GetCsIE_WedgeNbr(i);
		const Double_t energy = m_PreTreatedData->GetCsIEnergy(i);
		//
		// Look for matching times
		double time = -1000;
		size_t mysizeT = m_PreTreatedData->GetCsIMultTime();
		for(size_t j=0; j< mysizeT; ++j) {
			const size_t detT = m_PreTreatedData->GetCsIT_DetectorNbr(j);
			const size_t wedgeT = m_PreTreatedData->GetCsIT_WedgeNbr(j);
			if(det == detT && wedge == wedgeT) {
				// Match - set time
				time = m_PreTreatedData->GetCsITime(j);
				break;
			}
		}
		// add event
		SetCsIHit(det,wedge,energy,time);
	}
}

///////////////////////////////////////////////////////////////////////////
void TAnnularTelescopePhysics::BuildSiEvent() {
	// loop over theta strip energies
	const size_t mysizeE = m_PreTreatedData->GetSiThetaE_Mult();
	for (size_t iThetaE = 0; iThetaE < mysizeE; ++iThetaE) {
		//
		// Add theta strip energy info
		const Int_t& det = m_PreTreatedData->GetSiThetaE_DetectorNbr(iThetaE);
		const Int_t& thetaStrip = m_PreTreatedData->GetSiThetaE_StripNbr(iThetaE);
		const Double_t& thetaEnergy = m_PreTreatedData->GetSiThetaE_Energy(iThetaE);
		//
		// Check for corresponding time
		Double_t timeTheta = -1000;
		const size_t mysizeT = m_PreTreatedData->GetSiThetaT_Mult();
		for (size_t iThetaT = 0; iThetaT < mysizeT; ++iThetaT) {
			size_t detT = m_PreTreatedData->GetSiThetaT_DetectorNbr(iThetaT);
			size_t thT = m_PreTreatedData->GetSiThetaT_StripNbr(iThetaT);
			if(det == detT && thetaStrip == thT) {
				// Match - set time
				timeTheta = m_PreTreatedData->GetSiThetaT_Time(iThetaT);
				break;
			}
		}
		//
		// Now check for phi match
		Int_t phiStrip = -1000;
		Double_t phiEnergy = -1000;
		Double_t timePhi = -1000;
		// Check for match in energy window
		const size_t mysizePhiE = m_PreTreatedData->GetSiPhiE_Mult();
		for (size_t iPhiE = 0; iPhiE < mysizePhiE; ++iPhiE) {
			size_t detPhi = m_PreTreatedData->GetSiPhiE_DetectorNbr(iPhiE);
			Double_t e = m_PreTreatedData->GetSiPhiE_Energy(iPhiE);
			if(det == detPhi &&
				 fabs(thetaEnergy - e) < m_EnergyMatch.NSigma*m_EnergyMatch.Sigma)
			{
				// Match!
				phiStrip = m_PreTreatedData->GetSiPhiE_StripNbr(iPhiE);
				phiEnergy = e;
				//
				// Now see if we have a time
				const size_t mysizePhiT = m_PreTreatedData->GetSiPhiT_Mult();
				for (size_t iPhiT = 0; iPhiT < mysizePhiT; ++iPhiT) {
					const size_t detPhiT = m_PreTreatedData->GetSiPhiT_DetectorNbr(iPhiT);
					const size_t phiStripT = m_PreTreatedData->GetSiPhiT_StripNbr(iPhiT);
					if(det == detPhiT &&  phiStrip == phiStripT) {
						// Match - set time
						timePhi = m_PreTreatedData->GetSiPhiT_Time(iPhiT);
						break;
					}
				}
				break; // Done - phi energy match found
			}
		}
		// Set hit info
		SetSiHit(det,thetaStrip,phiStrip,thetaEnergy,phiEnergy,timeTheta,timePhi);
	}
}


///////////////////////////////////////////////////////////////////////////
namespace { inline bool match_si_csi(
	double siPitch, double siAngle, double csiPitch, double csiAngle) {
	const double siLow   = siAngle - siPitch/2.;
	const double siHigh  = siAngle + siPitch/2.;
	const double csiLow  = csiAngle - csiPitch/2.;
	const double csiHigh = csiAngle + csiPitch/2.;

	if ( (siLow  >= csiLow && siLow  < csiHigh) ||
			 (siHigh >= csiLow && siHigh < csiLow) ) {
		return true;
	}
	else {
		return false;
	}
}

class SiCsIMatch {
public:
	SiCsIMatch(const SiHit_t& siHit, const AnnularTelescope_Utils::Geometry& geo, bool usePitch = true):
		m_SiHit(siHit), m_Geo(geo), m_UsePitch(usePitch) { }
	bool operator() (const CsIHit_t& csiHit) {
		if(csiHit.Detector != m_SiHit.Detector) { return false; }
		if(csiHit.Wedge < 0)   { return false; }
		if(m_SiHit.PhiStrip < 0) { return false; }
		if(m_UsePitch) {
			return match_si_csi(
				m_Geo.Si_Phi_Angle_Pitch, m_Geo.Si_Strip_Phi_Angle.at(m_SiHit.PhiStrip-1),
				m_Geo.CsI_Wedge_Angle_Pitch, m_Geo.CsI_Wedge_Phi_Angle.at(csiHit.Wedge-1) );		
		}
		else { // no si pitch (except for f.p. errors)
			return match_si_csi(
				1e-4, m_Geo.Si_Strip_Phi_Angle.at(m_SiHit.PhiStrip-1),
				m_Geo.CsI_Wedge_Angle_Pitch, m_Geo.CsI_Wedge_Phi_Angle.at(csiHit.Wedge-1) );					
		}
	}
private:
	const SiHit_t& m_SiHit;
	const AnnularTelescope_Utils::Geometry& m_Geo;
	bool m_UsePitch;
};

}
void TAnnularTelescopePhysics::BuildTotalEvent(){
	Int_t iSi = 0;
	for(const auto& hitSi : m_SiHits) {
		// Skip events w/o phi strip number
		if(hitSi.PhiStrip < 0) { 
			continue;
		}
		//
		// Add Si energy, time
		const Double_t& siEnergy = hitSi.ThetaStripEnergy;
		const Double_t& siTime = hitSi.ThetaStripTime;
		TVector3 SiPos = GetPositionOfInteraction(iSi);
		TVector3 SiPos_R = GetRandomisedPositionOfInteraction(iSi);
		//
		// Match CsI wedge and detector (by phi angle)
		const	AnnularTelescope_Utils::Geometry& geo =
			m_WedgeGeometry.at(hitSi.Detector-1);
		vector<const CsIHit_t*> csiMatch;
		for(const auto& csiHit : m_CsIHits){
			if(csiHit.Wedge < 0) {
				continue;
			}
			if(SiCsIMatch(hitSi, geo)(csiHit)) {
				csiMatch.push_back(&csiHit);
			}
		}
		// Add CsI info
		Double_t csiEnergy = -1000;
		Double_t csiTime = -1000;
		Int_t csiWedge = -1000;
		if(!csiMatch.empty()) {
			if(csiMatch.size() == 1) { // unambiguous
				csiEnergy = csiMatch.front()->Energy;
				csiTime = csiMatch.front()->Time;
				csiWedge = csiMatch.front()->Wedge;
			} else { // ambiguous, pick the closest one		
				for(const auto& csi: csiMatch) {
					if(SiCsIMatch(hitSi, geo, false)(*csi)) {
						csiEnergy = csi->Energy;
						csiTime = csi->Time;
						csiWedge = csi->Wedge;
					}
				}
			}
		}
		// Set hit info
		SetHit(siEnergy,siTime,csiEnergy,csiTime,
					 hitSi.Detector,hitSi.ThetaStrip,
					 hitSi.PhiStrip,csiWedge,
					 SiPos,SiPos_R);
		++iSi;
	}
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
			m_EventData->GetCsIE_DetectorNbr(i),
			m_EventData->GetCsIE_WedgeNbr(i),
			m_EventData->GetCsIEnergy(i) );
	}

	// CsI time
	for(size_t i=0; i< m_EventData->GetCsIMultTime(); ++i){
		m_PreTreatedData->SetCsITime(
			m_EventData->GetCsIT_DetectorNbr(i),
			m_EventData->GetCsIT_WedgeNbr(i),
			m_EventData->GetCsITime(i) );
	}

	// Si energy (theta strips)
	for(size_t i=0; i< m_EventData->GetSiThetaE_Mult(); ++i){
		m_PreTreatedData->SetSiThetaEnergy(
			m_EventData->GetSiThetaE_DetectorNbr(i),
			m_EventData->GetSiThetaE_StripNbr(i),
			m_EventData->GetSiThetaE_Energy(i) );
	}

	// Si time (theta strips)
	for(size_t i=0; i< m_EventData->GetSiThetaT_Mult(); ++i){
		m_PreTreatedData->SetSiThetaTime(
			m_EventData->GetSiThetaT_DetectorNbr(i),
			m_EventData->GetSiThetaT_StripNbr(i),
			m_EventData->GetSiThetaT_Time(i) );
	}

	// Si energy (phi strips)
	for(size_t i=0; i< m_EventData->GetSiPhiE_Mult(); ++i){
		m_PreTreatedData->SetSiPhiEnergy(
			m_EventData->GetSiPhiE_DetectorNbr(i),
			m_EventData->GetSiPhiE_StripNbr(i),
			m_EventData->GetSiPhiE_Energy(i) );
	}

	// Si time (phi strips)
	for(size_t i=0; i< m_EventData->GetSiPhiT_Mult(); ++i){
		m_PreTreatedData->SetSiPhiTime(
			m_EventData->GetSiPhiT_DetectorNbr(i),
			m_EventData->GetSiPhiT_StripNbr(i),
			m_EventData->GetSiPhiT_Time(i) );
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

	m_CsIHits.clear();
	m_SiHits.clear();
	m_Hits.clear();
	
	CsIHit_Detector.clear();
	CsIHit_Wedge.clear();
	CsIHit_Energy.clear();
	CsIHit_Time.clear();

	SiHit_Detector.clear();
	SiHit_ThetaStrip.clear();
	SiHit_PhiStrip.clear();
	SiHit_ThetaStripEnergy.clear();
	SiHit_PhiStripEnergy.clear();
	SiHit_ThetaStripTime.clear();
	SiHit_PhiStripTime.clear();

	Hit_Si_Energy.clear();
	Hit_Si_Time.clear();
	Hit_CsI_Energy.clear();
	Hit_CsI_Time.clear();
	Hit_Detector.clear();
	Hit_Si_ThetaStrip.clear();
	Hit_Si_PhiStrip.clear();
	Hit_CsI_Wedge.clear();
	Hit_Position.clear();
	Hit_Position_R.clear();
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

