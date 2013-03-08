#ifndef DataProcessor_h
#define DataProcessor_h

// NPA
#include "DetectorManager.h"
#include "NPOptionManager.h"

// STL C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdlib>

// ROOT
#include <TSelector.h>
#include <TROOT.h>
#include <TChain.h>
#include <TTree.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TVector3.h>
#include <TRandom.h>
#include "TSystem.h"
#include <TProofOutputFile.h>
// NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "NPReaction.h"
#include "TInitialConditions.h"
#include "TSharcData.h"
#include "TSharcPhysics.h"
// Use CLHEP System of unit and Physical Constant
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"

class DataProcessor : public TSelector {
  public :
  
  // Define members here
  TTree* m_InputChain; // the input chain
  TTree* m_OutputTree; // the output tree
  
  NPA::DetectorManager* m_Detector ; // the detector
  TProofOutputFile* m_ProofFile; // For merging via file
  TFile* m_OutputFile ;
  
  DataProcessor();
  virtual ~DataProcessor();
  virtual Int_t   Version() const { return 2; }
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Begin(TTree*);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList*  GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();
  
  ClassDef(DataProcessor,2);
};
#endif