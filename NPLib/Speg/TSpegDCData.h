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
 *  This class holds SPEG  Raw data                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#ifndef __SPEGDCDATA__
#define __SPEGDCDATA__

#include <vector>

#include "TObject.h"



class TSpegDCData : public TObject {
 private:
   // Strips for CDM 11
   vector<UShort_t>	fSpeg_DC_Strip11_DetNbr;
   vector<UShort_t>	fSpeg_DC_Strip11_StripNbr;
   vector<UShort_t>	fSpeg_DC_Strip11_Energy;

   // Strips for CDM 12
   vector<UShort_t>	fSpeg_DC_Strip12_DetNbr;
   vector<UShort_t>	fSpeg_DC_Strip12_StripNbr;
   vector<UShort_t>	fSpeg_DC_Strip12_Energy;

   // Strips for CDM 21
   vector<UShort_t>	fSpeg_DC_Strip21_DetNbr;
   vector<UShort_t>	fSpeg_DC_Strip21_StripNbr;
   vector<UShort_t>	fSpeg_DC_Strip21_Energy;

   // Strips for CDM 22
   vector<UShort_t>	fSpeg_DC_Strip22_DetNbr;
   vector<UShort_t>	fSpeg_DC_Strip22_StripNbr;
   vector<UShort_t>	fSpeg_DC_Strip22_Energy;

   // EFil11
   vector<UShort_t>	fSpeg_DC_Efil11_DetNbr;
   vector<UShort_t>	fSpeg_DC_Efil11_Energy;

   // EFil12
   vector<UShort_t>	fSpeg_DC_Efil12_DetNbr;
   vector<UShort_t>	fSpeg_DC_Efil12_Energy;

   // EFil21
   vector<UShort_t>	fSpeg_DC_Efil21_DetNbr;
   vector<UShort_t>	fSpeg_DC_Efil21_Energy;

   // EFil22
   vector<UShort_t>	fSpeg_DC_Efil22_DetNbr;
   vector<UShort_t>	fSpeg_DC_Efil22_Energy;

   // Tplfil11
   vector<UShort_t>	fSpeg_DC_Tplfil11_DetNbr;
   vector<UShort_t>	fSpeg_DC_Tplfil11_Time;

   // Tplfil12
   vector<UShort_t>	fSpeg_DC_Tplfil12_DetNbr;
   vector<UShort_t>	fSpeg_DC_Tplfil12_Time;

   // Tplfil21
   vector<UShort_t>	fSpeg_DC_Tplfil21_DetNbr;
   vector<UShort_t>	fSpeg_DC_Tplfil21_Time;

   // Tplfil22
   vector<UShort_t>	fSpeg_DC_Tplfil22_DetNbr;
   vector<UShort_t>	fSpeg_DC_Tplfil22_Time;

   // TEMPS fil11
   vector<UShort_t>	fSpeg_DC_Tfil11_DetNbr;
   vector<UShort_t>	fSpeg_DC_Tfil11_Time;

   // TEMPS fil12
   vector<UShort_t>	fSpeg_DC_Tfil12_DetNbr;
   vector<UShort_t>	fSpeg_DC_Tfil12_Time;

   // TEMPS fil21
   vector<UShort_t>	fSpeg_DC_Tfil21_DetNbr;
   vector<UShort_t>	fSpeg_DC_Tfil21_Time;

   // TEMPS fill22
   vector<UShort_t>	fSpeg_DC_Tfil22_DetNbr;
   vector<UShort_t>	fSpeg_DC_Tfil22_Time;

 public:
   TSpegDCData();
   virtual ~TSpegDCData();

   void	Clear();
   void Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // Strips for CDM 11
   void	SetSpegDCDetNbr11(UShort_t DetNbr11)	{fSpeg_DC_Strip11_DetNbr.push_back(DetNbr11);}
   void SetSpegDCStripNbr11(UShort_t StripNbr11)	{fSpeg_DC_Strip11_StripNbr.push_back(StripNbr11);}
   void SetSpegDCEnergy11(UShort_t Energy11)	{fSpeg_DC_Strip11_Energy.push_back(Energy11);}

   // Strips for CDM 12
   void	SetSpegDCDetNbr12(UShort_t DetNbr12)	{fSpeg_DC_Strip12_DetNbr.push_back(DetNbr12);}
   void SetSpegDCStripNbr12(UShort_t StripNbr12)	{fSpeg_DC_Strip12_StripNbr.push_back(StripNbr12);}
   void SetSpegDCEnergy12(UShort_t Energy12)	{fSpeg_DC_Strip12_Energy.push_back(Energy12);}

   // Strips for CDM 21
   void	SetSpegDCDetNbr21(UShort_t DetNbr21)	{fSpeg_DC_Strip21_DetNbr.push_back(DetNbr21);}
   void SetSpegDCStripNbr21(UShort_t StripNbr21)	{fSpeg_DC_Strip21_StripNbr.push_back(StripNbr21);}
   void SetSpegDCEnergy21(UShort_t Energy21)	{fSpeg_DC_Strip21_Energy.push_back(Energy21);}

   // Strips for CDM 22
   void	SetSpegDCDetNbr22(UShort_t DetNbr22)	{fSpeg_DC_Strip22_DetNbr.push_back(DetNbr22);}
   void SetSpegDCStripNbr22(UShort_t StripNbr22)	{fSpeg_DC_Strip22_StripNbr.push_back(StripNbr22);}
   void SetSpegDCEnergy22(UShort_t Energy22)	{fSpeg_DC_Strip22_Energy.push_back(Energy22);}

   // E fil11
   void	SetSpegDCEfilDetNbr11(UShort_t DetNbr11)	{fSpeg_DC_Efil11_DetNbr.push_back(DetNbr11);}
   void SetSpegDCEfilEnergy11(UShort_t Energy11)	{fSpeg_DC_Efil11_Energy.push_back(Energy11);}

   // E fil12
   void	SetSpegDCEfilDetNbr12(UShort_t DetNbr12)	{fSpeg_DC_Efil12_DetNbr.push_back(DetNbr12);}
   void SetSpegDCEfilEnergy12(UShort_t Energy12)	{fSpeg_DC_Efil12_Energy.push_back(Energy12);}

   // E fil21
   void	SetSpegDCEfilDetNbr21(UShort_t DetNbr21)	{fSpeg_DC_Efil21_DetNbr.push_back(DetNbr21);}
   void SetSpegDCEfilEnergy21(UShort_t Energy21)	{fSpeg_DC_Efil21_Energy.push_back(Energy21);}

   // E fil22
   void	SetSpegDCEfilDetNbr22(UShort_t DetNbr22)	{fSpeg_DC_Efil22_DetNbr.push_back(DetNbr22);}
   void SetSpegDCEfilEnergy22(UShort_t Energy22)	{fSpeg_DC_Efil22_Energy.push_back(Energy22);}

   // Tplfil11
   void	SetSpegDCTplfilDetNbr11(UShort_t DetNbr11)	{fSpeg_DC_Tplfil11_DetNbr.push_back(DetNbr11);}
   void SetSpegDCTplfilTime11(UShort_t Time11)	{fSpeg_DC_Tplfil11_Time.push_back(Time11);}

   // Tplfil12
   void	SetSpegDCTplfilDetNbr12(UShort_t DetNbr12)	{fSpeg_DC_Tplfil12_DetNbr.push_back(DetNbr12);}
   void SetSpegDCTplfilTime12(UShort_t Time12)	{fSpeg_DC_Tplfil12_Time.push_back(Time12);}

   // Tplfil21
   void	SetSpegDCTplfilDetNbr21(UShort_t DetNbr21)	{fSpeg_DC_Tplfil21_DetNbr.push_back(DetNbr21);}
   void SetSpegDCTplfilTime21(UShort_t Time21)	{fSpeg_DC_Tplfil21_Time.push_back(Time21);}

   // Tplfil22
   void	SetSpegDCTplfilDetNbr22(UShort_t DetNbr22)	{fSpeg_DC_Tplfil22_DetNbr.push_back(DetNbr22);}
   void SetSpegDCTplfilTime22(UShort_t Time22)	{fSpeg_DC_Tplfil22_Time.push_back(Time22);}

   // TEMPS fil11
   void	SetSpegDCTfilDetNbr11(UShort_t DetNbr11)	{fSpeg_DC_Tfil11_DetNbr.push_back(DetNbr11);}
   void SetSpegDCTfilTime11(UShort_t Time11)	{fSpeg_DC_Tfil11_Time.push_back(Time11);}

   // TEMPS fil12
   void	SetSpegDCTfilDetNbr12(UShort_t DetNbr12)	{fSpeg_DC_Tfil12_DetNbr.push_back(DetNbr12);}
   void SetSpegDCTfilTime12(UShort_t Time12)	{fSpeg_DC_Tfil12_Time.push_back(Time12);}

   // TEMPS fil21
   void	SetSpegDCTfilDetNbr21(UShort_t DetNbr21)	{fSpeg_DC_Tfil21_DetNbr.push_back(DetNbr21);}
   void SetSpegDCTfilTime21(UShort_t Time21)	{fSpeg_DC_Tfil21_Time.push_back(Time21);}

   // TEMPS fil22
   void	SetSpegDCTfilDetNbr22(UShort_t DetNbr22)	{fSpeg_DC_Tfil22_DetNbr.push_back(DetNbr22);}
   void SetSpegDCTfilTime22(UShort_t Time22)	{fSpeg_DC_Tfil22_Time.push_back(Time22);}

   /////////////////////           GETTERS           ////////////////////////
   // Strips for CDM 11
   UShort_t	GetSpegDCMultStrip11()		{return fSpeg_DC_Strip11_DetNbr.size();}
   UShort_t	GetSpegDCDetNbr11(Int_t i)	{return fSpeg_DC_Strip11_DetNbr.at(i);}
   UShort_t	GetSpegDCStripNbr11(Int_t i)	{return fSpeg_DC_Strip11_StripNbr.at(i);}
   UShort_t	GetSpegDCEnergy11(Int_t i)	{return fSpeg_DC_Strip11_Energy.at(i);}
   // Strips for CDM 12
   UShort_t	GetSpegDCMultStrip12()		{return fSpeg_DC_Strip12_DetNbr.size();}
   UShort_t	GetSpegDCDetNbr12(Int_t i)	{return fSpeg_DC_Strip12_DetNbr.at(i);}
   UShort_t	GetSpegDCStripNbr12(Int_t i)	{return fSpeg_DC_Strip12_StripNbr.at(i);}
   UShort_t	GetSpegDCEnergy12(Int_t i)	{return fSpeg_DC_Strip12_Energy.at(i);}
   // Strips for CDM 21
   UShort_t	GetSpegDCMultStrip21()		{return fSpeg_DC_Strip21_DetNbr.size();}
   UShort_t	GetSpegDCDetNbr21(Int_t i)	{return fSpeg_DC_Strip21_DetNbr.at(i);}
   UShort_t	GetSpegDCStripNbr21(Int_t i)	{return fSpeg_DC_Strip21_StripNbr.at(i);}
   UShort_t	GetSpegDCEnergy21(Int_t i)	{return fSpeg_DC_Strip21_Energy.at(i);}
   // Strips for CDM 22
   UShort_t	GetSpegDCMultStrip22()		{return fSpeg_DC_Strip22_DetNbr.size();}
   UShort_t	GetSpegDCDetNbr22(Int_t i)	{return fSpeg_DC_Strip22_DetNbr.at(i);}
   UShort_t	GetSpegDCStripNbr22(Int_t i)	{return fSpeg_DC_Strip22_StripNbr.at(i);}
   UShort_t	GetSpegDCEnergy22(Int_t i)	{return fSpeg_DC_Strip22_Energy.at(i);}

   // E fil11
   UShort_t	GetSpegDCEfilMult11()		{return fSpeg_DC_Efil11_DetNbr.size();}
   UShort_t	GetSpegDCEfilDetNbr11(Int_t i)	{return fSpeg_DC_Efil11_DetNbr.at(i);}
   UShort_t	GetSpegDCEfilEnergy11(Int_t i)	{return fSpeg_DC_Efil11_Energy.at(i);}
   // E fil12
   UShort_t	GetSpegDCEfilMult12()		{return fSpeg_DC_Efil12_DetNbr.size();}
   UShort_t	GetSpegDCEfilDetNbr12(Int_t i)	{return fSpeg_DC_Efil12_DetNbr.at(i);}
   UShort_t	GetSpegDCEfilEnergy12(Int_t i)	{return fSpeg_DC_Efil12_Energy.at(i);}
   // E fil21
   UShort_t	GetSpegDCEfilMult21()		{return fSpeg_DC_Efil21_DetNbr.size();}
   UShort_t	GetSpegDCEfilDetNbr21(Int_t i)	{return fSpeg_DC_Efil21_DetNbr.at(i);}
   UShort_t	GetSpegDCEfilEnergy21(Int_t i)	{return fSpeg_DC_Efil21_Energy.at(i);}
   // E fil22
   UShort_t	GetSpegDCEfilMult22()		{return fSpeg_DC_Efil22_DetNbr.size();}
   UShort_t	GetSpegDCEfilDetNbr22(Int_t i)	{return fSpeg_DC_Efil22_DetNbr.at(i);}
   UShort_t	GetSpegDCEfilEnergy22(Int_t i)	{return fSpeg_DC_Efil22_Energy.at(i);}

   // Tplfil11
   UShort_t	GetSpegDCTplfilMult11()		{return fSpeg_DC_Tplfil11_DetNbr.size();}
   UShort_t	GetSpegDCTplfilDetNbr11(Int_t i)	{return fSpeg_DC_Tplfil11_DetNbr.at(i);}
   UShort_t	GetSpegDCTplfilTime11(Int_t i)	{return fSpeg_DC_Tplfil11_Time.at(i);}
   // Tplfil12
   UShort_t	GetSpegDCTplfilMult12()		{return fSpeg_DC_Tplfil12_DetNbr.size();}
   UShort_t	GetSpegDCTplfilDetNbr12(Int_t i)	{return fSpeg_DC_Tplfil12_DetNbr.at(i);}
   UShort_t	GetSpegDCTplfilTime12(Int_t i)	{return fSpeg_DC_Tplfil12_Time.at(i);}
   // Tplfil21
   UShort_t	GetSpegDCTplfilMult21()		{return fSpeg_DC_Tplfil21_DetNbr.size();}
   UShort_t	GetSpegDCTplfilDetNbr21(Int_t i)	{return fSpeg_DC_Tplfil21_DetNbr.at(i);}
   UShort_t	GetSpegDCTplfilTime21(Int_t i)	{return fSpeg_DC_Tplfil21_Time.at(i);}
   // Tplfil22
   UShort_t	GetSpegDCTplfilMult22()		{return fSpeg_DC_Tplfil22_DetNbr.size();}
   UShort_t	GetSpegDCTplfilDetNbr22(Int_t i)	{return fSpeg_DC_Tplfil22_DetNbr.at(i);}
   UShort_t	GetSpegDCTplfilTime22(Int_t i)	{return fSpeg_DC_Tplfil22_Time.at(i);}

   // TEMPS fil11
   UShort_t	GetSpegDCTfilMult11()		{return fSpeg_DC_Tfil11_DetNbr.size();}
   UShort_t	GetSpegDCTfilDetNbr11(Int_t i)	{return fSpeg_DC_Tfil11_DetNbr.at(i);}
   UShort_t	GetSpegDCTfilTime11(Int_t i)	{return fSpeg_DC_Tfil11_Time.at(i);}
   // TEMPS fil12
   UShort_t	GetSpegDCTfilMult12()		{return fSpeg_DC_Tfil12_DetNbr.size();}
   UShort_t	GetSpegDCTfilDetNbr12(Int_t i)	{return fSpeg_DC_Tfil12_DetNbr.at(i);}
   UShort_t	GetSpegDCTfilTime12(Int_t i)	{return fSpeg_DC_Tfil12_Time.at(i);}
   // TEMPS fil21
   UShort_t	GetSpegDCTfilMult21()		{return fSpeg_DC_Tfil21_DetNbr.size();}
   UShort_t	GetSpegDCTfilDetNbr21(Int_t i)	{return fSpeg_DC_Tfil21_DetNbr.at(i);}
   UShort_t	GetSpegDCTfilTime21(Int_t i)	{return fSpeg_DC_Tfil21_Time.at(i);}
   // TEMPS fil22
   UShort_t	GetSpegDCTfilMult22()		{return fSpeg_DC_Tfil22_DetNbr.size();}
   UShort_t	GetSpegDCTfilDetNbr22(Int_t i)	{return fSpeg_DC_Tfil22_DetNbr.at(i);}
   UShort_t	GetSpegDCTfilTime22(Int_t i)	{return fSpeg_DC_Tfil22_Time.at(i);}

   ClassDef(TSpegDCData,1)  // SpegDCData structure
};

#endif
