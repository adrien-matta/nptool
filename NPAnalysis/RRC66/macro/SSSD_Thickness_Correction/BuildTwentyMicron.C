#include<vector> 
#include<iostream>
#include<set>

#include"TROOT.h"
#include"TSystem.h"
#include"TFile.h"
#include"TString.h"
#include"TTree.h"
#include"TH2F.h"
#include"TCanvas.h"

#include"TMust2Physics.h"
#include"TSSSDPhysics.h"

const Int_t NBTWENTY=4;
const Int_t NBSTRIPS=128;
const Int_t NBX=2;
const Int_t NBY=2;
const Int_t MULTMAX=10;
TTree* tree ;
TMust2Physics* must2Phys;
TSSSDPhysics* sssdPhys;
TFile* outFile;

void BuildHisto();

void BuildTwentyMicron()
  {
    // Open ROOT file
    TString path = gSystem->Getenv("NPTOOL");
    path += "/Outputs/Analysis/";
//    TString path ="/media/rc66_1/NPTool/";
  
    char* fname = "NPAResult/MUST_SSSD";


    TString inFileName = fname;
    inFileName += ".root";
    TFile *inFile = new TFile(path + inFileName);
    tree   = (TTree*) inFile->Get("AnalysedTree");

    // ROOT output file
    TString outPath = path + "/Outputs/Analysis/";
    TString outFileName = Form("%s_Otherhistos%d.root", fname,NBX);
    outFile = new TFile(path + outFileName, "recreate");
    outFile->SetCompressionLevel(9);
    // Connect branches
    // Must2
    must2Phys = new TMust2Physics();
    tree->SetBranchStatus("MUST2", true); 
    tree->SetBranchAddress("MUST2", &must2Phys);
    // 20 microns
    sssdPhys = new TSSSDPhysics();
    tree->SetBranchStatus("SSSD", true); 
    tree->SetBranchAddress("SSSD", &sssdPhys);


    BuildHisto();

    // write and close output TFile
//    outFile->Write();
    outFile->Close();

    delete inFile;
    delete must2Phys;
    delete sssdPhys;
    delete outFile;
     
  }

void BuildHisto()
  { 
     // Prepare histograms
     //TH2F *dee[NBSTRIPS/NBX][NBSTRIPS/NBY];
    cout << "creating histo" << endl ;
    set<TH2F*> Temp;
    unsigned int BufferSize = 19000;
    
      for (Int_t i = 0; i < NBTWENTY; i++) {
        for (Int_t j = 0; j < NBSTRIPS/NBX; j++) {
           for (Int_t k = 0; k < NBSTRIPS/NBY; k++) {
              
              TString hname  = Form("hMM%d_%d_%d", i+1, j+1, k+1);
              TString htitle = Form("MM%d_%d_%d",  i+1, j+1, k+1);
              
              if( (i+j+k)%100==0)
              cout << "\r Histo " << i+1 << " " << j+1 << " " << k+1 << "   " << flush;
              
              Temp.insert(new TH2F(hname, htitle, 200, 0, 6, 200, 0, 6));
              
              if(Temp.size()==BufferSize)
                {
                  cout << "\r Flushing File    " << flush;
                  outFile->Write();
                  cout << "\r Cleaning Memory   " << flush;
                  set<TH2F*>::iterator it ;	
                  for ( it=Temp.begin() ; it!=Temp.end() ; it++ )
                    delete *it;
                  Temp.clear();
                }
           }
        }
      }
   
    
    cout << "\r Flushing File    " << flush;
    outFile->Write();
    cout << "\r Cleaning Memory   " << flush;
    set<TH2F*>::iterator it ;	
    for ( it=Temp.begin() ; it!=Temp.end() ; it++ )
      delete *it;
    Temp.clear();
    
      cout << endl << "reading data" << endl ;
      
     // Loop on data
     Int_t nentries = tree->GetEntries();
     cout << "TTree contains " << nentries << " events" << endl;
     
     for (Int_t i = 0; i < nentries; i++) {
        // display
        if (i%10000 == 0) {
        cout << "\r                     "<< flush;
        cout << "\r Entry " << i << flush;
        }
        // get entry
        tree->GetEntry(i);

        // treat data
        // get must2 multiplicity
        Int_t multMust2 = must2Phys->Si_E.size();
        
        if(multMust2<MULTMAX+1)
          {
            for (Int_t j = 0; j < multMust2; j++) {
               Int_t idMust2 = must2Phys->TelescopeNumber[j];
               Int_t multSSSD = sssdPhys->Energy.size();
               for (Int_t k = 0; k < multSSSD; k++) {
                  Int_t idSSSD  = sssdPhys->DetectorNumber[k];
                  // check if SSSD and Must2 are in coincidence
                  if (idSSSD == idMust2 ) {
                     // energies
                     Double_t eMust2 = must2Phys -> Si_E[j];
                     Double_t eSSSD  = sssdPhys  -> Energy[k];
                     // strips
                     Int_t stripXmust2 = must2Phys->Si_X[j] ;
                     Int_t stripYmust2 = must2Phys->Si_Y[j] ;
                     
                    if(eMust2+eSSSD>5.15) 
                     {
                       Int_t Hx = stripXmust2/NBX ;
                       Int_t Hy = stripYmust2/NBY ;
                       
                       if(Hx == 0 ) Hx = 1 ;
                       if(Hy == 0 ) Hy = 1 ;
                       
                    TString name  = Form("hMM%d_%d_%d", idSSSD, Hx , Hy );
                     
                     TH2F* buffer = (TH2F*)outFile->Get(name);
                     Temp.insert( buffer );
                     if(buffer!=NULL) 
                      buffer->Fill(eMust2, eSSSD);
                     else cout << "Histgramm " << name << " not found!!!" << endl ;
                     
                     }
                    
                    // else cout << "Histo " << name << " not found!" << endl ;
                      
                      if(Temp.size()==BufferSize)
                        {
                          cout << "\r Entry:" << i << " ; FLushing File " << flush;
                          outFile->Flush();
                          cout << "\r Entry:" << i << " ; Cleaning Memory " << flush;
                          for ( it=Temp.begin() ; it!=Temp.end() ; it++ )
                            delete *it;
                          Temp.clear();
                        }
                  }
                  else {
                     //cout << "MUST2 and 20 microns not in coincidence" << endl;
                  }
               }
            }
          
          }
        
     }
    
    cout << endl ;
    cout << endl << " Writting final file " << endl ;
    outFile->Write();
      cout << "\r Cleaning Memory " << endl;	
      for ( it=Temp.begin() ; it!=Temp.end() ; it++ )
        delete *it;
      Temp.clear();
      cout << endl;
    
  }
