#ifndef Analysis_h 
#define Analysis_h
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
#include"NPVAnalysis.h"
#include"TLassaPhysics.h"
#include "TInitialConditions.h"
#include "NPEnergyLoss.h"
#include "NPReaction.h"
#include "TRandom3.h"
#include "TMath.h"
#include <sstream>
#include <string>
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include <iostream>
#include <fstream>
#include <vector>


class Analysis: public NPL::VAnalysis{
  public:
    Analysis();
    ~Analysis();

  public: 
    void Init();
    void TreatEvent();
    void End();
    void InitOutputBranch();
    void InitInputBranch();
    void ReInitValue();
    static NPL::VAnalysis* Construct();

  private:
    double ELab;
    double E_ThickSi;
    double E_CsI;
    double EDelta;
    double PhiLab;
    double ThetaLab;
    double ThetaLab_Init;
    double X,Y,Z;
    double TelescopeNumber;
    double thresholdEnergy;
    double totalEnergy;
    double maxEnergy;
    double minEnergy;
    ostringstream maxEnergy_String;
    ostringstream minEnergy_String;
    double E_CsI_Temp;
    double devRange_0;
    double devRange_1;
    double devRange_Sum_0;
    double devRange_Sum_1;
    double devRangeSquared_Sum_0;
    double devRangeSquared_Sum_1;
    double devRange_Avg_0;
    double devRangeSquared_Avg_0;
    double devRange_Avg_1;
    double devRangeSquared_Avg_1;
    double sigma_Range_0;
    double sigma_Range_1;
    double devRangeMax_0;
    double devRangeMax_1;

    string hist_Title;
    string canvas_Title;

    int XStrip;
    int YStrip;

    TLassaPhysics* Lassa;
    TInitialConditions* Initial;
    string speciesName;

    // intermediate variable
    TRandom3 Rand;

    // Geometric Efficiency
    double geomEff;
    TH1F* hEmittTheta;
    TH1F* hDetecTheta;
    TH2F* hDetecThetaVsPhi;
    TH1F* hGeometricEfficiency;
    TCanvas *canvas_Emitt;
    TCanvas *canvas_Detec;
    TCanvas *canvas_DetecThetaVsPhi;
    TCanvas *canvas_GeometricEfficiency;
    string emitt_Title;
    string Detec_Title;
    string DetecThetaVsPhi_Title;
    string GeometricEfficiency_Title;

    // Peak Efficiencies
    int totalEvents;
    int detectedEvents;
    int peakEvents;
    double peakEff;
    int detectedEvs[16][16];
    int peakEvs[16][16];
    double peakEffs[16][16];
    double peakEffsAvg[8][8];
    TH2F* h_peakEff;
    TH2F* h_peakEffAvg;
    TCanvas* canvas_peakEffMatrix;
    TCanvas* canvas_peakEffAvgMatrix;
    string peakEffs_Title;
    string peakEffsAvg_Title;  
 
    // Files 
    ofstream simData;
    ofstream peakEffMatrix;
    ofstream peakEffAvgMatrix;
    ofstream detectEvMatrix;  
    string simData_Title;
    string peakEffMatrix_Title;
    string peakEffAvgMatrix_Title;
    string detectEvMatrix_Title;


    // Range calculation
    double zeroth_upperbound;
    double zeroth_lowerbound;
    double first_upperbound;
    double first_lowerbound;
    double slope;
    double zeroth_thres;
    double first_thres;
    double Energy_Temp;
    double Range_0;
    double Range_1;
    int x_0;
    int x_1;
    int x_neg1;

    //Histogram and Canvas Titles
    string peakEff_hTitle;
    string peakEff_cTitle;
    string peakEffAvg_hTitle;
    string peakEffAvg_cTitle;
    string emitt_hTitle;
    string emitt_cTitle;
    string detec_hTitle;
    string detec_cTitle;
    string detecThetaVsPhi_hTitle;
    string detecThetaVsPhi_cTitle;
    string geomEff_hTitle;
    string geomEff_cTitle;

    // CsI Nuclear Reaction Correction Fit Function
    double Range;
    double maxRange;
    double eval;
    double Random_value;
    TF1* ReactionLoss[5];
    NPL::EnergyLoss EnergyLoss_CsI[5];
    string EnergyLoss_Name;
    string EnergyLoss_Table;
    string fitFunction;    
    ostringstream Mu;
    ostringstream Beta;
    int species;
    string speciesNameArray[5] = {"proton","deuteron","triton","He3","alpha"};
    double par[5][2] = {{0.0360046, 0.0921675},
                        {0.0473272, 0.0433612},
                        {0.0558374, 0.0327295},
                        {0.0478917, 0.0475857},
                        {0.0518713, 0.036529}};
    
    vector <double> EnergyRange[5][2];
    double E_nucThres[5] = {13.617,7.022,5.106,11.064,8.936};
//    double E_nucThres[5] = {0.,0.,0.,0.,0.};

  int N_meat;
  int N_edge;
  double peakEffSum_meat;
  double peakEffSum_edge;
  double peakEffAvg_meat;
  double peakEffAvg_edge;
  double peakEffSquaredSum_meat;
  double peakEffSquaredSum_edge;
  double peakEffSquaredAvg_meat;
  double peakEffSquaredAvg_edge;
  double sigma_meat;
  double sigma_edge;



};
#endif
