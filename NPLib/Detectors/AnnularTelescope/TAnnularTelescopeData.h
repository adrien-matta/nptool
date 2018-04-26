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
#include <iostream>

// ROOT
#include "TObject.h"

class TAnnularTelescopeData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
private:
// CsI Energy
	std::vector<UShort_t> CsI_E_Detector; // detector number
	std::vector<UShort_t> CsI_E_Wedge;    // wedge number
	std::vector<Double_t> CsI_E_Energy;   // value (energy or time)
// CsI Time
	std::vector<UShort_t> CsI_T_Detector; // detector number
	std::vector<UShort_t> CsI_T_Wedge;    // wedge number
	std::vector<Double_t> CsI_T_Time;     // value (energy or time)
// Si Energy (theta strips)
	std::vector<UShort_t> Si_Theta_E_Detector;   // detector number
	std::vector<UShort_t> Si_Theta_E_Strip;      // radial strip number
	std::vector<Double_t> Si_Theta_E_Energy;     // value (energy or time)
// Si Time (theta strips)
	std::vector<UShort_t> Si_Theta_T_Detector;   // detector number
	std::vector<UShort_t> Si_Theta_T_Strip;      // radial strip number
	std::vector<Double_t> Si_Theta_T_Time;       // value (energy or time)
// Si Energy (phi strips)
	std::vector<UShort_t> Si_Phi_E_Detector;   // detector number
	std::vector<UShort_t> Si_Phi_E_Strip;      // radial strip number
	std::vector<Double_t> Si_Phi_E_Energy;     // value (energy or time)
// Si Time (phi strips)
	std::vector<UShort_t> Si_Phi_T_Detector;   // detector number
	std::vector<UShort_t> Si_Phi_T_Strip;      // radial strip number
	std::vector<Double_t> Si_Phi_T_Time;       // value (energy or time)
	
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
		CsI_E_Detector.push_back(DetNbr);
		CsI_E_Wedge.push_back(WedgeNbr);
		CsI_E_Energy.push_back(Energy);
	};//!

    // CsI Time
	void SetCsITime(UShort_t DetNbr, UShort_t WedgeNbr, Double_t Time)	{
		CsI_T_Detector.push_back(DetNbr);
		CsI_T_Wedge.push_back(WedgeNbr);
		CsI_T_Time.push_back(Time);
	};//!

	// Si Energy (theta strips)
	void SetSiThetaEnergy(UShort_t DetNbr, UShort_t StripNbr, Double_t Energy) {
		Si_Theta_E_Detector.push_back(DetNbr);
		Si_Theta_E_Strip.push_back(StripNbr);
		Si_Theta_E_Energy.push_back(Energy);
	}

	// Si Time (theta strips)
	void SetSiThetaTime(UShort_t DetNbr, UShort_t StripNbr, Double_t Time) {
		Si_Theta_T_Detector.push_back(DetNbr);
		Si_Theta_T_Strip.push_back(StripNbr);
		Si_Theta_T_Time.push_back(Time);
	}

	// Si Energy (phi strips)
	void SetSiPhiEnergy(UShort_t DetNbr, UShort_t StripNbr, Double_t Energy) {
		Si_Phi_E_Detector.push_back(DetNbr);
		Si_Phi_E_Strip.push_back(StripNbr);
		Si_Phi_E_Energy.push_back(Energy);
	}

	// Si Time (phi strips)
	void SetSiPhiTime(UShort_t DetNbr, UShort_t StripNbr, Double_t Time) {
		Si_Phi_T_Detector.push_back(DetNbr);
		Si_Phi_T_Strip.push_back(StripNbr);
		Si_Phi_T_Time.push_back(Time);
	}

	//////////////////////    GETTERS    ////////////////////////
	/////////////////////////////////////////////////////////////
	// Helper function to catch errors better
	template <class T>
	T GetArrayVal(const std::vector<T>& v, size_t i, const std::string& name) const {
		try { return v.at(i); }
		catch(std::exception& e) {
			std::cerr << "ERROR in AnnularTelescopeData, can't get value of "
								<< name << " at index " << i << std::endl;
			exit(1);
		}
	} //!
	
	// CsI Energy
	UShort_t GetCsIMultEnergy() const
		{return CsI_E_Detector.size();}
	UShort_t GetCsIE_DetectorNbr(const unsigned int &i) const 
		{return GetArrayVal(CsI_E_Detector,i,"CsI_E_Detector");}//!
	UShort_t GetCsIE_WedgeNbr(const unsigned int &i) const 
		{return GetArrayVal(CsI_E_Wedge,i,"CsI_E_Wedge");}//!
	Double_t GetCsIEnergy(const unsigned int &i) const 
		{return GetArrayVal(CsI_E_Energy,i,"CsI_E_Energy");}//!

	// CsI Time
	UShort_t GetCsIMultTime() const
		{return CsI_T_Detector.size();}
	UShort_t GetCsIT_DetectorNbr(const unsigned int &i) const 
		{return GetArrayVal(CsI_T_Detector,i,"CsI_T_Detector");}//!
	UShort_t GetCsIT_WedgeNbr(const unsigned int &i) const 
		{return GetArrayVal(CsI_T_Wedge,i,"CsI_T_Wedge");}//!
	Double_t GetCsITime(const unsigned int &i) const 
		{return GetArrayVal(CsI_T_Time,i,"CsI_T_Time");}//!
	
	// Si Energy (theta strips)
	UShort_t GetSiThetaE_Mult() const
		{return Si_Theta_E_Detector.size();}
	UShort_t GetSiThetaE_DetectorNbr(const unsigned int &i) const 
		{return GetArrayVal(Si_Theta_E_Detector,i,"Si_Theta_E_Detector");}//!
	UShort_t GetSiThetaE_StripNbr(const unsigned int &i) const 
		{return GetArrayVal(Si_Theta_E_Strip,i,"Si_Theta_E_Strip");}//!
	Double_t GetSiThetaE_Energy(const unsigned int &i) const 
		{return GetArrayVal(Si_Theta_E_Energy,i,"Si_Theta_E_Energy");}//!

	// Si Time (theta strips)
	UShort_t GetSiThetaT_Mult() const
		{return Si_Theta_T_Detector.size();}
	UShort_t GetSiThetaT_DetectorNbr(const unsigned int &i) const 
		{return GetArrayVal(Si_Theta_T_Detector,i,"Si_Theta_T_Detecor");}//!
	UShort_t GetSiThetaT_StripNbr(const unsigned int &i) const 
		{return GetArrayVal(Si_Theta_T_Strip,i,"Si_Theta_T_Strip");}//!
	Double_t GetSiThetaT_Time(const unsigned int &i) const 
		{return GetArrayVal(Si_Theta_T_Time,i,"Si_Theta_T_Time");}//!

		// Si Energy (phi strips)
	UShort_t GetSiPhiE_Mult() const
		{return Si_Phi_E_Detector.size();}
	UShort_t GetSiPhiE_DetectorNbr(const unsigned int &i) const 
		{return GetArrayVal(Si_Phi_E_Detector,i,"Si_Phi_E_Detector");}//!
	UShort_t GetSiPhiE_StripNbr(const unsigned int &i) const 
		{return GetArrayVal(Si_Phi_E_Strip,i,"Si_Phi_E_Strip");}//!
	Double_t GetSiPhiE_Energy(const unsigned int &i) const 
		{return GetArrayVal(Si_Phi_E_Energy,i,"Si_Phi_E_Energy");}//!

	// Si Time (phi strips)
	UShort_t GetSiPhiT_Mult() const
		{return Si_Phi_T_Detector.size();}
	UShort_t GetSiPhiT_DetectorNbr(const unsigned int &i) const 
		{return GetArrayVal(Si_Phi_T_Detector,i,"Si_Phi_T_Detecor");}//!
	UShort_t GetSiPhiT_StripNbr(const unsigned int &i) const 
		{return GetArrayVal(Si_Phi_T_Strip,i,"Si_Phi_T_Strip");}//!
	Double_t GetSiPhiT_Time(const unsigned int &i) const 
		{return GetArrayVal(Si_Phi_T_Time,i,"Si_Phi_T_Time");}//!

  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TAnnularTelescopeData,1)  // AnnularTelescopeData structure
};

#endif
/* Local Variables:  */
/* mode: c++         */
/* End:              */
