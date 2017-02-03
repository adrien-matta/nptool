#ifndef TCHARISSAPHYSICS_H
#define TCHARISSAPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : December 2013                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold charissa treated data                                       *
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
#include "TCharissaData.h"
#include "TCharissaSpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// ROOT
#include "TVector2.h"
#include "TVector3.h"
#include "TObject.h"
#include "TH1.h"

using namespace std ;

// Forward Declaration
class TCharissaSpectra;

class TCharissaPhysics : public TObject, public NPL::VDetector{
  public:
    TCharissaPhysics();
    ~TCharissaPhysics(); 

  public:
    void Clear();
    void Clear(const Option_t*) {};

  public:
    vector < TVector2 > Layer1_Match_X_Y();
	vector < TVector2 > Layer2_Match_X_Y();
	bool	ResolvePseudoEvent();
  int		Layer1_CheckEvent();
	int		Layer2_CheckEvent();

  public:

    //   Provide Physical Multiplicity
    unsigned int EventMultiplicity;

    //   Provide a Classification of Event
    vector<int> EventType ;

    // Telescope
    vector<int> TelescopeNumber ;

    //   Si
    vector<double> Layer1_Si_E ;
    vector<double> Layer1_Si_T ;
    vector<int>    Layer1_Si_X ;
    vector<int>    Layer1_Si_Y ;
	
	  vector<double> Layer2_Si_E ;
    vector<double> Layer2_Si_T ;
    vector<int>    Layer2_Si_X ;
    vector<int>    Layer2_Si_Y ;

    // Use for checking purpose
    vector<double> Layer1_Si_EX ;
    vector<double> Layer1_Si_TX ;
    vector<double> Layer1_Si_EY ;
    vector<double> Layer1_Si_TY ;
    vector<int>    Layer1_TelescopeNumber_X ;
    vector<int>    Layer1_TelescopeNumber_Y ;
	
	  vector<double> Layer2_Si_EX ;
    vector<double> Layer2_Si_TX ;
    vector<double> Layer2_Si_EY ;
    vector<double> Layer2_Si_TY ;
    vector<int>    Layer2_TelescopeNumber_X ;
    vector<int>    Layer2_TelescopeNumber_Y ;

    //   CsI
    vector<double>  CsI_E ;
    vector<double>  CsI_T ;
    vector<int>     CsI_N ;

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

  public:      //   Specific to CHARISSA Array

    //   Clear The PreTeated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

    //   Remove bad channel, calibrate the data and apply threshold
    void PreTreat();

    //   Return false if the channel is disabled by user
    //   Frist argument is either "X","Y","CsI"
    bool IsValidChannel(const string DetectorType, const int telescope , const int channel);

    //   Initialize the standard parameter for analysis
    //   ie: all channel enable, maximum multiplicity for strip = number of telescope
    void InitializeStandardParameter();

    //   Read the user configuration file; if no file found, load standard one
    void ReadAnalysisConfig();

    //   Add a Telescope using Corner Coordinate information
    void AddTelescope(   TVector3 C_X1_Y1,
        TVector3 C_X128_Y1,
        TVector3 C_X1_Y128,
        TVector3 C_X128_Y128);

    //   Add a Telescope using R Theta Phi of Si center information
    void AddTelescope( double theta,
        double phi,
        double distance,
        double beta_u,
        double beta_v,
        double beta_w);

    // Give and external TMustData object to TCharissaPhysics. Needed for online analysis for example.
    void SetRawDataPointer(TCharissaData* rawDataPointer) {m_EventData = rawDataPointer;}
    // Retrieve raw and pre-treated data
    TCharissaData* GetRawData()        const {return m_EventData;}
    TCharissaData* GetPreTreatedData() const {return m_PreTreatedData;}

    // Use to access the strip position
    double GetStripPositionX( const int N , const int X , const int Y )   const{ return m_StripPositionX[N-1][X-1][Y-1] ; }  ;
    double GetStripPositionY( const int N , const int X , const int Y )   const{ return m_StripPositionY[N-1][X-1][Y-1] ; }  ;
    double GetStripPositionZ( const int N , const int X , const int Y )   const{ return m_StripPositionZ[N-1][X-1][Y-1] ; }  ;

    double GetNumberOfTelescope() const { return m_NumberOfTelescope ; };

    // To be called after a build Physical Event
    int GetEventMultiplicity() const { return EventMultiplicity; };

    TVector3 GetPositionOfInteraction(const int i) const;
    TVector3 GetTelescopeNormal(const int i) const;

  private:   //   Parameter used in the analysis

    // By default take EX and TY.
    bool m_Take_E_Y;//!
    bool m_Take_T_Y;//!

    // Size Container to be used in the different loop of the analysis (value must be given locally)
    unsigned int m_Layer1_StripXEMult;//!
    unsigned int m_Layer1_StripYEMult;//!
    unsigned int m_Layer1_StripXTMult;//!
    unsigned int m_Layer1_StripYTMult;//!
	unsigned int m_Layer2_StripXEMult;//!
    unsigned int m_Layer2_StripYEMult;//!
    unsigned int m_Layer2_StripXTMult;//!
    unsigned int m_Layer2_StripYTMult;//!
    unsigned int m_CsIEMult;//!
    unsigned int m_CsITMult;//!


    //   Event over this value after pre-treatment are not treated / avoid long treatment time on spurious event
    unsigned int m_MaximumStripMultiplicityAllowed  ;//!
    //   Give the allowance in percent of the difference in energy between X and Y
    double m_StripEnergyMatchingSigma  ; //!
    double m_StripEnergyMatchingNumberOfSigma  ; //!

    // Raw Threshold
    int m_Si_X_E_RAW_Threshold ;//!
    int m_Si_Y_E_RAW_Threshold ;//!
    int m_CsI_E_RAW_Threshold  ;//!

    // Calibrated Threshold
    double m_Si_X_E_Threshold ;//!
    double m_Si_Y_E_Threshold ;//!
    double m_CsI_E_Threshold  ;//!

    // Geometric Matching
    // size in strip of a cristal
    int m_CsI_Size;//!
    // center position of the cristal on X
    vector< int > m_CsI_MatchingX;//!
    // center position of the cristal on X
    vector< int > m_CsI_MatchingY;//!

    // If set to true, all event that do not come in front of a cristal will be ignore all time (crossing or not),
    // Warning, this option reduce statistic, however it help eliminating unrealevent event that cross the DSSD
    // And go between pad or cristal.
    bool m_Ignore_not_matching_CsI;//!

  private:   //   Root Input and Output tree classes

    TCharissaData*         m_EventData;//!
    TCharissaData*         m_PreTreatedData;//!
    TCharissaPhysics*      m_EventPhysics;//!

  private:   //   Map of activated channel
    map< int, vector<bool> > m_XChannelStatus;//!
    map< int, vector<bool> > m_YChannelStatus;//!
    map< int, vector<bool> > m_CsIChannelStatus;//!

  private:   //   Spatial Position of Strip Calculated on bases of detector position

    unsigned int m_NumberOfTelescope;//!
	unsigned int m_NumberOfStrip;//!

    vector< vector < vector < double > > >   m_StripPositionX;//!
    vector< vector < vector < double > > >   m_StripPositionY;//!
    vector< vector < vector < double > > >   m_StripPositionZ;//!

  private: // Spectra Class   
    TCharissaSpectra*      m_Spectra;//! 

  public: // Spectra Getter
    map< string , TH1*> GetSpectra(); 

    public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TCharissaPhysics,1)  // CharissaPhysics structure
};

namespace CHARISSA_LOCAL
{

  //   tranform an integer to a string
  string itoa(int value);
  //   DSSD
  
  // Layer 1 
  //   X
  double fDE_X_E(const TCharissaData* Data, const int i);
  double fDE_X_T(const TCharissaData* Data, const int i);

  //   Y
  double fDE_Y_E(const TCharissaData* Data, const int i);
  double fDE_Y_T(const TCharissaData* Data, const int i);

  // Layer 2
  // X
  double fE_X_E(const TCharissaData* Data, const int i);
  double fE_X_T(const TCharissaData* Data, const int i);

  //   Y
  double fE_Y_E(const TCharissaData* Data, const int i);
  double fE_Y_T(const TCharissaData* Data, const int i);


  //   CsI
  double fCsI_E(const TCharissaData* Data, const int i);
  double fCsI_T(const TCharissaData* Data, const int i);

}


#endif
