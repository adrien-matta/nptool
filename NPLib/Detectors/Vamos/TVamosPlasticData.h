#ifndef __VAMOSPLASTICDATA__
#define __VAMOSPLASTICDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
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


#include <vector>

#include "TObject.h"



class TVamosPlasticData : public TObject {
 private:
   // ADC
   UShort_t	fVamos_Plastic_Energy_Left;
   UShort_t	fVamos_Plastic_Energy_Right;
   UShort_t	fVamos_Plastic_Time_LeftRight;

 public:
   TVamosPlasticData();
   virtual ~TVamosPlasticData();

   void	Clear();
   void Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   UShort_t	GetEnergyLeft()		{return fVamos_Plastic_Energy_Left;}
   UShort_t	GetEnergyRight()	{return fVamos_Plastic_Energy_Right;}
   UShort_t	GetTimeLeftRight()	{return fVamos_Plastic_Time_LeftRight;}

   /////////////////////           SETTERS           ////////////////////////
   // (E)
   void	SetEnergyLeft(UShort_t E)	{fVamos_Plastic_Energy_Left = E;}
   void	SetEnergyRight(UShort_t E)	{fVamos_Plastic_Energy_Right = E;}
   void	SetTimeLeftRight(UShort_t E)	{fVamos_Plastic_Time_LeftRight = E;}

   ClassDef(TVamosPlasticData,1)  // VamosPlasticData structure
};

#endif
