#ifndef TW1SPECTRA_H
#define TW1SPECTRA_H
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
 *  This class holds all the online spectra needed for W1                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TW1Data.h"
#include "TW1Physics.h"

// ForwardDeclaration
class TW1Physics;


class TW1Spectra:public VSpectra {
  public:
    // constructor and destructor
    TW1Spectra();
    TW1Spectra(unsigned int NumberOfDetectors);
    ~TW1Spectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TW1Data*);
    void FillPreTreatedSpectra(TW1Data*);
    void FillPhysicsSpectra(TW1Physics*);

  private: // Information on W1 
    unsigned int fNumberOfDetectors;
    unsigned int fNumberOfStripsFront;
    unsigned int fNumberOfStripsBack;
};

#endif
