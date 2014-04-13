/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * NPTool Author: Adrien MATTA          contact address: matta@ipno.in2p3.fr *
 * Class Author: Lee Evitts             contact address: evitts@triumf.ca    *
 *                                                                           *
 * Creation Date  : 11/04/2014                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Reads in a non-uniform magnetic field from user-defined file and assigns *
 *  it to an array.  Each time G4 calculates the next step, it calls         *
 *  GetFieldValue which reads from the array.                                *
 *                                                                           *
 *  The user file must be in the format of x, y, z (mm), bx, by, bz (tesla)  *
 *  where x, y & z start off at their smallest values... x increases first,  *
 *  then y, then z.                                                          *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#ifndef GlobalField_H
#define GlobalField_H 1

// G4
#include "globals.hh"
#include "G4MagneticField.hh"

// C++
#include <fstream>
#include <vector>

using namespace std;

class GlobalField : public G4MagneticField
{
  public:
    GlobalField(string);
    ~GlobalField();

    void GetFieldValue( const  double Point[4], double *Bfield ) const;
    
    // B-Field Values
    vector< vector< vector< double > > > fFieldX;
    vector< vector< vector< double > > > fFieldY;
    vector< vector< vector< double > > > fFieldZ;
    
    // Co-ordinates
    G4int fNumberOfXElements, fNumberOfYElements, fNumberOfZElements;
    G4double fMinimumX, fMinimumY, fMinimumZ;
    G4double fMaximumX, fMaximumY, fMaximumZ;
    G4double fDeltaX, fDeltaY, fDeltaZ;

};

#endif
