#ifndef TTrifoilPHYSICS_H
#define TTrifoilPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : Devember 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Trifoil treated data                                     *
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
#include "TTrifoilData.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"

using namespace std ;

class TTrifoilPhysics : public TObject, public NPL::VDetector
{
   public:
      TTrifoilPhysics();
      ~TTrifoilPhysics() {};

   public: 
      void Clear();   
      void Clear(const Option_t*) {};
         
   public:
      // EventType is True if the wave form analysis return a valid Trifoil event
      vector<double>  Energy;
      vector<double>  Time ;
      vector<double>  TimeStamp ;

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
   
   public:      //   Specific to Trifoil
  
      // Retrieve raw and pre-treated data
      TTrifoilData* GetRawData()        const {return m_EventData;}  

    private:   //   Root Input and Output tree classes
            
         TTrifoilData*         m_EventData;//!
         TTrifoilPhysics*      m_EventPhysics;//!

        
   public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TTrifoilPhysics,1)  // TrifoilPhysics structure
};

#endif
