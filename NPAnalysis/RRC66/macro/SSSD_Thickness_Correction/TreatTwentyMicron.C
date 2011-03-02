#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TH2F.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TSpectrum.h"
#include "TMath.h"
#include "TMarker.h"
#include "NPEnergyLoss.h"
using namespace NPL;
#include<sys/types.h>

using namespace std ;
const Int_t NumberOfStrip = 128 ;
const Int_t NBX = 2 ;
const Int_t NBY = 2 ;

// the highest energy peak will be aline on those values
const Double_t refSSSD = 2.70  ;
const Double_t refMust2 = 2.70 ;


//
TCanvas* mainC = new TCanvas("Detector Thickness", "DetThickness" , 800,800);
TCanvas* mainC2 = new TCanvas("Diff Mapping", "DiffMap" , 800,800);
//
void TreatTwentyMicron(Int_t detector_number =  4,const char* fname = "MUST_SSSD");

void TreatAllDetector()
  {
    mainC->Divide(2,2);
    mainC2->Divide(2,2);
    for(int i = 0 ; i < 4 ; i++)
      TreatTwentyMicron( i + 1 ) ;
  
    return;
  }

void TreatTwentyMicron(Int_t detector_number ,const char* fname )
{
   
   // Open ROOT file
   TString path = gSystem->Getenv("NPTOOL");
   path += "/Outputs/Analysis/NPAResult/";
   TString inFileName = fname;
   inFileName += "_Otherhistos2.root";
   TFile *inFile = new TFile(path + inFileName);

   // File with the correction
   ofstream CorrectionFile;
   ostringstream oss;
   oss << "Si" << detector_number << "_ThicknessCorrection.dat" ;
   CorrectionFile.open(oss.str().c_str());
    
   // File with the AutoCoherence Value
   ofstream AutCoherenceFile;
   ostringstream oss2;
   oss2 << "Si" << detector_number << "_AutoCoherence.dat" ;
   AutCoherenceFile.open(oss2.str().c_str());
   
//   // Prepare histograms
   TH2F* dee[NumberOfStrip/NBX][NumberOfStrip/NBY];
   TH1D* hProjMust2[NumberOfStrip/NBX][NumberOfStrip/NBY];
   TH1D* hProjSSSD[NumberOfStrip/NBX][NumberOfStrip/NBY];
   TH2I* DetMap = new TH2I("MAP", "Map of working pixel", 128,1,128,128,1,128);
   TH2F* DetMapTh = new TH2F("MAPTh", "Thickness Map", 128,1,128,128,1,128);
   TH2F* DiffMap = new TH2F("DIFMap", "Diff Map", 128,1,128,128,1,128);
//   
//   // Prepare Canvas
   TCanvas* displayIdent = new TCanvas("Identification", "Id",1400,900);
   displayIdent->Divide(3,1);
 
   // Declare EnergyLoss
  EnergyLoss alphaSi = EnergyLoss("EnergyLoss/alpha_Si.G4table","G4Table",10);

 TMarker Marker = TMarker(0, 0, 29);
 Marker.SetMarkerSize(3);
 int NumberOfEmptyHisto = 0;
      for (Int_t jj = 0; jj < NumberOfStrip; jj++) {
         for (Int_t kk = 0; kk < NumberOfStrip; kk++) {
         
            Int_t j = jj/NBX;
            Int_t k = kk/NBY;
            
            if(j%100 == 0 )
              {
                cout << "\r                                                        ";
                cout << "\r" << detector_number << " " << j+1 << " " << k+1 <<flush;
              }
            
            // histograms to be retrieved
            TString hname  = Form("hMM%d_%d_%d", detector_number, j+1, k+1);
            dee[j][k] = (TH2F*) inFile->Get(hname);
            
            hProjMust2[j][k] = dee[j][k] -> ProjectionX("projX",1,dee[j][k]->GetNbinsX());
            hProjSSSD[j][k] = dee[j][k] -> ProjectionY("projY",1,dee[j][k]->GetNbinsY());
            
            Double_t linf = 0;
            Double_t lsup = 6;
            
            displayIdent->cd(1);
            dee[j][k]->Draw("colz");
            
            
            if(hProjMust2[j][k]->GetEntries()!=0  && hProjSSSD[j][k]->GetEntries()!=0 )
               {
                  displayIdent->cd(2);
                  TF1* gaussX=new TF1("gaussX","gaus",linf,lsup); 
                  gaussX->SetRange(linf,lsup);
                  hProjMust2[j][k]->Fit(gaussX,"RQ");

                  Double_t meanMust2 = gaussX->GetParameter(1);
                  Double_t sigMust2 = gaussX->GetParameter(2);

                  displayIdent->cd(3);
                  TF1* gaussY=new TF1("gaussY","gaus",linf,lsup); 
                  gaussY->SetRange(linf,lsup);
                  hProjSSSD[j][k]->Fit(gaussY,"RQ");

                  Double_t meanSSSD = gaussY->GetParameter(1);  
                  Double_t sigSSSD = gaussY->GetParameter(2);          


                  if(sigMust2>1)  meanMust2 = hProjMust2[j][k]->GetMean(1);
                  if(sigSSSD>1)   meanSSSD = hProjSSSD[j][k]->GetMean(1);

                  displayIdent->cd(1);
                  Marker.SetX(meanMust2);
                  Marker.SetY(meanSSSD);
                  Marker.Draw("P");

                  Double_t correctionSSSD  =refSSSD - meanSSSD   ;
                  Double_t correctionMust2 =refMust2 - meanMust2  ;
                  CorrectionFile << jj+1 << " " <<  kk+1 << " " << correctionSSSD << " " << correctionMust2 <<endl;

                  DetMap->Fill(jj+1,kk+1);
                  Double_t Thickness = 0;


                  if (meanMust2>0)
                  {
                     // Evaluate the thin Si thickness with Must2 Energy
                     Thickness = alphaSi.EvaluateMaterialThickness(meanMust2,5.38,40*micrometer);
                     if(Thickness > 0 )
                        {
                           // Check for consistency: Calculate Energy loss in the Thin Si with the calculate Thickness and compare with the observe one
                           double Theoretical_FinalEnergy = alphaSi.Slow(5.38,Thickness,0);
                           double Theoretical_ELoss_ThinSi = 5.38 - Theoretical_FinalEnergy;
                           DiffMap->Fill(jj+1,kk+1,meanSSSD-Theoretical_ELoss_ThinSi/MeV);
                           DetMapTh->Fill(jj+1,kk+1,Thickness/micrometer);
                           AutCoherenceFile << jj+1 << " " <<  kk+1 << " " << abs(meanSSSD-Theoretical_ELoss_ThinSi/MeV) << endl;
                        }
                  }

                  else  AutCoherenceFile << jj+1 << " " <<  kk+1 << " " << -100 << endl;
                  
                  delete gaussY;
                  delete gaussX;
               }
              
              else 
               {
                  CorrectionFile << jj+1 << " " <<  kk+1 << " " << -100 << " " << -100 <<endl;
                  AutCoherenceFile << jj+1 << " " <<  kk+1 << " " << 100 << endl;
               }
               
         }
      }
      
   cout << endl;
   cout << "Number of Empty Histo " << NumberOfEmptyHisto << endl; 

   ((TH1*) DetMapTh) ->GetZaxis()->SetRangeUser(0,25);
   ((TH1*) DiffMap) ->GetZaxis()->SetRangeUser(-6,3);

        if(detector_number<3)  {mainC->cd(detector_number);DetMapTh->Draw("colz");mainC2->cd(detector_number);DiffMap->Draw("colz");}
   else if(detector_number==3) {mainC->cd(4);DetMapTh->Draw("colz");mainC2->cd(4);DiffMap->Draw("colz");}                
   else if(detector_number==4) {mainC->cd(3);DetMapTh->Draw("colz");mainC2->cd(3);DiffMap->Draw("colz");}

   mainC->Update();
   mainC2->Update();

   CorrectionFile.close();
   AutCoherenceFile.close();
   for (Int_t j = 0; j < NumberOfStrip/NBX; j++) 
      for (Int_t k = 0; k < NumberOfStrip/NBY; k++) 
         delete dee[j][k];

}






