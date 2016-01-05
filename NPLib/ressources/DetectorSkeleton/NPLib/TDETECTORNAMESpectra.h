#ifndef TDETECTORNAMESPECTRA_H
#define TDETECTORNAMESPECTRA_H
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

// Forward Declaration
class TDETECTORNAMEPhysics;


class TDETECTORNAMESpectra : public VSpectra 
{
  //////////////////////////////////////////////////////////////
  // constructor and destructor
  public:
    TDETECTORNAMESpectra();
    TDETECTORNAMESpectra(unsigned int NumberOfDetectors);
    ~TDETECTORNAMESpectra();

  //////////////////////////////////////////////////////////////
  // Initialization methods
  private:
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  //////////////////////////////////////////////////////////////
  // Filling methods
  public:
    void FillRawSpectra(TDETECTORNAMEData*);
    void FillPreTreatedSpectra(TDETECTORNAMEData*);
    void FillPhysicsSpectra(TDETECTORNAMEPhysics*);

  //////////////////////////////////////////////////////////////
  // Detector parameters 
  private:
    unsigned int fNumberOfDetectors;
};

#endif
