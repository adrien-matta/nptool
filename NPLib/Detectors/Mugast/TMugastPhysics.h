#ifndef TMUGASTPHYSICS_H
#define TMUGASTPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold must2 treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// STL
#include <map>
#include <vector>
// NPL
#include "NPCalibrationManager.h"
#include "NPInputParser.h"
#include "NPVDetector.h"
#include "TMugastData.h"
#include "TMugastSpectra.h"
// ROOT
#include "TH1.h"
#include "TObject.h"
#include "TVector2.h"
#include "TVector3.h"

using namespace std;

// Forward Declaration
class TMugastSpectra;

class TMugastPhysics : public TObject, public NPL::VDetector {
  public:
  TMugastPhysics();
  ~TMugastPhysics();

  public:
  void Clear();
  void Clear(const Option_t*){};

  public:
  vector<TVector2> Match_X_Y();
  bool Match_SecondLayer(int X, int Y, int StripNbr);
  bool ResolvePseudoEvent();
  int  CheckEvent();

  public:
  //   Provide Physical Multiplicity
  Int_t EventMultiplicity;

  //   Provide a Classification of Event
  vector<int> EventType;

  // Telescope
  vector<int> TelescopeNumber;
  map<int,MG_DetectorType> DetectorType;
  //   DSSD
  vector<double> DSSD_E;
  vector<double> DSSD_T;
  vector<int>    DSSD_X;
  vector<int>    DSSD_Y;

  //   Second Layer
  vector<double> SecondLayer_E;
  vector<double> SecondLayer_T;
  vector<int>    SecondLayer_N;

  // Physical Value
  vector<double> TotalEnergy;

  public: //   Innherited from VDetector Class
  //   Read stream at ConfigFile to pick-up parameters of detector
  //   (Position,...) using Token
  void ReadConfiguration(NPL::InputParser parser);

  //   Add Parameter to the CalibrationManger
  void AddParameterToCalibrationManager();

  //   Activated associated Branches and link it to the private member
  //   DetectorData address
  //   In this method mother Branches (Detector) AND daughter leaf
  //   (fDetector_parameter) have to be activated
  void InitializeRootInputRaw();

  //   Activated associated Branches and link it to the private member
  //   DetectorPhysics address
  //   In this method mother Branches (Detector) AND daughter leaf (parameter)
  //   have to be activated
  void InitializeRootInputPhysics();

  //   Create associated branches and associated private member DetectorPhysics
  //   address
  void InitializeRootOutput();

  //   This method is called at each event read from the Input Tree. Aime is to
  //   build treat Raw dat in order to extract physical parameter.
  void BuildPhysicalEvent();

  //   Same as above, but only the simplest event and/or simple method are used
  //   (low multiplicity, faster algorythm but less efficient ...).
  //   This method aimed to be used for analysis performed during experiment,
  //   when speed is requiered.
  //   NB: This method can eventually be the same as BuildPhysicalEvent.
  void BuildSimplePhysicalEvent();

  // Same as above but for online analysis
  void BuildOnlinePhysicalEvent() { BuildPhysicalEvent(); };

  //   Those two method all to clear the Event Physics or Data
  void ClearEventPhysics() { Clear(); }
  void ClearEventData() { m_EventData->Clear(); }

  // Method related to the TSpectra classes, aimed at providing a framework for
  // online applications
  // Instantiate the Spectra class and the histogramm throught it
  void InitSpectra();
  // Fill the spectra hold by the spectra class
  void FillSpectra();
  // Used for Online mainly, perform check on the histo and for example change
  // their color if issues are found
  void CheckSpectra();
  // Used for Online only, clear all the spectra hold by the Spectra class
  void ClearSpectra();

  public: //   Specific to MUGAST Array
  //   Clear The PreTeated object
  void ClearPreTreatedData() { m_PreTreatedData->Clear(); }

  //   Remove bad channel, calibrate the data and apply threshold
  void PreTreat();

  //   Return false if the channel is disabled by user
  //   Frist argument is either 0 for X,1 Y,2 SecondLayer 3
  bool IsValidChannel(const int& DetectorType, const int& telescope,
                      const int& channel);

  //   Initialize the standard parameter for analysis
  //   ie: all channel enable, maximum multiplicity for strip = number of
  //   telescope
  void InitializeStandardParameter();

  //   Read the user configuration file; if no file found, load standard one
  void ReadAnalysisConfig();

  //   Add a Telescope using Corner Coordinate information
  void AddTelescope(TVector3 C_X1_Y1, TVector3 C_X128_Y1, TVector3 C_X1_Y128,
                    TVector3 C_X128_Y128);

  //   Add a Telescope using R Theta Phi of Si center information
  void AddTelescope(double theta, double phi, double distance, double beta_u,
                    double beta_v, double beta_w);

 //   Special Method for Annular S1
  void AddTelescope(TVector3 C_Center);

  // Give and external TMustData object to TMugastPhysics. Needed for online
  // analysis for example.
  void SetRawDataPointer(void* rawDataPointer) {
    m_EventData = (TMugastData*)rawDataPointer;
  }
  // Retrieve raw and pre-treated data
  TMugastData* GetRawData() const { return m_EventData; }
  TMugastData* GetPreTreatedData() const { return m_PreTreatedData; }

  // Use to access the strip position
  double GetStripPositionX(const int N, const int X, const int Y) {
    return m_StripPositionX[ m_DetectorNumberIndex[N] - 1][X - 1][Y - 1];
  };
  double GetStripPositionY(const int N, const int X, const int Y) {
    return m_StripPositionY[ m_DetectorNumberIndex[N] - 1][X - 1][Y - 1];
  };
  double GetStripPositionZ(const int N, const int X, const int Y) {
    return m_StripPositionZ[ m_DetectorNumberIndex[N] - 1][X - 1][Y - 1];
  };

  double GetNumberOfTelescope() const { return m_NumberOfTelescope; };

  // To be called after a build Physical Even
  int GetEventMultiplicity() const { return EventMultiplicity; };

  double GetEnergyDeposit(const int i) const { return TotalEnergy[i]; };

  TVector3 GetPositionOfInteraction(const int i) ;
  TVector3 GetTelescopeNormal(const int i) ;

  private: //   Parameter used in the analysis
  // Shape of the detector Trapezoid or Square
  map<int, int> m_DetectorNumberIndex; 

  // By default take EX and TY.
  bool m_Take_E_Y; //!
  bool m_Take_T_Y; //!

  //   Event over this value after pre-treatment are not treated / avoid long
  //   treatment time on spurious event
  unsigned int m_MaximumStripMultiplicityAllowed; //!
  //   Give the allowance in percent of the difference in energy between X and Y
  double m_StripEnergyMatching; //!

  // Raw Threshold
  int m_DSSD_X_E_RAW_Threshold; //!
  int m_DSSD_Y_E_RAW_Threshold; //!
  int m_SecondLayer_E_RAW_Threshold; //!

    // Calibrated Threshold
  double m_DSSD_X_E_Threshold; //!
  double m_DSSD_Y_E_Threshold; //!
  double m_SecondLayer_E_Threshold; //!

  private: //   Root Input and Output tree classes
  TMugastData*    m_EventData; //!
  TMugastData*    m_PreTreatedData; //!
  TMugastPhysics* m_EventPhysics; //!

  private: //   Map of activated channel
  map<int, vector<bool>> m_XChannelStatus; //!
  map<int, vector<bool>> m_YChannelStatus; //!
  map<int, vector<bool>> m_SecondLayerChannelStatus; //!

  private: // Spatial Position of Strip Calculated on bases of detector position
  int m_NumberOfTelescope; //!

  vector<vector<vector<double>>> m_StripPositionX; //!
  vector<vector<vector<double>>> m_StripPositionY; //!
  vector<vector<vector<double>>> m_StripPositionZ; //!

  private:
  map<int, int>    m_HitDSSDX; //!
  map<int, int>    m_HitDSSDY; //!

  private: // Spectra Class
  TMugastSpectra* m_Spectra; //!
  
  public:
  void WriteSpectra(); //!

  public: // Spectra Getter
  map<string, TH1*> GetSpectra();

  public: // Static constructor to be passed to the Detector Factory
  static NPL::VDetector* Construct();
  ClassDef(TMugastPhysics, 1) // MugastPhysics structure
};

namespace MUGAST_LOCAL {
//   DSSD
//   X
double fDSSD_X_E(const TMugastData* Data, const int& i);
double fDSSD_X_T(const TMugastData* Data, const int& i);

//   Y
double fDSSD_Y_E(const TMugastData* Data, const int& i);
double fDSSD_Y_T(const TMugastData* Data, const int& i);

//  Second Layer 
double fSecondLayer_E(const TMugastData* Data, const int& i);
double fSecondLayer_T(const TMugastData* Data, const int& i);
}

#endif
