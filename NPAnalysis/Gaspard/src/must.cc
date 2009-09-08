#include "must.hh"
#include <cmath>
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////

//////// Constructor by Spherical Coordinate
must::must(double theta, double phi, double distance, double beta_u , double beta_v, double beta_w)
	{
		double Pi = 3.141592654 ;

		// convert from degree to radian:
		theta = theta * Pi/180. ; 
		phi   = phi   * Pi/180. ;

		//Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
		Hep3Vector U ;	
		//Vector V on Telescope Face (parallele to X Strip)
		Hep3Vector V ;
		//Vector W normal to Telescope Face (pointing CsI)
		Hep3Vector W ;
		//Vector position of Telescope Face center
		Hep3Vector C ;

		/*if(theta==180 && phi==90)
			{
				C = Hep3Vector (0,0,distance)	;
				U = Hep3Vector (1,0,0)			;
				V = Hep3Vector (0,1,0)			;
				W = Hep3Vector (0,0,1)			;
			}*/
			
		if(theta==0 && phi==0)
			{
				C = Hep3Vector (0,0,distance)	;
				U = Hep3Vector (1,0,0)			;
				V = Hep3Vector (0,1,0)			;
				W = Hep3Vector (0,0,1)			;
			}
			
		else
			{
				C = Hep3Vector (	distance * sin(theta) * cos(phi) ,
									distance * sin(theta) * sin(phi) ,
									distance * cos(theta)			 );
					
				Hep3Vector Y = Hep3Vector( cos(theta) * cos(phi),
							   cos(theta) * sin(phi),
							  -sin(theta));

				W = C.unit() ;
//				U = W.cross( Hep3Vector (0,1,0) ) ;
				U = W.cross( Y ) ;
			    V = W.cross(U);
				
				U = U.unit();
				V = V.unit();
				
				U.rotate( U , beta_u * Pi/180. ) ;
				V.rotate( U , beta_u * Pi/180. ) ;
				
				U.rotate( V , beta_v * Pi/180. ) ;
				V.rotate( V , beta_v * Pi/180. ) ;
				
				U.rotate( W , beta_w * Pi/180. ) ;
				V.rotate( W , beta_w * Pi/180. ) ;
			}

//		double Face = 98 					  	; //mm
		double Face = 50 					  	; //mm
		double NumberOfStrip = 128 				;
		double StripPitch = Face/NumberOfStrip	; //mm

		vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;
		double X , Y , Z  ;

		//Moving C to the 1.1 corner:
		C.setX( C.x() - ( Face/2 - StripPitch/2 ) * ( V.x() + U.x() ) )  ; 
		C.setY( C.y() - ( Face/2 - StripPitch/2 ) * ( V.y() + U.y() ) )  ; 
		C.setZ( C.z() - ( Face/2 - StripPitch/2 ) * ( V.z() + U.z() ) )  ; 

		for( int i = 0 ; i < 128 ; i++ )
			{
			
			lineX.clear()	;
			lineY.clear()	;
			lineZ.clear()	;
			
			for( int j = 0 ; j < 128 ; j++ )
				{
				
				X = C.x() + StripPitch * ( U.x()*i + V.x()*j )	;
				Y = C.y() + StripPitch * ( U.y()*i + V.y()*j )	;
				Z = C.z() + StripPitch * ( U.z()*i + V.z()*j )	;
							
				lineX.push_back(X)	;
				lineY.push_back(Y)	;
				lineZ.push_back(Z)	;			
				}
				
			StripPositionX.push_back(lineX)	;
			StripPositionY.push_back(lineY)	;
			StripPositionZ.push_back(lineZ)	;
			 
			}
	}

///////////////////////////////////////////////////////
	
must::must(  Hep3Vector C_X1_Y1 	,
			 Hep3Vector C_X128_Y1 	, 
			 Hep3Vector C_X1_Y128 	, 
			 Hep3Vector C_X128_Y128 , 
			 Hep3Vector TargetPosition = Hep3Vector(0,0,0)	)
	{

		//	Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
		Hep3Vector U = C_X128_Y1 - C_X1_Y1 				;	
		U = U.unit()									;
		
		//	Vector V on Telescope Face (parallele to X Strip)
		Hep3Vector V = C_X1_Y128 - C_X1_Y1 				;
		V = V.unit()									;
		
		//	Position Vector of Strip Center
		Hep3Vector StripCenter 					;
		//	Position Vector of X=1 Y=1 Strip 
		Hep3Vector Strip_1_1 					;		
		
		//	Geometry Parameter
		double Face = 98 					  	; //mm
		double NumberOfStrip = 128 				;
		double StripPitch = Face/NumberOfStrip	; //mm

		//	Buffer object to fill Position Array
		vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

		//	Moving StripCenter to 1.1 corner:
		Strip_1_1 = C_X1_Y1 + (U+V) * StripPitch/2 	;
		
		for( int i = 0 ; i < 128 ; i++ )
			{
				lineX.clear()	;
				lineY.clear()	;
				lineZ.clear()	;
				
				for( int j = 0 ; j < 128 ; j++ )
					{
						StripCenter  = Strip_1_1 + StripPitch*( i*U + j*V  )	;
						//StripCenter += -TargetPosition		;
						
						lineX.push_back( StripCenter.x() )	;
						lineY.push_back( StripCenter.y() )	;
						lineZ.push_back( StripCenter.z() )	;	
					}
					
				StripPositionX.push_back(lineX)	;
				StripPositionY.push_back(lineY)	;
				StripPositionZ.push_back(lineZ)	;
			 	
			}
	}

///////////////////////////////////////////////////////
double must::GetStripPositionX(int X , int Y)
	{
		return StripPositionX[X][Y];
	}
///////////////////////////////////////////////////////
double must::GetStripPositionY(int X , int Y)
	{
		return StripPositionY[X][Y];
	}
///////////////////////////////////////////////////////
double must::GetStripPositionZ(int X , int Y)
	{
		return StripPositionZ[X][Y];
	}
///////////////////////////////////////////////////////
void must::Print()
	{

		for( int i = 0 ; i < 128 ; i++ )
			{
			
			for( int j = 0 ; j < 128 ; j++ )
				{
				cout << i+1 << "  "<< j+1 << "  " 
				<< StripPositionX[i][j]   << "  " 
				<< StripPositionY[i][j]   << "  "
				<< StripPositionZ[i][j]   << "  " 
				<< endl ;
				}	
			}
	}

///////////////////////////////////////////////////////
///////////////// Class Array /////////////////////////
///////////////////////////////////////////////////////
array::array()
{NumberOfTelescope=0;}
///////////////////////////////////////////////////////
void array::addTelescope( double theta , double phi , double distance , double beta_u , double beta_v, double beta_w)
	{
		must myMust(theta , phi , distance , beta_u , beta_v, beta_w)	;
		NumberOfTelescope++												;
		myArray.push_back(myMust)										;
	}

///////////////
void array::addTelescope(Hep3Vector A , Hep3Vector B , Hep3Vector C , Hep3Vector D )		
	{
		must myMust( A , B , C , D)		;
		NumberOfTelescope++				;
		myArray.push_back(myMust)		;
	}


///////////////////////////////////////////////////////
double array::GetStripPositionX( int N , int X , int Y )
	{
		return myArray[N-1].GetStripPositionX( X-1 , Y-1 );
	}

///////////////////////////////////////////////////////
double array::GetStripPositionY( int N , int X , int Y )
	{
		return myArray[N-1].GetStripPositionY( X-1 , Y-1 );
	}

///////////////////////////////////////////////////////
double array::GetStripPositionZ( int N , int X , int Y )
	{
		return myArray[N-1].GetStripPositionZ( X-1 , Y-1 );
	}

///////////////////////////////////////////////////////
void array::Print()
	{
		for ( int i = 0 ; i < myArray.size() ; i++ )
			{ cout << " Telescope : " << i+1 << endl ; myArray.at(i).Print() ; }
	}

