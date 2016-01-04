#ifndef TDETECTORNAMEPHYSICS_H
#define TDETECTORNAMEPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold DETECTORNAME Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// STL
#include <vector>

// NPL
#include "TDETECTORNAMEData.h"
#include "TDETECTORNAMESpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"
#include "TCanvas.h"
// Forward declaration
class TDETECTORNAMESpectra;


using namespace std ;

class TDETECTORNAMEPhysics : public TObject, public NPL::VDetector{
  public:
    TDETECTORNAMEPhysics();
    ~TDETECTORNAMEPhysics() {};

  public: 
    void Clear();   
    void Clear(const Option_t*) {};

  public:
    //   DSSD
    vector<int> DetectorNumber ;
    vector<double> Energy ;
    vector<double> Time ;

  public: //   Innherited from VDetector Class

    //   Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
    void ReadConfiguration(string) ;


    //   Add Parameter to the CalibrationManger
    void AddParameterToCalibrationManager() ;      

    //   Activated associated Branches and link it to the private member DetectorData address
    //   In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
    void InitializeRootInputRaw() ;

    //   Activated associated Branches and link it to the private member DetectorPhysics address
    //   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
    void InitializeRootInputPhysics() ;

    //   Create associated branches and associated private member DetectorPhysics address
    void InitializeRootOutput() ;

    //   This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
    void BuildPhysicalEvent() ;

    //   Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
    //   This method aimed to be used for analysis performed during experiment, when speed is requiered.
    //   NB: This method can eventually be the same as BuildPhysicalEvent.
    void BuildSimplePhysicalEvent() ;

    // Same as above but for online analysis
    void BuildOnlinePhysicalEvent()  {BuildPhysicalEvent();};

    //   Those two method all to clear the Event Physics or Data
    void ClearEventPhysics() {Clear();}      
    void ClearEventData()    {m_EventData->Clear();}   

    // Method related to the TSpectra classes, aimed at providing a framework for online applications
    // Instantiate the Spectra class and the histogramm throught it
    void InitSpectra();
    // Fill the spectra hold by the spectra class
    void FillSpectra();
    // Used for Online mainly, perform check on the histo and for example change their color if issues are found
    void CheckSpectra();
    // Used for Online only, clear all the spectra hold by the Spectra class
    void ClearSpectra();
    // Write Spectra to file
    void WriteSpectra();

  public:      //   Specific to DETECTORNAME Array
    //   Clear The PreTeated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

    //   Remove bad channel, calibrate the data and apply threshold
    void PreTreat();

    //   Return false if the channel is disabled by user
    //   Frist argument is either "X","Y","SiLi","CsI"
    bool IsValidChannel( const int detector);

    //   Initialize the standard parameter for analysis
    //   ie: all channel enable, maximum multiplicity for strip = number of detector
    void InitializeStandardParameter();

    //   Read the user configuration file; if no file found, load standard one
    void ReadAnalysisConfig();

    // Give and external TMustData object to TDETECTORNAMEPhysics. Needed for online analysis for example.
    void SetRawDataPointer(TDETECTORNAMEData* rawDataPointer) {m_EventData = rawDataPointer;}
    
    // Retrieve raw and pre-treated data
    TDETECTORNAMEData* GetRawData()        const {return m_EventData;}
    TDETECTORNAMEData* GetPreTreatedData() const {return m_PreTreatedData;}

    TVector3 GetPositionOfInteraction(const int i) const;   

  private:   //   Parameter used in the analysis
    //  Threshold
    double m_E_RAW_Threshold ;//!
    double m_E_Threshold ;//!

  private:   //   Root Input and Output tree classes
    TDETECTORNAMEData*         m_EventData;//!
    TDETECTORNAMEData*         m_PreTreatedData;//!
    TDETECTORNAMEPhysics*      m_EventPhysics;//!


  private:   //   Map of activated channel
    map<int, bool> m_ChannelStatus;//!

  private:   //   Spatial Position of Detector
    int m_NumberOfDetector;//!
    vector< vector < vector < double > > >   m_PositionX;//!
    vector< vector < vector < double > > >   m_PositionY;//!
    vector< vector < vector < double > > >   m_PositionZ;//!

  private: // Spectra Class
    TDETECTORNAMESpectra* m_Spectra; // !

  public: // Spectra Getter
    map< string , TH1*> GetSpectra(); 
    vector<TCanvas*> GetCanvas();

  public: // Static constructor to be passed to the Detector Factory
    static NPL::VDetector* Construct();
    ClassDef(TDETECTORNAMEPhysics,1)  // DETECTORNAMEPhysics structure
};
#endif
