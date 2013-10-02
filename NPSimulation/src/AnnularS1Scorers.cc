#include "AnnularS1Scorers.hh"
#include "G4UnitsTable.hh"
#include "AnnularS1.hh"

using namespace ANNULARS1;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Added by Adrien MATTA:
// Those Scorer use TrackID as map index. This way ones can rebuild energy deposit,
// time of flight or position,... particle by particle for each event. Because standard
// scorer provide by G4 don't work this way but using a global ID for each event you should
// not use those scorer with some G4 provided ones or being very carefull doing so.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Energy Scorer (deal with multiple particle hit)
AnnularS1ScorerEnergy::AnnularS1ScorerEnergy(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}

AnnularS1ScorerEnergy::~AnnularS1ScorerEnergy()
{
}

G4bool AnnularS1ScorerEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(index, edep);
   return TRUE;
}

void AnnularS1ScorerEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void AnnularS1ScorerEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void AnnularS1ScorerEnergy::clear()
{
   EvtMap->clear();
}

void AnnularS1ScorerEnergy::DrawAll()
{
}

void AnnularS1ScorerEnergy::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
   std::map<G4int, G4double*>::iterator itr = EvtMap->GetMap()->begin();
   for (; itr != EvtMap->GetMap()->end(); itr++) {
      G4cout << "  copy no.: " << itr->first
      << "  energy deposit: " << G4BestUnit(*(itr->second), "Energy")
      << G4endl;
   }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Time Scorer
AnnularS1ScorerTime::AnnularS1ScorerTime(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}

AnnularS1ScorerTime::~AnnularS1ScorerTime()
{
}

G4bool AnnularS1ScorerTime::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   G4double TOF  = aStep->GetPreStepPoint()->GetGlobalTime();
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(index, TOF);
   return TRUE;
}

void AnnularS1ScorerTime::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
         GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void AnnularS1ScorerTime::EndOfEvent(G4HCofThisEvent*)
{
}

void AnnularS1ScorerTime::clear()
{
   EvtMap->clear();
}

void AnnularS1ScorerTime::DrawAll()
{
}

void AnnularS1ScorerTime::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Strip position Scorer
// Theta
AnnularS1ScorerThetaStripNumber::AnnularS1ScorerThetaStripNumber(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize =   StripPlaneSize ;
   m_NumberOfStrip    = NumberOfStrip  ;
}

AnnularS1ScorerThetaStripNumber::~AnnularS1ScorerThetaStripNumber()
{
}

G4bool AnnularS1ScorerThetaStripNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // Hit position in the world frame
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();

   // Hit position in the detector frame
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   // Radial position in the Oxy plane
   G4double r = sqrt(pow(POS(0), 2) + pow(POS(1), 2));

   // Phi angle
   G4double phi = atan2(POS(1), POS(0)) / deg;
   if (phi < 0) phi += 360;

   // Phi quadrant
   G4double PhiWidth = 360. / NbThetaQuadrant;
   G4double PhiQuadrantNumber = floor(phi / PhiWidth);

   // Theta strip pitch
   // Interstrip should be taken into account here. To be done
   G4double ThetaStripPitch = (FirstStageRmax - FirstStageRmin) / NbThetaStrips;
   G4double dummy = (r - FirstStageRmin);
   if (dummy < 0 && fabs(dummy) < 1e-6) dummy *= -1;
   G4double ThetaStripNumber = floor(dummy / ThetaStripPitch);
   ThetaStripNumber += PhiQuadrantNumber*NbThetaStrips;

   if (ThetaStripNumber < 1e-6) {
    /*  G4cout << "POS: " << POS << G4endl;
      G4cout << "r, phi " << r << "  " << phi << G4endl;
      G4cout << "PhiWidth, PhiQuadrantNumber " << PhiWidth << "  " << PhiQuadrantNumber << G4endl;
      G4cout << "ThetaStripPitch, ThetaStripNumber, dummy " << ThetaStripPitch << "  " << ThetaStripNumber << "  " << dummy << G4endl;*/
   }

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(index, ThetaStripNumber);
   return TRUE;
}

void AnnularS1ScorerThetaStripNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void AnnularS1ScorerThetaStripNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void AnnularS1ScorerThetaStripNumber::clear()
{
   EvtMap->clear();
}

void AnnularS1ScorerThetaStripNumber::DrawAll()
{
}

void AnnularS1ScorerThetaStripNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Phi
AnnularS1ScorerPhiStripNumber::AnnularS1ScorerPhiStripNumber(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize =   StripPlaneSize ;
   m_NumberOfStrip    = NumberOfStrip  ;
}

AnnularS1ScorerPhiStripNumber::~AnnularS1ScorerPhiStripNumber()
{
}

G4bool AnnularS1ScorerPhiStripNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // Hit position in the world frame
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();

   // Hit position in the detector frame
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   // Phi angle
   G4double phi = atan2(POS(1), POS(0)) / deg;
   if (phi < 0) phi += 360;

   // Phi strip number
   // Interstrip should be taken into account here. To be done
   G4double PhiWidth = 360. / NbPhiStrips;
   G4double PhiStripNumber = floor(phi / PhiWidth);   

//   G4cout << POS << G4endl;
//   G4cout << "phi " << phi << " PhiWidth  " << PhiWidth << "  PhiStripNumber " << PhiStripNumber << G4endl;

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(index, PhiStripNumber);
   return TRUE;
}

void AnnularS1ScorerPhiStripNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void AnnularS1ScorerPhiStripNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void AnnularS1ScorerPhiStripNumber::clear()
{
   EvtMap->clear();
}

void AnnularS1ScorerPhiStripNumber::DrawAll()
{
}

void AnnularS1ScorerPhiStripNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
