#ifndef __REACTION__
#define __REACTION__
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: matta@ipno.in2p3.fr   *
 *                                                                           *
 * Creation Date   : March 2009                                              *
 * Last update     : January 2011                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class deal with Two Body transfert Reaction                         *
 *  Physical parameter (Nuclei mass) are loaded from the nubtab03.asc file   *
 *  (2003 nuclear table of isotopes mass).                                   *
 *                                                                           *
 *  KineRelativistic: Used in NPSimulation                                   *
 *  A relativistic calculation is made to compute Light and Heavy nuclei     *
 *  angle given the Theta CM angle.                                          *
 *                                                                           *
 *  ReconstructRelativistic: Used in NPAnalysis                              *
 *  A relativistic calculation is made to compute Excitation energy given the*
 *  light angle and energy in Lab frame.                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 20/01/2011: Add support for excitation energy for light ejectile     *
 *                  (N. de Sereville)                                        *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>

#include "NPNucleus.h"

// ROOT header
#include "TLorentzVector.h"
#include "TLorentzRotation.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH2F.h"

using namespace std;

namespace NPL
{
   class Reaction
   {

      public:  // Constructors and Destructors
         Reaction();
         Reaction(string name1, string name2, string name3, string name4, double BeamEnergy , double ExcitationEnergyLight, double ExcitationEnergyHeavy, string Path, double CSThetaMin = 0, double CSThetaMax = 180);
         ~Reaction();

      public:  // Various Method
         void SetEveryThing(string name1, string name2, string name3, string name4, double BeamEnergy, double ExcitationEnergyLight, double ExcitationEnergyHeavy, string Path, double CSThetaMin, double CSThetaMax);
         void ReadConfigurationFile(string Path);

      private:
         Nucleus *fNuclei1;                 // Beam
         Nucleus *fNuclei2;                 // Target
         Nucleus *fNuclei3;                 // Light ejectile
         Nucleus *fNuclei4;                 // Heavy ejectile
         double   fQValue;                  // Q-value in MeV
         double   fBeamEnergy;              // Beam energy in MeV
         double   fThetaCM;                 // Center-of-mass angle in radian
         double   fExcitation3;             // Excitation energy in MeV
         double   fExcitation4;             // Excitation energy in MeV
         double*  fCrossSection;             // Differential CrossSection
         int      fCrossSectionSize;         // Size of array containing Differention CrossSection
         double   fCrossSectionAngleMin;    // Minimum angle of the differential cross-section given by the user
         double   fCrossSectionAngleMax;    // Maximum angle of the differential cross-section given by the user

      public:
         // Getters and Setters
         void     SetBeamEnergy(double eBeam)      {fBeamEnergy = eBeam;     initializePrecomputeVariable();}
         void     SetThetaCM(double angle)         {fThetaCM = angle;        initializePrecomputeVariable();}
         void     SetExcitation3(double exci)      {fExcitation3 = exci; initializePrecomputeVariable();}
         void     SetExcitation4(double exci)      {fExcitation4 = exci; initializePrecomputeVariable();}
         double   GetBeamEnergy() const            {return fBeamEnergy;}
         double   GetThetaCM() const               {return fThetaCM;}
         double   GetExcitation3() const           {return fExcitation3;}
         double   GetExcitation4() const           {return fExcitation4;}
         double   GetQValue() const                {return fQValue;}
         Nucleus* GetNucleus1() const              {return fNuclei1;}
         Nucleus* GetNucleus2() const              {return fNuclei2;}
         Nucleus* GetNucleus3() const              {return fNuclei3;}
         Nucleus* GetNucleus4() const              {return fNuclei4;}
         double*  GetCrossSection() const          {return fCrossSection;}
         int      GetCrossSectionSize() const      {return fCrossSectionSize;}
         double   GetCrossSectionAngleMin() const  {return fCrossSectionAngleMin;}
         double   GetCrossSectionAngleMax() const  {return fCrossSectionAngleMax;}

	    	   
	private: // intern precompute variable
	   void initializePrecomputeVariable();
	   double m1;
	   double m2;
	   double m3;
	   double m4;
	   
	   // Lorents Vector
	   TLorentzVector fEnergyImpulsionLab_1;
	   TLorentzVector fEnergyImpulsionLab_2;	
	   TLorentzVector fEnergyImpulsionLab_3;
	   TLorentzVector fEnergyImpulsionLab_4;
	   TLorentzVector fTotalEnergyImpulsionLab;
	   
	   TLorentzVector fEnergyImpulsionCM_1;
	   TLorentzVector fEnergyImpulsionCM_2;	
	   TLorentzVector fEnergyImpulsionCM_3;
	   TLorentzVector fEnergyImpulsionCM_4;
	   TLorentzVector fTotalEnergyImpulsionCM;
	   
	   // Impulsion Vector3
	   TVector3 fImpulsionLab_1;
	   TVector3 fImpulsionLab_2;
	   TVector3 fImpulsionLab_3;
	   TVector3 fImpulsionLab_4;
	   
	   TVector3 fImpulsionCM_1;
	   TVector3 fImpulsionCM_2;
	   TVector3 fImpulsionCM_3;
	   TVector3 fImpulsionCM_4;
	   
	   // CM Energy composante & CM impulsion norme
	   Double_t ECM_1;
	   Double_t ECM_2;
	   Double_t ECM_3;
	   Double_t ECM_4;
	   Double_t pCM_3;
	   Double_t pCM_4;
	   
	   // Mandelstam variable
	   Double_t s;
	   
	   // Center of Mass Kinematic
	   Double_t BetaCM;
	   

      public: // Kinematics
	   // Check that the reaction is alowed
	   bool CheckKinematic();

	   // Compute ThetaLab and EnergyLab for product of reaction
	   void KineRelativistic(double &ThetaLab3, double &KineticEnergyLab3,
							 double &ThetaLab4, double &KineticEnergyLab4);

	   // Return Excitation Energy
	   double ReconstructRelativistic(double EnergyLab, double ThetaLab);
	   

	   // Return ThetaCM
	   // EnergyLab: energy measured in the laboratory frame
	   // ExcitationEnergy: excitation energy previously calculated.
	   double EnergyLabToThetaCM(double EnergyLab, double ThetaLab);
	   
	   void SetNuclei3(double EnergyLab, double ThetaLab);
	   
	   
	   TGraph* GetKinematicLine3();
	   TGraph* GetKinematicLine4();
	   TGraph* GetBrhoLine3();
	   TGraph* GetThetaLabVersusThetaCM();
	   void PrintKinematic();

	   // Print private paremeter
	   void Print() const;
   };
}
#endif
