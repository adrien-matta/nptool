#include "Hyde2TrackerModule.h"



Hyde2TrackerModule::Hyde2TrackerModule()
{
   InitializeIndex();
}



Hyde2TrackerModule::~Hyde2TrackerModule()
{
}



void Hyde2TrackerModule::InitializeIndex()
{
   m_index["Square1"]     = 2000;
   m_index["Trapezoid1"]  = 2200;
   m_index["Trapezoid2"]  = 2300;
}
