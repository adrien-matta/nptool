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
 * Creation Date  : march 2015                                               *
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
#include"TNanaPhysics.h"
#include"TH1D.h"
#include"TH2F.h"
#include"TFile.h"
#include"TRandom.h"
class Analysis: public NPL::VAnalysis{
  public:
    Analysis();
    ~Analysis();

  public: 
    void Init();
    void TreatEvent();
    void End();
    std::vector<double> GammaPSD;
    std::vector<double> GammaE;
    std::vector<int> GammaDet,GammaEv;
    std::vector<unsigned long long> GammaT;
    unsigned long long initT;
    int counter, simault;
    long long  ts_start ,ts_end, ts_diff;
    double ts_diff_s;
    long multiLa;
    long bin;
    std::map<int,TH1*> fHistoTime, fHistoGamma,fHistoBTime;
    std::map<int,TH2*> fHisto2d,fHistoGT;
    


    static NPL::VAnalysis* Construct();
    
 int det_st;
 int det_lth;


    
    
 public: // Gamma spec analysis
 TH2D* gpsd;
 TH2D* gg;
 TH1D* CT;
 TH1D* TGT;
 TH1D* trip;
 string histo;

 private:
 TNanaPhysics* Nana;
 
 //These need to change 
 TH1D* h[4];
 TH1D* mult[4];
 //tHESE ARE RELIANT ON THE DET MAX WHICH IS GIVEN IN THE CXX FILE
    
 TH1D* backwards1stGate;
    TH1D* backwards2ndGate;
    TH1D* forwards1stGate;
    TH1D* forwards2ndGate;
    TH1D* Gate1;
    TH1D* Gate2;
    TH1D* nub;
    TH1D* sum57;
    TFile* HistoFile;
    
    
 protected:
    
   
};
#endif
