/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * NPTool Author: Adrien MATTA        contact address: matta@ipno.in2p3.fr   *
 * Class Author: Lee Evitts           contact address: evitts@triumf.ca      *
 *                                                                           *
 * Creation Date  : April 2014                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Read in a decay scheme for nuclear de-excitation (involving gamma rays,   *
 * ICC electrons and IPF e-/e+ pairs                                         *  
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "EventGeneratorNuclearDeexcitation.hh"

// NPS
#include "Particle.hh"

// NPL
#include "NPOptionManager.h"
#include "NPFunction.h"
using namespace NPL;

// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// G4
#include "G4ParticleTable.hh"

// ROOT
#include "TLorentzVector.h"
#include "TVector3.h"

// --------------------------------------------------
EventGeneratorNuclearDeexcitation::EventGeneratorNuclearDeexcitation(){
  fParticleStack = ParticleStack::getInstance();
}



// --------------------------------------------------
EventGeneratorNuclearDeexcitation::~EventGeneratorNuclearDeexcitation(){
  //m_CrossSectionHist.clear();
}



// --------------------------------------------------
void EventGeneratorNuclearDeexcitation::ReadConfiguration(string Path, int Occurence) {

  // Variable declaration
  int TokenOccurence = 0;
  bool ReadingStatus = false;
  bool BindingRead = false;
  fEmitSecondIPF = false;
  
  int CurrentLevelID, TransitionID, PolarityOrder, FinalLevelID;
  double CurrentLevelProb, CurrentLevelEnergy, TransitionProbability, ICC, IPFC;
  string FieldType;
  bool checkTP = false;
  bool checkICC = false;
  bool checkFT = false;
  bool checkPol = false;
  bool checkIPFC = false;
  bool checkID = false;
  
  int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();
  
  // Open file ..
  string LineBuffer;
  string DataBuffer;
  istringstream LineStream;
  ifstream inputFile(Path.c_str());
  if (inputFile.is_open()) {} else { return; }
  
  // .. while open ..
  while (!inputFile.eof()) {
    getline(inputFile, LineBuffer);
    
    if (LineBuffer.compare(0,19, "NuclearDeexcitation")==0) {
      TokenOccurence++;
      if (TokenOccurence == Occurence) {
        ReadingStatus = true;
        
        LineStream.clear();
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        DataBuffer.erase();
        LineStream >> DataBuffer;
        fNucleiName = DataBuffer;
        
        if(VerboseLevel==1) G4cout << "Nuclear De-excitation for " << fNucleiName << G4endl;
      } // END if (TokenOccurence == Occurence)
    } // END if (LineBuffer.compare(0,19, "NuclearDeexcitation")==0)    
    
    while (ReadingStatus) {
      getline(inputFile, LineBuffer);
      LineStream.clear();
      LineStream.str(LineBuffer);
      LineStream >> DataBuffer;
      
      if (DataBuffer == "BindingEnergy=") {
        BindingRead = true;
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        fBindingEnergy = atof(DataBuffer.c_str());
        if(VerboseLevel==1) G4cout << "  Binding Energy: " << fBindingEnergy << G4endl;
      } // END if (DataBuffer == "BindingEnergy=")
      
      else if (DataBuffer == "LevelID=") {
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        CurrentLevelID = atoi(DataBuffer.c_str());
        if(VerboseLevel==1) G4cout << "  Level ID: " << CurrentLevelID << G4endl;
      } // END if (DataBuffer == "LevelID=")
      
      else if (DataBuffer == "PopulationProbability=") {
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        CurrentLevelProb = atof(DataBuffer.c_str());
        if(VerboseLevel==1) G4cout << "    Level Probability: " << CurrentLevelProb << G4endl;
      } // END if (DataBuffer == "PopulationProbability=")
      
      else if (DataBuffer == "Energy=") {
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        CurrentLevelEnergy = atof(DataBuffer.c_str());
        if(VerboseLevel==1) G4cout << "    Level Energy: " << CurrentLevelEnergy << G4endl;
      } // END if (DataBuffer == "Energy=")
      
      else if (DataBuffer == "TransitionID=") {
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        TransitionID = atoi(DataBuffer.c_str());
        if(VerboseLevel==1) G4cout << "    Transition ID: " << TransitionID << G4endl;
      } // END if (DataBuffer == "TransitionID=")
      
      else if (DataBuffer == "TransitionProbability=") {
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        TransitionProbability = atof(DataBuffer.c_str());
        if(VerboseLevel==1) G4cout << "      Transition Probability: " << TransitionProbability << G4endl;
        checkTP = true;
      } // END if (DataBuffer == "TransitionProbability=")
      
      else if (DataBuffer == "InternalConvCoeff=") {
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        ICC = atof(DataBuffer.c_str());
        if(VerboseLevel==1) G4cout << "      Internal CC: " << ICC << G4endl;
        checkICC = true;
      } // END if (DataBuffer == "InternalConvCoeff=")
      
      else if (DataBuffer == "FieldType=") {
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        FieldType = DataBuffer.c_str();
        if(VerboseLevel==1) G4cout << "      Field Type: " << FieldType << G4endl;
        checkFT = true;
      } // END if (DataBuffer == "FieldType=")
      
      else if (DataBuffer == "PolarityOrder=") {
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        PolarityOrder = atoi(DataBuffer.c_str());
        if(VerboseLevel==1) G4cout << "      Polarity Order: " << PolarityOrder << G4endl;
        checkPol = true;
      } // END if (DataBuffer == "PolarityOrder=")
      
      else if (DataBuffer == "InternalPairFormCoeff=") {
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        IPFC = atof(DataBuffer.c_str());
        if(VerboseLevel==1) G4cout << "      IPF Coefficient: " << IPFC << G4endl;
        checkIPFC = true;
      } // END if (DataBuffer == "InternalPairFormCoeff=")
      
      else if (DataBuffer == "FinalLevelID=") {
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        LineStream >> DataBuffer;
        FinalLevelID = atoi(DataBuffer.c_str());
        if(VerboseLevel==1) G4cout << "      Final Level ID: " << FinalLevelID << G4endl;
        checkID = true;
      } // END if (DataBuffer == "FinalLevelID=")    
      
      // Add previously defined transition to vector
      if (checkTP && checkICC && checkFT && checkPol && checkIPFC && checkID) {
        AddTransition(CurrentLevelID, CurrentLevelProb, CurrentLevelEnergy,
                      TransitionID, TransitionProbability, ICC, FieldType, PolarityOrder, IPFC, FinalLevelID);
        if(VerboseLevel==1) G4cout << "    Transition " << TransitionID << " Added!" << G4endl << G4endl;
        checkTP = false;
        checkICC = false;
        checkFT = false;
        checkPol = false;
        checkIPFC = false;
        checkID = false;      
      } // END if (checkTP && ...
      
      if (inputFile.eof()) {ReadingStatus=false;}
      
    } // END while (ReadingStatus)
  } // END while (!inputFile.eof()) 
  
  
  for (int i=0; i<mLevelID.size(); i++) {
    // Re-order the level/transition probabilities
    if (mLevelID[i] == mLevelID[i+1]) {
      mLevelProb[i] = 0;
      mTransProb[i+1] += mTransProb[i];
    } // END if (mLevelID[i] == mLevelID[i+1])
    if (i>0) mLevelProb[i] += mLevelProb[i-1];
    
    // Normalize the coefficients
    double sumC = mTransICC[i] + mTransIPFC[i];
    if (sumC < 1.0) sumC += 1.0;
    mTransICC[i] /= sumC;
    mTransIPFC[i] /= sumC;
    
    // Fill transition energy
    if (mTransFID[i] >= 1) {
      for (int j=0; j<mLevelID.size(); j++) {
        if (mLevelID[j] == mTransFID[i]) mTransEnergy[i] = mLevelEnergy[i] - mLevelEnergy[j];
      }
    } else mTransEnergy[i] = mLevelEnergy[i];
    
    // Build the IPF cross section for each transition
    TH2F* h2 = BuildCrossSection(mTransEnergy[i], mTransField[i], mTransOrder[i]);
    mCrossSectionLeptonHist2D.push_back(h2);
    
  } // END for (int i=0; i<mLevelID.size(); i++)

  fInCascade = false;
}
  
  
  
// --------------------------------------------------
void EventGeneratorNuclearDeexcitation::GenerateEvent(G4Event*) { 

  // Variable Declaration
  int ChosenTrans = -1;
  int FinalLevel = 1000;
  int NbrTrans;
  double RandNumber;

  // Decide on which level is populated, if not already within a cascade
  if (!fInCascade) {
    RandNumber = RandFlat::shoot();
    fChosenLevel = mLevelID[0];
    for (int i=1; i<mLevelID.size(); i++) {
      if (RandNumber >= mLevelProb[i-1] && RandNumber < mLevelProb[i])
        fChosenLevel = mLevelID[i];
    } // END for (int i=0; i<mLevelID.size(); i++)
  } // END if (!fInCascade)

  if (!fEmitSecondIPF) {  

    // Decide on transition ...
    RandNumber = RandFlat::shoot();
    NbrTrans = 0;
    // .. get the number of transitions in the level
    for (int i=0; i<mLevelID.size(); i++) { if (mLevelID[i] == fChosenLevel) NbrTrans++; }
    // .. if there is only 1 transition, return it as the chosen transition ID
    if (NbrTrans == 1) { 
      for (int i=0; i<mLevelID.size(); i++) { if (mLevelID[i] == fChosenLevel) ChosenTrans = mTransID[i]; }
    } else if (NbrTrans > 1) {
      // .. if there is more than 1 transition, loop through until the first transition, get that ID
      for (int i=0; i<mLevelID.size(); i++) {
        if (mLevelID[i] == fChosenLevel) ChosenTrans = mTransID[i];
        break;
      } // END for (int i=0; i<mLevelID.size(); i++)
      // .. then with the random number, choose which transition
      for (int i=1; i<mLevelID.size(); i++) {
        if (mLevelID[i] == fChosenLevel && mLevelID[i-1] == fChosenLevel) {
          if (RandNumber >= mTransProb[i-1] && RandNumber < mTransProb[i]) ChosenTrans = mTransID[i];
        }
      } // END for (int i=1; i<mLevelID.size(); i++)
    } // END if (NbrTrans ...

    // Randomise direction of particle
    double cos_theta = RandFlat::shoot(-1., 1.);
    fEmissionTheta = acos(cos_theta);
    double phi = RandFlat::shoot()*2.*pi;    
    fParticleDirection= G4ThreeVector(  cos(phi)*sin(fEmissionTheta), sin(phi)*sin(fEmissionTheta), cos(fEmissionTheta));
    fPosition = G4ThreeVector( 0., 0., 0.);
    
    // Decide on transition type
    RandNumber = RandFlat::shoot();
    if (RandNumber < mTransICC[ChosenTrans-1]) { // Emit ICC
      fParticleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("e-");
      fParticleEnergy = mTransEnergy[ChosenTrans-1] - fBindingEnergy;
    } 
    else if ( RandNumber < (mTransICC[ChosenTrans-1] + mTransIPFC[ChosenTrans-1]) ) { // Emit IPF
      // Define e+      
      fParticleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("e+");
    
      // Define e-
      fElectronDefinition = G4ParticleTable::GetParticleTable()->FindParticle("e-");

      // Get the e+ energy and theta separation from cross section
      double thetaSeparation;     
      mCrossSectionLeptonHist2D[ChosenTrans-1]->GetRandom2(fParticleEnergy, thetaSeparation);
      thetaSeparation *= deg;

      // ... from which, get the e- energy
      fElectronEnergy = mTransEnergy[ChosenTrans-1] - fParticleEnergy - 1.022;
      fEmitSecondIPF = true;
      
      // Build the electron direction as a copy of the positron distribution where the polar angle is incremented by thetaSeparation
      fElectronPhi = phi; // copy the same value for now 
      fElectronTheta = fEmissionTheta + thetaSeparation;
      fElectronDirection= G4ThreeVector( cos(fElectronPhi)*sin(fElectronTheta), sin(fElectronPhi)*sin(fElectronTheta), cos(fElectronTheta) ); 
      
      // Rotate the electron direction around the positron vector, this will conserve theta separation.
      double phiSeparation   = RandFlat::shoot() * 2. * pi; 
      fElectronDirection.rotate(phiSeparation,fParticleDirection);  

    } else { // Emit gamma
      fParticleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
      fParticleEnergy = mTransEnergy[ChosenTrans-1];   
    }
  
    // Add particle to stack
    Particle fParticle(fParticleDefinition, fEmissionTheta, fParticleEnergy, fParticleDirection, fPosition);
    fParticleStack->AddParticleToStack(fParticle);
    
    // If the final level isn't ground state (0) then emit next transition in next generateevent
    FinalLevel = mTransFID[ChosenTrans-1];
    fChosenLevel = FinalLevel;
    if (FinalLevel != 0) fInCascade = true;
    else if (FinalLevel == 0) fInCascade = false;
  } // END   if (!fEmitSecondIPF)
  else if (fEmitSecondIPF) {
    Particle electronParticle(fElectronDefinition,fElectronTheta,fElectronEnergy,fElectronDirection, fPosition);
    fParticleStack->AddParticleToStack(electronParticle);
    fEmitSecondIPF = false;
  } // END else if (fEmitSecondIPF) {
    
}



// --------------------------------------------------
TH2F*  EventGeneratorNuclearDeexcitation::BuildCrossSection( double TransitionEnergy, string FieldType, double PolarityOrder) {

string HistName = "IPFCrossSection";

//prepare the variables
double Ep, Em ; // kinetic energy of e+ and e-
double angle ; // angle of separation between e+ and e-
double cross_section ; // giving the number of pairs per unit energy, per |dCos(theta)|, per Quantum 

// prepare the fixed parameters
int    l = (int)PolarityOrder ; 
int Nbinx = 180 ; // energy
int Nbiny = 180; // angle

double Ep_max = TransitionEnergy - 2*electron_mass_c2  ; // maximum energy of the positron
double Ep_min = 0 ; // minimum energy of the positron
double angle_max = 180  ; // maximum separation angle
double angle_min = 0  ; // minimum separation angle
 
//prepare the histogram
TH2F* h2  = new TH2F (HistName.c_str(),HistName.c_str(),       Nbinx,Ep_min,Ep_max,      Nbiny,angle_min,angle_max) ; 

// intantiate the start values
angle = angle_min ; 
Ep = Ep_min ;
Em = Ep_max - Ep ;  

int counter = 0 ;  
while (Ep < Ep_max) // iterates on energy (x-axis)
{

	while (angle < angle_max) // iterates on angle (y-axis)
	{		
	 //Get Cross section in Born approximation and fill the histogram
	 int bin = h2->FindBin(Ep,angle);
	 cross_section = GetBornCrossSection(FieldType, TransitionEnergy, l, Ep*MeV, Em*MeV, angle*deg) ;	 
     	 //if(cross_section < 0.01 )
	 h2->SetBinContent(bin,cross_section);
	 
	 // increment angle 
	 angle = angle + ((angle_max - angle_min)/Nbiny);
	 	  
	} 
	 // reset the angle
	 angle = angle_min ;
	 //increment kinetic energy values of e- and e+
	 Ep = Ep + ((Ep_max - Ep_min)/Nbinx) ; 
	 Em = Ep_max - Ep ;
}

	
return h2 ; 

}	


	
// --------------------------------------------------
double EventGeneratorNuclearDeexcitation::GetBornCrossSection(string FieldType, double TransitionEnergy, int l, double Ep, double Em, double angle) { //Born approximation 
 
// for PolarityOrder > 0 : the cross section is a parametric function calculated by M.E.Rose [Phys. Rev. 76 (1949) 678 ] 
// this calculation is valid for Z < 41, Gamma Energy > 2.5 MeV
// for PolarityOrder = 0 : the cross section is as parametric function calculated by J.R.Oppenheimer [Phys. Rev. 60 (1941) 159 ]
// this calculation is valid for Z < ??, Gamma Energy > ?? MeV 
// In both cases more precise calculations could be made that are not addressed here and could be found in the litterature.

// N.B : in Rose paper,  hbar =  c  =  electron mass = 1, 
// all energy values should be devided by electron rest mass energy, 
// fine structure constant = elm_coupling/hbarc is multiplied by hbarc.
// This will not affect 

//Calculate primary parameters 
 double api = 2*(fine_structure_const*hbarc)/pi ; 
 double M = electron_mass_c2 /electron_mass_c2  ; // electron mass in units of electron_mass
 double k = TransitionEnergy /electron_mass_c2 ; // Energy of Transition in units of electron_mass
 	Ep = Ep /electron_mass_c2 ; // Energy of Transition in units of electron_mass
 	Em = Em /electron_mass_c2 ; // Energy of Transition in units of electron_mass
 
// calculate total (kinetic + rest mass) energy of e+ and e-
 double Wp = Ep + M ;   
 double Wm = Em + M ;
 double Pp = sqrt(Wp*Wp - M*M) ; //  momentum magnitudes of e+ 
 double Pm = sqrt(Wm*Wm - M*M) ; //                         e- 	
 double Q = Pp*Pp + Pm*Pm + 2*Pp*Pm*cos(angle) ; // sum of momenta 	
	 
 //Calculate secondary parameters
 double Q_k = Q/k ; 
 double kk_QQ = 1 ; //(k*k - Q*Q)  ;   MHD : Need to check the theory! 
 double pp_Q =  Pp*Pm/Q ;
 double pp_QQ =  Pp*Pm/(Q*Q) ;
 double Pp_Pmcos =  Pp + (Pm * cos(angle)) ;
 double Pm_Ppcos =  Pm + (Pp * cos(angle)) ; 
  
 // cross section parts
 double cs1 = 0  ; 
 double cs2 = 0  ;
     		
 if (FieldType == "M" || FieldType == "Magnetic" || FieldType == "MAGNETIC") {
 	if (l!=0) { // Ml (l>0)
	 cs1 = api * pp_Q * pow(Q_k,(2*l+1)) / (kk_QQ*kk_QQ) ;
	 //cs2 = 1 + (Wp*Wm) - ( pp_QQ * Pm_Ppcos * Pp_Pmcos ) ;
	 cs2 = M*M + (Wp*Wm) - ( pp_QQ * Pm_Ppcos * Pp_Pmcos ) ; 

	 //return ( pow(Q_k,(2*l+1)) / kk_QQ_2 ) ; 
	 return (cs1*cs2*sin(angle)) ; 
	}
	else cout << " Magnetic monopoles does not exist. Will exit " << endl ; 
 }
 else 
	if (FieldType == "E" || FieldType == "Electric" || FieldType == "ELECTRIC") {
		if (l>0) { // El (l>0) 
		 cs1 = api/(l+1) * pp_Q * pow(Q_k,(2*l-1)) / (kk_QQ*kk_QQ) ;
		 cs2 = (2*l+1) * (Wp*Wm + 1 - (1./3 * Pp * Pm * cos(angle) ) )  ;
		 cs2 = cs2 + l * ( Q_k*Q_k - 2 ) * ( Wp*Wm - 1 + ( Pp * Pm * cos(angle) ) ) ; 
		 cs2 = cs2 +  1./3 * (l-1) * (Pp*Pm)  * ( (3/(Q*Q)) * (Pm_Ppcos) * (Pp_Pmcos) - cos(angle) ) ;
		 return (cs1*cs2*sin(angle)) ; 
	 	} 
 		else { // E0 case 
 		 cs1 = Pp*Pm ;
 		 cs2 = (Wp*Wm - M*M + Pp*Pm*cos(angle)) ;  
 		 return (cs1*cs2*sin(angle)) ; 
 		}
    }
  	else { 
	G4cout << "Field type can only be E / Electric / ELECTRIC (for Electric) or M / Magnetic / MAGNETIC (for Magnetic) " << endl;
    exit(1);
    return 0 ; 			
	}
	
}
