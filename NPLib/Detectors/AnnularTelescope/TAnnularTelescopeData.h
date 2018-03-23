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
	vector<Double_t>   fAnnularTelescope_CsI_Energy;

	// Time
	vector<UShort_t>   fAnnularTelescope_CsI_T_DetectorNbr;
	vector<Double_t>   fAnnularTelescope_CsI_Time;


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
	inline void SetCsIEnergy(const UShort_t& DetNbr,const Double_t& Energy){
		fAnnularTelescope_CsI_E_DetectorNbr.push_back(DetNbr);
		fAnnularTelescope_CsI_Energy.push_back(Energy);
	};//!

    // CsI Time
	inline void SetCsITime(const UShort_t& DetNbr,const Double_t& Time)	{
		fAnnularTelescope_CsI_T_DetectorNbr.push_back(DetNbr);     
		fAnnularTelescope_CsI_Time.push_back(Time);
	};//!


    //////////////////////    GETTERS    ////////////////////////
    // Energy
	inline UShort_t GetCsIMultEnergy() const
		{return fAnnularTelescope_CsI_E_DetectorNbr.size();}
	inline UShort_t GetCsIE_DetectorNbr(const unsigned int &i) const 
		{return fAnnularTelescope_CsI_E_DetectorNbr[i];}//!
	inline Double_t GetCsIEnergy(const unsigned int &i) const 
		{return fAnnularTelescope_CsI_Energy[i];}//!

	// Time
	inline UShort_t GetCsIMultTime() const
		{return fAnnularTelescope_CsI_T_DetectorNbr.size();}
	inline UShort_t GetCsIT_DetectorNbr(const unsigned int &i) const 
		{return fAnnularTelescope_CsI_T_DetectorNbr[i];}//!
	inline Double_t GetCsITime(const unsigned int &i) const 
		{return fAnnularTelescope_CsI_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TAnnularTelescopeData,1)  // AnnularTelescopeData structure
};

#endif
/* Local Variables:  */
/* mode: c++         */
/* End:              */
