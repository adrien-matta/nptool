#ifndef __NUCLEUS__
#define __NUCLEUS__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class manage a nucleus, data are read in the nubtab03.asc file      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// ROOT headers
#include "TLorentzVector.h"

// NPTOOL headers
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"

#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

#ifdef HEP_PHYSICAL_CONSTANTS_H
using namespace CLHEP;
#endif

// C++ headers
#include <string>
using namespace std;

#include <iostream>
namespace NPL {
  class Nucleus {
    
  public:
    Nucleus();
    Nucleus(string isotope);
    Nucleus(string isotope, const string& pathENSDF);
    Nucleus(int Z, int A);
    Nucleus(string name, vector<string> subpart, double binding,double Ex=0, string SpinParity="", double Spin=0, string Parity="", double LifeTime=-1);
    ~Nucleus();
  
  public:
    void SetUp(string isotope);
    void LoadENSDF(const string& isotope, const string& pathENSDF);
    
  private :
    //intrinsic properties
    string fName;         // Nucleus name
    string fNucleusName;
    int    fCharge;       // Nucleus charge
    int    fAtomicWeight; // Nucleus atomic weight
    double fMassExcess;   // Nucleus mass excess in keV
    string fSpinParity;   // Nucleus spin and parity
    double fSpin;         // Nucleus spin
    string fParity;       // Nucleus parity
    double fLifeTime;     // life time
    double fExcitationEnergy; // Excitation Energy
    //kinematic properties
    double fKineticEnergy;
    double fBeta;
    double fGamma;
    double fBrho;
    double fTimeOfFlight;
    double fVelocity;
    TLorentzVector fEnergyImpulsion;
    // ENSDF list of levels
    vector<double> fLevelEnergy;             // list of level energies
    vector<double> fLevelEnergyUncertainty;  // list of level energies uncertainty
    
  public:
    void EnergyToBrho(double Q=-1000);
    void EnergyToTof();
    void	BetaToVelocity();
    void BrhoToEnergy(double Q=-1000);
    void BrhoToTof()    {BrhoToEnergy(); EnergyToTof();}
    void	TofToEnergy();
    void	TofToBrho()    {TofToEnergy(); EnergyToBrho();}
    void	EnergyToBeta();
    void	BetaToEnergy();
    void	BetaToGamma();
    double DopplerCorrection(double EnergyLabGamma, double ThetaLabGamma);
    
    
  protected :
    void Extract(string line);
    
    public :
    void				   GetNucleusName();
    string			   GetName()			      const	{return fNucleusName;}
    int			  	   GetZ()				      const	{return fCharge;}
    int			  	   GetA()				      const	{return fAtomicWeight;}
    double			   GetMassExcess()		   const {return fMassExcess;}
    string   		   GetSpinParity()		   const	{return fSpinParity;}
    double			   GetSpin()			      const	{return fSpin;}
    string   		   GetParity()			      const	{return fParity;}
    double           GetLifeTime()           const {return fLifeTime;}
    double			   GetEnergy()			      const	{return fKineticEnergy;}
    double			   GetBrho()			      const	{return fBrho;}
    double			   GetTimeOfFlight()	      const {return fTimeOfFlight;}
    double			   GetBeta()			      const	{return fBeta;}
    double			   GetGamma()			      const	{return fGamma;}
    double			   GetVelocity()	   	   const	{return fVelocity;}
    TLorentzVector	GetEnergyImpulsion()    const {return fEnergyImpulsion;}
    double           GetExcitationEnergy()   const {return fExcitationEnergy;}
    void				SetName(const char* name)	{fName = name;}
    void				SetZ(int charge)					{fCharge = charge;}
    void				SetA(int mass)						{fAtomicWeight = mass;}
    void				SetMassExcess(double massexcess) {fMassExcess = massexcess;}
    void				SetSpinParity(const char* spinparity)	{fSpinParity = spinparity;}
    void				SetSpin(double spin) {fSpin = spin;}
    void				SetParity(const char* parity)	{fParity = parity;}
    void          SetLifeTime(double LifeTime) {fLifeTime=LifeTime;}
    void				SetKineticEnergy(double energy)	{fKineticEnergy = energy; EnergyToBrho(); EnergyToTof(); EnergyToBeta(); BetaToGamma();BetaToVelocity();}
    void				SetBrho(double brho) {fBrho = brho; BrhoToEnergy(); BrhoToTof(); EnergyToBeta(); BetaToGamma();BetaToVelocity();}
    void				SetTimeOfFlight(double tof) {fTimeOfFlight = tof; TofToEnergy(); TofToBrho(); EnergyToBeta(); BetaToGamma();BetaToVelocity();}
    void				SetEnergyImpulsion(TLorentzVector P) 	{fEnergyImpulsion = P;
      fKineticEnergy = P.E() - Mass();
      EnergyToBrho();
      EnergyToTof();
      EnergyToBeta();
      BetaToGamma();
      BetaToVelocity();}
    void SetExcitationEnergy(double Ex) {fExcitationEnergy=Ex;}
    void				SetBeta(double beta)					{fBeta = beta; BetaToGamma(); BetaToEnergy(); EnergyToTof(); EnergyToBrho();BetaToVelocity();}
      double GetEnergyCM(double EnergyLab, double ThetaLab, double PhiLab, double relativisticboost);
      double GetThetaCM(double EnergyLab, double ThetaLab, double PhiLab, double relativisticboost);
    
    // Nuclear mass in MeV
    double      Mass() const {return (fAtomicWeight*amu_c2 + fMassExcess/1000. - fCharge*electron_mass_c2+fExcitationEnergy);}
      double GetBindingEnergy() const {return (fCharge*proton_mass_c2 + (fAtomicWeight-fCharge)*neutron_mass_c2 + fCharge*electron_mass_c2 - fAtomicWeight*amu_c2 - fMassExcess/1000);}
    void        Print() const   ;

  public:
    void DefineMassByThreshold(const vector<string>& v); // Define the mass as the sum of the mass of the particle named in v
    void DefineMassByThreshold(const vector<NPL::Nucleus>& N); // Define the mass as the sum of the mass of the particle defined in N

  public:
    double GetSn() const;
    double GetSp() const;
    double GetS2n() const;
    double GetS2p() const;
    double GetSt() const;
    double GetS3He() const;
    double GetSa() const;
    double GetSXn(unsigned int X) const;
    double GetSXp(unsigned int X) const;
    void   PrintThreshold() const;

    // methods for ENSDF
  public:
    unsigned int    GetNumberOfLevels()               {return fLevelEnergy.size();}
    vector<double>  GetLevelEnergyList()              {return fLevelEnergy;}
    vector<double>  GetLevelEnergyUncertaintyList()   {return fLevelEnergyUncertainty;}
    double          GetLevelEnergy(Int_t i)           {return fLevelEnergy[i];}
    double          GetLevelEnergyUncertainty(Int_t i){return fLevelEnergyUncertainty[i];}
  };
}
#endif
