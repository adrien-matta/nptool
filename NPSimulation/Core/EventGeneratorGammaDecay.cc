/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : May 2012                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated a gamma decay of nuclei genera-*
 * ted by previous event generator. Multiple cases are supported:            *
 *  - Only one gamma is emmited, in this case a Cross section can be given   *
 *  - A cascade decay, in this case the CS is ignore                         *
 *  - If more than one cascade are given, Branching Ratio could be given     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "EventGeneratorGammaDecay.hh"

// NPS
#include "Particle.hh"

// NPL
#include "NPOptionManager.h"
#include "NPFunction.h"
#include "NPInputParser.h"
using namespace NPL;

// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// G4
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
// ROOT
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TF1.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorGammaDecay::EventGeneratorGammaDecay(){
  m_ParticleStack = ParticleStack::getInstance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorGammaDecay::~EventGeneratorGammaDecay(){
  m_CrossSectionHist.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorGammaDecay::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("GammaCascade");
  vector<string> token = {"BranchingRatio","Energies"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(NPOptionManager::getInstance()->GetVerboseLevel())
      cout << endl << "\033[1;35m//// Cascade for nuclei " << blocks[i]->GetMainValue() << endl;
    if(m_NucleiName=="")
      m_NucleiName = blocks[i]->GetMainValue();

    else if(m_NucleiName!=blocks[i]->GetMainValue()){
      cout << "ERROR: The decay of only one nuclei is presently supported\033[0m" << endl;
      exit(1);
    }
   
    vector<double> E = blocks[i]->GetVectorDouble("Energies","MeV");
    double BranchingRatio = blocks[i]->GetDouble("BranchingRatio","void");  
    vector<string> CS; 
    if(blocks[i]->HasToken("DifferentialCrossSection"))
      CS = blocks[i]->GetVectorString("DifferentialCrossSection");

    // Sanity Check
    if(E.size()>1 && CS.size()>0){
      cout << "ERROR: Cannot use Cross section with decay of multiple gamma" << endl;
    }
     
    if(CS.size()>0) 
      AddCascade(E, BranchingRatio, CS[0] , CS[1]);
    else
      AddCascade(E, BranchingRatio);
  }
PrepareCascade();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorGammaDecay::GenerateEvent(G4Event*){
  
  // Choose a Cascade to follow
  int ChoosenCascade = -1;
  double RandomNumber = RandFlat::shoot();
  
  for (unsigned int i = 1; i<m_BranchingRatio.size(); i++) {
    if(RandomNumber > m_BranchingRatio[i-1] && RandomNumber< m_BranchingRatio[i])
      ChoosenCascade=i;
  }
  
  if (ChoosenCascade==-1) ChoosenCascade=0;
  
  // Look for the decaying nucleus
  Particle decayingParticle = m_ParticleStack->SearchAndRemoveParticle(m_NucleiName);
  
  if(decayingParticle.GetParticleDefinition()==NULL){
    G4cout << "Gamma Decay Warning: The decaying particle " << m_NucleiName
    << " was not found in the particle stack " << G4endl;
    return ;
  }
  // Check for energies conservation (i.e: Cascade Energies lower than Excitation energie)
  string ExcitationString = decayingParticle.GetParticleDefinition()->GetParticleName();
  ExcitationString.erase(0,m_NucleiName.length()+1);
  ExcitationString.erase(ExcitationString.length()-1,ExcitationString.length());
  double ExcitationEnergy = atof(ExcitationString.c_str())*keV;
  
  // Compute the final excitation energy of the decaying nuclei
  double FinalExcitationEnergy = ExcitationEnergy-m_CascadeTotalEnergy[ChoosenCascade];
  if(FinalExcitationEnergy<0){
    G4cout << "Gamma Decay Warning: The cascade energy exceed the excitation energy of the decaying nuclei: "
    << G4endl << " Excitation Energy : " << ExcitationEnergy
    << G4endl << " Cascade Energy : "    <<    m_CascadeTotalEnergy[ChoosenCascade] << G4endl;
    FinalExcitationEnergy=0;
  }
  
  // Put back the decaying nucleus with its new excitation energy
  G4ParticleDefinition* FinalParticleDefition
  = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(decayingParticle.GetParticleDefinition()->GetAtomicNumber(), decayingParticle.GetParticleDefinition()->GetAtomicMass(), FinalExcitationEnergy*MeV);
  
  Particle FinalParticle = Particle(  FinalParticleDefition,
                                    decayingParticle.GetParticleThetaCM(),
                                    decayingParticle.GetParticleKineticEnergy(),
                                    decayingParticle.GetParticleMomentumDirection(),
                                    decayingParticle.GetParticlePosition(),
                                    decayingParticle.GetShootStatus());
  
  m_ParticleStack->AddParticleToStack(FinalParticle);
  
  // Instantiate and add the gamma to the particle stack
  for (unsigned int i = 0; i < m_Energies[ChoosenCascade].size(); i++) {
    G4ParticleDefinition* gammaDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    G4ThreeVector gammaDirection;
    double theta=0;
    double phi=0;
    // If more than one gamma shoot no cross section to follow
    if(m_Energies[ChoosenCascade].size()>1){
      
      // Shoot flat in cos(theta) and Phi to have isotropic emission
      double cos_theta = -1+2*RandFlat::shoot();
      theta = acos(cos_theta);
      phi = RandFlat::shoot()*2.*pi;
      
      gammaDirection= G4ThreeVector(  cos(phi)*sin(theta),
                                    sin(phi)*sin(theta),
                                    cos(theta));
    }
    
    // Only one gamma to shoot, use the given cross section
    else{
      theta = m_CrossSectionHist[ChoosenCascade]->GetRandom()*deg;
      phi   = RandFlat::shoot() * 2. *pi;
      gammaDirection= G4ThreeVector(  cos(phi)*sin(theta),
                                    sin(phi)*sin(theta),
                                    cos(theta));
    }
    
    // Doppler shifted gamma emission
    decayingParticle.GetParticleMomentumDirection();
    double gammaEnergy = m_Energies[ChoosenCascade][i];
    TLorentzVector GammaLV( gammaEnergy*cos(phi)*sin(theta),
                           gammaEnergy*sin(phi)*sin(theta),
                           gammaEnergy*cos(theta),
                           gammaEnergy);
    
    double NucleiEnergy= decayingParticle.GetParticleKineticEnergy()+FinalParticleDefition->GetPDGMass();
    double NucleiMomentum= sqrt(NucleiEnergy*NucleiEnergy-FinalParticleDefition->GetPDGMass()*FinalParticleDefition->GetPDGMass());
    TLorentzVector NuvleiLV( NucleiMomentum*decayingParticle.GetParticleMomentumDirection().x(),
                            NucleiMomentum*decayingParticle.GetParticleMomentumDirection().y(),
                            NucleiMomentum*decayingParticle.GetParticleMomentumDirection().z(),
                            NucleiEnergy);
    
    GammaLV.Boost(NuvleiLV.BoostVector());
    gammaDirection= G4ThreeVector(  GammaLV.Px(),
                                  GammaLV.Py(),
                                  GammaLV.Pz());
    // Add the gamma to the stack
    Particle gammaParticle(gammaDefinition,theta,GammaLV.E(),gammaDirection, decayingParticle.GetParticlePosition());
    m_ParticleStack->AddParticleToStack(gammaParticle);
  }
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorGammaDecay::SetTarget(Target* Target){
  m_Target = Target;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorGammaDecay::AddCascade(vector<double> Energies, double BranchingRatio, string CrossSectionPath, string CrossSectionName){
  m_BranchingRatio.push_back(BranchingRatio);
  m_CrossSectionPath.push_back(CrossSectionPath);
  m_CrossSectionName.push_back(CrossSectionName);
  m_Energies.push_back(Energies);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorGammaDecay::PrepareCascade(){
  // Change the given branching ratio so total is one (always have a decay during the event)
  double TotalRatio=0;
  for (unsigned int i = 0; i < m_BranchingRatio.size(); i++) {
    TotalRatio+=m_BranchingRatio[i]/100.;
  }
  
  // Check that the total ratio is not over 100% (below is allowed)
  if(TotalRatio>1) {
    cout << "Gamma Decay Error: Sum of branching ratio is over 100%" << endl;
    exit(1);
  }
  
  for (unsigned int i = 0; i < m_BranchingRatio.size(); i++) {
    m_BranchingRatio[i]=(m_BranchingRatio[i]/TotalRatio)/100.;
  }
  
  // Shift the Branching ratio for faster shooting during event generation
  for (unsigned int i = 1; i < m_BranchingRatio.size(); i++) {
    m_BranchingRatio[i]=m_BranchingRatio[i-1]+m_BranchingRatio[i];
  }
  
  // Compute the total energy of the cascade
  double TotalEnergy=0;
  for (unsigned int i = 0 ; i < m_Energies.size(); i++) {
    TotalEnergy=0;
    for (unsigned int j = 0; j < m_Energies[i].size(); j++) {
      TotalEnergy+=m_Energies[i][j];
    }
    m_CascadeTotalEnergy.push_back(TotalEnergy);
  }
  
  // Transform the particle name to G4 standard: i.e: 10He -> He10
  m_NucleiName = m_ParticleStack->ChangeNameToG4Standard(m_NucleiName);
  if (m_NucleiName=="proton"   ||
      m_NucleiName=="deuteron" ||
      m_NucleiName=="triton"   ||
      m_NucleiName=="3He"      ||
      m_NucleiName=="alpha"    ){
    G4cout << "Gamma Decay Error: Gamma Decay not allowed for light particles" << G4endl;
    exit(1);
  }
  
  /// Load the differential cross section
  for (unsigned int i = 0; i < m_BranchingRatio.size(); i++) {
    TH1F* h ;
    
    if(m_CrossSectionPath[i]!="_void_"){
      h = Read1DProfile(m_CrossSectionPath[i],m_CrossSectionName[i]);
      TF1* sinus = new TF1("sinus","1/sin(x*3.141592653589793/180.)",0,180);
      h->Divide(sinus,1);
      delete sinus;
    }

    else{
      int offset = 0;
      while(gDirectory->FindObjectAny(Form("_gammavoid_%i",offset))!=0)
        ++offset;
      
      h = new TH1F(Form("_gammavoid_%i",offset),Form("_gammavoid_%i",offset),1,0,1);
      }
    
    m_CrossSectionHist.push_back(h);
  }
}

