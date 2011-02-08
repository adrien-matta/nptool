/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : Feb 2010                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated pure phase space event and     *
 *  especially evaluate the background contribution of different phase space * 
 *  channel during an experiment. Manage up to 18 body phase space           *
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
#include "EventGeneratorPhaseSpace.hh"
#include "RootOutput.h"

//Root Headers
#include "TGenPhaseSpace.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorPhaseSpace::EventGeneratorPhaseSpace()
{
   //------------- Default Constructor -------------
   m_InitConditions   = new TInitialConditions()   ;
   m_Target           = new Target()               ;
   m_SigmaX           = 0                          ;
   m_SigmaY           = 0                          ;
   m_SigmaThetaX      = 0                          ;
   m_SigmaPhiY        = 0                          ;
   m_EventWeight      = 0                          ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorPhaseSpace::~EventGeneratorPhaseSpace()
{
  //------------- Default Destructor ------------
   delete m_InitConditions   ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void   EventGeneratorPhaseSpace::SetTarget(Target* Target) 
   {
      if(Target!=0)   
            m_Target = Target;
   }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorPhaseSpace::EventGeneratorPhaseSpace(   G4ParticleDefinition*          BeamParticle,
                                                      G4ParticleDefinition*          TargetParticle,
                                                      vector<G4ParticleDefinition*>  ReactionProducts,
                                                      double                         BeamEnergy,      
                                                      double                         BeamEnergySpread,
                                                      double                         SigmaX,
                                                      double                         SigmaY,
                                                      double                         SigmaThetaX,
                                                      double                         SigmaPhiY)
{
   //------------- Constructor with nuclei names and beam energy ------------

   SetEverything( BeamParticle,
                  TargetParticle,
                  ReactionProducts,
                  BeamEnergy,      
                  BeamEnergySpread,
                  SigmaX,
                  SigmaY,
                  SigmaThetaX,
                  SigmaPhiY);
                  
    m_EventWeight = 0;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorPhaseSpace::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("InitialConditions", "TInitialConditions", &m_InitConditions);
   pTree->Branch("EventWeight",&m_EventWeight,"EventWeigt/D");
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//    Inherit from VEventGenerator

void EventGeneratorPhaseSpace::ReadConfiguration(string Path)
{
////////General Reading needs////////
   string LineBuffer;
   string DataBuffer;

////////Reaction Setting needs///////
   string Beam, Target, Heavy, Light, CrossSectionPath ;
   G4double BeamEnergy = 0 , BeamEnergySpread = 0 , SigmaX = 0 , SigmaY = 0 , SigmaThetaX = 0 , SigmaPhiY=0;
   vector<G4ParticleDefinition*> ReactionProducts;
   G4ParticleDefinition* BeamParticle   = NULL ;
   G4ParticleDefinition* TargetParticle = NULL ;
   
   bool ReadingStatus          =   false ;
   bool check_Beam             = false ;
   bool check_Target           = false ;
   bool check_ReactionProducts = false ;
   bool check_BeamEnergy       = false ;
   bool check_BeamEnergySpread = false ;
   bool check_FWHMX            = false ;
   bool check_FWHMY            = false ;
   bool check_EmmitanceTheta   = false ;
   bool check_EmmitancePhi     = false ;
   
//////////////////////////////////////////////////////////////////////////////////////////
   ifstream ReactionFile;
   ReactionFile.open(Path.c_str());

   if (ReactionFile.is_open()) {} 
   else {
            return;
            }

   while (!ReactionFile.eof()) {
      //Pick-up next line
      getline(ReactionFile, LineBuffer);

      if (LineBuffer.compare(0, 10, "PhaseSpace") == 0) { ReadingStatus = true ;}


      while(ReadingStatus){
             
           ReactionFile >> DataBuffer;
           
           //Search for comment Symbol %
             if (DataBuffer.compare(0, 1, "%") == 0) {   ReactionFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
           
            else if (DataBuffer.compare(0, 5, "Beam=") == 0) {
               check_Beam = true ;
             ReactionFile >> DataBuffer;  ReactionFile >> DataBuffer;
               int A = atoi(DataBuffer.c_str());
               ReactionFile >> DataBuffer; ReactionFile >> DataBuffer;
               int Z = atoi(DataBuffer.c_str());
               ReactionFile >> DataBuffer; ReactionFile >> DataBuffer;
               int EXX = atoi(DataBuffer.c_str())*MeV;
               
               G4ParticleDefinition* ParticleBuffer = G4ParticleTable::GetParticleTable()->GetIon(Z, A, EXX) ;
               BeamParticle = ParticleBuffer ;
               
               G4cout << "Beam : A=" << A << " Z=" << Z << " Excitation Energy = " << EXX/MeV << "MeV" << G4endl;
            }
   
            else if (DataBuffer.compare(0, 7, "Target=") == 0) {
               check_Target = true ;
               ReactionFile >> DataBuffer;  ReactionFile >> DataBuffer;
               int A = atoi(DataBuffer.c_str());
               ReactionFile >> DataBuffer; ReactionFile >> DataBuffer;
               int Z = atoi(DataBuffer.c_str());
               ReactionFile >> DataBuffer; ReactionFile >> DataBuffer;
               int EXX = atoi(DataBuffer.c_str())*MeV;
               
               G4ParticleDefinition* ParticleBuffer = G4ParticleTable::GetParticleTable()->GetIon(Z, A, EXX) ;
               TargetParticle = ParticleBuffer ;
               
               G4cout << "Target : A=" << A << " Z=" << Z << " Excitation Energy = " << EXX/MeV << "MeV" << G4endl;
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

               else if  (DataBuffer.compare(0, 13, "DecayProduct=") == 0) {
               ReactionFile >> DataBuffer; ReactionFile >> DataBuffer;
               int A = atoi(DataBuffer.c_str());
               ReactionFile >> DataBuffer; ReactionFile >> DataBuffer;
               int Z = atoi(DataBuffer.c_str());
               ReactionFile >> DataBuffer; ReactionFile >> DataBuffer;
               int EXX = atoi(DataBuffer.c_str())*MeV;
               
               G4ParticleDefinition* ParticleBuffer ;
               
                  if          ( A==1 && Z==0 )
                  ParticleBuffer = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
               else if ( A==1 && Z==1)
                  ParticleBuffer = G4ParticleTable::GetParticleTable()->FindParticle("proton");
//               else if ( A==2 && Z==0)
//                  ParticleBuffer = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
//                  ParticleBuffer->SetPDGMass( ParticleBuffer->GetPDGMass*2);
               else
                  ParticleBuffer = G4ParticleTable::GetParticleTable()->GetIon(Z, A, EXX) ;
                  
               ReactionProducts.push_back(ParticleBuffer);
               
               G4cout << "Decay Product: A=" << A << " Z=" << Z << " Excitation Energy = " << EXX/MeV << "MeV" << G4endl;
            }
            
            else if  (DataBuffer.compare(0, 21, "EndOfDecayProductList") == 0) {
                 check_ReactionProducts = true ;
            }

           
            ///////////////////////////////////////////////////
         //   If no Transfert Token and no comment, toggle out
            else 
               {ReadingStatus = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
               
            ///////////////////////////////////////////////////
         //   If all Token found toggle out
            if(      check_Beam && check_Target &&  check_BeamEnergy && check_BeamEnergySpread && check_FWHMX && check_FWHMY && check_EmmitanceTheta 
               &&  check_EmmitancePhi && check_ReactionProducts)
               ReadingStatus = false ;   

         }
           

   }
   
   SetEverything( BeamParticle,
                  TargetParticle,
                  ReactionProducts,
                  BeamEnergy,      
                  BeamEnergySpread,
                  SigmaX,
                  SigmaY,
                  SigmaThetaX,
                  SigmaPhiY);

   ReactionFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorPhaseSpace::GenerateEvent(G4Event* anEvent , G4ParticleGun* particleGun)
{

   //   If first time, write the DeDx table
   if(anEvent->GetEventID()==0 && m_Target!=0)
      {
        m_Target->WriteDEDXTable(m_BeamParticle ,0, m_BeamEnergy+4*m_BeamEnergySpread);
        for(unsigned int k = 0 ; k < m_ReactionProducts.size() ; k++)
           {
            m_Target->WriteDEDXTable(m_ReactionProducts[k] ,0, m_BeamEnergy+4*m_BeamEnergySpread);
           }
      }

   // Clear contents of Precedent event (now stored in ROOTOutput)
   m_InitConditions->Clear()   ;
    m_EventWeight = 0                ;
    
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
   
   m_Target->CalculateBeamInteraction( 0, m_SigmaX, 0, m_SigmaThetaX,
                                       0, m_SigmaY, 0, m_SigmaPhiY,
                                       InitialBeamEnergy,
                                       m_BeamParticle,
                                       InterCoord, Beam_thetaX, Beam_phiY,
                                       Beam_theta, Beam_phi,
                                       FinalBeamEnergy);
                                              
   m_InitConditions->SetICIncidentEnergy(FinalBeamEnergy / MeV);

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
   //////////////// Phase Space Calculation /////////////////
   //////////////////////////////////////////////////////////
      
   // Masses array of ecay products in GeV (unit used by ROOT)
   int NumberOfReactionProducts = m_ReactionProducts.size() ;
   double* masses = new double[NumberOfReactionProducts];  

   for(int k = 0 ; k < NumberOfReactionProducts ; k++ )
    {
     masses[k] = m_ReactionProducts[k]-> GetPDGMass()/GeV ;
    }
    
   // Kinematics of reaction
   G4double M =  m_BeamParticle -> GetPDGMass(); 
   G4double InitialE         = FinalBeamEnergy + M ;
   G4double InitialMomentumX = sqrt( InitialE*InitialE - M*M) * sin(Beam_theta) * cos(Beam_phi);
   G4double InitialMomentumY = sqrt( InitialE*InitialE - M*M) * sin(Beam_theta) * sin(Beam_phi);
   G4double InitialMomentumZ = sqrt( InitialE*InitialE - M*M) * cos(Beam_theta);

   TLorentzVector Initial = TLorentzVector(InitialMomentumX/GeV, InitialMomentumY/GeV, InitialMomentumZ/GeV,InitialE/GeV) + TLorentzVector(0,0,0,m_TargetParticle -> GetPDGMass() / GeV);


   // Instentiate a Phase Space Generator, with flat distrution
   TGenPhaseSpace TPhaseSpace ;
      
   if( !TPhaseSpace.SetDecay(Initial, NumberOfReactionProducts , masses) ) cout << "Warning: Phase Space Decay forbiden by kinematic, or more than 18 particles "<<endl;

   //   Generate event and store the associate weight. Think to use this weigt to get correcte spectrum
   m_EventWeight = TPhaseSpace.Generate();

      
      TLorentzVector* ProductLV ;
      for ( int u = 0; u < NumberOfReactionProducts ; u++) 
         {
            ProductLV = TPhaseSpace.GetDecay(u);

            G4ThreeVector Momentum (   ProductLV->X()*GeV   ,
                                       ProductLV->Y()*GeV   ,
                                       ProductLV->Z()*GeV   );
            Momentum.unit() ;
                                                   
            G4double Energy   = ProductLV->E()*GeV-masses[u]*GeV ;
            
            //Set the gun to shoot
            particleGun->SetParticleDefinition(m_ReactionProducts[u]);
            particleGun->SetParticleMomentumDirection(Momentum);
            particleGun->SetParticleEnergy(Energy);
            particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorPhaseSpace::SetEverything(   G4ParticleDefinition*            BeamParticle,
                                                G4ParticleDefinition*            TargetParticle,
                                                vector<G4ParticleDefinition*>    ReactionProducts,
                                                double                           BeamEnergy,      
                                                double                           BeamEnergySpread,
                                                double                           SigmaX,
                                                double                           SigmaY,
                                                double                           SigmaThetaX,
                                                double                           SigmaPhiY)
{
   //------------- Constructor with nuclei names and beam energy ------------
   m_BeamParticle      = BeamParticle;
   m_TargetParticle    = TargetParticle;
   m_ReactionProducts  = ReactionProducts;
   m_BeamEnergy        = BeamEnergy;
   m_BeamEnergySpread  = BeamEnergySpread;
   m_SigmaX            = SigmaX;
   m_SigmaY            = SigmaY;
   m_SigmaThetaX       = SigmaThetaX;
   m_SigmaPhiY         = SigmaPhiY;
}

