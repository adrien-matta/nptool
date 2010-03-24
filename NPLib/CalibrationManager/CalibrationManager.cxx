/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : october 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This class manage the calibration coefficient of the detector in NPA      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   This class is a singleton                                               *
 *                                                                           *
 *****************************************************************************/
#include "CalibrationManager.h"

//	STL
#include <cstdlib>
#include <limits>
#include <cmath>
#include <sstream>

//////////////////////////////////////////////////////////////////
CalibrationManager* CalibrationManager::instance = 0;

CalibrationManager* CalibrationManager::getInstance(string configFileName)
	{
	   // A new instance of CalibrationManager is created if it does not exist:
	   if (instance == 0) {
	      instance = new CalibrationManager(configFileName);
	   }

	   // The instance of CalibrationManager is returned:
	   return instance;
	}
	
//////////////////////////////////////////////////////////////////
CalibrationManager::CalibrationManager(string configFileName)
	{
   // Read configuration file Buffer
   string lineBuffer, dataBuffer;

   // Open file
   ifstream inputConfigFile;
   inputConfigFile.open(configFileName.c_str());
	
   cout << "/////////////////////////////////" << endl;
   cout << "Getting list of Calibration File" << endl;

   if (!inputConfigFile) {
      cout << "Calibration Path file :" << configFileName << " not found " << endl; 
      return;
   }
   
   else 
		{
   		cout << "Reading list of file from :" << configFileName << endl; 
      while (!inputConfigFile.eof()) {
         getline(inputConfigFile, lineBuffer);
			      
         // search for token giving the list of Root files to treat
         if ( lineBuffer.compare(0, 19, "CalibrationFilePath") == 0 ) {
             while (!inputConfigFile.eof()) {
               inputConfigFile >> dataBuffer;
								
               // ignore comment Line 
               if (dataBuffer.compare(0, 1, "%") == 0) {
                  inputConfigFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
               }

               else if (!inputConfigFile.eof()) {
                  AddFile(dataBuffer);
                  cout << "Adding file " << dataBuffer << " to Calibration" << endl;
               }
	            }
	         }
	      }
	  	}
   	cout << "/////////////////////////////////" << endl;
   }
	
//////////////////////////////////////////////////////////////////
CalibrationManager::~CalibrationManager()
	{}
	
//////////////////////////////////////////////////////////////////
bool CalibrationManager::AddParameter(string DetectorName , string ParameterName , string Token )
	{
	  string ParameterPath = DetectorName + "/" + ParameterName ;
	  fToken[Token] = ParameterPath ;
	  return true;
	}

//////////////////////////////////////////////////////////////////
void CalibrationManager::LoadParameterFromFile()
	{
		ifstream CalibFile 	;
		string	 DataBuffer	;
		string   LineBuffer ;
		
		for(unsigned int i = 0 ; i < fFileList.size() ; i++)
			{
			  cout << fFileList[i].c_str() << endl;

				CalibFile.open( fFileList[i].c_str() );
				map<string,string>::iterator it ;
				
				if(!CalibFile)
					{
						cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;
						cout << " WARNING: FILE " << fFileList[i] << " IS MISSING "				  														<< endl ;
						cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;
					}
					
				else while( !CalibFile.eof() )
					{
						// Read the file Line by line
						getline(CalibFile, LineBuffer);
						
						// Create a istringstream to manipulate the line easely
					  istringstream theLine (LineBuffer,istringstream::in);
						theLine >> DataBuffer ;
						
						// Comment support, comment symbole is %
						if(DataBuffer.compare(0, 1, "%") == 0) {
						 	CalibFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
						 	
						//	Search word in the token list
						it=fToken.find(DataBuffer);
						
						//	if the word is find, values are read
						if( it!=fToken.end() )
							{
								vector<double> Coeff ;
								while( !theLine.eof() )
									{
										theLine >> DataBuffer ; 
										Coeff.push_back( atof(DataBuffer.c_str()) ) ;
										//	cout << atof(DataBuffer.c_str()) << endl;
									}
									
								//	Check this parameter is not already define
								if( fCalibrationCoeff.find(it->second) != fCalibrationCoeff.end() ) 
									cout << "WARNING: Parameter " << it->second << " Already found. It will be rewritted " << endl;
										
								//	Add the list of Coeff to the Coeff map using Parameter Path as index
								fCalibrationCoeff[ it->second ] = Coeff ;
							}
								
					}
				CalibFile.close() ;
			}
	}

//////////////////////////////////////////////////////////////////
double CalibrationManager::ApplyCalibration(string ParameterPath , double RawValue)
	{
		double CalibratedValue = 0 ;
		map< string , vector<double> >::iterator it ;
		
		//	Find the good parameter in the Map
		// Using Find method of stl is the fastest way
		it = fCalibrationCoeff.find(ParameterPath)  ;
		
		// If the find methods return the end iterator it's mean the parameter was not found
		if(it == fCalibrationCoeff.end() )
			{
			  /*
			  cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;
			  cout << " ERROR: PARAMETER " << ParameterPath << " IS NOT FOUND IN THE CALIBRATION DATA BASE  " << endl ;
			  cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl ;
			  */
			  return RawValue ;
			}
		
		// Else we take the second part of the element (first is index, ie: parameter path)
		// Second is the vector of Coeff
		vector<double> Coeff = it->second  ;
		
		// The vector size give the degree of calibration
		// We just apply the coeff and returned the calibrated value
		
		for(unsigned int i = 0 ; i < Coeff.size() ; i++)
			{
				CalibratedValue += Coeff[i]*pow(RawValue, (double)i);
			}
			
		return CalibratedValue ;
		
	}
//////////////////////////////////////////////////////////////////


