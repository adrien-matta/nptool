/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Cyril Lenain    contact adresse: lenain@lpccaen.in2p3.fr *  
 *                                                                           *
 * Creation Date  : June 2018                                                *
 * Last update    : 09/01/19                                                 *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Scorer for Drift Chamber based on cathode strips for X position and      *
 *  drift time for Y                                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *****************************************************************************/

#include "DriftChamberScorers.hh"
#include "G4UnitsTable.hh"
#include "G4VPhysicalVolume.hh"
using namespace DriftChamberScorers;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int DCData::CalculateIndex(const vector<unsigned int>& level) {

    unsigned int size       = level.size();
    unsigned int result     = 0;
    unsigned int multiplier = 1;
    for (unsigned i = 0; i < size; i++) {
        result += level[i] * multiplier;
        multiplier *= 1000;
    }
    return result;
}

vector<DCData>::iterator DCDataVector::find(const unsigned int& index) {
    for (vector<DCData>::iterator it = m_Data.begin(); it != m_Data.end(); it++) {
        if ((*it).GetIndex() == index)
            return it;
    }
    return m_Data.end();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_DriftChamber::PS_DriftChamber(G4String name, vector<G4int> NestingLevel,
        const G4ThreeVector DriftDir,
        const double DriftSpeed, G4int depth)
    : G4VPrimitiveScorer(name, depth) {
        m_NestingLevel = NestingLevel;
        m_Dir          = DriftDir;
        m_DriftSpeed   = DriftSpeed;
    }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
PS_DriftChamber::~PS_DriftChamber() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_DriftChamber::ProcessHits(G4Step* aStep, G4TouchableHistory*) {

    static unsigned int mysize = m_NestingLevel.size();

    // Extract X and DriftTime
    // We use PostStep and PreStep to have at least 2 points
    t_PrePosition     = aStep->GetPreStepPoint()->GetPosition();
    t_PostPosition    = aStep->GetPostStepPoint()->GetPosition();
    G4VSolid* t_solid = aStep->GetPreStepPoint()
        ->GetTouchableHandle()
        ->GetVolume()
        ->GetLogicalVolume()
        ->GetSolid();

    t_postout   = t_solid->DistanceToOut(t_PostPosition, -m_Dir);
    t_preout    = t_solid->DistanceToOut(t_PrePosition, -m_Dir);
    t_DriftTime = ((t_preout + t_postout) / m_DriftSpeed) * 0.5;
    t_X         = (t_PrePosition.x() + t_PostPosition.x()) * 0.5;

    t_Level.clear();

    for (unsigned int i = 0; i < mysize; i++) 
    {
        t_Level.push_back(
                aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(
                    m_NestingLevel[i]));    }

        vector<DCData>::iterator it;
        it = m_Data.find(DCData::CalculateIndex(t_Level)); 
        if (it != m_Data.end()) {
            it->Add(t_DriftTime / microsecond, t_X);
        }

        else { 
            m_Data.Set(t_DriftTime / microsecond, t_X , t_Level);
        }

        return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DriftChamber::Initialize(G4HCofThisEvent*) { clear(); }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DriftChamber::EndOfEvent(G4HCofThisEvent*) {

    // An average is taken
    for (vector<DCData>::iterator it = m_Data.begin(); it != m_Data.end(); it++) {

        it->Moy((*it).GetCounter());
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PS_DriftChamber::clear() {
    m_Data.clear();
    t_Level.clear(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DriftChamber::DrawAll() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DriftChamber::PrintAll() {}
//....oooOO0OOooo.......oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
