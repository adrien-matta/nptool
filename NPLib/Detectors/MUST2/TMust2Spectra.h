#ifndef TMUST2SPECTRA_H
#define TMUST2SPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : march 2011                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Must2                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TMust2Data.h"
#include "TMust2Physics.h"

// ForwardDeclaration
class TMust2Physics ;

class TMust2Spectra:public VSpectra{
  public:
    // constructor and destructor
    TMust2Spectra();
    TMust2Spectra(unsigned int NumberOfTelescope);
    ~TMust2Spectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TMust2Data*);
    void FillPreTreatedSpectra(TMust2Data*);
    void FillPhysicsSpectra(TMust2Physics*);

  private: // Information on MUST2
    unsigned int fNumberOfTelescope;
    unsigned int fStripX;
    unsigned int fStripY;
    unsigned int fPadSili;
    unsigned int fCrystalCsI;
};

#endif
