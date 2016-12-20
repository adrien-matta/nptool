//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4DriftElectron.hh 67971 2016-12-19 09:29:30Z gcosmo $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History: First implementation by Adrien Matta
//     
// ****************************************************************
//  First impelemenataion as an utility class  A. Matta, 19 december 2016
// ----------------------------------------------------------------

#ifndef G4DriftElectron_h
#define G4DriftElectron_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

// ######################################################################
// ###                         OPTICAL PHOTON                         ###
// ######################################################################

class G4DriftElectron : public G4ParticleDefinition
{
 private:
   static G4DriftElectron* theInstance;

 private:
  G4DriftElectron () {}

 public:
   ~G4DriftElectron (){}

   static G4DriftElectron* Definition();
   static G4DriftElectron* DriftElectronDefinition();
   static G4DriftElectron* DriftElectron();

};

#endif

