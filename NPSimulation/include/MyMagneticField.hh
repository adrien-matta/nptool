
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

#ifndef MyMagneticField_H
#define MyMagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4ThreeVector.hh"

class MyMagneticField : public G4MagneticField
{
  public:
    MyMagneticField(const G4ThreeVector& FieldVector);
    ~MyMagneticField();

    void GetFieldValue( const  G4double Point[3],
                               G4double *Bfield ) const;

    void SetFieldValue( const  G4ThreeVector& newFieldValue );


  private:
    G4double Bz;
    G4double fFieldComponents[3];
    G4double rmax;
    G4double zmax;
    G4double zmin;
};

#endif
