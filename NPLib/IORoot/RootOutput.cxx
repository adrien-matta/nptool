/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
#include <cstdlib>

#include "RootOutput.h"
#include "NPOptionManager.h"

using namespace std;

RootOutput *RootOutput::instance = 0;

RootOutput* RootOutput::getInstance(TString fileNameBase, TString treeNameBase)
{
   // A new instance of RootOutput is created if it does not exist:
   if (instance == 0) {
      instance = new RootOutput(fileNameBase, treeNameBase);
   }

   // The instance of RootOutput is returned:
   return instance;
}


void RootOutput::Destroy()
{
   if (instance != 0) {
      delete instance;
      instance = 0;
   }
}


RootOutput::RootOutput(TString fileNameBase, TString treeNameBase)
{
   // The file extension is added to the file name:
   TString GlobalPath = getenv("NPTOOL");
   TString fileName = GlobalPath + "/Outputs/";
   if (fileNameBase.Contains("root")) fileName += fileNameBase;
   else fileName += fileNameBase + ".root"; 

   // The ROOT file is created
   pRootFile = new TFile(fileName, "RECREATE");
   pRootTree = new TTree(treeNameBase, "Data created / analyzed with the NPTool package");
   pRootList = new TList();

   // Init TAsciiFile objects
   InitAsciiFiles();
}



void RootOutput::InitAsciiFiles()
{
   // get NPOptionManager pointer
   NPOptionManager* OptionManager  = NPOptionManager::getInstance();

   // Event generator
   // Get file name from NPOptionManager
   TString fileNameEG = OptionManager->GetReactionFile();
   pEventGenerator = new TAsciiFile();
   pEventGenerator->SetNameTitle("EventGenerator", fileNameEG.Data());
   pEventGenerator->Append(fileNameEG.Data());

   // Detector configuration 
   // Get file name from NPOptionManager
   TString fileNameDC = OptionManager->GetDetectorFile();
   pDetectorConfiguration = new TAsciiFile();
   pDetectorConfiguration->SetNameTitle("DetectorConfiguration", fileNameDC.Data());
   pDetectorConfiguration->Append(fileNameDC.Data());

   // Run to treat file
   // Get file name from NPOptionManager
   pRunToTreatFile = new TAsciiFile();
   if (!OptionManager->IsDefault("RunToTreat")) {
      TString fileNameRT = OptionManager->GetRunToReadFile();
      pRunToTreatFile->SetNameTitle("RunToTreat", fileNameRT.Data());
      pRunToTreatFile->Append(fileNameRT.Data());
   }

   // Calibration files
   pCalibrationFile = new TAsciiFile();
   if (!OptionManager->IsDefault("Calibration")) {
      TString fileNameCal = OptionManager->GetCalibrationFile();
      pCalibrationFile->SetNameTitle("Calibration", fileNameCal.Data());
   }

   // Analysis configuration files
   pAnalysisConfigFile = new TAsciiFile();
}



RootOutput::~RootOutput()
{
   // The data is written to the file and the tree is closed:
   if (pRootFile) {
      cout << endl;
      cout << "Got histograms and Tree !" << endl;
      pRootFile->Write();

      // write TAsciiFile if used
      // EventGenerator
      TString EGName = pEventGenerator->GetName();
      if (!EGName.IsNull()) pEventGenerator->Write();
      // DetectorConfiguration
      TString DCName = pDetectorConfiguration->GetName();
      if (!DCName.IsNull()) pDetectorConfiguration->Write();
      // CalibrationFile
      TString CFName = pCalibrationFile->GetName();
      if (!CFName.IsNull()) pCalibrationFile->Write();
      // RunToTreatFile
      TString RTName = pRunToTreatFile->GetName();
      if (!RTName.IsNull()) pRunToTreatFile->Write();
      // Analysis ConfigFile
      if (!pAnalysisConfigFile->IsEmpty()) pAnalysisConfigFile->Write();

      pRootFile->Close();
   } else {
      cout << "No histograms and Tree !" << endl;
   }
}
