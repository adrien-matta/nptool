#ifndef __HIRADATA__
#define __HIRADATA__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
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
class THiraData : public TObject {
   private:
    // SSSD
    // Energy
    vector<UShort_t>   fHiRA_ThinSi_StripE_DetectorNbr;
    vector<UShort_t>   fHiRA_ThinSi_StripE_StripNbr;
    vector<Double_t>   fHiRA_ThinSi_StripE_Energy;
    // Time
    vector<UShort_t>   fHiRA_ThinSi_StripT_DetectorNbr;
    vector<UShort_t>   fHiRA_ThinSi_StripT_StripNbr;
    vector<Double_t>   fHiRA_ThinSi_StripT_Time;
    
    // DSSD
    // X strips
    // Energy
    vector<UShort_t>   fHiRA_ThickSi_StripXE_DetectorNbr;
    vector<UShort_t>   fHiRA_ThickSi_StripXE_StripNbr;
    vector<Double_t>   fHiRA_ThickSi_StripXE_Energy;
    // Time
    vector<UShort_t>   fHiRA_ThickSi_StripXT_DetectorNbr;
    vector<UShort_t>   fHiRA_ThickSi_StripXT_StripNbr;
    vector<Double_t>   fHiRA_ThickSi_StripXT_Time;
    // Y strips
    // Energy
    vector<UShort_t>   fHiRA_ThickSi_StripYE_DetectorNbr;
    vector<UShort_t>   fHiRA_ThickSi_StripYE_StripNbr;
    vector<Double_t>   fHiRA_ThickSi_StripYE_Energy;
    // Time
    vector<UShort_t>   fHiRA_ThickSi_StripYT_DetectorNbr;
    vector<UShort_t>   fHiRA_ThickSi_StripYT_StripNbr;
    vector<Double_t>   fHiRA_ThickSi_StripYT_Time;


    // CsI
    // Energy
    vector<UShort_t>   fHiRA_CsIE_DetectorNbr;
    vector<UShort_t>   fHiRA_CsIE_CristalNbr;
    vector<Double_t>   fHiRA_CsIE_Energy;
    // Time
    vector<UShort_t>   fHiRA_CsIT_DetectorNbr;
    vector<UShort_t>   fHiRA_CsIT_CristalNbr;
    vector<Double_t>   fHiRA_CsIT_Time;

   public:
    THiraData();
    virtual ~THiraData();

    void   Clear();
    void  Clear(const Option_t*) {};
    void   Dump() const;

    /////////////////////           SETTERS           ////////////////////////
    // SSSD
    // (X,E)
    void   SetHiraThinSiStripEDetectorNbr(const UShort_t DetNbr) {fHiRA_ThinSi_StripE_DetectorNbr.push_back(DetNbr);}
    void   SetHiraThinSiStripEStripNbr(const UShort_t StripNbr)  {fHiRA_ThinSi_StripE_StripNbr.push_back(StripNbr);}
    void   SetHiraThinSiStripEEnergy(const Double_t Energy)      {fHiRA_ThinSi_StripE_Energy.push_back(Energy);}
    // (X,T)
    void   SetHiraThinSiStripTDetectorNbr(const UShort_t DetNbr) {fHiRA_ThinSi_StripT_DetectorNbr.push_back(DetNbr);}
    void   SetHiraThinSiStripTStripNbr(const UShort_t StripNbr)  {fHiRA_ThinSi_StripT_StripNbr.push_back(StripNbr);}
    void   SetHiraThinSiStripTTime(const Double_t Time)          {fHiRA_ThinSi_StripT_Time.push_back(Time);}
    
    // DSSD
    // (X,E)
    void   SetHiraThickSiStripXEDetectorNbr(const UShort_t DetNbr) {fHiRA_ThickSi_StripXE_DetectorNbr.push_back(DetNbr);}
    void   SetHiraThickSiStripXEStripNbr(const UShort_t StripNbr)  {fHiRA_ThickSi_StripXE_StripNbr.push_back(StripNbr);}
    void   SetHiraThickSiStripXEEnergy(const Double_t Energy)      {fHiRA_ThickSi_StripXE_Energy.push_back(Energy);}
    // (X,T)
    void   SetHiraThickSiStripXTDetectorNbr(const UShort_t DetNbr) {fHiRA_ThickSi_StripXT_DetectorNbr.push_back(DetNbr);}
    void   SetHiraThickSiStripXTStripNbr(const UShort_t StripNbr)  {fHiRA_ThickSi_StripXT_StripNbr.push_back(StripNbr);}
    void   SetHiraThickSiStripXTTime(const Double_t Time)          {fHiRA_ThickSi_StripXT_Time.push_back(Time);}
    // (Y,E)
    void   SetHiraThickSiStripYEDetectorNbr(const UShort_t DetNbr) {fHiRA_ThickSi_StripYE_DetectorNbr.push_back(DetNbr);}
    void   SetHiraThickSiStripYEStripNbr(const UShort_t StripNbr)  {fHiRA_ThickSi_StripYE_StripNbr.push_back(StripNbr);}
    void   SetHiraThickSiStripYEEnergy(const Double_t Energy)      {fHiRA_ThickSi_StripYE_Energy.push_back(Energy);}
    // (Y,T)
    void   SetHiraThickSiStripYTDetectorNbr(const UShort_t DetNbr) {fHiRA_ThickSi_StripYT_DetectorNbr.push_back(DetNbr);}
    void   SetHiraThickSiStripYTStripNbr(const UShort_t StripNbr)  {fHiRA_ThickSi_StripYT_StripNbr.push_back(StripNbr);}
    void   SetHiraThickSiStripYTTime(const Double_t Time)          {fHiRA_ThickSi_StripYT_Time.push_back(Time);}

    // CsI
    // (E)
    void   SetHiraCsIEDetectorNbr(const UShort_t DetNbr)    {fHiRA_CsIE_DetectorNbr.push_back(DetNbr);}
    void   SetHiraCsIECristalNbr(const UShort_t CristalNbr) {fHiRA_CsIE_CristalNbr.push_back(CristalNbr);}
    void   SetHiraCsIEEnergy(const Double_t Energy)         {fHiRA_CsIE_Energy.push_back(Energy);}
    // (T)
    void   SetHiraCsITDetectorNbr(const UShort_t DetNbr)    {fHiRA_CsIT_DetectorNbr.push_back(DetNbr);}
    void   SetHiraCsITCristalNbr(const UShort_t CristalNbr) {fHiRA_CsIT_CristalNbr.push_back(CristalNbr);}
    void   SetHiraCsITTime(const Double_t Time)             {fHiRA_CsIT_Time.push_back(Time);}

    /////////////////////           GETTERS           ////////////////////////
    // DSSD
    // (X,E)
    UShort_t   GetHiraStripXEMult()                     const {return fHiRA_ThickSi_StripXE_DetectorNbr.size();}
    UShort_t   GetHiraStripXEDetectorNbr(const Int_t i) const {return fHiRA_ThickSi_StripXE_DetectorNbr[i];}
    UShort_t   GetHiraStripXEStripNbr(const Int_t i)    const {return fHiRA_ThickSi_StripXE_StripNbr[i];}
    Double_t   GetHiraStripXEEnergy(const Int_t i)      const {return fHiRA_ThickSi_StripXE_Energy[i];}
    // (X,T)
    UShort_t   GetHiraStripXTMult()                     const {return fHiRA_ThickSi_StripXT_DetectorNbr.size();}
    UShort_t   GetHiraStripXTDetectorNbr(const Int_t i) const {return fHiRA_ThickSi_StripXT_DetectorNbr[i];}
    UShort_t   GetHiraStripXTStripNbr(const Int_t i)    const {return fHiRA_ThickSi_StripXT_StripNbr[i];}
    Double_t   GetHiraStripXTTime(const Int_t i)        const {return fHiRA_ThickSi_StripXT_Time[i];}
    // (Y,E)
    UShort_t   GetHiraStripYEMult()                     const {return fHiRA_ThickSi_StripYE_DetectorNbr.size();}
    UShort_t   GetHiraStripYEDetectorNbr(const Int_t i) const {return fHiRA_ThickSi_StripYE_DetectorNbr[i];}
    UShort_t   GetHiraStripYEStripNbr(const Int_t i)    const {return fHiRA_ThickSi_StripYE_StripNbr[i];}
    Double_t   GetHiraStripYEEnergy(const Int_t i)      const {return fHiRA_ThickSi_StripYE_Energy[i];}
    // (Y,T)
    UShort_t   GetHiraStripYTMult()                     const {return fHiRA_ThickSi_StripYT_DetectorNbr.size();}
    UShort_t   GetHiraStripYTDetectorNbr(const Int_t i) const {return fHiRA_ThickSi_StripYT_DetectorNbr[i];}
    UShort_t   GetHiraStripYTStripNbr(const Int_t i)    const {return fHiRA_ThickSi_StripYT_StripNbr[i];}
    Double_t   GetHiraStripYTTime(const Int_t i)        const {return fHiRA_ThickSi_StripYT_Time[i];}

    
    // CsI
    //(E)
    UShort_t   GetHiraCsIEMult()                       const {return fHiRA_CsIE_DetectorNbr.size();}
    UShort_t   GetHiraCsIEDetectorNbr(const Int_t i)   const {return fHiRA_CsIE_DetectorNbr[i];}
    UShort_t   GetHiraCsIECristalNbr(const Int_t i)    const {return fHiRA_CsIE_CristalNbr[i];}
    Double_t   GetHiraCsIEEnergy(const Int_t i)        const {return fHiRA_CsIE_Energy[i];}
    //(T)
    UShort_t   GetHiraCsITMult()                       const {return fHiRA_CsIT_DetectorNbr.size();}
    UShort_t   GetHiraCsITDetectorNbr(const Int_t i)   const {return fHiRA_CsIT_DetectorNbr[i];}
    UShort_t   GetHiraCsITCristalNbr(const Int_t i)    const {return fHiRA_CsIT_CristalNbr[i];}
    Double_t   GetHiraCsITTime(const Int_t i)          const {return fHiRA_CsIT_Time[i];}

    ClassDef(THiraData,2)  // HiraData structure
};

#endif
