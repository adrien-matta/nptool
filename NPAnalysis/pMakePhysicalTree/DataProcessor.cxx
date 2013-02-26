#include "DataProcessor.h"
ClassImp(DataProcessor);

//_____________________________________________________________________________
DataProcessor::DataProcessor(){
  m_InputChain = 0 ;
  m_OutputTree = 0 ;
}

//_____________________________________________________________________________
DataProcessor::~DataProcessor(){
}

//_____________________________________________________________________________
void DataProcessor::SlaveBegin(TTree*){
  TString NPLPath = gSystem->Getenv("NPTOOL");
  gROOT->ProcessLine(Form(".x %s/NPLib/scripts/NPToolLogon.C+", NPLPath.Data()));

  // Instantiate RootInput
  string runToReadfileName = NPOptionManager::getInstance(GetOption())->GetRunToReadFile();
  
  RootInput::getInstance(runToReadfileName);
  
  // if input files are not given, use those from TAsciiFile
  if (NPOptionManager::getInstance()->IsDefault("DetectorConfiguration")) {
    string name = RootInput::getInstance()->DumpAsciiFile("DetectorConfiguration");
    NPOptionManager::getInstance()->SetDetectorFile(name);
  }
  
  string OutputfileName = NPOptionManager::getInstance()->GetOutputFile();
  RootOutput::getInstance(OutputfileName, "S1107Physics");
  
  // get input files from NPOptionManager
  string detectorfileName = NPOptionManager::getInstance()->GetDetectorFile();
  
  // Instantiate the detector using a file
  m_Detector = new DetectorManager();
  m_Detector->ReadConfigurationFile(detectorfileName);
  
  m_InputChain = RootInput:: getInstance()->GetChain();
  m_OutputTree = RootOutput::getInstance()->GetTree();
  GetInputList()->Add(m_InputChain);
  GetOutputList()->Add(m_OutputTree);
}
//_____________________________________________________________________________
Bool_t DataProcessor::Process(Long64_t entry){
  m_Detector->ClearEventPhysics();
  m_InputChain->GetEntry(entry);
  m_Detector->BuildPhysicalEvent();
  m_OutputTree->Fill();
  return kTRUE;
}

//_____________________________________________________________________________
void DataProcessor::Terminate(){
  string OutputfileName = NPOptionManager::getInstance(GetOption())->GetOutputFile();
  RootOutput::getInstance(OutputfileName, "S1107Physics");

  TFile* OutputFile = RootOutput::getInstance()->InitFile(  NPOptionManager::getInstance(GetOption())->GetOutputFile());
  m_OutputTree = (TTree*) GetOutputList()->FindObject("S1107Physics");
  m_OutputTree->SetDirectory(OutputFile);
  m_OutputTree->Write();
  OutputFile->Flush();
  RootOutput::getInstance()->Destroy();
}

//_____________________________________________________________________________
void DataProcessor::SlaveTerminate(){
  RootOutput::getInstance()->Destroy();
  RootInput::getInstance()->Destroy();
  NPOptionManager::getInstance()->Destroy();
}
