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
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorBeam::~EventGeneratorBeam()
{}
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
	bool check_EmmitanceTheta 	= false ;
	bool check_EmmitancePhi 	= false ;
	
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

	         else if (DataBuffer.compare(0, 10, "BeamFWHMX=") == 0) {
	         	check_FWHMX = true ;
	            ReactionFile >> DataBuffer;
	            m_BeamFWHMX = atof(DataBuffer.c_str()) * mm;
	            G4cout << "Beam FWHM X: " << m_BeamFWHMX / mm << " mm" << G4endl;
	         }

	         else if (DataBuffer.compare(0, 10, "BeamFWHMY=") == 0) {
	            check_FWHMY = true ;
	            ReactionFile >> DataBuffer;
	            m_BeamFWHMY = atof(DataBuffer.c_str()) * mm;
	            G4cout << "Beam FWHM Y: " << m_BeamFWHMY / mm << " mm" << G4endl;
	         }

	         else if (DataBuffer.compare(0, 15, "EmmitanceTheta=") == 0) {
	            check_EmmitanceTheta = true ;
	            ReactionFile >> DataBuffer;
	            m_BeamEmmitanceTheta = atof(DataBuffer.c_str()) * rad;
	            G4cout << "Beam Emmitance Theta: " << m_BeamEmmitanceTheta / deg << " deg" << G4endl;
	         }
	         
	         else if (DataBuffer.compare(0, 13, "EmmitancePhi=") == 0) {
	         	check_EmmitancePhi = true ;
	            ReactionFile >> DataBuffer;
	            m_BeamEmmitancePhi = atof(DataBuffer.c_str()) * rad;
	            G4cout << "Beam Emmitance Phi: " << m_BeamEmmitancePhi / deg << " deg" << G4endl;
	         }
	          
         	///////////////////////////////////////////////////
			//	If no Beam Token and no comment, toggle out
	         else 
	         	{ReadingStatus = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
	         	
	         ///////////////////////////////////////////////////
			//	If all Token found toggle out
	         if( check_Z && check_A && check_Energy && check_EnergySpread && check_FWHMX && check_FWHMY && check_EmmitanceTheta && check_EmmitancePhi )
	         	ReadingStatus = false ;	
     	}
   }
   
   if( !check_Z || !check_A || !check_Energy || !check_EnergySpread || !check_FWHMX || !check_FWHMY || !check_EmmitanceTheta || !check_EmmitancePhi )	
   		{cout << "WARNING : Token Sequence Incomplete, Beam definition could not be Fonctionnal" << endl ;}
   		
  cout << "///////////////////////////////////////////////////" << endl << endl ;
   
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorBeam::GenerateEvent(G4Event* anEvent, G4ParticleGun* particleGun)
{

   // Vertex position and beam angle
   G4double x0 = 1000 * cm  ;
   G4double y0 = 1000 * cm  ;
   G4double Beam_thetaX = 0  ;
   G4double Beam_phiY   = 0  ;
   
   //shoot inside the target with correlated angle
   if (m_TargetRadius != 0) {
      while (sqrt(x0*x0 + y0*y0) > m_TargetRadius) 
      	{
      		RandomGaussian2D(0,0,m_BeamFWHMX / 2.35,m_BeamEmmitanceTheta,x0,Beam_thetaX);
      		RandomGaussian2D(0,0,m_BeamFWHMY / 2.35,m_BeamEmmitancePhi  ,y0,Beam_phiY  );
      	}
   }

   else 
   	{
     	RandomGaussian2D(0,0,0,m_BeamEmmitanceTheta,x0,Beam_thetaX);
     	RandomGaussian2D(0,0,0,m_BeamEmmitancePhi  ,y0,Beam_phiY  );
   }

	// Calculate Angle in spherical coordinate, passing by the direction vector dir	
	G4double Xdir =  cos( pi/2. - Beam_thetaX ) 							;
	G4double Ydir =  cos( pi/2. - Beam_phiY   )								;
	G4double Zdir =  sin( pi/2. - Beam_thetaX ) + sin(  pi/2. - Beam_phiY) 	;
	
	G4double Beam_theta = acos ( Zdir / sqrt( Xdir*Xdir + Ydir*Ydir + Zdir*Zdir ) );
	G4double Beam_phi   = atan2( Ydir , Xdir );

   //must shoot inside the target.
   G4double z0 = (-m_TargetThickness / 2 + CLHEP::RandFlat::shoot() * m_TargetThickness);


   // Move to the target
   x0 += m_TargetX ;
   y0 += m_TargetY ;
   z0 += m_TargetZ ;

   // Store initial value
   m_InitialBeamX       =  x0       	;
   m_InitialBeamY   	=  y0           ;
   m_InitialBeamTheta   =  Beam_theta   ;

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
   pTree->Branch("InitialBeamX"     , &m_InitialBeamX    ,  "InitialBeamX/D");
   pTree->Branch("InitialBeamY"     , &m_InitialBeamY    ,  "InitialBeamX/D");
   pTree->Branch("InitialBeamTheta" , &m_InitialBeamTheta   ,  "InitialBeamTheta/D");
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
