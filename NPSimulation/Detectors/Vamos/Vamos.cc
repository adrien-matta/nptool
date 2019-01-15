/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Cyril Lenain  contact address: lenain@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : octobre 2018                                             *
 * Last update    : 09/01/2019                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the Vamos spectrometer                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <cmath>
#include <limits>
#include <sstream>
// G4 Geometry object
#include "G4Box.hh"
#include "G4Tubs.hh"

// G4 sensitive
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"

// G4 various object
#include "G4Colour.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4Transform3D.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

// NPTool header
#include "CalorimeterScorers.hh"
#include "DriftChamberScorers.hh"
#include "InteractionScorers.hh"
#include "MaterialManager.hh"
#include "NPOptionManager.h"
#include "NPSDetectorFactory.hh"
#include "NPSHitsMap.hh"
#include "RootOutput.h"
#include "Vamos.hh"

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace Vamos_NS {

    // Energy,time and position Resolution
    const G4double EnergyThreshold = -10 * MeV;
    const G4double ResoTime        = 4.5 * ns;
    const G4double ResoEnergy      = 0.0001 * MeV;
    const G4double ResoDriftTime   = 0.0001 * ns;
    const G4double ResoPosX        = 0.0001 * mm;

    // Drift features
    const G4double      DriftSpeed = 1 * cm / microsecond;
    const G4ThreeVector DriftDir   = G4ThreeVector(0, 1, 0);

    // Geometry
    const G4double E_DCWidth     = 600 * mm;  // Entrance_DriftChamber
    const G4double E_DCLength    = 150 * mm;
    const G4double E_DCThickness = 60 * mm;

    const G4double MagnetWidth     = 1000 * mm;
    const G4double MagnetLenght    = 150 * mm;
    const G4double MagnetThickness = 200 * mm;
    const G4double DipolThickness  = 600 * mm;

    const G4double ChamberWidth     = 1000 * mm;
    const G4double ChamberLength    = 150 * mm;
    const G4double ChamberThickness = 120 * mm;

    // Mother Volume of Vamos
    const G4double Phi                  = 0 * deg;
    const G4double VamosVolumeWidth     = 4500 * mm;
    const G4double VamosVolumeLength    = 1000 * mm;
    const G4double VamosVolumeThickness = 5000 * mm;

    // SubVolume of detection
    const G4double DetectionVolumeThickness = 1300 * mm;
    const G4double DetectionVolumeWidth     = 1000 * mm;
    const G4double DetectionVolumeLength    = 1000 * mm;
    const G4double Det_Theta                = 0 * deg;

} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Vamos Specific Method
Vamos::Vamos() {

    m_Event             = new TVamosData();
    m_CalorimeterScorer = 0;
    m_DCScorer          = 0;
    m_Quad1             = 0;
    m_Quad2             = 0;
    m_Dipol             = 0;
    m_BeamCatcher       = 0;
    m_MWPPAC            = 0;
    m_DC3               = 0;
    m_DC4               = 0;
    m_DC1               = 0;
    m_DC2               = 0;

    ICcounter = 0;

    // RGB Color + Transparency
    m_VisQuad        = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    m_VisVolumeVamos = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.1));
    m_VisDC          = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
    m_VisCatcher     = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
    m_VisGasC4H10    = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
    m_VisGasCF4      = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.2));
}

Vamos::~Vamos() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace Vamos_NS;

void Vamos::AddVamos(G4double R, double Theta) {
    m_R     = R;
    m_Theta = Theta;
}

void Vamos::AddBeamCatcher(string Material, G4double Width, double Length,
        double Thickness, G4ThreeVector Pos) {
    CatcherMaterial  = Material;
    CatcherWidth     = Width;
    CatcherLength    = Length;
    CatcherThickness = Thickness;
    R_Catcher        = Pos[2];
    Pos[2]           = -DetectionVolumeThickness * 0.5 + CatcherThickness * 0.5;
    m_PosCatcher     = Pos;
}

// To add DriftChambers and the MWPPAC
void Vamos::AddDetector(G4double Z, string Gas, double Pressure,
        double Temperature) {
    m_Z.push_back(Z);
    m_Gas.push_back(Gas);
    m_Pressure.push_back(Pressure);
    m_Temperature.push_back(Temperature);
}

void Vamos::AddIC(G4double Z, double Thickness, string Gas, double Pressure,
        double Temperature) {
    m_ZIC.push_back(Z);
    m_ThicknessIC.push_back(Thickness);
    m_GasIC.push_back(Gas);
    m_PressureIC.push_back(Pressure);
    m_TemperatureIC.push_back(Temperature);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// The two entry DriftChambers 
G4LogicalVolume* Vamos::BuildDC1() {
    if (!m_DC1) {
        G4Box* box = new G4Box("Vamos_DC1", E_DCWidth * 0.5, E_DCLength * 0.5,
                E_DCThickness * 0.5);

        G4Material* DetectorMaterial
            = MaterialManager::getInstance()->GetGasFromLibrary(
                    m_Gas[0], m_Pressure[0], m_Temperature[0]);

        m_DC1 = new G4LogicalVolume(box, DetectorMaterial, "logic_Vamos_DC1", 0, 0,
                0);
        m_DC1->SetVisAttributes(m_VisGasC4H10);
        m_DC1->SetSensitiveDetector(m_DCScorer);
    }
    return m_DC1;
}

G4LogicalVolume* Vamos::BuildDC2() {
    if (!m_DC2) {
        G4Box* box = new G4Box("Vamos_DC2", E_DCWidth * 0.5, E_DCLength * 0.5,
                E_DCThickness * 0.5);

        G4Material* DetectorMaterial
            = MaterialManager::getInstance()->GetGasFromLibrary(
                    m_Gas[1], m_Pressure[1], m_Temperature[1]);

        m_DC2 = new G4LogicalVolume(box, DetectorMaterial, "logic_Vamos_DC2", 0, 0,
                0);
        m_DC2->SetVisAttributes(m_VisGasC4H10);
        m_DC2->SetSensitiveDetector(m_DCScorer);
    }
    return m_DC2;
}

// Quadruples and Dipole just to make the visualisation nice
G4LogicalVolume* Vamos::BuildQuad1() {
    if (!m_Quad1) {
        G4Box* box = new G4Box("Vamos_Box", Vamos_NS::MagnetWidth * 0.5,
                Vamos_NS::MagnetLenght * 0.5,
                Vamos_NS::MagnetThickness * 0.5);

        G4Material* VamosMaterial
            = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
        m_Quad1 = new G4LogicalVolume(box, VamosMaterial, "logic_Quad1", 0, 0, 0);
        m_Quad1->SetVisAttributes(m_VisQuad);
    }
    return m_Quad1;
}

G4LogicalVolume* Vamos::BuildQuad2() {
    if (!m_Quad2) {
        G4Box* box = new G4Box("Vamos_Quad2", Vamos_NS::MagnetWidth * 0.5,
                Vamos_NS::MagnetLenght * 0.5,
                Vamos_NS::MagnetThickness * 0.5);

        G4Material* VamosMaterial
            = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
        m_Quad1 = new G4LogicalVolume(box, VamosMaterial, "logic_Quad1", 0, 0, 0);
        m_Quad1->SetVisAttributes(m_VisQuad);
    }
    return m_Quad1;
}

G4LogicalVolume* Vamos::BuildDipol() {
    if (!m_Dipol) {
        G4Box* box = new G4Box("Vamos_Dipol", Vamos_NS::MagnetWidth * 0.5,
                Vamos_NS::MagnetLenght * 0.5,
                Vamos_NS::DipolThickness * 0.5);

        G4Material* VamosMaterial
            = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
        m_Dipol = new G4LogicalVolume(box, VamosMaterial, "logic_Dipol", 0, 0, 0);
        m_Dipol->SetVisAttributes(m_VisQuad);
    }
    return m_Dipol;
}

// Detection at the end of Vamos
G4LogicalVolume* Vamos::BuildBeamCatcher() {
    if (!m_BeamCatcher) {
        G4Box* box = new G4Box("Vamos_Catcher", CatcherWidth * 0.5,
                CatcherLength * 0.5, CatcherThickness * 0.5);

        G4Material* Material
            = MaterialManager::getInstance()->GetMaterialFromLibrary(
                    CatcherMaterial);
        m_BeamCatcher
            = new G4LogicalVolume(box, Material, "logic_Vamos_Catcher", 0, 0, 0);
        m_BeamCatcher->SetVisAttributes(m_VisCatcher);
        m_BeamCatcher->SetSensitiveDetector(m_CalorimeterScorer);
    }
    return m_BeamCatcher;
}

G4LogicalVolume* Vamos::BuildMWPPAC() {
    if (!m_MWPPAC) {
        G4Box* box = new G4Box("Vamos_MWPPAC", ChamberWidth * 0.5,
                ChamberLength * 0.5, ChamberThickness * 0.5);

        G4Material* DetectorMaterial
            = MaterialManager::getInstance()->GetGasFromLibrary(
                    m_Gas[2], m_Pressure[2], m_Temperature[2]);
        m_MWPPAC = new G4LogicalVolume(box, DetectorMaterial, "logic_Vamos_MWPPAC",
                0, 0, 0);
        m_MWPPAC->SetVisAttributes(m_VisGasC4H10);
    }
    return m_MWPPAC;
}

G4LogicalVolume* Vamos::BuildDC3() {
    if (!m_DC3) {
        G4Box* box = new G4Box("Vamos_DC3", ChamberWidth * 0.5, ChamberLength * 0.5,
                ChamberThickness * 0.5);

        G4Material* DetectorMaterial
            = MaterialManager::getInstance()->GetGasFromLibrary(
                    m_Gas[3], m_Pressure[3], m_Temperature[3]);

        m_DC3 = new G4LogicalVolume(box, DetectorMaterial, "logic_Vamos_DC3", 0, 0,
                0);
        m_DC3->SetVisAttributes(m_VisGasC4H10);
        m_DC3->SetSensitiveDetector(m_DCScorer);
    }
    return m_DC3;
}

G4LogicalVolume* Vamos::BuildDC4() {
    if (!m_DC4) {
        G4Box* box = new G4Box("Vamos_DC4", ChamberWidth * 0.5, ChamberLength * 0.5,
                ChamberThickness * 0.5);

        G4Material* DetectorMaterial
            = MaterialManager::getInstance()->GetGasFromLibrary(
                    m_Gas[4], m_Pressure[4], m_Temperature[4]);

        m_DC4 = new G4LogicalVolume(box, DetectorMaterial, "logic_Vamos_DC4", 0, 0,
                0);
        m_DC4->SetVisAttributes(m_VisGasC4H10);
        m_DC4->SetSensitiveDetector(m_DCScorer);
    }
    return m_DC4;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// In anticipation of use a macro
void Vamos::ClearGeometry(){

    m_Z.clear();
    m_Gas.clear();
    m_Pressure.clear();
    m_Temperature.clear();

    m_ZIC.clear();
    m_ThicknessIC.clear();
    m_PressureIC.clear();
    m_TemperatureIC.clear();
    m_GasIC.clear();

}   

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Virtual Method of NPS:VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetectorConfiguration Method

void Vamos::ReadConfiguration(NPL::InputParser parser) {

    vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Vamos");
    if (NPOptionManager::getInstance()->GetVerboseLevel())
        cout << "//// " << blocks.size() << " detectors found " << endl;

    vector<string> TokenBeamCatcher
        = {"Material", "Width", "Length", "Thickness", "Pos"};
    vector<string> sphe        = {"R", "Theta"};
    vector<string> TokenMWPPAC = {"Z", "Gas", "Pressure", "Temperature"};
    vector<string> TokenDC     = {"Z", "Gas", "Pressure", "Temperature"};
    vector<string> TokenIC = {"Z", "Thickness", "Gas", "Pressure", "Temperature"};

    for (unsigned int i = 0; i < blocks.size(); i++) {
        if (blocks[i]->HasTokenList(sphe)) {
            if (NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "////  Vamos " << i + 1 << endl;
            G4double R     = blocks[i]->GetDouble("R", "mm");
            G4double Theta = blocks[i]->GetDouble("Theta", "deg");
            AddVamos(R, Theta);
        }

        else if (blocks[i]->GetMainValue() == "BeamCatcher"
                && blocks[i]->HasTokenList(TokenBeamCatcher)) {
            if (NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "//// BeamCatcher" << i + 1 << endl;
            string        Material  = blocks[i]->GetString("Material");
            G4double      Width     = blocks[i]->GetDouble("Width", "mm");
            G4double      Length    = blocks[i]->GetDouble("Length", "mm");
            G4double      Thickness = blocks[i]->GetDouble("Thickness", "mm");
            G4ThreeVector Pos
                = NPS::ConvertVector(blocks[i]->GetTVector3("Pos", "mm"));
            AddBeamCatcher(Material, Width, Length, Thickness, Pos);
        }

        else if (blocks[i]->GetMainValue() == "MWPPAC"
                && blocks[i]->HasTokenList(TokenMWPPAC)) {
            if (NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "//// MWPPAC" << i + 1 << endl;
            G4double Z           = blocks[i]->GetDouble("Z", "mm");
            string   Gas         = blocks[i]->GetString("Gas");
            G4double Pressure    = blocks[i]->GetDouble("Pressure", "bar");
            G4double Temperature = blocks[i]->GetDouble("Temperature", "kelvin");
            AddDetector(Z, Gas, Pressure, Temperature);
        }

        else if (blocks[i]->GetMainValue() == "DC"
                && blocks[i]->HasTokenList(TokenDC)) {
            if (NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "//// DC" << i + 1 << endl;
            G4double Z           = blocks[i]->GetDouble("Z", "mm");
            string   Gas         = blocks[i]->GetString("Gas");
            G4double Pressure    = blocks[i]->GetDouble("Pressure", "bar");
            G4double Temperature = blocks[i]->GetDouble("Temperature", "kelvin");
            AddDetector(Z, Gas, Pressure, Temperature);
        }

        else if (blocks[i]->GetMainValue() == "IC"
                && blocks[i]->HasTokenList(TokenIC)) {
            if (NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "//// IC" << ICcounter+1 << endl;
            G4double Z           = blocks[i]->GetDouble("Z", "mm");
            G4double Thickness   = blocks[i]->GetDouble("Thickness", "mm");
            string   Gas         = blocks[i]->GetString("Gas");
            G4double Pressure    = blocks[i]->GetDouble("Pressure", "bar");
            G4double Temperature = blocks[i]->GetDouble("Temperature", "kelvin");
            AddIC(Z, Thickness, Gas, Pressure, Temperature);
            ICcounter++;
        }

        else {
            cout << "ERROR: check your input file formatting " << endl;
            exit(1);
        }

    }
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Vamos::ConstructDetector(G4LogicalVolume* world) {

    // Mother Volume of Vamos
    G4double R  = m_R + VamosVolumeThickness * 0.5;

    G4double X = R * sin(m_Theta) * cos(Phi);
    G4double Y = R * sin(m_Theta) * sin(Phi);
    G4double Z = R * cos(m_Theta);
    G4ThreeVector Det_pos = G4ThreeVector(X, Y, Z);

    G4RotationMatrix* Rot1 = new G4RotationMatrix();
    Rot1->rotateY(m_Theta);

    G4Box* MotherSolid
        = new G4Box("MotherVolume", VamosVolumeWidth * 0.5,
                VamosVolumeLength * 0.5, VamosVolumeThickness * 0.5);

    G4Material* VolumeMaterial
        = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
    G4LogicalVolume* MotherVolume = new G4LogicalVolume(
            MotherSolid, VolumeMaterial, "MotherVolume", 0, 0, 0);

    new G4PVPlacement(G4Transform3D(*Rot1, Det_pos), MotherVolume, "MotherVolume",
            world, false, 0);
    MotherVolume->SetVisAttributes(m_VisVolumeVamos);

    // SubVolume of Detection at the end of Vamos
    // The position of the subvolume is defined by the position of the BeamCatcher
    G4double R2
        = R_Catcher - CatcherThickness * 0.5 + DetectionVolumeThickness * 0.5;

    G4double      X2 = R2 * sin(Det_Theta) * cos(0);
    G4double      Z2 = R2 * cos(Det_Theta);
    G4ThreeVector Det_pos2 = G4ThreeVector(X2, 0, Z2);

    G4RotationMatrix* Rot2 = new G4RotationMatrix();
    Rot2->rotateY(Det_Theta);

    G4Box* MotherDetectorSolid
        = new G4Box("MotherDetector", DetectionVolumeWidth * 0.5,
                DetectionVolumeLength * 0.5, DetectionVolumeThickness * 0.5);

    G4LogicalVolume* MotherDetector = new G4LogicalVolume(
            MotherDetectorSolid, VolumeMaterial, "MotherDetector", 0, 0, 0);

    new G4PVPlacement(G4Transform3D(*Rot2, Det_pos2), MotherDetector,
            "MotherDetector", MotherVolume, false, 0);
    MotherDetector->SetVisAttributes(m_VisVolumeVamos);


    // Position the entry DCs and the magnets in the MotherVolume
    new G4PVPlacement(0, G4ThreeVector(0, 0, -VamosVolumeThickness * 0.5 + m_Z[0]),
            BuildDC1(), "Entrance_DC1", MotherVolume, false, 1);

    new G4PVPlacement(0, G4ThreeVector(0, 0, -VamosVolumeThickness * 0.5 + m_Z[1]),
            BuildDC2(), "Entrance_DC2", MotherVolume, false, 2);

    new G4PVPlacement(
            0,
            G4ThreeVector(0, 0, (-VamosVolumeThickness + MagnetThickness) * 0.5 + 400),
            BuildQuad1(), "Vamos", MotherVolume, false, 0);

    new G4PVPlacement(
            0,
            G4ThreeVector(0, 0,
                (-VamosVolumeThickness + MagnetThickness) * 0.5 + 700 * mm),
            BuildQuad2(), "Vamos", MotherVolume, false, 0);

    new G4PVPlacement(
            0,
            G4ThreeVector(0, 0,
                (-VamosVolumeThickness + MagnetThickness) * 0.5 + 1500 * mm),
            BuildDipol(), "Vamos", MotherVolume, false, 0);

    // Position the system of detection at the end of Vamos in the sub Volume
    new G4PVPlacement(0, m_PosCatcher, BuildBeamCatcher(), "BeamCatcher",
            MotherDetector, false, 3);

    new G4PVPlacement(
            0,
            G4ThreeVector(0, 0,
                -DetectionVolumeThickness * 0.5
                + (m_Z[2] - R_Catcher + CatcherThickness * 0.5)),
            BuildMWPPAC(), "MWPPAC", MotherDetector, false, 4);

    new G4PVPlacement(
            0,
            G4ThreeVector(0, 0,
                -DetectionVolumeThickness * 0.5
                + (m_Z[3] - R_Catcher + CatcherThickness * 0.5)),
            BuildDC3(), "DC", MotherDetector, false, 5);

    new G4PVPlacement(
            0,
            G4ThreeVector(0, 0,
                -DetectionVolumeThickness * 0.5
                + (m_Z[4] - R_Catcher + CatcherThickness * 0.5)),
            BuildDC4(), "DC", MotherDetector, false, 6);

    // Construct and position the Ionisations Chambers
    for (int i = 0; i < ICcounter; i++) {
        G4Box* box = new G4Box("Vamos_IC", ChamberWidth * 0.5, ChamberLength * 0.5,
                m_ThicknessIC[i] * 0.5);
        G4Material* GasIC = MaterialManager::getInstance()->GetGasFromLibrary(
                m_GasIC[i], m_PressureIC[i], m_TemperatureIC[i]);
        G4LogicalVolume* IC
            = new G4LogicalVolume(box, GasIC, "logic_Vamos_IC", 0, 0, 0);
        IC->SetVisAttributes(m_VisGasCF4);
        IC->SetSensitiveDetector(m_CalorimeterScorer);
        new G4PVPlacement(
                0,
                G4ThreeVector(0, 0,
                    -DetectionVolumeThickness * 0.5
                    + (m_ZIC[i] - R_Catcher + CatcherThickness * 0.5)),
                IC, "IC", MotherDetector, false, i + 7);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method

void Vamos::InitializeRootOutput() {
    RootOutput* pAnalysis = RootOutput::getInstance();
    TTree*      pTree     = pAnalysis->GetTree();
    if (!pTree->FindBranch("Vamos")) {
        pTree->Branch("Vamos", "TVamosData", &m_Event);
    }
    pTree->SetBranchAddress("Vamos", &m_Event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root Three
// Called at in the EventAction::EndOfEventAvtion
void Vamos::ReadSensitive(const G4Event*) {

    m_Event->Clear();

    ///////////
    // Calorimeter scorer
    CalorimeterScorers::PS_Calorimeter* Scorer
        = (CalorimeterScorers::PS_Calorimeter*)m_CalorimeterScorer->GetPrimitive(
                0);
    unsigned int size = Scorer->GetMult();
    for (unsigned int i = 0; i < size; i++) {
        vector<unsigned int> level = Scorer->GetLevel(i);
        G4double             Energy
            = RandGauss::shoot(Scorer->GetEnergy(i), Vamos_NS::ResoEnergy);
        if (Energy > Vamos_NS::EnergyThreshold) {
            G4double Time = RandGauss::shoot(Scorer->GetTime(i), Vamos_NS::ResoTime);
            int      DetectorNbr = level[0];
            m_Event->SetEnergy(DetectorNbr, Energy);
            m_Event->SetTime(DetectorNbr, Time);
        }
    }
    ///////////
    // DriftChamber  scorer
    DriftChamberScorers::PS_DriftChamber* Scorer2
        = (DriftChamberScorers::PS_DriftChamber*)m_DCScorer->GetPrimitive(0);
    unsigned int size2 = Scorer2->GetMult();
    for (unsigned int i = 0; i < size2; i++) {
        vector<unsigned int> level     = Scorer2->GetLevel(i);
        G4double               DriftTime 
            = RandGauss::shoot(Scorer2->GetDriftTime(i)/Scorer2->GetCounter(i), Vamos_NS::ResoDriftTime);
        G4double               X         
            = RandGauss::shoot(Scorer2->GetX(i)/Scorer2->GetCounter(i), ResoPosX);
        int DetectorNbr = level[0];
        m_Event->SetDrift(DetectorNbr, DriftTime, X);
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////
void Vamos::InitializeScorers() {
    // This check is necessary in case the geometry is reloaded
    bool already_exist  = false;
    m_DCScorer          = CheckScorer("DCScorer", already_exist);
    m_CalorimeterScorer = CheckScorer("VamosScorer", already_exist);
    if (already_exist)
        return;

    // Otherwise the scorer is initialised
    vector<int> level;
    level.push_back(0);

    G4VPrimitiveScorer* Calorimeter
        = new CalorimeterScorers::PS_Calorimeter("Calorimeter", level, 0);
    m_CalorimeterScorer->RegisterPrimitive(Calorimeter);

    G4VPrimitiveScorer* Drift = new DriftChamberScorers::PS_DriftChamber(
            "Drift", level, DriftDir, DriftSpeed, 0);
    m_DCScorer->RegisterPrimitive(Drift);

    // and register it to the multifunctionnal detector
    G4SDManager::GetSDMpointer()->AddNewDetector(m_DCScorer);
    G4SDManager::GetSDMpointer()->AddNewDetector(m_CalorimeterScorer);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Vamos::Construct() { return (NPS::VDetector*)new Vamos(); }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory //
////////////////////////////////////////////////////////////////////////////////
extern "C" {
    class proxy_nps_Vamos {
        public:
            proxy_nps_Vamos() {
                NPS::DetectorFactory::getInstance()->AddToken("Vamos", "Vamos");
                NPS::DetectorFactory::getInstance()->AddDetector("Vamos", Vamos::Construct);
            }
    };

    proxy_nps_Vamos p_nps_Vamos;
}
