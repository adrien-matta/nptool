#ifndef PhysicsList_h
#define PhysicsList_h 1
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
 *  A quite standard, non-modulable Geant4 PPhysicis list.                   *
 *  Well suited for low energy ions physics.                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * A good improvement should be a modular physicis list in order to deal     *
 * accuratly with different physics cases.                                   *
 *****************************************************************************/
#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class PhysicsList: public G4VUserPhysicsList
{
public:
   PhysicsList();
   ~PhysicsList();
   void MyOwnConstruction();


protected:
   // Construct particle and physics
   void ConstructParticle();
   void ConstructProcess();
   void ConstructDecay();


   void SetCuts();

protected:
// these methods Construct particles
   void ConstructBosons();
   void ConstructLeptons();
   void ConstructMesons();
   void ConstructBaryons();
   void ConstructIons();

protected:
// these methods Construct physics processes and register them
   void ConstructGeneral();
   void ConstructEM();
};

#endif







