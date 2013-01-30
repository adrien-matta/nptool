#include "Analysis.h"
using namespace std;

int main(int argc, char** argv)
{

/*---------------------------------Initialisation---------------------------------------*/

//Command line parsing
NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);

//Instantiate RootInput
string runToReadfileName = myOptionManager->GetRunToReadFile();
RootInput:: getInstance(runToReadfileName);

//If input files are not given, use those from TAsciiFile
if(myOptionManager->IsDefault("DetectorConfiguration")) 
{
	string name = RootInput::getInstance()->DumpAsciiFile("DetectorConfiguration");
	myOptionManager->SetDetectorFile(name);
}

//Get input files from NPOptionManager
string detectorfileName = myOptionManager->GetDetectorFile();
string reactionfileName    = myOptionManager->GetReactionFile();
string calibrationfileName = myOptionManager->GetCalibrationFile();
string OutputfileName = myOptionManager->GetOutputFile();
   
//Instantiate RootOutput
RootOutput::getInstance("Analysis/"+OutputfileName, "AnalysedTree");
   
//Instantiate the detector using a file
NPA::DetectorManager* myDetector = new DetectorManager();
myDetector->ReadConfigurationFile(detectorfileName);

//Get the formed Chained Tree and Treat it
TChain* Chain = RootInput:: getInstance() -> GetChain();

//Attach new branch
InitOutputBranch();

//Instantiate some Reaction
NPL::Reaction* Reaction_e582 = new Reaction;
Reaction_e582->ReadConfigurationFile(reactionfileName);

//Get TSpegPhysics pointer
TSpegPhysics *Speg = (TSpegPhysics*)  myDetector -> GetDetector("SPEG")	;

/*----------------------------------Reading events-------------------------------*/

//Get number of events to treat
cout << endl << "///////// Starting Analysis ///////// "<< endl;
int nentries = Chain->GetEntries();
cout << " Number of Event to be treated : " << nentries << endl;
clock_t begin = clock();
clock_t end = begin;

//Main loop on entries
for (int i=0; i<nentries; i++) 
{
	if(i%50000==0 && i!=0)  
	{
		cout.precision(5);
		end = clock();
		double TimeElapsed = (end-begin) / CLOCKS_PER_SEC;
		double percent = (double)i/nentries;
		double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
 		cout <<"        "<< flush;
		cout << "\r Progression : " << percent*100 << " % \t | \t Remaining time : ~" <<  TimeToWait <<"s"<< flush;
	}
	else if(i == nentries-1) 
	{ 
		cout << "\r Progression : " << " 100% " <<endl;
	}

//Get data
ReInitValue();  
Chain -> GetEntry(i);                             

//Make Physical Tree
Speg->ClearEventPhysics();
Speg->BuildPhysicalEvent();

//Get the usefull Data for SPEG
mass1 = Reaction_e582->GetNucleus1()->Mass();
mass2 = Reaction_e582->GetNucleus2()->Mass();
mass3 = Reaction_e582->GetNucleus3()->Mass();
mass4 = Reaction_e582->GetNucleus4()->Mass();
T1 = Reaction_e582->GetBeamEnergy();
if(Speg->X_FocalPlan.size()==1 && Speg->Phi_FocalPlan.size() == Speg->X_FocalPlan.size())
{
	for(unsigned int countSpeg = 0 ; countSpeg < Speg->X_FocalPlan.size() ; countSpeg++)
	{
		//Usefull data
		Thetafoc = Speg->Theta_FocalPlan[countSpeg];
		Phifoc = Speg->Phi_FocalPlan[countSpeg];
		Time_Plastic_Right = Speg->Time_Plastic_Right;
		Khi2 = Speg->Khi2;
		Z = Speg->Z[countSpeg];
		M_over_Q = Speg->M_over_Q[countSpeg];
		ThetaSPEG = acos(cos(Thetafoc*Pi/180)*cos(Phifoc*Pi/180));
		PhiSPEG = atan2(tan(Phifoc*Pi/180),sin(Thetafoc*Pi/180));
		ExcitationEnergy_4 = Speg->X_FocalPlan[countSpeg];

		//Nucleus3 kinetic energy calculation
		Energy_3 = GetSPEG_Energy(ExcitationEnergy_4,ThetaSPEG);

		//Some Physics results
		GetPhysicsResults(Energy_3, ThetaSPEG, PhiSPEG, ExcitationEnergy_4);
	}
}

//Fill Physical Tree
RootOutput::getInstance()->GetTree()->Fill();
}

cout << "A total of " << nentries << " event has been analysed " << endl ;

RootOutput::getInstance()->Destroy();
RootInput::getInstance()->Destroy();
NPOptionManager::getInstance()->Destroy();

return 0;
}

//Reaction : 1+2->3+4 (1=beam, 2=target=40Ca, 3=Nucleus in SPEG, 4=Excited Target)
//Nucleus3 kinetic energy calculation
double GetSPEG_Energy(double E4ex, double ThetaSPEG)
{
	A = pow(T1+mass1+mass2,2)+(-(T1+mass1)*(T1+mass1)+mass1*mass1)*cos(ThetaSPEG)*cos(ThetaSPEG);
	Q = (mass4+E4ex)*(mass4+E4ex)-mass3*mass3-mass2*mass2-mass1*mass1;
	B = Q*(T1+mass1+mass2)-2*(T1+mass1)*mass2*(T1+mass1+mass2);
	C = Q*Q/4+(T1+mass1)*(T1+mass1)*mass2*mass2-(T1+mass1)*mass2*Q+((T1+mass1)*(T1+mass1)*mass3*mass3-mass1*mass1*mass3*mass3)*cos(ThetaSPEG)*cos(ThetaSPEG);
	delta = B*B-4*A*C;
	T3 = (-B+sqrt(delta))/(2*A)-mass3;
	return(T3);
}

//Reaction : 1+2->3+4 (1=beam, 2=target=40Ca, 3=Nucleus in SPEG, 4=Excited Target)
void GetPhysicsResults(double T3, double ThetaSPEG, double PhiSPEG, double E4ex)
{
	//Analytic relativistic kinematic calculation
	P1 = sqrt(2*mass1*T1+T1*T1);
	P1Vector = TVector3(0,0,P1);
	P3 = sqrt(2*mass3*T3+T3*T3);
	P3Vector = TVector3(-P3*cos(PhiSPEG)*sin(ThetaSPEG),P3*sin(PhiSPEG)*sin(ThetaSPEG),P3*cos(ThetaSPEG));//The SPEG coordinate is different to the NPTOOL coordinate
	P4Vector = P1Vector-P3Vector;
	P4 = P4Vector.Mag();
	T4 = sqrt(mass4*mass4+P4*P4)-mass4;
	Theta_4 = P4Vector.Angle(P1Vector);
	Phi_4 = P4Vector.Phi();
}

void InitOutputBranch() 
{ 
     	RootOutput::getInstance()->GetTree()->Branch("Energy_3", &Energy_3,"Energy_3/D");
     	RootOutput::getInstance()->GetTree()->Branch("ThetaSPEG", &ThetaSPEG,"ThetaSPEG/D");
     	RootOutput::getInstance()->GetTree()->Branch("PhiSPEG", &PhiSPEG,"PhiSPEG/D");
	RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy_4", &ExcitationEnergy_4,"ExcitationEnergy_4/D");
     	RootOutput::getInstance()->GetTree()->Branch("Energy_4", &T4,"T4/D");
     	RootOutput::getInstance()->GetTree()->Branch("Theta_4", &Theta_4,"Theta_4/D");
     	RootOutput::getInstance()->GetTree()->Branch("Phi_4", &Phi_4,"Phi_4/D");
}

void ReInitValue()
{
	Thetafoc = -1000;
	Phifoc = -1000;
	M_over_Q = -1000;
	Z = -1000;
	ThetaSPEG = -1000;
	PhiSPEG = -1000;
	Time_Plastic_Right = -1000;
	Khi2 = -1000;
	Energy_3 = -1000;
	ExcitationEnergy_4 = -1000;
	Energy_4 = -1000;
	Theta_4 = -1000;
	Phi_4 = -1000;
	A = -1000;
	B = -1000;
	C = -1000;
	Q = -1000;
	delta = -1000;
	T3 = -1000;
	P3 = -1000;
	T4 = -1000;
	P4 = -1000;
	P1 = -1000;
}
