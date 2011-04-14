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
#ifndef __VAMOSCHIODATA__
#define __VAMOSCHIODATA__

#include <vector>

#include "TObject.h"



class TVamosCHIOData : public TObject {
 private:
   // Row A
   vector<UShort_t>	fVamos_CHIO_A_PadNbr;
   vector<UShort_t>	fVamos_CHIO_A_Energy;

   // Row B
   vector<UShort_t>	fVamos_CHIO_B_PadNbr;
   vector<UShort_t>	fVamos_CHIO_B_Energy;

   // Row C
   vector<UShort_t>	fVamos_CHIO_C_PadNbr;
   vector<UShort_t>	fVamos_CHIO_C_Energy;


 public:
   TVamosCHIOData();
   virtual ~TVamosCHIOData();

   void	Clear();
   void Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // A
   void	SetVamosCHIOPadNbrA(UShort_t PadNbrA)	 {fVamos_CHIO_A_PadNbr.push_back(PadNbrA);}
   void SetVamosCHIOEnergyA(UShort_t EnergyA)	 {fVamos_CHIO_A_Energy.push_back(EnergyA);}
   // B
   void	SetVamosCHIOPadNbrB(UShort_t PadNbrB)	 {fVamos_CHIO_B_PadNbr.push_back(PadNbrB);}
   void SetVamosCHIOEnergyB(UShort_t EnergyB)	 {fVamos_CHIO_B_Energy.push_back(EnergyB);}
   // C
   void	SetVamosCHIOPadNbrC(UShort_t PadNbrC)	 {fVamos_CHIO_C_PadNbr.push_back(PadNbrC);}
   void SetVamosCHIOEnergyC(UShort_t EnergyC)	 {fVamos_CHIO_C_Energy.push_back(EnergyC);}

   /////////////////////           GETTERS           ////////////////////////
   // A
   UShort_t	GetVamosCHIOMultA()		{return fVamos_CHIO_A_PadNbr.size();}
   UShort_t	GetVamosCHIOPadNbrA(Int_t i)	{return fVamos_CHIO_A_PadNbr.at(i);}
   UShort_t	GetVamosCHIOEnergyA(Int_t i)	{return fVamos_CHIO_A_Energy.at(i);}
   // B
   UShort_t	GetVamosCHIOMultB()		{return fVamos_CHIO_B_PadNbr.size();}
   UShort_t	GetVamosCHIOPadNbrB(Int_t i)	{return fVamos_CHIO_B_PadNbr.at(i);}
   UShort_t	GetVamosCHIOEnergyB(Int_t i)	{return fVamos_CHIO_B_Energy.at(i);}
   // C
   UShort_t	GetVamosCHIOMultC()		{return fVamos_CHIO_C_PadNbr.size();}
   UShort_t	GetVamosCHIOPadNbrC(Int_t i)	{return fVamos_CHIO_C_PadNbr.at(i);}
   UShort_t	GetVamosCHIOEnergyC(Int_t i)	{return fVamos_CHIO_C_Energy.at(i);}

   ClassDef(TVamosCHIOData,1)  // VamosCHIOData structure
};

#endif
