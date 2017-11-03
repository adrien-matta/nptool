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
#include <cassert>
#include <memory>
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
#include "TROOT.h"
#include "TVector3.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"


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
	m_ParticleA = 0;
	m_ParticleZ = 0;
	m_ParticleQ = 0;
	m_Particle = 0;
	m_Reaction = 0;
	m_Ex4 = 0;
	m_Ex3 = 0;
	SetLightParticleAngles(0,0);

	m_DoMinimization       = false;
	m_MinimizerName        = "Minuit";
	m_MinimizerAlgorithm   = "";
	m_MinimizerPluginFile  = "";
	m_MinimizerPluginClass = "";
	m_MinimizerFunction    =  0;

	double m_Xlow  = -15;
	double m_Ylow  = -15;
	double m_Xhigh = +15;
	double m_Yhigh = +15;
}

///////////////////////////////////////////////////////////////////////////
TMDMPhysics::~TMDMPhysics(){
//	if(m_Rayin) { delete m_Rayin; m_Rayin = 0; }
	if(m_Particle) { delete m_Particle; m_Particle = 0; }
	if(m_Reaction) { delete m_Reaction; m_Reaction = 0; }
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
  UInt_t mysize = m_PreTreatedData->GetMult();
	UInt_t i0=-1, i1=-1;
  for (UShort_t ix = 0; ix < mysize ; ix++) {
    for (UShort_t iy = 0; iy < mysize ; iy++) {
      if (m_PreTreatedData->GetDetectorNbr(ix) == m_PreTreatedData->GetDetectorNbr(iy)) {
				int detno = m_PreTreatedData->GetDetectorNbr(ix);

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

	// do minimization
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

  UInt_t mysize = m_EventData->GetMult();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Xpos(i) < m_X_Threshold && m_EventData->Get_Ypos(i) < m_Y_Threshold) {
      Double_t Xpos = Cal->ApplyCalibration("MDM/XPOS"+NPL::itoa(m_EventData->GetDetectorNbr(i)),m_EventData->Get_Xpos(i));
      Double_t Ypos = Cal->ApplyCalibration("MDM/YPOS"+NPL::itoa(m_EventData->GetDetectorNbr(i)),m_EventData->Get_Ypos(i));
			Double_t Mass = m_EventData->GetParticleMass(i);
			UShort_t Charge = m_EventData->GetParticleCharge(i);
      if (true) {
				m_PreTreatedData->SetHit(m_EventData->GetDetectorNbr(i), Xpos, Ypos, Charge, Mass);
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

			else if (whatToDo=="X_LOW") {
        AnalysisConfigFile >> DataBuffer;
        m_Xlow = atof(DataBuffer.c_str());
        cout << "\t" << whatToDo << " " << m_Xlow << endl;
      }

			else if (whatToDo=="X_HIGH") {
        AnalysisConfigFile >> DataBuffer;
        m_Xhigh = atof(DataBuffer.c_str());
        cout << "\t" << whatToDo << " " << m_Xhigh << endl;
      }

			else if (whatToDo=="Y_LOW") {
        AnalysisConfigFile >> DataBuffer;
        m_Ylow = atof(DataBuffer.c_str());
        cout << "\t" << whatToDo << " " << m_Ylow << endl;
      }

			else if (whatToDo=="Y_HIGH") {
        AnalysisConfigFile >> DataBuffer;
        m_Yhigh = atof(DataBuffer.c_str());
        cout << "\t" << whatToDo << " " << m_Yhigh << endl;
      }

			else if (whatToDo=="MINIMIZER_PLUGIN_FILE") {
				AnalysisConfigFile >> DataBuffer;
				m_MinimizerPluginFile  = DataBuffer;
				cout << "\t" << whatToDo << " " << m_MinimizerPluginFile << endl;
			}

			else if (whatToDo=="MINIMIZER_PLUGIN_CLASS") {
				AnalysisConfigFile >> DataBuffer;
				m_MinimizerPluginClass  = DataBuffer;
				cout << "\t" << whatToDo << " " << m_MinimizerPluginClass << endl;
			}

			else if (whatToDo=="DO_MINIMIZATION") {
				AnalysisConfigFile >> DataBuffer;
				m_DoMinimization = DataBuffer == "true" ? true :
					DataBuffer == "false" ? false : atoi(DataBuffer.c_str());
				cout << "\t" << whatToDo << " " << m_DoMinimization << endl;
			}

			else if (whatToDo=="MINIMIZER_NAME") {
				AnalysisConfigFile >> DataBuffer;
				m_MinimizerName  = DataBuffer;
				cout << "\t" << whatToDo << " " << m_MinimizerName << endl;
			}

			else if (whatToDo=="MINIMIZER_ALGORITHM") {
				AnalysisConfigFile >> DataBuffer;
				m_MinimizerAlgorithm  = DataBuffer;
				cout << "\t" << whatToDo << " " << m_MinimizerAlgorithm << endl;
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

	// Read analysis config file & initialize relavant variables
  ReadAnalysisConfig();
	
	m_Particle = new NPL::Nucleus(m_ParticleZ, m_ParticleA);

	if(DoMinimization()) {
		gROOT->ProcessLine(Form(".L %s+", m_MinimizerPluginFile.c_str()));
		m_MinimizerFunction = reinterpret_cast<TMDMPhysicsMinimizer*>(
			gROOT->ProcessLineFast( 
				Form("new %s((TMDMPhysics*)%p);",
						 m_MinimizerPluginClass.c_str(),
						 this)
				));
		if(!m_MinimizerFunction) {
			cerr << "ERROR: Invalid minimizer plugin file or class name.\n" <<
				"\tFile: "  << m_MinimizerPluginFile << "\n" <<
				"\tClass: " << m_MinimizerPluginClass << "\n";
			exit(1);
		}
	}
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
	// Note:: All inputs/outputs in RADIANS. Converted to degrees here
	// as required by MDMTrace, and then back to radians for the outputs
	//
	m_Trace->SetScatteredMass(GetParticle()->Mass()/amu_c2); // AMU
	m_Trace->SetScatteredCharge(GetParticleQ());
	m_Trace->SetScatteredAngle(thetaX/deg, thetaY/deg); // deg (converted)
	m_Trace->SetScatteredEnergy(Ekin/MeV); // MeV
			
	m_Trace->SendRay();
	m_Trace-> // angles in degrees here
		GetOxfordWirePositions(Fit_AngleX,Fit_Xpos[0],Fit_Xpos[1],Fit_Xpos[2],Fit_Xpos[3],
													 Fit_AngleY,Fit_Ypos[0],Fit_Ypos[1],Fit_Ypos[2],Fit_Ypos[3]);
	// convert to radians
	Fit_AngleX = Fit_AngleX*deg;
	Fit_AngleY = Fit_AngleY*deg;
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


void TMDMPhysics::MinimizeTarget(){
	//
	// check if we do the minimization
	if(DoMinimization() == false) { 
		return; 
	}
	// Set up minimizer
	std::unique_ptr<ROOT::Math::Minimizer> min(
		ROOT::Math::Factory::CreateMinimizer(
			m_MinimizerName, m_MinimizerAlgorithm.c_str()
			)
		);
	InitializeMinimizerWithDefaults(*min);
	min->SetFunction(*m_MinimizerFunction);

	// Set Initial parameters
	// Note: everything in RADIANS, conversion to deg for MDMTrace
	// happens in SendRay()
	int ivar = 0;
	m_MinimizerFunction->Initialize();
	if(m_MinimizerFunction->GetFixedThetaX() == false) {
		min->SetVariable(ivar++, "thetax", m_MinimizerFunction->GetInitialThetaX(), 0.01);
	}
	if(m_MinimizerFunction->GetFixedThetaY() == false) {
		min->SetVariable(ivar++, "thetay", m_MinimizerFunction->GetInitialThetaY(), 0.01);
	}
	if(m_MinimizerFunction->GetFixedEkin() == false) {
		min->SetVariable(ivar++, "ekin",   m_MinimizerFunction->GetInitialEkin(),   0.01);
	}

	// Do minimization
	min->Minimize();

	// Set outputs
	// Output angles are in radians
	// If using the initial angle, it's radians already
	// If using the minimized angle, it's 
	ivar = 0;
	if(m_MinimizerFunction->GetFixedThetaX()) {
		Target_Xang = m_MinimizerFunction->GetInitialThetaX();
	}	else {
		Target_Xang = min->X()[ivar++];
	}
	if(m_MinimizerFunction->GetFixedThetaY()) {
		Target_Yang = m_MinimizerFunction->GetInitialThetaY();
	}	else {
		Target_Yang = min->X()[ivar++];
	}
	if(m_MinimizerFunction->GetFixedEkin()) {
		Target_Ekin = m_MinimizerFunction->GetInitialEkin();
	}	else {
		Target_Ekin = min->X()[ivar++];
	}

	Fit_Chi2 = m_MinimizerFunction->operator()(min->X());
}


void TMDMPhysics::InitializeMinimizerWithDefaults(ROOT::Math::Minimizer& min){
	min.SetMaxFunctionCalls(1000);
	min.SetMaxIterations(1000);
	min.SetTolerance(0.001);
}

double TMDMPhysics::CalculateCentralEnergy(){
	double brho = (m_Field/tesla)*1.6; // tesla*meter
	m_Particle->SetBrho(brho);
	m_Particle->BrhoToEnergy(m_ParticleQ); // charge state
	return m_Particle->GetEnergy()/MeV;
}
