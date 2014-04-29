#ifndef ROOTOUTPUT_HH
#define ROOTOUTPUT_HH
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
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



// NPL headers
#include "TAsciiFile.h"

// ROOT headers
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TList.h"


class RootOutput{
public:
  // The analysis class is designed to be a singleton (i.e. only one instance
  // can exist). A member function called Instance is defined, which allows
  // the user to get a pointer to the existing instance or to create it if
  // it does not yet exist:
  // (see the constructor for an explanation of the arguments)
  static RootOutput* getInstance(TString fileNameBase = "Simulation",
                                 TString treeNameBase = "SimulatedTree");
  
  // The analysis class instance can be deleted by calling the Destroy
  // method (NOTE: The class destructor is protected, and can thus not be
  // called directly):
  static void Destroy();
  
protected:
  // Constructor (protected)
  RootOutput(TString fileNameBase, TString treeNameBase);
  
  // Destructor (protected)
  virtual ~RootOutput();
  
  // Prevent copying
  RootOutput(const RootOutput& only);
  const RootOutput& operator=(const RootOutput& only);
  
private:
  // The static instance of the RootOutput class:
  static RootOutput* instance;
  
private:
  void InitAsciiFiles();
  
public:
  TFile*      GetFile()                           {return pRootFile;}
  TTree*      GetTree()                           {return pRootTree;}
  TList*      GetList()                           {return pRootList;}
  TAsciiFile* GetAsciiFileEventGenerator()        {return pEventGenerator;}
  TAsciiFile* GetAsciiFileDetectorConfiguration() {return pDetectorConfiguration;}
  TAsciiFile* GetAsciiFileCalibration()           {return pCalibrationFile;}
  TAsciiFile* GetAsciiFileRunToTreat()            {return pRunToTreatFile;}
  TAsciiFile* GetAsciiFileAnalysisConfig()        {return pAnalysisConfigFile;}
  TFile*      InitFile(TString fileNameBase); // use only for proof environment
  
private:
  TFile      *pRootFile;
  TTree      *pRootTree;
  TList      *pRootList;
  TAsciiFile *pEventGenerator;
  TAsciiFile *pDetectorConfiguration;
  TAsciiFile *pCalibrationFile;
  TAsciiFile *pRunToTreatFile;
  TAsciiFile *pAnalysisConfigFile;
  
};

#endif // ROOTOUTPUT_HH
