#ifndef TLABR3SPECTRA_H
#define TLABR3SPECTRA_H
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
 *  This class holds all the online spectra needed for LaBr3              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "../include/VSpectra.h"
#include "TLaBr3Data.h"
#include "TLaBr3Physics.h"

// ForwardDeclaration
class TLaBr3Physics;

class TLaBr3Spectra: public VSpectra {
  public:
    // constructor and destructor
    TLaBr3Spectra();
    TLaBr3Spectra(unsigned int NumberOfTelescope);
    ~TLaBr3Spectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TLaBr3Data*);
    void FillPreTreatedSpectra(TLaBr3Data*);
    void FillPhysicsSpectra(TLaBr3Physics*);

  private: // Information on LaBr3
    unsigned int fNumberDetector;

};

#endif
