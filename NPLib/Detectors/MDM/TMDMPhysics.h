#ifndef TMDMPHYSICS_H
#define TMDMPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : October 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold MDM Treated data                                *
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
#include "TMDMData.h"
#include "TMDMSpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// forward declaration
class TMDMSpectra;
class Rayin;
class MDMTrace;


class TMDMPhysics : public TObject, public NPL::VDetector {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
public:
	TMDMPhysics();
	~TMDMPhysics();


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
	std::vector<double>   Xpos; // cm
	std::vector<double>   Ypos; // cm
	std::vector<double>   Zpos; // cm
	double                Xang; // deg
	double                Yang; // deg

private:
	double m_Angle;
	double m_Field;
	Rayin* m_Rayin; //!
	MDMTrace* m_Trace; //!

public:
	
	double GetAngle() { return m_Angle; }
	double GetField() { return m_Field; }
	
  /// A usefull method to bundle all operation to add a detector
	void AddDetector(double angle, double field, const std::string& rayin);
  
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

	// methods related to the TMDMSpectra class
	// instantiate the TMDMSpectra class and 
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
  // specific methods to MDM array
public:
	// remove bad channels, calibrate the data and apply thresholds
	void PreTreat();

	// clear the pre-treated object
	void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

	// read the user configuration file. If no file is found, load standard one
	void ReadAnalysisConfig();

	// give and external TMDMData object to TMDMPhysics. 
	// needed for online analysis for example
	void SetRawDataPointer(TMDMData* rawDataPointer) {m_EventData = rawDataPointer;}

	// do chi2 minimization to find most likely energy, angle parameters @target
	void MinimizeTarget(const double* xwire, const double* ywire, // inputs, len 4 array, cm
											double& ekin, double& ata, double& bta);  // outputs, MeV, deg
    
  // objects are not written in the TTree
private:
	TMDMData*         m_EventData;        //!
	TMDMData*         m_PreTreatedData;   //!
	TMDMPhysics*      m_EventPhysics;     //!

  // getters for raw and pre-treated data object
public:
	TMDMData* GetRawData()        const {return m_EventData;}
	TMDMData* GetPreTreatedData() const {return m_PreTreatedData;}

  // parameters used in the analysis
private:
	// thresholds
	double m_X_Threshold;     //!
	double m_Y_Threshold;     //!

  // number of detectors
private:
	int m_NumberOfDetectors;  //!

  // spectra class
private:
	TMDMSpectra* m_Spectra; // !

  // spectra getter
public:
	std::map<std::string, TH1*>   GetSpectra(); 

  // Static constructor to be passed to the Detector Factory
public:
	static NPL::VDetector* Construct();

	ClassDef(TMDMPhysics,1)  // MDMPhysics structure
};
#endif
