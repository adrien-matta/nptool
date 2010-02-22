/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Sandra Giron  contact address: giron@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2010                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold CATS treated data                                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TRandom.h"
#include "TCATSPhysics.h"
#include <cmath>
#include <algorithm>



ClassImp(TCATSPhysics)

  TCATSPhysics::TCATSPhysics()
{
  EventData 					= new TCATSData	;
	EventPhysics 				= this						;
}



TCATSPhysics::~TCATSPhysics()
{
}


////////////////////////////////////////// A SANDRA DE JOUER! ////////////////////////////////////////

			
//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void ReadConfiguration(string) 				
 	{
	}

//	Add Parameter to the CalibrationManger
void AddParameterToCalibrationManager()	
 	{
 	// voir MUST2
	}		
		
//	Activated associated Branches and link it to the private member DetectorData address
//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void InitializeRootInput() 					
 	{
 	// voir MUST2
	}

//	Create associated branches and associated private member DetectorPhysics address
void InitializeRootOutput() 		 		
 	{
 	// voir MUST2
	}
		
//	This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
void BuildPhysicalEvent()					
 	{
 	// voir les commentaire fait la ou la methode existe deja
	}
		
//	Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
//	This method aimed to be used for analysis performed during experiment, when speed is requiered.
//	NB: This method can eventually be the same as BuildPhysicalEvent.
void BuildSimplePhysicalEvent()				
 	{
 	// pourquoi pas une methode avec qui prend que le strip max par exemple...
	}



////////////////////////////////////////// LE RESTE DE LA CLASSE! ////////////////////////////////////////

void TCATSPhysics::Clear()
{  
  DetNumberX.clear()				; 
  StripX.clear()				;
  ChargeX.clear()                		; 
  ChargeSumX.clear()				;
  MultOverThreshX.clear()  		        ;  
  StripMaxX.clear()				;
  DetNumberY.clear()				; 
  StripY.clear()				;
  ChargeY.clear()				; 
  //  ChargeY_test.clear()				; 
  ChargeSumY.clear()				;  
  MultOverThreshY.clear()			;
  StripMaxY.clear() 				;
  //  StripMaxY_test.clear() 			;
  DetNumberX_Position.clear()                   ;
  DetNumberY_Position.clear()                   ;
  DetNumberZ_Position.clear()                   ;
  PositionX.clear() 				;
  PositionY.clear() 				;
  PositionZ.clear() 				;

  ReconstructionMethodX.clear()                 ;
  ReconstructionMethodY.clear()                 ;

  HitX = 0;
  HitY = 0;
}



void TCATSPhysics::Dump()
{
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
}

/*
Pas mal de chose a modifier:
- supprimer l'histoire des calibration par exemple...
- TCATSData doit etre un membre prive de la classe (non ecrit dans l'arbre de sorti), vois ce que j'ai fait sur MUST2
- Les positions doivent aussi etre des membre prive non ecrit, comme dans MUST2
- N'oublie pas que la methode ne doit plus avoir d'argument a la fin... (et qu'elle est deja declare plus haut...)
*/
void TCATSPhysics::BuildSimplePhysicalEvent( TCATSData* Data				, 
					     vector< vector <double> > 			&Ped_X 		,
					     vector< vector <double> > 			&Ped_Y 		,
					     vector< vector< vector<double> > > 	&OnlineCalib_X_E,
					     vector< vector< vector<double> > > 	&OnlineCalib_Y_E,	
					      vector< vector <double> > 		&Thresh_X 	,
					     vector< vector <double> > 			&Thresh_Y 	,
					     vector< vector< vector<double> > >  &StripPositionX  	,
					     vector< vector< vector<double> > >  &StripPositionY   	,
					     vector<double>                      &StripPositionZ   	)
{
  /*
  int 	        HitX 	       = 0 	;
  int 	        HitY 	       = 0 	;
  */    

  gRandom->SetSeed(0);

  //Data->Dump();
  double CalculatedStripX, CalculatedStripY ;
  double posX =0 , posY = 0;
  

  //	How many CATS?
  int NumberOfDetector = 0 ;
  int DetectorID = -1;
  
  for( ushort i = 0 ; i < Data->GetCATSMultX() ; i++ )
    { 
      //  if( NumberOfDetector < Data->GetCATSDetX(i) ) NumberOfDetector = Data->GetCATSDetX(i)	;   //determination of the number of CATS detectors

      if( Data->GetCATSDetX(i) != DetectorID)  {
	NumberOfDetector++;
      }

      if(NumberOfDetector == 2) break;	

    }

  
  // cout << "Nombre de CATS: " << NumberOfDetector << endl;


  // INITIALISATION OF VECTORS
  for(int ff = 0 ; ff < NumberOfDetector ; ff++ )
    {
      MultOverThreshX.push_back(-1); 
      StripMaxX.push_back(-1); 
      ChargeSumX.push_back(-1); 
      ReconstructionMethodX.push_back(NO);

      MultOverThreshY.push_back(-1); 
      StripMaxY.push_back(-1); 
      //   StripMaxY_test.push_back(-1); 
      ChargeSumY.push_back(-1);
      ReconstructionMethodY.push_back(NO);
    }
  //  cout << "Init OK" << endl;


  
  for(int gg = 0 ; gg < NumberOfDetector ; gg++ )
    { 
      //int    ff = NumberOfDetector - gg -1 ;
      int ff = gg ;
           
      CalculatedStripX = AnalyseX(Data, Ped_X , OnlineCalib_X_E, Thresh_X, StripPositionX, ff, NumberOfDetector);        //     cout << "Analyse X = " << CalculatedStripX << endl; 
      CalculatedStripY = AnalyseY(Data, Ped_Y , OnlineCalib_Y_E, Thresh_Y, StripPositionY, ff, NumberOfDetector);        //    cout << "Analyse Y = " << CalculatedStripY << endl;

      posX = CalculatePositionX(StripPositionX, StripMaxX[ff], Chargex, CalculatedStripX, ff, cor);   // cout << "Position X = " << posX << endl;
      posY = CalculatePositionY(StripPositionY, StripMaxY[ff], Chargey, CalculatedStripY, ff, cor);   // cout << "Position Y = " << posY << endl;

      DetNumberX_Position.push_back(ff+1);
      DetNumberY_Position.push_back(ff+1);
      DetNumberZ_Position.push_back(ff+1);
      
      PositionX.push_back(posX) ;                  
      PositionY.push_back(posY) ;                  
      PositionZ.push_back(StripPositionZ[ff]) ;
    }
	
 
  if(NumberOfDetector>1)
    { 
      /*
      cout << "PositionX[0]= " <<PositionX[0] << " PositionX[1] = " << PositionX[1] << " PositionX[0]-PositionX[1]= " << PositionX[0]-PositionX[1] << endl;
      cout << "PositionY[0]= " <<PositionY[0] << " PositionY[1] = " << PositionY[1] << " PositionY[0]-PositionY[1]= " << PositionY[0]-PositionY[1] << endl;
      cout << "PositionZ[0] = "<< PositionZ[0] << " PositionZ[1] = "<< PositionZ[1] << endl;
      */

      if(PositionX[0] != -40 && PositionY[0] != -40 && PositionX[1] != -40 && PositionY[1] != -40) {
	
	BeamDirection = TVector3 (PositionX[1]-PositionX[0] ,
				  PositionY[1]-PositionY[0] ,
				  PositionZ[1]-PositionZ[0] );
	//	BeamDirection.Unit(); Adrien
	
	double l = sqrt((PositionZ[0]-PositionZ[1])*(PositionZ[0]-PositionZ[1]));
	double L = - PositionZ[1];

	double t = (l+L) / l;
	
	//double t = - PositionZ[0] / BeamDirection.Z()  ;  Adrien
	//cout << "t = " << t << endl;
	
	/* Adrien stuff
	PositionOnTargetX = PositionX[0] + BeamDirection.X()*t;
	PositionOnTargetY = PositionY[0] + BeamDirection.Y()*t;
	*/
	
	PositionOnTargetX = PositionX[0] + (PositionX[1] - PositionX[0]) * t;
	PositionOnTargetY = PositionY[0] + (PositionY[1] - PositionY[0]) * t;

	BeamDirection.Unit();
			
	/* Pierre
	PositionOnTargetX = PositionX[1] + (PositionX[1] - PositionX[0]) * L /l;
	PositionOnTargetY = PositionY[1] + (PositionY[1] - PositionY[0]) * L /l;
	*/
      }

      else
	{
	  BeamDirection = TVector3 ( 	1 ,
					0 ,
					0 );
	  PositionOnTargetX = 3000	;
	  PositionOnTargetY = 3000	;
	}
    }
  
  else
    {
      BeamDirection = TVector3 ( 	1 ,
					0 ,
					0 );
      PositionOnTargetX = 5000	;
      PositionOnTargetY = 5000	;
    }
}


double TCATSPhysics::AnalyseX(TCATSData* Data, 
			      vector< vector <double> > 	        &Ped_X 		,
			      vector< vector< vector<double> > > 	&OnlineCalib_X_E,
			      vector< vector <double> > 		&Thresh_X 	,
			      vector< vector< vector<double> > >        &StripPositionX,
			      int ff,
			      int NumberOfDetector)
{
  // cout << "AnalyseX proccessing ..." << endl;

  // double	Chargex[28];
  for(ushort z=0; z<28; z++) {
    Chargex[z] = -1;
  }

  //  int HitX = 0;
  double ChargeSum_X = 0 ;
  double ChargeX_Buffer = 0;
  double CalculatedStripX=0;

  for(UShort_t i =0; i<Data->GetCATSMultX(); i++)
    {
      // cout <<  Data->GetCATSDetX(i)<< endl;  
      //cout <<  Data->GetCATSStripX(i)<< endl;  
      // cout << "ff+1 = " << ff+1 << endl;

      if( Data->GetCATSDetX(i) == ff+1 )
	{
	  int NX = Data->GetCATSDetX(i);   
	  int StrX = Data->GetCATSStripX(i) ;   // cout << NX << " " << StrX << endl ;
	  
	  if(NX > 2 || StrX > 28)	cout << NX << " " << StrX << endl ;
	  
	  double Q = Data->GetCATSChargeX(i) + gRandom->Rndm() - Ped_X[NX-1][StrX-1] ;
	  ChargeX_Buffer = OnlineCalib_X_E[NX-1][StrX-1][0] + Q * OnlineCalib_X_E[NX-1][StrX-1][1] + Q*Q * OnlineCalib_X_E[NX-1][StrX-1][2] ;
	
	  if(Data->GetCATSChargeX(i) > Thresh_X[NX-1][StrX-1] && NX <= NumberOfDetector  && StrX < 28)
	    {  
	      //  cout <<  Thresh_X[NX-1][StrX-1] << endl;

	      //	KNOWN INVERSION
	      if	(StrX == 15 && NX == 1) 	StrX = 16	;
	      else if (StrX == 16 && NX == 1) 		StrX = 15 	;
	      
	      MultOverThreshX[ff]++;
	      ChargeSum_X += ChargeX_Buffer;    
	      //ChargeSum += ChargeX_Buffer;
	      ChargeX.push_back( ChargeX_Buffer );   //  cout << "ChargeX_Buffer = " << ChargeX_Buffer << endl;
	      Chargex[StrX-1] = ChargeX_Buffer ;    // cout <<" Chargex[" << StrX-1 << "] " << Chargex[StrX-1] << endl;
	      StripX.push_back(StrX);
	      DetNumberX.push_back(NX) ;
	      HitX++;

	      //if ( ChargeX_Buffer > Chargex[ StripMaxX[ff] -1 ] ) StripMaxX[ff] = StrX ;      
	      if ( ChargeX[HitX-1] > Chargex[ StripMaxX[ff] -1 ] ) StripMaxX[ff] = StrX ;      
	      //cout << "X " << ff+1 << " " << StrX << " " << StripMaxX[ff] << " " << HitX-1 << " " << ChargeX[HitX-1] << " " << StripMaxX[ff]-1 << " " << Chargex[StripMaxX[ff]-1] << endl;
	      //   cout << "X " << ff+1 << " " << StrX << " " << StripMaxX[ff] << " " << HitX-1 << " " << ChargeX_Buffer << " " << StripMaxX[ff]-1 << " " << Chargex[StripMaxX[ff]-1] << endl;
	    }
	 
	}

      ChargeSumX[ff] = ChargeSum_X;
    }

  //cout << "Charge x = " << Chargex << " StripMaxX = " << StripMaxX[ff] << endl;

  ReconstructionMethodX[ff] = ChooseReconstruction(ff, 0, Chargex, StripMaxX[ff]);
  //if (ff==1) cout << "StripMaxX = " << StripMaxX[ff] << endl;
  //cout << ReconstructionMethodX[ff] << endl;

  if(ReconstructionMethodX[ff] == BAR5) CalculatedStripX = Barycentric5Method( Chargex, StripMaxX[ff] );
  if(ReconstructionMethodX[ff] == BAR3) CalculatedStripX = Barycentric3Method( Chargex, StripMaxX[ff] );
  if(ReconstructionMethodX[ff] == BAR4) CalculatedStripX = Barycentric4Method( Chargex, StripMaxX[ff] );
  if(ReconstructionMethodX[ff] == SECHS)CalculatedStripX = HyperbolicSequentMethod( Chargex, StripMaxX[ff] ); 
  if(ReconstructionMethodX[ff] == GAUSS)CalculatedStripX = GaussianMethodX(ff, Chargex, StripMaxX[ff], StripPositionX);
  
  // else cout << "Error in the choice of the method!" << endl;
  
  //  cout << "AnalyseX done!" << endl ;
  return(CalculatedStripX);
}

double TCATSPhysics::AnalyseY(TCATSData* Data, 
			      vector< vector <double> > 	        &Ped_Y 		,
			      vector< vector< vector<double> > > 	&OnlineCalib_Y_E,
			      vector< vector <double> > 		&Thresh_Y 	,
			      vector< vector< vector<double> > >        &StripPositionY,
			      int ff,
			      int NumberOfDetector)
{


  //  cout << "AnalyseY proccessing ..." << endl;

  //  double	Chargey[28];
  for(ushort z=0; z<28; z++) {
    Chargey[z] = -1;
    //    Chargey_test[z] = -1;
  }
 
  // int HitY = 0;

  double ChargeSum_Y = 0 ;
  double ChargeY_Buffer = 0;

  double CalculatedStripY=0;
  
  for(UShort_t i =0; i<Data->GetCATSMultY(); i++)
    {
      if( Data->GetCATSDetY(i) == ff+1 )
	{ 
	  int NY	 = Data	->	GetCATSDetY(i);
	  int StrY = Data	->	GetCATSStripY(i) ;//	cout << NY << endl ; //" " << StrY << endl ;
	  
	  if(NY > 2 || StrY > 32)	cout << NY << " " << StrY << endl ;
	  
	  if(StrY < 28)
	    {	
	      double Q = Data->GetCATSChargeY(i) + gRandom->Rndm() - Ped_Y[NY-1][StrY-1];
	      ChargeY_Buffer = OnlineCalib_Y_E[NY-1][StrY-1][0] + Q * OnlineCalib_Y_E[NY-1][StrY-1][1] + Q*Q * OnlineCalib_Y_E[NY-1][StrY-1][2] ;
	    }
	  
	  else {ChargeY_Buffer = 0 ;}
	  
	  if(Data->GetCATSChargeY(i) > Thresh_Y[NY-1][StrY-1] && NY <= NumberOfDetector  && StrY < 28)
	    {
	      //   cout <<  Thresh_Y[NY-1][StrY-1] << endl;
	      
	      //	KNOWN INVERSION
	      if	(StrY == 15 && NY == 1) 	StrY = 16	;
	      else if (StrY == 16 && NY == 1) 		StrY = 15 	;
	      //else if (StrY == 4 && NY == 2) 		StrY = 14 	;
	      //else if (StrY == 14 && NY == 2) 	StrY = 4 	;
	      
	      MultOverThreshY[ff]++;
	      //ChargeSum += ChargeY_Buffer;
	      ChargeSum_Y += ChargeY_Buffer;
	      
	      /*
	      if(ff ==1 && StrY ==15) {
		//	ChargeY_test.push_back( ChargeY_Buffer );   
		//		Chargey_test[StrY-1] = ChargeY_Buffer ;  
		ChargeY.push_back( -1 );   
		Chargey[StrY-1] = -1 ;   
	      }
	      */
	      //else{
		//		ChargeY_test.push_back( ChargeY_Buffer );   
		//		Chargey_test[StrY-1] = ChargeY_Buffer ;  
		ChargeY.push_back( ChargeY_Buffer );   //   cout << "ChargeY_Buffer = " << ChargeY_Buffer << endl;
		Chargey[StrY-1] = ChargeY_Buffer ;   // cout <<" Chargey[" << StrY-1 << "] " << Chargey[StrY-1] << endl;
		//      }
	      
	      StripY.push_back(StrY);
	      DetNumberY.push_back(NY) ;
	      HitY++;
	      
	      if (ChargeY[HitY-1] > Chargey[ StripMaxY[ff] -1  ]) StripMaxY[ff] = StrY;  // stripmax si pas de piste supprimee
	      	      
	      //  if (ChargeY_test[HitY-1] > Chargey_test[ StripMaxY_test[ff] -1  ]) StripMaxY_test[ff] = StrY;  // stripmax si pas de piste supprimee
	      
	      //if (ChargeY_Buffer > Chargey[ StripMaxY[ff] -1  ]) StripMaxY[ff] = StrY; 
	      //cout << "Y " << ff+1 << " " << StrY << " " << StripMaxY[ff] << " " << HitY-1 << " " << ChargeY[HitY-1] << " " << StripMaxY[ff]-1 << " " << Chargey[StripMaxY[ff]-1] << endl;
	      //cout << "Y " << ff+1 << " " << StrY << " " << StripMaxY[ff] << " " << HitY-1 << " " << ChargeY_Buffer << " " << StripMaxY[ff]-1 << " " << Chargey[StripMaxY[ff]-1] << endl;
	    }
	}
      //ChargeSumY[ff] = ChargeSum;
      ChargeSumY[ff] = ChargeSum_Y;
    }
  
  //  cout << "Charge y = " << Chargey << " StripMaxY = " << StripMaxY[ff]  << endl;

  ReconstructionMethodY[ff] = ChooseReconstruction(ff, 1, Chargey, StripMaxY[ff]);
  //if (ff==1) cout << "StripMaxY = " << StripMaxY[ff] << endl;
  //  cout << ReconstructionMethodY[ff] << endl;


  if(ReconstructionMethodY[ff] == BAR5) CalculatedStripY = Barycentric5Method( Chargey, StripMaxY[ff] );
  if(ReconstructionMethodY[ff] == BAR3) CalculatedStripY = Barycentric3Method( Chargey, StripMaxY[ff] );
  if(ReconstructionMethodY[ff] == BAR4) CalculatedStripY = Barycentric4Method( Chargey, StripMaxY[ff] );
  if(ReconstructionMethodY[ff] == SECHS)CalculatedStripY = HyperbolicSequentMethod( Chargey, StripMaxY[ff] ); 
  if(ReconstructionMethodY[ff] == GAUSS)CalculatedStripY = GaussianMethodY(ff, Chargey, StripMaxY[ff], StripPositionY);

  // else cout << "Error in the choice of the method!" << endl;
  
  //  cout << "AnalyseY done!" << endl;
  return(CalculatedStripY);
}

reconstruction TCATSPhysics::ChooseReconstruction(int ff, int type, double * charge, int StripMax)
{
  // if(ff ==1 && type ==1)  cout << StripMax << endl; 
  reconstruction method;
  if(ff ==0) { method = SECHS; }  //cout << "sechs" << endl; }
  //else method = GAUSS;
 
  
  else { 
    if(type == 1)  
      {
	method = GAUSS;
      } // cout << "bar5" << endl; }
    else { 
      method = SECHS;
    }
  }
  
  return(method);
} 

double  TCATSPhysics::CalculatePositionX( vector< vector< vector<double> > >  &StripPositionX,
			   int                                 StripMaxX,
			   double*                             Chargex,
			   double                              CalculatedStripX, 
			   int                                 ff, 
			   correction                          method)
{
  double positionX=-10;

  if(ReconstructionMethodX[ff] == GAUSS) positionX = CalculatedStripX;   // already in mm -> see gaussian method

  else
    {
      // cout << "CalculatedStripX = " << CalculatedStripX  << endl;
  //  Integer part
      int IStripX = (int) CalculatedStripX ;  //  cout << "IStripX = " << IStripX  << endl;
  
  // Decimal Part
      double DStripX = CalculatedStripX-IStripX ; // cout << "DStripX = " << DStripX  << endl;
  
  if( DStripX > 0.5) {IStripX++; DStripX = DStripX-1 ;}     else {DStripX = DStripX;} 
    
  // Calculate Geometrical Position 
  if( IStripX > 0 &&  IStripX < 29 ) {
    //if( IStripX>0 && IStripY>0 && StripMaxX[ff] > 0 && StripMaxY[ff] > 0 &&  StripMaxX[ff] < 29  && StripMaxY[ff] < 29 )
  
    // positionX = (DStripX)*2.54 + StripPositionX[ff][IStripX-1][0] ;  // conversion en mm initiale
  
    if(ff==0)    //CATS1
      {
	positionX = -(DStripX)*2.54 + StripPositionX[ff][IStripX-1][0] ;
	//	positionX =  2.54 * (15-CalculatedStripX) - 1.27 ;
	//cout << "ecartX1 = " << positionX - (-(DStripX)*2.54 + StripPositionX[ff][IStripX-1][0]) << endl;

	if(method == NOcor) positionX = positionX;
	else if(method == cor){
	  if(ReconstructionMethodX[ff] == BAR3) positionX = CorrectedPositionX3(ff, positionX, StripMaxX, 0.6, StripPositionX);
	  if(ReconstructionMethodX[ff] == BAR4) positionX = CorrectedPositionX4(ff, Chargex, positionX, StripMaxX, 0.77, StripPositionX );
	}

      }
    
    else if(ff==1)    //CATS2
      {
	positionX = (DStripX)*2.54 + StripPositionX[ff][IStripX-1][0] ;
	//	positionX =  2.54 * (CalculatedStripX-15) + 1.27 ;
	//cout << "ecartX2 = " << positionX - ((DStripX)*2.54 + StripPositionX[ff][IStripX-1][0]) << endl;

	if(method == NOcor) positionX = positionX;
	else if(method == cor){
	  if(ReconstructionMethodX[ff] == BAR3) positionX = CorrectedPositionX3(ff, positionX, StripMaxX, 0.53, StripPositionX);            
	  if(ReconstructionMethodX[ff] == BAR4) positionX = CorrectedPositionX4(ff, Chargex, positionX, StripMaxX, 0.67, StripPositionX);
	}
      }
    else  cout << "only 2CATS!! ff = " << ff << endl;
  }
  
  else  positionX = -40;

    }
 
  return(positionX);
}


double  TCATSPhysics::CalculatePositionY( vector< vector< vector<double> > >  &StripPositionY,
			   int                                 StripMaxY,
			   double *                            Chargey,
			   double                              CalculatedStripY, 
			   int                                 ff, 
			   correction                          method)
{
  double positionY = -10;

  if(ReconstructionMethodY[ff] == GAUSS) positionY = CalculatedStripY;  // already in mm -> see gaussian method

  else
    {
      
      //  Integer part
      int IStripY = (int) CalculatedStripY ; 
      
      // Decimal Part
      double DStripY = CalculatedStripY-IStripY ;
      
      if( DStripY > 0.5) {IStripY++; DStripY = DStripY-1 ; }   else {DStripY = DStripY; } 
      
      // Calculate Geometrical Position 
      if(IStripY > 0  && IStripY < 29 ) 
      //  if(IStripX>0 && IStripY>0 && StripMaxX[ff] > 0 && StripMaxY[ff] > 0 &&  StripMaxX[ff] < 29  && StripMaxY[ff] < 29 )
	{	
	  positionY = (DStripY)*2.54 + StripPositionY[ff][0][IStripY-1] ;  // conversion en mm initiale
	
	  /*  if(ff== 1) {
	    cout << CalculatedStripY << endl;
	    cout << positionY << endl;
	    cout << StripPositionY[ff][0][IStripY-1] << endl;
	    }*/
	  //positionY = 2.54 * ( CalculatedStripY-14) - 1.27;
	  //  cout << "ecartY" << positionY - ((DStripY)*2.54 + StripPositionY[ff][0][IStripY-1]) << endl;

	  if(ff ==0){
	    if(method == NOcor) positionY = positionY;
	    else if(method == cor) {
	      if(ReconstructionMethodY[ff] == BAR3) positionY = CorrectedPositionY3(ff, positionY, StripMaxY, 0.6, StripPositionY);
	      if(ReconstructionMethodY[ff] == BAR4) positionY = CorrectedPositionY4(ff, Chargey, positionY, StripMaxY, 0.75, StripPositionY);
	    }
	  }
	  
	  else if(ff ==1){
	    if(method == NOcor) positionY = positionY;
	    else if(method == cor){
	      if(ReconstructionMethodY[ff] == BAR3) positionY = CorrectedPositionY3(ff, positionY, StripMaxY, 0.57, StripPositionY);
	      if(ReconstructionMethodY[ff] == BAR4) positionY = CorrectedPositionY4(ff, Chargey, positionY, StripMaxY, 0.7, StripPositionY);
	    }
	  }
	  
	  else cout << "only 2CATS!! ff = " << ff << endl;
	}
      
      else  positionY = -40;
      // cout << IStripX << " " << IStripY << endl;
      
    }
  
  return(positionY);
  
}

double TCATSPhysics:: HyperbolicSequentMethod( double* Charge , int StripMax )
{
  double sechs = -10 ;
  
  if(StripMax > 2 && StripMax<27)
    {
      /*
      cout << "Charge[" << StripMax-1 << "] = " << Charge[StripMax-1-1] << endl;
      cout << "Charge[" << StripMax   << "] = " << Charge[StripMax-1]   << endl;
      cout << "Charge[" << StripMax+1 << "] = " << Charge[StripMax-1+1] << endl; 
      */
      
      double vs1 = sqrt( Charge[StripMax-1]/Charge[StripMax-1+1] ) 	;
      double vs2 = sqrt( Charge[StripMax-1]/Charge[StripMax-1-1] ) 	;
      double vs3 = 0.5*( vs1 + vs2 ) 					;
      double vs4 = log( vs3 + sqrt(vs3*vs3-1.0) ) 			;
      double vs5 = (vs1 - vs2)/(2.0*sinh(vs4)) 				;	
      
      if(vs5<0) vs5=-vs5 ;
      
      double vs6 = 0.5*log( (1.0+vs5)/(1.0-vs5) ) ;
      
      if ( Charge[StripMax-1+1]>Charge[StripMax-1-1] )
	{ sechs = StripMax + vs6/vs4 ;}
      
      else
	{ sechs = StripMax - vs6/vs4 ;}	

      //   cout << "vs1 = " << vs1 << " vs2 = " << vs2 << " vs3 = " << vs3 << " vs4 = " << vs4 << " vs5 = " << vs5 << " vs6 = " << vs6 << endl;
      
    }


  //  cout << "sechs = " << sechs << endl;

  return sechs ;
}


double TCATSPhysics:: GaussianMethodX(int ff, double* Chargex , int StripMax, vector< vector< vector<double> > >  &StripPositionX)
{
  int StripMax_ = StripMax - 1;
  double gauss = -1000;
  double Q[3];
  double StripPos[3];

 
  for(int j = 0; j<3 ; j++)
    {
      Q[j] = 0;
      StripPos[j] = 0;
    }

  // cout << "StripMax = " <<  StripMax <<endl;

  if(StripMax > 3 && StripMax < 26)
    {
      Q[0] = Chargex[StripMax_] ;  
      StripPos[0] = StripPositionX[ff][StripMax_][0];

      // cout << "Q[0] = " << Q[0] << " StripMax = " << StripMax << " StripPos[0] =" << StripPos[0] << endl;
      
      if(Chargex[StripMax_-1] !=-1)  {
	//cout << "no pb" << endl;
	Q[1] = Chargex[StripMax_-1];
	StripPos[1] = StripPositionX[ff][StripMax_-1][0];

	//	cout << "Q[1] = " << Q[1] << " Strip1 = " << StripMax-1 << " StripPos[1] =" << StripPos[1] << endl;
      }
      else {
	if(Chargex[StripMax_- 2] !=-1) {
	  //cout << "coconutsX-1! " << endl;
	  Q[1] = Chargex[StripMax_-2];
	  StripPos[1] = StripPositionX[ff][StripMax_ - 2][0];
	  //  cout << "Q[0] = " << Q[0] << " StripMax = " << StripMax << " StripPos[0] =" << StripPos[0] << endl;
	  // cout << "Q[1] = " << Q[1] << " Strip1 = " << StripMax-2 << " StripPos[1] =" << StripPos[1] << endl;
	}
	else {
	  if(Chargex[StripMax_- 3] !=-1) {
	    //cout << "coconutsX-2! " << endl;
	    Q[1] = Chargex[StripMax_- 3];
	    StripPos[1] = StripPositionX[ff][StripMax_ - 3][0];
	    //  cout << "Q[0] = " << Q[0] << " StripMax = " << StripMax << " StripPos[0] =" << StripPos[0] << endl;
	    // cout << "Q[1] = " << Q[1] << " Strip1 = " << StripMax-3 << " StripPos[1] =" << StripPos[1] << endl;
	  }
	  //else cout <<"pb avec les charges X1" << endl;
	}
      }
      
      if(Chargex[StripMax_+1] !=-1)  {
	//cout << "no pb" << endl;
	Q[2] = Chargex[StripMax_+1];
	StripPos[2] = StripPositionX[ff][StripMax_ + 1][0];
	
	//	cout << "Q[2] = " << Q[2] << " Strip2 = " << StripMax+1 << " StripPos[2] =" << StripPos[2] << endl;
      }
      else {
	if(Chargex[StripMax_ +2] !=-1)  {
	  //cout << "coconutsX+1! " << endl;
	  Q[2] = Chargex[StripMax_+2];
	  StripPos[2] = StripPositionX[ff][StripMax_ + 2][0];
	  // cout << "Q[0] = " << Q[0] << " StripMax = " << StripMax << " StripPos[0] =" << StripPos[0] << endl;
	  // cout << "Q[2] = " << Q[2] << " Strip2 = " << StripMax+2 << " StripPos[2] =" << StripPos[2] << endl;
	}
	else {
	  if(Chargex[StripMax_ +3] !=-1) {
	    //cout << "coconutsX+2! " << endl;
	    Q[2] = Chargex[StripMax_+3];
	    StripPos[2] = StripPositionX[ff][StripMax_ + 3][0];
	    //  cout << "Q[0] = " << Q[0] << " StripMax = " << StripMax << " StripPos[0] =" << StripPos[0] << endl;
	    // cout << "Q[2] = " << Q[2] << " Strip2 = " << StripMax+3 << " StripPos[2] =" << StripPos[2] << endl;
	  }
	  //else cout << "pb avec les charges X2" << endl; //Q[1] = 1 ; Strip[1] = 0 ;
	}
      }
      
      double Q0_Q1 = log(Q[0]/Q[1]);
      double Q0_Q2 = log(Q[0]/Q[2]);
      
      double num   = Q0_Q1 * (StripPos[2]*StripPos[2] - StripPos[0]*StripPos[0]) - Q0_Q2 * (StripPos[1]*StripPos[1] - StripPos[0]*StripPos[0])  ;
      double denom = Q0_Q1 * (StripPos[2] - StripPos[0]) - Q0_Q2 * (StripPos[1] - StripPos[0]) ;
      gauss = 0.5*num / denom;

    }

  return gauss;
  
}


double TCATSPhysics:: GaussianMethodY(int ff, double* Chargey , int StripMax, vector< vector< vector<double> > >  &StripPositionY)
{
  double Q[3];
  double StripPos[3];

  for(int j = 0; j<3 ; j++)
    {
      Q[j] = 0;
      StripPos[j] = 0;
    }

  int StripMax_ = StripMax - 1;

  double gauss = -1000;

  if(StripMax > 2 && StripMax<27)
    {
      Q[0] = Chargey[StripMax_] ;
      StripPos[0] = StripPositionY[ff][0][StripMax_];
      
      if(Chargey[StripMax_-1] !=-1)  {
	//cout << "no pb" << endl;
	Q[1] = Chargey[StripMax_-1];
	StripPos[1] = StripPositionY[ff][0][StripMax_-1];
      }
      else {
	if(Chargey[StripMax_-2] !=-1)  {
	  //cout << "coconutsY-1! " << endl;
	  Q[1] = Chargey[StripMax_-2];
	  StripPos[1] = StripPositionY[ff][0][StripMax_ - 2] ;
	}
	else {
	  if(Chargey[StripMax_-3] !=-1)  {
	    //cout << "coconutsY-2! " << endl;
	    Q[1] = Chargey[StripMax_-3];
	    StripPos[1] = StripPositionY[ff][0][StripMax_ - 3] ;
	  }
	  //else cout << "pb avec les charges Y1" << endl; //Q[1] = 1 ; Strip[1] = 0 
	}
      }
      
      if(Chargey[StripMax_+1] !=-1)  {
	//cout << "no pb" << endl;
	Q[2] = Chargey[StripMax_+1];
	StripPos[2] = StripPositionY[ff][0][StripMax_ + 1];
      }
      else {
	if(Chargey[StripMax_ +2] !=-1)  {
	  //cout << "coconutsY+1! " << endl;
	  Q[2] = Chargey[StripMax_+2];
	  StripPos[2] = StripPositionY[ff][0][StripMax_ + 2] ;
	}
	else {
	  if(Chargey[StripMax_ +3] !=-1)  {
	    //cout << "coconutsY+2! " << endl;
	    Q[2] = Chargey[StripMax_+3];
	    StripPos[2] = StripPositionY[ff][0][StripMax_ + 3] ;
	  }
	  //else cout << "pb avec les charges Y2" << endl; //Q[1] = 1 ; Strip[1] = 0 
	}
      }
	
      //else cout << "pb avec les charges Y2" << endl; //Q[1] = 1 ; Strip[1] = 0 
      

      double Q0_Q1 = log(Q[0]/Q[1]);
      double Q0_Q2 = log(Q[0]/Q[2]);
      
      double num   = Q0_Q1 * (StripPos[2]*StripPos[2] - StripPos[0]*StripPos[0]) - Q0_Q2 * (StripPos[1]*StripPos[1] - StripPos[0]*StripPos[0])  ;
      double denom = Q0_Q1 * (StripPos[2] - StripPos[0]) - Q0_Q2 * (StripPos[1] - StripPos[0]) ;
      gauss = 0.5*num / denom;

    }

  return gauss;
  
}


double TCATSPhysics:: Barycentric5Method( double* Charge , int StripMax )
{
  double Barycenter = 0 ;
 
  if(StripMax > 2 && StripMax < 27)
    {
      int StripMax_ = StripMax -1 ; // Use because numerotation of array start at 0 ;
      double NumberOfPoint = 0 ;
      double ChargeTotal =0;


      for(int i = -2 ; i < 3 ; i++)
	{
	  if(Charge[StripMax_+i]!=-1)	          // Charge initialized to -1
	    { 
	      Barycenter += (StripMax+i)*Charge[StripMax_+i] ;
	      NumberOfPoint++;
	      ChargeTotal+=Charge[StripMax_+i];
	    }
	}
            
      if(ChargeTotal>0) Barycenter = Barycenter / ChargeTotal ;
      else {Barycenter = -1000 ; } // cout << "Yo" << endl ;}
      
    }
  else
    {
      Barycenter = -1000 ;
      //        cout << "Strip max " << StripMax << endl;
    }
  
  return Barycenter ;
}


double TCATSPhysics:: Barycentric3Method( double* Charge , int StripMax )
{
  double Barycenter = 0 ;
   
  if(StripMax > 2 && StripMax < 27)
    {
      int StripMax_ = StripMax -1 ; // Use because numerotation of array start at 0 ;
      double NumberOfPoint = 0 ;
      double ChargeTotal =0;

      for(int i = -1 ; i < 2 ; i++)
	{
	  if(Charge[StripMax_+i]!=-1)	// Charge initialized to -1
	    { 
	      Barycenter += (StripMax+i)*Charge[StripMax_+i] ;
	      NumberOfPoint++;
	      ChargeTotal+=Charge[StripMax_+i];
	    }
	}
      
      if(ChargeTotal>0) Barycenter = Barycenter / ChargeTotal ;
      else {Barycenter = -1000 ;}  // cout << "Yo" << endl ;}
    }

  else
    {
      Barycenter = -1000 ;
      //        cout << "Strip max " << StripMax << endl;
    }
 
  return Barycenter ;
}


double TCATSPhysics:: Barycentric4Method( double* Charge , int StripMax )
{
  double Barycenter = 0 ;
   
  if(StripMax > 2 && StripMax < 27)     {

      int StripMax_ = StripMax -1 ; // Use because numerotation of array start at 0 ;
      double NumberOfPoint = 0 ;
      double ChargeTotal =0;

      if(Charge[StripMax_+1] > Charge[StripMax_-1]) {

	//	cout << "Barycentre droit" << endl;
	  for(int i = -1 ; i < 3 ; i++)  
	    {
	      if(Charge[StripMax_+i]!=-1)   {                            // Charge initialized to -1
		Barycenter += (StripMax+i)*Charge[StripMax_+i] ;
		NumberOfPoint++;
		ChargeTotal+=Charge[StripMax_+i];
	      }
	    }
      }
      
      else {
	//	cout << "Barycentre gauche" << endl;	
	for(int i = -2 ; i < 2 ; i++)
	  {
	    if(Charge[StripMax_+i]!=-1)	{                                 // Charge initialized to -1
	      Barycenter += (StripMax+i)*Charge[StripMax_+i] ;
	      NumberOfPoint++;
	      ChargeTotal+=Charge[StripMax_+i];
	    }
	  }
      }

      if(ChargeTotal>0) {
	Barycenter = Barycenter / ChargeTotal ;
      }
  
  }

  else
    {
      Barycenter = -1000 ;
      //        cout << "Strip max " << StripMax << endl;
    }

  return Barycenter ;
}


double TCATSPhysics::CorrectedPositionX3(int ff, double Position, int StripMax, double a, vector< vector< vector<double> > >  &StripPositionX )
{
  double Corrected_Position = 0;
  int StripMax_ = StripMax -1;
  double xmax = StripPositionX[ff][StripMax_][0] ;
  // cout << "xmax2 " << xmax << endl;
  Corrected_Position = (Position - xmax) / a + xmax;
  
  return Corrected_Position;
}

double TCATSPhysics::CorrectedPositionY3(int ff, double Position, int StripMax, double a, vector< vector< vector<double> > >  &StripPositionY )
{
  double Corrected_Position = 0;
  int StripMax_ = StripMax -1;
  double xmax = StripPositionY[ff][0][StripMax_];
  // cout << "xmax2 " << xmax << endl;
  Corrected_Position = (Position - xmax) / a + xmax;
  
  return Corrected_Position;
}

double TCATSPhysics::CorrectedPositionX4(int ff, double* Charge, double Position, int StripMax, double b, vector< vector< vector<double> > >  &StripPositionX )
{
  double Corrected_Position = 0;
  double xmax = 0;
  int StripMax_ = StripMax -1;

  if(Charge[StripMax_+1] > Charge[StripMax_-1]) {
    if(ff ==0)     xmax = StripPositionX[ff][StripMax_][0] - 1.27;
    else  xmax = StripPositionX[ff][StripMax_][0] + 1.27;
  }
  
  else{
    if(ff ==0)     xmax = StripPositionX[ff][StripMax_-1][0] - 1.27;
    else  xmax = StripPositionX[ff][StripMax_-1][0]  + 1.27;
  }

  Corrected_Position = (Position - xmax) / b + xmax;

  return Corrected_Position;
}

double TCATSPhysics::CorrectedPositionY4(int ff, double* Charge, double Position, int StripMax, double b, vector< vector< vector<double> > >  &StripPositionY )
{
  double Corrected_Position = 0;
  double xmax = 0;
  int StripMax_ = StripMax -1;

  if(Charge[StripMax_+1] > Charge[StripMax_-1]) {
    xmax = StripPositionY[ff][0][StripMax_] + 1.27 ;
  }

  else{
    xmax = StripPositionY[ff][0][StripMax_-1] + 1.27;
  }

  Corrected_Position = (Position - xmax) / b + xmax;

  return Corrected_Position;
}


