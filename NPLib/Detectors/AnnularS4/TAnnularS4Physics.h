#ifndef TAnnularS4PHYSICS_H
#define TAnnularS4PHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : January 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularS4 Treated data                                *
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
#include "TVector3.h"
// NPTool headers
#include "TAnnularS4Data.h"
#include "TAnnularS4Spectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// forward declaration
class TAnnularS4Spectra;



class TAnnularS4Physics : public TObject, public NPL::VDetector {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
  public:
    TAnnularS4Physics();
    ~TAnnularS4Physics() {};


  //////////////////////////////////////////////////////////////
  // Inherited from TObject and overriden to avoid warnings
  public: 
    void Clear();   
    void Clear(const Option_t*) {};


  //////////////////////////////////////////////////////////////
  // data obtained after BuildPhysicalEvent() and stored in
  // output ROOT file
  public:
    vector<int>     DetectorNumber;
    vector<double>  Energy;
    vector<double>  StripNbr;
    vector<double>  SectorNbr;
    vector<double>  Time;

  /// A usefull method to bundle all operation to add a detector
  void AddDetector(TVector3 Position);
  void AddDetector(double R, double Theta, double Phi);
  
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

    // methods related to the TAnnularS4Spectra class
    // instantiate the TAnnularS4Spectra class and 
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
  // specific methods to AnnularS4 array
  public:
    // remove bad channels, calibrate the data and apply thresholds
    void PreTreat();

    // clear the pre-treated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

    // read the user configuration file. If no file is found, load standard one
    void ReadAnalysisConfig();

    // give and external TAnnularS4Data object to TAnnularS4Physics. 
    // needed for online analysis for example
    void SetRawDataPointer(TAnnularS4Data* rawDataPointer) {m_EventData = rawDataPointer;}
    
    // Return the position of interaction in S4
    TVector3 GetPositionOfInteraction(const int i) const;
    
    double GetR(const int N)                                const{return m_R[N];}
    double GetTheta(const int N, const int RingStrip)       const{return m_ThetaAngleOfRingStrip[N][RingStrip-1];}
    double GetPhi(const int Sector)                         const {return m_PhiAngleOfSector[Sector-1];}
    
  // objects are not written in the TTree
  private:
    TAnnularS4Data*         m_EventData;        //!
    TAnnularS4Data*         m_PreTreatedData;   //!
    TAnnularS4Physics*      m_EventPhysics;     //!
    
    vector<double>          m_R; //!
    vector<vector<double>>  m_ThetaAngleOfRingStrip; //!
    vector<double>          m_PhiAngleOfSector; //!
    
    


  // getters for raw and pre-treated data object
  public:
    TAnnularS4Data* GetRawData()        const {return m_EventData;}
    TAnnularS4Data* GetPreTreatedData() const {return m_PreTreatedData;}

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
    TAnnularS4Spectra* m_Spectra; // !

  // spectra getter
  public:
    map<string, TH1*>   GetSpectra(); 
    vector<TCanvas*>    GetCanvas();

  // Static constructor to be passed to the Detector Factory
  public:
    static NPL::VDetector* Construct();

    ClassDef(TAnnularS4Physics,1)  // AnnularS4Physics structure
};
#endif
