#include "DataProcessor.h"
#include "TChain.h"
#include "TProof.h"
#include "TDSet.h"

void MakePhysicalTree(string option){
  
  string runToReadfileName = NPOptionManager::getInstance(option)->GetRunToReadFile();
  TDSet* DSet = new TDSet( *(RootInput::getInstance(runToReadfileName)->GetChain()) );
  TProof* plite;
  
  if(!gProof)
    plite = TProof::Open("lite://") ;
  else
    plite = gProof;
  
  plite->Process(DSet, "DataProcessor.cxx+", option.c_str());
  
  return ;
}