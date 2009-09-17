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
 *  This event Generator is used to simulated Radioactive Ion beam           *
 *  Emmitance, Energy spread and dispersion are taken into account           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// G4 header
#include "G4ParticleTable.hh"

// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// NPTool header
#include "EventGeneratorBeam.hh"
#include "RootOutput.h"

using namespace CLHEP;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorBeam::EventGeneratorBeam()
{
   m_InitConditions = new TInitialConditions();
   m_Target         = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorBeam::~EventGeneratorBeam()
{
   delete m_InitConditions;
   delete m_Target;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorBeam::ReadConfiguration(string Path)
{
   ////////General Reading needs////////
   string LineBuffer;
   string DataBuffer;

   ////////Reaction Setting needs///////
   G4double particleZ = 0 , particleA = 0 ;
   //////////////////////////////////////////////////////////////////////////////////////////
   ifstream ReactionFile;
   ReactionFile.open(Path.c_str());

	bool ReadingStatus 			= false ;
	bool check_Z 				= false ;
	bool check_A 				= false ;
	bool check_Energy 			= false ;
	bool check_EnergySpread 	= false ;
	bool check_FWHMX 			= false ;
	bool check_FWHMY 			= false ;
	bool check_SigmaThetaX	 	= false ;
	bool check_SigmaPhiY  	 	= false ;
	
   if (ReactionFile.is_open()) {} else {
      return;
   }

   while (!ReactionFile.eof()) {
      //Pick-up next line
      getline(ReactionFile, LineBuffer);



     if (LineBuffer.compare(0, 4, "Beam") == 0) {
         G4cout << "Beam Found" << G4endl ;
         ReadingStatus = true ;
         }


	while(ReadingStatus){
	
			 ReactionFile >> DataBuffer;
	
      		//Search for comment Symbol %
      		if (DataBuffer.compare(0, 1, "%") == 0) {/*Do Nothing*/;}

	        else if (DataBuffer.compare(0, 10, "ParticleZ=") == 0) {
	         	check_Z = true ;
	            ReactionFile >> DataBuffer;
	            particleZ = atof(DataBuffer.c_str());
	         }

	        
	         else if (DataBuffer.compare(0, 10, "ParticleA=") == 0) {
	         	check_A = true ;
	            ReactionFile >> DataBuffer;
	            particleA = atof(DataBuffer.c_str());
	            G4cout << "Beam Particle: Z:" << particleZ << "  A:" << particleA << G4endl;
	            m_particle = G4ParticleTable::GetParticleTable()->GetIon(particleZ, particleA, 0.);
	         }

	         else if (DataBuffer.compare(0, 11, "BeamEnergy=") == 0) {
	         	check_Energy = true ;
	            ReactionFile >> DataBuffer;
	            m_BeamEnergy = atof(DataBuffer.c_str()) * MeV;
	            G4cout << "Beam Energy: " << m_BeamEnergy / MeV << " MeV" << G4endl;
	         }

	         else if (DataBuffer.compare(0, 17, "BeamEnergySpread=") == 0) {
	         	check_EnergySpread = true ;
	            ReactionFile >> DataBuffer;
	            m_BeamEnergySpread = atof(DataBuffer.c_str()) * MeV;
	            G4cout << "Beam Energy Spread: " << m_BeamEnergySpread / MeV << " MeV" << G4endl;
	         }

	         else if (DataBuffer.compare(0, 7, "SigmaX=") == 0) {
	         	check_FWHMX = true ;
	            ReactionFile >> DataBuffer;
	            m_SigmaX = atof(DataBuffer.c_str()) * mm;
	            G4cout << "Sigma X: " << m_SigmaX / mm << " mm" << G4endl;
	         }

	         else if (DataBuffer.compare(0, 7, "SigmaY=") == 0) {
	            check_FWHMY = true ;
	            ReactionFile >> DataBuffer;
	            m_SigmaY = atof(DataBuffer.c_str()) * mm;
	            G4cout << "Sigma Y: " << m_SigmaY / mm << " mm" << G4endl;
	         }

	         else if (DataBuffer.compare(0, 12, "SigmaThetaX=") == 0) {
	            check_SigmaThetaX = true ;
	            ReactionFile >> DataBuffer;
	            m_SigmaThetaX = atof(DataBuffer.c_str()) * deg;
	            G4cout << "Sigma Theta X: " << m_SigmaThetaX / deg << " deg" << G4endl;
	         }
	         
	         else if (DataBuffer.compare(0, 10, "SigmaPhiY=") == 0) {
	         	check_SigmaPhiY = true ;
	            ReactionFile >> DataBuffer;
	            m_SigmaPhiY = atof(DataBuffer.c_str()) * deg;
	            G4cout << "Sigma Phi Y: " << m_SigmaPhiY / deg << " deg" << G4endl;
	         }
	          
         	///////////////////////////////////////////////////
			//	If no Beam Token and no comment, toggle out
	         else 
	         	{ReadingStatus = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
	         	
	         ///////////////////////////////////////////////////
			//	If all Token found toggle out
	         if( check_Z && check_A && check_Energy && check_EnergySpread && check_FWHMX && check_FWHMY && check_SigmaThetaX && check_SigmaPhiY )
	         	ReadingStatus = false ;	
     	}
   }
   
   if( !check_Z || !check_A || !check_Energy || !check_EnergySpread || !check_FWHMX || !check_FWHMY || !check_SigmaThetaX || !check_SigmaPhiY )	
   		{cout << "WARNING : Token Sequence Incomplete, Beam definition could not be Fonctionnal" << endl ;}
   		
  cout << "///////////////////////////////////////////////////" << endl << endl ;
   
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorBeam::GenerateEvent(G4Event* anEvent, G4ParticleGun* particleGun)
{
   m_InitConditions->Clear();
   
   ///////////////////////////////////////////////////////////////////////
   ///// Calculate the incident beam direction as well as the vertex /////
   ///// of interaction in target                                    /////
   ///////////////////////////////////////////////////////////////////////
   G4ThreeVector InterCoord;
   G4double Beam_thetaX = 0, Beam_phiY = 0;
   G4double Beam_theta  = 0, Beam_phi  = 0;
   CalculateBeamInteraction(0, m_SigmaX, 0, m_SigmaThetaX,
                            0, m_SigmaY, 0, m_SigmaPhiY,
                            m_Target,
                            InterCoord, Beam_thetaX, Beam_phiY,
                            Beam_theta, Beam_phi);

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

   // Store initial value
   m_InitConditions->SetICIncidentAngleTheta(Beam_theta / deg);
   m_InitConditions->SetICIncidentAnglePhi(Beam_phi / deg);

   //////////////////////////////////////////////////
   /////Now define everything for light particle/////
   //////////////////////////////////////////////////

   particleGun->SetParticleDefinition(m_particle);

   G4double particle_energy = RandGauss::shoot(m_BeamEnergy, m_BeamEnergySpread);
   // Direction of particle, energy and laboratory angle
   G4double momentum_x = sin(Beam_theta) * cos(Beam_phi)       	;
   G4double momentum_y = sin(Beam_theta) * sin(Beam_phi)      	;
   G4double momentum_z = cos(Beam_theta)             		;
   //Set the gun to shoot
   particleGun->SetParticleMomentumDirection(G4ThreeVector(momentum_x, momentum_y, momentum_z))   	;
   particleGun->SetParticleEnergy(particle_energy)                                  				;
   particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0))                           			;

   //Shoot the light particle
   particleGun->GeneratePrimaryVertex(anEvent)        ;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorBeam::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("InitialConditions", "TInitialConditions", &m_InitConditions);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
