#include "GaspardTrackerModule.h"



GaspardTrackerModule::GaspardTrackerModule()
{
   InitializeIndex();
}



GaspardTrackerModule::~GaspardTrackerModule()
{
}



void GaspardTrackerModule::InitializeIndex()
{
   m_index["Square"]     =    0;
   m_index["Trapezoid"]  =  100;
   m_index["Annular"]    =  200;
   m_index["DummyShape"] = 1000;
}
