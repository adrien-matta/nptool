#ifndef __SPLITPOLEDATA__
#define __SPLITPOLEDATA__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : november 2015                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class holds the raw data storage for the SplitPole focal plane   *
 *     detector at Orsay                                                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// ROOT headers
#include "TObject.h"


class TSplitPoleData : public TObject
{
   private:
      Double_t fPosition;
      Double_t fDeltaE;
      Double_t fWire;
      Double_t fPlasticP;
      Double_t fPlasticG;


   public:
      TSplitPoleData();
      virtual ~TSplitPoleData();

      void Clear();
      void Clear(const Option_t*) {};
      void Dump() const;

      /////////////////////           SETTERS           ////////////////////////
      void SetPosition(Double_t position) {fPosition = position;}
      void SetDeltaE(Double_t deltae)     {fDeltaE  = deltae;}
      void SetWire(Double_t wire)         {fWire    = wire;}
      void SetPlasticP(Double_t plp)      {fPlasticP = plp;}
      void SetPlasticG(Double_t plg)      {fPlasticG = plg;}
      void SetFocalPlane(Double_t position, Double_t deltae, Double_t wire, Double_t plp, Double_t plg) {
         SetPosition(position);
         SetDeltaE(deltae);
         SetWire(wire);
         SetPlasticP(plp);
         SetPlasticG(plg);
      }


      /////////////////////           GETTERS           ////////////////////////
      Double_t GetPosition()              const {return fPosition;}
      Double_t GetDeltaE()                const {return fDeltaE;}
      Double_t GetWire()                  const {return fWire;}
      Double_t GetPlasticP()              const {return fPlasticP;}
      Double_t GetPlasticG()              const {return fPlasticG;}


      ClassDef(TSplitPoleData, 1) // TSplitPoleData raw data 
};

#endif
