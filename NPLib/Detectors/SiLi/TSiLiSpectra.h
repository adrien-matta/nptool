#ifndef TSILI_H
#define TSILI_H
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
 *  This class holds all the online spectra needed for SiLi                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TSiLiData.h"
#include "TSiLiPhysics.h"

// ForwardDeclaration
class TSiLiPhysics;

class TSiLiSpectra:public VSpectra {
  public:
    // constructor and destructor
    TSiLiSpectra();
    TSiLiSpectra(unsigned int NumberOfTelescope);
    ~TSiLiSpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TSiLiData*);
    void FillPreTreatedSpectra(TSiLiData*);
    void FillPhysicsSpectra(TSiLiPhysics*);

  private: // Information on CHARISSA
    unsigned int fNumberDetector;
};

#endif
