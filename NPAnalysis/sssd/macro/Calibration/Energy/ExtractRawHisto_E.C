////////////////////////////////////////////////////////////////////////////
// This macro takes a converted ROOT file from the GANIL e569 exp. and    //
// create an histogram for each strip (X and Y) of the SSSD array filled //
// with the energy. The histograms are dumped in an output ROOT file.     //
//                                                                        //
// This is especially usefull for calibration purposes when there is no   //
// need to work directly on the TChain                                     //
////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TChain.h"
#include "TBranch.h"
#include "TH1F.h"

#include "../../../NPLib/include/TSSSDData.h"
#include "../../../NPLib/include/RootInput.h"

#define NBTELESCOPE	4
#define	NBSTRIPS	16

using namespace std ;

void ExtractSSSDHistos(const char* fname = "run_87-88")
{
	
	RootInput* Input = RootInput::getInstance("RunToTreat.txt");
	TChain* Chain = Input->GetChain();
		
   // connect the TSSSDData branch  
   Chain->SetBranchStatus("SSSD",true);
   Chain->SetBranchStatus("fSSSD*",true);
   TSSSDData *rawSSSD = new TSSSDData();
   Chain->SetBranchAddress("SSSD",&rawSSSD);

   // open the output ROOT file
   TString outFileName = "./Histograms/";
   outFileName += fname;
   outFileName += "_RawSSSDHistos.root";
   cout<< outFileName<< endl;
   TFile *outFile = new TFile(outFileName, "recreate");

   // prepare output histograms for SSSD
   TH1F* hStripEnergy[NBTELESCOPE][NBSTRIPS];
   for (Int_t i = 0; i < NBTELESCOPE; i++) 
      {
        for (Int_t j = 0; j < NBSTRIPS; j++) 
          {
             // strips XE
             TString hnameE     = Form("hSSSD%d_STR_E%d", i+1, j+1);
             TString htitleE    = Form("SSSD%d_STR_E%d", i+1, j+1);
             hStripEnergy[i][j] = new TH1F(hnameE, htitleE, 8192, 0, 8192);
          }
      }


   // read the data
   Int_t nentries = Chain->GetEntries();
//   nentries = 21000;
   cout << "TChain contains " << nentries << " events" << endl;
  
   for (Int_t i = 0; i < nentries; i++) 
    {
      if (i%10000 == 0) cout << "Entry " << i << endl;
      ///////////////////////
      // read SSSD branch //
      ///////////////////////
      
      Chain->GetEntry(i);
      // get StripE multiplicity
      Int_t multE = rawSSSD->GetEnergyMult();
      // loop on StripE multiplicity and get information concerning the event
      // fill histograms
      for (Int_t j = 0; j < multE; j++) 
        {
           Int_t det    = rawSSSD->GetEnergyDetectorNbr(j);
           Int_t strip = rawSSSD->GetEnergyStripNbr(j);
           Int_t energy = (Int_t) rawSSSD->GetEnergy(j);
           
           if ((det < NBTELESCOPE+1) && (strip < NBSTRIPS+1)) {
              hStripEnergy[det-1][strip-1]->Fill((Double_t)energy);
           }
           
           else {
              cout << "Error filling histograms: SSSD_E" << endl;
              rawSSSD->Dump();
           }
        }
     
      }

   // write on disk output file and close it
   outFile->Write();
   outFile->Close();
   
   return;
}
