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
 * Last update    : 03/11/2009                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  All event generator added in the project should derive from this virtual * 
 *  class.                                                                   *
 *  A VEventGenerator object is manage by the PrimaryGeneratorAction class   *
 *  and call the virtual method of this class implemented in the daughter    *
 *  class object.                                                            *
 *  This inheritance insure homogeneity and modularity of the code           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 03/11/09: Adding EffectiveThiknessBeforeInteraction in the           *
 *                CalculateBeamInteraction() method (N. de Sereville)        *
 *                                                                           *
 *****************************************************************************/
#include "VEventGenerator.hh"

// C++ headers
#include "cmath"

// G4 headers
#include "G4UnitsTable.hh"

// CLHEP headers
#include "Randomize.hh"


NPS::VEventGenerator::VEventGenerator()
{
}



NPS::VEventGenerator::~VEventGenerator()
{
}

