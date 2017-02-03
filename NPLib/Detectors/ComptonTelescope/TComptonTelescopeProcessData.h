#ifndef __COMPTONTELESCOPEPROCESSDATA__
#define __COMPTONTELESCOPEPROCESSDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : July 2012                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds the raw data for a Compton telescope prototype made of  *
 *  a silicon tracker detector and a LaBr3 calorimeter                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>
#include "TObject.h"

using namespace std ;
class TComptonTelescopeProcessData : public TObject {
   private:
      // DSSSD tracker
      // Energy
      vector<UShort_t>   fCT_Tracker_Compton_TowerNbr;
      vector<UShort_t>   fCT_Tracker_Compton_DetectorNbr;
      vector<Double_t>   fCT_Tracker_Compton_Energy;


   public:
      TComptonTelescopeProcessData();
      virtual ~TComptonTelescopeProcessData();

      void Clear();
      void Clear(const Option_t*) {};
      void Dump() const;

      /////////////////////           SETTERS           ////////////////////////
      // DSSD
      // (X,E)
      void SetCTTrackerComptonTowerNbr(const UShort_t TowNbr)      {fCT_Tracker_Compton_TowerNbr.push_back(TowNbr);}
      void SetCTTrackerComptonDetectorNbr(const UShort_t DetNbr)   {fCT_Tracker_Compton_DetectorNbr.push_back(DetNbr);}
      void SetCTTrackerComptonEnergy(const Double_t Energy)        {fCT_Tracker_Compton_Energy.push_back(Energy);}


      /////////////////////           GETTERS           ////////////////////////
      // DSSD
      // (X,E)
      UShort_t   GetCTTrackerComptonMult()                      const {return fCT_Tracker_Compton_TowerNbr.size();}
      UShort_t   GetCTTrackerComptonTowerNbr(const Int_t i)     const {return fCT_Tracker_Compton_TowerNbr[i];}
      UShort_t   GetCTTrackerComptonDetectorNbr(const Int_t i)  const {return fCT_Tracker_Compton_DetectorNbr[i];}
      Double_t   GetCTTrackerComptonEnergy(const Int_t i)       const {return fCT_Tracker_Compton_Energy[i];}

      ClassDef(TComptonTelescopeProcessData,1)  // ComptonTelescopeProcessData structure
};

#endif
