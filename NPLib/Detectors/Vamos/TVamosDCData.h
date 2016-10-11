#ifndef __VAMOSDCDATA__
#define __VAMOSDCDATA__
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



class TVamosDCData : public TObject {
 private:
   // Strips for row 1
   vector<UShort_t>	fVamos_DC_Strip1_DetNbr;
   vector<UShort_t>	fVamos_DC_Strip1_StripNbr;
   vector<UShort_t>	fVamos_DC_Strip1_Energy;

   // Strips for row 2
   vector<UShort_t>	fVamos_DC_Strip2_DetNbr;
   vector<UShort_t>	fVamos_DC_Strip2_StripNbr;
   vector<UShort_t>	fVamos_DC_Strip2_Energy;

   // EFil
   vector<UShort_t>	fVamos_DC_Efil_DetNbr;
   vector<UShort_t>	fVamos_DC_Efil_Energy;

   // EFil PileUp
   vector<UShort_t>	fVamos_DC_Efil_PileUp_DetNbr;
   vector<UShort_t>	fVamos_DC_Efil_PileUp_Energy;

   // TFil
   vector<UShort_t>	fVamos_DC_Tfil_DetNbr;
   vector<UShort_t>	fVamos_DC_Tfil_Time;

 public:
   TVamosDCData();
   virtual ~TVamosDCData();

   void	Clear();
   void Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // Strips for row 1
   void	SetVamosDCDetNbr1(UShort_t DetNbr1)	{fVamos_DC_Strip1_DetNbr.push_back(DetNbr1);}
   void SetVamosDCStripNbr1(UShort_t StripNbr1)	{fVamos_DC_Strip1_StripNbr.push_back(StripNbr1);}
   void SetVamosDCEnergy1(UShort_t Energy1)	{fVamos_DC_Strip1_Energy.push_back(Energy1);}
   // Strips for row 2
   void	SetVamosDCDetNbr2(UShort_t DetNbr2)	{fVamos_DC_Strip2_DetNbr.push_back(DetNbr2);}
   void SetVamosDCStripNbr2(UShort_t StripNbr2)	{fVamos_DC_Strip2_StripNbr.push_back(StripNbr2);}
   void SetVamosDCEnergy2(UShort_t Energy2)	{fVamos_DC_Strip2_Energy.push_back(Energy2);}

   // E fil
   void	SetVamosDCEfilDetNbr(UShort_t DetNbr)	{fVamos_DC_Efil_DetNbr.push_back(DetNbr);}
   void SetVamosDCEfilEnergy(UShort_t Energy)	{fVamos_DC_Efil_Energy.push_back(Energy);}

   // E fil PileUp
   void	SetVamosDCEfilPileUpDetNbr(UShort_t DetNbr)	{fVamos_DC_Efil_PileUp_DetNbr.push_back(DetNbr);}
   void SetVamosDCEfilPileUpEnergy(UShort_t Energy)	{fVamos_DC_Efil_PileUp_Energy.push_back(Energy);}

   // T fil
   void	SetVamosDCTfilDetNbr(UShort_t DetNbr)	{fVamos_DC_Tfil_DetNbr.push_back(DetNbr);}
   void SetVamosDCTfilTime(UShort_t Time)	{fVamos_DC_Tfil_Time.push_back(Time);}

   /////////////////////           GETTERS           ////////////////////////
   // Strips for row 1
   UShort_t	GetVamosDCMultStrip1()		{return fVamos_DC_Strip1_DetNbr.size();}
   UShort_t	GetVamosDCDetNbr1(Int_t i)	{return fVamos_DC_Strip1_DetNbr.at(i);}
   UShort_t	GetVamosDCStripNbr1(Int_t i)	{return fVamos_DC_Strip1_StripNbr.at(i);}
   UShort_t	GetVamosDCEnergy1(Int_t i)	{return fVamos_DC_Strip1_Energy.at(i);}
   // Strips for row 2
   UShort_t	GetVamosDCMultStrip2()		{return fVamos_DC_Strip2_DetNbr.size();}
   UShort_t	GetVamosDCDetNbr2(Int_t i)	{return fVamos_DC_Strip2_DetNbr.at(i);}
   UShort_t	GetVamosDCStripNbr2(Int_t i)	{return fVamos_DC_Strip2_StripNbr.at(i);}
   UShort_t	GetVamosDCEnergy2(Int_t i)	{return fVamos_DC_Strip2_Energy.at(i);}

   // E fil
   UShort_t	GetVamosDCEfilMult()		{return fVamos_DC_Efil_DetNbr.size();}
   UShort_t	GetVamosDCEfilDetNbr(Int_t i)	{return fVamos_DC_Efil_DetNbr.at(i);}
   UShort_t	GetVamosDCEfilEnergy(Int_t i)	{return fVamos_DC_Efil_Energy.at(i);}

   // E fil PileUp
   UShort_t	GetVamosDCEfilPileUpMult()		{return fVamos_DC_Efil_PileUp_DetNbr.size();}
   UShort_t	GetVamosDCEfilPileUpDetNbr(Int_t i)	{return fVamos_DC_Efil_PileUp_DetNbr.at(i);}
   UShort_t	GetVamosDCEfilPileUpEnergy(Int_t i)	{return fVamos_DC_Efil_PileUp_Energy.at(i);}

   // T fil
   UShort_t	GetVamosDCTfilMult()		{return fVamos_DC_Tfil_DetNbr.size();}
   UShort_t	GetVamosDCTfilDetNbr(Int_t i)	{return fVamos_DC_Tfil_DetNbr.at(i);}
   UShort_t	GetVamosDCTfilTime(Int_t i)	{return fVamos_DC_Tfil_Time.at(i);}

   ClassDef(TVamosDCData,1)  // VamosDCData structure
};

#endif
