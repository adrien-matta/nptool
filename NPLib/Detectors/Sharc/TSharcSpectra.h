#ifndef TSHARCSPECTRA_H
#define TSHARCSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 ****************************************************************************/

/*****************************************************************************
 * Original Author: A. Matta                 address: a.matta@surrey.ac.uk   *
 *                                                                           *
 * Creation Date  : June 2015                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Sharc                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TSharcData.h"
#include "TSharcPhysics.h"

// ForwardDeclaration
class TSharcPhysics ;

class TSharcSpectra:public VSpectra {
  public:
    // constructor and destructor
    TSharcSpectra();
    TSharcSpectra(unsigned int NumberOfDetector);
    ~TSharcSpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TSharcData*);
    void FillPreTreatedSpectra(TSharcData*);
    void FillPhysicsSpectra(TSharcPhysics*);

  private: // Information on SHARC
    unsigned int fNumberOfDetector;
    unsigned int fStripFront;
    unsigned int fStripBack;
    unsigned int fPad;
};

#endif
