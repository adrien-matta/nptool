#ifndef TSPLITPOLESPECTRA_H
#define TSPLITPOLESPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 ****************************************************************************/

/*****************************************************************************
 * Original Author: n. de Sereville        address: deserevi@ipno.in2p3.fr   *
 *                                                                           *
 * Creation Date  : November 2015                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for SplitPole             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TSplitPoleData.h"
#include "TSplitPolePhysics.h"

// ForwardDeclaration
class TSplitPolePhysics;


class TSplitPoleSpectra: public VSpectra {
  public:
    // constructor and destructor
    TSplitPoleSpectra();
    ~TSplitPoleSpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TSplitPoleData*);
    void FillPreTreatedSpectra(TSplitPoleData*);
    void FillPhysicsSpectra(TSplitPolePhysics*);
};

#endif
