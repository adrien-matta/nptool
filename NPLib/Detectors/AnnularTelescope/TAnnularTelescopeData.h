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

// ROOT
#include "TObject.h"

namespace ANNULAR_TELESCOPE {

struct CsIHit_t {
	std::vector<UShort_t> Detector; // detector number
	std::vector<UShort_t> Wedge;    // wedge number
	std::vector<Double_t> Value;    // value (energy or time)
};

struct SiHit_t {
	std::vector<UShort_t> Detector;   // detector number
	std::vector<UShort_t> ThetaStrip; // radial strip number
	std::vector<UShort_t> PhiStrip;   // azimuthal strip number
	std::vector<Double_t> Value;      // value (energy or time)
};

}

class TAnnularTelescopeData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
private: 
	// CsI Energy
	ANNULAR_TELESCOPE::CsIHit_t CsI_E;
	// CsI Time
	ANNULAR_TELESCOPE::CsIHit_t CsI_T;
	// Si Energy
	ANNULAR_TELESCOPE::SiHit_t Si_E;
	// Si Time
	ANNULAR_TELESCOPE::SiHit_t Si_T;
	
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
		CsI_E.Detector.push_back(DetNbr);
		CsI_E.Wedge.push_back(WedgeNbr);
		CsI_E.Value.push_back(Energy);
	};//!

    // CsI Time
	void SetCsITime(UShort_t DetNbr, UShort_t WedgeNbr, Double_t Time)	{
		CsI_T.Detector.push_back(DetNbr);
		CsI_T.Wedge.push_back(WedgeNbr);
		CsI_T.Value.push_back(Time);
	};//!

	// Si Energy
	void SetSiEnergy(UShort_t DetNbr, UShort_t ThetaStripNbr, UShort_t PhiStripNbr, Double_t Energy) {
		Si_E.Detector.push_back(DetNbr);
		Si_E.ThetaStrip.push_back(ThetaStripNbr);
		Si_E.PhiStrip.push_back(PhiStripNbr);
		Si_E.Value.push_back(Energy);
	}

	// Si Time
	void SetSiTime(UShort_t DetNbr, UShort_t ThetaStripNbr, UShort_t PhiStripNbr, Double_t Time) {
		Si_T.Detector.push_back(DetNbr);
		Si_T.ThetaStrip.push_back(ThetaStripNbr);
		Si_T.PhiStrip.push_back(PhiStripNbr);
		Si_T.Value.push_back(Time);
	}

	//////////////////////    GETTERS    ////////////////////////
	// CsI Energy
	UShort_t GetCsIMultEnergy() const
		{return CsI_E.Detector.size();}
	UShort_t GetCsIE_DetectorNbr(const unsigned int &i) const 
		{return CsI_E.Detector[i];}//!
	UShort_t GetCsIE_WedgeNbr(const unsigned int &i) const 
		{return CsI_E.Wedge[i];}//!
	Double_t GetCsIEnergy(const unsigned int &i) const 
		{return CsI_E.Value[i];}//!

	// CsI Time
	UShort_t GetCsIMultTime() const
		{return CsI_T.Detector.size();}
	UShort_t GetCsIT_DetectorNbr(const unsigned int &i) const 
		{return CsI_T.Detector[i];}//!
	UShort_t GetCsIT_WedgeNbr(const unsigned int &i) const 
		{return CsI_T.Wedge[i];}//!
	Double_t GetCsITime(const unsigned int &i) const 
		{return CsI_T.Value[i];}//!
	
	// Si Energy
	UShort_t GetSiMultEnergy() const
		{return Si_E.Detector.size();}
	UShort_t GetSiE_DetectorNbr(const unsigned int &i) const 
		{return Si_E.Detector[i];}//!
	UShort_t GetSiE_ThetaStripNbr(const unsigned int &i) const 
		{return Si_E.ThetaStrip[i];}//!
	UShort_t GetSiE_PhiStripNbr(const unsigned int &i) const 
		{return Si_E.PhiStrip[i];}//!
	Double_t GetSiEnergy(const unsigned int &i) const 
		{return Si_E.Value[i];}//!

	// Si Time
	UShort_t GetSiMultTime() const
		{return Si_T.Detector.size();}
	UShort_t GetSiT_DetectorNbr(const unsigned int &i) const 
		{return Si_T.Detector[i];}//!
	UShort_t GetSiT_ThetaStripNbr(const unsigned int &i) const 
		{return Si_T.ThetaStrip[i];}//!
	UShort_t GetSiT_PhiStripNbr(const unsigned int &i) const 
		{return Si_T.PhiStrip[i];}//!
	Double_t GetSiTime(const unsigned int &i) const 
		{return Si_T.Value[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TAnnularTelescopeData,1)  // AnnularTelescopeData structure
};

#endif
/* Local Variables:  */
/* mode: c++         */
/* End:              */
