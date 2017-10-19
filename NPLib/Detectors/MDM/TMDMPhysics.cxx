/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu     *
 *                                                                           *
 * Creation Date  : October 2017                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold MDM Treated  data                                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TMDMPhysics.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"
#include "NPSystemOfUnits.h"

//   ROOT
#include "TF1.h"
#include "TMath.h"
#include "TChain.h"
#include "TGraph.h"
#include "TVector3.h"


ClassImp(TMDMPhysics)

namespace {

const double ZPOS_[4] = { 2.0, 17.1, 33.4, 49.7 }; // cm

}


///////////////////////////////////////////////////////////////////////////
TMDMPhysics::TMDMPhysics()
: m_EventData(new TMDMData),
	m_PreTreatedData(new TMDMData),
	m_EventPhysics(this),
	m_Spectra(0),
	m_X_Threshold(1000000), // junk value
	m_Y_Threshold(1000000), // junk value
	m_NumberOfDetectors(0) {

	m_Rayin = 0;
	m_FitMethod = 0;
	m_ParticleA = 0;
	m_ParticleZ = 0;
	m_ParticleQ = 0;
	m_Particle = 0;
	m_Reaction = 0;
	SetLightParticleAngles(0,0);
}

///////////////////////////////////////////////////////////////////////////
TMDMPhysics::~TMDMPhysics(){
//	if(m_Rayin) { delete m_Rayin; m_Rayin = 0; }
	if(m_Particle) { delete m_Particle; m_Particle = 0; }
}

///////////////////////////////////////////////////////////////////////////
/// A usefull method to bundle all operation to add a detector
void TMDMPhysics::AddDetector(double angle, double field, const std::string& rayin){
  // In That simple case nothing is done
  // Typically for more complex detector one would calculate the relevant 
  // positions (stripped silicon) or angles (gamma array)

	m_Angle = angle;
	m_Field = field;
	m_Rayin = new MDMTrace::Rayin(rayin, false);
	m_Trace = MDMTrace::Instance();
	m_Trace->SetMDMAngle(angle/NPUNITS::mrad);  // mrad
  m_Trace->SetMDMDipoleField(field/NPUNITS::gauss); // gauss
	
  m_NumberOfDetectors++;
} 

///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();

  // match x and y
  UInt_t mysizeX = m_PreTreatedData->GetMultX();
  UInt_t mysizeY = m_PreTreatedData->GetMultY();
	UInt_t i0=-1, i1=-1;
  for (UShort_t ix = 0; ix < mysizeX ; ix++) {
    for (UShort_t iy = 0; iy < mysizeY ; iy++) {
      if (m_PreTreatedData->GetX_DetectorNbr(ix) == m_PreTreatedData->GetY_DetectorNbr(iy)) {
				int detno = m_PreTreatedData->GetX_DetectorNbr(ix);

				if(detno >= 0 && detno < 4) {
					DetectorNumber.push_back(detno);
					Xpos.push_back(m_PreTreatedData->Get_Xpos(ix));
					Ypos.push_back(m_PreTreatedData->Get_Ypos(iy));
					Zpos.push_back(ZPOS_[detno]);
				}
						
				if(detno == 0) { i0 = ix; }
				if(detno == 1) { i1 = ix; }
      }
    }
  }

	// calculate angles from first 2 wires
	if(i0!=-1 && i1!=-1) {
		Xang = atan((Xpos[i1] - Xpos[i0]) / (Zpos[i1] - Zpos[i0]));
		Yang = atan((Ypos[i1] - Ypos[i0]) / (Zpos[i1] - Zpos[i0]));
	}

	MinimizeTarget();
}

///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // X - position
  UInt_t mysize = m_EventData->GetMultX();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Xpos(i) < m_X_Threshold) {
      Double_t Xpos = Cal->ApplyCalibration("MDM/XPOS"+NPL::itoa(m_EventData->GetX_DetectorNbr(i)),m_EventData->Get_Xpos(i));
      if (true) {
        m_PreTreatedData->SetXpos(m_EventData->GetX_DetectorNbr(i), Xpos);
      }
    }
  }

	// Y - position
  mysize = m_EventData->GetMultY();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Ypos(i) < m_Y_Threshold) {
      Double_t Ypos = Cal->ApplyCalibration("MDM/YPOS"+NPL::itoa(m_EventData->GetY_DetectorNbr(i)),m_EventData->Get_Ypos(i));
      if (true) {
        m_PreTreatedData->SetYpos(m_EventData->GetY_DetectorNbr(i), Ypos);
      }
    }
  }
}


///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigMDM.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigMDM.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigMDM.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigMDM.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
	bool haveQ = false;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigMDM";
    if (LineBuffer.compare(0, name.length(), name) == 0) 
      ReadingStatus = true;

		cout << "Reading " << FileName << "...\n";
    // loop on tokens and data
    while (ReadingStatus ) {
      whatToDo="";
      AnalysisConfigFile >> whatToDo;

      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
      }

      else if (whatToDo=="X_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_X_Threshold = atof(DataBuffer.c_str());
        cout << "\t" << whatToDo << " " << m_X_Threshold << endl;
      }

      else if (whatToDo=="Y_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Y_Threshold = atof(DataBuffer.c_str());
        cout << "\t" << whatToDo << " " << m_Y_Threshold << endl;
      }

			else if (whatToDo=="FIT_METHOD") {
				AnalysisConfigFile >> DataBuffer;
				m_FitMethod = atoi(DataBuffer.c_str());
				cout << "\t" << whatToDo << " " << m_FitMethod << endl;
			}

			else if (whatToDo=="RECON_A") {
				AnalysisConfigFile >> DataBuffer;
				m_ParticleA = atoi(DataBuffer.c_str());
				cout << "\t" << whatToDo << " " << m_ParticleA << endl;
			}

			else if (whatToDo=="RECON_Z") {
				AnalysisConfigFile >> DataBuffer;
				m_ParticleZ = atoi(DataBuffer.c_str());
				cout << "\t" << whatToDo << " " << m_ParticleZ << endl;
			}

			else if (whatToDo=="RECON_Q") {
				AnalysisConfigFile >> DataBuffer;
				m_ParticleQ = atoi(DataBuffer.c_str());
				haveQ = true;
				cout << "\t" << whatToDo << " " << m_ParticleZ << endl;
			}

      else {
        ReadingStatus = false;
      }
    }
  }
	if(!haveQ) { 
		m_ParticleQ = m_ParticleZ;
		cout << "\t" << "No RECON_Q found, setting particle Q = Z (fully stripped)\n";
	}
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::Clear() {
  DetectorNumber.clear();
  Xpos.clear();
  Ypos.clear();
	Zpos.clear();
	Xang = -1000;
	Yang = -1000;
	Target_Xang = -1000;
	Target_Yang = -1000;
	Target_Ekin = -1000;
	Fit_Chi2    = -1000;
	for(int i=0; i< 4; ++i) {
		Fit_Xpos[i] = -1000;
	}
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("MDM");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> tokens = {"Angle","Field","Xaccept","Yaccept","Rayin"};

  for(UInt_t i = 0 ; i < blocks.size() ; i++){
		if(blocks[i]->HasTokenList(tokens)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  MDM " << i+1 <<  endl;
      double Angle = blocks[i]->GetDouble("Angle","deg");
			double Field = blocks[i]->GetDouble("Field","gauss");
      double XA    = blocks[i]->GetDouble("XAccept","deg");
      double YA    = blocks[i]->GetDouble("YAccept","deg");
      string Rayin = blocks[i]->GetString("Rayin");
      AddDetector(Angle,Field,Rayin);
    }
    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

  ReadAnalysisConfig();
	m_Particle = new NPL::Nucleus(m_ParticleZ, m_ParticleA);
}

///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::InitSpectra() {
  m_Spectra = new TMDMSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TMDMPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}

///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    Cal->AddParameter("MDM", "D"+ NPL::itoa(i+1)+"_ENERGY","MDM_D"+ NPL::itoa(i+1)+"_ENERGY");
    Cal->AddParameter("MDM", "D"+ NPL::itoa(i+1)+"_TIME","MDM_D"+ NPL::itoa(i+1)+"_TIME");
  }
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("MDM",  true );
  inputChain->SetBranchAddress("MDM", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("MDM", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("MDM", "TMDMPhysics", &m_EventPhysics);
}


///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::SendRay(double thetaX,double thetaY,double Ekin) const {
  // send ray through mdm and read wires x, y, and theta_x, theta_y
	// n.b. const due to requirements of ROOT::Math::IMultiGenFinction
	// but it needs to change internal variables (Fit_***), so these are
	// made muitable in their definition
	//
	m_Trace->SetScatteredMass(GetParticle()->Mass()/amu_c2);
	m_Trace->SetScatteredCharge(GetParticleQ());
	m_Trace->SetScatteredAngle(thetaX, thetaY); // deg
	m_Trace->SetScatteredEnergy(Ekin);
			
	m_Trace->SendRay();
	m_Trace->
		GetOxfordWirePositions(Fit_AngleX,Fit_Xpos[0],Fit_Xpos[1],Fit_Xpos[2],Fit_Xpos[3],
													 Fit_AngleY,Fit_Ypos[0],Fit_Ypos[1],Fit_Ypos[2],Fit_Ypos[3]);
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TMDMPhysics::Construct() {
  return (NPL::VDetector*) new TMDMPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_MDM{
public:
	proxy_MDM(){
		NPL::DetectorFactory::getInstance()->AddToken("MDM","MDM");
		NPL::DetectorFactory::getInstance()->AddDetector("MDM",TMDMPhysics::Construct);
	}
};

proxy_MDM p_MDM;
}

///////////////////////////////////////////////////////////////////////////
//   TARGET MINIMIZATION                                                 //
///////////////////////////////////////////////////////////////////////////

#include <memory>
#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"


namespace {

class FitFunctor : public ROOT::Math::IMultiGenFunction {
protected:
	FitFunctor(const TMDMPhysics* mdm):
		m_MDM(mdm){
	}

	ROOT::Math::IMultiGenFunction*	Clone() const{
		FitFunctor* out = new FitFunctor(m_MDM);
		return out;
	}
	
	// overload w/ specific fit function
	virtual unsigned int NDim() const { return 0; }

private:
	virtual double DoEval (const double*) const { return 0; }

public:
	const TMDMPhysics* m_MDM;
};

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

} // namespace


void TMDMPhysics::MinimizeTarget(){  // outputs, MeV, rad
	// reset values
	Fit_Chi2 = 0;
	for(int i=0; i< 4; ++i) {
		Fit_Xpos[i] = Fit_Ypos[i] = 0;
	}
	Fit_AngleX = Fit_AngleY = 0;

	switch(m_FitMethod) {
	case 0: // no fit
		Target_Ekin = CalculateCentralEnergy();
		Target_Ekin = CalculateCentralEnergy();
		CalculateAnalyticAngles(Target_Xang, Target_Yang);
		break;
	case 1:
		MinimizeWithXangle();
		break;
	case 2:
		MinimizeWithXangle();
		break;
	default:
		std::cerr << "Invalid fit method: " << m_FitMethod << ", defaulting to no fit...\n";
		m_FitMethod = 0;
		MinimizeTarget();
		break;
	}
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
		std::unique_ptr<TGraph> kin (m_Reaction->GetTheta3VsTheta4);
		double ThetaHeavy = kin.Eval(m_Light_ThetaLab);
		double PhiHeavy = m_Light_PhiLab - 180;
		if(m_Light_PhiLab < 0) { PhiHeavy += 360; }

		TVector3 v(sin(ThetaHeavy*deg)*cos(PhiHeavy*deg), 
							 sin(ThetaHeavy*deg)*sin(PhiHeavy*deg), 
							 cos(PhiHeavy*deg));

		Target_Xang = atan(v.X()/v.Z())/deg;
		Target_Yang = atan(v.Y()/v.Z())/deg;
	}
	
	R2WireX f(this);
	
	ROOT::Minuit2::Minuit2Minimizer min (ROOT::Minuit2::kMigrad); 
	InitializeMinimizerWithDefaults(&min);
	min.SetFunction(f);
	// Set the free variables to be minimized!
	min.SetVariable(0,"ekin"  ,Target_Ekin, 0.01 /*step*/);
	min.Minimize();

	Target_Ekin = min.X()[0] * MeV; // MeV
	Fit_Chi2 = f(min.X());
}

void TMDMPhysics::InitializeMinimizerWithDefaults(ROOT::Math::Minimizer* min){
	min->SetMaxFunctionCalls(10000);
	min->SetMaxIterations(1000);
	min->SetTolerance(0.001);
}

double TMDMPhysics::CalculateCentralEnergy(){
	double brho = (m_Field/tesla)*1.6; // tesla*meter
	m_Particle->SetBrho(brho);
	m_Particle->BrhoToEnergy(m_ParticleQ); // charge state
	return m_Particle->GetEnergy()/MeV;
}

void TMDMPhysics::CalculateAnalyticAngles(double& tx, double& ty){
	// n.b. not sure if this is universal!!
	// Taken from simulation of 14C(a,4n)14O @560 MeV
	// detecting 14O in MDM
	tx = -0.656*pow(Xang,2) + -0.414486*Xang; // RAD
	ty = -3.97*Yang; // RAD
}
