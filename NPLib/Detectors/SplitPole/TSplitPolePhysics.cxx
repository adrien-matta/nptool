/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : november 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *    This class hold SplitPole  Physics                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
//  NPL
#include "TSplitPolePhysics.h"
using namespace SplitPole_LOCAL;

// C++ headers
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>
#include <cmath>
#include <stdlib.h>
using namespace std;

// NPTool headers
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "NPCalibrationManager.h"
#include "NPOptionManager.h"

//  ROOT
#include "TChain.h"
#include "TSystemDirectory.h"
#include "TList.h"
#include "TSystem.h"
#include "TGraph.h"


ClassImp(TSplitPolePhysics)


///////////////////////////////////////////////////////////////////////////
TSplitPolePhysics::TSplitPolePhysics()
   : m_EventData(new TSplitPoleData),
     m_PreTreatedData(new TSplitPoleData),
     m_EventPhysics(this),
     m_RunStart(2015, 10, 6, 0, 0, 0),
     m_RunStop(2015, 10, 7, 0, 0, 0),
     m_RunLength(0),
     m_FrequenceClock(2.0516),
     m_TickMin(0),
     m_TickMax(0),
     m_RunNumber(0),
     m_CurrentRunNumber(0),
     m_CurrentNMR(new TSplitPoleNMR),
     m_MagneticFieldCorrection(0),
     m_TimeDelay(3657),
     m_LargeField(0),
     m_NmrFilePath("./")
{    
   Clear();
}



///////////////////////////////////////////////////////////////////////////
TSplitPolePhysics::~TSplitPolePhysics()
{
   delete m_EventData;
   delete m_PreTreatedData;
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::Clear()
{
   fPosition     = -1;
   fBrho         = -1;
   fDeltaE       = -1;
   fWire         = -1;
   fPlasticP     = -1;
   fPlasticG     = -1;
   fTick         = -1;
   fAbsoluteTick = -1;
   fTime1.clear();
   fTime2.clear();
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("SplitPole");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 


  vector<string> token = {""};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
 /*   if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  XXX " << i+1 <<  endl;
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }*/
  }


   // read specific SplitPole configuration parameters
   ReadAnalysisConfig();

   // read run time table
   ReadTimeTable();

   // read NMR files
   ReadNMR();
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::ReadTimeTable()
{
   cout << "\tReading time table file..." << endl;

   ifstream file("TimeTable.txt");

   // define variables
   Int_t run_midas, run_narval;
   Int_t date1, date2, time1, time2;
   TTimeStamp start, stop;
   pair<TTimeStamp, TTimeStamp> ptime;

   // read file
   while (file >> run_narval >> run_midas >> date1 >> time1 >> date2 >> time2) {
      start.Set(date1, time1, 0, 1, 0);
      stop.Set(date2, time2, 0, 1, 0);
      ptime.first  = start;
      ptime.second = stop;
      // fill maps
      m_TimeTable[run_midas] = ptime;
      m_NarvalMidasTable[run_narval] = run_midas;
   }

   // close file
   file.close();
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::ReadNMR()
{
   cout << "\tReading NMR files..." << endl;

   // current directory where npanalysis is executed
   string currentpath = gSystem->Getenv("PWD");

   // go to directory with all nmr files and get a list of them
   TSystemDirectory libdir("libdir", m_NmrFilePath);
   TList* listfile = libdir.GetListOfFiles();
   // check whether directory is empty or not
   if (listfile->GetEntries() > 2) {
      Int_t i = 0;
      // loop on nmr files
      while (listfile->At(i)) {
         TString libname = listfile->At(i++)->GetName();
         Int_t pos1 = libname.First("0123456789");
         Int_t pos2 = libname.First(".");
         TString sub = libname(pos1, pos2-pos1);
         Int_t run = 0;
         // if substring is digit only
         if (sub.IsDigit()) {
            // narval run number
            run = sub.Atoi();
            // fill map
            TString FileToRead = libname;
            FileToRead.Prepend("/");
            FileToRead.Prepend(m_NmrFilePath);
            m_NMRTable[m_NarvalMidasTable[run]] = new TSplitPoleNMR(FileToRead.Data(), m_TimeDelay);
         }
      }
   }

   // Since the libdir.GetListOfFiles() command cds to the
   // libidr directory, one has to return to the initial
   // directory
   gSystem->cd(currentpath.c_str());
}



///////////////////////////////////////////////////////////////////////////
Bool_t TSplitPolePhysics::IsSameRun()
{
   Bool_t isSameRun = true;
   if (m_CurrentRunNumber != m_RunNumber) isSameRun = false;

   return isSameRun;
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::AddParameterToCalibrationManager()
{
   CalibrationManager* Cal = CalibrationManager::getInstance();

   // position
   Cal->AddParameter("SplitPole", "POSITION", "POSITION");
}



///////////////////////////////////////////////////////////////////////////
void  TSplitPolePhysics::InitializeRootInputRaw() 
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus("SplitPole",  true);
   inputChain->SetBranchStatus("fPosition",  true);
   inputChain->SetBranchStatus("fDeltaE",    true);
   inputChain->SetBranchStatus("fWire",      true);
   inputChain->SetBranchStatus("fPlasticP",  true);
   inputChain->SetBranchStatus("fPlasticG",  true);
   inputChain->SetBranchAddress("SplitPole", &m_EventData);
   inputChain->SetBranchAddress("RunNumber", &m_RunNumber);
}



///////////////////////////////////////////////////////////////////////////
void  TSplitPolePhysics::InitializeRootInputPhysics() 
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus("SplitPole",  true);
   inputChain->SetBranchStatus("fPosition",  true);
   inputChain->SetBranchStatus("fBrho",      true);
   inputChain->SetBranchStatus("fDeltaE",    true);
   inputChain->SetBranchStatus("fWire",      true);
   inputChain->SetBranchStatus("fPlasticP",  true);
   inputChain->SetBranchStatus("fPlasticG",  true);
   inputChain->SetBranchAddress("SplitPole", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::InitializeRootOutput()
{
   TTree* outputTree = RootOutput::getInstance()->GetTree();
   outputTree->Branch("SplitPole", "TSplitPolePhysics", &m_EventPhysics);
   outputTree->Branch("RunNumber", &m_RunNumber, "RunNumber/I");
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::BuildPhysicalEvent()
{
   BuildSimplePhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::BuildSimplePhysicalEvent()
{
   // Select active channels and apply thresholds
   PreTreat();

   // Fill TSplitPolePhysics private members
   fPosition = m_PreTreatedData->GetPosition();
   fDeltaE   = m_PreTreatedData->GetDeltaE();
   fWire     = m_PreTreatedData->GetWire();
   fPlasticP = m_PreTreatedData->GetPlasticP();
   fPlasticG = m_PreTreatedData->GetPlasticG();
   fTick     = m_PreTreatedData->GetTick();

   for (UShort_t i = 0; i < m_PreTreatedData->GetTime1Multiplicity(); ++i) {   // loop on multiplicity
      fTime1.push_back(m_PreTreatedData->GetTime1(i));
   } // end loop on multiplicity

   for (UShort_t i = 0; i < m_PreTreatedData->GetTime2Multiplicity(); ++i) {   // loop on multiplicity
      fTime2.push_back(m_PreTreatedData->GetTime2(i));
   } // end loop on multiplicity


   // Magnetic field correction
   // store localy run number, run start and stop times and rmn data
   Bool_t isSameRun = IsSameRun();
   if (!isSameRun) {
      m_CurrentRunNumber = m_RunNumber;
      m_RunStart   = m_TimeTable[m_CurrentRunNumber].first;
      m_RunStop    = m_TimeTable[m_CurrentRunNumber].second;
      m_RunLength  = m_RunStop.AsDouble() - m_RunStart.AsDouble();
      m_CurrentNMR = m_NMRTable[m_CurrentRunNumber];
   }
   // Correct for magnetic field variation
   fAbsoluteTick = m_RunStart.AsDouble() + m_PreTreatedData->GetTick()/m_FrequenceClock;
   // check if NMR pointer exist
   if (m_CurrentNMR) {
      // check if magnetic field correction needed
      if (m_MagneticFieldCorrection) {
         fBrho = m_PreTreatedData->GetPlasticG() * m_CurrentNMR->EvalB(fAbsoluteTick);
      }
      else {
         if (!isSameRun) {
            cout << "\t\033[1;31mSplitPole Warning!!! run " << m_CurrentRunNumber << " will use mean magnetic field value " 
               << m_CurrentNMR->GetMean() << " T.m.\033[0m" << endl;
         }
         fBrho = m_PreTreatedData->GetPlasticG() * m_CurrentNMR->GetMean();
      }
   }
   else {
      fBrho = -1;
      if (!isSameRun) {
         cout << "\t\033[1;31mSplitPole Warning!!! run " << m_CurrentRunNumber << " no associated magnetic field...\033[0m" << endl;

      }
   }
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::PreTreat()
{
   // Clear pre treated object
   ClearPreTreatedData();

   // Fill pre treated object 
   m_PreTreatedData->SetPosition(m_EventData->GetPosition());
   m_PreTreatedData->SetDeltaE(m_EventData->GetDeltaE());
   m_PreTreatedData->SetWire(m_EventData->GetWire());
   m_PreTreatedData->SetPlasticP(m_EventData->GetPlasticP());
   m_PreTreatedData->SetPlasticG(fCalibPosition(m_EventData));
   m_PreTreatedData->SetTick(m_EventData->GetTick());

   for (UShort_t i = 0; i < m_EventData->GetTime1Multiplicity(); ++i) {   // loop on multiplicity
      m_PreTreatedData->SetTime1(m_EventData->GetTime1(i));
   } // end loop on multiplicity

   for (UShort_t i = 0; i < m_EventData->GetTime2Multiplicity(); ++i) {   // loop on multiplicity
      m_PreTreatedData->SetTime2(m_EventData->GetTime2(i));
   } // end loop on multiplicity
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::ReadAnalysisConfig()
{
   bool ReadingStatus = false;

   cout << "\t\033[1;35m/////////// Reading ConfigSplitPole.dat file ///////////" << endl;

   // path to file
   string FileName = "./configs/ConfigSplitPole.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << "\tNo ConfigSplitPole.dat found: Default parameter loaded for Analayis " << FileName << endl;
      return;
   }
   cout << "\tLoading user parameters from ConfigSplitPole.dat " << endl;

   // storing config file in the ROOT output file
   TAsciiFile *asciiFile = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
   asciiFile->AppendLine("%% ConfigSplitPole.dat %%");
   asciiFile->Append(FileName.c_str());
   asciiFile->AppendLine("");

   // read analysis config file
   string LineBuffer, DataBuffer, whatToDo;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 15, "ConfigSplitPole") == 0) ReadingStatus = true;

      // loop on tokens and data
      while (ReadingStatus) {
         whatToDo = "";
         AnalysisConfigFile >> whatToDo;

         // Search for comment symbol (%)
         if (whatToDo.compare(0, 1, "%") == 0) {
            AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
         }

         else if (whatToDo == "MAGNETIC_FIELD_CORRECTION") {
            AnalysisConfigFile >> DataBuffer;
            m_MagneticFieldCorrection = false;
            if (DataBuffer == "ON") m_MagneticFieldCorrection = true;
            cout << "\t" << whatToDo << "\t" << DataBuffer << endl;
         }

         else if (whatToDo == "TIME_DELAY") {
            AnalysisConfigFile >> DataBuffer;
            m_TimeDelay = atof(DataBuffer.c_str());
            cout << "\t" << whatToDo << "\t" << m_TimeDelay << endl;
         }

         else if (whatToDo == "NMR_FILE_PATH") {
            AnalysisConfigFile >> DataBuffer;
            m_NmrFilePath = DataBuffer;
            cout << "\t" << whatToDo << "\t" << m_NmrFilePath << endl;
         }

         else if (whatToDo == "NMR_LARGE_FIELD") {
            AnalysisConfigFile >> DataBuffer;
            m_LargeField = atof(DataBuffer.c_str());
            cout << "\t" << whatToDo << "\t" << m_LargeField << endl;
         }

         else {
            ReadingStatus = false;
         }
      }
   }
   cout << "\t/////////////////////////////////////////////////" << endl;
}   



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::InitSpectra()
{
   m_Spectra = new TSplitPoleSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::FillSpectra()
{
   m_Spectra->FillRawSpectra(m_EventData);
   m_Spectra->FillPreTreatedSpectra(m_PreTreatedData);
   m_Spectra->FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::CheckSpectra()
{
   m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::ClearSpectra()
{
   // To be done
}



///////////////////////////////////////////////////////////////////////////
void TSplitPolePhysics::WriteSpectra()
{
   m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
map<string, TH1*> TSplitPolePhysics::GetSpectra()
{
   if (m_Spectra)
      return m_Spectra->GetMapHisto();
   else {
      map< string , TH1*> empty;
      return empty;
   }
}



///////////////////////////////////////////////////////////////////////////
namespace SplitPole_LOCAL {
   Double_t fCalibPosition(const TSplitPoleData* m_EventData) 
   {
      return CalibrationManager::getInstance()->ApplyCalibration("SplitPole/POSITION",  m_EventData->GetPlasticG());
   }
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TSplitPolePhysics::Construct()
{
   return (NPL::VDetector*) new TSplitPolePhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_splitpole {
   public:
      proxy_splitpole() {
         NPL::DetectorFactory::getInstance()->AddToken("SplitPole", "SplitPole");
         NPL::DetectorFactory::getInstance()->AddDetector("SplitPole", TSplitPolePhysics::Construct);
      }
};

proxy_splitpole p;
}

