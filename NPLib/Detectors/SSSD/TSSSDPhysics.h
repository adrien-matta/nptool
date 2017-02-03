#ifndef __SSSDPhysics__
#define __SSSDPhysics__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the Single Side Striped Detector  Physics                *
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

//   NPL
#include "TSSSDData.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"
#include "NPInputParser.h"

class TSSSDPhysics : public TObject, public NPL::VDetector{
 public:   //   Constructor and Destructor
   TSSSDPhysics();
   ~TSSSDPhysics();

 public:   //   Calibrated Data
   vector<UShort_t>   DetectorNumber;
   vector<UShort_t>   StripNumber;
   vector<Double_t>   Energy;
   vector<Double_t>   Time;

 public:   //   inherrited from VDetector
   // Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
   void ReadConfiguration(NPL::InputParser);
      
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

   // Same as above but for online analysis
   void BuildOnlinePhysicalEvent()  {BuildSimplePhysicalEvent();};

   // Those two method all to clear the Event Physics or Data
   void ClearEventPhysics() {Clear();}
   void ClearEventData()    {EventData->Clear();}


 public: //   Specific to SSSD
   // Clear The PreTeated object
   void ClearPreTreatedData()   {PreTreatedData->Clear();}
      
   // Remove bad channel, calibrate the data and apply threshold
   void PreTreat();

   // Initialize the standard parameter for analysis
   // ie: all channel enable, maximum multiplicity for strip = number of telescope
   void InitializeStandardParameter();
   
   //   Read the user configuration file; if no file found, load standard one
   void ReadAnalysisConfig();

   // Give and external TMustData object to TMust2Physics. Needed for online analysis for example.
   void SetRawDataPointer(TSSSDData* rawDataPointer) {EventData = rawDataPointer;}
   // Retrieve raw and pre-treated data
   TSSSDData* GetRawData()        const {return EventData;}
   TSSSDData* GetPreTreatedData() const {return PreTreatedData;}


 private:   // Data not written in the tree
   int           NumberOfDetector;//!
   TSSSDData*    EventData;//!
   TSSSDData*    PreTreatedData;//!
   TSSSDPhysics* EventPhysics;//!

   double m_E_Threshold;//!
   double m_Pedestal_Threshold;//!
            

 private: //  Map of activated Channel
   map< int, vector<bool> > ChannelStatus;//!
    
 public:  
   void Clear();
   void Clear(const Option_t*) {};
   
   public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TSSSDPhysics,1)  // SSSDPhysics structure
};


namespace SSSD_LOCAL
{
   double fSi_E( const TSSSDData* EventData , const int i );
   double fSi_T( const TSSSDData* EventData , const int i );
}

#endif
