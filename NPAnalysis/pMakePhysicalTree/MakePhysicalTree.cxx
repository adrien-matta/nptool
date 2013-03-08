#include "DataProcessor.h"
#include "TChain.h"
#include "TProof.h"
#include "TDSet.h"

void MakePhysicalTree(string option,int thread=-1){
  
  TFile* f = new TFile("Local.root","RECREATE");
  f->Close();
  
  string runToReadfileName = NPOptionManager::getInstance(option)->GetRunToReadFile();
  TDSet* DSet = new TDSet( *(RootInput::getInstance(runToReadfileName)->GetChain()) );
  TProof* plite;
  
  if(!gProof)
    if(thread < 0 )
      plite = TProof::Open("lite://") ;
    else
      plite = TProof::Open("lite://",Form("workers=%i",thread)) ;
  else
    plite = gProof;
  
  option+=" --proof of=Local.root;stf";
  plite->Process(DSet, "DataProcessor.cxx+", option.c_str());
  
  return ;
}