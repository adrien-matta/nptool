#ifndef __Beam__
#define __Beam__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA  contact address: matta@lpccaen.in2p3.fr  *
 *                                                                           *
 * Creation Date   : January 2013                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class deal with Beam:                                               *
 * User can enter various parameter, such as emittance or use ASCII or root  *
 * TH1F distribution                                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>

// ROOT header
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TRandom.h"

using namespace std;

// NPL header
#include "NPNucleus.h"
#include "NPInputParser.h"
using namespace NPL;

namespace NPL{
  
  class Beam:public NPL::Nucleus{
    
  public:  // Constructors and Destructors
    Beam();
    Beam(string);
    ~Beam();
    
  public:  // Various Method
    void ReadConfigurationFile(string Path);
    void ReadConfigurationFile(NPL::InputParser);

  private:
    int fVerboseLevel;
  
  private:
    //Nucleus* fBeamNucleus;
    double fEnergy;
    double fExcitationEnergy;
    double fSigmaEnergy;
    double fMeanX;
    double fMeanY;
    double fSigmaX;
    double fSigmaY;
    double fMeanThetaX;
    double fMeanPhiY;
    double fSigmaThetaX;
    double fSigmaPhiY;
    
    // case of user given distribution
    TH1F* fEnergyHist;
    TH2F* fXThetaXHist;
    TH2F* fYPhiYHist;
    
  public:
    // Getters and Setters
    // Set
    // void SetBeamNucleus (Nucleus* BeamNucleus)  {delete fBeamNucleus ; fBeamNucleus = new Nucleus(BeamNucleus->GetZ(),BeamNucleus->GetA());}
    void SetEnergy      (double Energy)         {fEnergy=Energy;}
    void SetSigmaEnergy (double SigmaEnergy)    {fSigmaEnergy=SigmaEnergy;}
    void SetMeanX       (double MeanX)          {fMeanX=MeanX;}
    void SetMeanY       (double MeanY)          {fMeanY=MeanY;}
    void SetSigmaX      (double SigmaX)         {fSigmaX=SigmaX;}
    void SetSigmaY      (double SigmaY)         {fSigmaY=SigmaY;}
    void SetMeanThetaX  (double MeanThetaX)     {fMeanThetaX=MeanThetaX;}
    void SetMeanPhiY    (double MeanPhiY)       {fMeanPhiY=MeanPhiY;}
    void SetSigmaThetaX (double SigmaThetaX)    {fSigmaThetaX=SigmaThetaX;}
    void SetSigmaPhiY   (double SigmaPhiY)      {fSigmaPhiY=SigmaPhiY;}
    void SetEnergyHist  (TH1F*  EnergyHist)     {delete fEnergyHist; fEnergyHist   = EnergyHist;}
    void SetXThetaXHist (TH2F*  XThetaXHist)    {delete fXThetaXHist; fXThetaXHist = XThetaXHist;}
    void SetYPhiYHist   (TH2F*  YPhiYHist)      {delete fYPhiYHist; fYPhiYHist     = YPhiYHist;}
    void SetVerboseLevel(int verbose)           {fVerboseLevel = verbose;}

    // Get
    // Nucleus*  GetNucleus     () const {return fBeamNucleus;}
    double    GetEnergy      () const {return fEnergy;}
    double    GetExcitationEnergy() const {return fExcitationEnergy;}
    double    GetSigmaEnergy () const {return fSigmaEnergy;}
    double    GetMeanX       () const {return fMeanX;}
    double    GetMeanY       () const {return fMeanY;}
    double    GetSigmaX      () const {return fSigmaX;}
    double    GetSigmaY      () const {return fSigmaY;}
    double    GetMeanThetaX  () const {return fMeanThetaX;}
    double    GetMeanPhiY    () const {return fMeanPhiY;}
    double    GetSigmaThetaX () const {return fSigmaThetaX;}
    double    GetSigmaPhiY   () const {return fSigmaPhiY;}
    TH1F*     GetEnergyHist  () const {return fEnergyHist;}
    TH2F*     GetXThetaXHist () const {return fXThetaXHist;}
    TH2F*     GetYPhiYHist   () const {return fYPhiYHist;}
    int      GetVerboseLevel()  const {return fVerboseLevel;}

  private: // Event Generation private variable
    double fTargetSize;
    double fEffectiveTargetSize; // target size has seen from the beam axis
    double fTargetThickness;
    double fEffectiveTargetThickness; // target thickness has seen by the beam
    double fTargetAngle;
    double fTargetZ;
 
  public:
    void SetTargetSize(double TargetSize);
    void SetTargetThickness(double TargetThickness);
    void SetTargetAngle(double TargetAngle);
    void SetTargetZ(double TargetZ) {fTargetZ = TargetZ;}
    double GetTargetSize(){return fTargetSize;}
    double GetTargetThickness(){return fTargetThickness;}
    double GetTargetAngle(){return fTargetAngle;}
    double GetTargetZ() {return fTargetZ;}
    double GetTargetEffectiveThickness(){return fEffectiveTargetThickness;}
    double GetTargetEffectiveTargetSize(){return fEffectiveTargetSize;}

   
  public: // Event Generation
    void GenerateRandomEvent(double& E, double& X, double& Y, double& Z, double& ThetaX, double& PhiY );
  public: // Print private paremeter
    void Print() const;
  };
}
#endif
