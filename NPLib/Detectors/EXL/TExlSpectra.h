#ifndef TEXLSPECTRA_H
#define TEXLSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: R. Wilkinson   contact address: r.wilkinson@surrey.ac.uk *
 *                                                                           *
 * Creation Date  : June 2017                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for EXL                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TExlData.h"
#include "TExlPhysics.h"

// ForwardDeclaration
class TExlPhysics ;

class TExlSpectra:public VSpectra{
  public:
    // constructor and destructor
    TExlSpectra();
    TExlSpectra(unsigned int NumberOfDetector);
    ~TSpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TExlData*);
    void FillPreTreatedSpectra(TExlData*);
    void FillPhysicsSpectra(TExlPhysics*);

  private: // Information on EXL
    unsigned int fNumberOfDetector;
    unsigned int fNumberOfCrystal;
};

#endif
