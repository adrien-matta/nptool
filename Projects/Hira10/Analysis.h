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
#include"THiraPhysics.h"
#include "TInitialConditions.h"
#include "TInteractionCoordinates.h"
#include "NPEnergyLoss.h"
#include "NPReaction.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TLorentzVector.h"

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
    
    double event;
    double good_event;
    double ProtonEnergy;
    
private:
    double ThicknessCsI;
    double TargetThickness;
    double ExcitationEnergy;
    double ELab;
    double ECM;
    double ECM_initial;
    double ECM_initial_Hira;
    double E_ThinSi;
    double E_ThickSi;
    double E_CsI;
    double PhiLab;
    double ThetaLab;
    double ThetaLab_simu;
    double ThetaCM;
    double X,Y,Z;
    double TelescopeNumber;
    double EnergyThreshold;
    double X_target;
    double Y_target;
    double InitialEnergy;
    double InitialEnergy_Hira;
    double Rapidity_Lab;
    double Rapidity_CM;
    double Pper;
    double PID;
    int Particle;
    
    
    
    NPL::Reaction* TransferReaction;
    
    // intermediate variable
    TRandom3 Rand;
    
    
    TF1* f_proton;
    TF1* f_deuton;
    TF1* f_triton;
    TF1* f_3He;
    
    NPL::EnergyLoss Proton_CsI  ;
    NPL::EnergyLoss Deuton_CsI  ;
    NPL::EnergyLoss Triton_CsI  ;
    NPL::EnergyLoss Triton_CD2  ;
    NPL::EnergyLoss Triton_CH2  ;
    NPL::EnergyLoss Deuteron_CH2  ;
    NPL::EnergyLoss He3_CD2;
    NPL::EnergyLoss He3_CsI;
    NPL::EnergyLoss He4_CH2;
    
    NPL::Nucleus* proton;
    NPL::Nucleus* deuton;
    NPL::Nucleus* triton;
    NPL::Nucleus* helium3;
    NPL::Nucleus* alpha;
    NPL::Nucleus* beam;
    NPL::Nucleus* target;
    
    THiraPhysics* Hira;
    TInitialConditions* InitialConditions;
    TInteractionCoordinates* InteractionCoordinates;
};
#endif