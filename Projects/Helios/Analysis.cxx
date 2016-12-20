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

#include"NPPhysicalConstants.h"


////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){
   
  myInit= new TInitialConditions();
  myIntCoord= new TInteractionCoordinates();
  
  Hel = (Helios*)  m_DetectorManager -> GetDetector("Helios");

  Bfield=Hel->GetNominalMField();
  cout << "Nominal Magnetic field= " <<  Bfield  << " Tesla" << endl;

  LightCD2 = EnergyLoss("proton_CD2.G4table","G4Table",100 );
  //LightAl = EnergyLoss("proton_Al.G4table","G4Table",100);
  //LightSi = EnergyLoss("proton_Si.G4table","G4Table",100);
  //BeamCD2 = EnergyLoss("Sn132_CD2.G4table","G4Table",100);
  //BeamCD2 = EnergyLoss("Mg30_CD2.G4table","G4Table",100);
  BeamCD2 = EnergyLoss("Mg28_CD2.G4table","G4Table",100);
  //BeamCD2 = EnergyLoss("Hg206_CD2.G4table","G4Table",100);

  myReaction = new NPL::Reaction();
  myReaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());

  //TargetThickness = 0.189*micrometer;
   TargetThickness = m_DetectorManager->GetTargetThickness();  // * by micrometer to convert in mm  
   cout << "Target Thickness = " <<  TargetThickness << " mm" << endl; 


   OriginalBeamEnergy = myReaction->GetBeamEnergy();
   cout << "Beam Energy before target= " <<  OriginalBeamEnergy << " MeV" << endl; 


  pCharge= myReaction->GetNucleus3().GetZ();
//  pCharge=1;

  //double pAtomWeight= myReaction->GetNucleus3()->GetA();
  //double pMassExcess= myReaction->GetNucleus3()->GetMassExcess();
  //pMass= (pAtomWeight*amu_c2 + pMassExcess/1000)/amu_c2;
  //cout << "Mass of light particle= " <<  pMass << endl; 
  pMass= (myReaction->GetNucleus3().Mass())/amu_c2;
//    pMass=1.00782503207;  // MeV/c2 - for 132Sn(d,p)
//  pMass=2.0141017778;  // MeV/c2 - for 224Ra(d,d)
//  pMass=12*931.49;  // MeV/c2 - for 224Ra(C,C)


  cout << "Charge of light particle= " <<  pCharge << endl;
  cout << "Mass of light particle= " <<  pMass << " amu" << endl; 
  cout << "Reaction QValue= " <<  myReaction->GetQValue() << " MeV" <<endl; 


   Rand = new TRandom3();
   DetectorNumber = 0 ;
   ThetaNormalTarget = 0 ;

   ThetaHelSurface = 0;
   X_Hel = 0 ;
   Y_Hel = 0 ;
   Z_Hel = 0 ;
   Si_E_Hel = 0 ;
   E_Hel = 0;
   Si_X_Hel = 0;
   Si_Y_Hel = 0;
   
  InitOutputBranch();
  InitInputBranch();

   
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
  // Reinitiate calculated variable
  ReInitValue();
  double XTarget = -1001;
  double YTarget = -1001;

  TVector3 BeamDirection = TVector3(0,0,1);

  double BeamEnergy = BeamCD2.Slow(OriginalBeamEnergy,TargetThickness/2,0);
  myReaction->SetBeamEnergy(BeamEnergy); // set the new beam energy taking into account the beam energy loss in target befor reaction
 
   double Theta_extrap=0. ;
   double ThetaDet =0. ; 
   //double ExcitationE=0. ; double ExcitationE_extrap=0. ; 


   //TVector3 Pos; // for helisol but not for helios

 
   XTarget=myInit->GetIncidentPositionX();
   YTarget=myInit->GetIncidentPositionY();


  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////// LOOP on Helios //////////////////
  if( Hel->GetEnergyDeposit() >0){
 
    /************************************************/
    // Part 1 : Interaction position in detector

    if(XTarget>-1000 && YTarget>-1000){ // XTarget and YTarget can be known experimentaly only if there are beam tracker detectors !!!

      //Pos = Hel -> GetPositionOfInteraction(); // Lab Position of the light particle interaction in the Si Detector for helisol but not for helios
      Z_Hel = myIntCoord->GetDetectedPositionZ(0);

    }
    
    else{
      BeamDirection = TVector3(-1000,-1000,-1000);
    }
    
    /************************************************/
    
    /************************************************/
    // Part 2 : Impact Energy
    Energy = EDep = 0;
    Energy = Hel->GetEnergyDeposit();
     
	// Detector intrinsic energy resolution
    EDep   = Energy; // resolution already taken into account in simulation output for helios !!
    // EDep   = Rand->Gaus(Energy, 0.01/2.35); // 10 keV sigma resolution for helisol !!

    /************************************************/
    
    /************************************************/
    // Part 3 : Excitation energy calculation

    //Ex= EDep - 0.0138322*ZPos -11.5701;  // for 136Xe(d,p)
    Ex= -(EDep - 0.0116048*Z_Hel -7.21402);  // for 30Mg(d,p)31Mg-gs at 7.5 MeV/u and 2 Tesla 
    //Ex= -(EDep - 0.014929*Z_Hel -7.29597);  // for 30Mg(d,p)31Mg-gs at 7.5 MeV/u and 2.5 Tesla 
    //Ex= -(EDep - 0.014464*Z_Hel -8.4199);  // for 28Mg(d,p)29Mg-gs at 7.5 MeV/u and 2.5 Tesla 
    //Ex= -(EDep - 0.0176351*Z_Hel -11.0221);  // for 206Hg(d,p) at 10 MeV/u and 2.5 Tesla 



  }//end loop Helios

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
	
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
  RootOutput::getInstance()->GetTree()->Branch("EDep",&EDep,"EDep/D");
  RootOutput::getInstance()->GetTree()->Branch("Z_Hel",&Z_Hel,"Z_Hel/D");
  RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&myInit );
  RootInput:: getInstance()->GetChain()->SetBranchAddress("InteractionCoordinates",&myIntCoord );
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::ReInitValue(){
  ELab = -1000;
  EDep = -1000;
  Z_Hel= -1000; 
  Ex= -1000; 

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
class proxy{
  public:
    proxy(){
      NPL::AnalysisFactory::getInstance()->SetConstructor(Analysis::Construct);
    }
};

proxy p;
}

