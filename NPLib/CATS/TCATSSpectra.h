#ifndef TCATSSPECTRA_H
#define TCATSSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: A. Matta         contact address: a.matta@surrey.ac.uk   *
 *                                                                           *
 * Creation Date  : dec 2013                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for CATS                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "../include/VSpectra.h"
#include "TCATSData.h"
#include "TCATSPhysics.h"

// C++ STL headers
#include <vector>
using namespace std;


// Forward Declaration
class TCATSPhysics;


class TCATSSpectra:public VSpectra {
  public:
    // constructor and destructor
    TCATSSpectra();
    TCATSSpectra(unsigned int NumberOfCats);
    ~TCATSSpectra();

  public:
    // Initialization methods
    virtual void InitRawSpectra();
    virtual void InitPreTreatedSpectra();
    virtual void InitPhysicsSpectra();

  public:
    // Filling methods
    virtual void FillRawSpectra(TCATSData*);
    virtual void FillPreTreatedSpectra(TCATSData*);
    virtual void FillPhysicsSpectra(TCATSPhysics*);

  private: // Information on MUST2
    unsigned int fNumberOfCats;
    unsigned int fStripsNumber;
    int fEventLoopSize;
    int fEventLoopStep;
    int fEventLoopIndex;
    vector<double> fEventLoopQSum;

};

#endif
