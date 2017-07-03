// File : GUser.C
// Author: Luc Legeard
//////////////////////////////////////////////////////////////////////////////
//
// Class GUser
//
// Class for User treatment
//
////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
// ***************************************************************************
// *                                                                         *
// *   This program is free software; you can redistribute it and/or modify  *
// *   it under the terms of the GNU General Public License as published by  *
// *   the Free Software Foundation; either version 2 of the License, or     *
// *   (at your option) any later version.                                   *
// *                                                                         *
// ***************************************************************************/

#include "GUser.h"
#include "TROOT.h"
#include <TProfile.h>
#include <TRandom.h>
#include "RootOutput.h"
#include "NPOptionManager.h"
#include <map>
using namespace std;
//______________________________________________________________________________

ClassImp (GUser);

GUser::GUser (GDevice* DevIn, GDevice* DevOut):GAcq(DevIn,DevOut){
 NPOptionManager::getInstance()->Destroy();
 NPOptionManager* myOptionManager = NPOptionManager::getInstance("-D ../e748.detector -GH -C ../Calibration.txt ");

  // Constructor/initialisator of Acquisition object
  //
  // entry:
  // - Input Device
  // - Output Device
  fDevIn         = DevIn;
  fDevOut        = DevOut;

  // Start nptool facilities
  // Initialize root output
 // string OutputfileName = myOptionManager->GetOutputFile();
 // RootOutput::getInstance("Analysis/"+OutputfileName,"GRUTree");
//  m_NPTree= RootOutput::getInstance()->GetTree();

  string detectorfileName = NPOptionManager::getInstance()->GetDetectorFile();

  m_NPDetectorManager = new NPL::DetectorManager;
  m_NPDetectorManager->ReadConfigurationFile(detectorfileName);
  // Start ganil2root facilities
  m_G2RDetectorManager = new G2R::DetectorManager;
  vector<string> det = m_NPDetectorManager->GetDetectorList();
  unsigned int size = det.size();
  for(unsigned int i = 0 ; i < size ; i++){
    m_G2RDetectorManager->AddDetector(det[i]);
  } 

  // Set the Raw Data poiinter
  m_G2RDetectorManager->SetRawDataPointer(m_NPDetectorManager);

  // Register spectra to GRU
  vector < map < string, TH1* > > mySpectra = m_NPDetectorManager->GetSpectra();
   for (unsigned int i = 0; i < mySpectra.size(); ++i) {   // loop on mySpectra
      map<string, TH1*>::iterator it;
      for (it = mySpectra[i].begin(); it != mySpectra[i].end(); ++it) {   // loop on map
         string family = it->first.substr(0, it->first.find_last_of("/",  string::npos));
         GetSpectra()->AddSpectrum(it->second, family.c_str());
      } 
  } 

//    if(myOptionManager->GetOnline()){
    // Request Detector manager to give the Spectra to the server
//    m_NPDetectorManager->SetSpectraServer(); 
//  }



}

//_____________________________________________________________________________

GUser::~GUser()  {
  //Destructor of class GUser
  gROOT->cd();
}

//______________________________________________________________

void GUser::InitUser(){
  // Initialisation for global  user treatement
  // In this method , we can make histograms (prevously declared in GUser.h)
  // Ex : in  GUser.h we have declared           TH1I *fMyHisto ;
  //      in this methode we make it             fHisto = new TH1I ("MyHisto","MyHisto",1024,0,1024);
  //      we can include it in database          GetSpectra()->AddSpectrum(fMyHisto,"MyFamily");
}
//______________________________________________________________

void GUser::InitUserRun(){
  // Initialisation for user treatemeant for each  run
  // For specific user treatement
  m_G2RDetectorManager->Init(GetEvent()->GetDataParameters());
}

//______________________________________________________________
void GUser::User(){
static unsigned int count = 0;
count++;
  // Clear Data from previous event
  m_G2RDetectorManager->Clear();
  // Loop on new Data
  int size =  GetEventArrayLabelValueSize()/2;
  for (Int_t i = 0; i < size; i++) {
    if (m_G2RDetectorManager->Is(GetEventArrayLabelValue_Label(i),GetEventArrayLabelValue_Value(i))) {
      m_G2RDetectorManager->Treat();
    }
  }
  
  // Do nptool analysis
  m_NPDetectorManager->BuildPhysicalEvent();
//  if(count%1000==0)
//   m_NPDetectorManager->CheckSpectraServer();

  // Fill the nptool tree
  //m_NPTree->Fill();
}


//______________________________________________________________
void GUser::EndUserRun(){

  //  end of run ,  executed a end of each run
#if __cplusplus > 199711L
  m_NPDetectorManager->StopThread(); 
#endif

}

//______________________________________________________________
void GUser::EndUser(){
  // Write the generated spectra (from nptool)
  if(NPOptionManager::getInstance()->GetGenerateHistoOption())
    m_NPDetectorManager->WriteSpectra();

  // Write the Physics tree
  RootOutput::Destroy();

}

//_______________________________________________________________
void GUser::InitTTreeUser(){
  m_G2RDetectorManager->InitBranch(fTheTree);
}
