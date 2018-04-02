#ifndef TAnnularTelescopePHYSICS_H
#define TAnnularTelescopePHYSICS_H
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
 *  This class hold AnnularTelescope Treated data                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// C++ headers 
#include <vector>
#include <map>
#include <string>

// ROOT headers
#include "TObject.h"
#include "TH1.h"
#include "TVector3.h"
// NPTool headers
#include "AnnularTelescope_Utils.h"
#include "TAnnularTelescopeData.h"
#include "TAnnularTelescopeSpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// forward declaration
class TAnnularTelescopeSpectra;

struct CsIHit_t {
	Int_t    Detector;
	Int_t    Wedge;
	Double_t Energy;
	Double_t Time;
};
struct SiHit_t {
	Int_t    Detector;
	Int_t    ThetaStrip;
	Int_t    PhiStrip;
	Double_t ThetaStripEnergy;
	Double_t PhiStripEnergy;
	Double_t ThetaStripTime;
	Double_t SiHit_PhiStripTime;
};
struct Hit_t {
	Double_t Si_Energy;
	Double_t Si_Time;
	Double_t CsI_Energy;
	Double_t CsI_Time;
	Int_t Detector;
	Int_t Si_ThetaStrip;
	Int_t Si_PhiStrip;
	Int_t CsI_Wedge;
	TVector3 Position;
	TVector3 Position_R;
};


class TAnnularTelescopePhysics : public TObject, public NPL::VDetector {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
public:
	TAnnularTelescopePhysics();
	~TAnnularTelescopePhysics() {};


  //////////////////////////////////////////////////////////////
  // Inherited from TObject and overriden to avoid warnings
public: 
	void Clear();   
	void Clear(const Option_t*) {};

	void SetCsIHit(
		Int_t detector, Int_t wedge, Double_t energy, Double_t time){
		CsIHit_t hit = { detector, wedge, energy, time };
		m_CsIHits.push_back(hit);
		CsIHit_Detector.push_back(detector);
		CsIHit_Wedge.push_back(wedge);
		CsIHit_Energy.push_back(energy);
		CsIHit_Time.push_back(time);
	}
	void SetSiHit(
		Int_t detector, Int_t thetaStrip, Int_t phiStrip,
		Double_t thetaStripEnergy, Double_t phiStripEnergy,
		Double_t thetaStripTime, Double_t phiStripTime) {
		SiHit_t hit = {
			detector, thetaStrip, phiStrip,
			thetaStripEnergy, phiStripEnergy,
			thetaStripTime, phiStripTime
		};
		m_SiHits.push_back(hit);
		SiHit_Detector.        push_back(detector);
		SiHit_ThetaStrip.      push_back(thetaStrip);
		SiHit_PhiStrip.        push_back(phiStrip);
		SiHit_ThetaStripEnergy.push_back(thetaStripEnergy);
		SiHit_PhiStripEnergy.  push_back(phiStripEnergy);
		SiHit_ThetaStripTime.  push_back(thetaStripTime);
		SiHit_PhiStripTime.    push_back(phiStripTime);
	}
	void SetHit(
		Double_t siEnergy, Double_t siTime, Double_t csiEnergy, Double_t csiTime,
		Int_t detector, Int_t siThetaStrip, Int_t siPhiStrip, Int_t csiWedge,
		const TVector3& position, const TVector3& position_R){
		Hit_t hit;
		hit.Si_Energy = siEnergy; hit.Si_Time = siTime;
		hit.CsI_Energy = csiEnergy; hit.CsI_Time = csiTime;
		hit.Detector = detector; hit.Si_ThetaStrip = siThetaStrip;
		hit.Si_PhiStrip = siPhiStrip; hit.CsI_Wedge = csiWedge;
		hit.Position = position; hit.Position_R = position_R;

		m_Hits.push_back(hit);
		Hit_Si_Energy.push_back(siEnergy);
		Hit_Si_Time.push_back(siTime);
		Hit_CsI_Energy.push_back(csiEnergy);
		Hit_CsI_Time.push_back(csiTime);
		Hit_Position.push_back(position);
		Hit_Position_R.push_back(position_R);

		Hit_Detector.push_back(detector);
		Hit_Si_ThetaStrip.push_back(siThetaStrip);
		Hit_Si_PhiStrip.push_back(siPhiStrip);
		Hit_CsI_Wedge.push_back(csiWedge);
	}
	const CsIHit_t& GetCsIHit(size_t i) const {
		try { return m_CsIHits.at(i); }
		catch(std::exception&) {
			std::cerr 
				<< "ERROR: Invalid index: " << i << " to TAnnularTelescopePhysics::GetCsIHit\n";
			exit(1);
		}
	}
	const SiHit_t& GetSiHit(size_t i) const {
		try { return m_SiHits.at(i); }
		catch(std::exception&) {
			std::cerr 
				<< "ERROR: Invalid index: " << i << " to TAnnularTelescopePhysics::GetSiHit\n";
			exit(1);
		}
	}
	const Hit_t& GetHit(size_t i) const {
		try { return m_Hits.at(i); }
		catch(std::exception&) {
			std::cerr 
				<< "ERROR: Invalid index: " << i << " to TAnnularTelescopePhysics::GetHit\n";
			exit(1);
		}
	}
	const std::vector<CsIHit_t>& GetCsIHits() const { return m_CsIHits; }
	const std::vector<SiHit_t>& GetSiHits() const { return m_SiHits; }
	const std::vector<Hit_t>& GetHits() const { return m_Hits; }

private:
	std::vector<CsIHit_t> m_CsIHits; //!
	std::vector<SiHit_t>  m_SiHits; //!
	std::vector<Hit_t>    m_Hits; //!
  //////////////////////////////////////////////////////////////
  // data obtained after BuildPhysicalEvent() and stored in
  // output ROOT file
	//
	// CsI Hit
	std::vector<Int_t>    CsIHit_Detector;
	std::vector<Int_t>    CsIHit_Wedge;
	std::vector<Double_t> CsIHit_Energy;
	std::vector<Double_t> CsIHit_Time;
	//
	// Si Hit
	std::vector<Int_t>    SiHit_Detector;
	std::vector<Int_t>    SiHit_ThetaStrip;
	std::vector<Int_t>    SiHit_PhiStrip;
	std::vector<Double_t> SiHit_ThetaStripEnergy;
	std::vector<Double_t> SiHit_PhiStripEnergy;
	std::vector<Double_t> SiHit_ThetaStripTime;
	std::vector<Double_t> SiHit_PhiStripTime;
	//
	// Composed (Si+CsI) hit
	std::vector<Double_t> Hit_Si_Energy;
	std::vector<Double_t> Hit_Si_Time;
	std::vector<Double_t> Hit_CsI_Energy;
	std::vector<Double_t> Hit_CsI_Time;
	std::vector<Int_t>    Hit_Detector;
	std::vector<Int_t>    Hit_Si_ThetaStrip;
	std::vector<Int_t>    Hit_Si_PhiStrip;
	std::vector<Int_t>    Hit_CsI_Wedge;
	std::vector<TVector3> Hit_Position;
	std::vector<TVector3> Hit_Position_R;

public:
  // Add a detector
  void AddDetector(const AnnularTelescope_Utils::Geometry& g);
	
  //////////////////////////////////////////////////////////////
  // methods inherited from the VDetector ABC class
	//
	// read stream from ConfigFile to pick-up detector parameters
	void ReadConfiguration(NPL::InputParser);

	// add parameters to the CalibrationManger
	void AddParameterToCalibrationManager();

	// build CsI part of event
	void BuildCsIEvent();

	// build Si part of event
	void BuildSiEvent();

	// combine Si + CsI
	void BuildTotalEvent();
	
	// method called event by event, aiming at extracting the 
	// physical information from detector
	void BuildPhysicalEvent();
	
	// same as BuildPhysicalEvent() method but with a simpler
	// treatment
	void BuildSimplePhysicalEvent();

	// same as above but for online analysis
	void BuildOnlinePhysicalEvent()  {BuildPhysicalEvent();};

	// activate raw data object and branches from input TChain
	// in this method mother branches (Detector) AND daughter leaves 
	// (fDetector_parameter) have to be activated
	void InitializeRootInputRaw();

	// activate physics data object and branches from input TChain
	// in this method mother branches (Detector) AND daughter leaves 
	// (fDetector_parameter) have to be activated
	void InitializeRootInputPhysics();

	// create branches of output ROOT file
	void InitializeRootOutput();

	// clear the raw and physical data objects event by event
	void ClearEventPhysics() {Clear();}      
	void ClearEventData()    {m_EventData->Clear();}   

	// methods related to the TAnnularTelescopeSpectra class
	// instantiate the TAnnularTelescopeSpectra class and 
	// declare list of histograms
	void InitSpectra();

	// fill the spectra
	void FillSpectra();

	// used for Online mainly, sanity check for histograms and 
	// change their color if issues are found, for example
	void CheckSpectra();

	// used for Online only, clear all the spectra
	void ClearSpectra();

	// write spectra to ROOT output file
	void WriteSpectra();


  //////////////////////////////////////////////////////////////
  // specific methods to AnnularTelescope array
public:
	// remove bad channels, calibrate the data and apply thresholds
	void PreTreat();

	// clear the pre-treated object
	void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

	// read the user configuration file. If no file is found, load standard one
	void ReadAnalysisConfig();

	// give and external TAnnularTelescopeData object to TAnnularTelescopePhysics. 
	// needed for online analysis for example
	void SetRawDataPointer(TAnnularTelescopeData* rawDataPointer) {m_EventData = rawDataPointer;}
    
  // objects are not written in the TTree
private:
	TAnnularTelescopeData*         m_EventData;        //!
	TAnnularTelescopeData*         m_PreTreatedData;   //!
	TAnnularTelescopePhysics*      m_EventPhysics;     //!

	struct EnergyMatch_t {
		Int_t NSigma;
		Double_t Sigma;
	} m_EnergyMatch;  //!

  // getters for raw and pre-treated data object
public:
	TAnnularTelescopeData* GetRawData()        const {return m_EventData;}
	TAnnularTelescopeData* GetPreTreatedData() const {return m_PreTreatedData;}

	// getters for interaction position
	TVector3 GetPositionOfInteraction(int i) const;
	TVector3 GetRandomisedPositionOfInteraction(int i) const;

	// Number of detectors
	int GetNumberOfDetectors() const { return m_WedgeGeometry.size(); } //!
	
  // parameters used in the analysis
private:
	// dimensions of wedge
	std::vector<AnnularTelescope_Utils::Geometry> m_WedgeGeometry; //!
	
  // spectra class
private:
	TAnnularTelescopeSpectra* m_Spectra; //!

public:
	// Getters
  std::map<std::string, TH1*>   GetSpectra();
	
  // Static constructor to be passed to the Detector Factory
public:
	static NPL::VDetector* Construct();

	ClassDef(TAnnularTelescopePhysics,1)  // AnnularTelescopePhysics structure
};
#endif

/* Local Variables:  */
/* mode: c++         */
/* End:              */
