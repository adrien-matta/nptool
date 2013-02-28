#include "DataProcessor.h"
#include <cstdlib>
ClassImp(DataProcessor);

//_____________________________________________________________________________
DataProcessor::DataProcessor(){
  m_InputChain = 0 ;
  m_OutputTree = 0 ;
  m_ProofFile = 0 ;
  m_OutputFile = 0 ;
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
  
  //Merging via file:
    // Just create the object
    UInt_t opt = TProofOutputFile::kRegister | TProofOutputFile::kOverwrite | TProofOutputFile::kVerify;
    
    m_ProofFile = new TProofOutputFile("Local.root",TProofOutputFile::kDataset, opt );
    m_OutputFile = m_ProofFile->OpenFile("RECREATE");
  
  
  m_OutputTree->SetDirectory(m_OutputFile);
  m_OutputTree->AutoSave();
  m_OutputFile->Flush();
  //GetOutputList()->Add(m_OutputTree);
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
  sleep(1000);
  cout << 1 << endl ;
  string OutputfileName = NPOptionManager::getInstance(GetOption())->GetOutputFile();cout << 2 << endl ;
  RootOutput::getInstance(OutputfileName, "S1107Physics");cout << 3 << endl ;
  TFile* OutputFile = RootOutput::getInstance()->InitFile(  NPOptionManager::getInstance(GetOption())->GetOutputFile());cout << 4 << endl ;
  
  GetOutputList()->Print();
  m_ProofFile = dynamic_cast<TProofOutputFile*>(GetOutputList()->FindObject("Local.root"));cout << 5 << endl ;

  TString outputFile(m_ProofFile->GetOutputFileName());cout << 6 << endl ;
  m_OutputFile = TFile::Open(outputFile);cout << 7 << endl ;
  m_OutputTree = (TTree*) m_OutputFile->Get("S1107Physics");cout << 8 << endl ;
  m_OutputTree->SetDirectory(OutputFile);cout << 9 << endl ;
  m_OutputTree->Write();cout << 10 << endl ;
  OutputFile->Flush();cout << 11 << endl ;
  m_OutputFile->Close();cout << 12 << endl ;
  OutputFile->Close();cout << 13 << endl ;
  RootOutput::getInstance()->Destroy();cout << 14 << endl ;
  
  
/*  TFile* OutputFile = RootOutput::getInstance()->InitFile(  NPOptionManager::getInstance(GetOption())->GetOutputFile());
  m_OutputTree = (TTree*) GetOutputList()->FindObject("S1107Physics");
  m_OutputTree->SetDirectory(OutputFile);
  m_OutputTree->Write();
  OutputFile->Flush();
  RootOutput::getInstance()->Destroy();*/
}

//_____________________________________________________________________________
void DataProcessor::SlaveTerminate(){
  m_OutputFile->cd();
  m_OutputTree->Write();
  m_OutputFile->Flush();
    GetOutputList()->Add(m_ProofFile);
  m_OutputFile->Close();
  RootOutput::getInstance()->Destroy();
  RootInput::getInstance()->Destroy();
  NPOptionManager::getInstance()->Destroy();
}
