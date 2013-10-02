#ifndef EventGeneratorTransfert_H
#define EventGeneratorTransfert_H

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
		             double BeamEmmitanceTheta       ,
		             double BeamEmmitancePhi  ,
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
		   double         m_BeamFWHMX        ;
		   double         m_BeamFWHMY        ;
		   double         m_BeamEmmitanceTheta      ;
		   double         m_BeamEmmitancePhi      ;

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
		             double BeamEmmitanceTheta       ,
		             double BeamEmmitancePhi  ,
		             bool   ShootLight        ,
		             bool   ShootHeavy        ,
		             string Path);                     // Path of the differentiel Cross Section
};

#endif
