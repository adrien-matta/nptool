#ifndef TFPDTamuPHYSICS_H
#define TFPDTamuPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: m.moukaddam@surrey.ac.uk                                 *
 *                                                                           *
 * Creation Date  : November 2016                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold FPDTamu Treated data                                     *
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
#include "TVector3.h"
#include "TH1.h"
#include "TCanvas.h"

// NPTool headers
#include "TFPDTamuData.h"
#include "TFPDTamuSpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// forward declaration
class TFPDTamuSpectra;



class TFPDTamuPhysics : public TObject, public NPL::VDetector {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
  public:
    TFPDTamuPhysics();
    ~TFPDTamuPhysics() {};


  //////////////////////////////////////////////////////////////
  // Inherited from TObject and overriden to avoid warnings
  public: 
    void Clear();   
    void Clear(const Option_t*) {};


  //////////////////////////////////////////////////////////////
  // data obtained after BuildPhysicalEvent() and stored in
  // output ROOT file
  public:
  // Delta
  vector<int>    DeltaDetNumber;
  vector<double> DeltaCharge;//!
  vector<double> DeltaEnergy;
  vector<double> DeltaTime;//!
  //Micromega
  vector<int>    MicroRowNumber;
  vector<int>    MicroColNumber;
  vector<double> MicroPositionX;
  vector<double> MicroPositionZ;
  vector<double> MicroCharge;
  vector<double> MicroEnergy;
  vector<double> MicroTime;
  //Avalanche wire
  vector<int>    AWireDetNumber;
  vector<double> AWireLeftCharge;
  vector<double> AWireRightCharge;
  vector<double> AWirePositionX;
  vector<double> AWirePositionZ;
  //Plastic scintillator
  vector<double> PlastLeftCharge;
  vector<double> PlastRightCharge;
  vector<double> PlastLeftTime;
  vector<double> PlastRightTime;
  vector<double> PlastCharge;
  vector<double> PlastPositionX;
  vector<double> PlastPositionZ;

  //Calculated AWire and Plastic
  double   PlastPositionX_AW;
  TVector3 IonDirection;//!

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

    // methods related to the TFPDTamuSpectra class
    // instantiate the TFPDTamuSpectra class and 
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
  // specific methods to FPDTamu array
  public:
    // remove bad channels, calibrate the data and apply thresholds
    void PreTreat();

    // clear the pre-treated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

    // read the user configuration file. If no file is found, load standard one
    void ReadAnalysisConfig();

    // give and external TFPDTamuData object to TFPDTamuPhysics. 
    // needed for online analysis for example
    void SetRawDataPointer(TFPDTamuData* rawDataPointer) {m_EventData = rawDataPointer;}
    
    void Dump() const;

  // objects are not written in the TTree
  private:
    TFPDTamuData*         m_EventData;        //!
    TFPDTamuData*         m_PreTreatedData;   //!
    TFPDTamuPhysics*      m_EventPhysics;     //!

  // getters for raw and pre-treated data object
  public:
    TFPDTamuData* GetRawData()        const {return m_EventData;}
    TFPDTamuData* GetPreTreatedData() const {return m_PreTreatedData;}
    
    // Micromega specific used in analysis
    double GetMicroGroupEnergy(int lrow, int hrow, int lcol, int hcol) ; 
    double GetMicroRowGeomEnergy(int lrow, int hrow);
  
  // parameters used in the analysis
  private:
    // thresholds
    double m_E_RAW_Threshold; //!
    double m_E_Threshold;     //!
    double m_T_RAW_Threshold; //!
    double m_T_Threshold;     //!

  // add sub-detectors
  private:
    void AddDelta(TVector3 A, TVector3 B);  //!
    void AddMicro(TVector3 A, TVector3 B);  //!
    void AddAWire(TVector3 A, TVector3 B);  //!
    void AddPlast(TVector3 A, TVector3 B);  //!

    vector<TVector3> DeltaLeftPos;  //!
    vector<TVector3> DeltaRightPos; //!
    vector<TVector3> MicroLeftPos;  //!
    vector<TVector3> MicroRightPos; //!
    vector<TVector3> AWireLeftPos;  //!
    vector<TVector3> AWireRightPos; //!
    TVector3 PlastLeftPos;  //!
    TVector3 PlastRightPos; //!

  // number of detectors
  private:
    int m_NumberOfDetectors;  //!
    int m_NumberOfDelta;  //!
    int m_NumberOfMicro;  //!
    int m_NumberOfAWire;  //!
    int m_NumberOfPlast;  //!

  // spectra class
  private:
    TFPDTamuSpectra* m_Spectra; // !

  // spectra getter
  public:
    map<string, TH1*>   GetSpectra(); 
    vector<TCanvas*>    GetCanvas();

  // Static constructor to be passed to the Detector Factory
  public:
    static NPL::VDetector* Construct();

    ClassDef(TFPDTamuPhysics,1)  // FPDTamuPhysics structure
};
#endif
