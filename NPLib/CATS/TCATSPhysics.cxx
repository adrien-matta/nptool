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


#include "TCATSPhysics.h"
using namespace LOCAL_CATS;

//	STL
#include <cmath>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>

//	NPL
#include "RootInput.h"
#include "RootOutput.h"

//	ROOT
#include "TChain.h"
#include "TRandom.h"

ClassImp(TCATSPhysics)
///////////////////////////////////////////////////////////////////////////
TCATSPhysics::TCATSPhysics()
{
  EventData 				= new TCATSData()	;
  EventPhysics 				= this			;
  NumberOfCATS                          = 0                     ;
}



TCATSPhysics::~TCATSPhysics()
{
}


////////////////////////////////////////// A SANDRA DE JOUER! ////////////////////////////////////////

			
//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void TCATSPhysics::ReadConfiguration(string Path) 				
{
  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());
  string LineBuffer          		;
  string DataBuffer          		;
  
  double Ax , Bx , Cx , Dx , Ay , By , Cy , Dy , Az , Bz , Cz , Dz    	;
  TVector3 A , B , C , D                                          	;

  bool check_A = false 	;
  bool check_B = false  	;
  bool check_C = false 	;
  bool check_D = false 	;

  bool ReadingStatus = false ;

 
  while (!ConfigFile.eof()) 
    {
      getline(ConfigFile, LineBuffer);
      
      //If line is a Start Up CATS bloc, Reading toggle to true      
      if (LineBuffer.compare(0, 12, "CATSDetector") == 0) 
	{
	  cout << "///" << endl           		;
	  cout << "CATS Detector found: " << endl   ;  
	  ReadingStatus = true 					;
	}
      
      //	Else don't toggle to Reading Block Status
      else ReadingStatus = false ;
      
      //	Reading Block
      while(ReadingStatus)
	{
	  ConfigFile >> DataBuffer ;
	  //	Comment Line 
	  if(DataBuffer.compare(0, 1, "%") == 0) {
	    ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
	  }
	  
	  //	Finding another telescope (safety), toggle out
	  else if (DataBuffer.compare(0, 12, "CATSDetector") == 0) {
	    cout << "WARNING: Another CATS is found before standard sequence of Token, Error may occured in CATS definition" << endl ;
	    ReadingStatus = false ;
	  }
	  
	  //	Corner Position method
	  
	  else if (DataBuffer.compare(0, 6, "X1_Y1=") == 0) {
	    check_A = true;
	    ConfigFile >> DataBuffer ;
	    Ax = atof(DataBuffer.c_str()) ;
	    Ax = Ax  ;
	    ConfigFile >> DataBuffer ;
	    Ay = atof(DataBuffer.c_str()) ;
	    Ay = Ay  ;
	    ConfigFile >> DataBuffer ;
	    Az = atof(DataBuffer.c_str()) ;
	    Az = Az  ;
	    
	    A = TVector3(Ax, Ay, Az);
	    cout << "X1 Y1 corner position : (" << A.X() << ";" << A.Y() << ";" << A.Z() << ")" << endl;
	  }
	  
	  else if (DataBuffer.compare(0, 7, "X28_Y1=") == 0) {
	    check_B = true;
	    ConfigFile >> DataBuffer ;
	    Bx = atof(DataBuffer.c_str()) ;
	    Bx = Bx  ;
	    ConfigFile >> DataBuffer ;
	    By = atof(DataBuffer.c_str()) ;
	    By = By  ;
	    ConfigFile >> DataBuffer ;
	    Bz = atof(DataBuffer.c_str()) ;
	    Bz = Bz  ;
	    
	    B = TVector3(Bx, By, Bz);
	    cout << "X28 Y1 corner position : (" << B.X() << ";" << B.Y() << ";" << B.Z() << ")" << endl;
	  }
	  
	  else if (DataBuffer.compare(0, 7, "X1_Y28=") == 0) {
	    check_C = true;
	    ConfigFile >> DataBuffer ;
	    Cx = atof(DataBuffer.c_str()) ;
	    Cx = Cx  ;
	    ConfigFile >> DataBuffer ;
	    Cy = atof(DataBuffer.c_str()) ;
	    Cy = Cy  ;
	    ConfigFile >> DataBuffer ;
	    Cz = atof(DataBuffer.c_str()) ;
	    Cz = Cz  ;
	    
	    C = TVector3(Cx, Cy, Cz);
	    cout << "X1 Y28 corner position : (" << C.X() << ";" << C.Y() << ";" << C.Z() << ")" << endl;
	  }
	  
	  else if (DataBuffer.compare(0, 8, "X28_Y28=") == 0) {
	    check_D = true;
	    ConfigFile >> DataBuffer ;
	    Dx = atof(DataBuffer.c_str()) ;
	    Dx = Dx  ;
	    ConfigFile >> DataBuffer ;
	    Dy = atof(DataBuffer.c_str()) ;
	    Dy = Dy  ;
	    ConfigFile >> DataBuffer ;
	    Dz = atof(DataBuffer.c_str()) ;
	    Dz = Dz  ;
	    
	    D = TVector3(Dx, Dy, Dz);
	    cout << "X28 Y28 corner position : (" << D.X() << ";" << D.Y() << ";" << D.Z() << ")" << endl;
	    
	  }
	  
	  //	End Corner Position Method

	  /////////////////////////////////////////////////
	    //	If All necessary information there, toggle out
	    if (check_A && check_B && check_C && check_D)  
	      { 
		ReadingStatus = false; 
		
		///Add The previously define telescope
		  
		  AddCATS(	A   ,
			 	B   ,
				C   ,
				D   );
		  
		  check_A = false;
		  check_B = false;
		  check_C = false;
		  check_D = false;
	      }
	}  
		  
    }

  // Should be called in Analysis... 
  ReadPedestal("../../../offline/calibrations/CATS/CATScoeff/Piedestaux_368.txt");
  // path given from e530 directory

  cout << endl << "/////////////////////////////" << endl<<endl;

}

//	Add Parameter to the CalibrationManger
void TCATSPhysics::AddParameterToCalibrationManager()	
{
  CalibrationManager* Cal = CalibrationManager::getInstance();
  
  for(int i = 0 ; i < NumberOfCATS ; i++)
    {
      
      for( int j = 0 ; j < 28 ; j++)
	{
	  Cal->AddParameter("CATS", "D"+itoa(i+1)+"_X"+itoa(j+1)+"_Q","CATS_D"+itoa(i+1)+"_X"+itoa(j+1)+"_Q")	;
	  Cal->AddParameter("CATS", "D"+itoa(i+1)+"_Y"+itoa(j+1)+"_Q","CATS_D"+itoa(i+1)+"_Y"+itoa(j+1)+"_Q")	;
	} 
    }
}		

//	Activated associated Branches and link it to the private member DetectorData address
//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void TCATSPhysics::InitializeRootInput() 					
{
  TChain* inputChain = RootInput::getInstance()->GetChain()	;
  inputChain->SetBranchStatus( "CATS" , true )			;
  inputChain->SetBranchStatus( "fCATS_*" , true )		;
  inputChain->SetBranchAddress( "CATS" , &EventData )           ;
}

//	Create associated branches and associated private member DetectorPhysics address
void TCATSPhysics::InitializeRootOutput() 		 		
{
  TTree* outputTree = RootOutput::getInstance()->GetTree()		;
  outputTree->Branch( "CATS" , "TCATSPhysics" , &EventPhysics )	;
}


void TCATSPhysics::AddCATS(TVector3 C_X1_Y1, TVector3 C_X28_Y1, TVector3 C_X1_Y28, TVector3 C_X28_Y28)
{
  NumberOfCATS++			;

 //	Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that Y strip are along X axis)
  TVector3 U = C_X28_Y1 - C_X1_Y1 				;	
  U = U.Unit()									;
		
  //	Vector V on Telescope Face (parallele to X Strip)
  TVector3 V = C_X1_Y28 - C_X1_Y1 				;
  V = V.Unit()									;
		
  //	Position Vector of Strip Center
  TVector3 StripCenter 					;
  //	Position Vector of X=1 Y=1 Strip 
  TVector3 Strip_1_1 					;		
		
  //	Geometry Parameter
  
  double Face = 71.12 				; //mm
  double NumberOfStrip = 28 			;
  double StripPitch = Face / NumberOfStrip	; //mm
  
  //	Buffer object to fill Position Array
  vector<double> lineX ; 
  vector<double> lineY ; 
  //vector<double> lineZ ;

  vector< vector< double > >	OneDetectorStripPositionX	;
  vector< vector< double > >	OneDetectorStripPositionY	;
  double                 	OneDetectorStripPositionZ	;

  //	Moving StripCenter to 1.1 corner (strip center!) :
  Strip_1_1 = C_X1_Y1 + (U+V) * (StripPitch/2) 	;
  
  //cout << "Strip_1_1X = " << Strip_1_1.X()  << endl;
  //cout << "Strip_1_1Y = " << Strip_1_1.Y()  << endl;
  	
	
  for( int i = 0 ; i < 28 ; i++ )
    {
      lineX.clear()	;
      lineY.clear()	;
      //  lineZ.clear()	;
				
      for( int j = 0 ; j < 28 ; j++ )
	{
	  StripCenter  = Strip_1_1 + StripPitch*( i*U + j*V  )	;
	  //StripCenter += -TargetPosition		;
	  lineX.push_back( StripCenter.x() )	;
	  lineY.push_back( StripCenter.y() )	;
	  //  lineZ.push_back( StripCenter.z() )	;	
	}
	
      OneDetectorStripPositionX.push_back(lineX);
      OneDetectorStripPositionY.push_back(lineY);
    }

  OneDetectorStripPositionZ = C_X1_Y1.Z();

  StripPositionX.push_back(OneDetectorStripPositionX)	;
  StripPositionY.push_back(OneDetectorStripPositionY)	;
  StripPositionZ.push_back(OneDetectorStripPositionZ)	;

}

		
//	This method is called at each event read from the Input Tree. Aim is to build treat Raw dat in order to extract physical parameter. 
void TCATSPhysics::BuildPhysicalEvent()					
{
  // voir les commentaire fait la ou la methode existe deja
}
		
//	Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
//	This method aimed to be used for analysis performed during experiment, when speed is requiered.
//	NB: This method can eventually be the same as BuildPhysicalEvent.

/*
void TCATSPhysics::BuildSimplePhysicalEvent()				
{

  // cout <<  EventData-> GetCATSMultX() << endl;
  // pourquoi pas une methode avec qui prend que le strip max par exemple...
}
*/


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

  //FailedReconstructionX.clear()                  ;
  FailedReconstructionY.clear()                  ;


  HitX = 0;
  HitY = 0;
  
  NumberOfCATS = 0;
}



void TCATSPhysics::Dump()
{
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

  cout << "Number Of CATS : " << NumberOfCATS << endl;
  
  for(unsigned int i= 0; i < DetNumberX.size() ; i++) 
    {
      cout << "DetNumberX :  " << DetNumberX.at(i) << endl;
    }
  for(unsigned int i= 0; i < StripX.size() ; i++) 
    {
      cout << "StripX :  " << StripX.at(i) << endl;
    }
  for(unsigned int i= 0; i < StripMaxX.size() ; i++) 
    {
      cout << "StripMaxX :  " << StripMaxX.at(i) << endl;
    }
  for(unsigned int i= 0; i < ChargeX.size() ; i++) 
    {
      cout << "ChargeX :  " << ChargeX.at(i) << endl;
    }
  for(unsigned int i= 0; i < ChargeSumX.size() ; i++) 
    {
      cout << "ChargeSumX :  " << ChargeSumX.at(i) << endl;
    }
  for(unsigned int i= 0; i < MultOverThreshX.size() ; i++) 
    {
      cout << "MultOverThreshX :  " << MultOverThreshX.at(i) << endl;
    }



  for(unsigned int i= 0; i < DetNumberY.size() ; i++) 
    {
      cout << "DetNumberY :  " << DetNumberY.at(i) << endl;
    }
 for(unsigned int i= 0; i < StripY.size() ; i++) 
    {
      cout << "StripY :  " << StripY.at(i) << endl;
    }
  for(unsigned int i= 0; i < StripMaxY.size() ; i++) 
    {
      cout << "StripMaxY :  " << StripMaxY.at(i) << endl;
    }
  for(unsigned int i= 0; i < ChargeY.size() ; i++) 
    {
      cout << "ChargeY :  " << ChargeY.at(i) << endl;
    }
  for(unsigned int i= 0; i < ChargeSumY.size() ; i++) 
    {
      cout << "ChargeSumY :  " << ChargeSumY.at(i) << endl;
    }
  for(unsigned int i= 0; i < MultOverThreshY.size() ; i++) 
    {
      cout << "MultOverThreshY :  " << MultOverThreshY.at(i) << endl;
    }
  
  
  for(unsigned int i = 0; i < DetNumberX_Position.size() ; i++)
    {
      cout << "DetNumberX_Position : " << DetNumberX_Position.at(i) << endl;
    }                   
  for(unsigned int i = 0; i < DetNumberY_Position.size() ; i++)
    {
      cout << "DetNumberY_Position : " << DetNumberY_Position.at(i) << endl;
    }      
  for(unsigned int i = 0; i < DetNumberZ_Position.size() ; i++)
    {
      cout << "DetNumberZ_Position : " << DetNumberZ_Position.at(i) << endl;
    } 

  for(unsigned int i = 0; i < PositionX.size() ; i++)
    {
      cout << "PositionX : " << PositionX.at(i) << endl;
    }                   
  for(unsigned int i = 0; i < PositionY.size() ; i++)
    {
      cout << "PositionY : " << PositionY.at(i) << endl;
    }  
  for(unsigned int i = 0; i < PositionZ.size() ; i++)
    {
      cout << "PositionZ : " << PositionZ.at(i) << endl;
    }  

  for(unsigned int i = 0; i < ReconstructionMethodX.size() ; i++)
    {
      cout << "ReconstructionMethodX : " << ReconstructionMethodX.at(i) << endl;
    } 

  for(unsigned int i = 0; i < ReconstructionMethodY.size() ; i++)
    {
      cout << "ReconstructionMethodY : " << ReconstructionMethodY.at(i) << endl;
    } 
  /*
  for(unsigned int i = 0; i < FailedReconstructionX.size() ; i++)
    {
      cout << "FailedReconstructionX : " << FailedReconstructionX.at(i) << endl;
    } 
  */
  for(unsigned int i = 0; i < FailedReconstructionY.size() ; i++)
    {
      cout << "FailedReconstructionY : " << FailedReconstructionY.at(i) << endl;
    } 
  
 
}




void TCATSPhysics::ReadPedestal(string PedestalPath)
{
  int NumberOfStrips = 28   ; 
  vector<double> line 	; 
  line.resize(NumberOfStrips, 0);

  Pedestal_X.resize(NumberOfCATS, line);
  Pedestal_Y.resize(NumberOfCATS, line);

  Threshold_X.resize(NumberOfCATS, line);
  Threshold_Y.resize(NumberOfCATS, line);

  string DataBuffer, XY;
  int StripNumber = 0;

  ifstream Pedestal_File;
  Pedestal_File.open(PedestalPath.c_str());

  if( Pedestal_File.is_open() ) 
    cout << " Calibration File " << PedestalPath << " loading " << endl;
  else {
    cout << " Error, no calibration file" << PedestalPath << " found" << endl; 
    return;
  }

  while( !Pedestal_File.eof() ) {

    Pedestal_File >> DataBuffer ;
        
    for(int i = 0 ; i < NumberOfCATS ; i++)
      {
	std::ostringstream DetectorNumber ;
	DetectorNumber << i+1;   

	if(DataBuffer == "CATS"+DetectorNumber.str()+"_X")	
	  { 
	    for( int k = 0 ; k < NumberOfStrips ; k++ )
	      {
		Pedestal_File >> StripNumber; 	
		Pedestal_File >> DataBuffer;	Pedestal_X[i][k] = atof(DataBuffer.c_str()) ;

		// definition du seuil : piedestal + 3 * sigma(piedestal)
		Pedestal_File >> DataBuffer;  	Threshold_X[i][k] = Pedestal_X[i][k]+3*atof(DataBuffer.c_str()) ;   
		
	      }						    
	  }
	else if(DataBuffer == "CATS"+DetectorNumber.str()+"_Y")
	  {
	    for( int k = 0 ; k < NumberOfStrips ; k++ )
	      {
		Pedestal_File >> StripNumber; 	  
		Pedestal_File >> DataBuffer;	Pedestal_Y[i][k] = atof(DataBuffer.c_str()) ;

		// definition du seuil : piedestal + 3 * sigma(piedestal)
		Pedestal_File >> DataBuffer;	Threshold_Y[i][k] = Pedestal_Y[i][k]+3*atof(DataBuffer.c_str()) ;  // definition du seuil 

	      }						    
	  }
      }
  }

}


/*
  Pas mal de chose a modifier:
  - supprimer l'histoire des calibration par exemple...
  - TCATSData doit etre un membre prive de la classe (non ecrit dans l'arbre de sorti), vois ce que j'ai fait sur MUST2
  - Les positions doivent aussi etre des membre prive non ecrit, comme dans MUST2
*/
void TCATSPhysics::BuildSimplePhysicalEvent()
{
  /*
    int 	        HitX 	       = 0 	;
    int 	        HitY 	       = 0 	;
  */    

  
  gRandom->SetSeed(0);

  //EventData->Dump();
  double CalculatedStripX, CalculatedStripY ;
  double posX =0 , posY = 0;
  

  //	How many CATS?
  int NumberOfCATSHit = 0 ;
  int DetectorID = -1;
  
  for( unsigned short i = 0 ; i < EventData->GetCATSMultX() ; i++ )
    { 
      //  if( NumberOfCATSHit < EventData->GetCATSDetX(i) ) NumberOfCATSHit = EventData->GetCATSDetX(i)	;   //determination of the number of CATS detectors

      if( EventData->GetCATSDetX(i) != DetectorID)  {
	NumberOfCATSHit++;
      }

      if(NumberOfCATSHit == 2) break;	

    }

  
  // cout << "Nombre de CATS: " << NumberOfCATSHit << endl;


  // INITIALISATION OF VECTORS
  for(int ff = 0 ; ff < NumberOfCATSHit ; ff++ )
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
      
      //      FailedReconstructionX.push_back(NO); 
      FailedReconstructionY.push_back(NO);
      
    }
  //  cout << "Init OK" << endl;


  
  for(int gg = 0 ; gg < NumberOfCATSHit ; gg++ )
    { 
      //int    ff = NumberOfCATSHit - gg -1 ;
      int ff = gg ;
     

      CalculatedStripX = AnalyseX(ff, NumberOfCATSHit);        //     cout << "Analyse X = " << CalculatedStripX << endl; 
      CalculatedStripY = AnalyseY(ff, NumberOfCATSHit);        //    cout << "Analyse Y = " << CalculatedStripY << endl;
       
      //      CalculatedStripX = AnalyseX(EventData, Pedestal_X , OnlineCalib_X_E, Threshold_X, StripPositionX, ff, NumberOfCATSHit);        //     cout << "Analyse X = " << CalculatedStripX << endl; 
      //CalculatedStripY = AnalyseY(EventData, Pedestal_Y , OnlineCalib_Y_E, Threshold_Y, StripPositionY, ff, NumberOfCATSHit);        //    cout << "Analyse Y = " << CalculatedStripY << endl;

      //posX = CalculatePositionX(StripPositionX, StripMaxX[ff], Chargex, CalculatedStripX, ff, cor);   // cout << "Position X = " << posX << endl;
      //posY = CalculatePositionY(StripPositionY, StripMaxY[ff], Chargey, CalculatedStripY, ff, cor);   // cout << "Position Y = " << posY << endl;

      posX = CalculatePositionX(StripMaxX[ff], Chargex, CalculatedStripX, ff, cor);   // cout << "Position X = " << posX << endl;
      posY = CalculatePositionY(StripMaxY[ff], Chargey, CalculatedStripY, ff, cor);   // cout << "Position Y = " << posY << endl;


      DetNumberX_Position.push_back(ff+1);
      DetNumberY_Position.push_back(ff+1);
      DetNumberZ_Position.push_back(ff+1);
      
      PositionX.push_back(posX) ;                  
      PositionY.push_back(posY) ;                  
      PositionZ.push_back(StripPositionZ[ff]) ;
    }
	
 
  if(NumberOfCATSHit>1)
    { 
      /*
	cout << "PositionX[0]= " <<PositionX[0] << " PositionX[1] = " << PositionX[1] << " PositionX[0]-PositionX[1]= " << PositionX[0]-PositionX[1] << endl;
	cout << "PositionY[0]= " <<PositionY[0] << " PositionY[1] = " << PositionY[1] << " PositionY[0]-PositionY[1]= " << PositionY[0]-PositionY[1] << endl;
	cout << "PositionZ[0] = "<< PositionZ[0] << " PositionZ[1] = "<< PositionZ[1] << endl;
      */

      if(PositionX[0] > -35 && PositionX[0] < 35 && PositionY[0] > -35 && PositionY[0] < 35 && PositionX[1] > -35 && PositionX[1] < 35 && PositionY[1] > -35 && PositionY[1] < 35 ) 
      //      if(PositionX[0] != -1000 && PositionY[0] != -1000 && PositionX[1] != -1000 && PositionY[1] != -1000) 
	{
	  BeamDirection = TVector3 (PositionX[1]-PositionX[0] ,
				    PositionY[1]-PositionY[0] ,
				    PositionZ[1]-PositionZ[0] );
	  //	BeamDirection.Unit(); Adrien
	  
	  double l = sqrt((PositionZ[0]-PositionZ[1])*(PositionZ[0]-PositionZ[1]));
	  double L = - PositionZ[1];
	  
	  double t = (l+L) / l;
	  
	  PositionOnTargetX = PositionX[0] + (PositionX[1] - PositionX[0]) * t ;
	  PositionOnTargetY = PositionY[0] + (PositionY[1] - PositionY[0]) * t ;
	  
	  BeamDirection.Unit();
	}

      else
	{
	  //cout << "One of the CATS position was not reconstructed ! Impossible to reconstruct position on target ..." << endl;

	  //cout << "PositionX[0] = " <<  PositionX[0] << "   PositionY[0] = " << PositionY[0] << "   PositionX[1] = " << PositionX[1] <<  "   PositionY[1] = " << PositionY[1] << endl;
	  

	  BeamDirection = TVector3 ( 	1 ,
					0 ,
					0 );
	  
	  PositionOnTargetX = 3000	;
	  PositionOnTargetY = 3000	;
	}
    }
  
  else if(NumberOfCATSHit ==1)
    {
      cout << NumberOfCATSHit << endl;

      BeamDirection = TVector3 ( 	1 ,
					0 ,
					0 );
      PositionOnTargetX = 5000	;
      PositionOnTargetY = 5000	;
    }
}


double TCATSPhysics::AnalyseX(//TCATSData* Data, 
			      //vector< vector <double> > 	        &Pedestal_X 		,
			      //vector< vector< vector<double> > > 	&OnlineCalib_X_E,
			      //vector< vector <double> > 		&Threshold_X 	,
			      //vector< vector< vector<double> > >        &StripPositionX,
			      int ff,
			      int NumberOfCATSHit)
{
  // cout << "AnalyseX proccessing ..." << endl;

  // double	Chargex[28];
  for(unsigned short z=0; z<28; z++) {
    Chargex[z] = -1;
  }

  //  int HitX = 0;
  double ChargeSum_X = 0 ;
  double ChargeX_Buffer = 0;
  double CalculatedStripX=0;

  for(UShort_t i =0; i<EventData->GetCATSMultX(); i++)
    {
      // cout <<  EventData->GetCATSDetX(i)<< endl;  
      //cout <<  EventData->GetCATSStripX(i)<< endl;  
      // cout << "ff+1 = " << ff+1 << endl;

      if( EventData->GetCATSDetX(i) == ff+1 )
	{
	  int NX = EventData->GetCATSDetX(i);   
	  int StrX = EventData->GetCATSStripX(i) ;   // cout << NX << " " << StrX << endl ;
	  
	  if(NX > 2 || StrX > 28)	cout << NX << " " << StrX << endl ;
	  
	  double Q = EventData->GetCATSChargeX(i) + gRandom->Rndm() - Pedestal_X[NX-1][StrX-1] ;
	  //ChargeX_Buffer = OnlineCalib_X_E[NX-1][StrX-1][0] + Q * OnlineCalib_X_E[NX-1][StrX-1][1] + Q*Q * OnlineCalib_X_E[NX-1][StrX-1][2] ;

	  ChargeX_Buffer = CalibrationManager::getInstance()->ApplyCalibration("CATS/D"+itoa(NX)+"_X"+itoa(StrX)+"_Q",Q);

	
	  if(EventData->GetCATSChargeX(i) > Threshold_X[NX-1][StrX-1] && NX <= NumberOfCATSHit  && StrX < 28)
	    {  
	      //  cout <<  Threshold_X[NX-1][StrX-1] << endl;

	      //	KNOWN INVERSION
	      if	(StrX == 15 && NX == 1) 	StrX = 16	;
	      else if (StrX == 16 && NX == 1) 		StrX = 15 	;
	      
	      MultOverThreshX[ff]++;
	      ChargeSum_X += ChargeX_Buffer;    
	      //ChargeSum += ChargeX_Buffer;
	      ChargeX.push_back( ChargeX_Buffer );    // cout << "ChargeX_Buffer = " << ChargeX_Buffer << endl;
	      Chargex[StrX-1] = ChargeX_Buffer ;     //cout <<" Chargex[" << StrX-1 << "] " << Chargex[StrX-1] << endl;
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

  if(ReconstructionMethodX[ff] == SECHS)CalculatedStripX = HyperbolicSequentMethod( Chargex, StripMaxX[ff] ); 
  if(ReconstructionMethodX[ff] == GAUSS)CalculatedStripX = GaussianMethodX(ff, Chargex, StripMaxX[ff]);
  if(ReconstructionMethodX[ff] == BAR3) CalculatedStripX = Barycentric3Method( Chargex, StripMaxX[ff] );
  if(ReconstructionMethodX[ff] == BAR4) CalculatedStripX = Barycentric4Method( Chargex, StripMaxX[ff] );
  if(ReconstructionMethodX[ff] == BAR5) CalculatedStripX = Barycentric5Method( Chargex, StripMaxX[ff] );

  if(CalculatedStripX < 35 && CalculatedStripX > -35)    { } //FailedReconstructionX[ff] = NO ; }
  //  else                                                   { FailedReconstructionX[ff] = ReconstructionMethodX[ff] ; } // cout << CalculatedStripX << endl;}
  
  //cout << "in AnalyseX : " << CalculatedStripX << endl;

  // else cout << "Error in the choice of the method!" << endl;
  
  //  cout << "AnalyseX done!" << endl ;
  return(CalculatedStripX);
}

double TCATSPhysics::AnalyseY(int ff,
			      int NumberOfCATSHit)
{


  //  cout << "AnalyseY proccessing ..." << endl;

  //  double	Chargey[28];
  for(unsigned short z=0; z<28; z++) {
    Chargey[z] = -1;
    //    Chargey_test[z] = -1;
  }
 
  // int HitY = 0;

  double ChargeSum_Y = 0 ;
  double ChargeY_Buffer = 0;

  double CalculatedStripY=0;
  
  for(UShort_t i =0; i<EventData->GetCATSMultY(); i++)
    {
      if( EventData->GetCATSDetY(i) == ff+1 )
	{ 
	  int NY	 = EventData	->	GetCATSDetY(i);
	  int StrY = EventData	->	GetCATSStripY(i) ;//	cout << NY << endl ; //" " << StrY << endl ;
	  
	  if(NY > 2 || StrY > 32)	cout << NY << " " << StrY << endl ;
	  
	  if(StrY < 28)
	    {	
	      double Q = EventData->GetCATSChargeY(i) + gRandom->Rndm() - Pedestal_Y[NY-1][StrY-1];
	      //ChargeY_Buffer = OnlineCalib_Y_E[NY-1][StrY-1][0] + Q * OnlineCalib_Y_E[NY-1][StrY-1][1] + Q*Q * OnlineCalib_Y_E[NY-1][StrY-1][2] ;
	      ChargeY_Buffer = CalibrationManager::getInstance()->ApplyCalibration("CATS/D"+itoa(NY)+"_Y"+itoa(StrY)+"_Q",Q);
	    }
	  
	  else {ChargeY_Buffer = 0 ;}
	  
	  if(EventData->GetCATSChargeY(i) > Threshold_Y[NY-1][StrY-1] && NY <= NumberOfCATSHit  && StrY < 28)
	    {
	      //   cout <<  Threshold_Y[NY-1][StrY-1] << endl;
	      
	      //	KNOWN INVERSION
	      if	(StrY == 15 && NY == 1) 	StrY = 16	;
	      else if (StrY == 16 && NY == 1) 		StrY = 15 	;
	      //else if (StrY == 4 && NY == 2) 		StrY = 14 	;
	      //else if (StrY == 14 && NY == 2) 	StrY = 4 	;
	      
	      MultOverThreshY[ff]++;
	      //ChargeSum += ChargeY_Buffer;
	      ChargeSum_Y += ChargeY_Buffer;

	      /*
	      ChargeY.push_back( ChargeY_Buffer );   //   cout << "ChargeY_Buffer = " << ChargeY_Buffer << endl;
	      Chargey[StrY-1] = ChargeY_Buffer ;   // cout <<" Chargey[" << StrY-1 << "] " << Chargey[StrY-1] << endl;
	      */

      
	      //Specific treatment for e530 experiment ///////////////////////////////////////////////////////////////////
	      // pist15 of cats2 not working...
	      
	      if(ff ==1 && StrY ==15) 
		{
		  ChargeY.push_back( -1 );   
		  Chargey[StrY-1] = -1 ;   
		}
	      
		
	      else{
		ChargeY.push_back( ChargeY_Buffer );   //   cout << "ChargeY_Buffer = " << ChargeY_Buffer << endl;
		Chargey[StrY-1] = ChargeY_Buffer ;   // cout <<" Chargey[" << StrY-1 << "] " << Chargey[StrY-1] << endl;
	      }
	      ////////////////////////////////////////////////////////////////////////////////////////////////////////////
	      
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
  
  if(ReconstructionMethodY[ff] == SECHS)CalculatedStripY = HyperbolicSequentMethod( Chargey, StripMaxY[ff] ); 
  if(ReconstructionMethodY[ff] == GAUSS)CalculatedStripY = GaussianMethodY(ff, Chargey, StripMaxY[ff]);
  if(ReconstructionMethodY[ff] == BAR3) CalculatedStripY = Barycentric3Method( Chargey, StripMaxY[ff] );
  if(ReconstructionMethodY[ff] == BAR4) CalculatedStripY = Barycentric4Method( Chargey, StripMaxY[ff] );
  if(ReconstructionMethodY[ff] == BAR5) CalculatedStripY = Barycentric5Method( Chargey, StripMaxY[ff] );

  if(CalculatedStripY < 35 && CalculatedStripY > -35)  { FailedReconstructionY[ff] = NO ; }
  else                                                   FailedReconstructionY[ff] = ReconstructionMethodY[ff] ;

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
      method = GAUSS;
    }
  }
  
  return(method);
} 

double  TCATSPhysics::CalculatePositionX( int                                 StripMaxX,
					  double*                             Chargex,
					  double                              CalculatedStripX, 
					  int                                 ff, 
					  correction                          method)
{
  double positionX=-1000;
  int IStripX = 0;

  if(ReconstructionMethodX[ff] == GAUSS) positionX = CalculatedStripX;   // already in mm -> see gaussian method

  else
    {
      // cout << "CalculatedStripX = " << CalculatedStripX  << endl;
      //  Integer part
      IStripX = (int) CalculatedStripX ;   
  
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
	      //if(ReconstructionMethodX[ff] == BAR3) positionX = CorrectedPositionX3(ff, positionX, StripMaxX, 0.6, StripPositionX);
	      //if(ReconstructionMethodX[ff] == BAR4) positionX = CorrectedPositionX4(ff, Chargex, positionX, StripMaxX, 0.77, StripPositionX );
	      if(ReconstructionMethodX[ff] == BAR3) positionX = CorrectedPositionX3(ff, positionX, StripMaxX, 0.6);
	      if(ReconstructionMethodX[ff] == BAR4) positionX = CorrectedPositionX4(ff, Chargex, positionX, StripMaxX, 0.77);
	    }

	  }
    
	else if(ff==1)    //CATS2
	  {
	    positionX = (DStripX)*2.54 + StripPositionX[ff][IStripX-1][0] ;
	    //	positionX =  2.54 * (CalculatedStripX-15) + 1.27 ;
	    //cout << "ecartX2 = " << positionX - ((DStripX)*2.54 + StripPositionX[ff][IStripX-1][0]) << endl;

	    if(method == NOcor) positionX = positionX;
	    else if(method == cor){
	      //if(ReconstructionMethodX[ff] == BAR3) positionX = CorrectedPositionX3(ff, positionX, StripMaxX, 0.53, StripPositionX);            
	      //if(ReconstructionMethodX[ff] == BAR4) positionX = CorrectedPositionX4(ff, Chargex, positionX, StripMaxX, 0.67, StripPositionX);
	      if(ReconstructionMethodX[ff] == BAR3) positionX = CorrectedPositionX3(ff, positionX, StripMaxX, 0.53);            
	      if(ReconstructionMethodX[ff] == BAR4) positionX = CorrectedPositionX4(ff, Chargex, positionX, StripMaxX, 0.67);
	    }
	  }
	else  cout << "only 2CATS!! ff = " << ff << endl;
      }
  
      else { positionX = -1000; } //  cout << CalculatedStripX << " " << IStripX << " " << ff << endl; }

      // cout << "positionX " << positionX << "   ff " << ff << "  IStripX " << IStripX <<endl;
    }
 
 
  return(positionX);
}


double  TCATSPhysics::CalculatePositionY( int                                 StripMaxY,
					  double *                            Chargey,
					  double                              CalculatedStripY, 
					  int                                 ff, 
					  correction                          method)
{
  double positionY = -1000;

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
	      if(ReconstructionMethodY[ff] == BAR3) positionY = CorrectedPositionY3(ff, positionY, StripMaxY, 0.6);
	      if(ReconstructionMethodY[ff] == BAR4) positionY = CorrectedPositionY4(ff, Chargey, positionY, StripMaxY, 0.75);
	      //if(ReconstructionMethodY[ff] == BAR3) positionY = CorrectedPositionY3(ff, positionY, StripMaxY, 0.6, StripPositionY);
	      //if(ReconstructionMethodY[ff] == BAR4) positionY = CorrectedPositionY4(ff, Chargey, positionY, StripMaxY, 0.75, StripPositionY);
	    }
	  }
	  
	  else if(ff ==1){
	    if(method == NOcor) positionY = positionY;
	    else if(method == cor){
	      if(ReconstructionMethodY[ff] == BAR3) positionY = CorrectedPositionY3(ff, positionY, StripMaxY, 0.57);
	      if(ReconstructionMethodY[ff] == BAR4) positionY = CorrectedPositionY4(ff, Chargey, positionY, StripMaxY, 0.7);
	      //if(ReconstructionMethodY[ff] == BAR3) positionY = CorrectedPositionY3(ff, positionY, StripMaxY, 0.57, StripPositionY);
	      //if(ReconstructionMethodY[ff] == BAR4) positionY = CorrectedPositionY4(ff, Chargey, positionY, StripMaxY, 0.7, StripPositionY);
	    }
	  }
	  
	  else cout << "only 2CATS!! ff = " << ff << endl;
	}
      
      else  { positionY = -1000   ;  } // cout << CalculatedStripY << " " << IStripY << " " << ff << endl; }
      // cout << IStripX << " " << IStripY << endl;
      
    }
  
  return(positionY);
  
}

double TCATSPhysics:: HyperbolicSequentMethod( double* Charge , int StripMax )
{
  double sechs = -1000 ;
  
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

  else { 
    sechs = -1000; //cout << " StripMax = " << StripMax << " out of range for SECHS !" << endl;  
  }


  // cout << "sechs = " << sechs << endl;

  return sechs ;
}


double TCATSPhysics:: GaussianMethodX(int ff, double* Chargex , int StripMax)//, vector< vector< vector<double> > >  &StripPositionX)
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

  else {
    gauss = -1000;
    // cout << "Gaussian method X failed ! " << endl;

  }

  return gauss;
  
}


double TCATSPhysics:: GaussianMethodY(int ff, double* Chargey , int StripMax)
{
  double Q[3];
  double StripPos[3];

  double Q0_Q1, Q0_Q2;
  double num, denom;

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
      

      Q0_Q1 = log(Q[0]/Q[1]);
      Q0_Q2 = log(Q[0]/Q[2]);
      
      num   = Q0_Q1 * (StripPos[2]*StripPos[2] - StripPos[0]*StripPos[0]) - Q0_Q2 * (StripPos[1]*StripPos[1] - StripPos[0]*StripPos[0])  ;
      denom = Q0_Q1 * (StripPos[2] - StripPos[0]) - Q0_Q2 * (StripPos[1] - StripPos[0]) ;
      gauss = 0.5*num / denom;

    }

  else {
    gauss = -1000;
    //cout << "Gaussian method Y failed ! " << endl;
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


double TCATSPhysics::CorrectedPositionX3(int ff, double Position, int StripMax, double a) 
{
  double Corrected_Position = 0;
  int StripMax_ = StripMax -1;
  double xmax = StripPositionX[ff][StripMax_][0] ;
  // cout << "xmax2 " << xmax << endl;
  Corrected_Position = (Position - xmax) / a + xmax;
  
  return Corrected_Position;
}

double TCATSPhysics::CorrectedPositionY3(int ff, double Position, int StripMax, double a) 
{
  double Corrected_Position = 0;
  int StripMax_ = StripMax -1;
  double xmax = StripPositionY[ff][0][StripMax_];
  // cout << "xmax2 " << xmax << endl;
  Corrected_Position = (Position - xmax) / a + xmax;
  
  return Corrected_Position;
}

double TCATSPhysics::CorrectedPositionX4(int ff, double* Charge, double Position, int StripMax, double b) 
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

double TCATSPhysics::CorrectedPositionY4(int ff, double* Charge, double Position, int StripMax, double b) 
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

TVector3 TCATSPhysics::GetPositionOnTarget()
{
  TVector3 Position = TVector3 (GetPositionOnTargetX() 	,
				GetPositionOnTargetY() 	,
				0                     );  // a modifier !! passer la distance de la cible
  return(Position) ;	
}

double TCATSPhysics::GetCATSChargeSumX(int i)
{
  double ChargeSum = 0;
  
  for(unsigned int i = 0; i < ChargeSumX.size(); i++)
    {
      if     (DetNumberX.at(i) == 1) ChargeSum = ChargeSumX.at(i);
      else if(DetNumberX.at(i) == 2) ChargeSum = ChargeSumX.at(i);
    }

  return(ChargeSum);
}

int TCATSPhysics::GetCATSMultOverThreshX(int i)    
{
  int MultOverThresh = 0;
  
  for(unsigned int i = 0; i < MultOverThreshX.size(); i++)
    {
      if     (DetNumberX.at(i) == 1)  MultOverThresh =  MultOverThreshX.at(i);
      else if(DetNumberX.at(i) == 2)  MultOverThresh =  MultOverThreshX.at(i);
    }

  return MultOverThresh;
}

int TCATSPhysics::GetCATSStripMaxX(int i)
{
  int StripMax = 0;
  
  for(unsigned int i = 0; i < StripMaxX.size(); i++)
    {
      if     (DetNumberX.at(i) == 1)  StripMax =  StripMaxX.at(i);
      else if(DetNumberX.at(i) == 2)  StripMax =  StripMaxX.at(i);
    }

  return StripMax;
}

int TCATSPhysics::GetCATSDetNumberX_Position(int i)
{
  int DetNumber_Position = 0;
  
  for(unsigned int i = 0; i < DetNumberX_Position.size(); i++)
    {
      if     (DetNumberX.at(i) == 1)  DetNumber_Position =  DetNumberX_Position.at(i);
      else if(DetNumberX.at(i) == 2)  DetNumber_Position =  DetNumberX_Position.at(i);
    }

  return DetNumber_Position;
}

double TCATSPhysics::GetCATSPositionX(int i)
{
  double Position = 0;

  for(unsigned int i = 0; i < PositionX.size(); i++)
    {
      if     (DetNumberX.at(i) == 1)  Position =  PositionX.at(i);
      else if(DetNumberX.at(i) == 2)  Position =  PositionX.at(i);
    }

  return Position;
}


double TCATSPhysics::GetCATSChargeSumY(int i)
{
  double ChargeSum = 0;
  
  for(unsigned int i = 0; i < ChargeSumY.size(); i++)
    {
      if     (DetNumberY.at(i) == 1) ChargeSum = ChargeSumY.at(i);
      else if(DetNumberY.at(i) == 2) ChargeSum = ChargeSumY.at(i);
    }

  return(ChargeSum);
}

int TCATSPhysics::GetCATSMultOverThreshY(int i)    
{
  int MultOverThresh = 0;
  
  for(unsigned int i = 0; i < MultOverThreshY.size(); i++)
    {
      if     (DetNumberY.at(i) == 1)  MultOverThresh =  MultOverThreshY.at(i);
      else if(DetNumberY.at(i) == 2)  MultOverThresh =  MultOverThreshY.at(i);
    }

  return MultOverThresh;
}

int TCATSPhysics::GetCATSStripMaxY(int i)
{
  int StripMax = 0;
  
  for(unsigned int i = 0; i < StripMaxY.size(); i++)
    {
      if     (DetNumberY.at(i) == 1)  StripMax =  StripMaxY.at(i);
      else if(DetNumberY.at(i) == 2)  StripMax =  StripMaxY.at(i);
    }

  return StripMax;
}

int TCATSPhysics::GetCATSDetNumberY_Position(int i)
{
  int DetNumber_Position = 0;
  
  for(unsigned int i = 0; i < DetNumberY_Position.size(); i++)
    {
      if     (DetNumberY.at(i) == 1)  DetNumber_Position =  DetNumberY_Position.at(i);
      else if(DetNumberY.at(i) == 2)  DetNumber_Position =  DetNumberY_Position.at(i);
    }

  return DetNumber_Position;
}

double TCATSPhysics::GetCATSPositionY(int i)
{
  double Position = 0;

  for(unsigned int i = 0; i < PositionY.size(); i++)
    {
      if     (DetNumberY.at(i) == 1)  Position =  PositionY.at(i);
      else if(DetNumberY.at(i) == 2)  Position =  PositionY.at(i);
    }

  return Position;
}



namespace LOCAL_CATS
{
  //	tranform an integer to a string
  string itoa(int value)
  {
    std::ostringstream o;
    
    if (!(o << value))
      return ""	;
    
    return o.str();
  }
}
