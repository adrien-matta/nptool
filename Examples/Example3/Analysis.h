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
#include"NPEnergyLoss.h"
#include"NPBeam.h"
#include"RootOutput.h"
#include"RootInput.h"
#include "TFatimaPhysics.h"
#include "TInitialConditions.h"
#include <TRandom3.h>
#include <TVector3.h>
#include <TMath.h>

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
  double ELab, EDepCor;
  double ThetaDop, beta;
  int Det_Fatima;
  int ANucleus;
  const static double ThetaDet[];

  NPL::Beam* myBeam;
  //NPL::DetectorManager* myDetector;

  TInitialConditions* myInit ;
  //TInteractionCoordinates* myIntCoord ;

  double OriginalBeamEnergy ; // MEV
  double Energy ;
  double E_Fatima ;
  int Mult_Fatima;

  TFatimaPhysics* Fat;

    TFile* HistoFile;
    TH1D* E_DopplerCor;
    TH1D* E_Recorded;


  
};
#endif
