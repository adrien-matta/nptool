/*****************************************************************************
 * Copyright (C) 2009-2010    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: matta@ipno.in2p3.fr   *
 *                                                                           *
 * Creation Date   : March 2009                                              *
 * Last update     : January 2011                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class deal with Two Body transfert Reaction                         *
 *  Physical parameter (Nuclei mass) are loaded from the nubtab03.asc file   *
 *  (2003 nuclear table of isotopes mass).                                   *
 *                                                                           *
 *  KineRelativistic: Used in NPSimulation                                   *
 *  A relativistic calculation is made to compute Light and Heavy nuclei     * 
 *  angle given the Theta CM angle.                                          *
 *                                                                           * 
 *  ReconstructRelativistic: Used in NPAnalysis                              *
 *  A relativistic calculation is made to compute Excitation energy given the*
 *  light angle and energy in Lab frame.                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 20/01/2011: Add support for excitation energy for light ejectile     *
 *                  (N. de Sereville)                                        *
 *     Based on previous work by N.de Sereville                              *
 *                                                                           *
 *****************************************************************************/
 
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
//#include <stdlib.h>
#include <cmath>
#include <vector>

#include "NPReaction.h"

// Use CLHEP System of unit and Physical Constant
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"


using namespace NPL;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Reaction::Reaction()
{
   //------------- Default Constructor -------------

   fNuclei1              = new Nucleus();
   fNuclei2              = new Nucleus();
   fNuclei3              = new Nucleus();
   fNuclei4              = new Nucleus();
   fBeamEnergy           = 0;
   fThetaCM              = 0;
   fExcitation3      = 0;
   fExcitation4      = 0;
   fQValue               = 0;
   fCrossSectionAngleMin = 0;
   fCrossSectionAngleMax = 180;
   initializePrecomputeVariable();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Reaction::Reaction(string name1, string name2, string name3, string name4, double BeamEnergy, double ExcitationEnergyLight, double ExcitationEnergyHeavy ,string Path, double CSThetaMin, double CSThetaMax)
{
   SetEveryThing( name1, name2, name3, name4, BeamEnergy, ExcitationEnergyLight, ExcitationEnergyHeavy, Path, CSThetaMin, CSThetaMax);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void Reaction::SetEveryThing(string name1, string name2, string name3, string name4, double BeamEnergy, double ExcitationEnergyLight, double ExcitationEnergyHeavy, string Path, double CSThetaMin, double CSThetaMax)
{
   //------------- Constructor with nuclei names and beam energy ------------
     
   fNuclei1         = new Nucleus(name1);
   fNuclei2         = new Nucleus(name2);
   fNuclei3         = new Nucleus(name3);
   fNuclei4         = new Nucleus(name4);
   fBeamEnergy      = BeamEnergy;
   fThetaCM         = 0;
   fExcitation3 = ExcitationEnergyLight;
   fExcitation4 = ExcitationEnergyHeavy;
   fQValue          = (fNuclei1->GetMassExcess() + fNuclei2->GetMassExcess()
                      - fNuclei3->GetMassExcess() - fNuclei4->GetMassExcess()) / 1000;

   int masse  = fNuclei1->GetA() + fNuclei2->GetA() - fNuclei3->GetA() - fNuclei4->GetA();
   int charge = fNuclei1->GetZ() + fNuclei2->GetZ() - fNuclei3->GetZ() - fNuclei4->GetZ();
   if (masse || charge) {
      cout << endl;
      cout << "**********************************       Error       **********************************" << endl;
      cout << "* NPReaction: charge of mass not conserved.        Check you event generator file     *" << endl;
      cout << "***************************************************************************************" << endl;
      exit(1);
   }
   
   ///Read the differential cross section
   string GlobalPath = getenv("NPTOOL");
   string StandardPath = GlobalPath + "/Inputs/CrossSection/" + Path;
   ifstream CSFile;
   CSFile.open( StandardPath.c_str() );
   
   if(CSFile.is_open()) cout << "Reading Cross Section File " << Path << endl;
   
   // In case the file is not found in the standard path, the programm try to interpret the file name as an absolute or relative file path.
   else 
    {
      CSFile.open( Path.c_str() );
      if(CSFile.is_open()) { cout << "Reading Cross Section File " << Path << endl;}
      
      else {cout << "Cross Section File " << Path << " not found" << endl;return;}
    }

   double CSBuffer,AngleBuffer;
   vector<double> CrossSectionBuffer;
   double thetamin = 200;
   double thetamax = -10;
   while(!CSFile.eof()) {
      CSFile >> AngleBuffer;
      CSFile >> CSBuffer;   
      // only treat angular range defined by user
      if (AngleBuffer < CSThetaMin || AngleBuffer > CSThetaMax) continue; 
      double CSFinal = CSBuffer*sin(AngleBuffer*deg);
      CrossSectionBuffer.push_back(CSFinal);
      // determine theta min and max
      if (AngleBuffer < thetamin) thetamin = AngleBuffer;
      if (AngleBuffer > thetamax) thetamax = AngleBuffer;
   }

   // set theta min and max values
   fCrossSectionAngleMin = thetamin;
   fCrossSectionAngleMax = thetamax;

   CSFile.close();
   fCrossSectionSize = CrossSectionBuffer.size();
   fCrossSection = new double[fCrossSectionSize] ;
   for(int i = 0 ; i < fCrossSectionSize ; i++ )   fCrossSection[i] = CrossSectionBuffer[i];
   initializePrecomputeVariable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Reaction::~Reaction()
{
   //------------- Default Destructor ------------

   delete fNuclei1;
   delete fNuclei2;
   delete fNuclei3;
   delete fNuclei4;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
bool Reaction::CheckKinematic()
{
	double theta = fThetaCM;
	if (m1 > m2) theta = M_PI - fThetaCM;
	
	fEnergyImpulsionCM_3	= TLorentzVector(pCM_3*sin(theta),0,pCM_3*cos(theta),ECM_3);
	fEnergyImpulsionCM_4	= fTotalEnergyImpulsionCM - fEnergyImpulsionCM_3;
	
	fEnergyImpulsionLab_3 = fEnergyImpulsionCM_3;
	fEnergyImpulsionLab_3.Boost(0,0,BetaCM);
	fEnergyImpulsionLab_4 = fEnergyImpulsionCM_4;
	fEnergyImpulsionLab_4.Boost(0,0,BetaCM);
	
	if ( fabs(fTotalEnergyImpulsionLab.E() - (fEnergyImpulsionLab_3.E()+fEnergyImpulsionLab_4.E()))> 1e-6){
		cout << "Problem with energy conservation" << endl;
		return false;
	}
		
	else{
		//cout << "Kinematic OK" << endl;
		return true;
	}
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void Reaction::KineRelativistic(double &ThetaLab3, double &KineticEnergyLab3,
                                double &ThetaLab4, double &KineticEnergyLab4) 
{
	// 2-body relativistic kinematics: direct + inverse
	// EnergieLab3,4 : lab energy in MeV of the 2 ejectiles
	// ThetaLab3,4   : angles in rad

	// case of inverse kinematics
	double theta = fThetaCM;
	if (m1 > m2) theta = M_PI - fThetaCM;

	fEnergyImpulsionCM_3	= TLorentzVector(pCM_3*sin(theta),0,pCM_3*cos(theta),ECM_3);
	fEnergyImpulsionCM_4	= fTotalEnergyImpulsionCM - fEnergyImpulsionCM_3;
	
	fEnergyImpulsionLab_3 = fEnergyImpulsionCM_3;
	fEnergyImpulsionLab_3.Boost(0,0,BetaCM);
	fEnergyImpulsionLab_4 = fEnergyImpulsionCM_4;
	fEnergyImpulsionLab_4.Boost(0,0,BetaCM);
	
	// Angle in the lab frame
	ThetaLab3 = fEnergyImpulsionLab_3.Angle(fEnergyImpulsionLab_1.Vect());
	if (ThetaLab3 < 0) ThetaLab3 += M_PI;

	ThetaLab4 = fEnergyImpulsionLab_4.Angle(fEnergyImpulsionLab_1.Vect());
	if (fabs(ThetaLab3) < 1e-6) ThetaLab3 = 0;
	ThetaLab4 = fabs(ThetaLab4);
	if (fabs(ThetaLab4) < 1e-6) ThetaLab4 = 0;
	
	// Kinetic Energy in the lab frame
	KineticEnergyLab3 = fEnergyImpulsionLab_3.E() - m3;
	KineticEnergyLab4 = fEnergyImpulsionLab_4.E() - m4;
	
	// test for total energy conversion
	if (fabs(fTotalEnergyImpulsionLab.E() - (fEnergyImpulsionLab_3.E()+fEnergyImpulsionLab_4.E())) > 1e-6) 
		cout << "Problem for energy conservation" << endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double Reaction::ReconstructRelativistic(double EnergyLab, double ThetaLab) 
{
   // EnergyLab in MeV
   // ThetaLab in rad
	double E3 = m3 + EnergyLab;
	double p_Lab_3 = sqrt(E3*E3 - m3*m3);
	
	fEnergyImpulsionLab_3 = TLorentzVector(p_Lab_3*sin(ThetaLab),0,p_Lab_3*cos(ThetaLab),E3);
	fEnergyImpulsionLab_4 = fTotalEnergyImpulsionLab - fEnergyImpulsionLab_3;
	
	double Eex = fEnergyImpulsionLab_4.Mag() - fNuclei4->Mass();
   
	return Eex;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
//Return ThetaCM                           
double  Reaction::EnergyLabToThetaCM(double EnergyLab, double ThetaLab)
   {
	   double E3 = m3 + EnergyLab;
	   double p_Lab_3 = sqrt(E3*E3 - m3*m3);
	   
	   fEnergyImpulsionLab_3 = TLorentzVector(p_Lab_3*sin(ThetaLab),0,p_Lab_3*cos(ThetaLab),E3);
	   fEnergyImpulsionCM_3 = fEnergyImpulsionLab_3;
	   fEnergyImpulsionCM_3.Boost(0,0,-BetaCM);
	   
	   double ThetaCM = CLHEP::pi - fEnergyImpulsionCM_1.Angle(fEnergyImpulsionCM_3.Vect());

	   return(ThetaCM);
   }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void Reaction::Print() const
   {
      // Print informations concerning the reaction

      cout << "Reaction : " << fNuclei2->GetName() << "(" << fNuclei1->GetName() 
          << "," << fNuclei3->GetName() << ")" << fNuclei4->GetName() << "  @  " 
          << fBeamEnergy << " MeV" 
      << endl   ;
      
      cout << "Exc Nuclei 3 = " << fExcitation3 << " MeV" << endl;
      cout << "Exc Nuclei 4 = " << fExcitation4 << " MeV" << endl;
      cout << "Qgg = " << fQValue << " MeV" << endl;
   }
   


//////////////////////////////////////////////////////////////////////////////////////////////////////////   
void Reaction::ReadConfigurationFile(string Path)
   {   
      ////////General Reading needs////////
         string LineBuffer;
         string DataBuffer;

      ////////Reaction Setting needs///////
         string Beam, Target, Heavy, Light, CrossSectionPath ;
         double BeamEnergy = 0 , ExcitationEnergy3 = 0, ExcitationEnergy4 = 0;         
         double CSHalfOpenAngleMin = 0, CSHalfOpenAngleMax = 0;
         bool ReadingStatus = false ;
         bool check_Beam = false ;
         bool check_Target = false ;
         bool check_Light = false ;
         bool check_Heavy = false ;
         bool check_ExcitationEnergy3 = false ;
         bool check_ExcitationEnergy4 = false ;
         bool check_BeamEnergy = false ;
         bool check_CrossSectionPath = false ;

         
      //////////////////////////////////////////////////////////////////////////////////////////
         ifstream ReactionFile;
         string GlobalPath = getenv("NPTOOL");
          string StandardPath = GlobalPath + "/Inputs/EventGenerator/" + Path;
         ReactionFile.open(StandardPath.c_str());
         
         if (ReactionFile.is_open()) {cout << "Reading Reaction File " << Path << endl ;}

       // In case the file is not found in the standard path, the programm try to interpret the file name as an absolute or relative file path.
       else 
        {
          ReactionFile.open( Path.c_str() );
          if(ReactionFile.is_open()) { cout << "Reading Reaction File " << Path << endl;}
          
          else {cout << "Reaction File " << Path << " not found" << endl;return;}
        }

         while (!ReactionFile.eof()) {
            //Pick-up next line
            getline(ReactionFile, LineBuffer);

            

            if (LineBuffer.compare(0, 9, "Transfert") == 0) { ReadingStatus = true ;}


      while(ReadingStatus){
                
                 ReactionFile >> DataBuffer;
                 
                 //Search for comment Symbol %
                   if (LineBuffer.compare(0, 1, "%") == 0) {/* Do Nothing */;}
                 
                  else if (DataBuffer=="Beam=") {
                     check_Beam = true ;
                     ReactionFile >> DataBuffer;
                     Beam = DataBuffer;
                     cout << "Beam " << Beam << endl;
                  }
         
                  else if (DataBuffer=="Target=") {
                     check_Target = true ;
                     ReactionFile >> DataBuffer;
                     Target = DataBuffer;
                     cout << "Target " << Target << endl;
                  }

                  else if (DataBuffer=="Light=") {
                     check_Light = true ;
                     ReactionFile >> DataBuffer;
                     Light = DataBuffer;
                     cout << "Light " << Light << endl;
                  }

                 else if  (DataBuffer== "Heavy=") {
                     check_Heavy = true ;
                     ReactionFile >> DataBuffer;
                     Heavy = DataBuffer;
                     cout << "Heavy " << Heavy << endl;
                  }

                 else if  (DataBuffer=="ExcitationEnergy3=" || DataBuffer=="ExcitationEnergyLight=") {
                    check_ExcitationEnergy3 = true ;
                     ReactionFile >> DataBuffer;
                     ExcitationEnergy3 = atof(DataBuffer.c_str()) * MeV;
                     cout << "Excitation Energy Nuclei 3: " << ExcitationEnergy3 / MeV << " MeV" << endl;
                  }

                 else if  (DataBuffer=="ExcitationEnergy4=" || DataBuffer=="ExcitationEnergyHeavy=") {
                    check_ExcitationEnergy4 = true ;
                     ReactionFile >> DataBuffer;
                     ExcitationEnergy4 = atof(DataBuffer.c_str()) * MeV;
                     cout << "Excitation Energy Nuclei 4: " << ExcitationEnergy4 / MeV << " MeV" << endl;
                  }

                 else if  (DataBuffer=="BeamEnergy=") {
                    check_BeamEnergy = true ;
                     ReactionFile >> DataBuffer;
                     BeamEnergy = atof(DataBuffer.c_str()) * MeV;
                     cout << "Beam Energy " << BeamEnergy / MeV << " MeV" << endl;
                  }

                 else if  (DataBuffer== "CrossSectionPath=") {
                     check_CrossSectionPath = true ;
                     ReactionFile >> CrossSectionPath;
                     cout << "Cross Section File: " << CrossSectionPath << endl ;
                  }

                  else if (DataBuffer.compare(0, 17, "HalfOpenAngleMin=") == 0) {
                     ReactionFile >> DataBuffer;
                     CSHalfOpenAngleMin = atof(DataBuffer.c_str()) * deg;
                     cout << "HalfOpenAngleMin " << CSHalfOpenAngleMin / deg << " degree" << endl;
                  }

                  else if (DataBuffer.compare(0, 17, "HalfOpenAngleMax=") == 0) {
                     ReactionFile >> DataBuffer;
                     CSHalfOpenAngleMax = atof(DataBuffer.c_str()) * deg;
                     cout << "HalfOpenAngleMax " << CSHalfOpenAngleMax / deg << " degree" << endl;
                  }

                 
                  ///////////////////////////////////////////////////
               //   If no Transfert Token and no comment, toggle out
                  else 
                     {/*Ignore Token used by G4*/}
                     
                  ///////////////////////////////////////////////////
               //   If all Token found toggle out
                  if (check_Beam && check_Target && check_Light && check_Heavy && check_ExcitationEnergy3 
                     && check_ExcitationEnergy4 && check_BeamEnergy && check_CrossSectionPath)
                     ReadingStatus = false;
            }
         }
         
         SetEveryThing(Beam, Target, Light, Heavy,BeamEnergy,ExcitationEnergy3, ExcitationEnergy4,CrossSectionPath, CSHalfOpenAngleMin, CSHalfOpenAngleMax);

         ReactionFile.close();
   }
   

////////////////////////////////////////////////////////////////////////////////////////////   
void Reaction::initializePrecomputeVariable()
   { 
       m1 = fNuclei1->Mass();
       m2 = fNuclei2->Mass();
       m3 = fNuclei3->Mass() + fExcitation3;
       m4 = fNuclei4->Mass() + fExcitation4;
	   
	   s = m1*m1 + m2*m2 + 2*m2*(fBeamEnergy + m1);
	   
	   fTotalEnergyImpulsionCM = TLorentzVector(0,0,0,sqrt(s));
	   
	   ECM_1 = (s + m1*m1 - m2*m2)/(2*sqrt(s));
	   ECM_2 = (s + m2*m2 - m1*m1)/(2*sqrt(s));
	   ECM_3 = (s + m3*m3 - m4*m4)/(2*sqrt(s));
	   ECM_4 = (s + m4*m4 - m3*m3)/(2*sqrt(s));
	   
	   pCM_3 = sqrt(ECM_3*ECM_3 - m3*m3);
	   pCM_4 = sqrt(ECM_4*ECM_4 - m4*m4);
	   
	   fImpulsionLab_1 = TVector3(0,0,sqrt(fBeamEnergy*fBeamEnergy + 2*fBeamEnergy*m1));
	   fImpulsionLab_2 = TVector3(0,0,0);
	   
	   fEnergyImpulsionLab_1 = TLorentzVector(fImpulsionLab_1,m1+fBeamEnergy);
	   fEnergyImpulsionLab_2 = TLorentzVector(fImpulsionLab_2,m2);
	   
	   fTotalEnergyImpulsionLab = fEnergyImpulsionLab_1 + fEnergyImpulsionLab_2;
	   
	   BetaCM = fTotalEnergyImpulsionLab.Beta();
	   
	   fEnergyImpulsionCM_1 = fEnergyImpulsionLab_1;
	   fEnergyImpulsionCM_1.Boost(0,0,-BetaCM);

	   fEnergyImpulsionCM_2 = fEnergyImpulsionLab_2;
	   fEnergyImpulsionCM_2.Boost(0,0,-BetaCM);
}

////////////////////////////////////////////////////////////////////////////////////////////   
void Reaction::SetNuclei3(double EnergyLab, double ThetaLab)
{
	double p3 = sqrt(pow(EnergyLab,2) + 2*m3*EnergyLab);
	
	fEnergyImpulsionLab_3 = TLorentzVector(p3*sin(ThetaLab),0,p3*cos(ThetaLab),EnergyLab+m3);
	fEnergyImpulsionLab_4 = fTotalEnergyImpulsionLab - fEnergyImpulsionLab_3;
	
	fNuclei3->SetEnergyImpulsion(fEnergyImpulsionLab_3);
	fNuclei4->SetEnergyImpulsion(fEnergyImpulsionLab_4);
	
	fThetaCM = EnergyLabToThetaCM(EnergyLab, ThetaLab);
	fExcitation4 = ReconstructRelativistic(EnergyLab, ThetaLab);
}




////////////////////////////////////////////////////////////////////////////////////////////   
TGraph* Reaction::GetKinematicLine3()
{	
	int size = 360; 
	double x[size];
	double y[size];
	double theta3,E3,theta4,E4,Brho;
	
	for (int i = 0; i < size; ++i)
	{
		SetThetaCM(((double)i)/2*deg); 
		KineRelativistic(theta3, E3, theta4, E4);
		fNuclei3->SetKineticEnergy(E3);
		Brho = fNuclei3->GetBrho();
		
		x[i] = theta3/deg; 
		y[i] = E3; 
	}
	TGraph* KineLine3 = new TGraph(size,x,y);
	KineLine3->SetTitle("Kinematic Line of particule 3");
	return(KineLine3);
}



////////////////////////////////////////////////////////////////////////////////////////////   
TGraph* Reaction::GetKinematicLine4()
{	
	int size = 360; 
	double x[size];
	double y[size];
	double theta3,E3,theta4,E4,Brho;

	for (int i = 0; i < size; ++i)
	{
		SetThetaCM(((double)i)/2*deg); 
		KineRelativistic(theta3, E3, theta4, E4);
		fNuclei4->SetKineticEnergy(E4);
		Brho = fNuclei4->GetBrho();
		
		x[i] = theta4/deg; 
		y[i] = E4; 
	}
	TGraph* KineLine4= new TGraph(size,x,y);
	KineLine4->SetTitle("Kinematic Line of particule 4");
	return(KineLine4);
}

////////////////////////////////////////////////////////////////////////////////////////////   
TGraph* Reaction::GetBrhoLine3()
{	
	int size = 360; 
	double x[size];
	double y[size];
	double theta3,E3,theta4,E4;
	double Brho;
	
	for (int i = 0; i < size; ++i)
	{
		SetThetaCM(((double)i)/2*deg); 
		KineRelativistic(theta3, E3, theta4, E4);
		fNuclei3->SetKineticEnergy(E3);
		Brho = fNuclei3->GetBrho();
		
		x[i] = theta3/deg; 
		y[i] = Brho; 
	}
	TGraph* LineBrho3= new TGraph(size,x,y);
	return(LineBrho3);
}

////////////////////////////////////////////////////////////////////////////////////////////   
TGraph* Reaction::GetThetaLabVersusThetaCM()
{
	int size = 360; 
	double x[size];
	double y[size];
	double theta3,E3,theta4,E4;
	
	for (int i = 0; i < size; ++i)
	{
		SetThetaCM(((double)i)/2*deg); 
		KineRelativistic(theta3, E3, theta4, E4);
		
		x[i] = fThetaCM/deg; 
		y[i] = theta3/deg; 
	}
	TGraph* AngleLine= new TGraph(size,x,y);
	return(AngleLine);
}



////////////////////////////////////////////////////////////////////////////////////////////   
void Reaction::PrintKinematic()
{	
	int size = 360; 
	double theta3,E3,theta4,E4,Brho3,Brho4;
	
	cout << endl;
	cout << "*********************** Print Kinematic ***********************" << endl;
	cout << "ThetaCM" << "	" << "ThetaLab" << " " << "EnergyLab3" << "	" << "Brho3" << "	" << "EnergyLab4" << "	" << "Brho4" << endl;
	for (int i = 0; i < size; ++i)
	{
		SetThetaCM(((double)i)/2*deg); 
		KineRelativistic(theta3, E3, theta4, E4);
		
		fNuclei3->SetKineticEnergy(E3);
		Brho3 = fNuclei3->GetBrho();
		
		fNuclei4->SetKineticEnergy(E4);
		Brho4 = fNuclei4->GetBrho();
		
		cout << (double)i/2 << "	" << theta3/deg << "	" << E3 << "	" << Brho3 << "		" << E4 << "	" << Brho4 << endl;
	}
}

















