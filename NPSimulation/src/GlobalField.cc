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
 *  The first line of the user file must be the number of x, y & z points,   *
 *  then the rest of the data should follow the format of                    *
 *                           x y z bx by bz                                  *
 *  where x, y & z are in mm and bx, by & bz are in tesla.                   *
 *  where x, y & z start off at their smallest values... x increases first,  *
 *  then y, then z.                                                          *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "GlobalField.hh"

// G4
#include "G4SystemOfUnits.hh"

// NPL
#include"NPOptionManager.h"

GlobalField::GlobalField(string Path) {

  // Variable declaration
  G4double posx, posy, posz, bx, by, bz;
  G4double FieldUnit = tesla;
  G4double LengthUnit = mm;
  G4int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();
  
  // Open magnetic field file
  ifstream FieldFile;
  FieldFile.open(Path.c_str());
  
  // Check if open
  if (FieldFile.is_open()) {
    G4cout << "////////////////////////////////////////////////////" << G4endl;
    G4cout << "Reading in Magnetic Field from " << Path << G4endl;
  } else G4cout << "Error, unable to open " << Path << G4endl;
  
  // Read number of elements and resize vectors
  FieldFile >> fNumberOfXElements >> fNumberOfYElements >> fNumberOfZElements;
  fFieldX.resize(fNumberOfXElements);
  fFieldY.resize(fNumberOfXElements);
  fFieldZ.resize(fNumberOfXElements);
  for (G4int x=0; x<fNumberOfXElements; x++) {
    fFieldX[x].resize(fNumberOfYElements);
    fFieldY[x].resize(fNumberOfYElements);
    fFieldZ[x].resize(fNumberOfYElements);
    for (G4int y=0; y<fNumberOfYElements; y++) {
      fFieldX[x][y].resize(fNumberOfZElements);
      fFieldY[x][y].resize(fNumberOfZElements);
      fFieldZ[x][y].resize(fNumberOfZElements);
    }
  }

  // Read in values
  for (G4int z=0; z<fNumberOfZElements; z++) {
    for (G4int y=0; y<fNumberOfYElements; y++) {
      for (G4int x=0; x<fNumberOfXElements; x++) {
        FieldFile >> posx >> posy >> posz >> bx >> by >> bz;    
        if ( x==0 && y==0 && z==0 ) {
          fMinimumX = posx * LengthUnit;
          fMinimumY = posy * LengthUnit;
          fMinimumZ = posz * LengthUnit;
        }
        if ( x==(fNumberOfXElements-1) && y==(fNumberOfYElements-1) && z==(fNumberOfZElements-1) ) {
          fMaximumX = posx * LengthUnit;
          fMaximumY = posy * LengthUnit;
          fMaximumZ = posz * LengthUnit;
        }
        fFieldX[x][y][z] = bx * FieldUnit;
        fFieldY[x][y][z] = by * FieldUnit;
        fFieldZ[x][y][z] = bz * FieldUnit;
      }
    }
  }
  FieldFile.close();
  
  fDeltaX = fMaximumX - fMinimumX;
  fDeltaY = fMaximumY - fMinimumY;
  fDeltaZ = fMaximumZ - fMinimumZ;
  
  if (VerboseLevel==1) {
    G4cout << " The minimum position is (x, y, z) = (" 
           << fMinimumX/cm << ", " << fMinimumY/cm << ", " << fMinimumZ/cm << ") cm" << G4endl;
    G4cout << " The maximum position is (x, y, z) = (" 
           << fMaximumX/cm << ", " << fMaximumY/cm << ", " << fMaximumZ/cm << ") cm" << G4endl;
    G4cout << " Thus the size of the field is (dx, dy, dz) = ("
           << fDeltaX/cm << ", " << fDeltaY/cm << ", " << fDeltaZ/cm << ") cm" << G4endl;
  }
  
  
  
}


GlobalField::~GlobalField() { /* _ */;}

void GlobalField::GetFieldValue(const double point[4],double *Bfield) const {
  
  G4double ReqValueX = point[0];
  G4double ReqValueY = point[1];
  G4double ReqValueZ = point[2];

  // Check that the requested point is within the defined region
  if ( ReqValueX >= fMinimumX && ReqValueX <= fMaximumX &&
       ReqValueY >= fMinimumY && ReqValueY <= fMaximumY &&
       ReqValueZ >= fMinimumZ && ReqValueZ <= fMaximumZ ) {

    // Convert the requested absolute position into a fraction of the region, from 0 to 1   
    G4double ReqFractionX = (ReqValueX - fMinimumX) / fDeltaX;
    G4double ReqFractionY = (ReqValueY - fMinimumY) / fDeltaY;
    G4double ReqFractionZ = (ReqValueZ - fMinimumZ) / fDeltaZ;

    // The fraction is then multiplied by the array size and split into an integer & fraction
    G4double dblIndexX, dblIndexY, dblIndexZ;
    G4double IndexXFraction = modf(ReqFractionX*(fNumberOfXElements-1), &dblIndexX);
    G4double IndexYFraction = modf(ReqFractionY*(fNumberOfYElements-1), &dblIndexY);
    G4double IndexZFraction = modf(ReqFractionZ*(fNumberOfZElements-1), &dblIndexZ);
    G4int IndexX = static_cast<int>(dblIndexX);
    G4int IndexY = static_cast<int>(dblIndexY);
    G4int IndexZ = static_cast<int>(dblIndexZ);
    
    // Return the B-Field value
    Bfield[0] =
      fFieldX[IndexX  ][IndexY  ][IndexZ  ] * (1-IndexXFraction) * (1-IndexYFraction) * (1-IndexZFraction) +
      fFieldX[IndexX  ][IndexY  ][IndexZ+1] * (1-IndexXFraction) * (1-IndexYFraction) *    IndexZFraction  +
      fFieldX[IndexX  ][IndexY+1][IndexZ  ] * (1-IndexXFraction) *    IndexYFraction  * (1-IndexZFraction) +
      fFieldX[IndexX  ][IndexY+1][IndexZ+1] * (1-IndexXFraction) *    IndexYFraction  *    IndexZFraction  +
      fFieldX[IndexX+1][IndexY  ][IndexZ  ] *    IndexXFraction  * (1-IndexYFraction) * (1-IndexZFraction) +
      fFieldX[IndexX+1][IndexY  ][IndexZ+1] *    IndexXFraction  * (1-IndexYFraction) *    IndexZFraction  +
      fFieldX[IndexX+1][IndexY+1][IndexZ  ] *    IndexXFraction  *    IndexYFraction  * (1-IndexZFraction) +
      fFieldX[IndexX+1][IndexY+1][IndexZ+1] *    IndexXFraction  *    IndexYFraction  *    IndexZFraction ;
    Bfield[1] =
      fFieldY[IndexX  ][IndexY  ][IndexZ  ] * (1-IndexXFraction) * (1-IndexYFraction) * (1-IndexZFraction) +
      fFieldY[IndexX  ][IndexY  ][IndexZ+1] * (1-IndexXFraction) * (1-IndexYFraction) *    IndexZFraction  +
      fFieldY[IndexX  ][IndexY+1][IndexZ  ] * (1-IndexXFraction) *    IndexYFraction  * (1-IndexZFraction) +
      fFieldY[IndexX  ][IndexY+1][IndexZ+1] * (1-IndexXFraction) *    IndexYFraction  *    IndexZFraction  +
      fFieldY[IndexX+1][IndexY  ][IndexZ  ] *    IndexXFraction  * (1-IndexYFraction) * (1-IndexZFraction) +
      fFieldY[IndexX+1][IndexY  ][IndexZ+1] *    IndexXFraction  * (1-IndexYFraction) *    IndexZFraction  +
      fFieldY[IndexX+1][IndexY+1][IndexZ  ] *    IndexXFraction  *    IndexYFraction  * (1-IndexZFraction) +
      fFieldY[IndexX+1][IndexY+1][IndexZ+1] *    IndexXFraction  *    IndexYFraction  *    IndexZFraction ;
    Bfield[2] =
      fFieldZ[IndexX  ][IndexY  ][IndexZ  ] * (1-IndexXFraction) * (1-IndexYFraction) * (1-IndexZFraction) +
      fFieldZ[IndexX  ][IndexY  ][IndexZ+1] * (1-IndexXFraction) * (1-IndexYFraction) *    IndexZFraction  +
      fFieldZ[IndexX  ][IndexY+1][IndexZ  ] * (1-IndexXFraction) *    IndexYFraction  * (1-IndexZFraction) +
      fFieldZ[IndexX  ][IndexY+1][IndexZ+1] * (1-IndexXFraction) *    IndexYFraction  *    IndexZFraction  +
      fFieldZ[IndexX+1][IndexY  ][IndexZ  ] *    IndexXFraction  * (1-IndexYFraction) * (1-IndexZFraction) +
      fFieldZ[IndexX+1][IndexY  ][IndexZ+1] *    IndexXFraction  * (1-IndexYFraction) *    IndexZFraction  +
      fFieldZ[IndexX+1][IndexY+1][IndexZ  ] *    IndexXFraction  *    IndexYFraction  * (1-IndexZFraction) +
      fFieldZ[IndexX+1][IndexY+1][IndexZ+1] *    IndexXFraction  *    IndexYFraction  *    IndexZFraction ;
  } else { 
    // If not within the defined region, return null
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
  }
}
