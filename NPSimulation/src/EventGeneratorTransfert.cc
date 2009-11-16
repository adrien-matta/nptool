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
 *  A Relativistic computation is performed to determine angle and energy of *
 *   the different particle, knowing the ThetaCM angle given by a cross      *
 *   section shoot. Eleastic scattering can also be simulated.               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <iostream>
#include <fstream>
#include <limits>

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
#include "EventGeneratorTransfert.hh"
#include "RootOutput.h"

using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfert::EventGeneratorTransfert()
{
   //------------- Default Constructor -------------
   m_InitConditions	= new TInitialConditions();
   m_Reaction 		= new Reaction();
   m_Target		= 0;

   m_BeamEnergy		= 0;
   m_BeamEnergySpread	= 0;
   m_SigmaX		= 0;
   m_SigmaY		= 0;
   m_SigmaThetaX	= 0;
   m_SigmaPhiY		= 0;
   m_ShootLight		= 0;
   m_ShootHeavy		= 0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void	EventGeneratorTransfert::SetTarget(Target* Target) 
   {
   	if(Target!=0)	
   		{
   			m_Target = Target;
   			G4int LightZ = m_Reaction->GetNucleus3()->GetZ() ;
  			G4int LightA = m_Reaction->GetNucleus3()->GetA() ;
   			m_Target->WriteDEDXTable(G4ParticleTable::GetParticleTable()->GetIon(LightZ,LightA, 0.) ,0, m_BeamEnergy);
   		}
   
   }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfert::~EventGeneratorTransfert()
{
   //------------- Default Destructor ------------
   delete m_InitConditions;
   delete m_Reaction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfert::EventGeneratorTransfert(string name1             ,        // Beam nuclei
		             string name2             ,        // Target nuclei
		             string name3             ,        // Product of reaction
		             string name4             ,        // Product of reaction
		             double BeamEnergy        ,        // Beam Energy
		             double ExcitationEnergy  ,        // Excitation of Heavy Nuclei
		             double BeamEnergySpread  ,
		             double SigmaX         ,
		             double SigmaY         ,
		             double SigmaThetaX       ,
		             double SigmaPhiY  ,
		             bool   ShootLight        ,
		             bool   ShootHeavy        ,
		             string Path)                     // Path of the differentiel Cross Section
{
 SetEverything(		name1             ,        // Beam nuclei
		              name2             ,        // Target nuclei
		              name3             ,        // Product of reaction
		              name4             ,        // Product of reaction
		              BeamEnergy        ,        // Beam Energy
		              ExcitationEnergy  ,        // Excitation of Heavy Nuclei
		              BeamEnergySpread  ,
		              SigmaX         ,
		              SigmaY         ,
		              SigmaThetaX       ,
		              SigmaPhiY  ,
		               ShootLight        ,
		                ShootHeavy        ,
		             Path);        

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfert::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("InitialConditions", "TInitialConditions", &m_InitConditions);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfert::Print() const
{

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//    Inherit from VEventGenerator

void EventGeneratorTransfert::ReadConfiguration(string Path)
{
////////General Reading needs////////
   string LineBuffer;
   string DataBuffer;

////////Reaction Setting needs///////
   string Beam, Target, Heavy, Light, CrossSectionPath ;
   G4double BeamEnergy = 0 , ExcitationEnergy = 0 , BeamEnergySpread = 0 , SigmaX = 0 , SigmaY = 0 , SigmaThetaX = 0 , SigmaPhiY=0;
   bool  ShootLight     = false ;
   bool  ShootHeavy      = false ;
   
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
	      	 if (DataBuffer.compare(0, 1, "%") == 0) {	ReactionFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
 			 
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

	        else if  (DataBuffer.compare(0, 12, "SigmaThetaX=") == 0) {
	        	check_EmmitanceTheta = true ;
	            ReactionFile >> DataBuffer;
	            SigmaThetaX = atof(DataBuffer.c_str()) * deg;
	            G4cout << "Beam Emmitance Theta " << SigmaThetaX / deg << " deg" << G4endl;
	         }
	         
	        else if  (DataBuffer.compare(0, 10, "SigmaPhiY=") == 0) {
	        	check_EmmitancePhi = true ;
	            ReactionFile >> DataBuffer;
	            SigmaPhiY = atof(DataBuffer.c_str()) * deg;
	            G4cout << "Beam Emmitance Phi " << SigmaPhiY / deg << " deg" << G4endl;
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

			  
         	///////////////////////////////////////////////////
			//	If no Transfert Token and no comment, toggle out
	         else 
	         	{ReadingStatus = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
	         	
	         ///////////////////////////////////////////////////
			//	If all Token found toggle out
	         if(   	check_Beam && check_Target && check_Light && check_Heavy && check_ExcitationEnergy 
	         	&&  check_BeamEnergy && check_BeamEnergySpread && check_FWHMX && check_FWHMY && check_EmmitanceTheta 
	         	&&  check_EmmitancePhi && check_CrossSectionPath && check_ShootLight && check_ShootHeavy)
	         	ReadingStatus = false ;	

		}
	        

	}
   
   SetEverything(Beam            ,
         Target            ,
         Light          ,
         Heavy          ,
         BeamEnergy        ,
         ExcitationEnergy  ,
         BeamEnergySpread  ,
         SigmaX         ,
         SigmaY         ,
         SigmaThetaX       ,
         SigmaPhiY	,
         ShootLight        ,
         ShootHeavy        ,
         CrossSectionPath);
         
   		ReactionFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfert::GenerateEvent(G4Event* anEvent , G4ParticleGun* particleGun)
{	
   // Clear contents of Precedent event (now stored in ROOTOutput)
   m_InitConditions->Clear();

   //////////////////////////////////////////////////
   //////Define the kind of particle to shoot////////
   //////////////////////////////////////////////////
   // Light
   G4int LightZ = m_Reaction->GetNucleus3()->GetZ() ;
   G4int LightA = m_Reaction->GetNucleus3()->GetA() ;

   G4ParticleDefinition* LightName
   = G4ParticleTable::GetParticleTable()->GetIon(LightZ, LightA, 0.);

   // Recoil
   G4int HeavyZ = m_Reaction->GetNucleus4()->GetZ() ;
   G4int HeavyA = m_Reaction->GetNucleus4()->GetA() ;

   G4ParticleDefinition* HeavyName
   = G4ParticleTable::GetParticleTable()->GetIon(HeavyZ, HeavyA, m_Reaction->GetExcitation()*MeV);

   // Beam
   G4int BeamZ = m_Reaction->GetNucleus1()->GetZ();
   G4int BeamA = m_Reaction->GetNucleus1()->GetA();
   G4ParticleDefinition* BeamName = G4ParticleTable::GetParticleTable()->GetIon(BeamZ, BeamA, 0);

   ///////////////////////////////////////////////////////////////////////
   ///// Calculate the incident beam direction as well as the vertex /////
   ///// of interaction in target and Energy Loss of the beam within /////
   ///// the target.                                                 /////
   ///////////////////////////////////////////////////////////////////////
   G4ThreeVector InterCoord;
   
   G4double Beam_thetaX = 0, Beam_phiY = 0;
   G4double Beam_theta  = 0, Beam_phi  = 0;
   G4double FinalBeamEnergy = 0 ;
   G4double InitialBeamEnergy = RandGauss::shoot(m_BeamEnergy, m_BeamEnergySpread);
   
	m_Target->CalculateBeamInteraction(	0, m_SigmaX, 0, m_SigmaThetaX,
                            					0, m_SigmaY, 0, m_SigmaPhiY,
				                            	InitialBeamEnergy,
				                            	BeamName,
				                           	 	InterCoord, Beam_thetaX, Beam_phiY,
                            					Beam_theta, Beam_phi,
				                           	 	FinalBeamEnergy);
				                           	 	
	m_Reaction->SetBeamEnergy(FinalBeamEnergy);
  m_InitConditions->SetICIncidentEnergy(FinalBeamEnergy / MeV);
  
   // write vertex position to ROOT file
   G4double x0 = InterCoord.x();
   G4double y0 = InterCoord.y();
   G4double z0 = InterCoord.z();
   m_InitConditions->SetICPositionX(x0);//
   m_InitConditions->SetICPositionY(y0);//
   m_InitConditions->SetICPositionZ(z0);//

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

   // write angles/energy to ROOT file
   m_InitConditions->SetICEmittedAngleThetaLabIncidentFrame(ThetaLight / deg);
   m_InitConditions->SetICEmittedEnergy(EnergyLight/MeV);

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
      
      //Set the gun to shoot
      particleGun->SetParticleMomentumDirection(momentum_kine_world);
      //Shoot the light particle
      particleGun->GeneratePrimaryVertex(anEvent);
   }
   if (m_ShootHeavy) { // Case of heavy particle
      // Particle type
      particleGun->SetParticleDefinition(HeavyName);
      // Particle energy
      particleGun->SetParticleEnergy(EnergyHeavy);
      // Particle vertex position
      particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
      // Particle direction
      // Kinematical angles in the beam frame are transformed 
      // to the "world" frame
      G4ThreeVector momentum_kine_world = BeamToWorld * momentum_kineHeavy_beam;
      // get theta and phi in the world frame
      G4double theta_world = momentum_kine_world.theta();
      G4double phi_world   = momentum_kine_world.phi();
      if (phi_world < 1e-6) phi_world += 2*pi;
      // write angles in ROOT file
      m_InitConditions->SetICEmittedAngleThetaLabWorldFrame(theta_world / deg);
      m_InitConditions->SetICEmittedAnglePhiWorldFrame(phi_world / deg);
      //Set the gun to shoot
      particleGun->SetParticleMomentumDirection(momentum_kine_world);
      //Shoot the light particle
      particleGun->GeneratePrimaryVertex(anEvent);
   }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfert::SetEverything(string name1,		// Beam nuclei
                                            string name2,		// Target nuclei
                                            string name3,		// Product of reaction
                                            string name4,		// Product of reaction
                                            double BeamEnergy,		// Beam Energy
                                            double ExcitationEnergy,	// Excitation of Heavy Nuclei
                                            double BeamEnergySpread,
                                            double SigmaX,
                                            double SigmaY,
                                            double SigmaThetaX,
                                            double SigmaPhiY,
                                            bool   ShootLight,
                                            bool   ShootHeavy,
                                            string Path) 
{
   m_Reaction = new Reaction(name1, name2, name3, name4, BeamEnergy, ExcitationEnergy, Path);	
		
   m_BeamEnergy       =  BeamEnergy;
   m_BeamEnergySpread =  BeamEnergySpread;
   m_SigmaX           =  SigmaX;
   m_SigmaY           =  SigmaY;
   m_SigmaThetaX      =  SigmaThetaX;
   m_SigmaPhiY        =  SigmaPhiY;
   m_ShootLight       =  ShootLight;
   m_ShootHeavy       =  ShootHeavy;
}
 
