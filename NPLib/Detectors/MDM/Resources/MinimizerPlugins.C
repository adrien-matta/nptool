// ROOT
#include <TGraph.h>
#include <TVector3.h>
// NPTOOL
#include "NPSystemOfUnits.h"
// MDM
#include "TMDMPhysicsMinimizer.h"
#include "TMDMPhysics.h"


// Helper functions
// Check if xpos between limits for MDM
bool check_good_x(TMDMPhysics* m_MDM){
	int ngood = 0;
	for(size_t i=0; i< m_MDM->Xpos.size(); ++i) {
		if(m_MDM->Xpos[i] > m_MDM->GetXlow() &&
			 m_MDM->Xpos[i] < m_MDM->GetXhigh()) {
			++ngood;
		}
	}
	return (ngood > 1);
}

bool check_good_y(TMDMPhysics* m_MDM){
	int ngood = 0;
	for(size_t i=0; i< m_MDM->Ypos.size(); ++i) {
		if(m_MDM->Ypos[i] > m_MDM->GetYlow() &&
			 m_MDM->Ypos[i] < m_MDM->GetYhigh()) {
			++ngood;
		}
	}
	return (ngood > 1);
}

// Chi2 calculation (x-only) (used in many classes)
double calc_chi2_xy(TMDMPhysics* m_MDM){
	int ngood = 0;
	double chi2 = 0;
	for(size_t i=0; i< m_MDM->Xpos.size(); ++i) {
		size_t iDet = m_MDM->DetectorNumber[i];
		if(iDet > 3) { continue; }

		double X = m_MDM->Xpos[i];
		double F = m_MDM->Fit_Xpos[iDet];

		double Y = m_MDM->Ypos[i];
		double G = m_MDM->Fit_Ypos[iDet];
		
		if(X > m_MDM->GetXlow() && X < m_MDM->GetXhigh() &&
			 Y > m_MDM->GetYlow() && Y < m_MDM->GetYhigh() ){
			++ngood;
			double w = 1.; // "weight"
			double ch2;
			ch2 = pow(X - F, 2) / w;   chi2 += ch2;
			ch2 = pow(Y - G, 2) / w;   chi2 += ch2;
		}
	}
	return chi2 / ngood;
}

// Chi2 calculation (x-only) (used in many classes)
double calc_chi2_x(TMDMPhysics* m_MDM){
	int ngood = 0;
	double chi2 = 0;
	for(size_t i=0; i< m_MDM->Xpos.size(); ++i) {
		size_t iDet = m_MDM->DetectorNumber[i];
		if(iDet > 3) { continue; }

		double X = m_MDM->Xpos[i];
		double F = m_MDM->Fit_Xpos[iDet];

		if(X > m_MDM->GetXlow() && X < m_MDM->GetXhigh()){
			++ngood;
			double w = 1.; // "weight"
			double ch2 = pow(X - F, 2) / w;
			chi2 += ch2;
		}
	}
	return chi2 / ngood;
}

// R2 calculation (x-only) (used in many classes)
double calc_r2_x(TMDMPhysics* m_MDM){
	int nnn = 0;
	double ybar = 0;
	for(const auto& x : m_MDM->Xpos) {
		if(x > m_MDM->GetXlow() && x < m_MDM->GetXhigh()) {
			++nnn;	ybar += x;
		}
	}
	ybar /= nnn;

	double SStot = 0, SSres = 0;
	for(size_t i=0; i< m_MDM->Xpos.size(); ++i) {
		size_t iDet = m_MDM->DetectorNumber[i];
		if(iDet > 3) { continue; }

		double X = m_MDM->Xpos[i];				
		double F = m_MDM->Fit_Xpos[iDet];
		if(X > m_MDM->GetXlow() && X < m_MDM->GetXhigh()) {
			SStot += pow(X - ybar, 2);
			SSres += pow(X - F, 2);
		}
	}
	double  r2 = 1 - (SSres/SStot);
	return -r2; // negative (to minimize)
}


// Minimize over x, y angle and ekin, using unweighted chi2
// Initial starting point for angles taken as zero (change as needed)
// Initial starting point for ekin taken as central brho of magnet
class MinimizerChi2XY : public TMDMPhysicsMinimizer{
public:
	MinimizerChi2XY(TMDMPhysics* mdm = 0):
		TMDMPhysicsMinimizer(3, mdm){
	}
	ROOT::Math::IMultiGenFunction* Clone() const{
		MinimizerChi2XY* out = new MinimizerChi2XY(m_MDM);
		return out;
	}
	void Initialize(){
		m_FixedThetaX = false;
		m_FixedThetaY = false;
		m_FixedEkin   = false;

		m_InitialThetaX = 0.;
		m_InitialThetaY = 0.;
		m_InitialEkin = m_MDM->CalculateCentralEnergy();
	}
private:
	double DoEval(const double* x) const{
		double thetaX = x[0]; // deg
		double thetaY = x[1]; // deg
		double Ekin   = x[2]; // MeV
		if(check_good_x(m_MDM)){
			m_MDM->SendRay(thetaX,thetaY,Ekin);
		}

		// calculate Chi2
		return calc_chi2_xy(m_MDM);
	}
};

// Minimize over x angle and ekin, using unweighted chi2
// Initial starting point for angles taken as zero (change as needed)
// Initial starting point for ekin taken as central brho of magnet
class MinimizerChi2X : public TMDMPhysicsMinimizer{
public:
	MinimizerChi2X(TMDMPhysics* mdm = 0):
		TMDMPhysicsMinimizer(2, mdm){
	}
	ROOT::Math::IMultiGenFunction* Clone() const{
		MinimizerChi2X* out = new MinimizerChi2X(m_MDM);
		return out;
	}
	void Initialize(){
		m_FixedThetaX = false;
		m_FixedThetaY = true;
		m_FixedEkin   = false;

		m_InitialThetaX = 0.;
		m_InitialThetaY = 0.;
		m_InitialEkin = m_MDM->CalculateCentralEnergy();
	}
private:
	double DoEval(const double* x) const{
		double thetaX = x[0]; // deg
		double Ekin   = x[2]; // MeV
		if(check_good_x(m_MDM)){
			m_MDM->SendRay(thetaX,m_InitialThetaY,Ekin);
		}

		// calculate Chi2
		return calc_chi2_x(m_MDM);
	}
};


// Minimize over x-angle and ekin, using linear R2
// Initial starting point for angles taken as zero (change as needed)
// Initial starting point for ekin taken as central brho of magnet
class MinimizerR2X : public TMDMPhysicsMinimizer{
public:
	MinimizerR2X(TMDMPhysics* mdm = 0):
		TMDMPhysicsMinimizer(2, mdm){
	}
	ROOT::Math::IMultiGenFunction* Clone() const{
		MinimizerR2X* out = new MinimizerR2X(m_MDM);
		return out;
	}
	void Initialize(){
		m_FixedThetaX = false;
		m_FixedThetaY = true;
		m_FixedEkin   = false;

		m_InitialThetaX = 0.;
		m_InitialThetaY = 0.;
		m_InitialEkin = m_MDM->CalculateCentralEnergy();
	}
private:
	double DoEval(const double* x) const{
		double thetaX = x[0]; // deg
		double Ekin   = x[1]; // MeV
		if(check_good_x(m_MDM)){
			m_MDM->SendRay(thetaX,m_InitialThetaY,Ekin);
		}

		// calculate R2
		return calc_r2_x(m_MDM);
	}
};

// Minimize over ekin only
// Take x- and y- angle from the measured light particle angle
// and reaction kinematics
// Initial starting point for ekin taken as central brho of magnet
class MinimizerR2XYLight : public TMDMPhysicsMinimizer{
public:
	MinimizerR2XYLight(TMDMPhysics* mdm = 0):
		TMDMPhysicsMinimizer(1, mdm){
	}
	ROOT::Math::IMultiGenFunction* Clone() const{
		MinimizerR2XYLight* out = new MinimizerR2XYLight(m_MDM);
		return out;
	}
	void Initialize(){
		m_FixedThetaX = true;
		m_FixedThetaY = true;
		m_FixedEkin   = false;

		
		if(m_MDM->GetReaction() == 0) {
			static bool warn = true;
			if(warn) {
				warn = false;
				std::cerr << "WARNING in MinimizerR2XYLight::Initialize() :: " <<
					"Reaction not set, defaulting to ZERO angle for Theta_X and Theta_Y at " <<
					"the target...\n";
			}
			m_InitialThetaX = 0;
			m_InitialThetaY = 0;
		} else {
			double ThetaLight, PhiLight;
			m_MDM->GetLightParticleAngles(ThetaLight, PhiLight);
			
			std::unique_ptr<TGraph> kin (m_MDM->GetReaction()->GetTheta3VsTheta4(0.1));
			double ThetaHeavy = kin->Eval(ThetaLight);
			double PhiHeavy = PhiLight - 180;
			if(PhiLight < 0) { PhiHeavy += 360; }

			TVector3 v;
			v.SetMagThetaPhi(1,ThetaHeavy*NPUNITS::deg, PhiHeavy*NPUNITS::deg);
			m_InitialThetaX = atan(v.X()/v.Z())/NPUNITS::deg;
			m_InitialThetaY = atan(v.Y()/v.Z())/NPUNITS::deg;
		}
		m_InitialEkin = m_MDM->CalculateCentralEnergy();
	}
private:
	double DoEval(const double* x) const{
		double Ekin   = x[0]; // MeV
		if(check_good_x(m_MDM)){
			m_MDM->SendRay(m_InitialThetaX,m_InitialThetaY,Ekin);
		}
		
		// calculate R2 (from wires x)
		return calc_r2_x(m_MDM);
	}
};



# if 0

void TMDMPhysics::CalculateAnalyticAngles(double& tx, double& ty){
	// n.b. not sure if this is universal!!
	// Taken from simulation of 14C(a,4n)14O @560 MeV
	// detecting 14O in MDM
	tx = -0.656*pow(Xang,2) + -0.414486*Xang; // RAD
	ty = -3.97*Yang; // RAD
}


// Fit both x-angle and energy to the wire spectra
// take y-angle from "analytic" evaluation of RAYTRACE
// correlations
void TMDMPhysics::MinimizeWithXangle(){
	Target_Ekin = CalculateCentralEnergy();
	CalculateAnalyticAngles(Target_Xang, Target_Yang);

	std::unique_ptr<FitFunctor> f (nullptr);
	if(m_FitMethod == 1) {
		f.reset(new Chi2WireX(this));
	}
	else if(m_FitMethod == 2) {
		f.reset(new R2WireX(this));
	}
	else {
		assert(0 && "Shouldn't get here!!!");
	}

	ROOT::Minuit2::Minuit2Minimizer min (ROOT::Minuit2::kMigrad); 
	InitializeMinimizerWithDefaults(&min);
	min.SetFunction(*f);
	// Set the free variables to be minimized!
	min.SetVariable(0,"thetax",Target_Xang, 0.01 /*step*/);
	min.SetVariable(1,"ekin"  ,Target_Ekin, 0.01 /*step*/);
	min.Minimize();

	Target_Xang = min.X()[0] * deg; // rad
	Target_Ekin = min.X()[1] * MeV; // MeV
	Fit_Chi2 = f->operator()(min.X());
}

// Fit only the energy to the wire spectra
// Take angle from the LIGHT particle and reaction
// Minimize over x-angle and ekin, using linear R2
// Initial starting point for angles taken as zero (change as needed)
// Initial starting point for ekin taken as central brho of magnet
class MinimizerR2X : public TMDMPhysicsMinimizer{
public:
	MinimizerR2X(TMDMPhysics* mdm = 0):
		TMDMPhysicsMinimizer(2, mdm){
	}
	ROOT::Math::IMultiGenFunction* Clone() const{
		MinimizerR2X* out = new MinimizerR2X(m_MDM);
		return out;
	}
	void Initialize(){
		m_FixedThetaX = false;
		m_FixedThetaY = true;
		m_FixedEkin   = false;

		m_InitialThetaX = 0.;
		m_InitialThetaY = 0.;
		m_InitialEkin = m_MDM->CalculateCentralEnergy();
	}
private:
	double DoEval(const double* x) const{
		double thetaX = x[0]; // deg
		double Ekin   = x[1]; // MeV
		if(check_good_x(m_MDM)){
			m_MDM->SendRay(thetaX,m_InitialThetaY,Ekin);
		}

		// calculate R2
		return calc_r2_x(m_MDM);
	}
};

// kinematics
void TMDMPhysics::MinimizeUsingLightParticleAngle(){
	Target_Ekin = CalculateCentralEnergy();
	if(m_Reaction == 0) {
		static bool warn = true;
		if(warn) {
			warn = false;
			std::cerr << "WARNING in TMDMPhysics::MinimizeUsingLightParticleAngle() :: " <<
				"m_Reaction not set, defaulting to ZERO angle for Theta_X and Theta_Y at " <<
				"the target...\n";
		}
		Target_Xang = 0;
		Target_Yang = 0;
	} else {
		std::unique_ptr<TGraph> kin (m_Reaction->GetTheta3VsTheta4(0.1));
		double ThetaHeavy = kin->Eval(m_Light_ThetaLab);
		double PhiHeavy = m_Light_PhiLab - 180;
		if(m_Light_PhiLab < 0) { PhiHeavy += 360; }

		TVector3 v;
		v.SetMagThetaPhi(1,ThetaHeavy*deg,PhiHeavy*deg);
		Target_Xang = atan(v.X()/v.Z())/deg;
		Target_Yang = atan(v.Y()/v.Z())/deg;
	}
	
	R2WireX1 f(this, Target_Xang, Target_Yang);
	
	ROOT::Minuit2::Minuit2Minimizer min (ROOT::Minuit2::kMigrad); 
	InitializeMinimizerWithDefaults(&min);
	min.SetFunction(f);
	// Set the free variables to be minimized!
	min.SetVariable(0,"ekin"  ,Target_Ekin, 0.01 /*step*/);
	min.Minimize();

	Target_Ekin = min.X()[0] * MeV; // MeV
	Fit_Chi2 = f(min.X());
}




class Chi2WireX : public FitFunctor {
public:
	Chi2WireX(const TMDMPhysics* mdm):
		FitFunctor(mdm) { }

	double DoEval (const double* p) const{
		double thetaX = p[0]; // deg
		double Ekin   = p[1]; // MeV
		m_MDM->SendRay(thetaX,m_MDM->Yang/deg,Ekin);

		// calculate Chi2
		double chi2 = 0;
		assert(m_MDM->Xpos.size() == 4);

		for(int i=0; i< m_MDM->Xpos.size(); ++i) {

			size_t iDet = m_MDM->DetectorNumber[i];
			if(iDet > 3) { continue; }

			double X = m_MDM->Xpos[i];				
			double F = m_MDM->Fit_Xpos[iDet];

			if(X > -20 && X < 20) {
				double w = 1.; // "weight"
				double ch2 = pow(X - F, 2) / w;
				chi2 += ch2;
			}
		}
	
		return chi2;
	}
1
	unsigned int NDim() const { return 2; }
};

class R2WireX : public FitFunctor {
public:
	R2WireX(const TMDMPhysics* mdm):
		FitFunctor(mdm) { }

	double DoEval (const double* p) const{
		double thetaX = p[0]; // deg
		double Ekin   = p[1]; // MeV
		m_MDM->SendRay(thetaX,0,Ekin);

		// calculate R2
		int nnn = 0;
		double ybar = 0;
		for(const auto& x : m_MDM->Xpos) {
			if(x > -20 && x < 20) {
				++nnn;	ybar += x;
			}
		}
		ybar /= nnn;

		double SStot = 0, SSres = 0;
		for(int i=0; i< 4; ++i) {
			size_t iDet = m_MDM->DetectorNumber[i];
			if(iDet > 3) { continue; }

			double X = m_MDM->Xpos[i];				
			double F = m_MDM->Fit_Xpos[iDet];

			if(X > -20 && X < 20) {
				SStot += pow(X - ybar, 2);
				SSres += pow(X - F, 2);
			}
		}

		double r2 = 1 - (SSres/SStot);
		return -r2;
	}

	unsigned int NDim() const { return 2; }
};


class R2WireX1 : public FitFunctor {
public:
	double thetaX,thetaY;
	R2WireX1(const TMDMPhysics* mdm, double thetax, double thetay):
		FitFunctor(mdm) {
		thetaX = thetax;
		thetaY = thetay;
	}

	double DoEval (const double* p) const{
		double Ekin   = p[0]; // MeV
		m_MDM->SendRay(thetaX,thetaY,Ekin);

		// calculate R2
		int nnn = 0;
		double ybar = 0;
		for(const auto& x : m_MDM->Xpos) {
			if(x > -20 && x < 20) {
				++nnn;	ybar += x;
			}
		}
		ybar /= nnn;

		double SStot = 0, SSres = 0;
		for(int i=0; i< 4; ++i) {
			size_t iDet = m_MDM->DetectorNumber[i];
			if(iDet > 3) { continue; }

			double X = m_MDM->Xpos[i];				
			double F = m_MDM->Fit_Xpos[iDet];

			if(X > -20 && X < 20) {
				SStot += pow(X - ybar, 2);
				SSres += pow(X - F, 2);
			}
		}

		double r2 = 1 - (SSres/SStot);
		return -r2;
	}

	unsigned int NDim() const { return 1; }
};


#endif
