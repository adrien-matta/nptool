/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
  ConfigLassa
  MAX_STRIP_MULTIPLICITY 1
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

#include "Analysis.h"
#include "NPAnalysisFactory.h"
#include "NPDetectorManager.h"
#include "NPOptionManager.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "TInteractionCoordinates.h"
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){
  Lassa= (TLassaPhysics*) m_DetectorManager->GetDetector("LASSAArray");
  
  Initial = new TInitialConditions();
  InitOutputBranch();
  InitInputBranch();

  totalEvents = 0;
  detectedEvents = 0;
  peakEvents = 0;

  maxEnergy = -1.;
  minEnergy = 10000.;


  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 16; j++){
      detectedEvs[i][j] = 0;
      peakEvs[i][j] = 0;
      peakEffs[i][j] = -1.;  
      
      if(i < 8 && j < 8){ 
        peakEffsAvg[i][j] = -1.;
      }
    }
  }

  for(int i = 0; i < 5; i++){
    
    fitFunction = "";
    Beta.str("");
    Mu.str("");

    fitFunction = "1-exp(-1*";
    Beta << par[i][0];
    fitFunction += Beta.str();

    fitFunction += "*(x-";
    Mu << par[i][1];
    fitFunction += Mu.str();
    fitFunction += "))";

    ReactionLoss[i] = new TF1("Reaction Loss Fit Function",fitFunction.c_str(),0,100);
    
  }



 for(int i = 0; i < 5; i++){
  for(int j = 0; j < 2; j++){

  EnergyRange[i][j].clear();

    }
  }


  // Declare the histograms for the geometric
  // and peak efficiency

  // Peak Efficiencies

    h_peakEff  = new TH2F("Peak Efficiencies", "Peak Efficiencies",16,0,16,16,0,16);
    h_peakEff->GetXaxis()->SetTitle("X-Strip");
    h_peakEff->GetYaxis()->SetTitle("Y-Strip");

    h_peakEffAvg = new TH2F("Average Peak Efficiencies","Average Peak Efficiencies",8,0,8,8,0,8);
    h_peakEffAvg->GetXaxis()->SetTitle("X-Strip");
    h_peakEffAvg->GetYaxis()->SetTitle("Y-Strip");

    // Geometric Efficiencies

    hDetecTheta = new TH1F("hDetecTheta","DetecTheta",180,0,180);
    hDetecTheta->GetXaxis()->SetTitle("#Theta (deg)");
    hDetecTheta->GetYaxis()->SetTitle("Counts");

    hEmittTheta = new TH1F("hEmittTheta","EmittTheta",180,0,180);
    hEmittTheta->GetXaxis()->SetTitle("#Theta (deg)");
    hEmittTheta->GetYaxis()->SetTitle("Counts");

    hDetecThetaVsPhi = new TH2F("hDetecThetaVsPhi", "hDetecThetaVsPhi", 180,0,180,360,-180,180);
    hDetecThetaVsPhi->GetXaxis()->SetTitle("#Theta (deg)");
    hDetecThetaVsPhi->GetYaxis()->SetTitle("#Phi (deg)");

    hGeometricEfficiency = new TH1F("hEfficiency","Efficiency",180,0,180);
    hGeometricEfficiency->GetXaxis()->SetTitle("#Theta (deg)");
    hGeometricEfficiency->GetYaxis()->SetTitle("#epsilon (%)");

    // Initialize the Energy loss table for each species use. 

    for(int i = 0; i < 5; i++){
  
      EnergyLoss_Table = "_CsI.G4table";
      EnergyLoss_Table = speciesNameArray[i] + EnergyLoss_Table;

      EnergyLoss_CsI[i] = EnergyLoss(EnergyLoss_Table.c_str(),"G4Table",100);
    }
 
    peakEffSum_meat = 0.;
    peakEffSquaredSum_meat = 0.;
    peakEffAvg_meat = 0.;
    peakEffSquaredAvg_meat = 0.;
    N_meat = 0.;
    sigma_meat = 0.;  

    peakEffSum_edge = 0.;
    peakEffSquaredSum_edge = 0.;
    peakEffAvg_edge = 0.;
    peakEffSquaredAvg_edge = 0.;
    N_edge = 0.;
    sigma_edge = 0.;
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
  // Reinitiate calculated variable
  ReInitValue();

  totalEvents++;

  totalEnergy = Initial->GetKineticEnergy(0);
  speciesName = Initial->GetParticleName(0);  

  for(int i = 0; i < 5; i++){
    if(speciesName == speciesNameArray[i]){
      species = i;
    }
  }

  if(maxEnergy < totalEnergy){
    maxEnergy = totalEnergy;
  }

  if(minEnergy > totalEnergy){
    minEnergy = totalEnergy;  
  }

  if(minEnergy < .0001){
    minEnergy = 0.;
  }

  EDelta = .05*totalEnergy;

  ThetaLab_Init = Initial->GetThetaLab_WorldFrame(0);
  PhiLab_Init = Initial->GetPhiLab_WorldFrame(0);

  hEmittTheta->Fill(ThetaLab_Init);

  ///////////////////////////LOOP on Lassa Hit//////////////////////////////////
//  if(Lassa -> ThickSi_E.size() == 1){
  if(Lassa -> ThickSi_E.size() >= 1){ 

      TelescopeNumber = Lassa->TelescopeNumber[0];
      XStrip = Lassa->ThickSi_StripNumberX[0];
      YStrip = Lassa->ThickSi_StripNumberY[0];      

      ThetaLab_Detec = Initial->GetThetaLab_WorldFrame(0);
      PhiLab_Detec = Initial->GetPhiLab_WorldFrame(0);
      hDetecTheta->Fill(ThetaLab_Detec);

      hDetecThetaVsPhi->Fill(ThetaLab_Detec,PhiLab_Detec);

      X = Lassa->GetPositionOfInteraction(0).X();
      Y = Lassa->GetPositionOfInteraction(0).Y();
      Z = Lassa->GetPositionOfInteraction(0).Z();
  
      TVector3 PositionOnLassa = TVector3(X,Y,Z);
      TVector3 ZUnit = TVector3(0,0,1);

      double X_target = Initial->GetIncidentPositionX();
      double Y_target = Initial->GetIncidentPositionY();
      double Z_target = Initial->GetIncidentPositionZ();

      TVector3 PositionOnTarget = TVector3(X_target,Y_target,Z_target);
      TVector3 HitDirection = PositionOnLassa-PositionOnTarget;
      TVector3 HitDirectionUnit = HitDirection.Unit();

      TVector3 BeamDirection = Initial->GetBeamDirection();
      double XBeam = BeamDirection.X();
      double YBeam = BeamDirection.Y();
      double ZBeam = BeamDirection.Z();

      ThetaLab = BeamDirection.Angle(HitDirection);
      ThetaLab = ThetaLab/deg;

      PhiLab = HitDirection.Phi()/deg;

      E_ThickSi = Lassa->ThickSi_E[0];

      if(Lassa->CsI_E.size()>=1){  

        detectedEvents++;
        detectedEvs[XStrip][YStrip]++;
  
        E_CsI_Temp = Lassa->CsI_E[0];

     if(E_CsI_Temp > E_nucThres[species]){  

        zeroth_thres = .01;
        first_thres = .05;
        x_0 = -1;
        x_1 = -1;
        x_neg1 = -1;
        zeroth_upperbound = -1.;
        first_lowerbound = -1.;
        slope = 10000;
      

        // We want to check to see if the energy is within 2% of any of the stored energy values

        for(int i = 0; i < EnergyRange[species][1].size(); i++){

          zeroth_upperbound = (1+zeroth_thres)*EnergyRange[species][1][i];
          zeroth_lowerbound = (1-zeroth_thres)*EnergyRange[species][1][i];

          first_upperbound = (1+first_thres)*EnergyRange[species][1][i];
          first_lowerbound = (1-first_thres)*EnergyRange[species][1][i];

          if(zeroth_upperbound > E_CsI_Temp && E_CsI_Temp > zeroth_lowerbound) {x_0 = i;}
            
          if(first_upperbound > E_CsI_Temp && E_CsI_Temp > zeroth_upperbound){x_1 = i;}
            
          if(first_lowerbound < E_CsI_Temp && E_CsI_Temp < zeroth_lowerbound){x_neg1 = i;}

          if(x_0 != -1 && x_1 != -1 && x_neg1 != -1){break;}
           
        }

          // If there is no matching point, then a new (energy,range) point
          // will be added to the vector. 

        if(x_0 == -1){

          Range = EnergyLoss_CsI[species].EvaluateMaterialThickness(.0000001*MeV,Lassa->CsI_E[0]*MeV,15*cm,10*micrometer);
          Range_0 = Range; 
          Range_1 = Range;
          // Add a new point at the end
          EnergyRange[species][1].push_back(E_CsI_Temp);

          EnergyRange[species][2].push_back(Range);
        }

        else{

            if(x_1 == -1 || x_neg1 == -1){Range_1 = EnergyRange[species][2][x_0];}

            else{

              if(E_CsI_Temp > EnergyRange[species][1][x_0]){
                slope = (EnergyRange[species][2][x_1]-EnergyRange[species][2][x_0])/(EnergyRange[species][1][x_1]-EnergyRange[species][1][x_0]);
              }

              else {
              slope = (EnergyRange[species][2][x_0]-EnergyRange[species][2][x_neg1])/(EnergyRange[species][1][x_0]-EnergyRange[species][1][x_neg1]);
              } 
              
            Range_0 = EnergyRange[species][2][x_0];

            Range_1 = slope*(E_CsI_Temp-EnergyRange[species][1][x_0]) + Range_0;
          }       
        
      }

//        Range = EnergyLoss_CsI[species].EvaluateMaterialThickness(.0000001*MeV,Lassa->CsI_E[0]*MeV,15*cm,10*micrometer);
        Range = Range_1;

        eval = ReactionLoss[species]->Eval(Range/10);
        Random_value = Rand.Uniform(0,1); 

        if(Random_value>eval)E_CsI = Lassa->CsI_E[0];
        else E_CsI = Rand.Uniform(0,Lassa->CsI_E[0]);   

        }

        else {E_CsI = E_CsI_Temp;}

          //E_CsI = Lassa->CsI_E[0];
          
        ELab = E_ThickSi + E_CsI;
    
          //Check if the energy of the event is within the peak of the beam energy

        if(ELab > (totalEnergy-EDelta) && ELab < (totalEnergy+EDelta) && ELab > 0.){
          peakEvents++;
          peakEvs[XStrip][YStrip]++;
        } 
      }
    }      
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){

  geomEff = 100*((double)detectedEvents)/((double)totalEvents);

  peakEff = 100*((double)peakEvents)/((double)detectedEvents);

  hist_Title = "";
  canvas_Title = "";  

  maxEnergy_String.str("");

  maxEnergy_String << maxEnergy; 

  minEnergy_String.str("");

  minEnergy_String << minEnergy;

  cout << endl;
  cout << "Max Energy: " << maxEnergy << endl;
  cout << "Min Energy: " << minEnergy << endl;

  if(minEnergy == maxEnergy){
 
    hist_Title += speciesNameArray[species];
    hist_Title += " ";
    hist_Title += maxEnergy_String.str();
    hist_Title += " MeV";

    canvas_Title += "_";
    canvas_Title += speciesNameArray[species];
    canvas_Title += maxEnergy_String.str();
    canvas_Title += "MeV";
  }

  else{

    cout << endl;
    cout << "Else!" << endl;

    hist_Title += speciesNameArray[species];
    hist_Title += " ";
    hist_Title += minEnergy_String.str();
    hist_Title += "->";
    hist_Title += maxEnergy_String.str();
    hist_Title += " MeV";

    canvas_Title += "_";
    canvas_Title += speciesNameArray[species];
    canvas_Title += minEnergy_String.str();
    canvas_Title += "to";
    canvas_Title += maxEnergy_String.str();
    canvas_Title += "MeV";
  }

  // Files

  simData_Title = "simData";
  simData_Title += canvas_Title;
  simData_Title += ".csv";
  simData.open(simData_Title.c_str());

  peakEffMatrix_Title = "peakEffMatrix";
  peakEffMatrix_Title += canvas_Title;
  peakEffMatrix_Title += ".csv";
  peakEffMatrix.open(peakEffMatrix_Title.c_str());

  peakEffAvgMatrix_Title = "peakEffAvgMatrix";
  peakEffAvgMatrix_Title += canvas_Title;
  peakEffAvgMatrix_Title += ".csv";
  peakEffAvgMatrix.open(peakEffAvgMatrix_Title.c_str());

  detectEvMatrix_Title = "detectEvMatrix";
  detectEvMatrix_Title += canvas_Title;
  detectEvMatrix_Title += ".csv";
  detectEvMatrix.open(detectEvMatrix_Title.c_str());

  // Fill the histograms and files

  for(int i = 0; i < 16; i++){  
    for(int j = 0; j < 16; j++){
  
      if(detectedEvs[i][j] != 0){    
        peakEffs[i][j] = 100*((double)peakEvs[i][j])/((double)detectedEvs[i][j]);
        h_peakEff->Fill(i,j,peakEffs[i][j]);
      }

      else{
        peakEffs[i][j] = 0;
      }
    }
  }  

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      peakEffsAvg[i][j] = .25*(peakEffs[i][j] + peakEffs[i+8][j] + peakEffs[i][j+8] + peakEffs[i+8][j+8]);
      h_peakEffAvg->Fill(i,j,peakEffsAvg[i][j]);
    }
  }

  cout << endl;
  cout << "Total Events: " << totalEvents << endl;
  cout << "Detected Events: " << detectedEvents << endl;
  cout << "PeakEvents: " << peakEvents << endl;

  cout << "Geometric Efficiency: " << geomEff << endl;
  cout << "Peak Efficiency: " << peakEff << endl;

  cout << "Range List Length: " << EnergyRange[species][2].size() << endl;

  simData << "Species Name: " << speciesNameArray[species] << endl;
  simData << "Maximum Energy: " << maxEnergy << endl;
  simData << endl << endl;

  simData << "Total Events: " << totalEvents << endl;
  simData << "Detected Events: " << detectedEvents << endl;
  simData << "PeakEvents: " << peakEvents << endl;

  simData << "Geometric Efficiency: " << geomEff << endl;
  simData << "Peak Efficiency: " << peakEff << endl;

  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 16; j++){
   
      if(j == 0) peakEffMatrix << peakEffs[i][j] << ",";
      else{peakEffMatrix<< peakEffs[i][j] << ",";}
      if(j == 15) peakEffMatrix << endl; 

      if(j == 0) detectEvMatrix << detectedEvs[i][j] << ",";
      else{detectEvMatrix << detectedEvs[i][j] << ",";}
      if(j == 15) detectEvMatrix << endl;

      if(i < 8 && j < 8){

        if(j == 0) peakEffAvgMatrix << peakEffsAvg[i][j] << ",";
        else{peakEffAvgMatrix<< peakEffsAvg[i][j] << ",";}
        if(j == 15) peakEffAvgMatrix << endl;
      
      }
    }
  }


  // I want to calculate the mean and the standard deviation of
  // the peak efficiency in two regions (the meat of the crystal
  // and the edges). 

  // We will be averaging over the four quadrants of one detector.

  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 16; j++){

      if(0 < i && i < 7 && 0 < j && j < 7 || 0 < i && i < 7 && 8 < j && j < 15 || 8 < i && i < 15 && 0 < j && j < 7 || 8 < i && i < 15 && 8 < j && j < 15){
        peakEffSum_meat += peakEffs[i][j];
        peakEffSquaredSum_meat += peakEffs[i][j]*peakEffs[i][j]; 
        N_meat++;

        cout << endl;
        cout << "x: " << i << " " << "y: " << j << endl;
        cout << "Meat!" << endl;
        cout << "Peak Efficiency: " << peakEffs[i][j] << endl;

      }

      else{
        peakEffSum_edge += peakEffs[i][j];
        peakEffSquaredSum_edge += peakEffs[i][j]*peakEffs[i][j];
        N_edge++;

        cout << endl;
        cout << "x: " << i << " " << "y: " << j << endl;
        cout << "Edge!" << endl;
        cout << "Peak Efficiency: " << peakEffs[i][j] << endl;
      }
    }
  } 


  peakEffAvg_meat = peakEffSum_meat/N_meat;
  peakEffAvg_edge = peakEffSum_edge/N_edge;

  peakEffSquaredAvg_meat = peakEffSquaredSum_meat/N_meat;
  peakEffSquaredAvg_edge = peakEffSquaredSum_edge/N_edge;

  sigma_meat = sqrt(peakEffSquaredAvg_meat-peakEffAvg_meat*peakEffAvg_meat);
  sigma_edge = sqrt(peakEffSquaredAvg_edge-peakEffAvg_edge*peakEffAvg_edge);

  cout << endl;
  cout << "N_meat: " << N_meat << endl;
  cout << "N_edge: " << N_edge << endl;

  cout << "Peak Eff Avg Meat: " << peakEffAvg_meat << endl;
  cout << "Peak Eff Avg Edge: " << peakEffAvg_edge << endl;

  cout << "sigma_meat: " << sigma_meat << endl;
  cout << "sigma_edge: " << sigma_edge << endl;

  // Close the files

  detectEvMatrix.close();
  peakEffMatrix.close();
  peakEffAvgMatrix.close();
  simData.close();

  // Draw the histograms on canvases

  // Canvases
  
    // Peak Efficiencies
    
    canvas_peakEffMatrix = new TCanvas("Peak Eff Matrix","Peak Eff Matrix",1000,1000);
    canvas_peakEffMatrix->cd();
    h_peakEff->Draw("colz");
    h_peakEff->SetStats(0);
    h_peakEff->SetMaximum(100);
    h_peakEff->SetMinimum(30);
    peakEff_hTitle = "Peak Eff Matrix ";
    peakEff_hTitle += hist_Title;
    h_peakEff->SetTitle(peakEff_hTitle.c_str());
    peakEff_cTitle = "canvas_peakEffMatrix";
    peakEff_cTitle += canvas_Title;
    peakEff_cTitle += ".jpg";
    canvas_peakEffMatrix->SaveAs(peakEff_cTitle.c_str());

    canvas_peakEffAvgMatrix = new TCanvas("Peak Eff Average Matrix","Peak Eff Average",1000,1000);    
    canvas_peakEffAvgMatrix->cd();
    h_peakEffAvg->Draw("colz");
    h_peakEffAvg->SetStats(0);
    h_peakEffAvg->SetMaximum(100);
    h_peakEffAvg->SetMinimum(30);
    peakEffAvg_hTitle = "Peak Eff Average Matrix";
    peakEffAvg_hTitle += hist_Title;
    h_peakEff->SetTitle(peakEffAvg_hTitle.c_str());
    peakEffAvg_cTitle = "canvas_peakEffAvgMatrix";
    peakEffAvg_cTitle += canvas_Title;
    peakEffAvg_cTitle += ".jpg";
    canvas_peakEffAvgMatrix->SaveAs(peakEffAvg_cTitle.c_str());

    // Geometric Efficiency

    canvas_Emitt = new TCanvas("canvas_Emitt", "Distrib",900,900);
    canvas_Emitt->cd();
    hEmittTheta->Draw("");
    hDetecTheta->SetLineColor(kGreen);
    hDetecTheta->Draw("same");
  
    canvas_Detec = new TCanvas("canvas_Detec", "Distrib",900,900);
    canvas_Detec->cd();
    hDetecTheta->Draw("");

    canvas_DetecThetaVsPhi = new TCanvas("canvas_DetecThetaVsPhi", "Distrib",900,900);
    canvas_DetecThetaVsPhi->cd();
    hDetecThetaVsPhi->Draw("colz");

    canvas_GeometricEfficiency = new TCanvas("canvas_GeometricEfficiency", "Distrib",900,900);
    canvas_GeometricEfficiency->cd();
    hGeometricEfficiency->Divide(hDetecTheta, hEmittTheta, 100, 1);
    hGeometricEfficiency->Draw("");

  // Save canvases to jpg's

  emitt_hTitle = "Emitted Angle";
  emitt_hTitle += hist_Title;
  hEmittTheta->SetTitle(emitt_hTitle.c_str());

  detec_hTitle = "Detected Angle";
  detec_hTitle += hist_Title;
  hDetecTheta->SetTitle(detec_hTitle.c_str());

  detec_hTitle = "Detected Angle (Theta Vs Phi)";
  detec_hTitle += hist_Title;
  hDetecThetaVsPhi->SetTitle(detec_hTitle.c_str());
  
  geomEff_hTitle = "Detected Angle (Theta Vs Phi)";
  geomEff_hTitle += hist_Title;
  hGeometricEfficiency->SetTitle(geomEff_hTitle.c_str());

  emitt_cTitle = "canvas_Emitt";
  emitt_cTitle += canvas_Title;
  emitt_cTitle += ".jpg";
  canvas_Emitt->SaveAs(emitt_cTitle.c_str());

  detec_cTitle = "canvas_Detec";
  detec_cTitle += canvas_Title;
  detec_cTitle += ".jpg";
  canvas_Detec->SaveAs(detec_cTitle.c_str());

  detecThetaVsPhi_cTitle = "canvas_DetecThetaVsPhi";
  detecThetaVsPhi_cTitle += canvas_Title;
  detecThetaVsPhi_cTitle += ".jpg";  
  canvas_DetecThetaVsPhi->SaveAs(detecThetaVsPhi_cTitle.c_str());

  geomEff_cTitle = "canvas_GeometricEfficiency";
  geomEff_cTitle += canvas_Title;
  geomEff_cTitle += ".jpg";
  canvas_GeometricEfficiency->SaveAs(geomEff_cTitle.c_str());

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
  //RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("PhiLab",&PhiLab,"PhiLab/D");
//  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
//  RootOutput::getInstance()->GetTree()->Branch("totalEvents",&totalEvents,"totalEvents/I");
//  RootOutput::getInstance()->GetTree()->Branch("detectedEvents",&detectedEvents,"detectedEvents/I");
//  RootOutput::getInstance()->GetTree()->Branch("peakEvents",&peakEvents,"peakEvents/I");
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  RootInput:: getInstance()->GetChain()->SetBranchStatus("InitialConditions",true );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
  RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&Initial);
}

////////////////////////////////////////////////////////////////////////////////     
void Analysis::ReInitValue(){

  E_ThickSi = -1.;
  E_CsI = -1.;
  ELab = -1.;
  E_CsI_Temp = -1.;
  ThetaLab = -1000;
  PhiLab = -1000;
  X = -1000;
  Y = -1000;
  Z = -1000;
  TelescopeNumber = -1;
  EDelta = -1.;


  ThetaLab_Init = -1000;
  PhiLab_Init = -1000;

  fitFunction = "";
  Mu.str("");
  Beta.str("");
  species = -1;

  Range = -1.;
  Range_0 = -1.;
  Range_1 = -1.;
  devRange_0 = -1000.;
  devRange_1 = -1000.;
  eval = -1.;
  Random_value = -1.;

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

