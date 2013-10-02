#include "ConfigurationReader.hh" 
#include <vector>
#include <stdlib.h>
#include "CLHEP/Vector/ThreeVector.h"

using namespace std		;
using namespace CLHEP	;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void ReadConfiguration( string Path , array* myArray)
{

////////General Reading needs////////
	string LineBuffer;
	string DataBuffer;
	
////////MUST2 Detector needs////////
	Hep3Vector A,B,C,D;
	 
	double Theta,Phi,Distance,Beta_u,Beta_v,Beta_w;
	int SI,SILI,CSI,VIS;
	bool check_A=false;bool check_B=false;bool check_C=false;bool check_D=false;
	bool check_Theta=false;bool check_Phi=false;bool check_Distance=false;bool check_Beta=false;
	
	bool MUST2=false;
	bool Target=false;
	bool CryoTarget=false;
	
//////////Target Definition needs///////
	double TargetThickness, TargetRadius;
	
	//for cryogenic target
	
	double WindowsThickness, TargetTemperature, TargetPressure;
	
//////////////////////////////////////////////////////////////////////////////////////////
	ifstream ConfigFile;
	ConfigFile.open(Path.c_str());
	
	if( ConfigFile.is_open() ) 
	cout << " Configuration file " << Path << " loading " << endl;
	else{
	cout << " Error, no configuration file" << Path << " found" << endl; return;}
	
	int i=0;
	while( !ConfigFile.eof() )	
	{
	
		//Pick-up next line
		getline(ConfigFile, LineBuffer); i++;

		//Search for comment Symbol %
		if(LineBuffer.compare(0,1,"%")==0) {;}
		
		//Search for Telescope
		else if(LineBuffer.compare(0,9,"GPDSquare")==0) 
		{
		
		//A MUST2 Telescope is detected:
		
		//Check if it is the first telescope added. If yes, Instantiate a new Array linked to the logical World.
			if(!MUST2)
			{
			//MUST2Array= new MUST2Array(world);
			MUST2=true;			
			}
			
			check_A=false; check_B=false; check_C=false; check_D=false;
			check_Theta=false; check_Phi=false; check_Distance=false;
			cout << "///////////////" << endl;
			cout << "Telescope found" << endl;
			cout << "///////////////" << endl;
			
			ConfigFile >> DataBuffer;
			//Position method
			if(DataBuffer.compare(0,6,"X1_Y1=")==0) 
			{
				check_A=true;
			    ConfigFile >> DataBuffer ; A.setX( atof( DataBuffer.c_str() ) );
				ConfigFile >> DataBuffer ; A.setY( atof( DataBuffer.c_str() ) );
				ConfigFile >> DataBuffer ; A.setZ( atof( DataBuffer.c_str() ) );
				
				cout << "X1 Y1 corner position : " << A << endl; 
			}
			
			//Angle method
			if(DataBuffer.compare(0,6,"THETA=")==0) 
			{
				check_Theta=true;
			    ConfigFile >> DataBuffer ; Theta=atof(DataBuffer.c_str()) ;
				cout << "Theta:  "<< Theta << endl; 
			}
			
			
			ConfigFile >> DataBuffer;
			
			//Position method
			if(DataBuffer.compare(0,8,"X128_Y1=")==0) 
			{
				check_B=true;
			    ConfigFile >> DataBuffer ; B.setX( atof( DataBuffer.c_str() ) );
				ConfigFile >> DataBuffer ; B.setY( atof( DataBuffer.c_str() ) );
				ConfigFile >> DataBuffer ; B.setZ( atof( DataBuffer.c_str() ) );
		
				cout << "X128 Y1 corner position : " << B << endl; 
			}
			
			//Angle method
			if(DataBuffer.compare(0,4,"PHI=")==0) 
			{
				check_Phi=true;
			    ConfigFile >> DataBuffer ; Phi=atof(DataBuffer.c_str()) ;
				cout << "Phi:  "<< Phi << endl; 
			}
			
			ConfigFile >> DataBuffer;
			
			//Position method
			if(DataBuffer.compare(0,8,"X1_Y128=")==0) 
			{
				check_C=true;
				ConfigFile >> DataBuffer ; C.setX( atof( DataBuffer.c_str() ) );
				ConfigFile >> DataBuffer ; C.setY( atof( DataBuffer.c_str() ) );
				ConfigFile >> DataBuffer ; C.setZ( atof( DataBuffer.c_str() ) );

				cout << "X1 Y128 corner position : " << C << endl; 
			}
			
			//Angle method
			if(DataBuffer.compare(0,2,"R=")==0) 
			{
				check_Distance=true;
			    ConfigFile >> DataBuffer ; Distance=atof(DataBuffer.c_str()) ;
			    cout << "Distance:  "<< Distance << endl; 
			}
			
			ConfigFile >> DataBuffer;
			//Position method
			if(DataBuffer.compare(0,10,"X128_Y128=")==0) 
			{
				check_D=true;
				ConfigFile >> DataBuffer ; D.setX( atof( DataBuffer.c_str() ) );
				ConfigFile >> DataBuffer ; D.setY( atof( DataBuffer.c_str() ) );
				ConfigFile >> DataBuffer ; D.setZ( atof( DataBuffer.c_str() ) ); 
				
				cout << "X128 Y128 corner position : " << D << endl;  
			}
			
			//Angle method
			if(DataBuffer.compare(0,5,"BETA=")==0) 
			{
				check_Beta=true;
			    ConfigFile >> DataBuffer ; Beta_u=atof(DataBuffer.c_str()) ;
			    ConfigFile >> DataBuffer ; Beta_v=atof(DataBuffer.c_str()) ;
			    ConfigFile >> DataBuffer ; Beta_w=atof(DataBuffer.c_str()) ;
				cout << "Beta:  "<< Beta_u << Beta_v << Beta_w << endl; 
			}
			
			ConfigFile >> DataBuffer;
			if(DataBuffer.compare(0,10,"FIRSTSTAGE=")==0) 
			{
			ConfigFile >> DataBuffer;SI=atof(DataBuffer.c_str()) ;
			}
			
			ConfigFile >> DataBuffer;
			if(DataBuffer.compare(0,11,"SECONDSTAGE=")==0) 
			{
			ConfigFile >> DataBuffer;SILI=atof(DataBuffer.c_str()) ;
			}
			
			
			ConfigFile >> DataBuffer;
			if(DataBuffer.compare(0,10,"THIRDSTAGE=")==0) 
			{
			ConfigFile >> DataBuffer;CSI=atof(DataBuffer.c_str()) ;
			}
					
			
			///Add The previously define telescope
			
			//With position method
			if(check_A && check_B && check_C && check_D)
			{
				myArray->addTelescope( A  , 
									   B  , 
									   C  , 
									   D  );
									   
			}
					
			//with angle method
			if(check_Theta && check_Phi && check_Distance && check_Beta)
			{					   
				myArray->addTelescope(  Theta 		, 
									    Phi 		, 
									    Distance	,
									    Beta_u		,
									    Beta_v		,
									    Beta_w		);
									   
			}
			
			
		}
				
		//Nothing understandable
		else ;
		
	
	}
	
ConfigFile.close();
return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
Reaction* ReadReaction( string Path )
{
////////General Reading needs////////
	string LineBuffer;
	string DataBuffer;
	
////////Reaction Setting needs///////
string Beam,Target,Heavy,Light,CrossSectionPath;
double BeamEnergy,ExcitationEnergy;
Reaction* myReaction;

//////////////////////////////////////////////////////////////////////////////////////////
	ifstream ReactionFile;
	ReactionFile.open(Path.c_str());
	
	if( ReactionFile.is_open() ) 
	cout << " Reaction file " << Path << " loading " << endl;
	else{
	cout << " Error, no Reaction file" << Path << " found" << endl; return myReaction;}
	
	
	int i=0;
	while( !ReactionFile.eof() )	
	{
		//Pick-up next line
		getline(ReactionFile, LineBuffer); i++;

		//Search for comment Symbol %
		if(LineBuffer.compare(0,1,"%")==0) {;}
		
		else if(LineBuffer.compare(0,9,"Transfert")==0)  
		{
		
			ReactionFile >> DataBuffer;
			if(DataBuffer.compare(0,5,"Beam=")==0) 
				{
				ReactionFile >> DataBuffer;
				Beam = DataBuffer;
				cout << "Beam " << Beam << endl;
				}
			
			ReactionFile >> DataBuffer;
			if(DataBuffer.compare(0,7,"Target=")==0) 
				{
				ReactionFile >> DataBuffer;
				Target = DataBuffer;
				cout << "Target " << Target << endl;
				}
				
			ReactionFile >> DataBuffer;
			if(DataBuffer.compare(0,6,"Light=")==0) 
				{
				ReactionFile >> DataBuffer;
				Light = DataBuffer;
				cout << "Light " << Light << endl;
				}
			
			ReactionFile >> DataBuffer;
			if(DataBuffer.compare(0,6,"Heavy=")==0) 
				{
				ReactionFile >> DataBuffer;
				Heavy = DataBuffer;
				cout << "Heavy " << Heavy << endl;
				}
			
			ReactionFile >> DataBuffer;
			if(DataBuffer.compare(0,17,"ExcitationEnergy=")==0) 
				{
				ReactionFile >> DataBuffer;
				ExcitationEnergy = atof(DataBuffer.c_str());
				cout << "ExcitationEnergy " << ExcitationEnergy << " MeV" << endl;
				}
				
			ReactionFile >> DataBuffer;
			if(DataBuffer.compare(0,11,"BeamEnergy=")==0) 
				{
				ReactionFile >> DataBuffer;
				BeamEnergy = atof(DataBuffer.c_str());
				cout << "EnergyBeam " << BeamEnergy << " MeV" << endl;
				}
				
			ReactionFile >> DataBuffer;ReactionFile >> DataBuffer;
			ReactionFile >> DataBuffer;ReactionFile >> DataBuffer;
			ReactionFile >> DataBuffer;ReactionFile >> DataBuffer;
			ReactionFile >> DataBuffer;ReactionFile >> DataBuffer;
			ReactionFile >> DataBuffer;ReactionFile >> DataBuffer;
		
				
			ReactionFile >> DataBuffer;
			if(DataBuffer.compare(0,17,"CrossSectionPath=")==0) 
				{
				ReactionFile >> CrossSectionPath;
				cout << "CrossSectionPath " << CrossSectionPath << endl;
				}
		}
	}
	
myReaction = new  Reaction(Beam, Target, Light, Heavy,BeamEnergy,ExcitationEnergy,CrossSectionPath);

ReactionFile.close();
return myReaction;

}

