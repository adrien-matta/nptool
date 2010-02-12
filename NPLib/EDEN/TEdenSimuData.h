#ifndef __EdenSimuDATA__
#define __EdenSimuDATA__
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 11/02/2010                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *    This class described is used to store the results of the G4 simulation *
 *    concerning the EDEN neutron detector.                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    This class is mainly dedicated for the G4 simulation results and not   *
 *    for the analysis of real experiment.                                   *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std;


class TEdenSimuData : public TObject {
 private:
   vector<short>   fEdenSimu_DetectorNumber;
   vector<double>  fEdenSimu_Charge;	
   vector<double>  fEdenSimu_Time;
   
 public:
   TEdenSimuData();
   virtual ~TEdenSimuData();

   void	Clear();
   void	Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   int		GetEdenSimuMultiplicity()		{return fEdenSimu_DetectorNumber.size();}
   int		GetEdenSimuDetectorNumber(int i)	{return fEdenSimu_DetectorNumber.at(i);}
   double	GetEdenSimuCharge(int i)		{return fEdenSimu_Charge.at(i);}
   double	GetEdenSimuTime(int i)			{return fEdenSimu_Time.at(i);}
   
   /////////////////////           SETTERS           ////////////////////////
   void	SetEdenSimuDetectorNumber(int N)	{fEdenSimu_DetectorNumber.push_back(N);}
   void	SetEdenSimuCharge(double Q)		{fEdenSimu_Charge.push_back(Q);}
   void	SetEdenSimuTime(double T)		{fEdenSimu_Time.push_back(T);}
   
   ClassDef(TEdenSimuData,1)  // EdenSimuData structure
};

#endif
