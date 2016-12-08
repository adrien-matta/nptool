#ifndef EventGeneratorTwoBodyReaction_H
#define EventGeneratorTwoBodyReaction_H
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : January 2013                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated two body two body reaction.    *
 *  A Relativistic computation is performed to determine angle and energy of *
 *   the different particle, knowing the ThetaCM angle given by a cross      *
 *   section shoot. Eleastic scattering can also be simulated.               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 20/01/2011: Add support for excitation energy for light ejectile     *
 *                  (N. de Sereville)                                        *
 *    + 23/01/2013: Class change name (ild name EventGeneratorTransfert)     *
 *                  (A. MATTA)                                               *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <string>

// NPSimulation
#include "VEventGenerator.hh"
#include "Target.hh"
#include "Particle.hh"
#include "ParticleStack.hh"

// NPLib header
#include "NPReaction.h"

using namespace std;
using namespace CLHEP;
using namespace NPL ;


class EventGeneratorTwoBodyReaction : public NPS::VEventGenerator
{
   public:     // Constructors and Destructors
      // Default constructor used to allocate memory
      EventGeneratorTwoBodyReaction();

      // Default Destructor
      virtual ~EventGeneratorTwoBodyReaction();


   public: // Inherit from VEventGenerator class
      void ReadConfiguration(NPL::InputParser);
      void GenerateEvent(G4Event*);
      void SetTarget(Target* Target) ;


   private: // Particle Shoot Option
      ParticleStack* m_ParticleStack;
      bool m_ShootLight;
      bool m_ShootHeavy;


   private: // Target Parameter
      Target* m_Target;


   private: // Reaction and CrossSection Shoot
      Reaction* m_Reaction;


   private: // Beam Parameters
        string m_BeamName;

      // Other methods
      void Print() const;
      void InitializeRootOutput();
};

#endif
