#ifndef __CHIO_DIGDATA__
#define __CHIO_DIGDATA__

#include <vector>
#include "TObject.h"
#include "TGraph.h"
using namespace std;


class TChio_digData : public TObject {
 private:
   // ADC
   vector<double>	fChio_dig_Energy;
   vector<double>	fChio_dig_Time;


 public:
   TChio_digData();
   virtual ~TChio_digData();

   void Clear();
   void Clear(const Option_t*) {};
   void Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   inline unsigned int GetEsize()      {return fChio_dig_Energy.size();}
   inline vector<double> GetEnergy()   {return fChio_dig_Energy;}
   TGraph* GetEnergyAsGraph();

   /////////////////////           SETTERS           ////////////////////////
   // (E)
   void SetEnergy(vector<double>& E) {fChio_dig_Energy=E;}
   void AddEnergyPoint(double& E,double& T){
    fChio_dig_Energy.push_back(E);
    fChio_dig_Time.push_back(T);}

   ClassDef(TChio_digData,2)  // Chio_digData structure
};

#endif
