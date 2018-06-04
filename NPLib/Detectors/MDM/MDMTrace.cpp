#include "MDMTrace.h"
#include <iostream>
#include <math.h>
#include <stdio.h>

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;


MDMTrace::Rayin::Rayin(const string& filename, bool check):
  isOwner(true)
{
  if(check)
	{
		int length;
		{
			ifstream test_("rayin.dat");
			test_.seekg(0, ios::end);
			length = test_.tellg();
		}
		if(length > 0) {
			string answer;
			cerr << "\nWARNING: The file \"rayin.dat\" already exists in the current directory. "
					 << "Enter 'y' to remove it and replace it with a link to \"" << filename << "\" "
					 << "OR enter 'n' to continue with the existing \"rayin.dat\" file.\n"
					 << "Or enter 'q' to abort the program...\n";
			while(1) {
				cin >> answer;
				if     (answer == "y" || answer == "Y") { break;                  }
				else if(answer == "n" || answer == "N") { isOwner = false; break; }
				else if(answer == "q" || answer == "Q") { exit(1);                }
				else {
					cerr << "ERROR: invalid response: \"" << answer << "\". Please enter 'y', 'n', or 'q'\n";
				}
			}
		}
	}
  if(isOwner) { 
    cerr << "Creating link \"rayin.dat\" to the file \"" << filename << "\"...\n";
    stringstream sstr;
    sstr << "ln -fs " <<filename << " rayin.dat";
    system(sstr.str().c_str());
  }
}

MDMTrace::Rayin::~Rayin()
{
  if(isOwner) {
    cerr << "Removing link \"rayin.dat\"...\n";
    system("rm -f rayin.dat");
  }
}

#ifdef USE_RAYTRACE
#define EXT_ extern
#else
#define EXT_
#endif

extern "C" {
  EXT_ struct {
    double DATA[200][75];
    double ITITLE[200];
  } blck0_;

  EXT_ struct {
    double XI[1000];
    double YI[1000];
    double ZI[1000];
    double VXI[1000];
    double VYI[1000];
    double VZI[1000];
    double DELP[1000];
  } blck1_;

  EXT_ struct {
    double XO[1000];
    double YO[1000];
    double ZO[1000];
    double VXO[1000];
    double VYO[1000];
    double VZO[1000];
    double RTL[1000];
    double RLL[1000];
  } blck2_;

  EXT_ struct {
    double ENERGY;
    double VEL;
    double PMASS;
    double Q0;
  } blck4_;

  EXT_ struct {
    double THTSPEC;
    double TRGT1;
    double AM[4];
    double QVALUE;
    double EEXC;
    double THETACAL[10];
    double EKINE;
  } kineblck_;

EXT_ void raytrace_(int*)
#ifdef USE_RAYTRACE
	;
#else
{
	blck2_.XO[0]   = 1e10;
	blck2_.YO[0]   = 1e10;
	blck2_.ZO[0]   = 1e10;
	blck2_.VXO[0]  = 1e10;
	blck2_.VYO[0]  = 1e10;
	blck2_.VZO[0]  = 1e10;
	blck2_.RTL[0] = 1e10;
	blck2_.RLL[0] = 1e10;
	}
#endif
} // extern "C" {

MDMTrace* MDMTrace::instance_ = 0;
double MDMTrace::jeffParams_[6] = {-0.51927,0.038638,0.028404,-0.022797,-0.019275,0.755583};
double MDMTrace::oxfordWireSpacing_[3] = {15.1,16.3,16.3};

MDMTrace* MDMTrace::Instance() {
  if(!instance_) {
    instance_ = new MDMTrace;
    int flag = 0;
    raytrace_(&flag);
    kineblck_.TRGT1 = 0.;
    instance_->beamEnergy_ = 0;
    instance_->scatteredEnergy_ = 0;
    instance_->beamPositions_[0] = 0.;
    instance_->beamPositions_[1] = 0.;
    instance_->beamPositions_[2] = 0.;

  }
  return instance_;
}

void MDMTrace::SetBeamEnergy(double energy) {
  beamEnergy_ = energy;
}

double MDMTrace::GetBeamEnergy() const {
  return beamEnergy_;
}

void MDMTrace::SetMDMAngle(double angle) {
  kineblck_.THTSPEC = angle;
}

double MDMTrace::GetMDMAngle() const {
  return kineblck_.THTSPEC;
}

void MDMTrace::SetMDMBRho(double bRho) {
  double field = bRho/160.*1000;
  SetMDMDipoleField(field);
}

void MDMTrace::SetMDMDipoleField(double field) {
  double hallProbe = field/1.034;
  double multipoleHallProbe = hallProbe*0.71;
  std::cout << "CONFIRM: Hall probe for dipole should be set to " << hallProbe << std::endl;
  std::cout << "CONFIRM: Hall probe for multipole should be set to " << multipoleHallProbe << std::endl;
  double BQR = -1.*multipoleHallProbe*1e-4*jeffParams_[5];
  double BHR = BQR*jeffParams_[1]/jeffParams_[0];
  double BOR = BQR*jeffParams_[2]/jeffParams_[0];
  double BDR = BQR*jeffParams_[3]/jeffParams_[0];
  double BDDR = BQR*jeffParams_[4]/jeffParams_[0];

  blck0_.DATA[4][14] = field*1.e-4;
  blck0_.DATA[3][13]=BQR;
  blck0_.DATA[3][14]=BHR;
  blck0_.DATA[3][15]=BOR;
  blck0_.DATA[3][16]=BDR;
  blck0_.DATA[3][17]=BDDR;
}

double MDMTrace::GetMDMDipoleField() const {
  return blck0_.DATA[4][14]*1.e4;
}

void MDMTrace::SetScatteredAngle(double angle) { // degrees
  kineblck_.THETACAL[0] = angle;
  scatteredAngles_[0] = angle;
  scatteredAngles_[1] = 0.;
}

void MDMTrace::SetScatteredAngle(double xAngle,double yAngle) { // degrees
  kineblck_.THETACAL[0] = xAngle;
  scatteredAngles_[0] = xAngle;
  scatteredAngles_[1] = yAngle;
}

void MDMTrace::SetBeamPosition(double x, double y, double z) { // cm
  beamPositions_[0] = x;
  beamPositions_[1] = y;
  beamPositions_[2] = z;
}

double MDMTrace::GetScatteredAngle() const {
  return kineblck_.THETACAL[0];
}

void MDMTrace::SetQValue(double qValue) {
  kineblck_.QVALUE =qValue;
}

double MDMTrace::GetQValue() const {
  return kineblck_.QVALUE;
}

void MDMTrace::SetResidualEnergy(double energy) {
  kineblck_.EEXC = energy;
}

double MDMTrace::GetResidualEnergy() const {
  return kineblck_.EEXC;
}

void MDMTrace::SetScatteredEnergy(double energy) {
  scatteredEnergy_ = energy;
}

double MDMTrace::GetScatteredEnergy() const {
  return scatteredEnergy_;
}

void MDMTrace::SetTargetMass(double mass) {
  kineblck_.AM[1] = mass;
}

double MDMTrace::GetTargetMass() const {
  return kineblck_.AM[1];
}

void MDMTrace::SetProjectileMass(double mass) {
  kineblck_.AM[0] = mass;
}

double MDMTrace::GetProjectileMass() const {
  return kineblck_.AM[0];
}

void MDMTrace::SetScatteredMass(double mass) {
  blck4_.PMASS = mass;
}

double MDMTrace::GetScatteredMass() const {
  return blck4_.PMASS;
}

void MDMTrace::SetScatteredCharge(double charge) {
  blck4_.Q0 = charge;
}

double MDMTrace::GetScatteredCharge() const {
  return blck4_.Q0;
}

double MDMTrace::GetEnergyAfterKinematics() const {
  return kineblck_.EKINE*(1.+blck1_.DELP[0]/100.);
}

void MDMTrace::SendRayWithKinematics() {
  int flag = 1;
  blck4_.ENERGY = beamEnergy_;
  raytrace_(&flag);
}

void MDMTrace::SendRay() {
  int flag = 2;
  blck4_.ENERGY = scatteredEnergy_;
  blck1_.XI[0]=beamPositions_[0];
  blck1_.YI[0]=beamPositions_[1];
  blck1_.ZI[0]=beamPositions_[2];
  blck1_.VXI[0]=17.453*(scatteredAngles_[0]-kineblck_.THTSPEC);
  blck1_.VYI[0]=17.453*(scatteredAngles_[1]);
  blck1_.VZI[0]=0.;
  blck1_.DELP[0]=0.;
  raytrace_(&flag);
}

void MDMTrace::GetPositionAngleFirstWire(double& pos, double& ang) const {
  pos  = blck2_.XO[0];
  ang  = blck2_.VXO[0]/1000.*180./3.14159;
}

void MDMTrace::GetPositionAngleFirstWire(double& posX, double& posY, double& angX, double& angY) const {
  posX  = blck2_.XO[0];
  posY  = blck2_.YO[0];
  angX  = blck2_.VXO[0]/1000.*180./3.14159;
  angY  = blck2_.VYO[0]/1000.*180./3.14159;
}

void MDMTrace::GetOxfordWirePositions(double& x1,double& x2,double& x3,double& x4) {
  double oxfordWire1Pos = blck2_.XO[0];
  double oxfordWire1Ang = blck2_.VXO[0];
  double tanAngle = tan(1e-3*oxfordWire1Ang);

  x1 = oxfordWire1Pos;
  x2 = oxfordWire1Pos+tanAngle*oxfordWireSpacing_[0];
  x3 = oxfordWire1Pos+tanAngle*(oxfordWireSpacing_[0]+oxfordWireSpacing_[1]);
  x4 = oxfordWire1Pos+tanAngle*(oxfordWireSpacing_[0]+oxfordWireSpacing_[1]+oxfordWireSpacing_[2]);
}

void MDMTrace::GetOxfordWirePositions(double& a1,double& x1,double& x2,double& x3,double& x4) {
  double oxfordWire1Pos = blck2_.XO[0];
  double oxfordWire1Ang = blck2_.VXO[0];
  double tanAngle = tan(1e-3*oxfordWire1Ang);

  a1 = oxfordWire1Ang;

  x1 = oxfordWire1Pos;
  x2 = oxfordWire1Pos+tanAngle*oxfordWireSpacing_[0];
  x3 = oxfordWire1Pos+tanAngle*(oxfordWireSpacing_[0]+oxfordWireSpacing_[1]);
  x4 = oxfordWire1Pos+tanAngle*(oxfordWireSpacing_[0]+oxfordWireSpacing_[1]+oxfordWireSpacing_[2]);
} 

void MDMTrace::GetOxfordWirePositions(double& a1,double& x1,double& x2,double& x3,double& x4,
				      double& b1,double& y1,double& y2,double& y3,double& y4) {
  double oxfordWire1PosX = blck2_.XO[0];
  double oxfordWire1AngX = blck2_.VXO[0]; // mrad
  double tanAngleX = tan(1e-3*oxfordWire1AngX);

  double oxfordWire1PosY = blck2_.YO[0];
  double oxfordWire1AngY = blck2_.VYO[0]; // mrad
  double tanAngleY = tan(1e-3*oxfordWire1AngY);

  // x - plane //
  a1 = oxfordWire1AngX; // mrad

  x1 = oxfordWire1PosX;
  x2 = oxfordWire1PosX+tanAngleX*oxfordWireSpacing_[0];
  x3 = oxfordWire1PosX+tanAngleX*(oxfordWireSpacing_[0]+oxfordWireSpacing_[1]);
  x4 = oxfordWire1PosX+tanAngleX*(oxfordWireSpacing_[0]+oxfordWireSpacing_[1]+oxfordWireSpacing_[2]);

  // y - plane //
  b1 = oxfordWire1AngY; // mrad

  y1 = oxfordWire1PosY;
  y2 = oxfordWire1PosY+tanAngleY*oxfordWireSpacing_[0];
  y3 = oxfordWire1PosY+tanAngleY*(oxfordWireSpacing_[0]+oxfordWireSpacing_[1]);
  y4 = oxfordWire1PosY+tanAngleY*(oxfordWireSpacing_[0]+oxfordWireSpacing_[1]+oxfordWireSpacing_[2]);
} 
