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

#include "Analysis.h"
#include "NPFunction.h"
#include "NPAnalysisFactory.h"
#include "NPDetectorManager.h"
#include "NPOptionManager.h"
////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init() {

  agata_zShift=51*mm;

  // initialize input and output branches
  InitOutputBranch();
  InitInputBranch();
  // get MUST2 and Gaspard objects
  M2 = (TMust2Physics*)  m_DetectorManager -> GetDetector("M2Telescope");
  MG = (TMugastPhysics*) m_DetectorManager -> GetDetector("Mugast");
  CATS = (TCATSPhysics*) m_DetectorManager->GetDetector("CATSDetector");

  // get reaction information
  reaction.ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());
  OriginalBeamEnergy = reaction.GetBeamEnergy();
  // target thickness
  TargetThickness = m_DetectorManager->GetTargetThickness();
  string TargetMaterial = m_DetectorManager->GetTargetMaterial();
  // Cryo target case
  WindowsThickness = 0;//m_DetectorManager->GetWindowsThickness(); 
  string WindowsMaterial = "";//m_DetectorManager->GetWindowsMaterial();

  // energy losses
  string light=NPL::ChangeNameToG4Standard(reaction.GetNucleus3()->GetName());
  string beam=NPL::ChangeNameToG4Standard(reaction.GetNucleus1()->GetName());
  LightTarget = NPL::EnergyLoss(light+"_"+TargetMaterial+".G4table","G4Table",100 );
  LightAl = NPL::EnergyLoss(light+"_Al.G4table","G4Table",100);
  LightSi = NPL::EnergyLoss(light+"_Si.G4table","G4Table",100);
  BeamCD2 = NPL::EnergyLoss(beam+"_"+TargetMaterial+".G4table","G4Table",100);

  if(WindowsThickness){
    BeamWindow= new NPL::EnergyLoss(beam+"_"+WindowsMaterial+".G4table","G4Table",100); 
    LightWindow=  new NPL::EnergyLoss(light+"_"+WindowsMaterial+".G4table","G4Table",100);  
  }

  else{
    BeamWindow= NULL;
    LightWindow=NULL;
  }

  // initialize various parameters
  Rand = TRandom3();
  DetectorNumber = 0;
  ThetaNormalTarget = 0;
  ThetaM2Surface = 0;
  ThetaMGSurface = 0;
  Si_E_M2 = 0;
  CsI_E_M2 = 0;
  Energy = 0;
  ThetaGDSurface = 0;
  X = 0;
  Y = 0;
  Z = 0;
  dE = 0;
  BeamDirection = TVector3(0,0,1);

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent() {
  // Reinitiate calculated variable
  ReInitValue();
  double XTarget = CATS->GetPositionOnTarget().X();
  double YTarget = CATS->GetPositionOnTarget().Y();
  TVector3 BeamDirection = CATS->GetBeamDirection();

  BeamImpact = TVector3(XTarget,YTarget,0); 
  // determine beam energy for a randomized interaction point in target
  // 1% FWHM randominastion (E/100)/2.35
  //reaction.SetBeamEnergy(Rand.Gaus(myInit->GetIncidentFinalKineticEnergy(),myInit->GetIncidentFinalKineticEnergy()/235));

  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// LOOP on MUST2  ////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  for(unsigned int countMust2 = 0 ; countMust2 < M2->Si_E.size() ; countMust2++){
    /************************************************/
    //Part 0 : Get the usefull Data
    // MUST2
    int TelescopeNumber = M2->TelescopeNumber[countMust2];

    /************************************************/
    // Part 1 : Impact Angle
    ThetaM2Surface = 0;
    ThetaNormalTarget = 0;
    TVector3 HitDirection = M2 -> GetPositionOfInteraction(countMust2) - BeamImpact ;
    ThetaLab = HitDirection.Angle( BeamDirection );

    X = M2 -> GetPositionOfInteraction(countMust2).X();
    Y = M2 -> GetPositionOfInteraction(countMust2).Y();
    Z = M2 -> GetPositionOfInteraction(countMust2).Z();

    ThetaM2Surface = HitDirection.Angle(- M2 -> GetTelescopeNormal(countMust2) );
    ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;

    /************************************************/

    /************************************************/
    // Part 2 : Impact Energy
    Energy = ELab = 0;
    Si_E_M2 = M2->Si_E[countMust2];
    CsI_E_M2= M2->CsI_E[countMust2];

    // if CsI
    if(CsI_E_M2>0 ){
      // The energy in CsI is calculate form dE/dx Table because
      Energy = CsI_E_M2;
      Energy = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaM2Surface);
      Energy+=Si_E_M2;
    }

    else
      Energy = Si_E_M2;

    // Evaluate energy using the thickness
    ELab = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaM2Surface);
    // Target Correction
    ELab   = LightTarget.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);

    if(LightWindow)
      ELab = LightWindow->EvaluateInitialEnergy( ELab ,WindowsThickness, ThetaNormalTarget);
    /************************************************/

    /************************************************/
    // Part 3 : Excitation Energy Calculation
    Ex = reaction.ReconstructRelativistic( ELab , ThetaLab );
    ThetaLab=ThetaLab/deg;

    /************************************************/

    /************************************************/
    // Part 4 : Theta CM Calculation
    ThetaCM  = reaction.EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    /************************************************/
  }//end loop MUST2

  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// LOOP on MUGAST ////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  for(unsigned int countMugast = 0 ; countMugast < MG->DSSD_E.size() ; countMugast++){

    // Part 1 : Impact Angle
    ThetaMGSurface = 0;
    ThetaNormalTarget = 0;
    TVector3 HitDirection = MG -> GetPositionOfInteraction(countMugast) - BeamImpact ;
    ThetaLab = HitDirection.Angle( BeamDirection );

    X =  MG -> GetPositionOfInteraction(countMugast).X();
    Y =  MG -> GetPositionOfInteraction(countMugast).Y();
    Z =  MG -> GetPositionOfInteraction(countMugast).Z();

    ThetaMGSurface = HitDirection.Angle( TVector3(0,0,1) ) ;
    ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;


    // Part 2 : Impact Energy
    Energy = ELab = 0;
    Energy = MG->GetEnergyDeposit(countMugast);
    // Target Correction
    ELab   = LightTarget.EvaluateInitialEnergy( Energy ,TargetThickness*0.5, ThetaNormalTarget);

    if(LightWindow)
      ELab = LightWindow->EvaluateInitialEnergy( ELab ,WindowsThickness, ThetaNormalTarget);

    // Part 3 : Excitation Energy Calculation
    Ex = reaction.ReconstructRelativistic( ELab , ThetaLab );


    // Part 4 : Theta CM Calculation
    ThetaCM  = reaction.EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    ThetaLab=ThetaLab/deg;

  }//end loop Mugast
  
  ////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////// LOOP on AGATA ////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  if(nbTrack==1){ // keep only multiplicity one event
    TLorentzVector GammaLV;
    // Measured E
    double Egamma=trackE[0];
    // Gamma detection position
    TVector3 GammaHit(trackX1[0],trackY1[0],trackZ1[0]); 
    // Gamma Direction 
    TVector3 GammaDirection = GammaHit-BeamImpact;
    // Beta from Two body kinematic
    TVector3 beta = reaction.GetEnergyImpulsionLab_4().BoostVector();
    // Construct LV in lab frame
    GammaLV.SetPx(Egamma*GammaDirection.X());
    GammaLV.SetPy(Egamma*GammaDirection.Y());
    GammaLV.SetPz(Egamma*GammaDirection.Z());
    GammaLV.SetE(Egamma);
    // Boost back in CM
    GammaLV.Boost(-beta);
    // Get EDC
    EDC=GammaLV.Energy();
    }

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
  RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("EDC",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
  RootOutput::getInstance()->GetTree()->Branch("Run",&Run,"Run/I");
  RootOutput::getInstance()->GetTree()->Branch("X",&X,"X/D");
  RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D");
  RootOutput::getInstance()->GetTree()->Branch("Z",&Z,"Z/D");
  RootOutput::getInstance()->GetTree()->Branch("dE",&dE,"dE/D");
  // Vamos 
  RootOutput::getInstance()->GetTree()->Branch("LTS",&LTS,"LTS/l");

  // Agata
  // Time stamp of the agata trigger
  RootOutput::getInstance()->GetTree()->Branch("TStrack",&TStrack,"TStrack/l");

  // Array of reconstructed tracks
  RootOutput::getInstance()->GetTree()->Branch("nbTrack",&nbTrack,"nbTrack/I");
  RootOutput::getInstance()->GetTree()->Branch("trackE",trackE,"trackE[nbTrack]/F");
  RootOutput::getInstance()->GetTree()->Branch("trackX1",trackX1,"trackX1[nbTrack]/F");
  RootOutput::getInstance()->GetTree()->Branch("trackY1",trackY1,"trackY1[nbTrack]/F");
  RootOutput::getInstance()->GetTree()->Branch("trackZ1",trackZ1,"trackZ1[nbTrack]/F");
  RootOutput::getInstance()->GetTree()->Branch("trackT",trackT,"trackT[nbTrack]/F");
  RootOutput::getInstance()->GetTree()->Branch("trackCrystalID",trackCrystalID,"trackCrystalID[nbTrack]/I");

  // Array of reconstructed core
  RootOutput::getInstance()->GetTree()->Branch("nbCores",&nbCores,"nbCores/I");
  RootOutput::getInstance()->GetTree()->Branch("coreId",coreId,"coreId[nbCores]/I");
  RootOutput::getInstance()->GetTree()->Branch("coreTS",coreTS,"coreTS[nbCores]/l");
  RootOutput::getInstance()->GetTree()->Branch("coreE0",coreE0,"coreE0[nbCores]/F");
  //
}


////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  // RootInput:: getInstance()->GetChain()->SetBranchAddress("GATCONF",&vGATCONF);
  // Vamos
  RootInput::getInstance()->GetChain()->SetBranchAddress("LTS",&LTS);
  // Agata
  RootInput::getInstance()->GetChain()->SetBranchAddress("TStrack",&TStrack);
  RootInput::getInstance()->GetChain()->SetBranchAddress("nbTrack",&nbTrack);
  RootInput::getInstance()->GetChain()->SetBranchAddress("trackE",trackE);
  RootInput::getInstance()->GetChain()->SetBranchAddress("trackX1",trackX1);
  RootInput::getInstance()->GetChain()->SetBranchAddress("trackY1",trackY1);
  RootInput::getInstance()->GetChain()->SetBranchAddress("trackZ1",trackZ1);
  RootInput::getInstance()->GetChain()->SetBranchAddress("trackT",trackT);
  RootInput::getInstance()->GetChain()->SetBranchAddress("trackCrystalID",trackCrystalID);
  RootInput::getInstance()->GetChain()->SetBranchAddress("nbCores",&nbCores);
  RootInput::getInstance()->GetChain()->SetBranchAddress("coreId",coreId);
  RootInput::getInstance()->GetChain()->SetBranchAddress("coreTS",coreTS);
  RootInput::getInstance()->GetChain()->SetBranchAddress("coreE0",coreE0);
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::ReInitValue(){
  Ex = -1000 ;
  EDC= -1000;
  ELab = -1000;
  ThetaLab = -1000;
  ThetaCM = -1000;
  X = -1000;
  Y = -1000;
  Z = -1000;
  dE= -1000;
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the AnalysisFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VAnalysis* Analysis::Construct(){
  return (NPL::VAnalysis*) new Analysis();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_analysis{
  public:
    proxy_analysis(){
      NPL::AnalysisFactory::getInstance()->SetConstructor(Analysis::Construct);
    }
};

proxy_analysis p_analysis;
}

