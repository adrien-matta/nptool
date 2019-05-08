#ifndef TActarPHYSICS_H
#define TActarPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouace@ganil.fr    *
 *                                                                           *
 * Creation Date  : September 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Actar Treated data                                *
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
#include "MEventReduced.h"
#include "TActarData.h"
#include "TActarSpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
#include "NPTrack.h"
#include "NPRansac.h"
#include "NPCluster.h"

#define NumberOfCobo 16
#define NumberOfASAD 4
#define NumberOfAGET 4
#define NumberOfChannel 68

class TActarSpectra;

class TActarPhysics : public TObject, public NPL::VDetector {
    //////////////////////////////////////////////////////////////
    // constructor and destructor
public:
    TActarPhysics();
    ~TActarPhysics() {};


    //////////////////////////////////////////////////////////////
    // Inherited from TObject and overriden to avoid warnings
public:
    void Clear();
    void Clear(const Option_t*) {};


    //////////////////////////////////////////////////////////////
    // data obtained after BuildPhysicalEvent() and stored in
    // output ROOT file
public:
    vector<int> PadX;
    vector<int> PadY;
    vector<double> PadZ;
    vector<double> PadCharge;
    vector<int> BeamPadX;
    vector<int> BeamPadY;
    vector<double> BeamPadZ;
    vector<double> BeamPadCharge;
    vector<double> Si_E;
    vector<int> Si_Number;
    int TrackMult;

    /// A usefull method to bundle all operation to add a detector
    void AddDetector(TVector3 POS, string shape);
    void AddDetector(double R, double Theta, double Phi, string shape);

public:
    int GetTrackMult() {return m_Track.size();}
    vector<NPL::Track> GetTracks() {return m_Track;}


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

    // methods related to the TActarSpectra class
    // instantiate the TActarSpectra class and
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
    // specific methods to Actar array
public:
    // remove bad channels, calibrate the data and apply thresholds
    void PreTreat();

    // clear the pre-treated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

    // read the user configuration file. If no file is found, load standard one
    void ReadAnalysisConfig();

    void CleanPads();

    bool GoodHit(int iX, int iY);

    bool IsBeamZone(int X, int Y);

    // give and external TActarData object to TActarPhysics.
    // needed for online analysis for example
    void SetRawDataPointer(TActarData* rawDataPointer) {m_EventData = rawDataPointer;}

    // objects are not written in the TTree
private:
    TActarData*         m_EventData;        //!
    MEventReduced*      m_EventReduced;     //!
    TActarData*         m_PreTreatedData;   //!
    TActarPhysics*      m_EventPhysics;     //!
    NPL::Ransac*        m_Ransac;           //!
    NPL::Cluster*       m_Cluster;          //!
    vector<NPL::Track>  m_Track;            //!

    // getters for raw and pre-treated data object
public:
    TActarData* GetRawData()        const {return m_EventData;}
    TActarData* GetPreTreatedData() const {return m_PreTreatedData;}

    double GetDriftVelocity() {return fDriftVelocity;}
    double GetPadSizeX() {return fPadSizeX;}
    double GetPadSizeY() {return fPadSizeY;}
    int GetNumberOfPadsX() {return fNumberOfPadsX;}
    int GetNumberOfPadsY() {return fNumberOfPadsY;}
    double GetPRessure() {return fPressure;}
    string GetGasName() {return fGas;}

    // parameters used in the analysis
private:
    // thresholds
    int fHitThreshold;      //!
    int fQ_Threshold;       //!
    int fT_Threshold;       //!
    int fNumberOfPadsX;     //!
    int fNumberOfPadsY;     //!
    int fXBeamMax;          //!
    int fXBeamMin;          //!
    int fYBeamMax;          //!
    int fYBeamMin;          //!
    double fPadSizeX;       //!
    double fPadSizeY;       //!
    double fDriftVelocity;  //!
    double fPressure;       //!
    string fGas;            //!
    bool fRecoRansac;       //!
    bool fRecoCluster;      //!
    bool fRecoVisu;         //!
    map<int, int> Si_map;   //!
    string fInputTreeName;  //!

    int TABLE[6][NumberOfCobo*NumberOfASAD*NumberOfAGET*NumberOfChannel]; //!
    int Hit[128][128];  //!


    // number of detectors
private:
    int m_NumberOfDetectors;  //!
    int m_NumberOfPadSilicon; //!

    // spectra class
private:
    TActarSpectra* m_Spectra; //!

    //spme getters and setters
public:
    void SetRansacParameter(string filename);
    void SetClusterParameter(string filename);
    NPL::Ransac* GetRansacObject() {return m_Ransac;}
    bool GetRansacStatus() {return fRecoRansac;}
    NPL::Cluster* GetClusterObject() {return m_Cluster;}
    bool GetClusterStatus() {return fRecoCluster;}


    // spectra getter
public:
    map<string, TH1*>   GetSpectra();
    vector<TCanvas*>    GetCanvas();

    // Static constructor to be passed to the Detector Factory
public:
    static NPL::VDetector* Construct();

    ClassDef(TActarPhysics,1)  // ActarPhysics structure
};
#endif
