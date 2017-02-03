#ifndef __PlasticPhysics__
#define __PlasticPhysics__
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
 *  This class hold thePlastic Detector  Physics                             *
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
#include "TPlasticData.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"
#include "NPInputParser.h"

class TPlasticPhysics : public TObject, public NPL::VDetector{
   public:   //   Constructor and Destructor
      TPlasticPhysics();
      ~TPlasticPhysics();

   public:
      void  Clear();
      void  Clear(const Option_t*) {};
   
   public:   //   Calibrated Data
      vector<UShort_t>   DetectorNumber ;
      vector<Double_t>   Energy;
      vector<Double_t>   Time;

   public:   //   inherrited from VDetector
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
      void InitializeRootOutput();
      
      //   This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
      void BuildPhysicalEvent();
      
      //   Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
      //   This method aimed to be used for analysis performed during experiment, when speed is requiered.
      //   NB: This method can eventually be the same as BuildPhysicalEvent.
      void BuildSimplePhysicalEvent();

      // Same as above but for online analysis
      void BuildOnlinePhysicalEvent()  {BuildPhysicalEvent();};

      // Give and external TPlasticData object to TPlasticPhysics. Needed for online analysis for example.
      void SetRawDataPointer(TPlasticData* rawDataPointer) {EventData = rawDataPointer;}

      //   Those two method all to clear the Event Physics or Data
      void ClearEventPhysics() {Clear();}      
      void ClearEventData()    {EventData->Clear();}      

   private:   // Data not writted in the tree
      int                   NumberOfDetector ;//!
      TPlasticData*         EventData ;//!
      TPlasticPhysics*      EventPhysics ;//!

      public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TPlasticPhysics,1)  // PlasticPhysics structure
};

#endif
