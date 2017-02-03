#ifndef TTIARABARRELPHYSICS_H
#define TTIARABARRELPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : December 2013                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TiaraBarrel treated data                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *  
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// STL
#include <vector>
#include <map>
// NPL
#include "TTiaraBarrelData.h"
#include "TTiaraBarrelSpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"
#include "TH1.h"

class TTiaraBarrelSpectra;

using namespace std ;

class TTiaraBarrelPhysics : public TObject, public NPL::VDetector{
  public:
    TTiaraBarrelPhysics();
    ~TTiaraBarrelPhysics() {};

  public: 
    void Clear();   
    void Clear(const Option_t*) {};

  public:
    //   Provide Physical Multiplicity
    Int_t EventMultiplicity;

    // Detector
    vector<int> DetectorNumber ;

    // Inner Barrel
    vector<double> Strip_E;
    vector<double> Strip_T;
    vector<int>    Strip_N;
    vector<double> Strip_Pos;
   
    // Control stuff 
    vector<double> DownStream_E;
    vector<double> DownStream_T;
    vector<double> UpStream_E;
    vector<double> UpStream_T;
    vector<double> Back_E;
    vector<double> Back_T;

    // Outter Barrel
    vector<double> Outer_Strip_E;
    vector<double> Outer_Strip_T;
    vector<double> Outer_Strip_N;
    vector<double> Outer_Back_E;
    vector<double> Outer_Back_T;

  public:      //   Innherited from VDetector Class
    //   Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
    void ReadConfiguration(NPL::InputParser) ;

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

    // Method related to the TSpectra classes, aimed at providing a framework 
    // for online applications
    // Instantiate the Spectra class and the histogramm throught it
    void InitSpectra();
    // Fill the spectra hold by the spectra class
    void FillSpectra();
    // Used for Online mainly, perform check on the histo and for example change 
    // their color if issues are found
    void CheckSpectra();
    // Used for Online only, clear all the spectra hold by the Spectra class
    void ClearSpectra();
    // Write Spectra to file
    void WriteSpectra();
  public://   Specific to TiaraBarrel Array
    //   Clear The PreTeated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

    //   Remove bad channel, calibrate the data and apply threshold
    void PreTreat();

    //   Return false if the channel is disabled by user
    //   Frist argument is either "X","Y","SiLi","CsI"
    bool IsValidChannel(const string DetectorType, const int detector , const int channel);

    //   Initialize the standard parameter for analysis
    //   ie: all channel enable, maximum multiplicity for strip = number of detector
    void InitializeStandardParameter();

    //   Read the user configuration file; if no file found, load standard one
    void ReadAnalysisConfig();

    //   Add a Detector
    void AddDetector( double X,double Y,double Z);
 
    // Give and external TMustData object to TTiaraBarrelPhysics. Needed for online analysis for example.
    void SetRawDataPointer(TTiaraBarrelData* rawDataPointer) {m_EventData = rawDataPointer;}
    // Retrieve raw and pre-treated data
    TTiaraBarrelData* GetRawData()        const {return m_EventData;}
    TTiaraBarrelData* GetPreTreatedData() const {return m_PreTreatedData;}

    double GetNumberOfDetector() const { return m_NumberOfDetector; };

    // To be called after a build Physical Event 
    int GetEventMultiplicity() const { return EventMultiplicity; };

    TVector3 GetPositionOfInteraction(const int i) const;   
    TVector3 GetRandomisedPositionOfInteraction(const int i) const;  
    TVector3 GetDetectorNormal(const int i) const;

  private:   //   Parameter used in the analysis
    // By default take EX and TY.
    bool m_Take_E_Strip;//!
    bool m_Take_T_Back;//!

    //  Threshold
    double m_Strip_E_Threshold ;//!
    double m_Back_E_Threshold ;//!
    double m_OuterBack_E_Threshold ;//!
    double m_Maximum_FrontBack_Difference ;//!
  private:   //   Root Input and Output tree classes
    TTiaraBarrelData*         m_EventData;//!
    TTiaraBarrelData*         m_PreTreatedData;//!
    TTiaraBarrelPhysics*      m_EventPhysics;//!

  private:   //   Map of activated channel
    map< int, vector<bool> > m_InnerBarrelStripUpstreamChannelStatus;//!
    map< int, vector<bool> > m_InnerBarrelStripDownstreamChannelStatus;//!
    map< int, vector<bool> > m_OuterBarrelStripChannelStatus;//!
    map< int, vector<bool> > m_InnerBarrelBackChannelStatus;//!
    map< int, vector<bool> > m_OuterBarrelBackChannelStatus;//!

  private:   //   Spatial Position of Strip Calculated on bases of detector position
    int m_NumberOfDetector;//!
    vector< vector<double> > m_StripPositionX;//!
    vector< vector<double> > m_StripPositionY;//!
    vector< vector<double> > m_StripPositionZ;//!

  private: // Spectra
    TTiaraBarrelSpectra*      m_Spectra;//!

  public:
    map< string,TH1* > GetSpectra(); 

  private: // Usefull method
   // Calibrate data
  double Cal_Strip_Upstream_E(const int i);
  double Cal_Strip_Downstream_E(const int i);
  double Cal_Back_E(const int i);
  double Match_Strip_Upstream_E(const int i);
  double Match_Strip_Downstream_E(const int i);

  public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TTiaraBarrelPhysics,1)  // SharcPhysics structure

};
namespace TiaraBarrel_LOCAL{
 string itoa(unsigned int value);
}
#endif
