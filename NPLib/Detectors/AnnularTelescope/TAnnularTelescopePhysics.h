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


  //////////////////////////////////////////////////////////////
  // data obtained after BuildPhysicalEvent() and stored in
  // output ROOT file
public:
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
	std::vector<TVector3> Hit_Position;

  // Add a detector
  void AddDetector(const AnnularTelescope_Utils::Geometry& g);
	
  //////////////////////////////////////////////////////////////
  // methods inherited from the VDetector ABC class
public:
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
