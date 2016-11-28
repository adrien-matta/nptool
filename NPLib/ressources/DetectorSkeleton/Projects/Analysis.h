#ifndef Analysis_h 
#define Analysis_h
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
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
 *  This class describe  DETECTORNAME analysis project                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include"NPVAnalysis.h"
#include"TDETECTORNAMEPhysics.h"
class Analysis: public NPL::VAnalysis{
  public:
    Analysis();
    ~Analysis();

  public: 
    void Init();
    void TreatEvent();
    void End();

   static NPL::VAnalysis* Construct();

  private:
   TDETECTORNAMEPhysics* DETECTORNAME;

};
#endif
