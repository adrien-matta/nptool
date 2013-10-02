/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated two body TransfertReaction.    *
 *  A Phase Space calculation is then performed to decay the Heavy product.  *
 *  The TGenPhaseSpace from ROOT is used to calculate a phase space decay    *
 *  with flat distribution	                                             *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   									     *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <iostream>
#include <fstream>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4RotationMatrix.hh"

// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// NPTool headers
#include "EventGeneratorTransfertToResonance.hh"
#include "RootOutput.h"

//Root Headers
#include "TGenPhaseSpace.h"

using namespace std;
using namespace CLHEP;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfertToResonance::EventGeneratorTransfertToResonance()
{
   //------------- Default Constructor -------------
	m_InitConditions	= new TInitialConditions()	;
	m_Reaction = new Reaction() ;
	m_Target = 0;
	m_SigmaX       		=  0 ;
	m_SigmaY       		=  0 ;
	m_SigmaThetaX     =  0 ;
	m_SigmaPhiY 		=  0 ;
	m_ResonanceDecayZ 		=  0 ;
	m_ResonanceDecayA 		=  0 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfertToResonance::~EventGeneratorTransfertToResonance()
{
   //------------- Default Destructor ------------
	delete m_InitConditions;
	delete m_Reaction ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfertToResonance::EventGeneratorTransfertToResonance(	  string  	name1          		,
																	      string   	name2          		,
																	      string   	name3          		,
																	      string   	name4          		,
																	      double   	BeamEnergy        	,
																	      double   	ExcitationEnergy    ,
																	      double   	BeamEnergySpread  	,
																	      double   	SigmaX         	,
																	      double   	SigmaY         	,
																	      double   	SigmaThetaX  ,
																	      double   	SigmaPhiY    ,
																	      double    ResonanceWidth,
																	      int      	ResonanceDecayZ     ,
																	      int      	ResonanceDecayA     ,
																	      bool  	ShootLight        	,
																	      bool  	ShootHeavy        	,
																	      bool  	ShootDecayProduct   ,
																	      string   	Path				)
{
   //------------- Constructor with nuclei names and beam energy ------------

   	SetEverything(  name1          		,        //Beam nuclei
		            name2          		,        //Target nuclei
		            name3          		,        //Product of reaction
		            name4          		,        //Product of reaction
		            BeamEnergy        	,        //Beam Energy
		            ExcitationEnergy 	,        //Excitation of Heavy Nuclei
		            BeamEnergySpread  	,
		            SigmaX         	,
		            SigmaY         	,
		            SigmaThetaX  ,
		            SigmaPhiY    ,
		            ResonanceWidth ,
		            ResonanceDecayZ     ,
		            ResonanceDecayA     ,
		           	ShootLight        	,
		           	ShootHeavy        	,
		           	ShootDecayProduct   ,
		            Path				);        

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfertToResonance::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("InitialConditions", "TInitialConditions", &m_InitConditions);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfertToResonance::Print() const
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//    Inherit from VEventGenerator

void EventGeneratorTransfertToResonance::ReadConfiguration(string Path)
{
////////General Reading needs////////
   string LineBuffer;
   string DataBuffer;

////////Reaction Setting needs///////
   string Beam, Target, Heavy, Light, CrossSectionPath ;
   G4double BeamEnergy = 0 , ExcitationEnergy = 0 , BeamEnergySpread = 0 , SigmaX = 0 , SigmaY = 0 , SigmaThetaX = 0 , SigmaPhiY=0,  ResonanceWidth = 0 ,ResonanceDecayZ = 0 , ResonanceDecayA = 0  ;
   bool  ShootLight     = false ;
   bool  ShootHeavy      = false ;
   bool ShootDecayProduct = false ;
   
   bool ReadingStatus = false ;
   bool check_Beam = false ;
   bool check_Target = false ;
   bool check_Light = false ;
   bool check_Heavy = false ;
   bool check_ExcitationEnergy = false ;
   bool check_BeamEnergy = false ;
   bool check_BeamEnergySpread = false ;
   bool check_FWHMX = false ;
   bool check_FWHMY = false ;
   bool check_EmmitanceTheta = false ;
   bool check_EmmitancePhi = false ;
   bool check_CrossSectionPath = false ;
   bool check_ShootLight = false ;
   bool check_ShootHeavy = false ;
   bool check_ResonanceWidth = false ;
   bool check_ResonanceDecayZ = false ;
   bool check_ResonanceDecayA = false ;
   bool check_ShootDecayProduct = false ;
   
//////////////////////////////////////////////////////////////////////////////////////////
   ifstream ReactionFile;
   ReactionFile.open(Path.c_str());

   if (ReactionFile.is_open()) {} else {
      return;
   }

   while (!ReactionFile.eof()) {
      //Pick-up next line
      getline(ReactionFile, LineBuffer);

      

      if (LineBuffer.compare(0, 9, "Transfert") == 0) { ReadingStatus = true ;}


while(ReadingStatus){
 			
 			 ReactionFile >> DataBuffer;
 			 
 			 //Search for comment Symbol %
	      	 if (DataBuffer.compare(0, 1, "%") == 0) {/* Do Nothing */;}
 			 
	         else if (DataBuffer.compare(0, 5, "Beam=") == 0) {
	         	check_Beam = true ;
	            ReactionFile >> DataBuffer;
	            Beam = DataBuffer;
	            G4cout << "Beam " << Beam << G4endl;
	         }
	
	         else if (DataBuffer.compare(0, 7, "Target=") == 0) {
	            check_Target = true ;
	            ReactionFile >> DataBuffer;
	            Target = DataBuffer;
	            G4cout << "Target " << Target << G4endl;
	         }

	         else if (DataBuffer.compare(0, 6, "Light=") == 0) {
	         	check_Light = true ;
	            ReactionFile >> DataBuffer;
	            Light = DataBuffer;
	            G4cout << "Light " << Light << G4endl;
	         }

	        else if  (DataBuffer.compare(0, 6, "Heavy=") == 0) {
	            check_Heavy = true ;
	            ReactionFile >> DataBuffer;
	            Heavy = DataBuffer;
	            G4cout << "Heavy " << Heavy << G4endl;
	         }

	        else if  (DataBuffer.compare(0, 17, "ExcitationEnergy=") == 0) {
	        	check_ExcitationEnergy = true ;
	            ReactionFile >> DataBuffer;
	            ExcitationEnergy = atof(DataBuffer.c_str()) * MeV;
	            G4cout << "Excitation Energy " << ExcitationEnergy / MeV << " MeV" << G4endl;
	         }

	        else if  (DataBuffer.compare(0, 11, "BeamEnergy=") == 0) {
	        	check_BeamEnergy = true ;
	            ReactionFile >> DataBuffer;
	            BeamEnergy = atof(DataBuffer.c_str()) * MeV;
	            G4cout << "Beam Energy " << BeamEnergy / MeV << " MeV" << G4endl;
	         }

	        else if  (DataBuffer.compare(0, 17, "BeamEnergySpread=") == 0) {
	        	check_BeamEnergySpread = true ;
	            ReactionFile >> DataBuffer;
	            BeamEnergySpread = atof(DataBuffer.c_str()) * MeV;
	            G4cout << "Beam Energy Spread " << BeamEnergySpread / MeV << " MeV" << G4endl;
	         }

	        else if  (DataBuffer.compare(0, 7, "SigmaX=") == 0) {
	        	check_FWHMX = true ;
	            ReactionFile >> DataBuffer;
	            SigmaX = atof(DataBuffer.c_str()) * mm;
	            G4cout << "Beam FWHM X " << SigmaX << " mm" << G4endl;
	         }

	        else if  (DataBuffer.compare(0, 7, "SigmaY=") == 0) {
	        	check_FWHMY = true ;
	            ReactionFile >> DataBuffer;
	            SigmaY = atof(DataBuffer.c_str()) * mm;
	            G4cout << "Beam FWHM Y " << SigmaX << " mm" << G4endl;
	         }

	        else if  (DataBuffer.compare(0, 19, "SigmaThetaX=") == 0) {
	        	check_EmmitanceTheta = true ;
	            ReactionFile >> DataBuffer;
	            SigmaThetaX = atof(DataBuffer.c_str()) * deg;
	            G4cout << "Beam Emmitance Theta " << SigmaThetaX / deg << " deg" << G4endl;
	         }
	         
	        else if  (DataBuffer.compare(0, 17, "SigmaPhiY=") == 0) {
	        	check_EmmitancePhi = true ;
	            ReactionFile >> DataBuffer;
	            SigmaPhiY = atof(DataBuffer.c_str()) * deg;
	            G4cout << "Beam Emmitance Phi " << SigmaPhiY / deg << " deg" << G4endl;
	         }

			else if (DataBuffer.compare(0, 15, "ResonanceWidth=") == 0) {
				check_ResonanceWidth = true ;
            	ReactionFile >> DataBuffer;
            	ResonanceWidth = atof(DataBuffer.c_str())*MeV;
            	G4cout << "Resonance Width " << ResonanceWidth/MeV << " MeV" << G4endl;
         	 }

			else if (DataBuffer.compare(0, 17, "ResonanceDecayZ=") == 0) {
				check_ResonanceDecayZ = true ;
            	ReactionFile >> DataBuffer;
            	ResonanceDecayZ = atof(DataBuffer.c_str());
            	G4cout << "ResonanceDecayZ " << ResonanceDecayZ << G4endl;
         	 }

         	else if (DataBuffer.compare(0, 17, "ResonanceDecayA=") == 0) {
         		check_ResonanceDecayA = true ;
            	ReactionFile >> DataBuffer;
            	ResonanceDecayA = atof(DataBuffer.c_str());
            	G4cout << "ResonanceDecayA " << ResonanceDecayA << G4endl;
         	}

	        else if  (DataBuffer.compare(0, 17, "CrossSectionPath=") == 0) {
	        	check_CrossSectionPath = true ;
	            ReactionFile >> CrossSectionPath;
	            G4cout << "Cross Section File: " << CrossSectionPath << G4endl ;
	         }

	        else if  (DataBuffer.compare(0, 11, "ShootLight=") == 0) {
	        	check_ShootLight = true ;
	            ReactionFile >> DataBuffer;
	            if (atof(DataBuffer.c_str()) == 1) ShootLight = true ;
	            if (ShootLight)    G4cout << "Shoot Light particle      : yes" << G4endl;
	            else           G4cout << "Shoot Light particle      : no"  << G4endl;
	         }

	        else if  (DataBuffer.compare(0, 11, "ShootHeavy=") == 0) {
	        	check_ShootHeavy = true ;
	            ReactionFile >> DataBuffer;
	            if (atof(DataBuffer.c_str()) == 1) ShootHeavy = true ;
	            if (ShootHeavy)    G4cout << "Shoot Heavy particle      : yes" << G4endl;
	            else           G4cout << "Shoot Heavy particle      : no"  << G4endl;
	         }
	         
        	else if (DataBuffer.compare(0, 18, "ShootDecayProduct=") == 0) {
        		check_ShootDecayProduct = true ;
	            ReactionFile >> DataBuffer;
	            if (atof(DataBuffer.c_str()) == 1) ShootDecayProduct = true  ;
	            if (ShootDecayProduct)  G4cout << "Shoot DecayProducts from decay : yes" << G4endl;
	            else           G4cout << "Shoot DecayProducts from decay : no"  << G4endl;
	         }

			  
         	///////////////////////////////////////////////////
			//	If no Transfert Token and no comment, toggle out
	         else 
	         	{ReadingStatus = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
	         	
	         ///////////////////////////////////////////////////
			//	If all Token found toggle out
	         if(   	check_Beam && check_Target && check_Light && check_Heavy && check_ExcitationEnergy 
	         	&&  check_BeamEnergy && check_BeamEnergySpread && check_FWHMX && check_FWHMY && check_EmmitanceTheta 
	         	&&  check_EmmitancePhi && check_CrossSectionPath && check_ShootLight && check_ShootHeavy 
	         	&& check_ResonanceWidth && check_ResonanceDecayZ && check_ResonanceDecayA && check_ShootDecayProduct)
	         	ReadingStatus = false ;	

		}
	        

	}
   
   SetEverything(	Beam            	,
			         Target            	,
			         Light          	,
			         Heavy          	,
			         BeamEnergy       	,
			         ExcitationEnergy  	,
			         BeamEnergySpread  	,
			         SigmaX         	,
			         SigmaY         	,
			         SigmaThetaX ,
			         SigmaPhiY 	,
			         ResonanceWidth,
			         ResonanceDecayZ    ,
			         ResonanceDecayA    ,
			         ShootLight        	,
			         ShootHeavy        	,
			         ShootDecayProduct  ,
			         CrossSectionPath	);

   		ReactionFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfertToResonance::GenerateEvent(G4Event* anEvent , G4ParticleGun* particleGun)
{
   // Clear contents of Precedent event (now stored in ROOTOutput)
   m_InitConditions->Clear();

   //////////////////////////////////////////////////
   //////Define the kind of particle to shoot////////
   //////////////////////////////////////////////////
   G4int LightZ = m_Reaction->GetNucleus3()->GetZ() ;
   G4int LightA = m_Reaction->GetNucleus3()->GetA() ;

   G4ParticleDefinition* LightName
   = G4ParticleTable::GetParticleTable()->GetIon(LightZ, LightA, 0.);

   G4int HeavyZ = m_Reaction->GetNucleus4()->GetZ() ;
   G4int HeavyA = m_Reaction->GetNucleus4()->GetA() ;

   G4ParticleDefinition* HeavyName
   = G4ParticleTable::GetParticleTable()->GetIon(HeavyZ, HeavyA, m_Reaction->GetExcitation()*MeV);

   // Shoot the Resonance energy following the mean and width value
   double EXX = -10 ;
   
   while(EXX<0)
   EXX = RandGauss::shoot(m_ResonanceMean,m_ResonanceWidth) ;

	m_Reaction->SetExcitation( EXX  );

   ///////////////////////////////////////////////////////////////////////
   ///// Calculate the incident beam direction as well as the vertex /////
   ///// of interaction in target                                    /////
   ///////////////////////////////////////////////////////////////////////
   G4ThreeVector InterCoord;
   G4double Beam_thetaX = 0, Beam_phiY = 0;
   G4double Beam_theta  = 0, Beam_phi  = 0;
   
   	if(m_SigmaX==0 && m_SigmaY==0 && m_SigmaThetaX == 0 && m_SigmaPhiY==0) InterCoord = G4ThreeVector(0,0,0);
   
	else if( m_Target !=0)   
  	 CalculateBeamInteraction(	0, m_SigmaX, 0, m_SigmaThetaX,
                           	 	0, m_SigmaY, 0, m_SigmaPhiY,
                            	m_Target,
                            	InterCoord, Beam_thetaX, Beam_phiY,
                            	Beam_theta, Beam_phi);
                            	
	else 
		InterCoord = G4ThreeVector(0,0,0);

   // write vertex position to ROOT file
   G4double x0 = InterCoord.x();
   G4double y0 = InterCoord.y();
   G4double z0 = InterCoord.z();
   m_InitConditions->SetICPositionX(x0);
   m_InitConditions->SetICPositionY(y0);
   m_InitConditions->SetICPositionZ(z0);

   // write emittance angles to ROOT file
   m_InitConditions->SetICIncidentEmittanceTheta(Beam_thetaX / deg);
   m_InitConditions->SetICIncidentEmittancePhi(Beam_phiY / deg);

   // write angles to ROOT file
   m_InitConditions->SetICIncidentAngleTheta(Beam_theta / deg);
   m_InitConditions->SetICIncidentAnglePhi(Beam_phi / deg);

   //////////////////////////////////////////////////////////
   ///// Build rotation matrix to go from the incident //////
   ///// beam frame to the "world" frame               //////
   //////////////////////////////////////////////////////////
   G4ThreeVector col1(cos(Beam_theta) * cos(Beam_phi),
                      cos(Beam_theta) * sin(Beam_phi),
                      -sin(Beam_theta));
   G4ThreeVector col2(-sin(Beam_phi),
                      cos(Beam_phi),
                      0);
   G4ThreeVector col3(sin(Beam_theta) * cos(Beam_phi),
                      sin(Beam_theta) * sin(Beam_phi),
                      cos(Beam_theta));
   G4RotationMatrix BeamToWorld(col1, col2, col3);

   /////////////////////////////////////////////////////////////////
   ///// Angles for emitted particles following Cross Section //////
   ///// Angles are in the beam frame                         //////
   /////////////////////////////////////////////////////////////////
   // Beam incident energy
   G4double NominalBeamEnergy = m_BeamEnergy;
   G4double IncidentBeamEnergy = RandGauss::shoot(NominalBeamEnergy, m_BeamEnergySpread);
   m_Reaction->SetBeamEnergy(IncidentBeamEnergy);
   m_InitConditions->SetICIncidentEnergy(IncidentBeamEnergy / MeV);
   // Angles
   RandGeneral CrossSectionShoot(m_Reaction->GetCrossSection(), m_Reaction->GetCrossSectionSize());
   G4double ThetaCM = CrossSectionShoot.shoot() * (180*deg);
   G4double phi     = RandFlat::shoot() * 2*pi;
   // write angles to ROOT file
   m_InitConditions->SetICEmittedAngleThetaCM(ThetaCM / deg);
   m_InitConditions->SetICEmittedAnglePhiIncidentFrame(phi / deg);

   //////////////////////////////////////////////////
   /////  Momentum and angles from  kinematics  /////
   /////  Angles are in the beam frame          /////
   //////////////////////////////////////////////////
   // Variable where to store results
   G4double ThetaLight, EnergyLight, ThetaHeavy, EnergyHeavy;
   // Set the Theta angle of reaction
   m_Reaction->SetThetaCM(ThetaCM);
   // Compute Kinematic using previously defined ThetaCM
   m_Reaction->KineRelativistic(ThetaLight, EnergyLight, ThetaHeavy, EnergyHeavy);
   // Momentum in beam frame for light particle
   G4ThreeVector momentum_kineLight_beam(sin(ThetaLight) * cos(phi),
                                         sin(ThetaLight) * sin(phi),
                                         cos(ThetaLight));
   // Momentum in beam frame for heavy particle
   G4ThreeVector momentum_kineHeavy_beam(sin(ThetaHeavy) * cos(phi),
                                         sin(ThetaHeavy) * sin(phi),
                                         cos(ThetaHeavy));



   // Move to the target
   x0 += m_Target->GetTargetX() ;
   y0 += m_Target->GetTargetY() ;
   z0 += m_Target->GetTargetZ() ;

   // write vertex position to ROOT file
   m_InitConditions->SetICPositionX(x0);
   m_InitConditions->SetICPositionY(y0);
   m_InitConditions->SetICPositionZ(z0);

   //////////////////////////////////////////////////
   ///////// Set up everything for shooting /////////
   //////////////////////////////////////////////////
   if (m_ShootLight) {   // Case of light particle
      // Particle type
      particleGun->SetParticleDefinition(LightName);
      // Particle energy
      particleGun->SetParticleEnergy(EnergyLight);
      // Particle vertex position
      particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
      // Particle direction
      // Kinematical angles in the beam frame are transformed 
      // to the "world" frame
      G4ThreeVector momentum_kine_world = BeamToWorld * momentum_kineLight_beam;
      // get theta and phi in the world frame
      G4double theta_world = momentum_kine_world.theta();
      G4double phi_world   = momentum_kine_world.phi();
      if (phi_world < 1e-6) phi_world += 2*pi;
      // write angles in ROOT file
      m_InitConditions->SetICEmittedAngleThetaLabWorldFrame(theta_world / deg);
      m_InitConditions->SetICEmittedAnglePhiWorldFrame(phi_world / deg);
         // write angles/energy to ROOT file
   		m_InitConditions->SetICEmittedAngleThetaLabIncidentFrame(ThetaLight / deg);
   		m_InitConditions->SetICEmittedEnergy(EnergyLight/MeV);
      //Set the gun to shoot
      particleGun->SetParticleMomentumDirection(momentum_kine_world);
      //Shoot the light particle
      particleGun->GeneratePrimaryVertex(anEvent);
   }
   
   // Case of recoil particle
   /*   // Particle type
      particleGun->SetParticleDefinition(HeavyName);
      // Particle energy
      particleGun->SetParticleEnergy(EnergyHeavy);
      // Particle vertex position
      particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
      // Particle direction
      // Kinematical angles in the beam frame are transformed 
      // to the "world" frame*/
      G4ThreeVector momentum_kine_world = BeamToWorld * momentum_kineHeavy_beam;
      // get theta and phi in the world frame
      G4double theta_world = momentum_kine_world.theta();
      G4double phi_world   = momentum_kine_world.phi();
      if (phi_world < 1e-6) phi_world += 2*pi;
      
      EventGeneratorTransfertToResonance::ResonanceDecay(	EnergyHeavy    	,
      														theta_world     ,
      														phi_world      	,
      														x0            	,
      														y0            	,
      														z0             	,
      														anEvent        	,
      														particleGun		);
      
   
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfertToResonance::ResonanceDecay(  G4double EnergyHeavy    		,
													      G4double ThetaHeavy     		,
													      G4double PhiHeavy       		,
													      G4double x0             		,
													      G4double y0             		,
													      G4double z0             		,
													      G4Event* anEvent        		,
													      G4ParticleGun* particleGun	)
{
   G4double parentZ = m_Reaction->GetNucleus4()->GetZ() ;
   G4double parentA = m_Reaction->GetNucleus4()->GetA() ;

   G4int NumberOfNeutrons = (parentA - parentZ) - (m_ResonanceDecayA - m_ResonanceDecayZ)  	;
   G4int NumberOfProtons  = parentZ - m_ResonanceDecayZ                  				  	;
   G4int NumberOfDecayProducts = NumberOfNeutrons + NumberOfProtons         			  	;

   if (NumberOfNeutrons < 0 || NumberOfProtons < 0) {
      G4cout << "Error input for Resonance decay" << G4endl;
      return;
   }
   
   else {
		//Obtain Mass of daughter Nuclei
		G4ParticleDefinition* parent
		= G4ParticleTable::GetParticleTable()->GetIon(parentZ, parentA, m_Reaction->GetExcitation())     ;

		G4ParticleDefinition* daughter
		= G4ParticleTable::GetParticleTable()->GetIon(m_ResonanceDecayZ, m_ResonanceDecayA, 0.) ;

		G4ParticleDefinition* neutron
		=  G4ParticleTable::GetParticleTable()->FindParticle("neutron");

		G4ParticleDefinition* proton
		=  G4ParticleTable::GetParticleTable()->FindParticle("proton");

		G4double M  = parent   -> GetPDGMass()     ;
		G4double md = daughter -> GetPDGMass()     ;
		G4double mn = neutron  -> GetPDGMass()     ;
		G4double mp = proton   -> GetPDGMass()     ;
		
		G4double InitialE      	  = EnergyHeavy + M   			;
		G4double InitialMomentumX = sqrt( InitialE*InitialE - M*M) * sin(ThetaHeavy) * cos(PhiHeavy) 		;
		G4double InitialMomentumY = sqrt( InitialE*InitialE - M*M) * sin(ThetaHeavy) * sin(PhiHeavy) 		;
		G4double InitialMomentumZ = sqrt( InitialE*InitialE - M*M) * cos(ThetaHeavy)                 		;

		TLorentzVector Initial = TLorentzVector(InitialMomentumX/GeV, InitialMomentumY/GeV, InitialMomentumZ/GeV,InitialE/GeV);

		// Array of masses express in GeV/c2
		double* masses = new double[NumberOfDecayProducts+1];

		//	Filling Array
		masses[0] = md/GeV ;
	
		int ll = 1 ;
		for(int i = 0 ; i < NumberOfNeutrons ; i++)
				{masses[ll] = mn/GeV ; ll++;}
				
		for(int i = 0 ; i < NumberOfProtons ; i++)
				{masses[ll] = mp/GeV ; ll++;}
				
		// Instentiate a Phase Space Generator, with flat distrution
		TGenPhaseSpace TPhaseSpace ;

		if( !TPhaseSpace.SetDecay(Initial, NumberOfDecayProducts+1, masses) ) cout << "Warning: Phase Space Decay forbiden by kinematic, or more than 18 particles "<<endl;
		double MaxWt=TPhaseSpace.GetWtMax() ;
		double Weight = 0 	;
		double Rand   = 1	; 

		while( Rand > Weight )
			{  
				Weight = TPhaseSpace.Generate() 		;
				Rand = CLHEP::RandFlat::shoot()*MaxWt	; 
			}
		
		
		TLorentzVector* daugterLV ;
		double Energy;
		G4ThreeVector Momentum;
		 if (m_ShootHeavy) 
		 	{
		 		daugterLV = TPhaseSpace.GetDecay(0);
		 	
		 		Momentum = G4ThreeVector(	daugterLV->X()*GeV	,
											daugterLV->Y()*GeV	,
											daugterLV->Z()*GeV 	);				
		
				Energy   = daugterLV->E()*GeV-md ;
				Momentum.unit() ;		 	
		 	
				//Set the gun to shoot
				particleGun->SetParticleDefinition(daughter)                    ;
				particleGun->SetParticleMomentumDirection(Momentum)     		;
				particleGun->SetParticleEnergy(Energy)                          ;
				particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0))     ;
				// Shoot the Daugter
				particleGun->GeneratePrimaryVertex(anEvent) ;
				
			   // get theta and phi in the world frame
		       G4double theta_world = Momentum.theta();
		       G4double phi_world   = Momentum.phi();
		       if (phi_world < 1e-6) phi_world += 2*pi;
		       // write angles in ROOT file
		       m_InitConditions->SetICEmittedAngleThetaLabWorldFrame(theta_world / deg);
		       m_InitConditions->SetICEmittedAnglePhiWorldFrame(phi_world / deg);		
			   m_InitConditions->SetICEmittedEnergy(Energy);
			}
		
		if (m_ShootDecayProduct) 
			{
				G4int jj = 1   ;
				for ( int u = 0; u < NumberOfNeutrons ; u++) 
					{
						daugterLV = TPhaseSpace.GetDecay(jj);

						Momentum = G4ThreeVector(	daugterLV->X()*GeV	,
													daugterLV->Y()*GeV	,
													daugterLV->Z()*GeV 	);
						Energy   = daugterLV->E()*GeV-mn ;
						Momentum.unit() ;
						//Set the gun to shoot
						particleGun->SetParticleDefinition(neutron)                    	;
						particleGun->SetParticleMomentumDirection(Momentum)     		;
						particleGun->SetParticleEnergy(Energy)                          ;
						particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0))     ;
						// Shoot the Daugter
						particleGun->GeneratePrimaryVertex(anEvent) ;
						
						// get theta and phi in the world frame
						G4double theta_world = Momentum.theta();
						G4double phi_world   = Momentum.phi();
						if (phi_world < 1e-6) phi_world += 2*pi;
						// write angles in ROOT file
						m_InitConditions->SetICEmittedAngleThetaLabWorldFrame(theta_world / deg);
						m_InitConditions->SetICEmittedAnglePhiWorldFrame(phi_world / deg);		
						m_InitConditions->SetICEmittedEnergy(Energy);
						
						jj++;
					}
				
				for ( int u = 0; u < NumberOfProtons ; u++) 
					{
						daugterLV = TPhaseSpace.GetDecay(jj);

						Momentum = G4ThreeVector(	daugterLV->X()*GeV	,
													daugterLV->Y()*GeV	,
													daugterLV->Z()*GeV 	);
						Energy   = daugterLV->E()*GeV-mp ;
						Momentum.unit() ;
						//Set the gun to shoot
						particleGun->SetParticleDefinition(proton)                      ;
						particleGun->SetParticleMomentumDirection(Momentum)     		;
						particleGun->SetParticleEnergy(Energy)                          ;
						particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0))     ;
						// Shoot the Daugter
						particleGun->GeneratePrimaryVertex(anEvent) ;
						
					 	// get theta and phi in the world frame
						G4double theta_world = Momentum.theta();
						G4double phi_world   = Momentum.phi();
						if (phi_world < 1e-6) phi_world += 2*pi;
						// write angles in ROOT file
						m_InitConditions->SetICEmittedAngleThetaLabWorldFrame(theta_world / deg);
						m_InitConditions->SetICEmittedAnglePhiWorldFrame(phi_world / deg);		
						m_InitConditions->SetICEmittedEnergy(Energy);
						jj++;
					}
				
			
		}
		
		delete masses 	;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfertToResonance::SetEverything(	  string    name1          			,
													      string   name2          			,
													      string   name3          			,
													      string   name4          			,
													      double   BeamEnergy        		,
													      double   Excitation        		,
													      double   BeamEnergySpread  		,
													      double   SigmaX         			,
													      double   SigmaY         			,
													      double   SigmaThetaX      		,
													      double   SigmaPhiY       			,
													      double    ResonanceWidth          ,
													      int      ResonanceDecayZ      	,
													      int      ResonanceDecayA      	,
													      bool  ShootLight        			,
													      bool  ShootHeavy        			,
													      bool  ShootDecayProduct      		,
													      string   Path						)
{
   //------------- Constructor with nuclei names and beam energy ------------

	m_Reaction = new Reaction (	name1 ,
								name2 ,
								name3 ,
								name4 ,
								BeamEnergy,
								Excitation,
								Path) ;
   m_BeamEnergy         = BeamEnergy;
   m_BeamEnergySpread   =  BeamEnergySpread  ;
   m_SigmaX       =  SigmaX         	;
   m_SigmaY       =  SigmaY         	;
   m_SigmaThetaX    =  SigmaThetaX    	 ;
   m_SigmaPhiY    =  SigmaPhiY    	 ;
   m_ResonanceWidth	=	ResonanceWidth	;
   m_ResonanceMean	= Excitation		;
   m_ResonanceDecayZ =  ResonanceDecayZ      ;
   m_ResonanceDecayA =  ResonanceDecayA      ;
   m_ShootLight      =  ShootLight       	 ;
   m_ShootHeavy      =  ShootHeavy        	;
   m_ShootDecayProduct    =  ShootDecayProduct      	;

}

