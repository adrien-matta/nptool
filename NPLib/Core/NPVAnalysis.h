#ifndef NPAnalysis_h 
#define NPAnalysis_h
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : march 2025                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Class describing the property of an Analysis object                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include"NPDetectorManager.h"
namespace NPA{
  class VAnalysis{
    public:
     VAnalysis();
     ~VAnalysis();

    public: 
      virtual void TreatEvent(){};
      virtual void Init(){};
      virtual void End(){};
      
      void SetDetectorManager(NPA::DetectorManager*);

    protected:
      NPA::DetectorManager* m_DetectorManager;
  };
}
#endif
