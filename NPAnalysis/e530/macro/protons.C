/////////////////////////////////////////////////////////////
//
//    Read a CATS run and display some spectra             //
//
/////////////////////////////////////////////////////////////


#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TMust2Physics.h"
#include "TCATSPhysics.h"
#include "TTacData.h"

#include <iostream>

using namespace std;

void protons(const char* frun = "run_241_249")
{
  TH2F* E_Theta_calc     = new TH2F("Kinematics","Kinematics",180,100,160,5000,0,25000);
 
  //Read kinematics file
  double energy[90], theta[90];
  ifstream KineFile;
  string Kine_Path = "/vol0/sandra/These/cinematique/" ;
  string FilePath ;
  FilePath = Kine_Path + "cinematique_60Fe_fond.dat";	
  cout << FilePath << endl ;
  KineFile.open( FilePath.c_str() );
  if(KineFile.is_open()) cout << "open Ok " << endl ;
  else cout << "pb!" << endl;

  for(int i = 0; i< 90 ; i++)
    {
      KineFile >>  theta[i] >> energy[i];
      // cout <<  theta[i] << " " <<  energy[i] << endl;
      //      E_Theta_calc->Fill(theta,energy);
      energy[i] = energy[i];
    }
  
  TGraph * gr     = new TGraph(90,theta,energy); 
  cout << "done" << endl;

  // open the ROOT file to process
  TString path  = gSystem->Getenv("NPTOOL");
  path         += "/Outputs/Analysis/";

  TString inFileName = frun;
  TFile *inFile = new TFile(path + inFileName + ".root");
  inFile->ls();
  
  TTree *tree   = (TTree*) inFile->Get("Analysed_Data");
  
  //connect TMust2Physics Branch
  TMust2Physics *M2 = new TMust2Physics();
  TBranch *brMust2 = tree->GetBranch("MUST2");
  brMust2 ->SetAddress(&M2);

  //connect TCatsPhysics Branch
  TCATSPhysics *cats = new TCATSPhysics();
  TBranch *brCats = tree->GetBranch("CATS");
  brCats ->SetAddress(&cats);
  

  //connect TTacData Branch
  /*
  TTacData *Tac = new TTacData();
  TBranch *brTac = tree->GetBranch("TAC");
  brTac ->SetAddress(&Tac);
  */  

  // connect the E and Theta Branches
  double E, Theta;
  //UShort_t tac_pl_cats2;

  TBranch      *ELab = tree->GetBranch("ELab");
  ELab->SetAddress(&E);
  TBranch      *ThetaLab = tree->GetBranch("ThetaLab");
  ThetaLab->SetAddress(&Theta);
  /*
  TBranch      *Tac = tree->GetBranch("tac");
  Tac->SetAddress(&tac_pl_cats2);
  */
      
  // prepare output histograms 
  TH2F* target        = new TH2F("Positiontarget","PositionOnTarget",500,-30,30,500,-30,30);
  TH2F* E_Theta       = new TH2F("E vs theta tot","E vs theta",180,110,170,500,0,30);
  TH2F* E_Theta1      = new TH2F("E vs theta EDE","E vs theta EDE",180,110,170,500,0,30);
  TH2F* E_Theta2      = new TH2F("E vs theta ETof","E vs theta ETof",180,110,170,500,0,30);

  TH2F* DE_E          = new TH2F("DE vs E","DE vs E",1000,0,25000,1000,0,10000);
  TH2F* DE_E_protons  = new TH2F("DE vs E protons","DE vs E protons",1000,0,25000,1000,0,10000);
  TH2F* DE_E1         = new TH2F("DE vs E tel 1","DE vs E Tel1",1000,0,25000,1000,0,10000);
  TH2F* DE_E_protons1 = new TH2F("DE vs E protons Tel1","DE vs E protons Tel1",1000,0,25000,1000,0,10000);
 
  TH2F* DE_TOF           = new TH2F("E vs TOF","E vs TOF",120,360,400,500,0,10000);
  TH2F* DE_TOF_protons12 = new TH2F("E vs TOF protons","E vs TOF protons",120,360,400,500,0,10000);
  TH2F* DE_TOF_protons3  = new TH2F("E vs TOF protons","E vs TOF protons",120,360,400,500,0,10000);

  //TH2F* DE_TOF        = new TH2F("E vs TOF","E vs TOF",120,200,260,500,0,10000);
  //TH2F* DE_TOF_protons= new TH2F("E vs TOF protons","E vs TOF protons",120,200,260,500,0,10000);

  TH2F* position_CATS2  = new TH2F("position_CATS2","PositionY vs Position X (CATS2)",150,-10,10,150,-10,10);
  //TH2F* position_CATS2  = new TH2F("position_CATS2","PositionY vs Position X (CATS2)",100,-5000,5000,100,-5000,5000);

  //open the ROOT file for cuts
  TFile *FCuts = new TFile("cut_protons.root");
  TCutG *cut_protons = (TCutG*) FCuts->Get("protons");
  
  TFile *FCuts_Jo = new TFile("cut_e_tof_jo.root");
  TCutG *cut_protons_E_TOF_Jo = (TCutG*) FCuts_Jo->Get("protons_E_TOF_Jo");

  TFile *FCuts_1 = new TFile("cut_protons_E_TOF1.root");
  TCutG *cut_protons_E_TOF1 = (TCutG*) FCuts_1->Get("protons_E_TOF1");

  TFile *FCuts_2 = new TFile("cut_protons_E_TOF2.root");
  TCutG *cut_protons_E_TOF2 = (TCutG*) FCuts_2->Get("protons_E_TOF2");

  TFile *FCuts_3 = new TFile("cut_protons_E_TOF3.root");
  TCutG *cut_protons_E_TOF3 = (TCutG*) FCuts_3->Get("protons_E_TOF3");
  /*
  TFile *FCuts_cats = new TFile("cut_cats2.root");
  TCutG *cut_cats2 = (TCutG*) FCuts_cats->Get("cut_cats2");
  */
  int evt_EDE =0, evt_ETOF =0;
    
  // read the data
  
  int nentries = tree->GetEntries();
  cout << "TTree contains " << nentries << " events" << endl;
  

  for (int i = 0; i < nentries; i++) {
    //for (int i = 0; i < 100; i++) {
    if (i%10000 == 0) cout << "Entry " << i << endl;
    //////////////////////
    // read Cats branch //
    //////////////////////
    tree ->GetEntry(i);
   
    int Cats_NumberX = cats->GetCATSDetNumberX_Position(2);
    int Cats_NumberY = cats->GetCATSDetNumberY_Position(2);

   
    /*
    if(cut_cats2 -> IsInside(cats->GetCATSPositionX(Cats_NumberX),cats->GetCATSPositionY(Cats_NumberY)))
      {

      }
    */
    //else
      {

	//position_CATS2   -> Fill(cats -> GetPositionOnTargetX(), cats -> GetPositionOnTargetY());	
	position_CATS2   -> Fill(cats -> GetCATSPositionX(2), cats -> GetCATSPositionY(2));	
	
	//if(cats->PositionOnTargetX < 4000 && cats->PositionOnTargetX > 2000 && cats->PositionOnTargetY > 2000 && cats->PositionOnTargetY < 4000)
	if(cats->PositionOnTargetX > -20 && cats->PositionOnTargetX < 20 && cats->PositionOnTargetY > -20 && cats->PositionOnTargetY < 30)
	//if(cats->PositionOnTargetX > -30 && cats->PositionOnTargetX < -10 && cats->PositionOnTargetY > -20 && cats->PositionOnTargetY < 30)
	  { 
	    for(unsigned int hit = 0; hit < M2 -> Si_E.size() ; hit ++)
	      {
		//  if(tac_pl_cats2 > 3450 && tac_pl_cats2  < 3650)
		{
		  if(M2 -> TelescopeNumber[hit] !=4 ) 
		    {
		      if(M2 -> TelescopeNumber[hit]==1)  DE_E1   -> Fill(M2 -> SiLi_E[hit], M2 -> Si_E[hit]);
		      
		      DE_E   -> Fill(M2 -> SiLi_E[hit], M2 -> Si_E[hit]);
		      		      
		      if(cut_protons -> IsInside(M2 -> SiLi_E[hit], M2 -> Si_E[hit])) 
			{
			  evt_EDE ++;
			  DE_E_protons   -> Fill(M2 -> SiLi_E[hit], M2 -> Si_E[hit]);
			  
			  E_Theta        -> Fill(Theta,E);
			  E_Theta1       -> Fill(Theta,E);
			  
			  target->Fill(cats->PositionOnTargetX,cats->PositionOnTargetY);
			  //  M2->Dump();
			  
			  if(M2 -> TelescopeNumber[hit] == 1) DE_E_protons1  -> Fill(M2 -> SiLi_E[hit], M2 -> Si_E[hit]);
			}
		      
		      else 
			{
			  if(M2 -> TelescopeNumber[hit] == 1 ) 
			    {
			      evt_ETOF ++;
			      DE_TOF -> Fill(M2 -> Si_T[hit], M2 -> Si_E[hit]);		  
			      
			      //   if(cut_protons_E_TOF_Jo -> IsInside(M2 -> Si_T[hit], M2 -> Si_E[hit]) )
			      if(cut_protons_E_TOF1 -> IsInside(M2 -> Si_T[hit], M2 -> Si_E[hit]) )
			      	{
				  DE_TOF_protons12 -> Fill(M2 -> Si_T[hit], M2 -> Si_E[hit]);
				  E_Theta        -> Fill(Theta,E);
				  E_Theta2       -> Fill(Theta,E);
				  
				  target->Fill(cats->PositionOnTargetX,cats->PositionOnTargetY);
				  //M2->Dump();
				}
			    }
			  
			  else if(M2 -> TelescopeNumber[hit] == 2 ) 
			    {
			      evt_ETOF ++;
			      DE_TOF -> Fill(M2 -> Si_T[hit], M2 -> Si_E[hit]);		  
			      
			      //  if(cut_protons_E_TOF_Jo -> IsInside(M2 -> Si_T[hit], M2 -> Si_E[hit]) )
			      if(cut_protons_E_TOF2 -> IsInside(M2 -> Si_T[hit], M2 -> Si_E[hit]) )
			      {
				  DE_TOF_protons12 -> Fill(M2 -> Si_T[hit], M2 -> Si_E[hit]);
				  E_Theta        -> Fill(Theta,E);
				  E_Theta2       -> Fill(Theta,E);
				  
				  target->Fill(cats->PositionOnTargetX,cats->PositionOnTargetY);
				 
			    }
			    }
			  
			  else
			    {
			      //  if(cut_protons_E_TOF_Jo -> IsInside(M2 -> Si_T[hit], M2 -> Si_E[hit]) )
			      if(cut_protons_E_TOF3 -> IsInside(M2 -> Si_T[hit], M2 -> Si_E[hit]) )
			      {
				  evt_ETOF ++;
				  DE_TOF_protons3 -> Fill(M2 -> Si_T[hit], M2 -> Si_E[hit]);
				  E_Theta        -> Fill(Theta,E);
				  E_Theta2       -> Fill(Theta,E);
				  
				  target->Fill(cats->PositionOnTargetX,cats->PositionOnTargetY);
				 
			    }
			    }
			}  // cut protons

		    } // det Number not 4
		  
		} // tac condition if any
	      } //hit loop
	  } // condition target position
      }  // cut cats2
    
  }  // loop over the events
  
  
  cout << evt_EDE << " " << evt_ETOF << " " << endl;

  TCanvas * c1 = new TCanvas("c1","interesting",900,900);
  c1 -> Divide(2,2);
  c1-> cd(1);
  position_CATS2->Draw("colz");
  c1->cd(2);
  target->Draw("colz");
  c1->cd(3);
  E_Theta->Draw("colz");
  c1->cd(4);
  
  TCanvas * c2 = new TCanvas("c2","DE_E",900,900);
  c2->Divide(2,2);
  c2->cd(1);
  DE_E->Draw("colz");
  c2->cd(2);
  DE_E_protons->Draw("colz");

  
  TCanvas * c2b = new TCanvas("c2b","DE_Tof",900,900);
  c2b->Divide(2,2);
  c2b->cd(1);
  DE_TOF->Draw("colz");
  c2b->cd(3);
  DE_TOF_protons12->Draw("colz");
  c2b->cd(4);
  DE_TOF_protons3->Draw("colz");
 
  TCanvas * c3 = new TCanvas("c3","E_Theta",900,900);
  c3 -> Divide(2,2);
  c3->cd(1);
  E_Theta->Draw("colz");
  gr->Draw("same*");
  c3->cd(3);
  E_Theta1->Draw("colz");
  c3->cd(4);
  E_Theta2->Draw("colz");
  
  
  TCanvas * c4 = new TCanvas("c4","target",900,900);
  target->Draw("colz");

  TCanvas * c5 = new TCanvas("c5","cats2",900,900);
  position_CATS2->Draw("colz");
 
  
}
