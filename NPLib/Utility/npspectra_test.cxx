#include"TApplication.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TError.h"
#include "TRandom.h"
#include"NPSpectraServer.h"
#include<cstdlib>
#include<iostream>

int main(int argc , char** argv){
  // Root will not issue any BS warning message
  gErrorIgnoreLevel = 10000;

  NPL::SpectraServer* server = NPL::SpectraServer::getInstance();

  // Create two test spectrum
  TH1D* h1 = new TH1D("h1","h1",1000,-10,10);
  TH2D* h2 = new TH2D("h2","h2",500,-10,10,500,-10,10);
  // Add them to the server
  server->AddSpectra(h1);
  server->AddSpectra(h2);


  // Load test
  for(unsigned int i = 0 ; i < 100 ; i++){
  TH1D* hh1 = new TH1D(Form("hh1D%i",i),"hh1D",1000,-10,10);
  TH2D* hh2 = new TH2D(Form("hh2D%i",i),"hh2D",1000,-10,10,1000,-10,10);
  // Add them to the server
  server->AddSpectra(hh1);
  server->AddSpectra(hh2);
  }

   while(true){
    h1->Reset();
    h2->Reset();
    double meanX = gRandom->Uniform(-5,5);
    double meanY = gRandom->Uniform(-5,5);
    for(unsigned int i=0 ; i< 1000 ; i++){
    // Random value for the fill
    double x = gRandom->Gaus(meanX,1); 
    double y = gRandom->Gaus(meanY,1); 

    // Fill them
    server->FillSpectra("h1",x);
    server->FillSpectra("h2",x,y);
    }
    
    server->CheckRequest(); 
    }

  return 0;
}

