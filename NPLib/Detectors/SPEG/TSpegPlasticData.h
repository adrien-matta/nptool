#ifndef __SPEGPLASTICDATA__
#define __SPEGPLASTICDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
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
 *  This class holds SPEG Raw data                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#include <vector>

#include "TObject.h"



class TSpegPlasticData : public TObject {
 private:
   // ADC
   UShort_t	fSpeg_Plastic_Energy_Left;
   UShort_t	fSpeg_Plastic_Energy_Right;
   UShort_t	fSpeg_Plastic_Time_Left;
   UShort_t	fSpeg_Plastic_Time_Right;

 public:
   TSpegPlasticData();
   virtual ~TSpegPlasticData();

   void	Clear();
   void Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   UShort_t	GetEnergyLeft()		{return fSpeg_Plastic_Energy_Left;}
   UShort_t	GetEnergyRight()	{return fSpeg_Plastic_Energy_Right;}
   UShort_t	GetTimeLeft()	{return fSpeg_Plastic_Time_Left;}
   UShort_t	GetTimeRight()	{return fSpeg_Plastic_Time_Right;}

   /////////////////////           SETTERS           ////////////////////////
   // (E)
   void	SetEnergyLeft(UShort_t E)	{fSpeg_Plastic_Energy_Left = E;}
   void	SetEnergyRight(UShort_t E)	{fSpeg_Plastic_Energy_Right = E;}
   void	SetTimeLeft(UShort_t E)	{fSpeg_Plastic_Time_Left = E;}
   void	SetTimeRight(UShort_t E)	{fSpeg_Plastic_Time_Right = E;}

   ClassDef(TSpegPlasticData,1)  // SpegPlasticData structure
};

#endif
