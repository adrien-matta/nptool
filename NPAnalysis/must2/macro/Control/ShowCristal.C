#include<iostream>
using namespace std;
#include"TMust2Data.h"
#include"TCanvas.h"
#include"TH2I.h"
#include"RootInput.h"
#include"TString.h"

void ShowCristal()
{
  
  TChain* chain = RootInput::getInstance("RunData.txt")->GetChain();
  
  TMust2Data* must2 = new TMust2Data();
  chain->SetBranchAddress("MUST2",&must2);
  
  TCanvas* c1 = new TCanvas();
  c1->Divide(4,4);
  
  TH2I* histo[16];
  
  for(int i = 0 ; i < 16 ; i++) 
    {
      TString hname = Form("CsI_%d",i+1);
      histo[i] = new TH2I(hname,hname,128,1,128,128,1,128);
    }
  cout << chain->GetEntries() << endl ;
  int j = 0 ;
  for(int i = 0 ; i < chain->GetEntries() ; i++)
    {
      chain->GetEntry(i);
      if(i% 10000 == 0) 
        {
         cout << "\r " << i << "  " <<  j << flush ;
        }
      
      if(must2->GetMMStripXEMult()>0 && must2->GetMMStripYEMult()>0 && must2->GetMMCsIEMult()>0)
        {
          if(must2->GetMMStripXEDetectorNbr(0)==must2->GetMMStripYEDetectorNbr(0)  && must2->GetMMStripXEDetectorNbr(0) == must2->GetMMCsIEDetectorNbr(0))
            {
              int X   = must2->GetMMStripXEStripNbr(0);
              int Y   = must2->GetMMStripYEStripNbr(0);
              int CsI = must2->GetMMCsIECristalNbr(0);
              int CsIE = must2->GetMMCsIEEnergy(0);
              
              for(int k = 0 ; k < must2->GetMMCsIEMult() ; k++)
                if(must2->GetMMCsIEEnergy(k)>CsIE && must2->GetMMStripXEDetectorNbr(0) == must2->GetMMCsIEDetectorNbr(k) ) {CsIE=must2->GetMMCsIEEnergy(k) ;  CsI=must2->GetMMCsIECristalNbr(k); }
                
              histo[CsI-1] -> Fill (X,Y);
              j++;
            }
        }
    }

  for(int i = 0 ; i < 16 ; i++) 
    {
      c1->cd(i+1);
      histo[i]-> Draw("colz");
    }
RootInput::Destroy();
  return;
}

void ShowPad()
{
  
  TChain* chain = RootInput::getInstance("RunData.txt")->GetChain();
  
  TMust2Data* must2 = new TMust2Data();
  chain->SetBranchAddress("MUST2",&must2);
  
  TCanvas* c1 = new TCanvas();
  c1->Divide(4,4);
  
  TH2I* histo[16];
  
  for(int i = 0 ; i < 16 ; i++) 
    {
      TString hname = Form("SiLi_%d",i+1);
      histo[i] = new TH2I(hname,hname,128,1,128,128,1,128);
    }
    
  for(int i = 0 ; i < 16 ; i++) 
  {
    c1->cd(i+1);
    histo[i]-> Draw("colz");
  }
    
  cout << chain->GetEntries() << endl ;
  int j = 0 ;
  for(int i = 0 ; i < chain->GetEntries() ; i++)
    {
    
      if( j>1000000) break;
      
      if( j%10000==0) c1->Update();
      
      chain->GetEntry(i);
      if(i% 10000 == 0) 
        {
         cout << "\r " << i << "  " <<  j << flush ;
        }
      
      if(must2->GetMMStripXEMult()>0 && must2->GetMMStripYEMult()>0 && must2->GetMMSiLiEMult()>0)
        {
          if(must2->GetMMStripXEDetectorNbr(0)==must2->GetMMStripYEDetectorNbr(0)  && must2->GetMMStripXEDetectorNbr(0) == must2->GetMMSiLiEDetectorNbr(0) )
            {
              int X   = must2->GetMMStripXEStripNbr(0);
              int Y   = must2->GetMMStripYEStripNbr(0);
              int SiLi = must2->GetMMSiLiEPadNbr(0);
              int SiLiE = must2->GetMMSiLiEEnergy(0);
              
              for(int k = 0 ; k < must2->GetMMSiLiEMult() ; k++)
                if(must2->GetMMSiLiEEnergy(k)>SiLiE && must2->GetMMStripXEDetectorNbr(0) == must2->GetMMSiLiEDetectorNbr(k) ) {SiLiE=must2->GetMMSiLiEEnergy(k) ;  SiLi=must2->GetMMSiLiEPadNbr(k); }
                
              histo[SiLi-1] -> Fill (X,Y);
              j++;
            }
        }
    }

c1->Update();
RootInput::Destroy();
  return;
}
