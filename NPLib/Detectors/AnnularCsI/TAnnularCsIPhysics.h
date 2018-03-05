#ifndef TAnnularCsIPHYSICS_H
#define TAnnularCsIPHYSICS_H
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
 *  This class hold AnnularCsI Treated data                                  *
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
#include "AnnularCsI_Utils.h"
#include "TAnnularCsIData.h"
#include "TAnnularCsISpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// forward declaration
class TAnnularCsISpectra;



class TAnnularCsIPhysics : public TObject, public NPL::VDetector {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
public:
	TAnnularCsIPhysics();
	~TAnnularCsIPhysics() {};


  //////////////////////////////////////////////////////////////
  // Inherited from TObject and overriden to avoid warnings
public: 
	void Clear();   
	void Clear(const Option_t*) {};


  //////////////////////////////////////////////////////////////
  // data obtained after BuildPhysicalEvent() and stored in
  // output ROOT file
public:
	std::vector<int>      DetectorNumber;
	std::vector<double>   Energy;
	std::vector<double>   Time;

  // Add A wedge detector
  void AddDetector(double R_min, double R_max, 
									 double Phi_min, double Phi_max,
									 double Z);
	
  //////////////////////////////////////////////////////////////
  // methods inherited from the VDetector ABC class
public:
	// read stream from ConfigFile to pick-up detector parameters
	void ReadConfiguration(NPL::InputParser);

	// add parameters to the CalibrationManger
	void AddParameterToCalibrationManager();

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

	// methods related to the TAnnularCsISpectra class
	// instantiate the TAnnularCsISpectra class and 
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
  // specific methods to AnnularCsI array
public:
	// remove bad channels, calibrate the data and apply thresholds
	void PreTreat();

	// clear the pre-treated object
	void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

	// read the user configuration file. If no file is found, load standard one
	void ReadAnalysisConfig();

	// give and external TAnnularCsIData object to TAnnularCsIPhysics. 
	// needed for online analysis for example
	void SetRawDataPointer(TAnnularCsIData* rawDataPointer) {m_EventData = rawDataPointer;}
    
  // objects are not written in the TTree
private:
	TAnnularCsIData*         m_EventData;        //!
	TAnnularCsIData*         m_PreTreatedData;   //!
	TAnnularCsIPhysics*      m_EventPhysics;     //!

  // getters for raw and pre-treated data object
public:
	TAnnularCsIData* GetRawData()        const {return m_EventData;}
	TAnnularCsIData* GetPreTreatedData() const {return m_PreTreatedData;}

	// getters for interaction position
	TVector3 GetPositionOfInteraction(int i) const;
	
  // parameters used in the analysis
private:
	// thresholds
	double m_E_RAW_Threshold; //!
	double m_E_Threshold;     //!
	
	// interaction position
	std::vector<TVector3> m_WedgePosition; //!
	// dimensions of wedge
	std::vector<AnnularCsI_Utils::Geometry> m_WedgeGeometry; //!
	
  // spectra class
private:
	TAnnularCsISpectra* m_Spectra; // !

public:
	// Getters
  std::map<std::string, TH1*>   GetSpectra();
	UInt_t GetNumberOfDetectors()   const { return m_WedgePosition.size(); }
	double GetRawEThreshold()       const { return m_E_RAW_Threshold; }
	double GetEThreshold()          const { return m_E_Threshold; }
	int    GetDetectorNumber(int i) const { return DetectorNumber.at(i); }
	double GetEnergy(int i)         const { return Energy.at(i); }
	double GetTime(int i)           const { return Time.at(i); }
	int    MatchToSi(const TVector3& position) const;
	
  // Static constructor to be passed to the Detector Factory
public:
	static NPL::VDetector* Construct();

	ClassDef(TAnnularCsIPhysics,1)  // AnnularCsIPhysics structure
};
#endif

/* Local Variables:  */
/* mode: c++         */
/* End:              */
