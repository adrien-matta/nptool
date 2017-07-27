// File :  GUser.h
// Author: Luc Legeard
//////////////////////////////////////////////////////////////////////////////
//
// Class GUser
//
// This class manage user methodes
//
/////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
// ***************************************************************************
// *                                                                         *
// *   This program is free software; you can redistribute it and/or modify  *
// *   it under the terms of the GNU General Public License as published by  *
// *   the Free Software Foundation; either version 2 of the License, or     *
// *   (at your option) any later version.                                   *
// *                                                                         *
// ***************************************************************************/


#ifndef GUser_H
#define GUser_H

#include <sstream>
using std::ostream;
#include "General.h"
#include <TObject.h>
#include "GAcq.h"
#include "GDevice.h"

#ifndef __MAKECINT__
// Ganil2Root
#include "G2RDetectorManager.h"
// NPTool
#include "NPDetectorManager.h"
#endif

//_________________________________________________________________________________________

class GUser : public  GAcq{
  protected:
    int fVerbose;// level (0 to 10 ) of verbose default = 0  ( level 10 is good for debug)        

  public:
    GUser(GDevice* DevIn= NULL, GDevice* DevOut= NULL);// default constructor of GUser object 
	 ~GUser() ; 

    virtual void InitUser();
    virtual void InitUserRun();
    virtual void User();
    virtual void EndUserRun();
    virtual void EndUser();
    virtual void InitTTreeUser(); 
    ClassDef (GUser ,1); // User Treatment of Data

#ifndef __MAKECINT__
  private: // NPTool  
    NPL::DetectorManager* m_NPDetectorManager;
    TTree* m_NPTree;
  private: // Ganil2Root
    G2R::DetectorManager* m_G2RDetectorManager;
#endif

};

#endif

