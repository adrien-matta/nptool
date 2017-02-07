#ifndef SageHVField_h
#define SageHVField_h 1

#include "globals.hh"
#include "G4ElectroMagneticField.hh"
#include "G4MagneticField.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

class SageHVField
#ifndef STANDALONE
 : public G4ElectroMagneticField
#endif
{

  // Storage space for the table
  vector< vector< vector< double > > > xField;
  vector< vector< vector< double > > > yField;
  vector< vector< vector< double > > > zField;

  vector< vector< vector< double > > > xeField;
  vector< vector< vector< double > > > yeField;
  vector< vector< vector< double > > > zeField;


  // The dimensions of the table
  int nx,ny,nz;
  // The physical limits of the defined region
  double minx, maxx, miny, maxy, minz, maxz;
  // The physical extent of the defined region
  double dx, dy, dz;
  double fZoffset;
  double fHVstrength;
  bool invertX, invertY, invertZ;

public:
  SageHVField(const char* filename, double zOffset, double HVstrength);
  
  void  GetFieldValue( const  double Point[4],
		       double *Bfield          ) const;

  G4bool DoesFieldChangeEnergy() const {return true;}
};

//class PurgMagTabulatedField3D
//#ifndef STANDALONE
// : public G4MagneticField
//#endif
//{
//  
//  // Storage space for the table
//  vector< vector< vector< double > > > xField;
//  vector< vector< vector< double > > > yField;
//  vector< vector< vector< double > > > zField;
//  // The dimensions of the table
//  int nx,ny,nz; 
//  // The physical limits of the defined region
//  double minx, maxx, miny, maxy, minz, maxz;
//  // The physical extent of the defined region
//  double dx, dy, dz;
//  double fZoffset;
//  bool invertX, invertY, invertZ;
//
//public:
//  PurgMagTabulatedField3D(const char* filename, double zOffset );
//  void  GetFieldValue( const  double Point[4],
//		       double *Bfield          ) const;
//};

#endif
