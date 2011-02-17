#ifndef __W1Physics__
#define __W1Physics__
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : january 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class holds the physics class for the W1 detector from Micron    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
//   STL
#include <vector>
using namespace std ;

//   ROOT
#include "TObject.h"
#include "TVector2.h"
#include "TVector3.h"

//   NPL
#include "TW1Data.h"
#include "../include/VDetector.h"
#include "../include/CalibrationManager.h"


class TW1Physics : public TObject, public NPA::VDetector
{
 public:   //   Constructor and Destructor
   TW1Physics();
   ~TW1Physics();

 public:
   void Clear();
   void Clear(const Option_t*) {};
   

 private: // data obtained after BuildPhysicalEvent() and stored in ROOT output file
   vector<Int_t>     fEventType;
   vector<Int_t>     fDetectorNumber;

   // First stage
   vector<Double_t>  fEnergy;
   vector<Double_t>  fTime;
   vector<Int_t>     fFrontStrip;
   vector<Int_t>     fBackStrip;

 public:
   // setters
   void SetEventType(Int_t evtType)          {fEventType.push_back(evtType);}
   void SetDetectorNumber(Int_t moduleNbr)   {fDetectorNumber.push_back(moduleNbr);}
   void SetEnergy(Double_t ener)             {fEnergy.push_back(ener);}
   void SetTime(Double_t time)               {fTime.push_back(time);}
   void SetFrontStrip(Int_t x)               {fFrontStrip.push_back(x);}
   void SetBackStrip(Int_t y)                {fBackStrip.push_back(y);}

   // getters
   Int_t    GetEventMultiplicity()           {return fEnergy.size();}
   Int_t    GetEventType(Int_t i)            {return fEventType.at(i);}
   Int_t    GetDetectorNumber(Int_t i)       {return fDetectorNumber.at(i);}
   Double_t GetEnergy(Int_t i)               {return fEnergy.at(i);}
   Double_t GetTime(Int_t i)                 {return fTime.at(i);}
   Int_t    GetFrontStrip(Int_t i)           {return fFrontStrip.at(i);}
   Int_t    GetBackStrip(Int_t i)            {return fBackStrip.at(i);}


 public:   //   inherrited from VDetector
   // Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
   void ReadConfiguration(string);
      
   // Add Parameter to the CalibrationManger
   void AddParameterToCalibrationManager();      

   //   Activated associated Branches and link it to the private member DetectorData address
   //   In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
   void InitializeRootInputRaw() ;

   //   Activated associated Branches and link it to the private member DetectorPhysics address
   //   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
   void InitializeRootInputPhysics() ;

   // Create associated branches and associated private member DetectorPhysics address
   void InitializeRootOutput();

   // This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
   void BuildPhysicalEvent();
      
   // Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
   // This method aimed to be used for analysis performed during experiment, when speed is requiered.
   // NB: This method can eventually be the same as BuildPhysicalEvent.
   void BuildSimplePhysicalEvent();

   // Those two method all to clear the Event Physics or Data
   void ClearEventPhysics()   {Clear();}
   void ClearEventData()      {m_EventData->Clear();}


 public: //   Specific to W1
   // Remove bad channel, calibrate the data and apply threshold
   void PreTreat();

   // Clear The PreTeated object
   void ClearPreTreatedData()   {m_PreTreatedData->Clear();}
      
   // Return false if the channel is disabled by user
   // Frist argument is either "Front" or "Back"
   bool IsValidChannel(string Type, int detector, int channel);

   // Initialize the standard parameter for analysis
   // ie: all channel enable, maximum multiplicity for strip = number of telescope
   void InitializeStandardParameter();
   
   //   Read the user configuration file; if no file found, load standard one
   void ReadAnalysisConfig();

   // Add detector using cartesian coordiantes
   void AddDetector(TVector3 C_X1_Y1, TVector3 C_X16_Y1, TVector3 C_X1_Y16, TVector3 C_X16_Y16);

   // Add detector using spherical coordinates
   void AddDetector(double theta, double phi, double distance, double beta_u, double beta_v, double beta_w);

   // Use for reading Calibration Run, very simple methods; only apply calibration, no condition
   void ReadCalibrationRun() {};


 public: // Methods used for event treatement
   int               EventType();
   vector<TVector2>  Match_Front_Back();

 private: // Data not written in the tree
   TW1Data*    m_EventData;            //!
   TW1Data*    m_PreTreatedData;       //!
   TW1Physics* m_EventPhysics;         //!


 private: // Map of activated Channel
   map< int, vector<bool> > m_FrontChannelStatus;  //!
   map< int, vector<bool> > m_BackChannelStatus;   //!
   

 private: // Parameters used in the analysis
   // If multiplicity is greated than m_MaximumStripMultiplicityAllowed 
   // after PreTreat(), event is not treated
   int m_MaximumStripMultiplicityAllowed; //!
   // Tolerance in percent for the energy difference between Front and Back
   double m_StripEnergyMatchingTolerance; //!


 private: // Energy thresholds
   // Raw Threshold
   int    m_FrontE_Raw_Threshold;   //!
   int    m_BackE_Raw_Threshold;    //!
   // Calibrated Threshold
   double m_FrontE_Calib_Threshold; //!
   double m_BackE_Calib_Threshold;  //!


 private: // Spatial Position of Strip Calculated on bases of detector position
   int m_NumberOfDetector;   //!
   vector< vector < vector < double > > > m_StripPositionX; //!
   vector< vector < vector < double > > > m_StripPositionY; //!
   vector< vector < vector < double > > > m_StripPositionZ; //!

 public:
   double   GetNumberOfDetectors()                   {return m_NumberOfDetector;};
   double   GetStripPositionX(int N, int X, int Y)   {return m_StripPositionX[N-1][X-1][Y-1];};
   double   GetStripPositionY(int N, int X, int Y)   {return m_StripPositionY[N-1][X-1][Y-1];};
   double   GetStripPositionZ(int N, int X, int Y)   {return m_StripPositionZ[N-1][X-1][Y-1];};
   TVector3 GetPositionOfInteraction(int i);
   TVector3 GetDetectorNormal(int i);
   void     DumpStrippingScheme(int detecNumber);


 private:   // Geometry and strip number
   double m_SiliconFace;      //!     // mm
   int    m_NumberOfStrips;   //!
   double m_StripPitch;       //!


   ClassDef(TW1Physics,1)  // TW1Physics
};


namespace LOCAL
{
   double fW1_Front_E(TW1Data* EventData, int i);
   double fW1_Front_T(TW1Data* EventData, int i);
   double fW1_Back_E(TW1Data*  EventData, int i);
   double fW1_Back_T(TW1Data*  EventData, int i);
}

#endif
