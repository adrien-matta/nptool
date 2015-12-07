#ifndef __SPLITPOLENMR__
#define __SPLITPOLENMR__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : april 2012                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class reads the rmn file and provides some simple        *
 *             operations: apply delay, calculate mean value for subset of   *
 *             the data, ....                                                *
 *             Operations support both absolute and relative time            *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + Based on cSplitRun class from Iulian Stefan                          *
 *                                                                           *
 *****************************************************************************/

// ROOT headers
#include "TObject.h"
#include "TTimeStamp.h"
#include "TGraph.h"
#include "TString.h"

// C++ headers
#include <iostream>
#include <fstream>
using namespace std;


class TSplitPoleNMR : public TObject
{
   private:
      TTimeStamp  fOpenFileTime;    // 
      Double_t    fDelay;           // Trun-Trmn in sec
      TString     fFileName;
      TGraph*     fRmnRelativeTime; // in sec wrt file beginning
      TGraph*     fRmn;             // absolute time taking into account fDelay
      bool        fIsRelativeTime;  // specify if returned graph is in relative of absolute units
      bool        fIsLargeField;    // specifiy if field is greater than 1 T
      Double_t    fMean;            // mean value for field
      Double_t    fMin;             // min value for field
      Double_t    fMax;             // max value for field


   public:
      TSplitPoleNMR();
      TSplitPoleNMR(const char* fileName);
      TSplitPoleNMR(const char* fileName, Double_t delay);
      virtual ~TSplitPoleNMR();

      void     SetDelay(Double_t delay) {fDelay = delay; ApplyDelay();}
      void     SetIsRelativeTime(bool isRelativeTime);
      void     SetLargeField(bool isLargeField);
      void     SetFileName(TString fileName) {fFileName = fileName;}

      Double_t GetDelay()     {return fDelay;}
      Double_t GetMean()      {return fMean;}
      Double_t GetMin()       {return fMin;}
      Double_t GetMax()       {return fMax;}
      TString  GetFileName()  {return fFileName;}

      Int_t    ReadRmnFile();
      TGraph*  GetRmnGraph() const;
      void     Draw(const char* option) const;
      void     ApplyDelay();

      void     Dump();

      Double_t EvalB(Double_t Time) const;
      Double_t Mean(Double_t t1, Double_t t2) const;

      ClassDef(TSplitPoleNMR,1);
};

#endif
