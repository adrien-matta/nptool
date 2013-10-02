#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class PhysicsList: public G4VUserPhysicsList
{
public:
   PhysicsList();
   ~PhysicsList();
   void MyOwnConstruction();


protected:
   // Construct particle and physics
   void ConstructParticle();
   void ConstructProcess();
   void ConstructDecay();


   void SetCuts();

protected:
// these methods Construct particles
   void ConstructBosons();
   void ConstructLeptons();
   void ConstructMesons();
   void ConstructBaryons();
   void ConstructIons();

protected:
// these methods Construct physics processes and register them
   void ConstructGeneral();
   void ConstructEM();
};

#endif







