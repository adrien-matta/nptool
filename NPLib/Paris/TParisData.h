#ifndef __ParisDATA__
#define __ParisDATA__
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 04/12/2009                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of the Paris detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;


class TParisData : public TObject {
 private:
   vector<double>	fParis_Energy;
   vector<double>	fParis_Time;
   vector<short>	fParis_Number;
   
 public:
   TParisData();
   virtual ~TParisData();

   void	Clear();
   void	Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   double	GetEnergy(int i)		{return fParis_Energy[i];}
   // (T)
   double	GetTime(int i)			{return fParis_Time[i];}
   // (N)
   int		GetParisNumber(int i)		{return fParis_Number[i];}
   
   
   double	GetEnergySize()			{return fParis_Energy.size();}
   // (T)
   double	GetTimeSize()			{return fParis_Time.size();}
   // (N)
   int      	GetParisNumberSize()		{return fParis_Number.size();}
   
   
   /////////////////////           SETTERS           ////////////////////////
   // (E)
   void	SetEnergy(double E)			{fParis_Energy.push_back(E);}
   void	SetTime(double T)			{fParis_Time.push_back(T);}
   void	SetParisNumber(int N)			{fParis_Number.push_back(N);}
   
   ClassDef(TParisData,1)  // ParisData structure
};

#endif
