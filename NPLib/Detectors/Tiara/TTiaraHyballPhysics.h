#ifndef TTIARAHYBALLPHYSICS_H
#define TTIARAHYBALLPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TiaraHyball treated data                                 *
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
#include "TTiaraHyballData.h"
#include "TTiaraHyballSpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"

// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"
#include "TH1.h"
#include "TRandom3.h"

class TTiaraHyballSpectra;

using namespace std ;

class TTiaraHyballPhysics : public TObject, public NPL::VDetector{
  public:
    TTiaraHyballPhysics();
    ~TTiaraHyballPhysics() {};

  public: 
    void Clear();   
    void Clear(const Option_t*) {};

  public: 
    vector < TVector2 > Match_Ring_Sector() ;
    int  CheckEvent();

  public:
    //   Provide Physical Multiplicity
    Int_t EventMultiplicity;

    //   Provide a Classification of Event
    vector<int> EventType ;

    // Detector
    vector<int> DetectorNumber ;

    //   DSSD
    vector<double> Strip_E ;
    vector<double> Strip_T ;
    vector<double> StripRing_E ;
    vector<double> StripRing_T ;
    vector<double> StripSector_E ;
    vector<double> StripSector_T ;
    vector<int>    Strip_Ring ;
    vector<int>    Strip_Sector ;

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

    // Method related to the TSpectra classes, aimed at providing a framework for online applications
    // Instantiate the Spectra class and the histogramm throught it
    void InitSpectra();
    // Fill the spectra hold by the spectra class
    void FillSpectra();
    // Used for Online mainly, perform check on the histo and for example change their color if issues are found
    void CheckSpectra();
    // Used for Online only, clear all the spectra hold by the Spectra class
    void ClearSpectra();
    //
    void WriteSpectra();

  public:      //   Specific to TiaraHyball Array

    //   Clear The PreTeated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

    //   Remove bad channel, calibrate the data and apply threshold
    void PreTreat();

    //   Return false if the channel is disabled by user
    //   Frist argument is either "X","Y","SiLi","CsI"
    bool IsValidChannel(const string DetectorType, const int telescope , const int channel);

    //   Initialize the standard parameter for analysis
    //   ie: all channel enable, maximum multiplicity for strip = number of telescope
    void InitializeStandardParameter();

    //   Read the user configuration file; if no file found, load standard one
    void ReadAnalysisConfig();

    //   Add a Detector
    void AddWedgeDetector( double R,double Phi,double Z);

    // Give and external TMustData object to TTiaraHyballPhysics. Needed for online analysis for example.
    void SetRawDataPointer(TTiaraHyballData* rawDataPointer) {m_EventData = rawDataPointer;}
    // Retrieve raw and pre-treated data
    TTiaraHyballData* GetRawData()        const {return m_EventData;}
    TTiaraHyballData* GetPreTreatedData() const {return m_PreTreatedData;}

    // Use to access the strip position
    double GetStripPositionX( const int N , const int Ring , const int Sector )   const{ return m_StripPositionX[N-1][Ring-1][Sector-1] ; }  ;
    double GetStripPositionY( const int N , const int Ring , const int Sector )   const{ return m_StripPositionY[N-1][Ring-1][Sector-1] ; }  ;
    double GetStripPositionZ( const int N , const int Ring , const int Sector )   const{ return m_StripPositionZ[N-1][Ring-1][Sector-1] ; }  ;

//    double GetNumberOfDetector() const { return m_NumberOfDetector; };

    // To be called after a build Physical Event 
    int GetEventMultiplicity() const { return EventMultiplicity; };

    TVector3 GetPositionOfInteraction(const int i) const;
    TVector3 GetRandomisedPositionOfInteraction(const int i) const;   
    TVector3 GetDetectorNormal(const int i) const;

  private:   //   Parameter used in the analysis
    // By default take EX and TY.
    bool m_Take_E_Ring;//!
    bool m_Take_T_Sector;//!

    //   Event over this value after pre-treatment are not treated / avoid long treatment time on spurious event   
    unsigned int m_MaximumStripMultiplicityAllowed  ;//!
    //   Give the allowance in percent of the difference in energy between X and Y
    double m_StripEnergyMatchingSigma  ; //!
    double m_StripEnergyMatchingNumberOfSigma  ; //!

    //  Threshold
    double m_StripRing_E_RAW_Threshold ;//!
    double m_StripRing_E_Threshold ;//!
    double m_StripSector_E_RAW_Threshold ;//!
    double m_StripSector_E_Threshold ;//!
    double m_Maximum_FrontBack_Difference;//!
  private:   //   Root Input and Output tree classes
    TTiaraHyballData*         m_EventData;//!
    TTiaraHyballData*         m_PreTreatedData;//!
    TTiaraHyballPhysics*      m_EventPhysics;//!

  private:   //   Map of activated channel
    map< int, vector<bool> > m_RingChannelStatus;//!
    map< int, vector<bool> > m_SectorChannelStatus;//! 

  private:   //   Spatial Position of Strip Calculated on bases of detector position
    int m_NumberOfDetector;//!
    vector< vector < vector < double > > >   m_StripPositionX;//!
    vector< vector < vector < double > > >   m_StripPositionY;//!
    vector< vector < vector < double > > >   m_StripPositionZ;//!

  private: // Spectra
    TTiaraHyballSpectra*      m_Spectra;//!

  public:
    map< string,TH1* > GetSpectra(); 

    public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TTiaraHyballPhysics,1)  // SharcPhysics structure
};

namespace TiaraHyball_LOCAL{
  //   tranform an integer to a string
  string itoa(unsigned int value);
  //   DSSD
  //   Ring
  double fStrip_Ring_E(const TTiaraHyballData* Data, const int i);
  double fStrip_Ring_Matchstick(const TTiaraHyballData* Data, const int i);
  double fStrip_Ring_T(const TTiaraHyballData* Data, const int i);

  //   Sector   
  double fStrip_Sector_E(const TTiaraHyballData* Data, const int i);
  double fStrip_Sector_Matchstick(const TTiaraHyballData* Data, const int i);
  double fStrip_Sector_T(const TTiaraHyballData* Data, const int i);
}

#endif
