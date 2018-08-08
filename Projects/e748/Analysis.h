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
#include "TMust2Physics.h"
#include "TCATSPhysics.h"
#include "TModularLeafPhysics.h"
#include "TInitialConditions.h"
#include "NPEnergyLoss.h"
#include "NPReaction.h"
#include "TRandom3.h"
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
    double Ex;
    double ELab;
    double ThetaLab;
    double ThetaCM;
    NPL::Reaction* myReaction;

    // intermediate variable
    TRandom3 Rand;
    int DetectorNumber;
    double ThetaNormalTarget;
    double ThetaM2Surface; 
    double X_M2;
    double Y_M2;
    double Z_M2;
    double Si_E_M2;
    double CsI_E_M2; 
    double Energy ;
    double E_M2 ;
    double Si_X_M2;
    double Si_Y_M2;
    double ZTarget;
    double TimeCorr;
    double BeamLength;
    double ParticleLength;
    double TargetThickness;
    double OriginalThetaLab;
    double OriginalELab;
    double BeamEnergy;
    int run_major;
    int run_minor;
    NPL::EnergyLoss He3CD2  ;
    NPL::EnergyLoss He3Al   ;
    NPL::EnergyLoss He3Si   ;
    NPL::EnergyLoss BeamCD2 ;
    NPL::EnergyLoss BeamMylar ;
    NPL::EnergyLoss BeamIsobutane ;



    TMust2Physics* M2;
   TCATSPhysics* CATS;
   TModularLeafPhysics* ModularLeaf;
   TInitialConditions* Initial;
    //other variables 
   Short_t         vADC_CHIO_V15;
   Short_t         vADC_VOIE_29;
   Short_t         vCHIO;
   Short_t         vCONFDEC;
   Short_t         vCONFDEC_AGAVA;
   Short_t         vDATATRIG;
   Short_t         vDATATRIG_CHIO;
   Short_t         vE1D6;
   Short_t         vE2D6;
   Short_t         vED4;
   Short_t         vEXL_HF;
   Short_t         vGALD4X;
   Short_t         vGALD4Y;
   Short_t         vGATCONF;
   Short_t         vQCaviar;
   Short_t         vQPlast;
   Short_t         vTCAVHF;
   Short_t         vTE1D6CAV;
   Short_t         vTE1D6GAL;
   Short_t         vTE1D6HF;
   Short_t         vTED4HF;
   Short_t         vTGALD4HF;
   Short_t         vT_CATS1_2;
   Short_t         vT_CATS1_CAV;
   Short_t         vT_MUVI_CATS1;
   Short_t         vT_PL_CATS1;
   Short_t         vT_PL_CATS2;
   Short_t         vT_PL_CHIO;
   Short_t         vT_PLchCATS1;


};
#endif
