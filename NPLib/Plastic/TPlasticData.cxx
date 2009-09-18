#include <iostream>
using namespace std;

#include "TPlasticData.h"


ClassImp(TPlasticData)

TPlasticData::TPlasticData()
{
}



TPlasticData::~TPlasticData()
{
}



void TPlasticData::Clear()
{
    fPlastic_Energy.clear();
    fPlastic_Number.clear();
    fPlastic_Time.clear();
}



void TPlasticData::Dump()
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

	for(short i = 0 ; i<fPlastic_Energy.size() ; i ++)
		{
			cout << "Plastic Number " << fPlastic_Number[i] << " Energy: " << fPlastic_Energy[i]  << " Time: "<< fPlastic_Time[i] << endl;
		
		}
   
}
