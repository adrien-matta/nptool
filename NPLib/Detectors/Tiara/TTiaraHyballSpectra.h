#ifndef TTIARAHYBALLSPECTRA_H
#define TTIARAHYBALLSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : dec 2013                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Tiara/Hyball          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TTiaraHyballData.h"
#include "TTiaraHyballPhysics.h"

// Forward Declaration
class TTiaraHyballPhysics;

class TTiaraHyballSpectra:public VSpectra {
  public:
    // constructor and destructor
    TTiaraHyballSpectra();
    ~TTiaraHyballSpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TTiaraHyballData*);
    void FillPreTreatedSpectra(TTiaraHyballData*);
    void FillPhysicsSpectra(TTiaraHyballPhysics*);

  private: // Information on MUST2
    unsigned int fRingsNumber;
    unsigned int fSectorsNumber;
    unsigned int fWedgesNumber;
};

#endif
