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
#include "NPVAnalysis.h"
#include "TLassaPhysics.h"
#include "TInitialConditions.h"
#include "NPEnergyLoss.h"
#include "NPReaction.h"
#include "TRandom3.h"
#include "TF1.h"
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
    double ECM;
    double ThetaCM;
    double ELab_nucl;
    double E_ThickSi;
    double E_CsI;
    double PhiLab;
    double ThetaLab;
    double ThetaLabInitial;
    double X,Y,Z;
    double TelescopeNumber;
    double thresholdEnergy;
    double InitialEnergy;
    double InitialEnergy_Lassa;
    double ECM_initial;
    double ECM_initial_Lassa;
    double ThicknessCsI;
    double R_alpha;
    double PID;
    double y_lab;
    double y0_lab;
    double y_CM;
    double y0_CM;
    double y_beam_lab;
    double y_beam_CM;
    double Ptot;
    double Pper;
    
    int totalEvents;
    int detectedEvents;
    int peakEvents;

    double geomEff;
    double peakEff;

    // intermediate variable
    TRandom3 Rand;
    
    TF1* f_proton;
    TF1* f_deuton;
    TF1* f_triton;
    
    //Energy loss table
    NPL::EnergyLoss Proton_CsI;
    NPL::EnergyLoss Deuton_CsI;
    NPL::EnergyLoss Triton_CsI;
    NPL::EnergyLoss He3_CsI;
    
    NPL::Nucleus *proton;
    NPL::Nucleus *deuton;
    NPL::Nucleus *triton;
    NPL::Nucleus *helium3;
    NPL::Nucleus *alpha;
    NPL::Nucleus *beam;
    NPL::Nucleus *target;
    NPL::Nucleus *lightparticle;
    
    TLassaPhysics* Lassa;
	TInitialConditions* InitialConditions;
};
#endif
