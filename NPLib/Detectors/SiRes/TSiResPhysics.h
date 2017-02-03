#ifndef __SiResPhysics__
#define __SiResPhysics__
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
 *  This class hold theSiRes Detector  Physics                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
 
//   STL
#include <vector>
using namespace std ;

class TSiResSpectra;
//   ROOT
#include "TObject.h"

//   NPL
#include "TSiResData.h"
#include "TSiResSpectra.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"
#include "NPInputParser.h"
class TSiResPhysics : public TObject, public NPL::VDetector
{
   public:   //   Constructor and Destructor
      TSiResPhysics();
      ~TSiResPhysics();

   public:
      void  Clear();
      void  Clear(const Option_t*) {};
   
   public:   //   Calibrated Data
      vector<UShort_t>   DetectorNumber ;
      vector<Double_t>   Energy;
      vector<UShort_t>   ChannelNumber;
      vector<Double_t>   EnergyBack;
      vector<Double_t>   Time;
      vector<Double_t>   x;
      vector<Double_t>   y;

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

      // Give and external TSiResData object to TSiResPhysics. Needed for online analysis for example.
      void SetRawDataPointer(TSiResData* rawDataPointer) {EventData = rawDataPointer;}

      //   Those two method all to clear the Event Physics or Data
      void ClearEventPhysics() {Clear();}      
      void ClearEventData()    {EventData->Clear();}      

    // Method related to the TSpectra classes, aimed at providing a framework for online applications
    // Instantiate the Spectra class and the histogramm throught it
    void InitSpectra();
    // Fill the spectra hold by the spectra class
    void FillSpectra();
    // Used for Online mainly, perform check on the histo and for example change their color if issues are found
    void CheckSpectra();
    // Used for Online only, clear all the spectra hold by the Spectra class
    void ClearSpectra();
    //   Clear The PreTeated object
    void ClearPreTreatedData()   {PreTreatedData->Clear();}

    //   Remove bad channel, calibrate the data and apply threshold
    void PreTreat();
    void Treat();
    
   private:   // Data not writted in the tree
      int                NumberOfDetector ;//!
      TSiResData*         EventData ;//!
      TSiResData*         PreTreatedData ;//!
      TSiResPhysics*      EventPhysics ;//!
      TSiResSpectra*	     m_Spectra;//!
      double 		 m_SiRes_E_Threshold;   
      double 		 m_SiRes_RAW_Threshold;   
      double 		 m_SiRes_EBack_Threshold;   
      double 		 m_SiRes_RAWBack_Threshold;   
  public: // Spectra Getter
    map< string , TH1*> GetSpectra(); 

      public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TSiResPhysics,1)  // SiResPhysics structure
};

#endif
