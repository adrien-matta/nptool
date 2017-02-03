#ifndef __SPLITPOLEPhysics__
#define __SPLITPOLEPhysics__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : November 2015                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class holds the physics class for the SplitPole detector         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
//   STL
#include <vector>
#include <utility>
#include <map>
using namespace std;

//   ROOT
#include "TObject.h"
#include "TH1.h"
#include "TTimeStamp.h"

//   NPL
#include "TSplitPoleData.h"
#include "TSplitPoleNMR.h"
#include "TSplitPoleSpectra.h"
#include "NPVDetector.h"
#include "NPInputParser.h"

// forward declaration
class TSplitPoleSpectra;


class TSplitPolePhysics : public TObject, public NPL::VDetector
{
   public:   //   Constructor and Destructor
      TSplitPolePhysics();
      ~TSplitPolePhysics();

   public:
      void Clear();
      void Clear(const Option_t*) {};


   private: // data obtained after BuildPhysicalEvent() and stored in ROOT output file
      Double_t fPosition;
      Double_t fBrho;
      Double_t fDeltaE;
      Double_t fWire;
      Double_t fPlasticP;
      Double_t fPlasticG;
      Double_t fTick;
      Double_t fAbsoluteTick;
      vector<Double_t> fTime1;
      vector<Double_t> fTime2;

   public:
      // setters
      void SetPosition(Double_t position) {fPosition = position;}
      void SetBrho(Double_t brho)         {fBrho     = brho;}
      void SetDeltaE(Double_t deltae)     {fDeltaE   = deltae;}
      void SetWire(Double_t wire)         {fWire     = wire;}
      void SetPlasticP(Double_t plp)      {fPlasticP = plp;}
      void SetPlasticG(Double_t plg)      {fPlasticG = plg;}
      void SetTime1(Double_t time)        {fTime1.push_back(time);}
      void SetTime2(Double_t time)        {fTime2.push_back(time);}

      // getters
      Double_t GetPosition()              const {return fPosition;}
      Double_t GetBrho()                  const {return fBrho;}
      Double_t GetDeltaE()                const {return fDeltaE;}
      Double_t GetWire()                  const {return fWire;} 
      Double_t GetPlasticP()              const {return fPlasticP;}
      Double_t GetPlasticG()              const {return fPlasticG;}
      Double_t GetTime1(Int_t i)          const {return fTime1[i];}
      Double_t GetTime2(Int_t i)          const {return fTime2[i];}
      UShort_t GetTime1Multiplicity()     const {return fTime1.size();}    //
      UShort_t GetTime2Multiplicity()     const {return fTime2.size();}    //


   public:   //   inherited from VDetector
      // Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
      void ReadConfiguration(NPL::InputParser);

      // Add Parameter to the CalibrationManger
      void AddParameterToCalibrationManager();      

      // Activate associated branches and link them to the private member object m_EventData
      void InitializeRootInputRaw() ;

      // Activate associated branches and link them to the private member m_EventPhysics
      void InitializeRootInputPhysics() ;

      // Create associated branches and associated private member m_EventPhysics
      void InitializeRootOutput();

      // This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
      void BuildPhysicalEvent();

      // Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
      // This method aimed to be used for analysis performed during experiment, when speed is requiered.
      // NB: This method can eventually be the same as BuildPhysicalEvent.
      void BuildSimplePhysicalEvent();

      // Same as above but for online analysis
      void BuildOnlinePhysicalEvent()  {BuildPhysicalEvent();};

      // Those two method all to clear the Event Physics or Data
      void ClearEventPhysics()   {Clear();}
      void ClearEventData()      {m_EventData->Clear();}

      // Methods related to the TSplitPoleSpectra classes
      // Instantiate the TSplitPoleSpectra class and the histograms
      void InitSpectra();
      // Fill the spectra defined in TSplitPoleSpectra
      void FillSpectra();
      // Used for Online mainly, perform check on the histo and for example change their color if issues are found
      void CheckSpectra();
      // Used for Online only, clear all the spectra hold by the Spectra class
      void ClearSpectra();
      // Write Spectra to file
      void WriteSpectra();


   public: //   Specific to SplitPole
      // Remove bad channel, calibrate the data and apply thresholds
      void PreTreat();

      // Clear the pre treated object
      void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

      // Read the user configuration file; if no file found, load standard one
      void ReadAnalysisConfig();

      // Give an external TSplitPoleData object to TSplitPolePhysics. Needed for online analysis for example.
      void SetRawDataPointer(TSplitPoleData* rawDataPointer) {m_EventData = rawDataPointer;}


   private: // Data not written in the tree
      TSplitPoleData*    m_EventData;            //!
      TSplitPoleData*    m_PreTreatedData;       //!
      TSplitPolePhysics* m_EventPhysics;         //!


   private: // Map of activated Channel
      //   map< int, vector<bool> > m_FrontChannelStatus;  //!
      //   map< int, vector<bool> > m_BackChannelStatus;   //!


   private: // parameters needed for magnetic field correction
      map<Int_t, pair<TTimeStamp, TTimeStamp> > m_TimeTable; //!
      map<Int_t, TSplitPoleNMR*> m_NMRTable; //!
      map<Int_t, Int_t> m_NarvalMidasTable; //!
      TTimeStamp     m_RunStart;          //!
      TTimeStamp     m_RunStop;           //!
      Double_t       m_RunLength;         //! // in sec
      Double_t       m_FrequenceClock;    //! // in Hz
      Double_t       m_TickMin;           //!
      Double_t       m_TickMax;           //!
      Int_t          m_RunNumber;         //! // read event by event from TTree
      Int_t          m_CurrentRunNumber;  //!
      TSplitPoleNMR* m_CurrentNMR;        //!

   private: // Parameters used in the analysis
      Bool_t   m_MagneticFieldCorrection;  //!
      Double_t m_TimeDelay;   //!
      Double_t m_LargeField;  //!
      TString  m_NmrFilePath; //!

   // methods for magnetic field correction
   public: // called once
      void  ReadTimeTable();
      void  ReadNMR();

   public: // called event by event
      Bool_t IsSameRun();


   private: // Spectra Class
      TSplitPoleSpectra* m_Spectra; // !

   public: // Spectra Getter
      map<string, TH1*> GetSpectra();

   public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();

     ClassDef(TSplitPolePhysics,1)  // TSplitPolePhysics
};


namespace SplitPole_LOCAL
{
   Double_t fCalibPosition(const TSplitPoleData* EventData);
}

#endif
