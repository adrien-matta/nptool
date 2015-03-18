#ifndef TEXOGAMSPECTRA_H
#define TEXOGAMSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
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
 *  This class holds all the online spectra needed for Exogam                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TExogamData.h"
#include "TExogamPhysics.h"

// ForwardDeclaration
class TExogamPhysics ;

class TExogamSpectra:public VSpectra {
  public:
    // constructor and destructor
    TExogamSpectra();
    TExogamSpectra(unsigned int NumberOfClover);
    ~TExogamSpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TExogamData*);
    void FillPreTreatedSpectra(TExogamData*);
    void FillPhysicsSpectra(TExogamPhysics*);

  private: // Information on Exogam
    unsigned int fNumberOfClover ;
    unsigned int fnbinsRaw;
    unsigned int fbinRawMin;
    unsigned int fbinRawMax;
    unsigned int fnbinsCal;
    unsigned int fbinCalMin;
    unsigned int fbinCalMax;
    unsigned int fNumberOfSegments;
    unsigned int fNumberOfCores;
};

#endif
