#ifndef __MDMDATA__
#define __MDMDATA__
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : October 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold MDM Raw data                                    *
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

class TMDMData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private:
	  // Detector Number
    vector<UShort_t>   fMDM_DetectorNbr;

    // X - position
    vector<Double_t>   fMDM_Xpos;

    // Y - position
    vector<Double_t>   fMDM_Ypos;

		// Kinetic Energy
		vector<Double_t>   fMDM_Kinetic_Energy;
		
		// Particle ID
		vector<UShort_t>   fMDM_Particle_Charge;
		vector<Double_t>   fMDM_Particle_Mass;
		

  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TMDMData();
    ~TMDMData();
    

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
    //
		inline void SetHit(UShort_t DetNbr, Double_t x, Double_t y, UShort_t charge, Double_t mass, Double_t eKin = 0){
      fMDM_DetectorNbr.push_back(DetNbr);
      fMDM_Xpos.push_back(x);
      fMDM_Ypos.push_back(y);
			fMDM_Kinetic_Energy.push_back(eKin);
			fMDM_Particle_Mass.push_back(mass);
			fMDM_Particle_Charge.push_back(charge);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    inline UShort_t GetMult() const
      {return fMDM_DetectorNbr.size();}
    inline UShort_t GetDetectorNbr(const unsigned int &i) const 
      {return fMDM_DetectorNbr[i];}//!
    // X - position
    inline Double_t Get_Xpos(const unsigned int &i) const 
      {return fMDM_Xpos[i];}//!
		// Y - position
    inline Double_t Get_Ypos(const unsigned int &i) const 
      {return fMDM_Ypos[i];}//!
		// Kinetic Energy
		inline Double_t Get_Kinetic_Energy(const unsigned int &i) const 
      {return fMDM_Kinetic_Energy[i];}//!
		// Particle ID
		inline Double_t GetParticleMass(const unsigned int& i) const
		  {return fMDM_Particle_Mass[i];}//!
		inline UShort_t GetParticleCharge(const unsigned int& i) const
		  {return fMDM_Particle_Charge[i];}//!
		
  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TMDMData,3)  // MDMData structure
};

#endif
