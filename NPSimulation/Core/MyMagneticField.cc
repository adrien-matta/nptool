
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
 * Last update    : 15/05/16                                                         *
 *---------------------------------------------------------------------------*
 * Decription: This class defines the Magnetic field for Helios/Helisol      *
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
  rbore = 49.*cm; // bore radius  !! 49 instead of 50 to avoid issues at volume boundaries
  deltaB_Bz0=0.005; // => 0.5%
  //deltaB_Bz0=0.001; // => 0.1%
  //deltaB_Bz0=0.01; // => 1%
  //deltaB_Bz0=0.; // => 0%
  //zmin = 5.*cm; 
  zbore = 136.6*cm; // bore half length

   Bz0=fFieldComponents[2]; // nominal field
   //G4cout << "FieldVector.z()=" << FieldVector.z() << G4endl;
   //G4cout << "tesla=" << tesla << " megavolt*ns/mm2" << G4endl;
   G4cout << "Bz0=" << Bz0 << " megavolt*ns/mm2 (default units)" << G4endl;
   G4cout << "Bz0=" << Bz0/tesla << " tesla" << G4endl;
   //G4cout << "zbore=" << zbore << " mm (default units)" << G4endl;
   //G4cout << "rbore=" << rbore << " mm (default units)" << G4endl;

}

MyMagneticField::~MyMagneticField()
{;}

void MyMagneticField::GetFieldValue(const double Point[3],double *Bfield) const
{

  // Uniform magnetic field along z axis confine in a cylinder
  Bfield[0] = fFieldComponents[0];
  Bfield[1] = fFieldComponents[1];
 

/* homogeneous Mag field
   if(std::abs(Point[2])<zbore && (std::sqrt(Point[0]*Point[0] + Point[1]*Point[1]))<rbore)
  { Bfield[2] = fFieldComponents[2];   }
  else
  { Bfield[2] = 0.; }
*/

/* unhomogeneous field w/r to beam axis distance, inside the bore */

	/* if R is radial distance to beam axis) :
  if(std::abs(Point[2])<zbore && (std::sqrt(Point[0]*Point[0] + Point[1]*Point[1]))<rbore)
   { 
	   //Bfield[0] = (point[0]/2) * fFieldComponents[2] *(deltaB_B/pow(rbore,2)) * 2* point[2]); 
	   //Bfield[1] = (point[1]/2) * fFieldComponents[2] *(deltaB_B/pow(rbore,2)) * 2* point[2]);
	   Bfield[2] = fFieldComponents[2] *(1-deltaB_B*std::pow( std::abs( std::sqrt( std::pow(Point[0],2) + std::pow(Point[1],2)))/(rbore), 2)); 
	}
	else
	{
		Bfield[0] = 0.; 
		Bfield[1] = 0.;  	
		Bfield[2] = 0.; 
	}
*/

/* unhomogeneous field if R is radial distance to target position */
  if(std::abs(Point[2])<zbore && (std::sqrt(Point[0]*Point[0] + Point[1]*Point[1]))<rbore)
    { 
		Bfield[0] = (Point[0]/2) * Bz0 *(deltaB_Bz0/pow(rbore,2) * 2* Point[2]);  // = x/2 * d(Bfield[2])/dz
	    Bfield[1] = (Point[1]/2) * Bz0 *(deltaB_Bz0/pow(rbore,2) * 2* Point[2]);  // = y/2 * d(Bfield[2])/dz
		Bfield[2] = Bz0 *(1-deltaB_Bz0*std::pow( std::abs( std::sqrt( std::pow(Point[0],2) + std::pow(Point[1],2) + std::pow(Point[2],2)))/(rbore), 2)); // = Bz(0) - deltaB*(R/rbore)^2
	}
	else
	{ 
		Bfield[0] = 0.; 
		Bfield[1] = 0.; 
		Bfield[2] = 0.; 
	}	


     //G4cout << "Point[0]= " << Point[0] << " Point[1]= " << Point[1] << " Point[2]= " << Point[2] <<G4endl;
     //G4cout << "get Bfield[0]=" << Bfield[0]/tesla << " tesla" << G4endl;
     //G4cout << "get Bfield[1]=" << Bfield[1]/tesla << " tesla" << G4endl;
     //G4cout << "get Bfield[2]=" << Bfield[2]/tesla << " tesla" << G4endl;




}



void MyMagneticField::SetFieldValue(const G4ThreeVector &NewBfieldValue) 
{

  // Uniform magnetic field along z axis confine in a cylinder
  fFieldComponents[0] = NewBfieldValue.x();
  fFieldComponents[1] = NewBfieldValue.y();
  fFieldComponents[2] = NewBfieldValue.z();

}
