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
 *	The TGenPhaseSpace from ROOT is used to calculate a phase space decay    *
 *	with flat distribution	                                                 *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   									                                     *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>

// NPSimulation
#include "VEventGenerator.hh"

// NPLib
#include "TInitialConditions.h"

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
	         double   SigmaX         ,
	         double   SigmaY         ,
	         double   SigmaThetaX       ,
	         double	  SigmaPhiY,
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
	   TInitialConditions*	m_InitConditions;
	   
	private: // Beam Parameter
	   double         m_BeamEnergy ;
	   double         m_BeamEnergySpread      ;
	   double         m_SigmaX          ;
	   double         m_SigmaY          ;
	   double         m_SigmaThetaX        ;
	   double         m_SigmaPhiY        ;

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
	   void     ResonanceDecay(	G4double EnergyHeavy    ,
					         	G4double ThetaHeavy     ,
						        G4double PhiHeavy       ,
						        G4double x0             ,
						        G4double y0             ,
						        G4double z0             ,
						        G4Event* anEvent        ,
						        G4ParticleGun* particleGun);

		//	This method return a random Vector of dimension N and magnitude R
		//  The return distribution populate uniformely the surface of the N-Sphere of radius R  
		vector<double>	PhaseSpaceUniformGenerator( int N , double R);

	   void     SetEverything(string    name1          ,         //Beam nuclei
		         string   name2          ,        //Target nuclei
		         string   name3          ,        //Product of reaction
		         string   name4          ,        //Product of reaction
		         double   BeamEnergy        ,        //Beam Energy
		         double   ExcitationEnergy  ,        //Excitation of Heavy Nuclei
		         double   BeamEnergySpread  ,
		         double   SigmaX         ,
		         double   SigmaY         ,
		         double   SigmaThetaX       ,
		         double   SigmaPhiY       ,
		         int      ResonanceDecayZ      ,
		         int      ResonanceDecayA      ,
		         bool  ShootLight        ,
		         bool  ShootHeavy        ,
		         bool  ShootDecayProduct      ,
		         string   Path);                  //Path of the differentiel Cross Section
};

#endif
