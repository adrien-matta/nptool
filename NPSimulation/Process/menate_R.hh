//----------------------------------------------------------------------
//----------------------------------------------------------------------
//
// menate_R.hh 
//
// Description - a GEANT4 discrete process that models neutron
// elastic scattering with non-relativistic kinematics.
//
// started on 6 May 2008
//
// a class derived from G4VDiscreteProcess
// - Includes all processes (elastic and inelastic) from original
// menate program, including kinematics
//
// Written by: Brian Roeder, Postdoc - LPC Caen, Texas A&M Univ. 
// email - broeder@comp.tamu.edu
//
// Translated from Fortran program menates.for by P. Desesquelles et al.
// Nucl. Instr. and Meths. A 307 Pg. 366 (1991)
// Uses cross sections from A. Del Geuerra et al., NIM 135 Pg. 337 (1976).
// except for n-p and n-12C elastic scattering total cross sections 
// that come from MCNPx.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// To include in physics list :
//  #include "menate_R.hh"
//  pManager = G4Neutron::Neutron()->GetProcessManager();
// 	
//    G4String NeutronProcessName = "menateR_neutron";
//    menate_R* theMenate = new menate_R(NeutronProcessName);
//    theMenate->SetMeanFreePathCalcMethod("ORIGINAL");
//
//  pManager->AddDiscreteProcess(theMenate);
// 
// -> Need also to set an environment variable called: MENATEG4XS
//    to point towards the total cross section data file directory.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// version comments
//----------------------------------------------------------------------
// version 0.1 - 18 Apr. 2008
// added elastic scattering and kinetmatics from :
// n+H1 -> n + p out (elastic)
// n+12C -> n + 12C (elastic)
//------------------------------------------------------------
// version 1.0 - 23 Apr 2008 -> working version
// modified functions and cross section read-in functions
// added all reactions considered in original MENATE including:
// n+H -> n+p elastic
// n+12C -> n+12C elastic
// n+12C -> n'+12C+gamma(4.4MeV)
// n+12C -> a+9Be
// n+12C -> n'+ 3a
// n+12C -> p + 12B
// n+12C -> n' + p + 11B (breakup)
// n+12C -> n' + n + 11C (breakup)
// All reactions considered together for MeanFreePath and
// reaction probability consideration
// MeanFreePath considers only total reaction probability
// PostStepDoIt chooses reaction
// **** This version includes fix in "Available_Eng" for
// Alpha2 in process 12C(n,n')3a (bug in FORTRAN version of MENATE)
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// version 1.1 - 24 Apr 2008 -> working version
// added function "ShareGammaEngC12()" to kinematics for 12C(n,n'g)12C*
// process as in original MENATE FORTRAN code. This change effects NE213
// efficiency in DEMON module between about 5-11 MeV, making the result
// of the GEANT+MENATE efficiency calc. closer to that obtained by original
// MENATE code. Efficiency below 5 MeV in GEANT code not changed.
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// 25 April 2008 - version 1.2 BTR
// ---- Add Normalization of Momentum ThreeVector in GenMomDir to avoid
// G4 Error Message. Noted that Norm diff from 1 is always less then 1e-7, so 
// error is probably due to slight rounding error.
//-------------------------------------------------------------------------
// 28 April 2008 - version 1.2b BTR
// ---- Added getenv command to ReadCrossSectionFile() Function
// Program looks for environment variable MENATEG4XS to find cross sections
// for use with menate.cc, otherwise throws exception.
//-------------------------------------------------------------------------
// 29 April 2008 - version 1.3 BTR
// --- Noted that MeanFreePath and reaction selection calculations are 
// different from method used in previous version. Added function
// SetMeanFreePathCalcMethod() to switch between calc in previous
// version and that method used specifically by MENATE
// Modified GetMeanFreePath() and ChooseReaction() part of PostStepDoIt to add
// MENATE original method.
//---------------------------------------------------------------------------
// 6 May 2008 - version 1.4 BTR
// --- Begin modification to original MENATE code. Remove parts with no sense
// (such as 400 keV gammas from 12C). Include angular distributions.
//
// 19 May 2008 - Finished revisions to version 1.4 BTR
// --- Added parameterizations of angular distributions for n+p elastic
// and n+12C elastic. (n+p, DEMONS), (n+12C, modified DEMONS). 
// Now using ENDF cross sections for n+p and n+12C elastic
// scattering. Extended total cross sections out to 999 MeV using total cross
// sections from DEMONS (Los Alamos) program. 
// Note : This version leaves the inelastic 12C(n,n') processes as isotropic.
// in the center of mass cross section.
// -------------------------------------------------------------------------- 
// 2011-2012 Zach Kohley, see .hh file for detials of modification and addition
// of Al and Fe material
//--------------------------------------------------------------------------
//
// 6 Sept 2018
// Added to NPTool package by Greg Christian (TAMU), gchristian@tamu.edu
// Code inherited from MoNA-simulation package,
// https://github.com/baumann3141/MoNA-simulation
//
// See menate_R.cc for list of changes made w.r.t. the
// MoNA-simulation version

#ifndef menate_R_hh 
#define menate_R_hh

#include "globals.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ios.hh"
#include "Randomize.hh" 
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4ThreeVector.hh"
#include "G4LorentzVector.hh"
#include "G4VParticleChange.hh"

#include "G4Material.hh"
#include "G4UnitsTable.hh"

#include "TH1F.h"

#ifndef CS_Class
#define CS_Class

class CrossSectionClass
{
private:
  // Data Members - each data member has one part of datafile!
 
  G4String ElementName;

  G4int NumberOfLines;
  G4double KinEng;
  G4double TotalCrossSection;

public:

  // constructor
  CrossSectionClass()
  {
   NumberOfLines = 0;
   ElementName = "Nothing";
   KinEng = 0;
   TotalCrossSection = 0;
  }

  // destructor
  ~CrossSectionClass() 
  {;}

public:
  // Setting and Access Functions

  void SetNumberOfLines(G4int Lines)
  { NumberOfLines = Lines; }

  void SetElementName(G4String ElName)
  { ElementName = ElName; }

  void SetKinEng(G4double Energy)
  { KinEng = Energy; }

  void SetTotalCrossSection(G4double CrossSection)
  { TotalCrossSection = CrossSection; }

  G4int GetNumberOfLines()
  { return NumberOfLines; }

  G4String GetElementName()
  { return ElementName; }

  G4double GetKinEng()
  { return KinEng; }

  G4double GetTotalCrossSection()
  { return TotalCrossSection; } 

  void DumpData()
  {
    G4cout << "Number of Lines Total " << NumberOfLines
           << ", Element Name = " << ElementName
           << ", KinEng = " << KinEng
           << " , CrossSection = " << TotalCrossSection << G4endl;
  }

}; 
#endif 

class menate_R : public G4VDiscreteProcess
{
public:

  // constructor
  menate_R(const G4String& processName="menate_R");

  // destructor
  ~menate_R();

public:
  // These are the functions in the Process: Derived from G4VDiscreteProcess.hh

  G4bool IsApplicable(const G4ParticleDefinition& aParticle);
  // Decides if process applicable or not. Only valid for Neutrons
  // at some energy.

  G4double GetMeanFreePath(const G4Track& aTrack, G4double previousStepSize,
                           G4ForceCondition* condition); 
  // Returns MeanFreePath for a particle in a given material in mm

  G4VParticleChange* PostStepDoIt(const G4Track &aTrack, const G4Step &aStep);
  // This is the important function where you define the process
  // Returns steps, track and secondary particles

  void SetMeanFreePathCalcMethod(G4String Method);

private:

 // Hide assignment operator as private 
  menate_R& operator=(const menate_R &right);

  // Copy constructor
  menate_R(const menate_R&);

  // Chooses Reaction in PostStepDoIt

  G4String ChooseReaction();

  // Other Pre-Defined Class Variables and Functions are below!

  G4double Absolute(G4double Num);
  G4double SIGN(G4double A1, G4double B2);
 
  // Returns Cross Section for a given element, energy
  G4double GetCrossSection(G4double KinEng, CrossSectionClass* theXS);

  void ReadCrossSectionFile(G4String FileName, CrossSectionClass* theXS);

  G4double GetXSInterpolation(G4double KinEng, G4double LEng, G4double HEng, 
                               G4double LXS, G4double HXS);

  // Functions translated from FORTRAN "MENATE" code 

  G4ThreeVector GenMomDir(G4ThreeVector MomIn, G4double theta, G4double phi);

  G4double ShareGammaEngC12(G4double A_Eng);

  G4double Evaporate_Eng(G4double AMass, G4double Avail_Eng);

  // Angular Distribution Generators

  G4double NP_AngDist(G4double NEng);
  G4double NC12_DIFF(G4double NEng);
  void Setup_AlElastic_AngDist(); 
  void Setup_FeElastic_AngDist(); 
  G4double AlElastic_AngDist(G4double NEng); 
  G4double FeElastic_AngDist(G4double NEng); 
 
private:

  // Class Variables

  TH1F *hFe_AngDist_30MeV;
  TH1F *hFe_AngDist_50MeV;
  TH1F *hFe_AngDist_70MeV;
  TH1F *hAl_AngDist_30MeV;
  TH1F *hAl_AngDist_50MeV;
  TH1F *hAl_AngDist_70MeV;



  G4bool H_Switch;
  G4bool C_Switch; 
  G4bool Fe_Switch;
  G4bool Al_Switch;

  // Hold Number Density for H or C
  G4double Num_H;  
  G4double Num_C;
  G4double Num_Fe;
  G4double Num_Al;

  // Variables used in MENATE functions above

  G4double AMass_Material;  // Atomic Mass NE213 (according to MENATE)
  G4String CalcMeth;

  // integers for ShareGammaEngC12 Function
  G4int Num_gamma_4439k;  // Num of 4.4MeV gammas

  G4double Sigma;
  G4double ProbDistPerReaction[12];
  G4double ProbTot;

  CrossSectionClass theHydrogenXS[181];
  CrossSectionClass theCarbonXS[852];

  CrossSectionClass theC12NGammaXS[37];
  CrossSectionClass theC12ABe9XS[32];
  CrossSectionClass theC12NPB12XS[15];

  CrossSectionClass theC12NNPB11XS[10];
  CrossSectionClass theC12N2NC11XS[11];
  CrossSectionClass theC12NN3AlphaXS[35];

  CrossSectionClass theIronElasXS[1040];
  CrossSectionClass theIronNonelasXS[278];

  CrossSectionClass theAlElasXS[380];
  CrossSectionClass theAlNonelasXS[325];


  // for storing current scattering element

  G4String ElementName;
  G4int A;             // Integer Mass of Recoil (from GetN())
  G4int Z;             // Integer Charge of Recoil (from GetZ())

  G4double Pi;
  G4double Two_Pi;

};
#endif
