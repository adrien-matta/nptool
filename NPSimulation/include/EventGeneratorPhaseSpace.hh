#ifndef EventGeneratorPhaseSpace_H
#define EventGeneratorPhaseSpace_H
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : Feb 2010                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated pure phase space event and     *
 *  especially evaluate the background contribution of different phase space * 
 *  channel during an experiment. Manage up to 18 body phase space           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   					                                     													 *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>

// G4 headers
#include "G4ParticleDefinition.hh"

// NPSimulation
#include "VEventGenerator.hh"
#include "Target.hh"

// NPLib
#include "TInitialConditions.h"

using namespace std;

class EventGeneratorPhaseSpace : public VEventGenerator
{
	public:     // Constructors and Destructors

	   // Default constructor used to allocate memory
	   EventGeneratorPhaseSpace();

	   // This is the constructor to be used
	   EventGeneratorPhaseSpace(	G4ParticleDefinition*   				BeamParticle   						,
  															G4ParticleDefinition*   				TargetParticle 						,
		         										vector<G4ParticleDefinition*>		ReactionProducts					,
		         										double   												BeamEnergy       					,      
		         										double  	 											BeamEnergySpread 					,
		         										double   												SigmaX         						,
		         										double   												SigmaY         						,
		         										double   												SigmaThetaX      					,
		         										double   												SigmaPhiY       					);

	   // Default Destructor
	   virtual ~EventGeneratorPhaseSpace();

	public:     // Inherit from VEventGenerator class
	   void        ReadConfiguration(string)              ;
	   void        GenerateEvent(G4Event*, G4ParticleGun*)       ;
	   void        SetTarget(Target* Target);

	private: // Particle Shoot Option
	   bool        m_ShootLight         ;
	   bool        m_ShootHeavy         ;
	   bool        m_ShootDecayProduct  ;

	private: // Value to store in the output tree : initial value of beam and reaction and phase space event weight
		TInitialConditions*	m_InitConditions;
		//	When the Phase Space Generator is called, the weight of the current configuration is return and stored in this variable
		//	Spectrum then need to be weighted by this paramater to be realistic
		//	NB: This procedure avoid long calculation time of the rejection methods previously instantiate and therefore allow simulation of manybody phase space decay 	 	
		double 			m_EventWeight				;

	private: // Target Parameter
    	Target*        m_Target;

	private: //	Experimental Parameter
		G4ParticleDefinition* 	  			m_BeamParticle   						;
		G4ParticleDefinition*   				m_TargetParticle 						;
		double   												m_BeamEnergy        				;      
		double  	 											m_BeamEnergySpread  				;
		double   												m_SigmaX         						;
		double   												m_SigmaY         						;
		double   												m_SigmaThetaX    					  ;
		double   												m_SigmaPhiY       					;
		vector<G4ParticleDefinition*> 	m_ReactionProducts 					;

	private:   //Other
	   void     InitializeRootOutput()  ;

	   void     SetEverything(	G4ParticleDefinition*   				BeamParticle   		,
		         									G4ParticleDefinition*   				TargetParticle 		,
		         									vector<G4ParticleDefinition*>		ReactionProducts	,
		         									double   												BeamEnergy        ,      
		         									double  	 											BeamEnergySpread  ,
		         									double   												SigmaX         		,
		         									double   												SigmaY         		,
		         									double   												SigmaThetaX       ,
		         									double   												SigmaPhiY       	);
};

#endif
