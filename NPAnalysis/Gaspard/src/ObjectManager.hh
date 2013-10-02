// You can use this file to declare your spectra, file, energy loss , ... and whatever you want.
// This way you can remove all unnecessary declaration in the main programm.
// In order to help debugging and organising we use Name Space.


// -----------------------------------DOUBLE, INT, BOOL--------------------------------------------

namespace VARIABLE
	{
	double Xs,Ys,Zs,N			;
	double Theta = 0 			;
	double Ex = 0 				;
	double ThetaCM = 0 			;
	int X,Y						;
	bool check_light = false	;
	
	}
	
using namespace VARIABLE ;
// ----------------------------------------------------------------------------------------------



// -----------------------------------GRAPH------------------------------------------------------
#include <TObject.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2.h>
#include <TH2F.h>
#include <TGraph2D.h>

namespace GRAPH
	{
		//	Declare your Spectra here:
        const int NbElements = 18;
	
			TH2F *hXY1 = new TH2F("hXY1","", 128 , 1 , 128 , 128 , 1 , 128 )				;
			TH2F *hXY2 = new TH2F("hXY2","", 128 , 1 , 128 , 128 , 1 , 128 )				;
			TH2F *hXY3 = new TH2F("hXY3","", 128 , 1 , 128 , 128 , 1 , 128 )				;
			TH2F *hXY4 = new TH2F("hXY4","", 128 , 1 , 128 , 128 , 1 , 128 )				;
			TH2F *hXY5 = new TH2F("hXY5","", 128 , 1 , 128 , 128 , 1 , 128 )				;
			TH2F *hXY6 = new TH2F("hXY6","", 128 , 1 , 128 , 128 , 1 , 128 )				;
                        TH2F *hHitPattern[NbElements];

                        TH1F *hEnerX  = new TH1F("hEnerX",  "Energy X", 1000, -1, 10);
                        TH1F *hEnerY  = new TH1F("hEnerY",  "Energy Y", 1000, -1, 10);
                        TH2F *hEnerXY = new TH2F("hEnerXY", "Energy X vs Y", 100, -1, 10, 100, -1, 10);
                        TH2F *hEnerTheta = new TH2F("hEnerTheta", "Energy v.s. Theta", 90, 0, 180, 400, 0, 100);

			TH2F *hXY    = new TH2F("hXY","", 300 , -130 , 130 , 300 , -130 , 130 )			;
			TH2F *hEA  = new TH2F("hEA" ,"", 120 , 0 , 60 , 260 , 0 , 130 )					;
			TH1F *hEx = new TH1F("hEx", "E* (MeV)", 10000, -10, 40);
			TH1F *hEHexa = new TH1F("hEHexa","EHexa (MeV)", 1000 , -5 , 5 )					;
			TH1F *hThetaCM = new TH1F("hThetaCM","ThetaCM (deg)", 30 , 0 , 60 )				;
			TH1F *hTheta = new TH1F("hTheta","Theta Lab (deg)",180,0,180)					;
			TH2F *hThetaLabCM    = new TH2F("hThetaLabCM","", 90 , 0 , 180 , 90 , 0 , 180 )	;	

		//	Declare your Cut here:

	}

using namespace GRAPH ;
// ----------------------------------------------------------------------------------------------


// -----------------------------------ENERGY LOSS------------------------------------------------
/*
#include "NPEnergyLossCorrection.h"
using namespace NPL ;
namespace ENERGYLOSS
	{
	
//	EnergyLoss ProtonTarget = EnergyLoss 	(	"/home/Adrien/Desktop/dEdX.txt" ,
//												100 							,
//												1								);
	
	}
	
using namespace ENERGYLOSS ;
*/
// ----------------------------------------------------------------------------------------------



