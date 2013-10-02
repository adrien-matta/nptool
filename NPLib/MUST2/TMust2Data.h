#ifndef __MUST2DATA__
#define __MUST2DATA__

#include <vector>
#include "TObject.h"

using namespace std ;
class TMust2Data : public TObject {
	private:
		// DSSD
		// X strips
		// Energy
		vector<UShort_t>	fMM_StripXE_DetectorNbr;
		vector<UShort_t>	fMM_StripXE_StripNbr;
		vector<Double_t>	fMM_StripXE_Energy;
		// Time
		vector<UShort_t>	fMM_StripXT_DetectorNbr;
		vector<UShort_t>	fMM_StripXT_StripNbr;
		vector<Double_t>	fMM_StripXT_Time;
		// Y strips
		// Energy
		vector<UShort_t>	fMM_StripYE_DetectorNbr;
		vector<UShort_t>	fMM_StripYE_StripNbr;
		vector<Double_t>	fMM_StripYE_Energy;
		// Time
		vector<UShort_t>	fMM_StripYT_DetectorNbr;
		vector<UShort_t>	fMM_StripYT_StripNbr;
		vector<Double_t>	fMM_StripYT_Time;

		// SiLi
		// Energy
		vector<UShort_t>	fMM_SiLiE_DetectorNbr;
		vector<UShort_t>	fMM_SiLiE_PadNbr;
		vector<Double_t>	fMM_SiLiE_Energy;
		// Time
		vector<UShort_t>	fMM_SiLiT_DetectorNbr;
		vector<UShort_t>	fMM_SiLiT_PadNbr;
		vector<Double_t>	fMM_SiLiT_Time;

		// CsI
		// Energy
		vector<UShort_t>	fMM_CsIE_DetectorNbr;
		vector<UShort_t>	fMM_CsIE_CristalNbr;
		vector<Double_t>	fMM_CsIE_Energy;
		// Time
		vector<UShort_t>	fMM_CsIT_DetectorNbr;
		vector<UShort_t>	fMM_CsIT_CristalNbr;
		vector<Double_t>	fMM_CsIT_Time;

	public:
		TMust2Data();
		virtual ~TMust2Data();

		void	Clear();
		void	Dump();

		/////////////////////           SETTERS           ////////////////////////
		// DSSD
		// (X,E)
		void	SetMMStripXEDetectorNbr(UShort_t DetNbr) {fMM_StripXE_DetectorNbr.push_back(DetNbr);}
		void	SetMMStripXEStripNbr(UShort_t StripNbr)  {fMM_StripXE_StripNbr.push_back(StripNbr);}
		void	SetMMStripXEEnergy(Double_t Energy)      {fMM_StripXE_Energy.push_back(Energy);}
		// (X,T)
		void	SetMMStripXTDetectorNbr(UShort_t DetNbr) {fMM_StripXT_DetectorNbr.push_back(DetNbr);}
		void	SetMMStripXTStripNbr(UShort_t StripNbr)  {fMM_StripXT_StripNbr.push_back(StripNbr);}
		void	SetMMStripXTTime(Double_t Time)          {fMM_StripXT_Time.push_back(Time);}
		// (Y,E)
		void	SetMMStripYEDetectorNbr(UShort_t DetNbr) {fMM_StripYE_DetectorNbr.push_back(DetNbr);}
		void	SetMMStripYEStripNbr(UShort_t StripNbr)  {fMM_StripYE_StripNbr.push_back(StripNbr);}
		void	SetMMStripYEEnergy(Double_t Energy)      {fMM_StripYE_Energy.push_back(Energy);}
		// (Y,T)
		void	SetMMStripYTDetectorNbr(UShort_t DetNbr) {fMM_StripYT_DetectorNbr.push_back(DetNbr);}
		void	SetMMStripYTStripNbr(UShort_t StripNbr)  {fMM_StripYT_StripNbr.push_back(StripNbr);}
		void	SetMMStripYTTime(Double_t Time)          {fMM_StripYT_Time.push_back(Time);}

		// SiLi
		//(E)
		void	SetMMSiLiEDetectorNbr(UShort_t DetNbr)   {fMM_SiLiE_DetectorNbr.push_back(DetNbr);}
		void	SetMMSiLiEPadNbr(UShort_t PadNbr)        {fMM_SiLiE_PadNbr.push_back(PadNbr);}
		void	SetMMSiLiEEnergy(Double_t Energy)        {fMM_SiLiE_Energy.push_back(Energy);}
		//(T)
		void	SetMMSiLiTDetectorNbr(UShort_t DetNbr)   {fMM_SiLiT_DetectorNbr.push_back(DetNbr);}
		void	SetMMSiLiTPadNbr(UShort_t PadNbr)        {fMM_SiLiT_PadNbr.push_back(PadNbr);}
		void	SetMMSiLiTTime(Double_t Time)            {fMM_SiLiT_Time.push_back(Time);}

		// CsI
		// (E)
		void	SetMMCsIEDetectorNbr(UShort_t DetNbr)    {fMM_CsIE_DetectorNbr.push_back(DetNbr);}
		void	SetMMCsIECristalNbr(UShort_t CristalNbr) {fMM_CsIE_CristalNbr.push_back(CristalNbr);}
		void	SetMMCsIEEnergy(Double_t Energy)         {fMM_CsIE_Energy.push_back(Energy);}
		// (T)
		void	SetMMCsITDetectorNbr(UShort_t DetNbr)    {fMM_CsIT_DetectorNbr.push_back(DetNbr);}
		void	SetMMCsITCristalNbr(UShort_t CristalNbr) {fMM_CsIT_CristalNbr.push_back(CristalNbr);}
		void	SetMMCsITTime(Double_t Time)             {fMM_CsIT_Time.push_back(Time);}

		/////////////////////           GETTERS           ////////////////////////
		// DSSD
		// (X,E)
		UShort_t	GetMMStripXEMult()               {return fMM_StripXE_DetectorNbr.size();}
		UShort_t	GetMMStripXEDetectorNbr(Int_t i) {return fMM_StripXE_DetectorNbr.at(i);}
		UShort_t	GetMMStripXEStripNbr(Int_t i)    {return fMM_StripXE_StripNbr.at(i);}
		Double_t	GetMMStripXEEnergy(Int_t i)      {return fMM_StripXE_Energy.at(i);}
		// (X,T)
		UShort_t	GetMMStripXTMult()               {return fMM_StripXT_DetectorNbr.size();}
		UShort_t	GetMMStripXTDetectorNbr(Int_t i) {return fMM_StripXT_DetectorNbr.at(i);}
		UShort_t	GetMMStripXTStripNbr(Int_t i)    {return fMM_StripXT_StripNbr.at(i);}
		Double_t	GetMMStripXTTime(Int_t i)        {return fMM_StripXT_Time.at(i);}
		// (Y,E)
		UShort_t	GetMMStripYEMult()               {return fMM_StripYE_DetectorNbr.size();}
		UShort_t	GetMMStripYEDetectorNbr(Int_t i) {return fMM_StripYE_DetectorNbr.at(i);}
		UShort_t	GetMMStripYEStripNbr(Int_t i)    {return fMM_StripYE_StripNbr.at(i);}
		Double_t	GetMMStripYEEnergy(Int_t i)      {return fMM_StripYE_Energy.at(i);}
		// (Y,T)
		UShort_t	GetMMStripYTMult()               {return fMM_StripYT_DetectorNbr.size();}
		UShort_t	GetMMStripYTDetectorNbr(Int_t i) {return fMM_StripYT_DetectorNbr.at(i);}
		UShort_t	GetMMStripYTStripNbr(Int_t i)    {return fMM_StripYT_StripNbr.at(i);}
		Double_t	GetMMStripYTTime(Int_t i)        {return fMM_StripYT_Time.at(i);}

		// SiLi
		//(E)
		UShort_t	GetMMSiLiEMult()				{return fMM_SiLiE_DetectorNbr.size();}
		UShort_t	GetMMSiLiEDetectorNbr(Int_t i)  {return fMM_SiLiE_DetectorNbr.at(i);}
		UShort_t	GetMMSiLiEPadNbr(Int_t i)       {return fMM_SiLiE_PadNbr.at(i);}
		Double_t	GetMMSiLiEEnergy(Int_t i)       {return fMM_SiLiE_Energy.at(i);}
		//(T)
		UShort_t	GetMMSiLiTMult()				{return fMM_SiLiT_DetectorNbr.size();}
		UShort_t	GetMMSiLiTDetectorNbr(Int_t i)  {return fMM_SiLiT_DetectorNbr.at(i);}
		UShort_t	GetMMSiLiTPadNbr(Int_t i)       {return fMM_SiLiT_PadNbr.at(i);}
		Double_t	GetMMSiLiTTime(Int_t i)         {return fMM_SiLiT_Time.at(i);}

		// CsI 
		//(E)
		UShort_t	GetMMCsIEMult()					{return fMM_CsIE_DetectorNbr.size();}
		UShort_t	GetMMCsIEDetectorNbr(Int_t i)   {return fMM_CsIE_DetectorNbr.at(i);}
		UShort_t	GetMMCsIECristalNbr(Int_t i)    {return fMM_CsIE_CristalNbr.at(i);}
		Double_t	GetMMCsIEEnergy(Int_t i)        {return fMM_CsIE_Energy.at(i);}
		//(T)
		UShort_t	GetMMCsITMult()					{return fMM_CsIT_DetectorNbr.size();}
		UShort_t	GetMMCsITDetectorNbr(Int_t i)   {return fMM_CsIT_DetectorNbr.at(i);}
		UShort_t	GetMMCsITCristalNbr(Int_t i)    {return fMM_CsIT_CristalNbr.at(i);}
		Double_t	GetMMCsITTime(Int_t i)          {return fMM_CsIT_Time.at(i);}

		ClassDef(TMust2Data,1)  // Must2Data structure
};

#endif
