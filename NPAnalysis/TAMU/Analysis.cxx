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
#include<iostream>
using namespace std;
#include"Analysis.h"
#include"NPAnalysisFactory.h"
#include"NPDetectorManager.h"
#include"NPOptionManager.h"
#include"RootOutput.h"
#include"RootInput.h"
////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){
	TH  = (TTiaraHyballPhysics*) m_DetectorManager -> GetDetector("TiaraHyballWedge");
  TB  = (TTiaraBarrelPhysics*) m_DetectorManager -> GetDetector("TiaraInnerBarrel=");
  proton_CD2 = EnergyLoss("proton_CD2.G4table","G4Table",100 );
  proton_Al = EnergyLoss("proton_Al.G4table","G4Table",10);
  proton_Si = EnergyLoss("proton_Si.G4table","G4Table",10);
  P30_CD2 = EnergyLoss("P30[0.0]_CD2.G4table","G4Table",100);
  Initial = new TInitialConditions();


  Rand = new TRandom3();
	ThetaNormalTarget = 0 ;
	ThetaTHSurface = 0;
  ThetaTBSurface = 0;
	Si_E_TH = 0 ;
  Si_E_InnerTB = 0;
  Si_E_OuterTB = 0;
  Si_E_TB = 0 ;
	Energy = 0;
	TargetThickness = m_DetectorManager->GetTargetThickness()*micrometer;
	
  XTarget = 0;
	YTarget =0;
	BeamDirection = TVector3(0,0,1);
	P30dpReaction = new Reaction ;
	P30dpReaction -> ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile()) ;

 InitOutputBranch();
 InitInputBranch();

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
		// Reinitiate calculated variable
		ReInitValue();

		// Beam energy is measured using F3 and F2 plastic TOF (time of flight)
		double BeamEnergy = Rand->Gaus(Initial->GetIncidentInitialKineticEnergy(),4.5);
		BeamEnergy = P30_CD2.Slow(BeamEnergy,TargetThickness/2.,0);
		P30dpReaction->SetBeamEnergy(BeamEnergy);

////////////////////////////////////////// LOOP on TiaraHyball + SSSD Hit //////////////////////////////////////////
		for(unsigned int countTiaraHyball = 0 ; countTiaraHyball < TH->Strip_E.size() ; countTiaraHyball++){
			/************************************************/

			// TiaraHyball

			/************************************************/

			// Part 1 : Impact Angle
			ThetaTHSurface = 0;
			ThetaNormalTarget = 0;
			if(XTarget>-1000 && YTarget>-1000){
				TVector3 BeamImpact(XTarget,YTarget,0);
				TVector3 HitDirection = TH -> GetRandomisedPositionOfInteraction(countTiaraHyball) - BeamImpact ;
				ThetaLab = HitDirection.Angle( BeamDirection );
				ThetaTHSurface = HitDirection.Angle(TVector3(0,0,-1) );
				ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
			}
			else{
				BeamDirection = TVector3(-1000,-1000,-1000);
				ThetaTHSurface    = -1000  ;
				ThetaNormalTarget = -1000  ;
			}

			/************************************************/

			// Part 2 : Impact Energy
			Energy = ELab = 0;
			Si_E_TH = TH->Strip_E[countTiaraHyball];
			Energy = Si_E_TH;

			// Evaluate energy using the thickness 
			ELab = proton_Al.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaTHSurface); 
			// Target Correction
			ELab = proton_CD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget); 

			/************************************************/

			// Part 3 : Excitation Energy Calculation
			Ex = P30dpReaction -> ReconstructRelativistic( ELab , ThetaLab );

			/************************************************/

			// Part 4 : Theta CM Calculation
			ThetaCM  = P30dpReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
		  ThetaLab=ThetaLab/deg;
	
      /************************************************/

      // Part 5 : Implementing impact matrix for the entire Hyball (all 6 wedges)
      /*TVector3 HyballImpactPosition = TH -> GetPositionOfInteraction(countTiaraHyball);
      HyballIMX = HyballImpactPosition.X();
      HyballIMY = HyballImpactPosition.Y();
      HyballIMZ = HyballImpactPosition.Z();*/

      /************************************************/

      // Part 6 : Implementing randomised position impact matrix for the Hyball
      TVector3 HyballRandomImpactPosition = TH -> GetRandomisedPositionOfInteraction(countTiaraHyball);
      TiaraIMX = HyballRandomImpactPosition.X();
      TiaraIMY = HyballRandomImpactPosition.Y();
      TiaraIMZ = HyballRandomImpactPosition.Z();

      /************************************************/

		} // end loop TiaraHyball

////////////////////////////////////////// LOOP on TiaraBarrel /////////////////////////////////////////////////////
		for(unsigned int countTiaraBarrel = 0 ; countTiaraBarrel < TB->Strip_E.size() ; countTiaraBarrel++){
			/************************************************/

      //TiaraBarrel

			/************************************************/

			// Part 1 : Impact Angle
			ThetaTBSurface = 0;
			ThetaNormalTarget = 0;
			if(XTarget>-1000 && YTarget>-1000){
				TVector3 BeamImpact(XTarget,YTarget,0);
				TVector3 HitDirection = TB -> GetRandomisedPositionOfInteraction(countTiaraBarrel) - BeamImpact ;
				ThetaLab = HitDirection.Angle( BeamDirection );
				ThetaTBSurface = HitDirection.Angle(TVector3(0,0,-1) );
				ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
			}
			else{
				BeamDirection = TVector3(-1000,-1000,-1000);
				ThetaTBSurface    = -1000  ;
				ThetaNormalTarget = -1000  ;
			}
			/************************************************/

      // Part 2 : Impact Energy
			Energy = ELab = 0;
			Si_E_InnerTB = TB->Strip_E[countTiaraBarrel];
      if(TB->Outer_Strip_E[countTiaraBarrel] != -1000){
        Si_E_OuterTB = TB->Outer_Strip_E[countTiaraBarrel];
        Energy = Si_E_InnerTB + Si_E_OuterTB;
      }
			else {
        Energy = Si_E_InnerTB;
      }

			// Evaluate energy using the thickness 
			ELab = proton_Al.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaTBSurface); 
			// Target Correction
			ELab = proton_CD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);

			/************************************************/

			// Part 3 : Excitation Energy Calculation
			Ex = P30dpReaction -> ReconstructRelativistic( ELab , ThetaLab );

		  /************************************************/

			// Part 4 : Theta CM Calculation
			ThetaCM  = P30dpReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
		  ThetaLab=ThetaLab/deg;
	
      /************************************************/      

      // Part 5 : Implementing impact matrix for the Tiara Barrel (all 8 detecting strips)
      /*TVector3 BarrelImpactPosition = TB -> GetPositionOfInteraction(countTiaraBarrel);
      BarrelIMX = BarrelImpactPosition.X();
      BarrelIMY = BarrelImpactPosition.Y();
      BarrelIMZ = BarrelImpactPosition.Z();*/

      /************************************************/

      // Part 6 : Implementing randomised position impact matrix for both the entire Barrel (all 8 strips) and each strip making up the octagonal Barrel individually
      TVector3 BarrelRandomImpactPosition = TB -> GetRandomisedPositionOfInteraction(countTiaraBarrel);
      TiaraIMX = BarrelRandomImpactPosition.X();
      TiaraIMY = BarrelRandomImpactPosition.Y();
      TiaraIMZ = BarrelRandomImpactPosition.Z();

      /************************************************/

		} // end loop TiaraBarrel
/////////////////////////////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
}


void Analysis::ReInitValue(){
	Ex = -1000 ;
	ELab = -1000;
	ThetaLab = -1000;
	ThetaCM = -1000;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
	RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
	RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
	RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
	RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixX",&TiaraIMX,"TiaraImpactMatrixX/D");
	RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixY",&TiaraIMY,"TiaraImpactMatrixY/D");
	RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixZ",&TiaraIMZ,"TiaraImpactMatrixZ/D");
}
/////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
	RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&Initial );
	RootInput:: getInstance()->GetChain()->SetBranchStatus("InitialConditions",true );
	RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
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

