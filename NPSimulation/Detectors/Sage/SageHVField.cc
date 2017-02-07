#include "SageHVField.hh"

#include <math.h>
#include <assert.h>

SageHVField::SageHVField( const char* filename, double zOffset, double HVstrength)
	:fZoffset(zOffset),fHVstrength(HVstrength),invertX(false),invertY(false),invertZ(false)
{

	double lenUnit= centimeter;//meter;
	double fieldUnit= gauss;
	//double fieldUnit= tesla;
	double ElectricfieldUnit = volt/centimeter;
	G4cout << "\n-----------------------------------------------------------"
		<< "\n      High Voltage field"
		<< "\n-----------------------------------------------------------";

	G4cout << "\n ---> " "Reading the field grid from " << filename << " ... " << endl;
	ifstream file( filename ); // Open the file for reading.
	if (file.good()){

		// Ignore first blank line
		char buffer[256];
		file.getline(buffer,256);

		// Read table dimensions
		file >> nx >> ny >> nz;

		G4cout << "  [ Number of values x,y,z: "
			<< nx << " " << ny << " " << nz << " ] "
			<< endl;

		// Set up storage space for table for magnetic field
		xField.resize( nx );
		yField.resize( nx );
		zField.resize( nx );
		int ix, iy, iz;
		for (ix=0; ix<nx; ix++) {
			xField[ix].resize(ny);
			yField[ix].resize(ny);
			zField[ix].resize(ny);
			for (iy=0; iy<ny; iy++) {
				xField[ix][iy].resize(nz);
				yField[ix][iy].resize(nz);
				zField[ix][iy].resize(nz);
			}
		}
		G4cout << "Magnetic Field Storage Set" << endl;
		// Set up storage space for table for electric field
		xeField.resize( nx );
		yeField.resize( nx );
		zeField.resize( nx );

		for (ix=0; ix<nx; ix++) {
			xeField[ix].resize(ny);
			yeField[ix].resize(ny);
			zeField[ix].resize(ny);
			for (iy=0; iy<ny; iy++) {
				xeField[ix][iy].resize(nz);
				yeField[ix][iy].resize(nz);
				zeField[ix][iy].resize(nz);
			}
		}
		G4cout << "Electric Field Storage Set" << endl;

		cout << "Electric storage space set" << endl;
		// Ignore other header information
		// The first line whose second character is '0' is considered to
		// be the last line of the header.
		do {
			file.getline(buffer,256);
		} while ( buffer[1]!='0');
		cout << "reading in data" << endl;
		// Read in the data
		double xval,yval,zval,bx,by,bz,ex,ey,ez;
		for (ix=0; ix<nx; ix++) {
			for (iy=0; iy<ny; iy++) {
				for (iz=0; iz<nz; iz++) {

					file >> xval >> yval >> zval >> bx >> by >> bz >> ex >> ey >> ez;
					if ( ix==0 && iy==0 && iz==0 ) {
						minx = xval * lenUnit;
						miny = yval * lenUnit;
						minz = zval * lenUnit;
					}

					xField[ix][iy][iz] = bx * fieldUnit;
					yField[ix][iy][iz] = by * fieldUnit;
					zField[ix][iy][iz] = bz * fieldUnit;

					xeField[ix][iy][iz] = fHVstrength * ex * ElectricfieldUnit;
					yeField[ix][iy][iz] = fHVstrength * ey * ElectricfieldUnit;
					zeField[ix][iy][iz] = fHVstrength * ez * ElectricfieldUnit;
				}
			}
		}
		file.close();

		maxx = xval * lenUnit;
		maxy = yval * lenUnit;
		maxz = zval * lenUnit;

		cout << "\n ---> ... done reading " << endl;

		// G4cout << " Read values of field from file " << filename << endl;
		G4cout << " ---> assumed the order:  x, y, z, Bx, By, Bz, Ex, Ey, Ez "
			<< "\n ---> Min values x,y,z: "
			<< minx/cm << " " << miny/cm << " " << minz/cm << " cm "
			<< "\n ---> Max values x,y,z: "
			<< maxx/cm << " " << maxy/cm << " " << maxz/cm << " cm "
			<< "\n ---> The field will be offset by " << zOffset/mm << " mm " << endl;

		// Should really check that the limits are not the wrong way around.
		if (maxx < minx) {swap(maxx,minx); invertX = true;}
		if (maxy < miny) {swap(maxy,miny); invertY = true;}
		if (maxz < minz) {swap(maxz,minz); invertZ = true;}
		G4cout << "\nAfter reordering if neccesary"
			<< "\n ---> Min values x,y,z: "
			<< minx/cm << " " << miny/cm << " " << minz/cm << " cm "
			<< " \n ---> Max values x,y,z: "
			<< maxx/cm << " " << maxy/cm << " " << maxz/cm << " cm ";

		dx = maxx - minx;
		dy = maxy - miny;
		dz = maxz - minz;
		G4cout << "\n ---> Dif values x,y,z (range): "
			<< dx/cm << " " << dy/cm << " " << dz/cm << " cm in z "
			<< "\n-----------------------------------------------------------" << endl;
	}
	else {
		file.close();
		G4cout << "The intensity file, " << filename<< " doesn't exist" << endl;
	}
}
void SageHVField::GetFieldValue(const double point[4],
		double *Bfield ) const
{

	double x = point[0];
	double y = point[1];
	double z = point[2] + fZoffset;

	// Check that the point is within the defined region
	if ( x>=minx && x<=maxx &&
			y>=miny && y<=maxy &&
			z>=minz && z<=maxz ) {

		// Position of given point within region, normalized to the range
		// [0,1]
		double xfraction = (x - minx) / dx;
		double yfraction = (y - miny) / dy;
		double zfraction = (z - minz) / dz;

		if (invertX) { xfraction = 1 - xfraction;}
		if (invertY) { yfraction = 1 - yfraction;}
		if (invertZ) { zfraction = 1 - zfraction;}

		// Need addresses of these to pass to modf below.
		// modf uses its second argument as an OUTPUT argument.
		double xdindex, ydindex, zdindex;

		// Position of the point within the cuboid defined by the
		// nearest surrounding tabulated points
		double xlocal = ( std::modf(xfraction*(nx-1), &xdindex));
		double ylocal = ( std::modf(yfraction*(ny-1), &ydindex));
		double zlocal = ( std::modf(zfraction*(nz-1), &zdindex));

		// The indices of the nearest tabulated point whose coordinates
		// are all less than those of the given point
		int xindex = static_cast<int>(xdindex);
		int yindex = static_cast<int>(ydindex);
		int zindex = static_cast<int>(zdindex);


#ifdef DEBUG_INTERPOLATING_FIELD
		G4cout << "Local x,y,z: " << xlocal << " " << ylocal << " " << zlocal << endl;
		G4cout << "Index x,y,z: " << xindex << " " << yindex << " " << zindex << endl;
		double valx0z0, mulx0z0, valx1z0, mulx1z0;
		double valx0z1, mulx0z1, valx1z1, mulx1z1;
		valx0z0= table[xindex  ][0][zindex];  mulx0z0=  (1-xlocal) * (1-zlocal);
		valx1z0= table[xindex+1][0][zindex];  mulx1z0=   xlocal    * (1-zlocal);
		valx0z1= table[xindex  ][0][zindex+1]; mulx0z1= (1-xlocal) * zlocal;
		valx1z1= table[xindex+1][0][zindex+1]; mulx1z1=  xlocal    * zlocal;
#endif
		//*
		// Full 3-dimensional version
		Bfield[0] =
			xField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
			xField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
			xField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
			xField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
			xField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
			xField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
			xField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
			xField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
		Bfield[1] =
			yField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
			yField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
			yField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
			yField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
			yField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
			yField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
			yField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
			yField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
		Bfield[2] =
			zField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
			zField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
			zField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
			zField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
			zField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
			zField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
			zField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
			zField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
		Bfield[3] =
			xeField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
			xeField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
			xeField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
			xeField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
			xeField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
			xeField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
			xeField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
			xeField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
		Bfield[4] =
			yeField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
			yeField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
			yeField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
			yeField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
			yeField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
			yeField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
			yeField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
			yeField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
		Bfield[5] =
			zeField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
			zeField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
			zeField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
			zeField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
			zeField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
			zeField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
			zeField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
			zeField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;

	} else {
		Bfield[0] = 0.0;
		Bfield[1] = 0.0;
		Bfield[2] = 0.0;
		Bfield[3] = 0.0;
		Bfield[4] = 0.0;
		Bfield[5] = 0.0;
	}
}
//PurgMagTabulatedField3D::PurgMagTabulatedField3D(const char* filename, 
//						 double zOffset ) 
//  :fZoffset(zOffset),invertX(false),invertY(false),invertZ(false)
//{    
// 
//  double lenUnit= meter;
//  double fieldUnit= tesla; 
//  G4cout << "\n-----------------------------------------------------------"
//	 << "\n      Magnetic field"
//	 << "\n-----------------------------------------------------------";
//    
//  G4cout << "\n ---> " "Reading the field grid from " << filename << " ... " << endl; 
//
//  //
//  //This is a thread-local class and we have to avoid that all workers open the 
//  //file at the same time
//  //G4AutoLock lock(&myPurgMagTabulatedField3DLock);
//
//  ifstream file( filename ); // Open the file for reading.
//  
//  if (!file.is_open())
//    {
//      G4ExceptionDescription ed;
//      ed << "Could not open input file " << filename << G4endl;
//      G4Exception("PurgMagTabulatedField3D::PurgMagTabulatedField3D",
//		  "pugmag001",FatalException,ed);
//    }
//
//  // Ignore first blank line
//  char buffer[256];
//  file.getline(buffer,256);
//  
//  // Read table dimensions 
//  file >> nx >> ny >> nz; // Note dodgy order
//
//  G4cout << "  [ Number of values x,y,z: " 
//	 << nx << " " << ny << " " << nz << " ] "
//	 << endl;
//
//  // Set up storage space for table
//  xField.resize( nx );
//  yField.resize( nx );
//  zField.resize( nx );
//  int ix, iy, iz;
//  for (ix=0; ix<nx; ix++) {
//    xField[ix].resize(ny);
//    yField[ix].resize(ny);
//    zField[ix].resize(ny);
//    for (iy=0; iy<ny; iy++) {
//      xField[ix][iy].resize(nz);
//      yField[ix][iy].resize(nz);
//      zField[ix][iy].resize(nz);
//    }
//  }
//  
//  // Ignore other header information    
//  // The first line whose second character is '0' is considered to
//  // be the last line of the header.
//  do {
//    file.getline(buffer,256);
//  } while ( buffer[1]!='0');
//  
//  // Read in the data
//  double xval,yval,zval,bx,by,bz;
//  double permeability; // Not used in this example.
//  for (ix=0; ix<nx; ix++) {
//    for (iy=0; iy<ny; iy++) {
//      for (iz=0; iz<nz; iz++) {
//        file >> xval >> yval >> zval >> bx >> by >> bz >> permeability;
//        if ( ix==0 && iy==0 && iz==0 ) {
//          minx = xval * lenUnit;
//          miny = yval * lenUnit;
//          minz = zval * lenUnit;
//        }
//        xField[ix][iy][iz] = bx * fieldUnit;
//        yField[ix][iy][iz] = by * fieldUnit;
//        zField[ix][iy][iz] = bz * fieldUnit;
//      }
//    }
//  }
//  file.close();
//
//  lock.unlock();
//
//  maxx = xval * lenUnit;
//  maxy = yval * lenUnit;
//  maxz = zval * lenUnit;
//
//  G4cout << "\n ---> ... done reading " << endl;
//
//  // G4cout << " Read values of field from file " << filename << endl; 
//  G4cout << " ---> assumed the order:  x, y, z, Bx, By, Bz "
//	 << "\n ---> Min values x,y,z: " 
//	 << minx/cm << " " << miny/cm << " " << minz/cm << " cm "
//	 << "\n ---> Max values x,y,z: " 
//	 << maxx/cm << " " << maxy/cm << " " << maxz/cm << " cm "
//	 << "\n ---> The field will be offset by " << zOffset/cm << " cm " << endl;
//
//  // Should really check that the limits are not the wrong way around.
//  if (maxx < minx) {swap(maxx,minx); invertX = true;} 
//  if (maxy < miny) {swap(maxy,miny); invertY = true;} 
//  if (maxz < minz) {swap(maxz,minz); invertZ = true;} 
//  G4cout << "\nAfter reordering if neccesary"  
//	 << "\n ---> Min values x,y,z: " 
//	 << minx/cm << " " << miny/cm << " " << minz/cm << " cm "
//	 << " \n ---> Max values x,y,z: " 
//	 << maxx/cm << " " << maxy/cm << " " << maxz/cm << " cm ";
//
//  dx = maxx - minx;
//  dy = maxy - miny;
//  dz = maxz - minz;
//  G4cout << "\n ---> Dif values x,y,z (range): " 
//	 << dx/cm << " " << dy/cm << " " << dz/cm << " cm in z "
//	 << "\n-----------------------------------------------------------" << endl;
//}
//
//void PurgMagTabulatedField3D::GetFieldValue(const double point[4],
//				      double *Bfield ) const
//{
//
//  double x = point[0];
//  double y = point[1];
//  double z = point[2] + fZoffset;
//
//  // Check that the point is within the defined region 
//  if ( x>=minx && x<=maxx &&
//       y>=miny && y<=maxy && 
//       z>=minz && z<=maxz ) {
//    
//    // Position of given point within region, normalized to the range
//    // [0,1]
//    double xfraction = (x - minx) / dx;
//    double yfraction = (y - miny) / dy; 
//    double zfraction = (z - minz) / dz;
//
//    if (invertX) { xfraction = 1 - xfraction;}
//    if (invertY) { yfraction = 1 - yfraction;}
//    if (invertZ) { zfraction = 1 - zfraction;}
//
//    // Need addresses of these to pass to modf below.
//    // modf uses its second argument as an OUTPUT argument.
//    double xdindex, ydindex, zdindex;
//    
//    // Position of the point within the cuboid defined by the
//    // nearest surrounding tabulated points
//    double xlocal = ( std::modf(xfraction*(nx-1), &xdindex));
//    double ylocal = ( std::modf(yfraction*(ny-1), &ydindex));
//    double zlocal = ( std::modf(zfraction*(nz-1), &zdindex));
//    
//    // The indices of the nearest tabulated point whose coordinates
//    // are all less than those of the given point
//    int xindex = static_cast<int>(xdindex);
//    int yindex = static_cast<int>(ydindex);
//    int zindex = static_cast<int>(zdindex);
//    
//
//#ifdef DEBUG_INTERPOLATING_FIELD
//    G4cout << "Local x,y,z: " << xlocal << " " << ylocal << " " << zlocal << endl;
//    G4cout << "Index x,y,z: " << xindex << " " << yindex << " " << zindex << endl;
//    double valx0z0, mulx0z0, valx1z0, mulx1z0;
//    double valx0z1, mulx0z1, valx1z1, mulx1z1;
//    valx0z0= table[xindex  ][0][zindex];  mulx0z0=  (1-xlocal) * (1-zlocal);
//    valx1z0= table[xindex+1][0][zindex];  mulx1z0=   xlocal    * (1-zlocal);
//    valx0z1= table[xindex  ][0][zindex+1]; mulx0z1= (1-xlocal) * zlocal;
//    valx1z1= table[xindex+1][0][zindex+1]; mulx1z1=  xlocal    * zlocal;
//#endif
//
//        // Full 3-dimensional version
//    Bfield[0] =
//      xField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
//      xField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
//      xField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
//      xField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
//      xField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
//      xField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
//      xField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
//      xField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
//    Bfield[1] =
//      yField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
//      yField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
//      yField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
//      yField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
//      yField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
//      yField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
//      yField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
//      yField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
//    Bfield[2] =
//      zField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
//      zField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
//      zField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
//      zField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
//      zField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
//      zField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
//      zField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
//      zField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
//
//  } else {
//    Bfield[0] = 0.0;
//    Bfield[1] = 0.0;
//    Bfield[2] = 0.0;
//  }
//}
//
