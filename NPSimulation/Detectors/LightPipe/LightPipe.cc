/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : July 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  LightPipe simulation                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
#include <fstream>
#include <algorithm>
//G4 Geometry object
#include "G4Tubs.hh"
#include "G4Box.hh"

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4Material.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

// ROOT
#include "TSystem.h"
#include "TMath.h"

// NPTool header
#include "LightPipe.hh"
#include "CalorimeterScorers.hh"
#include "PhotoDiodeScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
#include "NPSHitsMap.hh"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// LightPipe Specific Method
LightPipe::LightPipe(){
  m_Event = new TLightPipeData() ;
  m_LightPipeScorer = 0;

  // RGB Color + Transparency
  m_VisSquare = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));
	m_VisPipe   = new G4VisAttributes(G4Colour(0, 1, 0, 0.5));
	m_VisPD     = new G4VisAttributes(G4Colour(0.1, 0.2, 0.3));
	m_ScintillatorMaterial = CreateScintillatorMaterial();
	m_PipeMaterial = CreatePipeMaterial();
	//m_Wrapping = CreateWrappingMaterial();
	m_ReflectiveSurface = CreateReflectiveSurface();

	m_VisSquare->SetForceWireframe(true);
	m_VisPipe->SetForceWireframe(true);
}

LightPipe::~LightPipe(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void LightPipe::AddDetector(G4int nrow, G4int ncol, G4int nlayer, G4double width, G4double thickness, G4double pipe_width, G4double pipe_thickness){
	m_Detector.emplace_back(make_tuple(nrow,ncol,nlayer,width,thickness,pipe_width,pipe_thickness));
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void LightPipe::ReadConfiguration(NPL::InputParser parser){

	G4double width = 10*mm;
	G4double thickness = 10*mm;
	G4double pipe_width = 3*mm;
	G4double pipe_thickness = 1*mm;
	G4int nrow   = 1;
	G4int ncol   = 1;
	G4int nlayer = 1;

  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("LightPipe");
  if(true || NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 
	vector<string> lp   = {"WIDTH","THICKNESS","PIPE_WIDTH","PIPE_THICKNESS","NROW","NCOL","NLAYER"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
		if(blocks[i]->HasTokenList(lp)){
      if(true || NPOptionManager::getInstance()->GetVerboseLevel()){
        cout << endl << "////  LightPipe " << i+1 <<  endl;
			}
			width = blocks[i]->GetDouble("WIDTH", "mm");
			thickness = blocks[i]->GetDouble("THICKNESS", "mm");
			pipe_width = blocks[i]->GetDouble("PIPE_WIDTH", "mm");
			pipe_thickness = blocks[i]->GetDouble("PIPE_THICKNESS", "mm");
			nrow = blocks[i]->GetInt("NROW");
			ncol = blocks[i]->GetInt("NCOL");
			nlayer = blocks[i]->GetInt("NLAYER");
    }
    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
	//
	AddDetector(nrow, ncol, nlayer, width, thickness, pipe_width, pipe_thickness);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void LightPipe::ConstructDetector(G4LogicalVolume* world){
	bool warnOverlap = false;


	//Create experimental hall
	G4Material* Vacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum") ;
	G4double expHall_x = 10.*m;
	G4double expHall_y = 10.*m;
	G4double expHall_z = 10.*m;
    
	G4Box* fExperimentalHall_box = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
	G4LogicalVolume* fExperimentalHall_log = new G4LogicalVolume(fExperimentalHall_box,
																															 Vacuum,"expHall_log",0,0,0);
	G4VPhysicalVolume* fExperimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
																																fExperimentalHall_log,"expHall",0,false,0);
    
	fExperimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);
	
	auto BuildRectangle = [this](G4double width, G4double length, G4double thickness, G4Material* material){
    G4Box* box = new G4Box("LightPipe_Box",width*0.5,
													 length*0.5,thickness*0.5);
    G4LogicalVolume* Detector = new G4LogicalVolume(box,material,"logic_LightPipe_Box",0,0,0);
    Detector->SetVisAttributes(this->m_VisSquare);
//    Detector->SetSensitiveDetector(this->m_LightPipeScorer);
		return Detector;
	};
	auto getCenter = [&](int i, int imax, double width){
		return (i - (imax/2.))*width + width/2.;
	};
	
	//int i=0, j=0, k=0;

	int iPipeX=1,/* iPipeY=1,*/ iDet=1;
	for(const auto& det : m_Detector) {
		const G4int& nrow = get<0>(det);
		const G4int& ncol = get<1>(det);
		//const G4int& nlayer = get<2>(det);
		const G4double& width = get<3>(det);
		const G4double& thickness = get<4>(det);
		const G4double& pipe_width = get<5>(det);
		const G4double& pipe_thickness = get<6>(det);
		//const G4double  pd_thickness = 1*mm;

		vector<vector<G4PVPlacement*> > physVol(nrow);
		for(auto& v : physVol) { v.resize(ncol); }


		auto buildRow = [&](G4int irow, G4double z){
			//G4double rowWidthX = nrow*width;
			G4double pipe_length = width*ncol + 1*cm;
			//
			// Build light pipe above detectors
			//
			// Create geometric object
			G4ThreeVector pipePos(
				0, getCenter(irow, nrow, width) + width/2. + pipe_thickness/2., z);
			auto pipe = BuildRectangle(
				pipe_length, pipe_width, pipe_thickness, m_PipeMaterial);
			pipe->SetVisAttributes(m_VisPipe);
			// Rotate it
			G4RotationMatrix* myRotation = new G4RotationMatrix();
			myRotation->rotateX(90.*deg);
			// Create PV Placement
		  new G4PVPlacement(
				myRotation, pipePos, pipe, "LightPipe_PipeX", world, false, iPipeX++, warnOverlap);

			std::vector<G4PVPlacement*> pvRow;
			for(int icol=0; icol< ncol; ++icol){
				//
				// Build row of detectors
				//
				//
				// Create geometric object
				G4ThreeVector Det_pos
					(getCenter(icol,ncol,width), getCenter(irow,nrow,width), z);
				auto Scintillator = BuildRectangle(width, width, thickness, m_ScintillatorMaterial);
//				Scintillator->SetSensitiveDetector(this->m_LightPipeScorer);
				// Create PV placement
				pvRow.push_back(
					new G4PVPlacement(
						0, Det_pos, Scintillator, "LightPipe_Detector", world, false, iDet++, warnOverlap) );
			}
			// Create reflective surfaces between detectors
			for(int icol=0; icol< ncol; ++icol){
				// to the left
				if(icol != 0) {
					new G4LogicalBorderSurface("CrystalSurface", pvRow.at(icol-1), pvRow.at(icol), m_ReflectiveSurface);
				} else {
					new G4LogicalBorderSurface("CrystalSurface", fExperimentalHall_phys, pvRow.at(icol), m_ReflectiveSurface);
				}
				// to the right
				if(icol != ncol-1) {
					new G4LogicalBorderSurface("CrystalSurface", pvRow.at(icol), pvRow.at(icol+1), m_ReflectiveSurface);
				} else {
				new G4LogicalBorderSurface("CrystalSurface", pvRow.at(icol), fExperimentalHall_phys,  m_ReflectiveSurface);
				}
			}
			return pvRow;
		};

		buildRow(5,0);
#if 0
		
		int pdNum = 1;
		for(int ilayer = 0; ilayer< nlayer; ++ilayer){
			G4double detZ = getCenter(ilayer,nlayer,thickness) + pipe_thickness*ilayer;
			for(int irow = 0; irow< nrow; ++irow){
				for(int icol = 0; icol< ncol; ++icol){
					//
					// Build detectors
					G4ThreeVector Det_pos
						(getCenter(irow,nrow,width),
						 getCenter(icol,ncol,width),
						 detZ);
					auto Rect = BuildRectangle(width, width, thickness, m_ScintillatorMaterial);
					auto pvScint = new G4PVPlacement(0, Det_pos, Rect,"LightPipe_Detector",world,false,i+1,warnOverlap);
					physVol.at(irow).at(icol) = pvScint;
					++i;
				} // for(icol...)
			} // for(irow...)
			//
			// Reflection between detectors
			for(int irow = -1; irow< nrow; ++irow){
				for(int icol = -1; icol< ncol; ++icol){
					if(icol > -1) {
						auto pvLeft = irow != -1 ? physVol.at(irow).at(icol) : fExperimentalHall_phys;
						auto pvRight = irow != nrow-1 ? physVol.at(irow+1).at(icol) : fExperimentalHall_phys;
						new G4LogicalBorderSurface("CrystalSurface", pvLeft, pvRight, m_ReflectiveSurface);
						new G4LogicalBorderSurface("CrystalSurface", pvRight, pvLeft, m_ReflectiveSurface);
					}
					if(irow > -1) {
						auto pvUp = icol != -1 ? physVol.at(irow).at(icol) : fExperimentalHall_phys;
						auto pvDn = icol != ncol - 1 ? physVol.at(irow).at(icol+1) : fExperimentalHall_phys;
						new G4LogicalBorderSurface("CrystalSurface", pvUp, pvDn, m_ReflectiveSurface);
						new G4LogicalBorderSurface("CrystalSurface", pvDn, pvUp, m_ReflectiveSurface);
					}
					//new G4LogicalSkinSurface("m_ReflectiveSurface",logicCsI,m_ReflectiveSurface);
				}
			}
			//
			// Build Pipes		
			auto buildPipe = [&](bool horizontal, G4double layer_center) {
				G4PVPlacement* pvLast = 0;
				G4double pipe_center_z = layer_center + thickness*0.5 + pipe_thickness*0.5;

				int ircmax = horizontal ? ncol : nrow;
				for(int irc=0; irc< ircmax; ++irc){
					G4double pipe_center_xy = getCenter(irc,ircmax,width);
					G4ThreeVector LG_pos = horizontal ?
						G4ThreeVector(0,pipe_center_xy-thickness/2.,pipe_center_z-thickness/2.) :
						G4ThreeVector(pipe_center_xy,0,pipe_center_z);
					G4Material* mat = m_PipeMaterial;

					G4double pipe_length = horizontal ? width*nrow : width*ncol;
					pipe_length += 1*cm;
					auto pipe =  horizontal ?
						BuildRectangle(pipe_length, pipe_width, pipe_thickness, mat) :
						BuildRectangle(pipe_width, pipe_length, pipe_thickness, mat) ;
					pipe->SetVisAttributes(m_VisPipe);
					G4PVPlacement* pv = 0;
					if(horizontal){
						G4RotationMatrix* myRotation = new G4RotationMatrix();
						myRotation->rotateX(90.*deg);
						pv = new G4PVPlacement(myRotation, LG_pos, pipe, "LightPipe_PipeX",world,false, j+1, warnOverlap);
						++j;
					} else {
						pv = new G4PVPlacement(0, LG_pos, pipe, "LightPipe_PipeY",world,false, k+1, warnOverlap);
						++k;
					}
					new G4LogicalBorderSurface("CrystalSurface", pv, fExperimentalHall_phys, m_ReflectiveSurface);
					if(pvLast) {
						new G4LogicalBorderSurface("CrystalSurface", pvLast, pv, m_ReflectiveSurface);
						new G4LogicalBorderSurface("CrystalSurface", pv, pvLast, m_ReflectiveSurface);
					}
					pvLast = pv;
#if 0
					int icrmax = horizontal ? nrow : ncol;
					for(int icr = 0; icr< icrmax; ++icr) {
						auto pvDet = horizontal ? physVol.at(icr).at(irc) : physVol.at(irc).at(icr);
						new G4LogicalBorderSurface("CrystalSurface", pv, pvDet, m_ReflectiveSurface);
					}
#endif
#if 0
					//
					// Photodiode
					G4String NamePD = "PhotoDiode";
					G4Material* PDMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
					G4Box* solidPhotoDiode = horizontal ?
						new G4Box(NamePD, 0.5*pipe_thickness, 0.5*pipe_width, 0.5*pd_thickness) :
						new G4Box(NamePD, 0.5*pipe_width, 0.5*pipe_thickness, 0.5*pd_thickness) ;
						
					G4LogicalVolume* logicPD = new G4LogicalVolume(solidPhotoDiode, PDMaterial, NamePD,0,0,0);
					logicPD->SetSensitiveDetector(m_PDScorer);
					logicPD->SetVisAttributes(m_VisPD);

					G4RotationMatrix* myRotation = new G4RotationMatrix();
					if(horizontal) { myRotation->rotateY(90.*deg); }
					else           { myRotation->rotateX(90.*deg); }
					//
					// RIGHT / TOP
					G4int side = horizontal ? TLightPipeData::Right_t : TLightPipeData::Top_t;
					G4ThreeVector PD_Pos = horizontal ?
						G4ThreeVector(pipe_length*0.5 + 0.5*pd_thickness, pipe_center_xy, pipe_center_z) :
						G4ThreeVector(pipe_center_xy, pipe_length*0.5 + 0.5*pd_thickness, pipe_center_z) ;
					new G4PVPlacement(myRotation, PD_Pos, logicPD, NamePD, world, false, pdNum, warnOverlap);
					m_DetectorMap[pdNum++] = make_tuple(side, ilayer, irc);
					//
					// LEFT / BOTTOM
					side = horizontal ? TLightPipeData::Left_t : TLightPipeData::Bottom_t;
					if(horizontal) { PD_Pos.set(-PD_Pos.x(),+PD_Pos.y(),PD_Pos.z()); }
					else           { PD_Pos.set(+PD_Pos.x(),-PD_Pos.y(),PD_Pos.z()); }
					new G4PVPlacement(myRotation, PD_Pos, logicPD, NamePD, world, false, pdNum, warnOverlap);
					m_DetectorMap[pdNum++] = make_tuple(side, ilayer, irc);
#endif
				}
			};
			//
			if(!(ilayer%2)) { // even
				// even layers get vertical light pipes downstream
				buildPipe(false, detZ);
			}
			buildPipe(true, detZ);
		} // for(ilayer)
#endif
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void LightPipe::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("LightPipe")){
    pTree->Branch("LightPipe", "TLightPipeData", &m_Event) ;
  }
  pTree->SetBranchAddress("LightPipe", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void LightPipe::ReadSensitive(const G4Event* event){
  m_Event->Clear();
	return;
  ///////////
  // Calorimeter scorer
  NPS::HitsMap<G4double*>* CaloHitMap;
  std::map<G4int, G4double**>::iterator Calo_itr;

  G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("LightPipeScorer/Calorimeter");
  CaloHitMap = (NPS::HitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));

  // Loop on the Calo map
#if 0
  for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){

    G4double* Info = *(Calo_itr->second);
    double Energy = RandGauss::shoot(Info[0],LightPipe_NS::ResoEnergy);
    if(Energy>LightPipe_NS::EnergyThreshold){
      //double Time = RandGauss::shoot(Info[1],LightPipe_NS::ResoTime);
      int DetectorNbr = (int) Info[2];
      m_Event->SetEnergy(0,0,DetectorNbr,Energy);
    }
  }
#endif
  // clear map for next event
  CaloHitMap->clear();

	// PhotoDiode //
	NPS::HitsMap<G4double*>* PhotoDiodeHitMap;
	std::map<G4int, G4double**>::iterator PhotoDiode_itr;

	G4int PhotoDiodeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("PDScorer/PhotoDiode");
	PhotoDiodeHitMap = (NPS::HitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(PhotoDiodeCollectionID));

	// Loop on the PhotoDiode map
	map<int, int> NumberOfOpticalPhoton; // <det no, # photons>
	for(const auto& hit : *PhotoDiodeHitMap->GetMap()) {
		G4double* Info = *(hit.second);
		G4int detectorNumber = Info[7];
		if(NumberOfOpticalPhoton.find(detectorNumber) == NumberOfOpticalPhoton.end()){
			NumberOfOpticalPhoton[detectorNumber] = 0;
		}
		NumberOfOpticalPhoton[detectorNumber] += Info[8];
	}

	for(const auto& pd : NumberOfOpticalPhoton) {
		int det = pd.first; // detector number
		int numPhoton = pd.second;
		auto detMap = m_DetectorMap.find(det);
		if(detMap != m_DetectorMap.end()){
			const auto& side = get<0>(detMap->second);
			const auto& layer = get<1>(detMap->second);
			const auto& row = get<2>(detMap->second);
			m_Event->SetEnergy(side, layer, row, numPhoton);
		} else {
			std::cerr << "WARNING:: Detector number encountered without map! The number is: " << det << "\nSkipping event...\n";
		}
	}

	PhotoDiodeHitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void LightPipe::InitializeScorers() {
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false;
	vector<G4int> NestingLevel;
	NestingLevel.push_back(0);
	NestingLevel.push_back(1);
  m_LightPipeScorer = CheckScorer("LightPipeScorer",already_exist) ;
	m_PDScorer = CheckScorer("PDScorer",already_exist) ;

  if(already_exist) {
    return ;
	}

  // Otherwise the scorer is initialised
  vector<int> level; level.push_back(0);
  G4VPrimitiveScorer* Calorimeter= new CalorimeterScorers::PS_Calorimeter("Calorimeter",NestingLevel, 0) ;
  //and register it to the multifunctionnal detector
  m_LightPipeScorer->RegisterPrimitive(Calorimeter);

	G4VPrimitiveScorer* PDScorer =
		new PHOTODIODESCORERS::PS_PhotoDiode_Rectangle
		("PhotoDiode",0,GetPipeWidth(0),GetPipeWidth(0),1,1);
	m_PDScorer->RegisterPrimitive(PDScorer);
	
  G4SDManager::GetSDMpointer()->AddNewDetector(m_LightPipeScorer) ;
	G4SDManager::GetSDMpointer()->AddNewDetector(m_PDScorer) ;
}
        
G4Material* LightPipe::CreateScintillatorMaterial() const {
	// p-Terphenyl
	// Taken from Proteus, Inc. specs
	// See here: http://people.physics.tamu.edu/christian/files/p-terphenyl.png
	//
	G4double specificGravity = 1.23; // from proteus
	G4double densityReference = 0.999972*g/cm3; // water @4-deg C [ the standard ]
	G4double density = specificGravity*densityReference;
	
	G4Material* material = new G4Material("p_Terphenyl_Scint", density, 2);
	material->AddElement(MaterialManager::getInstance()->GetElementFromLibrary("H"), 10);
	material->AddElement(MaterialManager::getInstance()->GetElementFromLibrary("C"), 14);
	
	// Adding Scintillation property:
	vector<double> energy, scint, fast, slow, rindx, atten;
	{
		// Read emission spectrum from datfile (+ add constant parameters)
		ifstream ifs(gSystem->ExpandPathName("$NPTOOL/NPSimulation/Detectors/LightPipe/p-terphenyl_emission.dat"));
		if(!ifs.good()){
			std::cerr << "ERROR: Couldn't open file: \"$NPTOOL/NPSimulation/Detectors/LightPipe/p-terphenyl_emission.dat\"\n";
			exit(1);
		}
		double wl, pr;
		while(ifs >> wl >> pr) {
			energy.emplace_back( h_Planck*c_light / (wl*nm) ); // convert to energy
			scint.emplace_back(pr);   // scintillation probability
			rindx.emplace_back(1.65); // refractive index
			fast.emplace_back(3*ns);     // FAST component ???
			slow.emplace_back(100*ns);   // SLOW component ???
			atten.emplace_back(4.73*mm); // Attenuation length (from https://arxiv.org/pdf/1305.0442.pdf)
		}
	}

	// Set Material Properties
	G4int numPoints = energy.size();
	G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
	MPT->AddConstProperty("SCINTILLATIONYIELD", 27000/MeV); // from proteus
	MPT->AddProperty("SCINTILLATION", &energy[0], &scint[0], numPoints);
	MPT->AddProperty("RINDEX", &energy[0], &rindx[0], numPoints) ;
	MPT->AddProperty("ABSLENGTH", &energy[0], &atten[0], numPoints);
	MPT->AddProperty("FASTCOMPONENT", &energy[0], &fast[0], numPoints);
	MPT->AddProperty("SLOWCOMPONENT", &energy[0], &slow[0], numPoints);
	MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
	MPT->AddConstProperty("FASTTIMECONSTANT", 20*ns); // ?????
	MPT->AddConstProperty("SLOWTIMECONSTANT", 100*ns); // ?????
	MPT->AddConstProperty("YIELDRATIO",1.0); // ?????
	material->SetMaterialPropertiesTable(MPT);
	return material;
}

G4Material* LightPipe::CreatePipeMaterial() const {
	// Bicron BC-482A
	// https://www.crystals.saint-gobain.com/sites/imdf.crystals.com/files/documents/bc482a-bc484-data-sheet.pdf
	// https://www.crystals.saint-gobain.com/sites/imdf.crystals.com/files/documents/organics-plastic-scintillators.pdf
	//
	G4double density = 1.03*g/cm3;
	
	G4Material* material = new G4Material("BC482A_WLS", density, 2);
	material->AddElement(MaterialManager::getInstance()->GetElementFromLibrary("H"), 10); // H:C ratio - 1.110
	material->AddElement(MaterialManager::getInstance()->GetElementFromLibrary("C"), 9);
	
	// Adding WLS property
	auto readDatfile = [](const char* fname, vector<double>& energy, vector<double>& abs, vector<double>& emit){
		ifstream ifs(gSystem->ExpandPathName(fname));
		if(!ifs.good()) {
			std::cerr << "ERROR: no file: \"" << fname << "\"\n";
			exit(1);
		}
		// skip header
		std::string dummy;
		std::getline(ifs, dummy);

		double wl, pr_a, pr_e;
		while(ifs >> wl >> pr_a >> pr_e) {
			energy.emplace_back( h_Planck*c_light / (wl*nm) ); // convert to energy
			abs.emplace_back(pr_a);   // absorption probability
			emit.emplace_back(pr_e);  // emission probability
		}
		// SORT IN ORDER OF INCREASING ENERGY
		vector<int> isort(energy.size());
		vector<double> e0 = energy, pa0 = abs, pe0 = emit;
		TMath::Sort((int)energy.size(), &energy[0], &isort[0]);
		for(size_t i=0; i< energy.size(); ++i){
			energy.at(i) = e0.at(isort.at(i));
			abs.at(i) = pa0.at(isort.at(i));
			emit.at(i) = pe0.at(isort.at(i));
		}
	};
	// Absorption & Emission
	vector<double> energy, p_abs, p_emit;
	readDatfile("$NPTOOL/NPSimulation/Detectors/LightPipe/BC482A_properties.dat", energy, p_abs, p_emit);
	//
	// Absorption is given as a probability, but GEANT4 needs a length
	// Invert and set the minimum to 0.4336 mm, which is the (measured)
	// attenuation length for EJ-280 @peak absorption.
	// This is not exact, but it's close.
	// For absorption of 0, set attenuation length very long (5 m)
	for(auto&& p : p_abs) {
		p = p > 0 ? (0.4336*mm) / p : 5*m;
	}

	//
	const size_t numPoints = energy.size();
	vector<double> rindx(numPoints, 1.59);
	vector<double> abslength(numPoints, 400*cm); // BULK attenuation length
	
	// Set Material Properties
	G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
	MPT->AddProperty("RINDEX", &energy[0], &rindx[0], numPoints);
	MPT->AddProperty("ABSLENGTH", &energy[0], &abslength[0], numPoints);
	MPT->AddProperty("WLSABSLENGTH", &energy[0], &p_abs[0], numPoints);
	MPT->AddProperty("WLSCOMPONENT", &energy[0], &p_emit[0], numPoints);
	MPT->AddConstProperty("WLSTIMECONSTANT", 12.*ns);
	MPT->AddConstProperty("WLSMEANNUMBEROFPHOTONS", 0.86);

	material->SetMaterialPropertiesTable(MPT);
	return material;
}

G4Material* LightPipe::CreateWrappingMaterial() const {
	// Teflon (C2F4) -- for now
	//
	G4double density = 2.2*g/cm3;
	
	G4Material* material = new G4Material("TEFLON", density, 2);
	material->AddElement(MaterialManager::getInstance()->GetElementFromLibrary("F"), 4); // H:C ratio - 1.110
	material->AddElement(MaterialManager::getInstance()->GetElementFromLibrary("C"), 2);
	return material;
}

G4OpticalSurface* LightPipe::CreateReflectiveSurface() const {
	G4OpticalSurface* OpticalCrystalSurface = new G4OpticalSurface("CrystalSurface");
	OpticalCrystalSurface->SetType(dielectric_metal);
	//polished: smooth perfectly polished surcface
	//ground: rough surface
	OpticalCrystalSurface->SetFinish(polished);
	//unified
	//glisur
	OpticalCrystalSurface->SetModel(glisur);
            
	G4double pp[] = {0.01*eV, 10*eV};
	const G4int num = sizeof(pp)/sizeof(G4double);
	G4double reflectivity[] = {1., 1.};
	G4double efficiency[] = {1., 1.};
            
	G4MaterialPropertiesTable* OpticalCrystalSurfaceProperty = new G4MaterialPropertiesTable();
            
	OpticalCrystalSurfaceProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
	OpticalCrystalSurfaceProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
	OpticalCrystalSurface->SetMaterialPropertiesTable(OpticalCrystalSurfaceProperty);
	return OpticalCrystalSurface;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* LightPipe::Construct(){
  return  (NPS::VDetector*) new LightPipe();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
class proxy_nps_LightPipe{
public:
	proxy_nps_LightPipe(){
		NPS::DetectorFactory::getInstance()->AddToken("LightPipe","LightPipe");
		NPS::DetectorFactory::getInstance()->AddDetector("LightPipe",LightPipe::Construct);
	}
};

proxy_nps_LightPipe p_nps_LightPipe;
}
