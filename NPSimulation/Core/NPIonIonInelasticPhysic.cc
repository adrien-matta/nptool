/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address: morfouac@nscl.msu.edu *
 *                                                                           *
 * Creation Date  : August 2016                                              *
 * Last update    : August 2016                                              *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Modular Physics list calling Geant4 reference list                       *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
//
//
// In this class the models for ion-ion interactions at intermediate energies (0 - 1 GeV per nucleon)
// can be activate. This class can be used alternatively to the "binary_ion" physics list
//
// The usefullness of this class is that you can explicitally see the total inelastic sections
// activated and the models called. Moreover you can choose to activate for ions (from deuteron
// to heavier nucleus) three different and exclusive models: the Binary Light Ion cascade, the QMD
// and The Wilson.

// For hadrotherapy pouposes, where distributions of produced fragments is importante we strongly
// suggest to use Binary or QMD. The Binary model is the default and at moment, you can swith beetween models decommenting
// the line of code and recompiling

#include "NPIonIonInelasticPhysic.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// Total cross section for inelastic processes
#include "G4TripathiCrossSection.hh"
#include "G4TripathiLightCrossSection.hh"
#include "G4IonsShenCrossSection.hh"
#include "G4BGGNucleonElasticXS.hh"
#include "G4ComponentGGHadronNucleusXsc.hh"
#include "G4CrossSectionInelastic.hh"


// Elastic
#include "G4HadronElasticProcess.hh"
#include "G4ElasticHadrNucleusHE.hh"
#include "G4DiffuseElastic.hh"
#include "G4HadronElastic.hh"

// Inelastic
#include "G4ProtonInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"
#include "G4BinaryLightIonReaction.hh"
#include "G4QMDReaction.hh"
#include "G4IonInelasticProcess.hh"
#include "G4GeneralSpaceNNCrossSection.hh"

/////////////////////////////////////////////////////////////////////////////
NPIonIonInelasticPhysic::NPIonIonInelasticPhysic(const G4String& name):
G4VPhysicsConstructor(name)
{
    G4cout << G4endl
    << "A local inelastic model is activated for all ions"
    << G4endl;
}

/////////////////////////////////////////////////////////////////////////////
NPIonIonInelasticPhysic::~NPIonIonInelasticPhysic()
{}

/////////////////////////////////////////////////////////////////////////////
void NPIonIonInelasticPhysic::ConstructProcess()
{
    G4ParticleDefinition* particle = 0;
    G4ProcessManager* processManager = 0;
    
    // ****************************************************************
    // ***                Ion-Ion models definition                 ***
    // ****************************************************************
    G4QMDReaction* JQMDmodel = new G4QMDReaction();
    JQMDmodel -> SetMinEnergy(0*MeV);
    JQMDmodel -> SetMaxEnergy(10*GeV);
    
    G4BinaryLightIonReaction* ligthBinary = new G4BinaryLightIonReaction();
    ligthBinary -> SetMinEnergy(0*MeV);
    ligthBinary -> SetMaxEnergy(10*GeV);
    
    G4TripathiCrossSection* TripatiCrossSections = new G4TripathiCrossSection;
    G4TripathiLightCrossSection* TripatiLightCrossSections = new G4TripathiLightCrossSection;
    G4IonsShenCrossSection* ShenCrossSections = new G4IonsShenCrossSection;
    G4ComponentGGHadronNucleusXsc* GlauberGribovCrossSection = new G4ComponentGGHadronNucleusXsc;
    
    G4CrossSectionInelastic* GlauberGribovDataSet = new G4CrossSectionInelastic(GlauberGribovCrossSection);
    
    // ******************
    // **** Elastic ****
    // ******************
    /*particle = G4Proton::Proton();
    //G4ElasticHadrNucleusHE* hadronElasticModel = new G4ElasticHadrNucleusHE();
    //G4HadronElastic* hadronElasticModel1 = new G4HadronElastic();
    G4DiffuseElastic* hadronElasticModel2 = new G4DiffuseElastic();
    hadronElasticModel2->SetPlabLowLimit(0.1*MeV);//Default is 20 MeV
    hadronElasticModel2->SetRecoilKinEnergyLimit(10*keV);//Default is 100 keV
    hadronElasticModel2->SetLowestEnergyLimit(0.0*keV);//Default value is 0 keV
    hadronElasticModel2->SetHEModelLowLimit(0.0*keV);//Default value is 0 keV
    hadronElasticModel2->SetQModelLowLimit(0.0*keV);//Default value is 0 keV
    
    G4HadronElasticProcess* hadronElasticProcess = new G4HadronElasticProcess();
    
    hadronElasticProcess->AddDataSet(GlauberGribovCrossSection);
    hadronElasticProcess->AddDataSet( new G4BGGNucleonElasticXS(particle));
    
    //hadronElasticProcess->RegisterMe(hadronElasticModel1);
    hadronElasticProcess->RegisterMe(hadronElasticModel2);

   
    processManager = particle -> GetProcessManager();
    processManager -> AddDiscreteProcess(hadronElasticProcess);*/

    // ****************
    // **** Proton ****
    // ****************
    G4ProtonInelasticProcess* protonInelasticProcess = new G4ProtonInelasticProcess;
    
    protonInelasticProcess -> AddDataSet(ShenCrossSections);
    protonInelasticProcess -> AddDataSet(TripatiCrossSections);
    protonInelasticProcess -> AddDataSet(TripatiLightCrossSections);
    //protonInelasticProcess -> AddDataSet(GlauberGribovDataSet);
    
    protonInelasticProcess -> RegisterMe(ligthBinary);
    //protonInelasticProcess -> RegisterMe(JQMDmodel);
    //dprotonInelasticProcess -> RegisterMe(WilsonModel);
    
    particle = G4Proton::Proton();
    processManager = particle -> GetProcessManager();
    processManager -> AddDiscreteProcess(protonInelasticProcess);
    
    double energy = 0;
    for(int i=0; i<50;i++){
        energy += 5;
        G4DynamicParticle* dp = new G4DynamicParticle(particle,G4ThreeVector(0,0,1),energy*MeV);
        G4Element* element = new G4Element("Tin","Sn",50,120*g/mole);
        //G4Element* element = new G4Element("Cupper","Cu",29,59*g/mole);
        cout << "Glauber | Energy = " << energy << " | Cross Section = " << GlauberGribovCrossSection->GetInelasticGlauberGribov(dp,50,70)/barn << " barn" << endl;
        cout << GlauberGribovCrossSection->GetHNinelasticXsc(dp,element) << endl;
        cout << "Tripathi | Energy = " << energy << " | Cross Section = " << TripatiLightCrossSections->GetCrossSection(dp,element,0)/barn << " barn" << endl;
        cout << "Shen | Energy = " << energy << " | Cross Section = " << ShenCrossSections->GetCrossSection(dp,element,0)/barn << " barn" << endl;
        
    }
    
    // ****************
    // *** Deuteron ***
    // ****************
    G4DeuteronInelasticProcess* deuteronInelasticProcess = new G4DeuteronInelasticProcess;
    
    deuteronInelasticProcess -> AddDataSet(ShenCrossSections);
    deuteronInelasticProcess -> AddDataSet(TripatiCrossSections);
    deuteronInelasticProcess -> AddDataSet(TripatiLightCrossSections);
    
    deuteronInelasticProcess -> RegisterMe(ligthBinary);
    //deuteronInelasticProcess -> RegisterMe(JQMDmodel);
    //deuteronInelasticProcess -> RegisterMe(WilsonModel);
    
    particle = G4Deuteron::Deuteron();
    processManager = particle -> GetProcessManager();
    processManager -> AddDiscreteProcess(deuteronInelasticProcess);
    
    // **************
    // *** Triton ***
    // **************
    G4TritonInelasticProcess* tritonInelasticProcess = new G4TritonInelasticProcess;
    
    tritonInelasticProcess -> AddDataSet(ShenCrossSections);
    tritonInelasticProcess -> AddDataSet(TripatiCrossSections);
    tritonInelasticProcess -> AddDataSet(TripatiLightCrossSections);
    
    tritonInelasticProcess -> RegisterMe(ligthBinary);
    //tritonInelasticProcess -> RegisterMe(JQMDmodel);
    //tritonInelasticProcess -> RegisterMe(WilsonModel);
    
    particle = G4Triton::Triton();
    processManager = particle -> GetProcessManager();
    processManager -> AddDiscreteProcess(tritonInelasticProcess);
    
    // *************
    // *** Alpha ***
    // *************
    G4AlphaInelasticProcess* alphaInelasticProcess = new G4AlphaInelasticProcess;
    
    alphaInelasticProcess -> AddDataSet(ShenCrossSections);
    alphaInelasticProcess -> AddDataSet(TripatiCrossSections);
    alphaInelasticProcess -> AddDataSet(TripatiLightCrossSections);
    
    alphaInelasticProcess -> RegisterMe(ligthBinary);
    //alphaInelasticProcess -> RegisterMe(JQMDmodel);
    //alphaIonInelasticProcess -> RegisterMe(WilsonModel);
    
    particle = G4Alpha::Alpha();
    processManager = particle -> GetProcessManager();
    processManager -> AddDiscreteProcess(alphaInelasticProcess);
    
    // *******************
    // *** Generic Ion ***
    // *******************
    G4IonInelasticProcess* genericIonInelasticProcess = new G4IonInelasticProcess();
    
    genericIonInelasticProcess -> AddDataSet(ShenCrossSections);
    genericIonInelasticProcess -> AddDataSet(TripatiCrossSections);
    genericIonInelasticProcess -> AddDataSet(TripatiLightCrossSections);
    
    genericIonInelasticProcess -> RegisterMe(ligthBinary);
    //genericIonInelasticProcess -> RegisterMe(JQMDmodel);
    //genericIonInelasticProcess -> RegisterMe(WilsonModel);
    
    particle = G4GenericIon::GenericIon();
    processManager = particle -> GetProcessManager();
    processManager -> AddDiscreteProcess(genericIonInelasticProcess);
}



