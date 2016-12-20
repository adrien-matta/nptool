#ifndef TNeutronWallPHYSICS_H
#define TNeutronWallPHYSICS_H
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
 *  This class hold NeutronWall Treated data                                *
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
using namespace std;

// ROOT headers
#include "TObject.h"
#include "TH1.h"
#include "TCanvas.h"

// NPTool headers
#include "TNeutronWallData.h"
#include "TNeutronWallSpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// forward declaration
class TNeutronWallSpectra;



class TNeutronWallPhysics : public TObject, public NPL::VDetector {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
  public:
    TNeutronWallPhysics();
    ~TNeutronWallPhysics() {};


  //////////////////////////////////////////////////////////////
  // Inherited from TObject and overriden to avoid warnings
  public: 
    void Clear();   
    void Clear(const Option_t*) {};


  //////////////////////////////////////////////////////////////
  // data obtained after BuildPhysicalEvent() and stored in
  // output ROOT file
  public:
    vector<int>      DetectorNumber;
    vector<int>      NW_PadNumber;
    vector<double>   NW_Energy;
    vector<double>   NW_Time;
    vector<double>   NW_PosX;
    vector<double>   NW_PosY;
    vector<double>   NW_PosZ;
    vector<int>      VW_PadNumber;
    vector<double>   VW_Energy;
    vector<double>   VW_Time;
    vector<double>   VW_PosX;
    vector<double>   VW_PosY;
    vector<double>   VW_PosZ;


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

    // methods related to the TNeutronWallSpectra class
    // instantiate the TNeutronWallSpectra class and 
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
  // specific methods to NeutronWall array
  public:
    // remove bad channels, calibrate the data and apply thresholds
    void PreTreat();

    // clear the pre-treated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

    // read the user configuration file. If no file is found, load standard one
    void ReadAnalysisConfig();

    // give and external TNeutronWallData object to TNeutronWallPhysics. 
    // needed for online analysis for example
    void SetRawDataPointer(TNeutronWallData* rawDataPointer) {m_EventData = rawDataPointer;}
    
  // objects are not written in the TTree
  private:
    TNeutronWallData*         m_EventData;        //!
    TNeutronWallData*         m_PreTreatedData;   //!
    TNeutronWallPhysics*      m_EventPhysics;     //!

  // getters for raw and pre-treated data object
  public:
    TNeutronWallData* GetRawData()        const {return m_EventData;}
    TNeutronWallData* GetPreTreatedData() const {return m_PreTreatedData;}

  // parameters used in the analysis
  private:
    // thresholds
    double m_E_RAW_Threshold; //!
    double m_E_Threshold;     //!

  // number of detectors
  private:
    int m_NumberOfDetectors;  //!

  // spectra class
  private:
    TNeutronWallSpectra* m_Spectra; // !

  // spectra getter
  public:
    map<string, TH1*>   GetSpectra(); 
    vector<TCanvas*>    GetCanvas();

  // Static constructor to be passed to the Detector Factory
  public:
    static NPL::VDetector* Construct();

    ClassDef(TNeutronWallPhysics,1)  // NeutronWallPhysics structure
};
#endif
