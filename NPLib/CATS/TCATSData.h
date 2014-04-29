#ifndef __CATSDATA__
#define __CATSDATA__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Sandra Giron  contact address: giron@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2010                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold CATS raw data                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"

using namespace std;


class TCATSData : public TObject {
 protected:
   // X strips
   vector<UShort_t>	fCATS_DetX;
   vector<UShort_t>	fCATS_StripX;
   vector<UShort_t>	fCATS_ChargeX;

   // Y strips
   vector<UShort_t>	fCATS_DetY;
   vector<UShort_t>	fCATS_StripY;
   vector<UShort_t>	fCATS_ChargeY;

   //Q Fil
   vector<UShort_t>	fCATS_DetQ;
   vector<UShort_t>	fCATS_Charge;

 public:
   TCATSData();
   virtual ~TCATSData();

   void	Clear();
   void Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // X
   void SetCATSDetX(UShort_t DetX)	      {fCATS_DetX.push_back(DetX);}
   void SetCATSStripX(UShort_t StripX)	  {fCATS_StripX.push_back(StripX);}
   void SetCATSChargeX(UShort_t ChargeX)  {fCATS_ChargeX.push_back(ChargeX);}
   // Y
   void SetCATSDetY(UShort_t DetY)	      {fCATS_DetY.push_back(DetY);}
   void SetCATSStripY(UShort_t StripY)	  {fCATS_StripY.push_back(StripY);}
   void SetCATSChargeY(UShort_t ChargeY)  {fCATS_ChargeY.push_back(ChargeY);}

   //Q fil
   void SetCATSDetQ(UShort_t DetQ)	      {fCATS_DetQ.push_back(DetQ);}
   void SetCATSCharge(UShort_t Charge)    {fCATS_Charge.push_back(Charge);}

   /////////////////////           GETTERS           ////////////////////////
   // X
   UShort_t	GetCATSMultX()		            const {return fCATS_DetX.size();}
   UShort_t	GetCATSDetX(Int_t i)	         const {return fCATS_DetX.at(i);}
   UShort_t	GetCATSStripX(Int_t i)        const {return fCATS_StripX.at(i);}
   UShort_t	GetCATSChargeX(Int_t i)	      const {return fCATS_ChargeX.at(i);}
   // Y
   UShort_t	GetCATSMultY()		            const {return fCATS_DetY.size();}
   UShort_t	GetCATSDetY(Int_t i)	         const {return fCATS_DetY.at(i);}
   UShort_t	GetCATSStripY(Int_t i)        const {return fCATS_StripY.at(i);}
   UShort_t	GetCATSChargeY(Int_t i)	      const {return fCATS_ChargeY.at(i);}
   //Q fil
   UShort_t	GetCATSMultQ()                const {return fCATS_DetQ.size();}
   UShort_t	GetCATSDetQ(Int_t i)          const {return fCATS_DetQ.at(i);}
   UShort_t	GetCATSCharge(Int_t i)        const {return fCATS_Charge.at(i);}

   ClassDef(TCATSData,2)  // CATSData structure
};

#endif
