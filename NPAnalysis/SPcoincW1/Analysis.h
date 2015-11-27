#ifndef Analysis_h 
#define Analysis_h
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : November 2015                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *    Analysis for the SP + W1 coincidence setup                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "NPVAnalysis.h"

// NPTool headers
#include "TW1Physics.h"
#include "TSplitPolePhysics.h"

// ROOT headers
#include "TCutG.h"


class Analysis: public NPL::VAnalysis
{
  public:
    Analysis();
    ~Analysis();

  public: 
    void Init();
    void TreatEvent();
    void End();

    void Clear();
    static NPL::VAnalysis* Construct();

  private:
    TW1Physics        *m_W1;
    TSplitPolePhysics *m_SP;

  private:
    TCutG *m_cutg_SP_p;

  private:
    // W1's
    Double_t fEnergy;
    Double_t fTime;
    Int_t    fDetector;
    Int_t    fStripFront;
    Int_t    fStripBack;
    // SP
    Double_t fBrho;

};
#endif
