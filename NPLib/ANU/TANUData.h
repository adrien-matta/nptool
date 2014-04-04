#ifndef __ANUDATA__
#define __ANUDATA__
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 30/01/12                                                 *
 * Last update    : 04/04/14                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the results of the G4 simulation for the    *
 *             tracker part of the ANU detector.                         	 *
 *             The format mimics what was used for the GANIL experiments     *
 *             after conversion of the raw data with GRU. Ask                *
 *             N. de Sereville for more informations.                        *
 *             The same format will be used for the ANU Setup                *
 *             for developpement puposes                                     *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *         The ANU detector is an array of 6 identical SiLi detectors        *
 *         having the shape of a camembert. It has no strips                 *
 *****************************************************************************/

#include <vector>
#include "TObject.h"

using namespace std ;

class TANUData : public TObject
{
protected:
   // First Stage
   // Energy
   vector<UShort_t>  fANU_SiLi_E_DetectorNbr;
   vector<Double_t>  fANU_SiLi_E_Energy;
   // Time
   vector<UShort_t>  fANU_SiLi_T_DetectorNbr;
   vector<Double_t>  fANU_SiLi_T_Time;



public:
   TANUData();
   virtual ~TANUData();

   void  Clear();
   void  Clear(const Option_t*) {};
   void  Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // RSD
   // (E)
   void  SetANUSiLiEDetectorNbr(UShort_t DetNbr) {
      fANU_SiLi_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetANUSiLiEEnergy(Double_t Energy)      {
      fANU_SiLi_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetANUSiLiTDetectorNbr(UShort_t DetNbr) {
      fANU_SiLi_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetANUSiLiTTime(Double_t Time)          {
      fANU_SiLi_T_Time.push_back(Time);
   }


   /////////////////////           GETTERS           ////////////////////////
   // RSD
   // (E)
   UShort_t GetANUSiLiEMult()               {
      return fANU_SiLi_E_DetectorNbr.size();
   }
   UShort_t GetANUSiLiEDetectorNbr(Int_t i) {
      return fANU_SiLi_E_DetectorNbr.at(i);
   }
   Double_t GetANUSiLiEEnergy(Int_t i)      {
      return fANU_SiLi_E_Energy.at(i);
   }
   // (T)
   UShort_t GetANUSiLiTMult()               {
      return fANU_SiLi_T_DetectorNbr.size();
   }
   UShort_t GetANUSiLiTDetectorNbr(Int_t i) {
      return fANU_SiLi_T_DetectorNbr.at(i);
   }
   Double_t GetANUSiLiTTime(Int_t i)      {
      return fANU_SiLi_T_Time.at(i);
   }

 

   ClassDef(TANUData, 1) // ANUData structure
};

#endif
