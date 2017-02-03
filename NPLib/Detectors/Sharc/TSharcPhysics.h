#ifndef TSHARCPHYSICS_H
#define TSHARCPHYSICS_H
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
 *  This class hold Sharc treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *  
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// STL
#include <vector>

// NPL
#include "TSharcData.h"
#include "TSharcSpectra.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"
#include "TCanvas.h"
#include "TRandom3.h"
// Forward declaration
class TSharcSpectra;


using namespace std ;

class TSharcPhysics : public TObject, public NPL::VDetector{
  public:
    TSharcPhysics();
    ~TSharcPhysics() {};

  public: 
    void Clear();   
    void Clear(const Option_t*) {};

  public: 
    vector < TVector2 > Match_Front_Back() ;
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
    vector<double> StripFront_E ;
    vector<double> StripFront_T ;
    vector<double> StripBack_E ;
    vector<double> StripBack_T ;
    vector<int>    Strip_Front ;
    vector<int>    Strip_Back ;

    vector<double> PAD_E ;
    vector<double> PAD_T ;

    // Used to apply Pixel Cal
    vector<double> StripFront_OriginalE; //!
    vector<double> DeadLayer; //!    
    // Used for Calibration
    vector<double> Strip_Front_RawE;

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
    // Write Spectra to file
    void WriteSpectra();

  public:      //   Specific to Sharc Array

    //   Clear The PreTeated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

    //   Remove bad channel, calibrate the data and apply threshold
    void PreTreat();

    //   Return false if the channel is disabled by user
    //   Frist argument is either "X","Y","SiLi","CsI"
    bool IsValidChannel(const string& DetectorType, const int& telescope , const int& channel);

    //   Initialize the standard parameter for analysis
    //   ie: all channel enable, maximum multiplicity for strip = number of telescope
    void InitializeStandardParameter();

    //   Read the user configuration file; if no file found, load standard one
    void ReadAnalysisConfig();

    //   Add a Detector
    void AddBoxDetector( double Z);
    void AddQQQDetector( double R,double Phi,double Z);

    // Give and external TMustData object to TSharcPhysics. Needed for online analysis for example.
    void SetRawDataPointer(TSharcData* rawDataPointer) {m_EventData = rawDataPointer;}
    // Retrieve raw and pre-treated data
    TSharcData* GetRawData()        const {return m_EventData;}
    TSharcData* GetPreTreatedData() const {return m_PreTreatedData;}

    // Use to access the strip position
    inline double GetStripPositionX( const int& N , const int& Front , const int& Back )   const{ return m_StripPositionX[N-1][Front-1][Back-1] ; }  ;
    inline double GetStripPositionY( const int& N , const int& Front , const int& Back )   const{ return m_StripPositionY[N-1][Front-1][Back-1] ; }  ;
    inline double GetStripPositionZ( const int& N , const int& Front , const int& Back )   const{ return m_StripPositionZ[N-1][Front-1][Back-1] ; }  ;

    inline double GetNumberOfDetector() const { return m_NumberOfDetector; };

    // To be called after a build Physical Event 
    inline int GetEventMultiplicity() const { return EventMultiplicity; };

    TVector3 GetPositionOfInteraction(const int& i, bool random=false) const;   
    TVector3 GetDetectorNormal(const int& i) const;
    double   GetDeadLayer(const int& i) const;

  private:   //   Parameter used in the analysis

    // By default take EX and TY.
    bool m_Take_E_Front;//!
    bool m_Take_T_Back;//!

    //   Event over this value after pre-treatment are not treated / avoid long treatment time on spurious event   
    unsigned int m_MaximumStripMultiplicityAllowed  ;//!
    //   Give the allowance in percent of the difference in energy between X and Y
    double m_StripEnergyMatchingSigma  ; //!
    double m_StripEnergyMatchingNumberOfSigma  ; //!

    //  Threshold
    double m_StripFront_E_RAW_Threshold ;//!
    double m_StripFront_E_Threshold ;//!
    double m_StripBack_E_RAW_Threshold ;//!
    double m_StripBack_E_Threshold ;//!
    double m_PAD_E_RAW_Threshold ;//!
    double m_PAD_E_Threshold ;//!

  private:   //   Root Input and Output tree classes

    TSharcData*         m_EventData;//!
    TSharcData*         m_PreTreatedData;//!
    TSharcPhysics*      m_EventPhysics;//!


  private:   //   Map of activated channel
    map< int, vector<bool> > m_FrontChannelStatus;//!
    map< int, vector<bool> > m_BackChannelStatus;//! 
    map< int, vector<bool> > m_PADChannelStatus;//!

  private:   //   Spatial Position of Strip Calculated on bases of detector position

    int m_NumberOfDetector;//!
    vector< vector < vector < double > > >   m_StripPositionX;//!
    vector< vector < vector < double > > >   m_StripPositionY;//!
    vector< vector < vector < double > > >   m_StripPositionZ;//!
    vector< TVector3 > m_DetectorNormal;//!
    vector< TVector3 > m_U;//!
    vector< TVector3 > m_V;//!
    TRandom3* m_Rand;//!
    double m_BoxPitchBack ;//!
    double m_BoxPitchFront;//!
    double m_QQQPitchBack ;//!
    double m_QQQPitchFront;//!

  private: // Spectra Class
    TSharcSpectra* m_Spectra; // !

  public: // Spectra Getter
    map< string , TH1*> GetSpectra(); 
    vector<TCanvas*> GetCanvas();

  public: // Static constructor to be passed to the Detector Factory
    static NPL::VDetector* Construct();
    ClassDef(TSharcPhysics,1)  // SharcPhysics structure
};

namespace Sharc_LOCAL
{
  //   DSSD
  //   Front
  double fStrip_Front_E(const TSharcData* Data, const int& i);
  double fStrip_Front_T(const TSharcData* Data, const int& i);

  //   Back   
  double fStrip_Back_E(const TSharcData* Data, const int& i);
  double fStrip_Back_T(const TSharcData* Data, const int& i);

  //   PAD   
  double fPAD_E(const TSharcData* Data, const int& i);
  double fPAD_T(const TSharcData* Data, const int& i);

}


#endif
