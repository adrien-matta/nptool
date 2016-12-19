/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : December 2016                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Class to simulate electron drift in a gas detector such as Ionisation     *
 * chamber and TPC                                                           *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#ifndef ElectronDrift_h
#define ElectronDrift_h

#include "G4VFastSimulationModel.hh"
#include "PhysicsList.hh"

class G4VPhysicalVolume;

class DriftElectron : public G4VFastSimulationModel{
  public:
    DriftElectron (G4String, G4Region*);
    DriftElectron (G4String);
    ~DriftElectron ();

    virtual G4bool IsApplicable(const G4ParticleDefinition&);
    virtual G4bool ModelTrigger(const G4FastTrack &);
    virtual void DoIt(const G4FastTrack&, G4FastStep&);

  private:

    PhysicsList* m_PhysicsList;
};


#endif 
