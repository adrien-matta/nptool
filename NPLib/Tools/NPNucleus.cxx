/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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

// C++ headers
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "NPNucleus.h"

using namespace std;
using namespace NPL;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Nucleus::Nucleus()
{
   //----------- Default Constructor ----------
   fName	 	 = "XX DEFAULT XX"	;
   fCharge	 	 = 0				;
   fAtomicWeight = 0				;
   fMassExcess	 = 0				;
   fSpinParity	 = ""				;
   fSpin	 	 = 0				;
   fParity	 	 = ""				;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Nucleus::Nucleus(string isotope)
{
   //----------- Constructor Using nubtab03.asc by name----------
   // open the file to read and check if it is open
   
   const char* Isotope = isotope.c_str();
   
   ifstream inFile;
   string Path = getenv("NPLIB") ;
   string FileName = Path + "/Tools/nubtab03.asc";
   inFile.open(FileName.c_str());
   
   // reading the file
   string line, s_name;
   if (inFile.is_open()) {
      while (!inFile.eof()) {
         getline(inFile,line);
	 s_name = line.substr(11,6);
	 if (s_name.find(Isotope) != string::npos) break;
      }
      Extract(line.data());
   }
   else cout << "Unable to open file nuclear data base file " << FileName << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Nucleus::Nucleus(int Z, int A)
{
   //----------- Constructor Using nubtab03.asc by Z and A----------

   // open the file to read and check if it is open
   ifstream inFile;
   string Path = getenv("NPLIB") ;
   string FileName = Path + "/Tools/nubtab03.asc";
   inFile.open(FileName.c_str());
   
   // reading the file
   string line, s_charge, s_mass;
   int charge, mass;
   // read the data
   if (inFile.is_open()) {
      while (!inFile.eof()) {
         getline(inFile,line);
	 s_mass   = line.substr(0,3);
	 s_charge = line.substr(4,4);
	 charge = atoi(s_charge.data());
	 mass   = atoi(s_mass.data());
	 if (mass == A && charge == Z*10) break;
      }
      Extract(line.data());
   }
   else cout << "Unable to open file nuclear data base file " << FileName << endl;
   
   // close the file
   inFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Nucleus::~Nucleus()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void Nucleus::Extract(const char* line)
{
   string ligne = line;

   // name of the isotope
   string s_name = ligne.substr(11,6);
   fName = s_name.data();

   // charge and mass
   string s_mass   = ligne.substr(0,3);
   string s_charge = ligne.substr(4,4);
   fAtomicWeight = atoi(s_mass.data());
   fCharge       = atoi(s_charge.data()); fCharge /= 10;

   // mass excess
   string s_excess = ligne.substr(18,10);
   fMassExcess = atof(s_excess.data());

   // spin and parity
   string s_spinparity = ligne.substr(79,14);
   fSpinParity = s_spinparity.data();
   size_t found_p = s_spinparity.find("+");
   size_t found_m = s_spinparity.find("-");
//   size_t found_( = s_jpi.find("(");
//   size_t found_) = s_jpi.find(")");
//   size_t found_# = s_jpi.find("#");
   // parity
   if (found_p != string::npos) fParity = "+";
   if (found_m != string::npos) fParity = "-";
   // spin
   if (s_spinparity.find("0")    != string::npos) fSpin = 0		;
   if (s_spinparity.find("1")    != string::npos) fSpin = 1		;
   if (s_spinparity.find("2")    != string::npos) fSpin = 2		;
   if (s_spinparity.find("3")    != string::npos) fSpin = 3		;
   if (s_spinparity.find("4")    != string::npos) fSpin = 4		;
   if (s_spinparity.find("5")    != string::npos) fSpin = 5		;
   if (s_spinparity.find("6")    != string::npos) fSpin = 6		;
   if (s_spinparity.find("7")    != string::npos) fSpin = 7		;
   if (s_spinparity.find("8")    != string::npos) fSpin = 8		;
   if (s_spinparity.find("9")    != string::npos) fSpin = 9		;
   if (s_spinparity.find("10")   != string::npos) fSpin = 10	;
   if (s_spinparity.find("1/2")  != string::npos) fSpin = 0.5	;
   if (s_spinparity.find("3/2")  != string::npos) fSpin = 1.5	;
   if (s_spinparity.find("5/2")  != string::npos) fSpin = 2.5	;
   if (s_spinparity.find("7/2")  != string::npos) fSpin = 3.5	;
   if (s_spinparity.find("9/2")  != string::npos) fSpin = 4.5	;
   if (s_spinparity.find("11/2") != string::npos) fSpin = 5.5	;
   if (s_spinparity.find("13/2") != string::npos) fSpin = 6.5	;
   if (s_spinparity.find("15/2") != string::npos) fSpin = 7.5	;
   if (s_spinparity.find("17/2") != string::npos) fSpin = 8.5	;
   if (s_spinparity.find("19/2") != string::npos) fSpin = 9.5	;
   if (s_spinparity.find("21/2") != string::npos) fSpin = 10.5	;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void Nucleus::Print() const
{
   //------------ Imprime a l'ecran les caracteristiques d'un noyau -------
   cout << endl;
   cout << "Isotope: " << fName << endl;
   cout << "Z = " << fCharge << "   A = " << fAtomicWeight << endl;
   cout << "Mass Excess: " << fMassExcess << " keV" << endl;
   cout << "Jpi = " << fSpinParity << " (" << fSpin << fParity << ")" << endl;
}

string Nucleus::GetName() const 
{
string Name=string(fName,1);
return Name;

}


