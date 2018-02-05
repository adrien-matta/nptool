/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : Octobre 2017                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Use to kill the beam track and replace it with the reaction product       *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#ifndef BeamReaction_h
#define BeamReaction_h

#include "G4VFastSimulationModel.hh"
#include "PhysicsList.hh"
#include "NPReaction.h"
class G4VPhysicalVolume;
namespace NPS{
  class BeamReaction : public G4VFastSimulationModel{
    public:
      BeamReaction (G4String, G4Region*);
      BeamReaction (G4String);
      ~BeamReaction ();

    public:
      void ReadConfiguration();
      virtual G4bool IsApplicable(const G4ParticleDefinition&);
      virtual G4bool ModelTrigger(const G4FastTrack &);
      virtual void DoIt(const G4FastTrack&, G4FastStep&);
 
    private:
      NPL::Reaction m_Reaction;
      string m_BeamName;
      double m_PreviousEnergy;
      double m_PreviousLength;
  };
}


#endif 
