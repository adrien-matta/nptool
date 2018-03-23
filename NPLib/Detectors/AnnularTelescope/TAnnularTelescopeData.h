#ifndef AnnularTelescopeDATA_Include_Guard
#define AnnularTelescopeDATA_Include_Guard
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : March 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularTelescope Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// STL
#include <vector>
using namespace std;

// ROOT
#include "TObject.h"

class TAnnularTelescopeData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
private: 
	// CsI Energy
	vector<UShort_t>   fAnnularTelescope_CsI_E_DetectorNbr;
	vector<UShort_t>   fAnnularTelescope_CsI_E_WedgeNbr;
	vector<Double_t>   fAnnularTelescope_CsI_Energy;

	// Si Energy
	vector<UShort_t>   fAnnularTelescope_Si_E_DetectorNbr;
	vector<UShort_t>   fAnnularTelescope_Si_E_ThetaStripNbr;
	vector<UShort_t>   fAnnularTelescope_Si_E_PhiStripNbr;
	vector<Double_t>   fAnnularTelescope_Si_Energy;
	
	// CsI Time
	vector<UShort_t>   fAnnularTelescope_CsI_T_DetectorNbr;
	vector<UShort_t>   fAnnularTelescope_CsI_T_WedgeNbr;
	vector<Double_t>   fAnnularTelescope_CsI_Time;

	// Si Time
	vector<UShort_t>   fAnnularTelescope_Si_T_DetectorNbr;
	vector<UShort_t>   fAnnularTelescope_Si_T_ThetaStripNbr;
	vector<UShort_t>   fAnnularTelescope_Si_T_PhiStripNbr;
	vector<Double_t>   fAnnularTelescope_Si_Time;


	
  //////////////////////////////////////////////////////////////
  // Constructor and destructor
public: 
	TAnnularTelescopeData();
	~TAnnularTelescopeData();
    

  //////////////////////////////////////////////////////////////
  // Inherited from TObject and overriden to avoid warnings
public:
	void Clear();
	void Clear(const Option_t*) {};
	void Dump() const;


  //////////////////////////////////////////////////////////////
  // Getters and Setters
  // Prefer inline declaration to avoid unnecessary called of 
  // frequently used methods
  // add //! to avoid ROOT creating dictionnary for the methods
public:
	//////////////////////    SETTERS    ////////////////////////
	// CsI Energy
	void SetCsIEnergy(UShort_t DetNbr, UShort_t WedgeNbr, Double_t Energy){
		fAnnularTelescope_CsI_E_DetectorNbr.push_back(DetNbr);
		fAnnularTelescope_CsI_E_WedgeNbr.push_back(WedgeNbr);
		fAnnularTelescope_CsI_Energy.push_back(Energy);
	};//!

    // CsI Time
	void SetCsITime(UShort_t DetNbr, UShort_t WedgeNbr, Double_t Time)	{
		fAnnularTelescope_CsI_T_DetectorNbr.push_back(DetNbr);     
		fAnnularTelescope_CsI_T_WedgeNbr.push_back(WedgeNbr);
		fAnnularTelescope_CsI_Time.push_back(Time);
	};//!

	// Si Energy
	void SetSiEnergy(UShort_t DetNbr, UShort_t ThetaStripNbr, UShort_t PhiStripNbr, Double_t Energy) {
		fAnnularTelescope_Si_E_DetectorNbr.push_back(DetNbr);
		fAnnularTelescope_Si_E_ThetaStripNbr.push_back(ThetaStripNbr);
		fAnnularTelescope_Si_E_PhiStripNbr.push_back(PhiStripNbr);
		fAnnularTelescope_Si_Energy.push_back(Energy);
	}

	// Si Time
	void SetSiTime(UShort_t DetNbr, UShort_t ThetaStripNbr, UShort_t PhiStripNbr, Double_t Time) {
		fAnnularTelescope_Si_T_DetectorNbr.push_back(DetNbr);
		fAnnularTelescope_Si_T_ThetaStripNbr.push_back(ThetaStripNbr);
		fAnnularTelescope_Si_T_PhiStripNbr.push_back(PhiStripNbr);
		fAnnularTelescope_Si_Time.push_back(Time);
	}



    //////////////////////    GETTERS    ////////////////////////
    // CsI Energy
	UShort_t GetCsIMultEnergy() const
		{return fAnnularTelescope_CsI_E_DetectorNbr.size();}
	UShort_t GetCsIE_DetectorNbr(const unsigned int &i) const 
		{return fAnnularTelescope_CsI_E_DetectorNbr[i];}//!
	UShort_t GetCsIE_WedgeNbr(const unsigned int &i) const 
		{return fAnnularTelescope_CsI_E_WedgeNbr[i];}//!
	Double_t GetCsIEnergy(const unsigned int &i) const 
		{return fAnnularTelescope_CsI_Energy[i];}//!

	// CsI Time
	UShort_t GetCsIMultTime() const
		{return fAnnularTelescope_CsI_T_DetectorNbr.size();}
	UShort_t GetCsIT_DetectorNbr(const unsigned int &i) const 
		{return fAnnularTelescope_CsI_T_DetectorNbr[i];}//!
	UShort_t GetCsIT_WedgeNbr(const unsigned int &i) const 
		{return fAnnularTelescope_CsI_T_WedgeNbr[i];}//!
	Double_t GetCsITime(const unsigned int &i) const 
		{return fAnnularTelescope_CsI_Time[i];}//!
	
	// Si Energy
	UShort_t GetSiMultEnergy() const
		{return fAnnularTelescope_Si_E_DetectorNbr.size();}
	UShort_t GetSiE_DetectorNbr(const unsigned int &i) const 
		{return fAnnularTelescope_Si_E_DetectorNbr[i];}//!
	UShort_t GetSiE_ThetaStripNbr(const unsigned int &i) const 
		{return fAnnularTelescope_Si_E_ThetaStripNbr[i];}//!
	UShort_t GetSiE_PhiStripNbr(const unsigned int &i) const 
		{return fAnnularTelescope_Si_E_PhiStripNbr[i];}//!
	Double_t GetSiEnergy(const unsigned int &i) const 
		{return fAnnularTelescope_Si_Energy[i];}//!

	// Si Time
	UShort_t GetSiMultTime() const
		{return fAnnularTelescope_Si_T_DetectorNbr.size();}
	UShort_t GetSiT_DetectorNbr(const unsigned int &i) const 
		{return fAnnularTelescope_Si_T_DetectorNbr[i];}//!
	UShort_t GetSiT_ThetaStripNbr(const unsigned int &i) const 
		{return fAnnularTelescope_Si_T_ThetaStripNbr[i];}//!
	UShort_t GetSiT_PhiStripNbr(const unsigned int &i) const 
		{return fAnnularTelescope_Si_T_PhiStripNbr[i];}//!
	Double_t GetSiTime(const unsigned int &i) const 
		{return fAnnularTelescope_Si_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TAnnularTelescopeData,1)  // AnnularTelescopeData structure
};

#endif
/* Local Variables:  */
/* mode: c++         */
/* End:              */
