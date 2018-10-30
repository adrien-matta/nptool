#ifndef Tbeam_dumpSPECTRA_H
#define Tbeam_dumpSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: valerian  contact address: girardalcindor@ganil.fr                        *
 *                                                                           *
 * Creation Date  : November 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold beam_dump Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "Tbeam_dumpData.h"
#include "Tbeam_dumpPhysics.h"

// Forward Declaration
class Tbeam_dumpPhysics;


class Tbeam_dumpSpectra : public VSpectra {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
  public:
    Tbeam_dumpSpectra();
    Tbeam_dumpSpectra(unsigned int NumberOfDetectors);
    ~Tbeam_dumpSpectra();

  //////////////////////////////////////////////////////////////
  // Initialization methods
  private:
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  //////////////////////////////////////////////////////////////
  // Filling methods
  public:
    void FillRawSpectra(Tbeam_dumpData*);
    void FillPreTreatedSpectra(Tbeam_dumpData*);
    void FillPhysicsSpectra(Tbeam_dumpPhysics*);

  //////////////////////////////////////////////////////////////
  // Detector parameters 
  private:
    unsigned int fNumberOfDetectors;
};

#endif
