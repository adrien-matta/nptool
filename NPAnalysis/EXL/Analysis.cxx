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
Reaction_e582->ReadConfigurationFile("40Ca.reaction");

//Get TExlPhysics pointer
TExlPhysics *Exl = (TExlPhysics*)  myDetector -> GetDetector("EXL")	;

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
Exl->ClearEventPhysics();
Exl->BuildPhysicalEvent();

//Get the usefull Data for EXL
if(Exl->EXL_Energy.size()>0)
{
	for(unsigned int countExl = 0 ; countExl < Exl->EXL_Energy.size() ; countExl++)
	{ 
		//Usefull data
		if(Exl->CrystalNumber[countExl]<18)
		{
			EXL_Number.push_back(Exl->CrystalNumber[countExl]);
			EXL_Energy.push_back(Exl->EXL_Energy[countExl]/1000);//Calibration in keV, results in MeV
			//EXL Energy Doppler correction : 1+2->3*+4 and 3*->3+gamma (3 is moving => Doppler effect)
			TVector3 ExlPosition(Exl->GetPositionOfInteraction(Exl->CrystalNumber[countExl]-1));
			P3Vector = TVector3(0,0,1);
			Theta = ExlPosition.Angle(P3Vector);
			Beta = 0.30;
			EXL_Energy_corr.push_back(Exl->DopplerCorrection(Exl->EXL_Energy[countExl]/1000, Theta, Beta));
		}
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

void InitOutputBranch() 
{ 
     	RootOutput::getInstance()->GetTree()->Branch("EXL_Energy_corr", &EXL_Energy_corr,"EXL_Energy_corr/D");
}

void ReInitValue()
{
	EXL_Number.clear();
	EXL_Energy.clear();
	EXL_Energy_corr.clear();
	Theta = -1000;
	Beta = -1000;
}
