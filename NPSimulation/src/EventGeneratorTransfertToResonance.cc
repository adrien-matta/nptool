// C++ headers
#include <iostream>
#include <fstream>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"

// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// NPTool headers
#include "EventGeneratorTransfertToResonance.hh"
#include "RootOutput.h"

using namespace std;
using namespace CLHEP;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfertToResonance::EventGeneratorTransfertToResonance()
{
   //------------- Default Constructor -------------

	m_Reaction = new Reaction() ;
   m_BeamFWHMX       		=  0 ;
   m_BeamFWHMY       		=  0 ;
   m_BeamEmmitanceTheta     =  0 ;
   m_BeamEmmitancePhi 		=  0 ;
   m_ResonanceDecayZ 		=  0 ;
   m_ResonanceDecayA 		=  0 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfertToResonance::~EventGeneratorTransfertToResonance()
{
   //------------- Default Destructor ------------

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
																	      double   	BeamFWHMX         	,
																	      double   	BeamFWHMY         	,
																	      double   	BeamEmmitanceTheta  ,
																	      double   	BeamEmmitancePhi    ,
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
		            BeamFWHMX         	,
		            BeamFWHMY         	,
		            BeamEmmitanceTheta  ,
		            BeamEmmitancePhi    ,
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
   pTree->Branch("ThetaCM"       , &m_InitialThetaCM         ,  "ThetaCM/D");
   pTree->Branch("InitialLightEnergy", &m_InitialLightEnergy   ,  "InitialLightEnergy/D");
   pTree->Branch("InitialLightTheta"   , &m_InitialLightTheta  ,  "InitialLightTheta/D");
   pTree->Branch("InitialBeamEnergy"   , &m_InitialBeamEnergy  ,  "InitialBeamEnergy/D");
   pTree->Branch("InitialBeamTheta" , &m_InitialBeamTheta   ,  "InitialBeamTheta/D");
   pTree->Branch("InitialBeamX"     , &m_InitialBeamX    ,  "InitialBeamX/D");
   pTree->Branch("InitialBeamY"     , &m_InitialBeamY    ,  "InitialBeamY/D");
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
   G4double BeamEnergy = 0 , ExcitationEnergy = 0 , BeamEnergySpread = 0 , BeamFWHMX = 0 , BeamFWHMY = 0 , BeamEmmitanceTheta = 0 , BeamEmmitancePhi=0, ResonanceDecayZ = 0 , ResonanceDecayA = 0  ;
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
	      	 if (LineBuffer.compare(0, 1, "%") == 0) {/* Do Nothing */;}
 			 
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

	        else if  (DataBuffer.compare(0, 11, "BeamFWHMX=") == 0) {
	        	check_FWHMX = true ;
	            ReactionFile >> DataBuffer;
	            BeamFWHMX = atof(DataBuffer.c_str()) * mm;
	            G4cout << "Beam FWHM X " << BeamFWHMX << " mm" << G4endl;
	         }

	        else if  (DataBuffer.compare(0, 11, "BeamFWHMY=") == 0) {
	        	check_FWHMY = true ;
	            ReactionFile >> DataBuffer;
	            BeamFWHMY = atof(DataBuffer.c_str()) * mm;
	            G4cout << "Beam FWHM Y " << BeamFWHMX << " mm" << G4endl;
	         }

	        else if  (DataBuffer.compare(0, 19, "BeamEmmitanceTheta=") == 0) {
	        	check_EmmitanceTheta = true ;
	            ReactionFile >> DataBuffer;
	            BeamEmmitanceTheta = atof(DataBuffer.c_str()) * rad;
	            G4cout << "Beam Emmitance Theta " << BeamEmmitanceTheta / deg << " deg" << G4endl;
	         }
	         
	        else if  (DataBuffer.compare(0, 17, "BeamEmmitancePhi=") == 0) {
	        	check_EmmitancePhi = true ;
	            ReactionFile >> DataBuffer;
	            BeamEmmitancePhi = atof(DataBuffer.c_str()) * rad;
	            G4cout << "Beam Emmitance Phi " << BeamEmmitancePhi / deg << " deg" << G4endl;
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
	         	&& check_ResonanceDecayZ && check_ResonanceDecayA && check_ShootDecayProduct)
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
			         BeamFWHMX         	,
			         BeamFWHMY         	,
			         BeamEmmitanceTheta ,
			         BeamEmmitancePhi 	,
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
   //////////////////////////////////////////////////
   //////Define the kind of particle to shoot////////
   //////////////////////////////////////////////////

   G4int LightZ = m_Reaction->GetNucleus3()->GetZ() ;
   G4int LightA = m_Reaction->GetNucleus3()->GetA() ;

   G4ParticleDefinition* LightName
   = G4ParticleTable::GetParticleTable()->GetIon(LightZ, LightA, 0.);

   G4int HeavyZ = m_Reaction->GetNucleus4()->GetZ() ;
   G4int HeavyA = m_Reaction->GetNucleus4()->GetA() ;

   //////////////////////////////////////////////////
   /////Choose ThetaCM following Cross Section //////
   //////////////////////////////////////////////////

   //Calling RandGeneral fonction, which taking a double array as argument
    CLHEP::RandGeneral CrossSectionShoot(m_Reaction->GetCrossSection(), m_Reaction->GetCrossSectionSize() )    ;
   G4double ThetaCM = CrossSectionShoot.shoot() * (180 * deg)                ;
   //G4double ThetaCM = 0;
   //Set the Theta angle of reaction
   m_InitialThetaCM = ThetaCM;

   //////////////////////////////////////////////////
   ////Momentum and direction set with kinematics////
   //////////////////////////////////////////////////

   //Variable where to store results.
   G4double ThetaLight, EnergyLight, ThetaHeavy, EnergyHeavy;
   //Compute Kinematic using previously defined ThetaCM
   m_Reaction->KineRelativistic(ThetaLight, EnergyLight, ThetaHeavy, EnergyHeavy);
   //to write thetaCM in the root file
   m_InitialLightTheta  = ThetaLight / deg  ;
   m_InitialLightEnergy = EnergyLight / MeV ;

   // Vertex position and beam angle
   // 11Li Beam@Riken
   G4double x0 = 1000 * cm  ;
   G4double y0 = 1000 * cm  ;
   //shoot inside the target.

   if (m_TargetRadius != 0) {
      while (sqrt(x0*x0 + y0*y0) > m_TargetRadius) {
         x0 = RandGauss::shoot(0, m_BeamFWHMX / 2.35) ;
         y0 = RandGauss::shoot(0, m_BeamFWHMY / 2.35) ;
      }
   }

   else {
      x0 = 0 ;
      y0 = 0 ;
   }
   
   // FIXME a changer pour prendre en compte correctement l'emmitance
   G4double Beam_theta = RandGauss::shoot(0, m_BeamEmmitanceTheta)      ;

   // must shoot inside the target.
   G4double z0 = (-m_TargetThickness / 2 + CLHEP::RandFlat::shoot() * m_TargetThickness) ;

   // Move to the target
   x0 += m_TargetX ;
   y0 += m_TargetY ;
   z0 += m_TargetZ ;

   // Store initial value
   m_InitialBeamX       =  x0          ;
   m_InitialBeamY    =  y0             ;
   m_InitialBeamTheta   =  Beam_theta     ;

   //////////////////////////////////////////////////
   /////Now define everything for light particle/////
   //////////////////////////////////////////////////
   particleGun->SetParticleDefinition(LightName);

   G4double theta = ThetaLight + Beam_theta        ;
   G4double phi = CLHEP::RandFlat::shoot() * 2 * pi   ;
   G4double particle_energy = EnergyLight          ;

   // Direction of particle, energy and laboratory angle
   G4double momentum_x = sin(theta) * cos(phi)       ;
   G4double momentum_y = sin(theta) * sin(phi)       ;
   G4double momentum_z = cos(theta)             ;

   //Set the gun to shoot
   particleGun->SetParticleMomentumDirection(G4ThreeVector(momentum_x, momentum_y, momentum_z))   ;
   particleGun->SetParticleEnergy(particle_energy)                                  ;
   particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0))                           ;

   //Shoot the light particle
   if (m_ShootLight) particleGun->GeneratePrimaryVertex(anEvent)                        ;

   //////////////////////////////////////////////////
   /////Now define everything for heavy particle/////
   //////////////////////////////////////////////////

   theta = ThetaHeavy + Beam_theta  ;
   particle_energy = EnergyHeavy ;

   if (m_ShootHeavy)  ResonanceDecay(	HeavyZ            ,
           								HeavyA            ,
            							EnergyHeavy       ,
            							theta             ,
            							phi               ,
            							x0                ,
							            y0                ,
							            z0                ,
							            anEvent           ,
							            particleGun		  );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfertToResonance::ResonanceDecay(G4int parentZ           ,
      G4int parentA           ,
      G4double EnergyHeavy    ,
      G4double ThetaHeavy        ,
      G4double PhiHeavy       ,
      G4double x0             ,
      G4double y0             ,
      G4double z0             ,
      G4Event* anEvent        ,
      G4ParticleGun* particleGun)
{
   //FIXME
   G4int NumberOfNeutrons = (parentA - parentZ) - (m_ResonanceDecayA - m_ResonanceDecayZ)  	;
   G4int NumberOfProtons  = parentZ - m_ResonanceDecayZ                  				  	;
   G4int NumberOfDecayProducts = NumberOfNeutrons + NumberOfProtons         			  	;

   /*G4int NumberOfNeutrons = 1  ;
   G4int NumberOfProtons  = 0 ;
   G4int NumberOfDecayProducts = NumberOfNeutrons + NumberOfProtons           ;*/

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

      // FIXME Hexaneutron
      /*  mn   = 6*(neutron-> GetPDGMass())-100*keV   ;
         G4double E = (M*M - mn*mn + md*md) / (2*M)   ;
          G4double p = sqrt(( M*M - (md + mn)*(md + mn) ) * ( M*M - (md - mn)*(md - mn) )) / (2*M) ;*/

      //   G4double cosThetaCM = CLHEP::RandFlat::shoot()*2-1 ;
      //     G4double ThetaCM = acos(cosThetaCM)              ;
      //   G4double PhiCM =  CLHEP::RandFlat::shoot() * 2 * pi   ;

      // FIXME Hexaneutron
      /*
          G4double DaughterMomentumX = sin(ThetaCM)*cos(PhiCM)       ;
         G4double DaughterMomentumY = sin(ThetaCM)*sin(PhiCM)        ;
         G4double DaughterMomentumZ = cos(ThetaCM)                ;*/

      G4double Q  =  m_Reaction->GetExcitation() + M - md - mn * NumberOfNeutrons - mp * NumberOfProtons    ;

      vector<G4double> DecayProductsMomentumCM  ;
      vector<G4double> DecayProductsMomentumXCM ;
      vector<G4double> DecayProductsMomentumYCM ;
      vector<G4double> DecayProductsMomentumZCM ;
      vector<G4double> DecayProductsThetaCM     ;
      vector<G4double> DecayProductsPhiCM       ;

      G4double AvaibleEnergy = Q          ;
      //Calculate Energy of Heavy
      G4double MomentumCMHeavy = CLHEP::RandFlat::shoot() * AvaibleEnergy ;
      AvaibleEnergy = AvaibleEnergy - MomentumCMHeavy                ;

      G4double MomentumCM     = 0            ;
      // Calculate DecayProducts Properties
      for (G4int i = 0 ; i < NumberOfDecayProducts ; i++) {
         if (i != NumberOfDecayProducts - 1)   MomentumCM = CLHEP::RandFlat::shoot() * AvaibleEnergy   ;
         else                    MomentumCM = AvaibleEnergy                   ;

         AvaibleEnergy = AvaibleEnergy - MomentumCM        ;
         DecayProductsMomentumCM.push_back(MomentumCM)      ;

         G4double cosThetaCM = CLHEP::RandFlat::shoot() * 2 - 1 ;
         G4double ThetaCM = acos(cosThetaCM)             ;
         DecayProductsThetaCM.push_back(ThetaCM)            ;

         G4double PhiCM =  CLHEP::RandFlat::shoot() * 2 * pi   ;
         DecayProductsPhiCM.push_back(PhiCM)             ;

         DecayProductsMomentumXCM.push_back(sin(ThetaCM)*cos(PhiCM))   ;
         DecayProductsMomentumYCM.push_back(sin(ThetaCM)*sin(PhiCM))   ;
         DecayProductsMomentumZCM.push_back(cos(ThetaCM))           ;
      }

      // Applied conservation of Momentum to daughter nuclei
      G4double DaughterMomentumX = 0   ;
      G4double DaughterMomentumY = 0   ;
      G4double DaughterMomentumZ = 0   ;

      for (G4int i = 0 ; i < NumberOfDecayProducts ; i++) {
         DaughterMomentumX = (DaughterMomentumX - DecayProductsMomentumCM[i] * DecayProductsMomentumXCM[i])  ;
         DaughterMomentumY = (DaughterMomentumY - DecayProductsMomentumCM[i] * DecayProductsMomentumYCM[i])  ;
         DaughterMomentumZ = (DaughterMomentumZ - DecayProductsMomentumCM[i] * DecayProductsMomentumZCM[i])  ;
      }

      // Daughter MomentumCM
      G4double p =
         sqrt(DaughterMomentumX * DaughterMomentumX + DaughterMomentumY * DaughterMomentumY + DaughterMomentumZ * DaughterMomentumZ) ;

      // Daughter EnergyCm
      G4double E = sqrt(p * p + md * md)            ;
      DaughterMomentumX = DaughterMomentumX / p ;
      DaughterMomentumY = DaughterMomentumY / p ;
      DaughterMomentumZ = DaughterMomentumZ / p ;

      // CM to Lab //

      // Initial Lab to CM Momentum
      G4double InitialE      = sqrt(EnergyHeavy * EnergyHeavy + M * M)      ;
      G4double InitialMomentum  = EnergyHeavy                        ;
      G4double InitialMomentumX = sin(ThetaHeavy) * cos(PhiHeavy)         ;
      G4double InitialMomentumY =   sin(ThetaHeavy) * sin(PhiHeavy)       ;
      G4double InitialMomentumZ =   cos(ThetaHeavy)                     ;

      // Beta and Gamma CM to Lab
      /*    G4double betaX = (DaughterMomentumX*p - InitialMomentumX*InitialMomentum)/E   ;
         G4double betaY = (DaughterMomentumY*p - InitialMomentumY*InitialMomentum)/E   ;
         G4double betaZ = (DaughterMomentumZ*p - InitialMomentumZ*InitialMomentum)/E   ;
         G4double beta  = sqrt (betaX*betaX + betaY*betaY  + betaZ*betaZ   )        ;
         G4double beta2 = beta*beta                                     ;
         G4double gamma = 1 / ( sqrt(1 - beta2 ) )                         ;*/

      G4double betaX = -(InitialMomentumX * InitialMomentum) / InitialE  ;
      G4double betaY = -(InitialMomentumY * InitialMomentum) / InitialE  ;
      G4double betaZ = -(InitialMomentumZ * InitialMomentum) / InitialE  ;
      G4double beta  = sqrt(betaX * betaX + betaY * betaY  + betaZ * betaZ)        ;
      G4double beta2 = beta * beta                                     ;
      G4double gamma = 1 / (sqrt(1 - beta2))                          ;
      // Calculate everything for heavy particule

      /*   G4double NewEnergy =
          gamma*E
        - betaX*gamma*DaughterMomentumX*p
        - betaY*gamma*DaughterMomentumY*p
        - betaZ*gamma*DaughterMomentumZ*p;*/

      G4double NewMomentumX =
         - betaX * gamma * E
         + DaughterMomentumX * p + (gamma - 1) * (betaX * betaX) / (beta2) * DaughterMomentumX * p
         + (gamma - 1) * (betaX * betaY) / (beta2) * DaughterMomentumY * p
         + (gamma - 1) * (betaX * betaZ) / (beta2) * DaughterMomentumZ * p                ;

      G4double NewMomentumY =
         - betaY * gamma * E
         + (gamma - 1) * (betaY * betaX) / (beta2) * DaughterMomentumX * p
         + DaughterMomentumY * p + (gamma - 1) * (betaY * betaY) / (beta2) * DaughterMomentumY * p
         + (gamma - 1) * (betaY * betaZ) / (beta2) * DaughterMomentumZ * p                ;

      G4double NewMomentumZ =
         - betaZ * gamma * E
         + (gamma - 1) * (betaZ * betaX) / (beta2) * DaughterMomentumX * p
         + (gamma - 1) * (betaZ * betaY) / (beta2) * DaughterMomentumY * p
         + DaughterMomentumZ * p + (gamma - 1) * (betaZ * betaZ) / (beta2) * DaughterMomentumZ * p   ;

      G4double
      NewEnergy =  sqrt(NewMomentumX * NewMomentumX + NewMomentumY * NewMomentumY + NewMomentumZ * NewMomentumZ) ;
      NewMomentumX = NewMomentumX / NewEnergy                                                   ;
      NewMomentumY = NewMomentumY / NewEnergy                                                   ;
      NewMomentumZ = NewMomentumZ / NewEnergy                                                   ;

      //Set the gun to shoot
      particleGun->SetParticleDefinition(daughter)                                           ;
      particleGun->SetParticleMomentumDirection(G4ThreeVector(NewMomentumX, NewMomentumY, NewMomentumZ))     ;
      particleGun->SetParticleEnergy(NewEnergy)                                                ;
      particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0))                                   ;

      // Shoot the Daugter
      particleGun->GeneratePrimaryVertex(anEvent)                                               ;

      if (m_ShootDecayProduct) {
         G4int jj = 0   ;
         for (; jj < NumberOfNeutrons ; jj++) {
            p = sqrt(DecayProductsMomentumXCM[jj] * DecayProductsMomentumXCM[jj]
                     + DecayProductsMomentumYCM[jj] * DecayProductsMomentumYCM[jj]
                     + DecayProductsMomentumZCM[jj] * DecayProductsMomentumZCM[jj]) ;

            E = sqrt(p * p + mn * mn)   ;

            NewMomentumX =
               - betaX * gamma * E
               + DecayProductsMomentumXCM[jj] * p + (gamma - 1) * (betaX * betaX) / (beta2) * DecayProductsMomentumXCM[jj] * p
               + (gamma - 1) * (betaX * betaY) / (beta2) * DecayProductsMomentumYCM[jj] * p
               + (gamma - 1) * (betaX * betaZ) / (beta2) * DecayProductsMomentumZCM[jj] * p                 ;

            NewMomentumY =
               - betaY * gamma * E
               + (gamma - 1) * (betaY * betaX) / (beta2) * DecayProductsMomentumXCM[jj] * p
               + DecayProductsMomentumYCM[jj] * p + (gamma - 1) * (betaY * betaY) / (beta2) * DecayProductsMomentumYCM[jj] * p
               + (gamma - 1) * (betaY * betaZ) / (beta2) * DecayProductsMomentumYCM[jj] * p                 ;

            NewMomentumZ =
               - betaZ * gamma * E
               + (gamma - 1) * (betaZ * betaX) / (beta2) * DecayProductsMomentumXCM[jj] * p
               + (gamma - 1) * (betaZ * betaY) / (beta2) * DecayProductsMomentumYCM[jj] * p
               + DecayProductsMomentumZCM[jj] * p + (gamma - 1) * (betaZ * betaZ) / (beta2) * DecayProductsMomentumZCM[jj] * p  ;

            NewEnergy =  sqrt(NewMomentumX * NewMomentumX + NewMomentumY * NewMomentumY + NewMomentumZ * NewMomentumZ)   ;
            NewMomentumX = NewMomentumX / NewEnergy                                                   ;
            NewMomentumY = NewMomentumY / NewEnergy                                                   ;
            NewMomentumZ = NewMomentumZ / NewEnergy                                                   ;
            //Set the gun to shoot
            particleGun->SetParticleDefinition(neutron)                                               ;
            particleGun->SetParticleMomentumDirection(G4ThreeVector(NewMomentumX, NewMomentumY, NewMomentumZ))     ;
            particleGun->SetParticleEnergy(NewEnergy)                                                ;
            particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0))                                   ;
            particleGun->GeneratePrimaryVertex(anEvent)                                   ;
         }

         for (; jj < NumberOfProtons ; jj++) {
            p = sqrt(DecayProductsMomentumXCM[jj] * DecayProductsMomentumXCM[jj]
                     + DecayProductsMomentumYCM[jj] * DecayProductsMomentumYCM[jj]
                     + DecayProductsMomentumZCM[jj] * DecayProductsMomentumZCM[jj]) ;

            E = sqrt(p * p + mp * mp)   ;

            NewMomentumX =
               - betaX * gamma * E
               + DecayProductsMomentumXCM[jj] * p + (gamma - 1) * (betaX * betaX) / (beta2) * DecayProductsMomentumXCM[jj] * p
               + (gamma - 1) * (betaX * betaY) / (beta2) * DecayProductsMomentumYCM[jj] * p
               + (gamma - 1) * (betaX * betaZ) / (beta2) * DecayProductsMomentumZCM[jj] * p                 ;

            NewMomentumY =
               - betaY * gamma * E
               + (gamma - 1) * (betaY * betaX) / (beta2) * DecayProductsMomentumXCM[jj] * p
               + DecayProductsMomentumYCM[jj] * p + (gamma - 1) * (betaY * betaY) / (beta2) * DecayProductsMomentumYCM[jj] * p
               + (gamma - 1) * (betaY * betaZ) / (beta2) * DecayProductsMomentumYCM[jj] * p                 ;

            NewMomentumZ =
               - betaZ * gamma * E
               + (gamma - 1) * (betaZ * betaX) / (beta2) * DecayProductsMomentumXCM[jj] * p
               + (gamma - 1) * (betaZ * betaY) / (beta2) * DecayProductsMomentumYCM[jj] * p
               + DecayProductsMomentumZCM[jj] * p + (gamma - 1) * (betaZ * betaZ) / (beta2) * DecayProductsMomentumZCM[jj] * p  ;

            NewEnergy =  sqrt(NewMomentumX * NewMomentumX + NewMomentumY * NewMomentumY + NewMomentumZ * NewMomentumZ)   ;
            NewMomentumX = NewMomentumX / NewEnergy                                                   ;
            NewMomentumY = NewMomentumY / NewEnergy                                                   ;
            NewMomentumZ = NewMomentumZ / NewEnergy                                                   ;
            //Set the gun to shoot
            particleGun->SetParticleDefinition(neutron)                                               ;
            particleGun->SetParticleMomentumDirection(G4ThreeVector(NewMomentumX, NewMomentumY, NewMomentumZ))     ;
            particleGun->SetParticleEnergy(NewEnergy)                                                ;
            particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0))                                   ;
            particleGun->GeneratePrimaryVertex(anEvent)                                   ;
         }
      }
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfertToResonance::SetEverything(string    name1          ,
      string   name2          ,
      string   name3          ,
      string   name4          ,
      double   BeamEnergy        ,
      double   Excitation        ,
      double   BeamEnergySpread  ,
      double   BeamFWHMX         ,
      double   BeamFWHMY         ,
      double   BeamEmmitanceTheta       ,
      double   BeamEmmitancePhi       ,
      int      ResonanceDecayZ      ,
      int      ResonanceDecayA      ,
      bool  ShootLight        ,
      bool  ShootHeavy        ,
      bool  ShootDecayProduct      ,
      string   Path)
{
   //------------- Constructor with nuclei names and beam energy ------------

	m_Reaction = new Reaction (	name1 ,
								name2 ,
								name3 ,
								name4 ,
								BeamEnergy,
								Excitation,
								Path) ;

   m_BeamEnergySpread   =  BeamEnergySpread  ;
   m_BeamFWHMX       =  BeamFWHMX         	;
   m_BeamFWHMY       =  BeamFWHMY         	;
   m_BeamEmmitanceTheta    =  BeamEmmitanceTheta    	 ;
   m_BeamEmmitancePhi    =  BeamEmmitancePhi    	 ;
   m_ResonanceDecayZ =  ResonanceDecayZ      ;
   m_ResonanceDecayA =  ResonanceDecayA      ;
   m_ShootLight      =  ShootLight       	 ;
   m_ShootHeavy      =  ShootHeavy        	;
   m_ShootDecayProduct    =  ShootDecayProduct      	;

}

