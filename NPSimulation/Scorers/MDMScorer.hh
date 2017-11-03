#ifndef MDMScorer_h
#define MDMScorer_h 1
#include "G4VPrimitiveScorer.hh"
#include "G4ThreeVector.hh"
#include "NPSHitsMap.hh"
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......         
class MDMScorer : public G4VPrimitiveScorer
{

public: // with description
  MDMScorer(G4String name, G4String VolumeName, G4int depth);
  virtual ~MDMScorer();

protected: // with description
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
  virtual void Initialize(G4HCofThisEvent*);
  virtual void EndOfEvent(G4HCofThisEvent*);
  virtual void clear();
  virtual void DrawAll();
  virtual void PrintAll();

public:
  struct Infos {
    double Edep;
    double Mass;
    double Charge;
    G4ThreeVector Pos;
    G4ThreeVector Mom;
    Infos& operator+=(const Infos& rhs)
    {
      // Only sum edep - leave others the same
      // (want to be same as beginning of track)
      Edep = Edep + rhs.Edep;
      return *this;
    }
  };
  
private:
  G4String m_VolumeName;
  G4int HCID;
  NPS::HitsMap<Infos>* EvtMap;
};       




#endif
