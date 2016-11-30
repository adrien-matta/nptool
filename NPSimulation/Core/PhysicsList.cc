/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : October 2015                                             *
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

//NPS
#include "PhysicsList.hh"

// Local physic directly implemented from the Hadronthrapy example
// Physic dedicated to the ion-ion inelastic processes
#include "NPIonIonInelasticPhysic.hh"

// G4
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"

/////////////////////////////////////////////////////////////////////////////
PhysicsList::PhysicsList() : G4VModularPhysicsList(){
    m_EmList = "Option4";
    defaultCutValue = 1*mm;//0.2*mm;
    opticalPhysicsList = NULL;
    
    ReadConfiguration("PhysicsListOption.txt");
    G4LossTableManager::Instance();
    SetVerboseLevel(0);
    
    // ******     Definition of defaults for the physics processes *****
    // ******     in case no physics is called by the macro file   *****
    //
    // The default physics corresponds to the actual QGSP_BIC_HP list
    // but with the following differences:
    // --> G4EmStandardPhysics_option4 for the electromagnetic processes
    //     is used n place of the less accurate G4EmStandardPhysics
    // --> The G4RadioactiveDecayPhysics is add
    // --> G4HadronPhysicsQGSP_BIC is used in place of G4HadronPhysicsQGSP_BIC_HP
    // --> G4HadronElasticPhysics is used in place of G4HadronElasticPhysics_HP
    
    // Elecromagnetic physics
    // Using the more accurate option4
    
    emPhysicsList=NULL;
    if(m_EmList == "Option4")
        emPhysicsList = new G4EmStandardPhysics_option4();
    
    else if(m_EmList== "Livermore")
        emPhysicsList = new G4EmLivermorePhysics();
    
    else if( m_EmList == "Penelope")
        emPhysicsList = new G4EmPenelopePhysics();
    
    else{
        std::cout << "\r\033[1;31mERROR: User given physics list " << m_EmList << " is not supported, option are Option4 Livermore Penelope\033[0m" <<std::endl;
        exit(1);
    }
    
    // Hadronic physics
    if(m_IonBinaryCascadePhysics){
        m_PhysList["IonBinaryCascadePhysics"] = new G4IonBinaryCascadePhysics();
    }
    if(m_EmExtraPhysics)
        m_PhysList["EmExtraPhysics"]=new G4EmExtraPhysics();
    
    if(m_HadronElasticPhysics){
        m_PhysList["G4HadronElasticPhysics"]=new G4HadronElasticPhysics();
        m_PhysList["G4IonElasticPhysics"]=new G4IonElasticPhysics();
    }
    
    if(m_NPIonInelasticPhysics)
        m_PhysList["NPIonInelasticPhysics"]=new NPIonIonInelasticPhysic();
    
    if(m_StoppingPhysics)
        m_PhysList["StoppingPhysics"]=new G4StoppingPhysics();
    
    if(m_HadronPhysicsINCLXX){
        m_PhysList["HadronPhysicsINCLXX"] = new G4HadronPhysicsINCLXX();
        m_PhysList["IonPhysicsINCLXX"] = new G4IonINCLXXPhysics();
    }
    
    if(m_HadronPhysicsQGSP_BIC_HP){
#if NPS_GEANT4_VERSION_MAJOR > 9
        m_PhysList["HadronPhysicsQGSP_BIC_HP"] = new G4HadronPhysicsQGSP_BIC_HP();
        
#else
        std::cout << "\r\032[1;31m Warning: physics list HadronPhysicsQGSP_BIC_HP require Geant4 10, process not activated \033[0m" <<std::endl;
#endif
    }
    // Optical Photon for scintillator simulation
    if(m_OpticalPhysics){
        opticalPhysicsList = new G4OpticalPhysics(0);
        opticalPhysicsList->SetMaxNumPhotonsPerStep(100);
        opticalPhysicsList->SetScintillationYieldFactor(0.1);
        opticalPhysicsList->SetTrackSecondariesFirst(kScintillation,true);
        opticalPhysicsList->SetTrackSecondariesFirst(kCerenkov,true);
    }
    
    
    // Decay physics
    // Add Radioactive decay
    // Gamma decay of known states
    if(m_Decay){
        decay_List =  new G4DecayPhysics();
        radioactiveDecay_List = new G4RadioactiveDecayPhysics()  ;
        m_PhysList["decay_list"]= decay_List;
        m_PhysList["radioactiveDecay"]=radioactiveDecay_List;
    }
    
    else{
        decay_List = 0;
        radioactiveDecay_List = 0;
    }
    
}

////////////////////////////////////////////////////////////////////////////////
void PhysicsList::ReadConfiguration(std::string filename){
    m_IonBinaryCascadePhysics = 0;
    m_EmExtraPhysics = 0;
    m_HadronElasticPhysics = 0;
    m_NPIonInelasticPhysics = 0;
    m_StoppingPhysics = 0;
    m_OpticalPhysics = 0;
    m_HadronPhysicsQGSP_BIC_HP = 0;
    m_HadronPhysicsINCLXX = 0;
    m_Decay = 0;
    
    std::ifstream file(filename.c_str());
    if(!file.is_open()){
        std::cout << "WARNING: Could not find Physics List option file " << filename << " | Using default Physics List" << std::endl;
        return;
    }
    
    std::cout << "Reading Physics list option file " << filename << std::endl;
    
    std::string name;
    std::string str_value;
    double value;
    while(file >> name >> str_value){
        value = std::atof(str_value.c_str());
        if(name == "EmPhysicsList")
            m_EmList = str_value;
        else if(name == "DefaultCutOff")
            defaultCutValue = value*mm;
        else if(name == "IonBinaryCascadePhysics")
            m_IonBinaryCascadePhysics = value;
        else if(name == "NPIonInelasticPhysics")
            m_NPIonInelasticPhysics = value;
        else if (name == "EmExtraPhysics")
            m_EmExtraPhysics= value;
        else if (name == "HadronElasticPhysics")
            m_HadronElasticPhysics= value;
        else if (name == "StoppingPhysics")
            m_StoppingPhysics= value;
        else if (name == "OpticalPhysics")
            m_OpticalPhysics= value;
        else if (name == "HadronPhysicsQGSP_BIC_HP")
            m_HadronPhysicsQGSP_BIC_HP= value;
        else if (name == "HadronPhysicsINCLXX")
            m_HadronPhysicsINCLXX= value;
        else if (name == "Decay")
            m_Decay = value;
        else
            std::cout <<"WARNING: Physics List Token '" << name << "' unknown. Token is ignored." << std::endl;
    }
    
    // Most special process need decay to be activated
    if( (m_IonBinaryCascadePhysics || m_EmExtraPhysics || m_HadronElasticPhysics || m_NPIonInelasticPhysics
         || m_StoppingPhysics || m_HadronPhysicsQGSP_BIC_HP || m_HadronPhysicsINCLXX) && !m_Decay){
        m_Decay = true;
        std::cout << "Information: Selected process require Decay to be activated." << std::endl;
    }
    
}
/////////////////////////////////////////////////////////////////////////////
PhysicsList::~PhysicsList(){
}

/////////////////////////////////////////////////////////////////////////////
void PhysicsList::ConstructParticle(){
    if(m_OpticalPhysics){
        //G4OpticalPhoton::OpticalPhotonDefinition();
        ((G4VPhysicsConstructor*) opticalPhysicsList)->ConstructParticle();
        
    }
    
    if(decay_List){
        decay_List -> ConstructParticle();
        radioactiveDecay_List->ConstructParticle();
    }
    
    else{
        // If decay is not activated we have to declare the particle ourself
        G4He3::He3Definition();
        G4Deuteron::DeuteronDefinition();
        G4Triton::TritonDefinition();
        G4Alpha::AlphaDefinition();
        G4Proton::ProtonDefinition();
        G4AntiProton::AntiProtonDefinition();
        G4Neutron::NeutronDefinition();
        G4Electron::ElectronDefinition();
        G4Positron::PositronDefinition();
        G4NeutrinoE::NeutrinoEDefinition();
        G4AntiNeutrinoE::AntiNeutrinoEDefinition();
        G4Geantino::GeantinoDefinition();
        G4ChargedGeantino::ChargedGeantinoDefinition();
        G4Gamma::GammaDefinition();
        //  mesons
        G4PionPlus     ::PionPlusDefinition();
        G4PionMinus    ::PionMinusDefinition();
        G4PionZero     ::PionZeroDefinition();
        G4Eta          ::EtaDefinition();
        G4EtaPrime     ::EtaPrimeDefinition();
        //  G4RhoZero      ::RhoZeroDefinition();
        G4KaonPlus     ::KaonPlusDefinition();
        G4KaonMinus    ::KaonMinusDefinition();
        G4KaonZero     ::KaonZeroDefinition();
        G4AntiKaonZero ::AntiKaonZeroDefinition();
        G4KaonZeroLong ::KaonZeroLongDefinition();
        G4KaonZeroShort::KaonZeroShortDefinition();
        // Ion
        G4IonConstructor ionConstructor     ;
        ionConstructor.ConstructParticle()  ;
    }
}

/////////////////////////////////////////////////////////////////////////////
void PhysicsList::ConstructProcess(){
    // Transportation
    AddTransportation();
    
    // Electromagnetic physics
    emPhysicsList -> ConstructProcess();
    if(opticalPhysicsList){
        ((G4VPhysicsConstructor*) opticalPhysicsList)->ConstructProcess();
    }
    // Hadronic physics
    std::map<std::string,G4VPhysicsConstructor*>::iterator it;
    for(it = m_PhysList.begin(); it!= m_PhysList.end(); it++){
        it->second -> ConstructProcess();
    }
    BiasCrossSectionByFactor(m_IonBinaryCascadePhysics);
    
    em_option.SetBuildCSDARange(true);
    em_option.SetDEDXBinningForCSDARange(10*10);
    em_option.SetFluo(true);
    em_option.SetAuger(true);
    
    
    
    return;
}
/////////////////////////////////////////////////////////////////////////////
void PhysicsList::AddStepMax(){
}

/////////////////////////////////////////////////////////////////////////////
void PhysicsList::SetCuts(){
    
    if (verboseLevel >0){
        G4cout << "PhysicsList::SetCuts:";
        G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
    }
    
    // Special Cut for optical photon to be emmitted
    SetCutsWithDefault();
}

////////////////////////////////////////////////////////////////////////////////
/////// Friend Method for CS biasing
////////////////////////////////////////////////////////////////////////////////
void PhysicsList::BiasCrossSectionByFactor(double factor){
    factor++;
}


