
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class defines the Magnetic field for Helios              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "MyMagneticField.hh"

MyMagneticField::MyMagneticField(const G4ThreeVector& FieldVector)
{
  
  fFieldComponents[0]= FieldVector.x()*tesla;
  fFieldComponents[1]= FieldVector.y()*tesla;
  fFieldComponents[2]= FieldVector.z()*tesla;

  //rmax = std::sqrt(625.)*cm;
  rmax = 50.*cm;
  zmin = 5.*cm;
  zmax = 200.*cm;

  //G4cout << "rmax=" << rmax << G4endl;

}

MyMagneticField::~MyMagneticField()
{;}

void MyMagneticField::GetFieldValue(const double Point[3],double *Bfield) const
{

  // Uniform magnetic field along z axis confine in a cylinder
  Bfield[0] = fFieldComponents[0];
  Bfield[1] = fFieldComponents[1];
 
  //if(std::abs(Point[2])<zmax && (sqr(Point[0])+sqr(Point[1]))<rmax_sq)
  //if(std::abs(Point[2])<zmax && std::abs(Point[2])>zmin && (std::sqrt(Point[0]*Point[0] + Point[1]*Point[1]))<rmax_sq)
  if(std::abs(Point[2])<zmax && (std::sqrt(Point[0]*Point[0] + Point[1]*Point[1]))<rmax)
  { Bfield[2] = fFieldComponents[2];   }
  else
  { Bfield[2] = 0.; }

}



void MyMagneticField::SetFieldValue(const G4ThreeVector &NewBfieldValue) 
{

  // Uniform magnetic field along z axis confine in a cylinder
  fFieldComponents[0] = NewBfieldValue.x();
  fFieldComponents[1] = NewBfieldValue.y();
  fFieldComponents[2] = NewBfieldValue.z();

}
