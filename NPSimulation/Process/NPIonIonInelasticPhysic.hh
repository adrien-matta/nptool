/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address: morfouac@nscl.msu.edu *
 *                                                                           *
 * Creation Date  : August 2016                                              *
 * Last update    : August 2016                                              *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Modular Physics list calling Geant4 reference list                       *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
//
//

#ifndef NPIONINELASTICPHYSIC_H
#define NPIONIONINELASTICPHYSIC_H 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh" 

class NPIonIonInelasticPhysic: public G4VPhysicsConstructor 
{
 public:
  NPIonIonInelasticPhysic (const G4String& name = "npt_ion_ion_inelastic");
   virtual ~NPIonIonInelasticPhysic();

 protected:
   void ConstructParticle(){};
   void ConstructProcess();
};
#endif 








