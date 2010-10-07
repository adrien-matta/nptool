#include "TGaspardTrackerModule.h"



TGaspardTrackerModule::TGaspardTrackerModule()
{
   InitializeIndex();
}



TGaspardTrackerModule::~TGaspardTrackerModule()
{
}



void TGaspardTrackerModule::InitializeIndex()
{
   m_index["Square"]     =    0;
   m_index["Trapezoid"]  =  100;
   m_index["Annular"]    =  200;
   m_index["DummyShape"] = 1000;
}
