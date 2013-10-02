#ifndef EventGeneratorTransfert_H
#define EventGeneratorTransfert_H
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
 *  A Relativistic computation is performed to determine angle and energy of *
 *   the different particle, knowing the ThetaCM angle given by a cross      *
 *   section shoot. Eleastic scattering can also be simulated.               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <string>

// NPTool headers
#include "VEventGenerator.hh"
#include "TInitialConditions.h"

// NPLib header
#include "NPReaction.h"

using namespace std;
using namespace NPL ;


class EventGeneratorTransfert : public VEventGenerator
	{

		public:     // Constructors and Destructors
		   // Default constructor used to allocate memory
		   EventGeneratorTransfert();

		   // This is the constructor to be used
		   EventGeneratorTransfert(string name1             ,        // Beam nuclei
		             string name2             ,        // Target nuclei
		             string name3             ,        // Product of reaction
		             string name4             ,        // Product of reaction
		             double BeamEnergy        ,        // Beam Energy
		             double ExcitationEnergy  ,        // Excitation of Heavy Nuclei
		             double BeamEnergySpread  ,
		             double BeamFWHMX         ,
		             double BeamFWHMY         ,
		             double SigmaThetaX       ,
		             double SigmaPhiY  ,
		             bool   ShootLight        ,
		             bool   ShootHeavy        ,
		             string Path);                     // Path of the differentiel Cross Section

		   // Default Destructor
		   virtual ~EventGeneratorTransfert();


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
		   bool        m_ShootLight   ;
		   bool        m_ShootHeavy   ;

		private: // TTree to store initial value of beam and reaction
		   TInitialConditions*	m_InitConditions;

		private: // Beam Parameter
		   double         m_BeamEnergy ;
		   double         m_BeamEnergySpread ;
		   double         m_SigmaX        ;
		   double         m_SigmaY        ;
		   double         m_SigmaThetaX      ;
		   double         m_SigmaPhiY      ;

		private: // Target Parameter
		   double         m_TargetThickness ;
		   double         m_TargetRadius    ;
		   double         m_TargetX         ;
		   double         m_TargetY         ;
		   double         m_TargetZ         ;

		private: // Reaction and CrossSection Shoot
			Reaction*	m_Reaction			;
		
	   //Other
	   void     Print() const  ;
	   void            InitializeRootOutput();

	   void     SetEverything(string name1             ,        // Beam nuclei
		             string name2             ,        // Target nuclei
		             string name3             ,        // Product of reaction
		             string name4             ,        // Product of reaction
		             double BeamEnergy        ,        // Beam Energy
		             double ExcitationEnergy  ,        // Excitation of Heavy Nuclei
		             double BeamEnergySpread  ,
		             double BeamFWHMX         ,
		             double BeamFWHMY         ,
		             double SigmaThetaX       ,
		             double SigmaPhiY  ,
		             bool   ShootLight        ,
		             bool   ShootHeavy        ,
		             string Path);                     // Path of the differentiel Cross Section
};

#endif
