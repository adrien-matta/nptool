/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    : 03/02/11                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class is a singleton class which deals with the ROOT     *
 *             output file and tree both for NPSimulation and NPAnalysis.    *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   + 03/02/11: Add support for TAsciiFile objects (N. de Sereville)        *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "RootOutput.h"
#include "NPOptionManager.h"

using namespace std;

RootOutput* RootOutput::instance = 0;
////////////////////////////////////////////////////////////////////////////////
RootOutput* RootOutput::getInstance(std::string fileNameBase, std::string treeNameBase){
  // A new instance of RootOutput is created if it does not exist:
  if (instance == 0) {
    instance = new RootOutput(fileNameBase.c_str(), treeNameBase.c_str());
  }

  // The instance of RootOutput is returned:
  return instance;
}

////////////////////////////////////////////////////////////////////////////////
void RootOutput::Destroy(){ 
  if (instance != 0) {
    delete instance;
    instance = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////
RootOutput::RootOutput(std::string fileNameBase, std::string treeNameBase){
  TDirectory* currentPath= gDirectory;

  // The file extension is added to the file name:
  std::string GlobalPath = getenv("NPTOOL");

  // The ROOT file is created
  if(!NPOptionManager::getInstance()->GetPROOF()){
    std::string fileName = GlobalPath + "/Outputs/";
    if (fileNameBase.find("root")!=std::string::npos) fileName += fileNameBase;
    else fileName += fileNameBase + ".root";

    pRootFile = new TFile(fileName.c_str(), "RECREATE");

    if(treeNameBase=="SimulatedTree"){
      string path = getenv("NPTOOL");
        path+="/.last_sim_file";
      ofstream last_sim_file(path.c_str());
      last_sim_file << "TTreeName" << endl 
       << "  " << treeNameBase <<endl
       << "RootFileName" <<endl
       << "  " << fileName<<endl;
      last_sim_file.close();
    }

    else if(treeNameBase=="PhysicsTree"){
      string path = getenv("NPTOOL");
        path+="/.last_phy_file";
      ofstream last_phy_file(path.c_str());
      last_phy_file << "TTreeName" << endl 
       << "  " << treeNameBase <<endl
       << "RootFileName" <<endl
       << "  " << fileName<<endl;
      last_phy_file.close();
    }


    else if(treeNameBase=="ResultTree"){
      string path = getenv("NPTOOL");
      path+="/.last_res_file";
      ofstream last_res_file(path.c_str());
      last_res_file << "TTreeName" << endl 
       << "  " << treeNameBase <<endl
       << "RootFileName" <<endl
       << "  " << fileName<<endl;
      last_res_file.close();
    }
  
    else{
      string path = getenv("NPTOOL");
        path+="/.last_any_file";
      ofstream last_any_file(path.c_str());
      last_any_file << "TTreeName" << endl 
       << "  " << treeNameBase <<endl
       << "RootFileName" <<endl
       << "  " << fileName << endl;
      last_any_file.close();
    }



  }

  else{ // the file path must be the current directory
    // Does not create the Output file at instantiation
    pRootFile = 0 ;
  }

  pRootTree = new TTree(treeNameBase.c_str(), "Data created / analysed with the NPTool package");
  pRootFile->SetCompressionLevel(1);
  pRootList = new TList();

  // Init TAsciiFile objects
  InitAsciiFiles();
  gDirectory->cd(currentPath->GetPath()); 
}


////////////////////////////////////////////////////////////////////////////////
void RootOutput::InitAsciiFiles(){
  // get NPOptionManager pointer
  NPOptionManager* OptionManager  = NPOptionManager::getInstance();

  // Event generator
  // Get file name from NPOptionManager
  std::string fileNameEG = OptionManager->GetReactionFile();
  pEventGenerator = new TAsciiFile();
  pEventGenerator->SetNameTitle("EventGenerator", fileNameEG.c_str());
  pEventGenerator->Append(fileNameEG.c_str());
  pEventGenerator->Write(0,TAsciiFile::kOverwrite);
  
  // Detector configuration 
  // Get file name from NPOptionManager
  std::string fileNameDC = OptionManager->GetDetectorFile();
  pDetectorConfiguration = new TAsciiFile();
  pDetectorConfiguration->SetNameTitle("DetectorConfiguration", fileNameDC.c_str());
  pDetectorConfiguration->Append(fileNameDC.c_str());
  pDetectorConfiguration->Write(0,TAsciiFile::kOverwrite);

  // Run to treat file
  // Get file name from NPOptionManager
  pRunToTreatFile = new TAsciiFile();
  if (!OptionManager->IsDefault("RunToTreat")) {
    std::string fileNameRT = OptionManager->GetRunToReadFile();
    pRunToTreatFile->SetNameTitle("RunToTreat", fileNameRT.c_str());
    pRunToTreatFile->Append(fileNameRT.c_str());
    pRunToTreatFile->Write(0,TAsciiFile::kOverwrite);
  }

  // Calibration files
  pCalibrationFile = new TAsciiFile();
  if (!OptionManager->IsDefault("Calibration")) {
    std::string fileNameCal = OptionManager->GetCalibrationFile();
    pCalibrationFile->SetNameTitle("Calibration", fileNameCal.c_str());
    pCalibrationFile->Write(0,TAsciiFile::kOverwrite);
  }

  // Analysis configuration files
  pAnalysisConfigFile = new TAsciiFile();
  pAnalysisConfigFile->SetNameTitle("AnalysisConfig", "AnalysisConfig");
  pAnalysisConfigFile->Write(0,TAsciiFile::kOverwrite);
}



////////////////////////////////////////////////////////////////////////////////
RootOutput::~RootOutput(){ 
  // The data is written to the file and the tree is closed:
  if (pRootFile && !NPOptionManager::getInstance()->GetPROOF()) {
    TDirectory* currentPath= gDirectory;
    gDirectory->cd(pRootFile->GetPath());
   
    // write TAsciiFile if used
    // EventGenerator
    if (!pEventGenerator->IsEmpty()) pEventGenerator->Write(0,TAsciiFile::kOverwrite);
    // DetectorConfiguration
    if (!pDetectorConfiguration->IsEmpty()) pDetectorConfiguration->Write(0,TAsciiFile::kOverwrite);
    // CalibrationFile
    if (!pCalibrationFile->IsEmpty()) pCalibrationFile->Write(0,TAsciiFile::kOverwrite);
    // RunToTreatFile
    if (!pRunToTreatFile->IsEmpty()) pRunToTreatFile->Write(0,TAsciiFile::kOverwrite);
    // Analysis ConfigFile
    if (!pAnalysisConfigFile->IsEmpty()) pAnalysisConfigFile->Write(0,TAsciiFile::kOverwrite);
   
    cout << endl;
    cout << endl << "Root Output summary" << endl;
    cout << "  - Number of entries in the Tree: " << pRootTree->GetEntries() << endl;
    cout << "  - Number of bites written to file: " << pRootTree->Write(0, TObject::kOverwrite) << endl;
    pRootFile->Flush();
    pRootFile->Purge(1);

    gDirectory->cd(currentPath->GetPath());
    pRootFile->Close();
  }

  else if (pRootFile && NPOptionManager::getInstance()->GetPROOF()){
    if (!pEventGenerator->IsEmpty()) pEventGenerator->Write(0,TAsciiFile::kOverwrite);
    // DetectorConfiguration
    if (!pDetectorConfiguration->IsEmpty()) pDetectorConfiguration->Write(0,TAsciiFile::kOverwrite);
    // CalibrationFile
    if (!pCalibrationFile->IsEmpty()) pCalibrationFile->Write(0,TAsciiFile::kOverwrite);
    // RunToTreatFile
    if (!pRunToTreatFile->IsEmpty()) pRunToTreatFile->Write(0,TAsciiFile::kOverwrite);
    // Analysis ConfigFile
    if (!pAnalysisConfigFile->IsEmpty()) pAnalysisConfigFile->Write(0,TAsciiFile::kOverwrite);
  }

  else if(!pRootFile && NPOptionManager::getInstance()->GetPROOF()){

  }

  else {
    cout << "\033[1;31mNo histograms and Tree !\033[0m" << endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
TFile* RootOutput::InitFile(std::string fileNameBase){

  if(NPOptionManager::getInstance()->GetPROOF()){
    std::string GlobalPath = getenv("NPTOOL");
    std::string fileName = GlobalPath + "/Outputs/Analysis/";
    if (fileNameBase.find("root")!=std::string::npos) fileName += fileNameBase;
    else fileName += fileNameBase + ".root";
    pRootFile = new TFile(fileName.c_str(), "RECREATE");
    pRootFile->Flush();
    return pRootFile;
  }

  else{
    cout << "ERROR: Do not use RootOutput::InitFile without a proof environment (use --proof option to NPTool)" << endl ;
    exit(1);
  }
}
