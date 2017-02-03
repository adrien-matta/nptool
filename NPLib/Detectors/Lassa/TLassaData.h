#ifndef __LASSADATA__
#define __LASSADATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address:                       *
 *                                                                           *
 * Creation Date  : febuary 2015                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Hira Raw data                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>
#include "TObject.h"

using namespace std ;
class TLassaData : public TObject {
   private:
    // DSSD
    // X strips
    // Energy
    vector<UShort_t>   fLassa_ThickSi_StripXE_DetectorNbr;
    vector<UShort_t>   fLassa_ThickSi_StripXE_StripNbr;
    vector<Double_t>   fLassa_ThickSi_StripXE_Energy;
    // Time
    vector<UShort_t>   fLassa_ThickSi_StripXT_DetectorNbr;
    vector<UShort_t>   fLassa_ThickSi_StripXT_StripNbr;
    vector<Double_t>   fLassa_ThickSi_StripXT_Time;
    // Y strips
    // Energy
    vector<UShort_t>   fLassa_ThickSi_StripYE_DetectorNbr;
    vector<UShort_t>   fLassa_ThickSi_StripYE_StripNbr;
    vector<Double_t>   fLassa_ThickSi_StripYE_Energy;
    // Time
    vector<UShort_t>   fLassa_ThickSi_StripYT_DetectorNbr;
    vector<UShort_t>   fLassa_ThickSi_StripYT_StripNbr;
    vector<Double_t>   fLassa_ThickSi_StripYT_Time;


    // CsI
    // Energy
    vector<UShort_t>   fLassa_CsIE_DetectorNbr;
    vector<UShort_t>   fLassa_CsIE_CristalNbr;
    vector<Double_t>   fLassa_CsIE_Energy;
    // Time
    vector<UShort_t>   fLassa_CsIT_DetectorNbr;
    vector<UShort_t>   fLassa_CsIT_CristalNbr;
    vector<Double_t>   fLassa_CsIT_Time;

   public:
    TLassaData();
    virtual ~TLassaData();

    void   Clear();
    void  Clear(const Option_t*) {};
    void   Dump() const;

    // DSSD
    // (X,E)
    void   SetLassaThickSiStripXEDetectorNbr(const UShort_t DetNbr) {fLassa_ThickSi_StripXE_DetectorNbr.push_back(DetNbr);}
    void   SetLassaThickSiStripXEStripNbr(const UShort_t StripNbr)  {fLassa_ThickSi_StripXE_StripNbr.push_back(StripNbr);}
    void   SetLassaThickSiStripXEEnergy(const Double_t Energy)      {fLassa_ThickSi_StripXE_Energy.push_back(Energy);}
    // (X,T)
    void   SetLassaThickSiStripXTDetectorNbr(const UShort_t DetNbr) {fLassa_ThickSi_StripXT_DetectorNbr.push_back(DetNbr);}
    void   SetLassaThickSiStripXTStripNbr(const UShort_t StripNbr)  {fLassa_ThickSi_StripXT_StripNbr.push_back(StripNbr);}
    void   SetLassaThickSiStripXTTime(const Double_t Time)          {fLassa_ThickSi_StripXT_Time.push_back(Time);}
    // (Y,E)
    void   SetLassaThickSiStripYEDetectorNbr(const UShort_t DetNbr) {fLassa_ThickSi_StripYE_DetectorNbr.push_back(DetNbr);}
    void   SetLassaThickSiStripYEStripNbr(const UShort_t StripNbr)  {fLassa_ThickSi_StripYE_StripNbr.push_back(StripNbr);}
    void   SetLassaThickSiStripYEEnergy(const Double_t Energy)      {fLassa_ThickSi_StripYE_Energy.push_back(Energy);}
    // (Y,T)
    void   SetLassaThickSiStripYTDetectorNbr(const UShort_t DetNbr) {fLassa_ThickSi_StripYT_DetectorNbr.push_back(DetNbr);}
    void   SetLassaThickSiStripYTStripNbr(const UShort_t StripNbr)  {fLassa_ThickSi_StripYT_StripNbr.push_back(StripNbr);}
    void   SetLassaThickSiStripYTTime(const Double_t Time)          {fLassa_ThickSi_StripYT_Time.push_back(Time);}

    // CsI
    // (E)
    void   SetLassaCsIEDetectorNbr(const UShort_t DetNbr)    {fLassa_CsIE_DetectorNbr.push_back(DetNbr);}
    void   SetLassaCsIECristalNbr(const UShort_t CristalNbr) {fLassa_CsIE_CristalNbr.push_back(CristalNbr);}
    void   SetLassaCsIEEnergy(const Double_t Energy)         {fLassa_CsIE_Energy.push_back(Energy);}
    // (T)
    void   SetLassaCsITDetectorNbr(const UShort_t DetNbr)    {fLassa_CsIT_DetectorNbr.push_back(DetNbr);}
    void   SetLassaCsITCristalNbr(const UShort_t CristalNbr) {fLassa_CsIT_CristalNbr.push_back(CristalNbr);}
    void   SetLassaCsITTime(const Double_t Time)             {fLassa_CsIT_Time.push_back(Time);}

    /////////////////////           GETTERS           ////////////////////////
    // DSSD
    // (X,E)
    UShort_t   GetLassaStripXEMult()                     const {return fLassa_ThickSi_StripXE_DetectorNbr.size();}
    UShort_t   GetLassaStripXEDetectorNbr(const Int_t i) const {return fLassa_ThickSi_StripXE_DetectorNbr[i];}
    UShort_t   GetLassaStripXEStripNbr(const Int_t i)    const {return fLassa_ThickSi_StripXE_StripNbr[i];}
    Double_t   GetLassaStripXEEnergy(const Int_t i)      const {return fLassa_ThickSi_StripXE_Energy[i];}
    // (X,T)
    UShort_t   GetLassaStripXTMult()                     const {return fLassa_ThickSi_StripXT_DetectorNbr.size();}
    UShort_t   GetLassaStripXTDetectorNbr(const Int_t i) const {return fLassa_ThickSi_StripXT_DetectorNbr[i];}
    UShort_t   GetLassaStripXTStripNbr(const Int_t i)    const {return fLassa_ThickSi_StripXT_StripNbr[i];}
    Double_t   GetLassaStripXTTime(const Int_t i)        const {return fLassa_ThickSi_StripXT_Time[i];}
    // (Y,E)
    UShort_t   GetLassaStripYEMult()                     const {return fLassa_ThickSi_StripYE_DetectorNbr.size();}
    UShort_t   GetLassaStripYEDetectorNbr(const Int_t i) const {return fLassa_ThickSi_StripYE_DetectorNbr[i];}
    UShort_t   GetLassaStripYEStripNbr(const Int_t i)    const {return fLassa_ThickSi_StripYE_StripNbr[i];}
    Double_t   GetLassaStripYEEnergy(const Int_t i)      const {return fLassa_ThickSi_StripYE_Energy[i];}
    // (Y,T)
    UShort_t   GetLassaStripYTMult()                     const {return fLassa_ThickSi_StripYT_DetectorNbr.size();}
    UShort_t   GetLassaStripYTDetectorNbr(const Int_t i) const {return fLassa_ThickSi_StripYT_DetectorNbr[i];}
    UShort_t   GetLassaStripYTStripNbr(const Int_t i)    const {return fLassa_ThickSi_StripYT_StripNbr[i];}
    Double_t   GetLassaStripYTTime(const Int_t i)        const {return fLassa_ThickSi_StripYT_Time[i];}

    
    // CsI
    //(E)
    UShort_t   GetLassaCsIEMult()                       const {return fLassa_CsIE_DetectorNbr.size();}
    UShort_t   GetLassaCsIEDetectorNbr(const Int_t i)   const {return fLassa_CsIE_DetectorNbr[i];}
    UShort_t   GetLassaCsIECristalNbr(const Int_t i)    const {return fLassa_CsIE_CristalNbr[i];}
    Double_t   GetLassaCsIEEnergy(const Int_t i)        const {return fLassa_CsIE_Energy[i];}
    //(T)
    UShort_t   GetLassaCsITMult()                       const {return fLassa_CsIT_DetectorNbr.size();}
    UShort_t   GetLassaCsITDetectorNbr(const Int_t i)   const {return fLassa_CsIT_DetectorNbr[i];}
    UShort_t   GetLassaCsITCristalNbr(const Int_t i)    const {return fLassa_CsIT_CristalNbr[i];}
    Double_t   GetLassaCsITTime(const Int_t i)          const {return fLassa_CsIT_Time[i];}

    ClassDef(TLassaData,2)  // LassaData structure
};

#endif
