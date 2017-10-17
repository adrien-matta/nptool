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
    // X - position
    vector<UShort_t>   fMDM_X_DetectorNbr;
    vector<Double_t>   fMDM_Xpos;

    // Y - position
    vector<UShort_t>   fMDM_Y_DetectorNbr;
    vector<Double_t>   fMDM_Ypos;


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
    // X - position
    inline void SetXpos(const UShort_t& DetNbr,const Double_t& x){
      fMDM_X_DetectorNbr.push_back(DetNbr);
      fMDM_Xpos.push_back(x);
    };//!

    // Y - position
    inline void SetYpos(const UShort_t& DetNbr,const Double_t& y){
      fMDM_Y_DetectorNbr.push_back(DetNbr);
      fMDM_Ypos.push_back(y);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // X - position
    inline UShort_t GetMultX() const
      {return fMDM_X_DetectorNbr.size();}
    inline UShort_t GetX_DetectorNbr(const unsigned int &i) const 
      {return fMDM_X_DetectorNbr[i];}//!
    inline Double_t Get_Xpos(const unsigned int &i) const 
      {return fMDM_Xpos[i];}//!

		// Y - position
    inline UShort_t GetMultY() const
      {return fMDM_Y_DetectorNbr.size();}
    inline UShort_t GetY_DetectorNbr(const unsigned int &i) const 
      {return fMDM_Y_DetectorNbr[i];}//!
    inline Double_t Get_Ypos(const unsigned int &i) const 
      {return fMDM_Ypos[i];}//!

  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TMDMData,1)  // MDMData structure
};

#endif
