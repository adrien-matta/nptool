#include <iostream>
#include <fstream>

#include "RootInput.h"


RootInput *RootInput::instance = 0;

RootInput* RootInput::getInstance(string configFileName)
{
   // A new instance of RootInput is created if it does not exist:
   if (instance == 0) {
      instance = new RootInput(configFileName);
   }

   // The instance of RootInput is returned:
   return instance;
}


void RootInput::Destroy()
{
   if (instance != 0) {
      delete instance;
      instance = 0;
   }
}


// fileNameBase doit etre le nom du TChain.
RootInput::RootInput(string configFileName)
{
	bool CheckTreeName 		= false	;
	bool CheckRootFileName 	= false ;

	// Read configuration file Buffer
   string lineBuffer, dataBuffer;

   // Open file
   ifstream inputConfigFile;
   inputConfigFile.open(configFileName.c_str());
	
   pRootChain = new TChain()	;
   
   if(!inputConfigFile) { cout << "Run to Read file :" << configFileName << " not found " << endl ; return ;}
   
   else 
		{
			while (!inputConfigFile.eof()) 
				{
				      getline(inputConfigFile, lineBuffer);
				      
				      // search for token giving the TTree name
				      if (lineBuffer.compare(0, 9, "TTreeName") == 0) 
				      	{
					         inputConfigFile >> dataBuffer;
					         // initialize pRootChain
					         pRootChain->SetName(dataBuffer.c_str());
					         CheckTreeName = true ;
				      	}
				      
				      // search for token giving the list of Root files to treat
				      else if (lineBuffer.compare(0, 12, "RootFileName") == 0  &&  pRootChain)
				      	{
				      		inputConfigFile >> dataBuffer;
					         while (!inputConfigFile.eof()) 
					         	{
						            inputConfigFile >> dataBuffer;
						            pRootChain->Add(dataBuffer.c_str());
						            CheckRootFileName = true ;
				         		}
				         
				      	}
		   		}
		
		}
		   
   
   if(!CheckRootFileName || !CheckTreeName) cout << "WARNING: Token not found for InputTree Declaration : Input Tree may not be instantiate properly" << endl ;
   
   
}



RootInput::~RootInput()
{
   delete pRootChain;
}
