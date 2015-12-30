#ifndef TSIRES_H
#define TSIRES_H
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
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
 *  This class holds all the online spectra needed for SiRes                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TSiResData.h"
#include "TSiResPhysics.h"

// ForwardDeclaration
class TSiResPhysics;

class TSiResSpectra: public VSpectra {
  public:
    // constructor and destructor
    TSiResSpectra();
    TSiResSpectra(unsigned int NumberOfTelescope);
    ~TSiResSpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TSiResData*);
    void FillPreTreatedSpectra(TSiResData*);
    void FillPhysicsSpectra(TSiResPhysics*);

  private: 
    unsigned int fNumberDetector;
};

#endif
