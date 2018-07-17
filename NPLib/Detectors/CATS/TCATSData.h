#ifndef __CATSDATA__
#define __CATSDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
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
   ~TCATSData();

    void Clear();
    void Clear(const Option_t*) {};
    void Dump() const;

    /////////////////////           SETTERS           ////////////////////////
    // X
    inline void SetStripX(const UShort_t& DetX, const UShort_t& StripX, const UShort_t& ChargeX){
      fCATS_DetX.push_back(DetX);                                                 
      fCATS_StripX.push_back(StripX);                                             
      fCATS_ChargeX.push_back(ChargeX);                                           
    }
    // Y
    inline void SetStripY(const UShort_t& DetY, const UShort_t& StripY, const UShort_t& ChargeY){
      fCATS_DetY.push_back(DetY);                                                 
      fCATS_StripY.push_back(StripY);                                             
      fCATS_ChargeY.push_back(ChargeY);                                           
    }

    //Q fil
    inline void SetQ(const UShort_t& DetQ, const UShort_t& Charge){
      fCATS_DetQ.push_back(DetQ);
      fCATS_Charge.push_back(Charge);
    }
    /////////////////////           GETTERS           ////////////////////////
    // X
    inline UShort_t	GetCATSMultX()		              const {return fCATS_DetX.size();}
    inline UShort_t	GetCATSDetX(const Int_t& i)	    const {return fCATS_DetX[i];}
    inline UShort_t	GetCATSStripX(const Int_t& i)   const {return fCATS_StripX[i];}
    inline UShort_t	GetCATSChargeX(const Int_t& i)	const {return fCATS_ChargeX[i];}
    // Y
    inline UShort_t	GetCATSMultY()		              const {return fCATS_DetY.size();}
    inline UShort_t	GetCATSDetY(const Int_t& i)	    const {return fCATS_DetY[i];}
    inline UShort_t	GetCATSStripY(const Int_t& i)   const {return fCATS_StripY[i];}
    inline UShort_t	GetCATSChargeY(const Int_t& i)	const {return fCATS_ChargeY[i];}
    //Q fil
    inline UShort_t	GetCATSMultQ()                 const {return fCATS_DetQ.size();}
    inline UShort_t	GetCATSDetQ(const Int_t& i)    const {return fCATS_DetQ[i];}
    inline UShort_t	GetCATSCharge(const Int_t& i)  const {return fCATS_Charge[i];}

    ClassDef(TCATSData,2)  // CATSData structure
};

#endif
