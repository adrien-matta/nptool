#ifndef TSHARCSPECTRA_H
#define TSHARCSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold DETECTORNAME Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TDETECTORNAMEData.h"
#include "TDETECTORNAMEPhysics.h"

// ForwardDeclaration
class TDETECTORNAMEPhysics ;

class TDETECTORNAMESpectra:public VSpectra {
  public:
    // constructor and destructor
    TDETECTORNAMESpectra();
    TDETECTORNAMESpectra(unsigned int NumberOfDetector);
    ~TDETECTORNAMESpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TDETECTORNAMEData*);
    void FillPreTreatedSpectra(TDETECTORNAMEData*);
    void FillPhysicsSpectra(TDETECTORNAMEPhysics*);

  private: // Information on SHARC
    unsigned int fNumberOfDetector;
    unsigned int fStripFront;
    unsigned int fStripBack;
    unsigned int fPad;
};

#endif
