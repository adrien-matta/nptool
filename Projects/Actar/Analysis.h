#ifndef Analysis_h
#define Analysis_h
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Actar analysis project                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include "NPVAnalysis.h"
#include "TActarPhysics.h"
#include "NPEnergyLoss.h"
#include "NPReaction.h"
#include "NPTrack.h"



class Analysis: public NPL::VAnalysis{
public:
    Analysis();
    ~Analysis();
    
public:
    void Init();
    void TreatEvent();
    void End();
    void InitOutputBranch();
    void ReInitValue();
    void GetMayaSiHitPosition(double xm, double xh, double ym, double yh, double zm, double zh);

    
    
    static NPL::VAnalysis* Construct();
    
public:
    double DriftVelocity;
    double PadSizeX;
    double PadSizeY;
    int NumberOfPadsX;
    int NumberOfPadsY;
    
    
private:
    double fSiDistanceX=128+47;
    
    TActarPhysics* Actar;
    
    vector<NPL::Track> vTrack;
    
    double BeamAngle;
    vector<double> vScalar;
    vector<double> ThetaLab;
    vector<double> ELab;
    vector<int> SiNumber;
    vector<double> ESi;
    vector<double> DE;
    vector<double> Ex;
    vector<double> ThetaCM;
    vector<double> XVertex;
    vector<double> YVertex;
    vector<double> ZVertex;
    vector<double> SiPosY;
    vector<double> SiPosZ;
    
    NPL::EnergyLoss EnergyLoss_3He;
    NPL::EnergyLoss EnergyLoss_17C;
    NPL::Reaction* TheReaction;
    
    
};
#endif
