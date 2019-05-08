#ifndef TMugastSPECTRA_H
#define TMugastSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: matta@lpccaen.in2p3.fr *
 *                                                                           *
 * Creation Date  : February 2019                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Mugast                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TMugastData.h"
#include "TMugastPhysics.h"

// ForwardDeclaration
class TMugastPhysics ;

class TMugastSpectra:public VSpectra{
  public:
    // constructor and destructor
    TMugastSpectra();
    TMugastSpectra(map<int,int> TelescopeIndex);
    ~TMugastSpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TMugastData*);
    void FillPreTreatedSpectra(TMugastData*);
    void FillPhysicsSpectra(TMugastPhysics*);

  private: // Information on Mugast
    map<int,int> fTelescopeToIndex;
    map<int,int> fIndexToTelescope;
    unsigned int fNumberOfTelescope;
    unsigned int fStripX;
    unsigned int fStripY;
    unsigned int fStripSecondLayer;
};

#endif
