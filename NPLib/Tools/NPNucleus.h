#ifndef __NUCLEUS__
#define __NUCLEUS__
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class manage a nucleus, data are read in the nubtab03.asc file      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#define uma      931.49432
#include <string>

using namespace std;


namespace NPL 
	{
		class Nucleus {

		public:
		   Nucleus();
		   Nucleus(string isotope);
		   Nucleus(int Z, int A);
		   ~Nucleus();



		 private :
		   const char*		fName;			// Nucleus name
		   int				fCharge;		// Nucleus charge
		   int				fAtomicWeight;	// Nucleus atomic weight
		   double			fMassExcess;	// Nucleus mass excess in keV
		   const char*		fSpinParity;	// Nucleus spin and parity
		   double			fSpin;			// Nucleus spin
		   const char*		fParity;		// Nucleus parity
		   
		 protected :
		   void Extract(const char* line);

		 public :
		   string		    GetName() 			const	;
		   int				GetZ() 				const 				{return fCharge;}
		   int				GetA() 				const 				{return fAtomicWeight;}
		   double			GetMassExcess() 	const 				{return fMassExcess;}
		   const char*		GetSpinParity() 	const 				{return fSpinParity;}
		   double			GetSpin() 			const 				{return fSpin;}
		   const char*		GetParity() 		const 				{return fParity;}
		   void				SetName(const char* name) 				{fName = name;}
		   void				SetZ(int charge) 						{fCharge = charge;}
		   void				SetA(int mass) 							{fAtomicWeight = mass;}
		   void				SetMassExcess(double massexcess) 		{fMassExcess = massexcess;}
		   void				SetSpinParity(const char* spinparity) 	{fSpinParity = spinparity;}
		   void				SetSpin(double spin) 					{fSpin = spin;}
		   void				SetParity(const char* parity) 			{fParity = parity;}

		   // Nuclear mass in MeV
		   double			Mass() const {return (fAtomicWeight*uma + fMassExcess/1000.);}
		   void				Print() const	;
		};


	}


#endif
