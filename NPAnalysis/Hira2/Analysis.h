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
    double TargetThickness;
    double ExcitationEnergy;
    double ELab;
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
    
    
    
    NPL::Reaction* TransferReaction;
    
    // intermediate variable
    TRandom3 Rand;
    
    
    TF1* f_proton;
    NPL::EnergyLoss Proton_CsI  ;
    
    THiraPhysics* Hira;
    TInitialConditions* InitialConditions;
    TInteractionCoordinates* InteractionCoordinates;
};
#endif