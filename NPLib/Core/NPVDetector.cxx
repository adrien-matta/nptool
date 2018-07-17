/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date   :   June 2009                                             *
 * Last update     :                                                         *
 *---------------------------------------------------------------------------*
 * Decription:  Class VDetector (virtual) for NPAnalysis                     *
 * All detector class used in NPAnalysis should derived from this virtual    *
 *  class. Those VDetector daughter will deal with geometry, calibration and *
 *  basic data treatment.                                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   See MUST2 array for exemple of VDetector derived class                  *
 *                                                                           *
 *****************************************************************************/
#include "NPVDetector.h"
#include "NPOptionManager.h"
using namespace NPL ;

ClassImp(VDetector);

// Constructor
VDetector::VDetector(){
}


// Destructor
VDetector::~VDetector(){
}

////////////////////////////////////////////////////////////////////////////////
namespace NPL{
}

