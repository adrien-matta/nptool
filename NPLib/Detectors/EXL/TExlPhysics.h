#ifndef __ExlPhysics__
#define __ExlPhysics__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre    contact address: lefebvrl@ipno.in2p3.fr   *
 *                                                                           *
 * Creation Date  : October 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the Exl Detector  Physics                                *
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
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"

//   NPL
#include "TExlData.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"
#include "NPInputParser.h"
class TExlPhysics : public TObject, public NPL::VDetector
{
   public:   //   Constructor and Destructor
      TExlPhysics();
      ~TExlPhysics();

   public:
      void  Clear();
      void  Clear(const Option_t*) {};
   
   public:   //   Calibrated Data
      vector<UShort_t>   CrystalNumber ;
      vector<Double_t>   EXL_Energy;
      Double_t EXL_Time;

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

      //Doppler effect correction
      double DopplerCorrection(double Energy, double Theta, double beta);

      // Same as above but for online analysis
      void BuildOnlinePhysicalEvent()  {BuildPhysicalEvent();};
 
      //Add 18 CsI EXL crystal
      void AddEXL(vector <TVector3> Center_CsI_Crystals);

      //Get Position Of Interaction
      TVector3 GetPositionOfInteraction(int N);   

      // Give and external TExlData object to TExlPhysics. Needed for online analysis for example.
      void SetRawDataPointer(TExlData* rawDataPointer) {EventData = rawDataPointer;}

      //   Those two method all to clear the Event Physics or Data
      void ClearEventPhysics() {Clear();}      
      void ClearEventData()    {EventData->Clear();}      

   private:   // Data not writted in the tree
      unsigned int                   NumberOfCrystal ;//!
      TExlData*         EventData ;//!
      TExlPhysics*      EventPhysics ;//!
         
        // Raw Threshold
         int m_E_RAW_Threshold ;//!
	//CsI crystal position
         vector <TVector3> CsIPosition;//!

      public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TExlPhysics,1)  // ExlPhysics structure
};

#endif
