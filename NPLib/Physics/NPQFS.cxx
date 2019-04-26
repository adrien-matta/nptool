/*****************************************************************************
 * Copyright (C) 2009-2019   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  F. Flavigny contact address: flavigny@ipno.in2p3.fr    *
 *                                                                           *
 * Creation Date   : April 2019                                              *
 * Last update     : April 2019                                              *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class deal with Quasi Free Scattering Reaction in which a cluster   *
 *  or a nucleon is removed from a projectile  by interaction with a target  *
 *  nucleon (proton target in general)                                       *
 *                                                                           *
 * Labeling is:                                                              *
 *              A --> i  ==> B + (i -> a) =>  B + 1 + 2                      *
 *                                                                           *
 *  where:                                                                   *
 *    +  A is the beam nucleus                                               *
 *    +  i is the target nucleon (proton)                                    *
 *                                                                           *
 *    +  B is the residual fragment (beam-like)                              *
 *    +  1 is the scattered target nucleon  (former i)                       *
 *    +  2 is the knocked-out cluster/nucleon (noted a) in the intermediate  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    +                                                                      *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <vector>

#include "NPQFS.h"
#include "NPCore.h"
#include "NPOptionManager.h"
#include "NPFunction.h"

// Use CLHEP System of unit and Physical Constant
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

// ROOT
#include"TF1.h"

ClassImp(QFS)
/*
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
QFS::QFS(){
    fVerboseLevel=0;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
QFS::~QFS(){
}
*/

