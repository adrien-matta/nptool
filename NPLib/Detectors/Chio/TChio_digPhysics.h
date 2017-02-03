#ifndef __CHIODIGCAL__
#define __CHIODIGCAL__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Sandra Giron  contact address: giron@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold chio treated data                                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

//   STL
#include <vector>
#include <algorithm>

//   ROOT
#include "TObject.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TF1.h"

//   NPLib
#include "TChio_digData.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"
#include "NPInputParser.h"
using namespace std ;

class TChio_digPhysics : public TObject, public NPL::VDetector
{
  
 public:   //   Constructor and Destructor
  TChio_digPhysics();
  ~TChio_digPhysics();
   
 private:   // Output data of interest
  
  ///// this is private in TChio_digCal.h but public in TPlastic_digPhysics.h...??

  // ADC
  vector<double> Energy;
  /* Double_t Energy[350]; */
  //sum
  // --- used in Ionization chamber
  /* vector<double> Sum; */
  /* vector<double> Ampl; */
  /* vector<double> Sum_sscorr; */
  /* vector<double> Ampl_sscorr; */
  /* vector<short> Peak; */
  /* vector<double> Peak_dist; */
  /* vector<short> Peak_stop; */
  /* vector<double> Peak_base; */
  /* int PileupNbr; */
  // --- modified by megumi
  int Npileup; // number of pileup
   
  vector<int> peak_start; // start of peak
  vector<int> peak_stop;  // top of peak
  vector<int> risetime;
  vector<double> rawAmplitude;
  vector<double> calAmplitude;

  TF1 *fdecay;

 private:   //   Root Input and Output tree classes
     
  /* TChio_digData*      EventData;//! */
  TChio_digData*      EventData;//!
  TChio_digPhysics*   EventPhysics;//!
     
 public:      //   Inherited from VDetector Class
     
  //   Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
  void ReadConfiguration(NPL::InputParser);
     
  //   Add Parameter to the CalibrationManger
  void AddParameterToCalibrationManager();      
     
  //   Activated associated Branches and link it to the private member DetectorData address
  //   In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
  void InitializeRootInputRaw() ;

  //   Activated associated Branches and link it to the private member DetectorPhysics address
  //   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
  void InitializeRootInputPhysics() ;
   
  //   Create associated branches and associated private member DetectorPhysics address
  void InitializeRootOutput() ;
     
  //   This method is called at each event read from the Input Tree. Aim is to build treat Raw dat in order to extract physical parameter. 
  void BuildPhysicalEvent();
     
  //   Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
  //   This method aimed to be used for analysis performed during experiment, when speed is requiered.
  //   NB: This method can eventually be the same as BuildPhysicalEvent.
  void BuildSimplePhysicalEvent();
     
   // Same as above but for online analysis
   void BuildOnlinePhysicalEvent()  {BuildPhysicalEvent();};

  //   Those two method all to clear the Event Physics or Data
  void ClearEventPhysics() {Clear();}      
  void ClearEventData()    {EventData->Clear();}      
     
 public :   //   Specific to ionisation chamber

  void   Clear();
  void   Clear(const Option_t*) {};
  void   Dump() const;
   
  // Give and external TMustChio_digData object to TChio_anPhysics. Needed for online analysis.
   void SetRawDataPointer(TChio_digData* rawDataPointer) {EventData = rawDataPointer;}

   //    double  GetChioEnergy() {return ...}
  
  public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TChio_digPhysics,1);  // Physics structure
  
};


namespace LOCAL_CHIO_DIG
{
  double CalibRawAmplitude(double);
}

#endif
