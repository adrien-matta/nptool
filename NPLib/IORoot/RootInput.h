#ifndef ROOTINPUT_HH
#define ROOTINPUT_HH

// C++ header
#include <string>

// ROOT headers
#include "TFile.h"
#include "TChain.h"
#include "TList.h"

using namespace std;


class RootInput
{
public:
   // The analysis class is designed to be a singleton (i.e. only one instance
   // can exist). A member function called Instance is defined, which allows
   // the user to get a pointer to the existing instance or to create it if
   // it does not yet exist:
   // (see the constructor for an explanation of the arguments)
   static RootInput* getInstance(string configFileName = "configFile");

   // The analysis class instance can be deleted by calling the Destroy
   // method (NOTE: The class destructor is protected, and can thus not be
   // called directly):
   static void Destroy();

protected:
   // Constructor (protected)
   RootInput(string configFileName);

   // Destructor (protected)
   virtual ~RootInput();

   // Prevent copying
   RootInput(const RootInput& only);
   const RootInput& operator=(const RootInput& only);

private:
   // The static instance of the RootInput class:
   static RootInput* instance;

public:
   TChain*	GetChain() {return pRootChain;};

private:
   TChain	*pRootChain;
};

#endif // ROOTINPUT_HH
