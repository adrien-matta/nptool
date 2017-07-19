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
#include "TMicroballPhysics.h"
#include "THiraPhysics.h"
#include "TNeutronWallPhysics.h"
#include "TForwardArrayPhysics.h"
#include "TInitialConditions.h"
#include "TInteractionCoordinates.h"
#include "NPNucleus.h"

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
	double ThetaLabMB;
	double PhiLabMB;
	double ThetaLabHira;
	double PhiLabHira;
    double ThetaLabNW;
    double PhiLabNW;
    double NWEnergy;
    double NWTOF;
    double NWELab;
    double ThetaLabFA;
    double PhiLabFA;
    int MBMultiplicity;
    int NWMultiplicity;
    double X_Hira;
    double Y_Hira;
    double Z_Hira;
    double EF, E_CsI, HiraELab;

  private:
	TMicroballPhysics* MB;
	THiraPhysics* Hira;
    TNeutronWallPhysics* NW;
    TForwardArrayPhysics* FA;

	TInitialConditions* InitialConditions;
	TInteractionCoordinates* InteractionCoordinates;
    
    NPL::Nucleus* neutron;

};
#endif
