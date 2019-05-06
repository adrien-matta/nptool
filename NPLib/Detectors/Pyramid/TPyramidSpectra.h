#ifndef TPyramidSPECTRA_H
#define TPyramidSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam  contact address: mohamad.moukaddam@iphc.cnrs.fr                        *
 *                                                                           *
 * Creation Date  : November 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Pyramid Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TPyramidData.h"
#include "TPyramidPhysics.h"

// ForwardDeclaration
class TPyramidPhysics ;

class TPyramidSpectra: public VSpectra {
  public:
    // constructor and destructor
    TPyramidSpectra();
    TPyramidSpectra(unsigned int NumberOfDetector);
    ~TPyramidSpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TPyramidData*);
    void FillPreTreatedSpectra(TPyramidData*);
    void FillPhysicsSpectra(TPyramidPhysics*);

  private: // Information 
    unsigned int fNumberOfDetector;
    unsigned int fInnerStrip;
    unsigned int fOuterStrip;
};

#endif
