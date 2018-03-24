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
 *  This class hold AnnularTelescope Raw data                                *
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

namespace ANNULAR_TELESCOPE_DATA {

struct CsIHit_t {
	std::vector<UShort_t> Detector; // detector number
	std::vector<UShort_t> Wedge;    // wedge number
	std::vector<Double_t> Value;    // value (energy or time)
};

struct SiHit_t {
	std::vector<UShort_t> Detector;   // detector number
	std::vector<UShort_t> Strip;      // radial strip number
	std::vector<Double_t> Value;      // value (energy or time)
};

}

class TAnnularTelescopeData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
private: 
	// CsI Energy
	ANNULAR_TELESCOPE_DATA::CsIHit_t CsI_E;
	// CsI Time
	ANNULAR_TELESCOPE_DATA::CsIHit_t CsI_T;
	// Si Energy (theta strips)
	ANNULAR_TELESCOPE_DATA::SiHit_t Si_Theta_E;
	// Si Time (theta strips)
	ANNULAR_TELESCOPE_DATA::SiHit_t Si_Theta_T;
	// Si Energy (phi strips)
	ANNULAR_TELESCOPE_DATA::SiHit_t Si_Phi_E;
	// Si Time (phi strips)
	ANNULAR_TELESCOPE_DATA::SiHit_t Si_Phi_T;
	
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

	// Si Energy (theta strips)
	void SetSiThetaEnergy(UShort_t DetNbr, UShort_t StripNbr, Double_t Energy) {
		Si_Theta_E.Detector.push_back(DetNbr);
		Si_Theta_E.Strip.push_back(StripNbr);
		Si_Theta_E.Value.push_back(Energy);
	}

	// Si Time (theta strips)
	void SetSiThetaTime(UShort_t DetNbr, UShort_t StripNbr, Double_t Time) {
		Si_Theta_T.Detector.push_back(DetNbr);
		Si_Theta_T.Strip.push_back(StripNbr);
		Si_Theta_T.Value.push_back(Time);
	}

	// Si Energy (phi strips)
	void SetSiPhiEnergy(UShort_t DetNbr, UShort_t StripNbr, Double_t Energy) {
		Si_Phi_E.Detector.push_back(DetNbr);
		Si_Phi_E.Strip.push_back(StripNbr);
		Si_Phi_E.Value.push_back(Energy);
	}

	// Si Time (phi strips)
	void SetSiPhiTime(UShort_t DetNbr, UShort_t StripNbr, Double_t Time) {
		Si_Phi_T.Detector.push_back(DetNbr);
		Si_Phi_T.Strip.push_back(StripNbr);
		Si_Phi_T.Value.push_back(Time);
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
	
	// Si Energy (theta strips)
	UShort_t GetSiThetaE_Mult() const
		{return Si_Theta_E.Detector.size();}
	UShort_t GetSiThetaE_DetectorNbr(const unsigned int &i) const 
		{return Si_Theta_E.Detector[i];}//!
	UShort_t GetSiThetaE_StripNbr(const unsigned int &i) const 
		{return Si_Theta_E.Strip[i];}//!
	Double_t GetSiThetaE_Energy(const unsigned int &i) const 
		{return Si_Theta_E.Value[i];}//!

	// Si Time (theta strips)
	UShort_t GetSiThetaT_Mult() const
		{return Si_Theta_T.Detector.size();}
	UShort_t GetSiThetaT_DetectorNbr(const unsigned int &i) const 
		{return Si_Theta_T.Detector[i];}//!
	UShort_t GetSiThetaT_StripNbr(const unsigned int &i) const 
		{return Si_Theta_T.Strip[i];}//!
	Double_t GetSiThetaT_Time(const unsigned int &i) const 
		{return Si_Theta_T.Value[i];}//!

		// Si Energy (phi strips)
	UShort_t GetSiPhiE_Mult() const
		{return Si_Phi_E.Detector.size();}
	UShort_t GetSiPhiE_DetectorNbr(const unsigned int &i) const 
		{return Si_Phi_E.Detector[i];}//!
	UShort_t GetSiPhiE_StripNbr(const unsigned int &i) const 
		{return Si_Phi_E.Strip[i];}//!
	Double_t GetSiPhiE_Energy(const unsigned int &i) const 
		{return Si_Phi_E.Value[i];}//!

	// Si Time (phi strips)
	UShort_t GetSiPhiT_Mult() const
		{return Si_Phi_T.Detector.size();}
	UShort_t GetSiPhiT_DetectorNbr(const unsigned int &i) const 
		{return Si_Phi_T.Detector[i];}//!
	UShort_t GetSiPhiT_StripNbr(const unsigned int &i) const 
		{return Si_Phi_T.Strip[i];}//!
	Double_t GetSiPhiT_Time(const unsigned int &i) const 
		{return Si_Phi_T.Value[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TAnnularTelescopeData,1)  // AnnularTelescopeData structure
};

#endif
/* Local Variables:  */
/* mode: c++         */
/* End:              */
