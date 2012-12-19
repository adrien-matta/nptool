/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/
/*****************************************************************************
 * Original Author: L. Lefebvre  contact address: lefebvrl@ipno.in2p3.fr     *
 *                                                                           *
 * Creation Date  : June 2011                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds Speg Raw data                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#ifndef __SPEGCHIODATA__
#define __SPEGCHIODATA__

#include <vector>

#include "TObject.h"



class TSpegCHIOData : public TObject {
 private:

   vector<UShort_t>	fSpeg_CHIO_Energy;

 public:
   TSpegCHIOData();
   virtual ~TSpegCHIOData();

   void	Clear();
   void Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           SETTERS           ////////////////////////

   void SetSpegCHIOEnergy(UShort_t Energy)	 {fSpeg_CHIO_Energy.push_back(Energy);}

   /////////////////////           GETTERS           ////////////////////////

   UShort_t	GetSpegCHIOEnergy(Int_t i)	{return fSpeg_CHIO_Energy.at(i);}
   UShort_t	GetMultSpegCHIOEnergy()                 {return fSpeg_CHIO_Energy.size();}

   ClassDef(TSpegCHIOData,1)  // SpegCHIOData structure
};

#endif
