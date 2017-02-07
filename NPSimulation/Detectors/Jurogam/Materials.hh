#ifndef Materials_h
#define Materials_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4Material;

class Materials
{
  public:
  //Materials();
  ~Materials();
  static Materials* GetInstance();

  G4Material* GetMaterial(const G4String&);


  private:
  Materials();

  void CreateMaterials();


  private:

    static Materials* instance;

};

#endif



