#ifndef TREACTIONCONDITIONS_H
#define TREACTIONCONDITIONS_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouace@ganil.fr    *
 *                                                                           *
 * Creation Date  : 17/07/18                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class records all the information concerning the beam    *
 *             rwaction generators, e.g. vertex of interaction, angles of    *
 *             emitted particles...                                          *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/



// STL Header
#include <vector>
#include <string>
#include <cmath>
using namespace std ;

// Root Header
#include "TObject.h"
#include "TVector3.h"

// NPTOOL headers
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

class TReactionConditions : public TObject{
private:
    
    // Beam beam parameter
    string fRC_Beam_Particle_Name;
    double fRC_Beam_Emittance_ThetaX;
    double fRC_Beam_Emittance_PhiY;
    double fRC_Beam_Emittance_Theta;
    double fRC_Beam_Emittance_Phi;
    double fRC_Beam_Reaction_Energy;
    double fRC_Vertex_Position_X;
    double fRC_Vertex_Position_Y;
    double fRC_Vertex_Position_Z;
    double fRC_ExcitationEnergy3;
    double fRC_ExcitationEnergy4;
    double fRC_ThetaCM;
    // emmitted particles
    vector<string> fRC_Particle_Name;
    vector<double> fRC_Theta;
    vector<double> fRC_Phi;
    vector<double> fRC_Kinetic_Energy;
    vector<double> fRC_Momentum_Direction_X;
    vector<double> fRC_Momentum_Direction_Y;
    vector<double> fRC_Momentum_Direction_Z;

    
public:
    TReactionConditions();
    virtual ~TReactionConditions();
    
    void  Clear();
    void  Clear(const Option_t*) {Clear();};
    void  Dump() const;
    
    /////////////////////           SETTERS           ////////////////////////
    // Beam parameter
    void SetBeamParticleName   (const string & Beam_Particle_Name)   {fRC_Beam_Particle_Name = Beam_Particle_Name;}//!
    void SetBeamReactionEnergy  (const double & Beam_Reaction_Energy) {fRC_Beam_Reaction_Energy  = Beam_Reaction_Energy;}//!
    void SetBeamEmittanceTheta (const double & Beam_Emittance_Theta) {fRC_Beam_Emittance_Theta = Beam_Emittance_Theta;}//!
    void SetBeamEmittancePhi   (const double & Beam_Emittance_Phi)   {fRC_Beam_Emittance_Phi   = Beam_Emittance_Phi;}//!
    void SetBeamEmittanceThetaX (const double & Beam_Emittance_ThetaX) {fRC_Beam_Emittance_ThetaX = Beam_Emittance_ThetaX;}//!
    void SetBeamEmittancePhiY   (const double & Beam_Emittance_PhiY)   {fRC_Beam_Emittance_PhiY   = Beam_Emittance_PhiY;}//!
    // Beam status at the initial interaction point
    void SetVertexPositionX     (const double & Vertex_Position_X)      {fRC_Vertex_Position_X     = Vertex_Position_X;}//!
    void SetVertexPositionY     (const double & Vertex_Position_Y)      {fRC_Vertex_Position_Y     = Vertex_Position_Y;}//!
    void SetVertexPositionZ     (const double & Vertex_Position_Z)      {fRC_Vertex_Position_Z     = Vertex_Position_Z;}//!
    void SetExcitationEnergy3  (const double& Ex) {fRC_ExcitationEnergy3=Ex;};//!
    void SetExcitationEnergy4  (const double& Ex) {fRC_ExcitationEnergy4=Ex;};//!
    void SetThetaCM            (const double & ThetaCM)               {fRC_ThetaCM = ThetaCM;}//!
    
    // emmitted particles
    void SetParticleName       (const string & Particle_Name)         {fRC_Particle_Name.push_back(Particle_Name);}//!
    void SetTheta              (const double & Angle)                 {fRC_Theta.push_back(Angle);}//!
    void SetPhi              (const double & AnglePhi)                 {fRC_Phi.push_back(AnglePhi);}//!
    void SetKineticEnergy      (const double & Kinetic_Energy)        {fRC_Kinetic_Energy.push_back(Kinetic_Energy);}//!
    void SetMomentumDirectionX (const double & Momentum_Direction_X)  {fRC_Momentum_Direction_X.push_back(Momentum_Direction_X);}//!
    void SetMomentumDirectionY (const double & Momentum_Direction_Y)  {fRC_Momentum_Direction_Y.push_back(Momentum_Direction_Y);}//!
    void SetMomentumDirectionZ (const double & Momentum_Direction_Z)  {fRC_Momentum_Direction_Z.push_back(Momentum_Direction_Z);}//!
    
    /////////////////////           GETTERS           ////////////////////////
    // Beam parameter
    string GetBeamParticleName    () const  {return fRC_Beam_Particle_Name   ;}//!
    double GetBeamEnergy          () const  {return fRC_Beam_Reaction_Energy  ;}//!
    double GetBeamEmittanceTheta  () const  {return fRC_Beam_Emittance_Theta ;}//!
    double GetBeamEmittancePhi    () const  {return fRC_Beam_Emittance_Phi   ;}//!
    double GetBeamEmittanceThetaX () const  {return fRC_Beam_Emittance_ThetaX ;}//!
    double GetBeamEmittancePhiY   () const  {return fRC_Beam_Emittance_PhiY   ;}//!
    
    // Beam status at the initial Beam point
    double GetVertexPositionX     () const {return fRC_Vertex_Position_X     ;}//!
    double GetVertexPositionY     () const {return fRC_Vertex_Position_Y     ;}//!
    double GetVertexPositionZ     () const {return fRC_Vertex_Position_Z     ;}//!
    double GetExcitation3         () const {return fRC_ExcitationEnergy3     ;}//!       
    double GetExcitation4         () const {return fRC_ExcitationEnergy4     ;}//!       
    double GetThetaCM             () const {return fRC_ThetaCM;}//!
    
    // emmitted particles
    int GetParticleMultiplicity()                const {return fRC_Kinetic_Energy.size();}//!
    string GetParticleName        (const int &i) const {return fRC_Particle_Name[i];}//!
    double GetTheta               (const int &i) const {return fRC_Theta[i];}//!
    double GetPhi                 (const int &i) const {return fRC_Phi[i];}//!
    double GetKineticEnergy       (const int &i) const {return fRC_Kinetic_Energy[i];}//!
    double GetMomentumDirectionX  (const int &i) const {return fRC_Momentum_Direction_X[i];}//!
    double GetMomentumDirectionY  (const int &i) const {return fRC_Momentum_Direction_Y[i];}//!
    double GetMomentumDirectionZ  (const int &i) const {return fRC_Momentum_Direction_Z[i];}//!

    TVector3 GetBeamDirection         () const ;
    TVector3 GetParticleDirection     (const int i) const ; 
 
    double GetThetaLab_WorldFrame (const int i) const {
        return (GetParticleDirection(i).Angle(TVector3(0,0,1)))/deg;
    } 
    
    double GetThetaLab_BeamFrame (const int i) const{
        return (GetParticleDirection(i).Angle(GetBeamDirection()))/deg;
    } 
    
    unsigned int GetEmittedMult() const {return fRC_Particle_Name.size();} 
    
    ClassDef(TReactionConditions, 1) // TReactionConditions structure
};

#endif
