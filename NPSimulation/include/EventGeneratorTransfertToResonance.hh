#ifndef EventGeneratorTransfertToResonance_H
#define EventGeneratorTransfertToResonance_H
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated two body TransfertReaction.    *
 *  A Phase Space calculation is then performed to decay the Heavy product.  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   This class is not yet operationnal.                                     *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>

// NPTool header
#include "VEventGenerator.hh"

// NPLib header
#include "NPReaction.h"

using namespace std;
using namespace NPL ;



class EventGeneratorTransfertToResonance : public VEventGenerator
{
public:     // Constructors and Destructors

   // Default constructor used to allocate memory
   EventGeneratorTransfertToResonance();

   // This is the constructor to be used
   EventGeneratorTransfertToResonance(string   name1          ,         //Beam nuclei
         string   name2          ,        //Target nuclei
         string   name3          ,        //Product of reaction
         string   name4          ,        //Product of reaction
         double   BeamEnergy        ,        //Beam Energy
         double   ExcitationEnergy  ,        //Excitation of Heavy Nuclei
         double   BeamEnergySpread  ,
         double   BeamFWHMX         ,
         double   BeamFWHMY         ,
         double   BeamEmmitanceTheta       ,
         double	  BeamEmmitancePhi,
         int      ResonanceDecayZ      ,
         int      ResonanceDecayA      ,
         bool  ShootLight        ,
         bool  ShootHeavy        ,
         bool  ShootDecayProduct      ,
         string   Path);                  //Path of the differentiel Cross Section

   // Default Destructor
   virtual ~EventGeneratorTransfertToResonance();

public:     // Inherit from VEventGenerator class
   void        ReadConfiguration(string)              ;
   void        GenerateEvent(G4Event*, G4ParticleGun*)       ;

   void        SetTargetCoordinate(G4double X, G4double Y, G4double Z) {
      m_TargetX = X;
      m_TargetY = Y;
      m_TargetZ = Z;
   }

   void        SetTargetThickness(double TargetThickness) {
      m_TargetThickness = TargetThickness ;
   }

   void        SetTargetRadius(double TargetRadius) {
      m_TargetRadius    = TargetRadius    ;
   }

private: // Particle Shoot Option
   bool        m_ShootLight         ;
   bool        m_ShootHeavy         ;
   bool        m_ShootDecayProduct       ;

private: // TTree to store initial value of beam and reaction
   double         m_InitialLightEnergy ;
   double         m_InitialLightTheta     ;
   double         m_InitialBeamEnergy     ;
   double         m_InitialBeamTheta      ;
   double         m_InitialBeamX       ;
   double         m_InitialBeamY       ;
   double		  m_InitialThetaCM		;
   
private: // Beam Parameter
   double         m_BeamEnergySpread      ;
   double         m_BeamFWHMX          ;
   double         m_BeamFWHMY          ;
   double         m_BeamEmmitanceTheta        ;
   double         m_BeamEmmitancePhi        ;

private: // Target Parameter
   double         m_TargetThickness    ;
   double         m_TargetRadius       ;
   double         m_TargetX            ;
   double         m_TargetY            ;
   double         m_TargetZ            ;

private: // Reaction
	Reaction*	m_Reaction				;

private: // Resonance decay channel
   int            m_ResonanceDecayZ    ;
   int            m_ResonanceDecayA    ;

   //Other
   void     Print() const        ;
   void     InitializeRootOutput()  ;
   void     ResonanceDecay(G4int parentZ           ,
         G4int parentA           ,
         G4double EnergyHeavy    ,
         G4double ThetaHeavy        ,
         G4double PhiHeavy       ,
         G4double x0             ,
         G4double y0             ,
         G4double z0             ,
         G4Event* anEvent        ,
         G4ParticleGun* particleGun);


   void     SetEverything(string    name1          ,         //Beam nuclei
         string   name2          ,        //Target nuclei
         string   name3          ,        //Product of reaction
         string   name4          ,        //Product of reaction
         double   BeamEnergy        ,        //Beam Energy
         double   ExcitationEnergy  ,        //Excitation of Heavy Nuclei
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
         string   Path);                  //Path of the differentiel Cross Section
};

#endif
