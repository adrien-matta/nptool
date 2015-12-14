/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : march 2025                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Class describing the property of an Analysis object                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include<iostream>
using namespace std;
#include"Analysis.h"
#include"NPAnalysisFactory.h"
#include"NPDetectorManager.h"
////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){
  //HistoFile=new TFile("/media/rs00178/rob_hdd/PSA_Code/Histograms/25ns_Eu_4hr","RECREATE");

  HistoFile=new TFile("Eu_Simu.root","RECREATE");
  Nana= (TNanaPhysics*) m_DetectorManager->GetDetector("Nana");

  int tb =201 ;
  int ts = 100;


  int bs = 1024;  //int bs = 2048;
  double ss = 4096;
  TGT = new TH1D("TGT","TGT",ts,0,tb);
  gg = new TH2D("gg","gg",bs,0,ss,bs,0,ss);
  gg->SetTitle("Total g-g Coincidence Plot");
  gg->SetYTitle("Energy (keV)");
  gg->SetXTitle("Energy (keV)");

  bin = 1e4;


  //*****************************************************************************
  //*********************IMPORTANT***********************************************
  //*****************************************************************************
  //number of the detectors pushed back (should be detector channels uysed in digi!)
  //CONSECUTIVE CHANNELS ONLY! THISNEEDS WORK
  det_st = 1;
  det_lth = 2;

  gpsd = new TH2D("Gamma_vs_PSD","Gamma_vs_PSD",1024,0,4096,1000,0,1);
  trip = new TH1D("Trip","Trip",bs,0,ss);
  Gate2  = new TH1D("2ndGate","2ndGate",bs,0,ss);
  backwards2ndGate = new TH1D("backwards2ndGate","backwards2ndGate",bs,0,ss);
  forwards2ndGate = new TH1D("forwards2ndGate","forwards2ndGate",bs,0,ss);
  Gate1 = new TH1D("1stGate","1stGate",bs,0,ss);
  backwards1stGate = new TH1D("backwards1stGate","backwards1stGate",bs,0,ss);
  forwards1stGate = new TH1D("forwards1stGate","forwards1stGate",bs,0,ss);

  CT = new TH1D("CT","CT",ts,0,tb);

  int timeid,id2d;

  for(int i = det_st; i <det_st+det_lth ;i++){
    for(int j = i; j < det_st+det_lth; j++){
      TH1D* h = new TH1D(Form("Det_%i and_%i Time_Diff_Plot",i,j),
          Form("Det_%i and_%i Time_Diff_Plot",i,j),ts,-tb,tb);

      TH1D* g = new TH1D(Form("Det_%i_and_%i Gamma_Coinc_Plot",i,j),
          Form("Det_%i_and_%i_Gamma_Coinc_Plot",i,j),bs,0,ss);

      TH2D* k = new TH2D(Form("GG_of_%i&%i",i,j),Form("GG_of_%i&%i",i,j),bs,0,ss,bs,0,ss);


      TH1D* l = new TH1D(Form("BACKWARDSDet_%i and_%i Time_Diff_Plot",i,j),
          Form("BACKWARDSDet_%i and_%i Time_Diff_Plot",i,j),ts,0,tb);

      timeid = i*100 + j*10;
      id2d = (i+1)*(j+1);
      fHisto2d[id2d]=k;
      timeid = i*100 + j*10;
      fHistoTime[timeid]=h;
      fHistoGamma[timeid]=g;
      fHistoBTime[timeid]=l;
    }
  }

  for(int i = det_st + det_lth -1; i > det_st -1; i--){
    TH2D* gt = new TH2D(Form("Gamma_vs_Time_For_Det#%i",i),Form("Gamma_vs_Time_For_Det#%i",i),1000,0,1000,2048,0,2048);
    fHistoGT[i] = gt;
    for(int j = i-1; j > det_st -1; j--){
      TH1D* h = new TH1D(Form("Det_%i and_%i Time_Diff_Plot",i,j),
          Form("Det_%i and_%i Time_Diff_Plot",i,j),ts,-tb,tb);


      TH1D* g = new TH1D(Form("Det_%i_and_%i_Gamma_Coinc_Plot",i,j),
          Form("Det_%i_and_%i_Gamma_Coinc_Plot",i,j),bs,0,ss);

      TH1D* l = new TH1D(Form("BACKWARDSDet%iand_%iTime_Diff_Plot",i,j),
          Form("BACKWARDSDet_%i and_%i Time_Diff_Plot",i,j),ts,0,tb);
      timeid = i*100 + j*10;
      fHistoTime[timeid]=h;
      fHistoGamma[timeid]=g;
      fHistoBTime[timeid]=l;

    }
  }

  for(int i = 0; i <3; i++){
    // may need more due to the nature of the timing so the reverse would need own histograms
    mult[i]= new TH1D(Form("multiplicity%i",i+1),Form("multiplicity%i",i+1),bs,0,ss);
  }
  for(int i = 0; i <det_lth; i++){ 
    h[i]= new TH1D(Form("Detector%i",i+det_st),Form("Detector%i",i+det_st),bs,0,ss);
    sum57 = new TH1D(Form("sum%i",i+det_st),Form("sum%i",i+det_st),bs,0,ss);
  }
  }

  ////////////////////////////////////////////////////////////////////////////////
  void Analysis::TreatEvent(){
    // counter++
    std::vector<int> GammaGateHI;
    std::vector<int> GammaGateLO;
    GammaGateLO.push_back(1383);
    GammaGateHI.push_back(1433);
    GammaGateLO.push_back(98);
    GammaGateHI.push_back(137);
    GammaGateLO.push_back(319);
    GammaGateHI.push_back(369);
    GammaGateLO.push_back(753);
    GammaGateHI.push_back(803);


    double time;

    int v1 = rand() % 100; 

    for(unsigned int i = 0 ; i <Nana->GetEventData()->GetNanaLaBr3Mult() ; i++){   
      sum57->Fill(Nana->GetEventData()->GetNanaLaBr3EnergyLong(i)*1000 );

      for( int j = det_st ; j < det_st+det_lth; j++){
        if(Nana->GetEventData()->GetNanaLaBr3DetectorNbr(i) ==j){
          h[j-det_st]->Fill( Nana->GetEventData()->GetNanaLaBr3EnergyLong(i)*1000 );
        }
      }
      for(int k = 1 ; k <6; k++){
        if(Nana->GetEventData()->GetNanaLaBr3Mult() == k){

          mult[k-1]->Fill(Nana->GetEventData()->GetNanaLaBr3EnergyLong(i)*1000);

        }
      }

      if(Nana->GetEventData()->GetNanaLaBr3Mult()>1){
        GammaE.push_back(Nana->GetEventData()->GetNanaLaBr3EnergyLong(i)*1000);
        GammaT.push_back(Nana->GetEventData()->GetNanaLaBr3Time(i));
        GammaPSD.push_back(Nana->GetEventData()->GetNanaLaBr3PSD(i));
        GammaDet.push_back(Nana->GetEventData()->GetNanaLaBr3DetectorNbr(i));
        GammaEv.push_back(Nana->GetEventData()->GetNanaLaBr3Event(i));
      }


    }//End of the for loop over all of the hits within the event 

    //Processing over the two vectors for coincident
    // note at the ns regime it is impossible to sort events that happen at the same time, it is therefore impossible to put them in the correct histogram. At the moment this is corrected by putting them in both possible histograms. 
    if(Nana->GetEventData()->GetNanaLaBr3Mult()>1){
      int btime;
      int i;
      int j;
      for( i = 0 ; i < GammaE.size(); i++){
        time =double(GammaT[i])/1000.;
        TH2D* gt = dynamic_cast<TH2D*>(fHistoGT[GammaDet[i]]);
        fHistoGT[GammaDet[i]]->Fill(time,GammaE[i]);
        gpsd->Fill(GammaE[i],GammaPSD[i]);
        for( j = i+1; j < GammaE.size(); j++){
          ts_diff= GammaT[j]-GammaT[i];

          gg->Fill(GammaE[i],GammaE[j]);
          gg->Fill(GammaE[j],GammaE[i]);


          //This is for the absolute time code, here this will never happen... he said
          if(GammaEv[i]-GammaEv[j]!=0)  cout << GammaEv[i]-GammaEv[j] <<endl;

          //regular gated spectrum, asks if 380ish was followed by 80ish anywhere in the event
          if(GammaE[i] > GammaGateLO[0] && GammaE[i] < GammaGateHI[0] && GammaE[j] > GammaGateLO[1] && GammaE[j] < GammaGateHI[1]){
            TH1D* t = dynamic_cast<TH1D*>(fHistoGamma[GammaDet[i]*100+GammaDet[j]*10]);
            t->Fill(GammaE[j]+double(v1/bin));
            //i is first and the first gate is the gamma that is emitted first.

            TH1D* g = dynamic_cast<TH1D*>(fHistoTime[GammaDet[i]*100+GammaDet[j]*10]);
            g->Fill(GammaT[j]-GammaT[i]);
          }//end of gating on 1 to 2 in cascade
          //asks if the 2nd gamma in cascade was followed by the first (non-physical, still happens)
          if(GammaE[i] > GammaGateLO[1] && GammaE[i] < GammaGateHI[1] && GammaE[j] > GammaGateLO[0] && GammaE[j] < GammaGateHI[0]){

            TH1D* l = dynamic_cast<TH1D*>(fHistoBTime[GammaDet[i]*100+GammaDet[j]*10]);
            l->Fill(GammaT[j]-GammaT[i]);
            TH1D* g = dynamic_cast<TH1D*>(fHistoTime[GammaDet[j]*100+GammaDet[i]*10]);
            btime = GammaT[i]-GammaT[j];
            g->Fill(btime);

          }//end of 2-1 gating
          //g-g matrix making(should add these together, no reason they arent the same... so there isnt a reason there are two histograms)
          TH2D* k = dynamic_cast<TH2D*>(fHisto2d[(GammaDet[i]+1)*(GammaDet[j]+1)]);
          k->Fill(GammaE[i],GammaE[j]) ;
          k->Fill(GammaE[j],GammaE[i]);

          if (GammaT[i] == GammaT[j]) simault++;
          //Gated Spectrum Manipulation
          //Names are GGHI GGLO 
          if(GammaE[i] > GammaGateLO[0] && GammaE[i] < GammaGateHI[0]){
            //what followed the 1st in cascade
            forwards1stGate->Fill(GammaE[j]);
          }

          if(GammaE[i] > GammaGateLO[1] && GammaE[i] < GammaGateHI[1]){
            //what followed the second
            forwards2ndGate->Fill(GammaE[j]);
          }

          if(GammaE[j] > GammaGateLO[0] && GammaE[j] < GammaGateHI[0]){
            //what was before the 1st in cascade
            backwards1stGate->Fill(GammaE[i]);
          }

          if(GammaE[j] > GammaGateLO[1] && GammaE[j] < GammaGateHI[1]){
            //what was before the second..
            backwards2ndGate->Fill(GammaE[i]);
          }


        }
        if(Nana->GetEventData()->GetNanaLaBr3Mult()>3){
          for(int r= j+1; r <  GammaE.size(); r++){
            if(GammaE[i] > 340 && GammaE[i] < 370){
              if(GammaE[j] >70 && GammaE[j] < 90){
                trip->Fill(GammaE[r]);

              }
            }
          }
        } 
      }
    }
    GammaEv.clear();
    GammaPSD.clear();
    GammaE.clear();
    GammaT.clear();
    GammaDet.clear();
    multiLa = 0;
  }

  ////////////////////////////////////////////////////////////////////////////////
  void Analysis::End(){
    Gate1->Add(backwards1stGate);
    HistoFile->mkdir("2DMatrices","2DMatrices");
    HistoFile->mkdir("Multiplicity","Multiplicity");
    HistoFile->Write();
    HistoFile->Close();
  }


  ////////////////////////////////////////////////////////////////////////////////
  //            Construct Method to be pass to the DetectorFactory              //
  ////////////////////////////////////////////////////////////////////////////////
  NPL::VAnalysis* Analysis::Construct(){
    return (NPL::VAnalysis*) new Analysis();
  }

  ////////////////////////////////////////////////////////////////////////////////
  //            Registering the construct method to the factory                 //
  ////////////////////////////////////////////////////////////////////////////////
  extern "C"{
    class proxy{
      public:
        proxy(){
          NPL::AnalysisFactory::getInstance()->SetConstructor(Analysis::Construct);
        }
    };

  proxy p;
}

