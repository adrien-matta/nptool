#ifndef must_h
#define must_h 1

#include <vector>
#include <TFile.h>
#include <TH2.h>
#include <TH2F.h>
#include "CLHEP/Vector/ThreeVector.h"
using namespace std 	;
using namespace CLHEP	;

class must
{
public:
	must( double theta , double phi , double distance , double beta_u , double beta_v, double beta_w)	;
	must(	Hep3Vector C_X1_Y1 			,
			Hep3Vector C_X128_Y1 		, 
			Hep3Vector C_X1_Y128 		, 
			Hep3Vector C_X128_Y128 		, 
			Hep3Vector TargetPosition 	);
	
	double 	GetStripPositionX( int X , int Y )			;
	double 	GetStripPositionY( int X , int Y )			;
	double 	GetStripPositionZ( int X , int Y )			;
	void 	Print()										;

private:

	vector< vector < double > > StripPositionX			;
	vector< vector < double > > StripPositionY			;
	vector< vector < double > > StripPositionZ			;

};


class array
{
public:
	array();
	void addTelescope( double theta , double phi , double distance , double beta_u , double beta_v , double beta_w)	;
	void addTelescope(	Hep3Vector C_X1_Y1 								,
			 			Hep3Vector C_X128_Y1 							, 
			 			Hep3Vector C_X1_Y128 							, 
			 			Hep3Vector C_X128_Y128 							);
			 			
	double GetStripPositionX( int N , int X , int Y )					;
	double GetStripPositionY( int N , int X , int Y )					;
	double GetStripPositionZ( int N , int X , int Y )					;
	double GetNumberOfTelescope() {return NumberOfTelescope;}			;
	void Print()														;
	
private:
	vector< must > myArray	;
	int NumberOfTelescope   ;

public:	// Histogram

/*//	Hit Density
	vector<TH2F*>	HitDensity	;
	
	//	Time of Flight Identification
	vector<TH2F*>	TOF			;
	
	//	EDE Identification
	vector<TH2F*>	EDE			;
	
	//	Kinematic Line
	KinematicLine				;
	
	//	Excitation Energy
	ExcitationEnergy			;*/

};

#endif
