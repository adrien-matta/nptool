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

   fNoy1       = new Nucleus();
   fNoy2       = new Nucleus();
   fNoy3       = new Nucleus();
   fNoy4       = new Nucleus();
   fBeamEnergy = 0;
   fThetaCM    = 0;
   fExcitation = 0;
   fQValue     = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Reaction::Reaction(string name1, string name2, string name3, string name4, double BeamEnergy, double ExcitationEnergy,string Path)
{
	SetEveryThing( name1, name2, name3, name4, BeamEnergy, ExcitationEnergy, Path) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void Reaction::SetEveryThing(string name1, string name2, string name3, string name4, double BeamEnergy, double ExcitationEnergy,string Path)
{
   //------------- Constructor with nuclei names and beam energy ------------
     
   fNoy1       = new Nucleus(name1);
   fNoy2       = new Nucleus(name2);
   fNoy3       = new Nucleus(name3);
   fNoy4       = new Nucleus(name4);
   fBeamEnergy = BeamEnergy;
   fThetaCM    = 0;
   fExcitation = ExcitationEnergy;
   fQValue     = (  fNoy1->GetMassExcess() + fNoy2->GetMassExcess()
		  - fNoy3->GetMassExcess() - fNoy4->GetMassExcess()) / 1000;

   int masse  = fNoy1->GetA() + fNoy2->GetA() - fNoy3->GetA() - fNoy4->GetA();
   int charge = fNoy1->GetZ() + fNoy2->GetZ() - fNoy3->GetZ() - fNoy4->GetZ();
   if (masse || charge) cout << "Problem with charge or mass conservation" << endl;
   
   ///Read the differential cross section
   string GlobalPath = getenv("NPTOOL");
   Path = GlobalPath + "/Inputs/CrossSection/" + Path;
   ifstream CSFile;
   CSFile.open( Path.c_str() );
   
   if(CSFile.is_open()) { cout << "Reading Cross Section File " << Path << endl;}
   else {cout << "Cross Section File " << Path << " not found" << endl;return;}

	double CSBuffer,AngleBuffer;
	vector<double> CrossSectionBuffer ;
	
   while(!CSFile.eof())
   {
     CSFile >> AngleBuffer;
     CSFile >> CSBuffer;   
     double CSFinal = CSBuffer*sin(AngleBuffer*deg);
     CrossSectionBuffer.push_back(CSFinal);
   }
  
   CSFile.close();
   CrossSectionSize = CrossSectionBuffer.size();
   CrossSection = new double[CrossSectionSize] ;
   for(int i = 0 ; i <CrossSectionSize ; i++ )	CrossSection[i] = CrossSectionBuffer[i];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Reaction::~Reaction()
{
   //------------- Default Destructor ------------

   delete fNoy1;
   delete fNoy2;
   delete fNoy3;
   delete fNoy4;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void Reaction::KineRelativistic(double &ThetaLab3, double &EnergieLab3,
			        double &ThetaLab4, double &EnergieLab4) const
{
// 2-body relativistic kinematics: direct + inverse
// EnergieLab3,4 : lab energy in MeV of the 2 ejectiles
// ThetaLab3,4   : angles in rad

   double m1 = fNoy1->Mass();
   double m2 = fNoy2->Mass();
   double m3 = fNoy3->Mass();
   double m4 = fNoy4->Mass() + fExcitation;

   // center-of-mass velocity
   double WtotLab = (fBeamEnergy + m1) + m2;
   double P1 = sqrt(pow(fBeamEnergy,2) + 2*m1*fBeamEnergy);
   double B = P1 / WtotLab;
   double G = 1 / sqrt(1 - pow(B,2));

   // total energy of the ejectiles in the center-of-mass
   double W3cm = (pow(WtotLab,2) + pow(G,2)*(pow(m3,2) - pow(m4,2)))
   		   / (2 * G * WtotLab);
   double W4cm = (pow(WtotLab,2) + pow(G,2)*(pow(m4,2) - pow(m3,2)))
   		   / (2 * G * WtotLab);

   // velocity of the ejectiles in the center-of-mass
   double beta3cm  = sqrt(1 - pow(m3,2)/pow(W3cm,2));
   double beta4cm  = sqrt(1 - pow(m4,2)/pow(W4cm,2));
//   double gamma3cm = 1 / sqrt(1 - pow(beta3cm,2));
//   double gamma4cm = 1 / sqrt(1 - pow(beta4cm,2));

   // Constants of the kinematics
   double K3 = B / beta3cm;
   double K4 = B / beta4cm;
   
   // case of inverse kinematics
   double theta = fThetaCM;
   if (m1 > m2) theta = M_PI - fThetaCM;

   // lab angles
   ThetaLab3 = atan(sin(theta) / (cos(theta) + K3) / G);
   if (ThetaLab3 < 0) ThetaLab3 += M_PI;
   ThetaLab4 = atan(sin(M_PI + theta) / (cos(M_PI + theta) + K4) / G);
   if (fabs(ThetaLab3) < 1e-6) ThetaLab3 = 0;
   ThetaLab4 = fabs(ThetaLab4);
   if (fabs(ThetaLab4) < 1e-6) ThetaLab4 = 0;
   
   // total and kinetic energies in the lab
   double W3lab = W3cm * G * (1 + B*beta3cm*cos(theta));
   double W4lab = W4cm * G * (1 + B*beta4cm*cos(theta + M_PI));
   // test for total energy conversion
   if (fabs(WtotLab - (W3lab+W4lab)) > 1e-6) 
      cout << "Problem for energy conservation" << endl;
   EnergieLab3 = W3lab - m3;
   EnergieLab4 = W4lab - m4;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double Reaction::ReconstructRelativistic(double EnergyLab, double ThetaLab) const
	{
		// EnergyLab in MeV
		// ThetaLab in rad

		double m1 = fNoy1->Mass()	;
		double m2 = fNoy2->Mass()	;
		double m3 = fNoy3->Mass()	;
		double m4 = fNoy4->Mass()	;	

		double P1 = sqrt(2*m1*fBeamEnergy+(fBeamEnergy*fBeamEnergy))	;
		double P3 = sqrt(2*m3*EnergyLab+(EnergyLab*EnergyLab))			;
		double P4 = sqrt(P1*P1+P3*P3-(2*P1*P3*cos(ThetaLab)))			;
		double E4 = fBeamEnergy+m1+m2-(EnergyLab+m3)					;
		double m4e = sqrt((E4*E4)-(P4*P4))								;
		double Eex= m4e-m4												;
		
		return Eex;
	}
	
/*double Reaction::ReconstructThetaHeavy(double EnergyLab, double ThetaLab)
	{
		// EnergyLab in MeV
		// ThetaLab in rad

		double m1 = fNoy1->Mass()	;
		double m2 = fNoy2->Mass()	;
		double m3 = fNoy3->Mass()	;
		double m4 = fNoy4->Mass()	;	

		double P1 = sqrt(2*m1*fBeamEnergy+(fBeamEnergy*fBeamEnergy))	;
		double P3 = sqrt(2*m3*EnergyLab+(EnergyLab*EnergyLab))			;
		double P4 = sqrt(P1*P1+P3*P3-(2*P1*P3*cos(ThetaLab)))			;
		
		
			double cosThetaHeavy = (P1 - P4cos(hetaLab))/P3	;

	}*/


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
//Return ThetaCM									
double  Reaction::EnergyLabToThetaCM( double EnergyLab , double ExcitationEnergy = -500) const
	{
		if(ExcitationEnergy == -500) ExcitationEnergy = fExcitation; 
	
		double m1 = fNoy1->Mass()				;
		double m2 = fNoy2->Mass()				;
		double m3 = fNoy3->Mass()				;
		double m4 = (fNoy4->Mass()+ExcitationEnergy)	;
		double E1 = (fBeamEnergy+m1)			;
		double E3 = (EnergyLab+m3)				;
		  
		// Compute Mandelstan variable
		double s =  2*m2*E1 + m1*m1 + m2*m2	;
		double u = -2*m2*E3 + m2*m2 + m3*m3	;
		// Compute CM impulsion:
		//before reaction
		double P2CM =  ( sqrt(  ( s-(m1-m2)*(m1-m2) )*( s-(m1+m2)*(m1+m2) )  ) ) / (2*sqrt(s))	;
		// after reaction
		double P3CM =  ( sqrt(  ( s-(m3-m4)*(m3-m4) )*( s-(m3+m4)*(m3+m4) )  ) ) / (2*sqrt(s))	;

		// Compute CM Energy
		double E2CM = (s + m2*m2 -m1*m1)/(2*sqrt(s))			;
		double E3CM = (s + m3*m3 -m4*m4)/(2*sqrt(s))			;

		double u0 = m2*m2 + m3*m3 - 2*(E2CM*E3CM + P2CM*P3CM)	;

		double Pi = 3.141592654 	;
		double ThetaCM = Pi - acos (  1-(u-u0)/(2*P2CM*P3CM)  )	;

		return(ThetaCM);
	}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void Reaction::Print() const
	{
		// Print informations concerning the reaction

		cout << "Reaction : " << fNoy2->GetName() << "(" << fNoy1->GetName() 
			 << "," << fNoy3->GetName() << ")" << fNoy4->GetName() << "  @  " 
			 << fBeamEnergy << " MeV" 
		<< endl	;
		
		cout << "Exc = " << fExcitation << " MeV" << endl;
		cout << "Qgg = " << fQValue << " MeV" << endl;
	}
	
	
void Reaction::ReadConfigurationFile(string Path)
	{   
		////////General Reading needs////////
		   string LineBuffer;
		   string DataBuffer;

		////////Reaction Setting needs///////
		   string Beam, Target, Heavy, Light, CrossSectionPath ;
		   double BeamEnergy = 0 , ExcitationEnergy = 0 ;		   
		   bool ReadingStatus = false ;
		   bool check_Beam = false ;
		   bool check_Target = false ;
		   bool check_Light = false ;
		   bool check_Heavy = false ;
		   bool check_ExcitationEnergy = false ;
		   bool check_BeamEnergy = false ;
		   bool check_CrossSectionPath = false ;

		   
		//////////////////////////////////////////////////////////////////////////////////////////
		   ifstream ReactionFile;
		   string GlobalPath = getenv("NPTOOL");
 		   Path = GlobalPath + "/Inputs/Reaction/" + Path;
		   ReactionFile.open(Path.c_str());
			
		   if (ReactionFile.is_open()) {cout << "Reading Reaction File " << Path << endl ;}
		   else 
		   	{
		   		cout << "Reaction File " << Path << " not Found! " << endl ;
		      	return;
		   	}

		   while (!ReactionFile.eof()) {
		      //Pick-up next line
		      getline(ReactionFile, LineBuffer);

		      

		      if (LineBuffer.compare(0, 9, "Transfert") == 0) { ReadingStatus = true ;}


		while(ReadingStatus){
		 			
		 			 ReactionFile >> DataBuffer;
		 			 
		 			 //Search for comment Symbol %
			      	 if (LineBuffer.compare(0, 1, "%") == 0) {/* Do Nothing */;}
		 			 
			         else if (DataBuffer.compare(0, 5, "Beam=") == 0) {
			         	check_Beam = true ;
			            ReactionFile >> DataBuffer;
			            Beam = DataBuffer;
			            cout << "Beam " << Beam << endl;
			         }
			
			         else if (DataBuffer.compare(0, 7, "Target=") == 0) {
			            check_Target = true ;
			            ReactionFile >> DataBuffer;
			            Target = DataBuffer;
			            cout << "Target " << Target << endl;
			         }

			         else if (DataBuffer.compare(0, 6, "Light=") == 0) {
			         	check_Light = true ;
			            ReactionFile >> DataBuffer;
			            Light = DataBuffer;
			            cout << "Light " << Light << endl;
			         }

			        else if  (DataBuffer.compare(0, 6, "Heavy=") == 0) {
			            check_Heavy = true ;
			            ReactionFile >> DataBuffer;
			            Heavy = DataBuffer;
			            cout << "Heavy " << Heavy << endl;
			         }

			        else if  (DataBuffer.compare(0, 17, "ExcitationEnergy=") == 0) {
			        	check_ExcitationEnergy = true ;
			            ReactionFile >> DataBuffer;
			            ExcitationEnergy = atof(DataBuffer.c_str()) * MeV;
			            cout << "Excitation Energy " << ExcitationEnergy / MeV << " MeV" << endl;
			         }

			        else if  (DataBuffer.compare(0, 11, "BeamEnergy=") == 0) {
			        	check_BeamEnergy = true ;
			            ReactionFile >> DataBuffer;
			            BeamEnergy = atof(DataBuffer.c_str()) * MeV;
			            cout << "Beam Energy " << BeamEnergy / MeV << " MeV" << endl;
			         }

			        else if  (DataBuffer.compare(0, 17, "CrossSectionPath=") == 0) {
			        	check_CrossSectionPath = true ;
			            ReactionFile >> CrossSectionPath;
			            cout << "Cross Section File: " << CrossSectionPath << endl ;
			         }

					  
		         	///////////////////////////////////////////////////
					//	If no Transfert Token and no comment, toggle out
			         else 
			         	{/*Ignore Token used by G4*/}
			         	
			         ///////////////////////////////////////////////////
					//	If all Token found toggle out
			         if(   	check_Beam && check_Target && check_Light && check_Heavy && check_ExcitationEnergy 
			         	&&  check_BeamEnergy && check_CrossSectionPath )
			         	ReadingStatus = false ;	

				}
			        

			}
		   
		   
		   SetEveryThing(Beam, Target, Light, Heavy,BeamEnergy,ExcitationEnergy,CrossSectionPath);

		   		ReactionFile.close();
	}
	
	
	
	
	
	
	
	
	
	
	
	
