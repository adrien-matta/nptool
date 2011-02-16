#include<iostream>
using namespace std;
#include"TMust2Physics.h"
#include"TCanvas.h"
#include"TH1F.h"
#include"TF1.h"
#include"RootInput.h"
#include"TString.h"
#include"TLatex.h"


void Find_Satellites(TH1F *h);
Double_t source_Pu(Double_t *x, Double_t *par);
Double_t source_Am(Double_t *x, Double_t *par);
Double_t source_Cm(Double_t *x, Double_t *par);

void EnergyCalibrationCheck()
{
  
  TChain* chain = RootInput::getInstance("RunCalibration.txt")->GetChain();
  
  TMust2Physics* must2 = new TMust2Physics();
  chain->SetBranchAddress("MUST2",&must2);
    
   int SelectedStripX = 64;
   TString hname = Form("Strip_X_%d",SelectedStripX ); 
   TString htitle = Form("Strip X %d",SelectedStripX ); 
   TH1F* OneStripHistoX= new TH1F(hname,htitle,1000,4.5,6.5);
   TH1F* AllStripHistoX= new TH1F("All_Strip_X","All Strip X",1000,4.5,6.5);
   
   int SelectedStripY = 64;
   hname = Form("Strip_Y_%d",SelectedStripY ); 
   htitle = Form("Strip Y %d",SelectedStripY ); 
   TH1F* OneStripHistoY= new TH1F(hname,htitle,1000,4.5,6.5);
   TH1F* AllStripHistoY= new TH1F("All_Strip_Y","All Strip Y",1000,4.5,6.5);
   
   
   OneStripHistoX->SetStats(true);
   AllStripHistoX->SetStats(true);
   OneStripHistoY->SetStats(true);
   AllStripHistoY->SetStats(true);
   
  cout << chain->GetEntries() << endl ;
//  for(int i = 0 ; i < chain->GetEntries() ; ++i)
for(int i = 0 ; i < 1000 ; ++i)
    {
      chain->GetEntry(i);
      if(i% 10000 == 0) 
         cout << "\r " << i  << flush ;
        
      for(int k = 0 ; k< must2->Si_EX.size(); ++k)
        {
          AllStripHistoX->Fill(must2->Si_EX[k]);
          if(must2->Si_X[k] == SelectedStripX)
            OneStripHistoX->Fill(must2->Si_EX[k]);
        }
        
        
      for(int k = 0 ; k< must2->Si_EY.size(); ++k)
        {
          AllStripHistoY->Fill(must2->Si_EY[k]);
          
          if(must2->Si_Y[k] == SelectedStripY)
            OneStripHistoY->Fill(must2->Si_EY[k]);
        }
      
        
        
    }

  cout << endl ;
  Find_Satellites(AllStripHistoX);
  Find_Satellites(OneStripHistoX);
  Find_Satellites(AllStripHistoY);
  Find_Satellites(OneStripHistoY);

  TCanvas* c1 = new TCanvas("Summary","Summary",900,900);
  c1->Divide(2,2);

  c1->cd(1);
  OneStripHistoX-> Draw("");
  TLatex* tex1 = new TLatex(OneStripHistoX->	GetMaximum() - OneStripHistoX->	GetMaximum()/10, OneStripHistoX->GetBinContent(OneStripHistoX->GetMaximumBin())/10, "a");
   tex1->SetLineWidth(2);
   tex1->Draw();
  c1->cd(2);
  AllStripHistoX-> Draw("");
  TLatex* tex2 = new TLatex(10, 10, "b");
   tex2->SetLineWidth(2);
   tex2->Draw();
  c1->cd(3);
  OneStripHistoY-> Draw("");
  TLatex* tex3 = new TLatex(10, 10, "c");
   tex3->SetLineWidth(2);
   tex3->Draw();
  c1->cd(4);
  AllStripHistoY-> Draw("");
  TLatex* tex4 = new TLatex(10, 10, "d");
   tex4->SetLineWidth(2);
   tex4->Draw();
    
  RootInput::Destroy();
  return;
}

void Find_Satellites(TH1F *h)
  {
    
      double mean[3];
      mean[0] = 5.7;
      mean[1] = 5.4;
      mean[2] = 5.1;
      
      Float_t linf1 =0 , lsup1 =0, linf2 =0 , lsup2 =0 , linf3 =0 , lsup3=0;
    
      linf1 = 5.7-0.2; lsup1 = 5.7+0.2;
      linf2 = 5.4-0.2; lsup2 = 5.4+0.2;
      linf3 = 5.1-0.2; lsup3 = 5.0+0.2;
      
      TF1 *Pu = new TF1("fit_sat_Pu", source_Pu, linf1, lsup1, 6);
      Pu->SetParameters(150,mean[0],mean[0]-12.4/1000.,mean[0]-51.6/1000.,0.02);
      Pu->SetParLimits(2,mean[0]-12.4/1000.-10,mean[0]-12.6/1000.+10);
      Pu->SetParLimits(3,mean[0]-51.6/1000.-10,mean[0]-51.6/1000.+10);
      Pu->SetParNames("Constant","Mean_value1","Mean_value2","Mean_value3","SigmaPu");
      h->Fit("fit_sat_Pu", "RQ");
      
      TF1 *Am = new TF1("fit_sat_Am", source_Am, linf2, lsup2, 6);
      Am->SetParameters(150,mean[1],mean[1]-43.2/1000.,mean[1]-98.4/1000.,0.02);
      Am->SetParLimits(2,mean[1]-43.2/1000.-10,mean[1]-43.2/1000.+10);
      Am->SetParLimits(3,mean[1]-98.4/1000.-10,mean[1]-98.4/1000.+10);
      Am->SetParNames("Constant","Mean_value1","Mean_value2","Mean_value3","SigmaAm");
      h->Fit("fit_sat_Am", "RQ+");
      
      
      TF1 *Cm = new TF1("fit_sat_Cm", source_Cm, linf3, lsup3, 6);
      Cm->SetParameters(150,mean[2],mean[2]-43.1/1000.,0.02);
      Cm->SetParLimits(2,mean[2]-43.1/1000.-10,mean[0]-43.1/1000.-10);
      Cm->SetParNames("Constant","Mean_value1","Mean_value2","SigmaCm");
      h->Fit("fit_sat_Cm", "RQ+");
  }

///////////////////////////////////////////////
Double_t source_Pu(Double_t *x, Double_t *par)
  {
    // [0] : constant
    // [1] : position peak1
    // [2] : position peak2
    // [3] : position peak3
    // [4] : sigma

    Double_t arg1 = 0;
    Double_t arg2 = 0;
    Double_t arg3 = 0;

    if(par[4]!=0) { 
      arg1 = (x[0]-par[1])/par[4];
      arg2 = (x[0]-par[2])/par[4];
      arg3 = (x[0]-par[3])/par[4];
    }

    else cout << " Attention, sigma est nul !" << endl;

    Double_t gaus1 =           par[0]*exp(-0.5*arg1*arg1);
    Double_t gaus2 = 15.1/73.8*par[0]*exp(-0.5*arg2*arg2);
    Double_t gaus3 = 11.5/73.8*par[0]*exp(-0.5*arg3*arg3);
    Double_t fitval = gaus1+gaus2+gaus3;

    return fitval;
  }

///////////////////////////////////////////////
Double_t source_Am(Double_t *x, Double_t *par)
  {
    // [0] : constant
    // [1] : position peak1
    // [2] : position peak2
    // [3] : position peak3
    // [4] : sigma

    Double_t arg1 = 0;
    Double_t arg2 = 0;
    Double_t arg3 = 0;
    
    if(par[4]!=0) {
      arg1 = (x[0]-par[1])/par[4];
      arg2 = (x[0]-par[2])/par[4];
      arg3 = (x[0]-par[3])/par[4];
    }

    else cout << " Attention, sigma est nul !" << endl;

    Double_t gaus1 =           par[0]*exp(-0.5*arg1*arg1);
    Double_t gaus2 = 13.0/84.5*par[0]*exp(-0.5*arg2*arg2);
    Double_t gaus3 = 1.6/84.5 *par[0]*exp(-0.5*arg3*arg3);
    Double_t fitval= gaus1+gaus2+gaus3;

    return fitval;
  }

///////////////////////////////////////////////
Double_t source_Cm(Double_t *x, Double_t *par)
  {
    // [0] : constante
    // [1] : position peak1
    // [2] : position peak2
    // [3] : sigma
    
    Double_t arg1 = 0;
    Double_t arg2 = 0;
    
    if(par[3]!=0) { 
      arg1 = (x[0]-par[1])/par[3];
      arg2 = (x[0]-par[2])/par[3];
    }
    
    else cout << " Attention, sigma est nul !" << endl;
    
    Double_t gaus1 =           par[0]*exp(-0.5*arg1*arg1);
    Double_t gaus2 = 23.6/76.4*par[0]*exp(-0.5*arg2*arg2);
    Double_t fitval= gaus1+gaus2; 
    
    return fitval;
  }  
