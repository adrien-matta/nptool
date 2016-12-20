#ifndef __REACTION__
#define __REACTION__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author : Pierre MORFOUACE contact: morfouac@nscl.msu.edu         *
 *                                                                           *
 * Creation Date   : April 2016                                              *
 * Last update     : April 2016                                              *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class simulate particule with a give energy                         *
 *  and angular distirubtion in the lab.                                     *
 *  Physical parameter (Nuclei mass) are loaded from the nubtab03.asc file   *
 *  (2003 nuclear table of isotopes mass).                                   *
 *                                                                           *
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

// NPL
#include "NPNucleus.h"
#include "NPBeam.h"
#include "NPInputParser.h"
using namespace NPL;

// ROOT header
#include "TLorentzVector.h"
#include "TLorentzRotation.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1F.h"

using namespace std;

namespace NPL{

  class Reaction{

    public:  // Constructors and Destructors
      Reaction();
      // This constructor aim to provide a fast way to instantiate a reaction without input file
      // The string should be of the form "A(b,c)D@E" with E the ernegy of the beam in MeV
      Reaction(string);
      virtual ~Reaction();

    public:  // Various Method
      void ReadConfigurationFile(string Path);
      void ReadConfigurationFile(NPL::InputParser);


    private:
      int fVerboseLevel;

    private: // use for Monte Carlo simulation
      bool fshoot3;
      bool fshoot4;

    private: // use to display the kinematical line
      TGraph* fKineLine3 ;
      TGraph* fKineLine4 ;
      TGraph* fTheta3VsTheta4;
      TGraph* fLineBrho3;
      TGraph* fAngleLine;
    private:
      Beam     fNuclei1;                 // Beam
      Nucleus  fNuclei2;                 // Target
      Nucleus  fNuclei3;                 // Light ejectile
      Nucleus  fNuclei4;                 // Heavy ejectile
      double   fQValue;                  // Q-value in MeV
      double   fBeamEnergy;              // Beam energy in MeV
      double   fThetaCM;                 // Center-of-mass angle in radian
      double   fExcitation3;             // Excitation energy in MeV
      double   fExcitation4;             // Excitation energy in MeV
      TH1F*    fCrossSectionHist;        // Differential cross section in CM frame
      TH2F*    fDoubleDifferentialCrossSectionHist; // Diff. CS CM frame vs Beam E
      TH1F*    fExcitationEnergyHist;    // Distribution of Excitation energy
    public:
      // Getters and Setters
      void     SetBeamEnergy(double eBeam)      {fBeamEnergy = eBeam;     initializePrecomputeVariable();}
      void     SetThetaCM(double angle)         {fThetaCM = angle;        initializePrecomputeVariable();}
      void     SetExcitation3(double exci)      {fExcitation3 = exci; initializePrecomputeVariable();}
      void     SetExcitation4(double exci)      {fExcitation4 = exci; initializePrecomputeVariable();}
      // For retro compatibility
      void     SetExcitationLight(double exci)  {fExcitation3 = exci; initializePrecomputeVariable();}
      void     SetExcitationHeavy(double exci)  {fExcitation4 = exci; initializePrecomputeVariable();}
      void     SetVerboseLevel(int verbose)     {fVerboseLevel = verbose;}
      void     SetCrossSectionHist  (TH1F*  CrossSectionHist)     
        {delete fCrossSectionHist; fCrossSectionHist   = CrossSectionHist;}

      void     SetDoubleDifferentialCrossSectionHist(TH2F* CrossSectionHist) 
        {fDoubleDifferentialCrossSectionHist = CrossSectionHist;}
      double   GetBeamEnergy() const            {return fBeamEnergy;}
      double   GetThetaCM() const               {return fThetaCM;}
      double   GetExcitation3() const           {return fExcitation3;}
      double   GetExcitation4() const           {return fExcitation4;}
      double   GetQValue() const                {return fQValue;}
      Nucleus  GetNucleus1() const              {return fNuclei1;}
      Nucleus  GetNucleus2() const              {return fNuclei2;}
      Nucleus  GetNucleus3() const              {return fNuclei3;}
      Nucleus  GetNucleus4() const              {return fNuclei4;}
      TH1F*    GetCrossSectionHist() const      {return fCrossSectionHist;}
      int      GetVerboseLevel()         const  {return fVerboseLevel;}
      bool     GetShoot3()         const        {return fshoot3;}
      bool     GetShoot4()         const        {return fshoot4;}


    public:
      // Modify the CS histo to so cross section shoot is within ]HalfOpenAngleMin,HalfOpenAngleMax[
      void SetCSAngle(double CSHalfOpenAngleMin,double CSHalfOpenAngleMax);

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
      Double_t pCM_1;
      Double_t pCM_2;
      Double_t pCM_3;
      Double_t pCM_4;

      // Mandelstam variable
      Double_t s;

      // Center of Mass Kinematic
      Double_t BetaCM;

    public:
      TLorentzVector GetEnergyImpulsionLab_1() const {return fEnergyImpulsionLab_1;}
      TLorentzVector GetEnergyImpulsionLab_2() const {return fEnergyImpulsionLab_2;}
      TLorentzVector GetEnergyImpulsionLab_3() const {return fEnergyImpulsionLab_3;}
      TLorentzVector GetEnergyImpulsionLab_4() const {return fEnergyImpulsionLab_4;}


    public: // Kinematics
      // Check that the reaction is alowed
      bool CheckKinematic();

      // Use fCrossSectionHist to shoot a Random ThetaCM and set fThetaCM to this value
      double ShootRandomThetaCM();

      // Use fExcitationEnergyHist to shoot a Random Excitation energy and set fExcitation4 to this value
      void ShootRandomExcitationEnergy();

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

      TGraph* GetKinematicLine3(double AngleStep_CM=1);
      TGraph* GetKinematicLine4(double AngleStep_CM=1);
      TGraph* GetBrhoLine3(double AngleStep_CM=1);
      TGraph* GetThetaLabVersusThetaCM(double AngleStep_CM=1);
      TGraph* GetELabVersusThetaCM(double AngleStep_CM=1);
      TGraph* GetTheta3VsTheta4(double AngleStep_CM=1);
      void PrintKinematic();
      double	GetP_CM_1()	{return pCM_1;}
      double	GetP_CM_2()	{return pCM_2;}
      double	GetP_CM_3()	{return pCM_3;}
      double	GetP_CM_4()	{return pCM_4;}
      double	GetE_CM_1()	{return ECM_1;}
      double	GetE_CM_2()	{return ECM_2;}
      double	GetE_CM_3()	{return ECM_3;}
      double	GetE_CM_4()	{return ECM_4;}

      // calculate total cross section
      Double_t GetTotalCrossSection() const;

      // Print private paremeter
      void Print() const;

      ClassDef(Reaction,0)

  };
}
#endif
