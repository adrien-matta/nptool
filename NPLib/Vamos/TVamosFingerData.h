/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : July 2009                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds VAMOS Raw data                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#ifndef __VAMOSFINGERDATA__
#define __VAMOSFINGERDATA__

#include <vector>

#include "TObject.h"



class TVamosFingerData : public TObject {
 private:
   // ADC
   UShort_t	fVamos_Finger_Energy;

 public:
   TVamosFingerData();
   virtual ~TVamosFingerData();

   void	Clear();
   void Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   UShort_t	GetFingerEnergy()	{return fVamos_Finger_Energy;}

   /////////////////////           SETTERS           ////////////////////////
   // (E)
   void	SetFingerEnergy(UShort_t E)	{fVamos_Finger_Energy = E;}

   ClassDef(TVamosFingerData,1)  // VamosFingerData structure
};

#endif
