#ifndef __ModularLeafPhysics__
#define __ModularLeafPhysics__
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
 *  This class hold theModularLeaf Detector  Physics                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
 
//   STL
#include <vector>

//   ROOT
#include "TObject.h"
#include "TVector2.h"
#include "TVector3.h"

//   NPL
#include "TModularLeafSpectra.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"
#include "NPInputParser.h"

using namespace std ;

// Forward Declaration
class TModularLeafSpectra;

class TModularLeafPhysics : public TObject, public NPL::VDetector{
   public:   //   Constructor and Destructor
      TModularLeafPhysics();
      ~TModularLeafPhysics();

   public:
      void  Clear();
      void  Clear(const Option_t*) {};
   
   private:   //   Raw and Calibrated Data
      double m_DefaultValue; // !
      std::map<std::string, double> m_CalibratedData;
      std::map<std::string, short> m_RawData; 
       
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

      // Give and external TModularLeafData object to TModularLeafPhysics. Needed for online analysis for example.
      // Retrieve raw and pre-treated data

      //   Those two method all to clear the Event Physics or Data
      void ClearEventPhysics() {Clear();}      
      void ClearEventData();      

      // Needed for Online spectra
      void InitSpectra();
      void FillSpectra();
      void CheckSpectra();
      void ClearSpectra();
      void WriteSpectra() ;
      map< string , TH1*> GetSpectra() ;

    private: // Spectra Class
        TModularLeafSpectra* m_Spectra;//!

   private:   // Data not writted in the tree
      int                       m_NumberOfDetector ;//!
      TModularLeafPhysics*      m_EventPhysics ;//!
      std::vector<std::string>  m_leafs;//!
      std::vector<std::string>  m_leafs_X;//!
      std::vector<std::string>  m_leafs_Y;//!

   public:
      inline double GetCalibratedValue(const std::string& label){return m_CalibratedData[label];};
      inline void SetModularData(const string& lbl, const Short_t& val){m_RawData[lbl] = val;};

   public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TModularLeafPhysics,1)  // ModularLeafPhysics structure
};

#endif
