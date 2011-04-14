#ifndef __CATSCAL__
#define __CATSCAL__
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

//   STL
#include <vector>

//   ROOT
#include "TObject.h"
#include "TVector3.h"

//   NPLib
#include "TCATSData.h"
#include "../include/VDetector.h"
#include "../include/CalibrationManager.h"

using namespace std ;

/* J'aime pas trop cette partie, je pense que deja ca pourrait etre mieux une variable interne te disant quel methode tu as utiliser
et d'ailleur d'ecrire cette varaible dans l'arbre de sortie pour une question de tracabilite.
Ensuite tu peux faire un Set et un Get de cette variable (je preconise un string avec un nom completement lisible... :p ).
Ensuite dans ton build tu appelle une methode unique, qui elle appellera la methode correcte apres avoir fait les tests...

Si apres tu veux vraiment ameliorer les performances le mieux est de definir un pointer de fonction que tu appelle a chaque event... 
mais c'est un peu plus complique,
voila un liens si jamais ca t'interresse: http://www.newty.de/fpt/intro.html

Ceci dit ce n'est que des points de detail.
*/

enum reconstruction{NO,SECHS,GAUSS,BAR3,BAR4,BAR5};
//enum correction{BAR3cor,BAR4cor,NOcor,GAUSScor};
enum correction{NOcor,cor};

class TCATSPhysics : public TObject, public NPA::VDetector
{
  
 public:   //   Constructor and Destructor
  TCATSPhysics();
  ~TCATSPhysics();
   
 public:   // Output data of interest
  //for a CATS

  // marker of the cats used
  int ff ;

  //   Vector of dim = multiplicity of event on all detector
  vector<int>        DetNumberX; 
  vector<int>        StripX;
  vector<double>     ChargeX; 
     
  //   Vector of dim = number of CATS
  vector<double>   ChargeSumX;
  vector<int>      MultOverThreshX;  
  vector<int>      StripMaxX;
     
     
  //   Vector of dim = multiplicity of event on all detector
  vector<int>        DetNumberY; 
  vector<int>        StripY;
  vector<double>     ChargeY;
  //  vector<double>   ChargeY_test   ;
     
  //   Vector of dim = number of CATS
  vector<double>    ChargeSumY;  
  vector<int>       MultOverThreshY;
  vector<int>       StripMaxY;
  //    vector<int>       StripMaxY_test;
     
  //   Calculate
  vector<int>       DetNumberX_Position;
  vector<int>       DetNumberY_Position;
  vector<int>       DetNumberZ_Position;
  vector<double>    PositionX;
  vector<double>    PositionY;
  vector<double>    PositionZ;
  double            PositionOnTargetX;
  double            PositionOnTargetY;
     
  TVector3      BeamDirection      ;  //!

  double                Chargex[28];  //!
  double                Chargey[28];  //!
      
  int HitX;    //!
  int HitY;    //!

  vector<reconstruction>   ReconstructionMethodX;
  vector<reconstruction>   ReconstructionMethodY;

    
  // vector<reconstruction>       FailedReconstructionX;
  vector<reconstruction> FailedReconstructionY;
                                  
 private:   //   Root Input and Output tree classes
     
  TCATSData*      EventData;//!
  TCATSPhysics*   EventPhysics;//!
     
 public:      //   Innherited from VDetector Class
     
   //   Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
   void ReadConfiguration(string);
     
   //   Add Parameter to the CalibrationManger
   void AddParameterToCalibrationManager();      
     
   //   Activated associated Branches and link it to the private member DetectorData address
   //   In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
   void InitializeRootInputRaw() ;

   //   Activated associated Branches and link it to the private member DetectorPhysics address
   //   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
   void InitializeRootInputPhysics() ;
   
   //   Create associated branches and associated private member DetectorPhysics address
   void InitializeRootOutput() ;
     
   //   This method is called at each event read from the Input Tree. Aim is to build treat Raw dat in order to extract physical parameter. 
   void BuildPhysicalEvent();
     
   //   Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
   //   This method aimed to be used for analysis performed during experiment, when speed is requiered.
   //   NB: This method can eventually be the same as BuildPhysicalEvent.
   void BuildSimplePhysicalEvent();
   
   // Same as above but for online analysis
   void BuildOnlinePhysicalEvent()  {BuildPhysicalEvent();};

   //   Those two method all to clear the Event Physics or Data
   void ClearEventPhysics() {Clear();}      
   void ClearEventData()    {EventData->Clear();}      

   // Give and external TMustData object to TMust2Physics. Needed for online analysis for example.
   void SetRawDataPointer(TCATSData* rawDataPointer) {EventData = rawDataPointer;}


 private :

  // redundant information : could be optimized in the future
  vector< vector< vector<double> > >   StripPositionX;//!
  vector< vector< vector<double> > >   StripPositionY;//!
  vector<double>                       StripPositionZ;//!  
  int NumberOfCATS;//!   

  vector< vector <double> >   Pedestal_X;//!
  vector< vector <double> >   Pedestal_Y;//!
     
  vector< vector <double> >   Threshold_X;//!     
  vector< vector <double> >   Threshold_Y;//!

      
 public :   //   Specific to CATS

  void   Clear();
  void  Clear(const Option_t*) {};
  void   Dump() const;
     
  void AddCATS(TVector3 C_X1_Y1, TVector3 C_X28_Y1, TVector3 C_X1_Y28, TVector3 C_X28_Y28);

  void ReadPedestal(string PedestalPath);

  double AnalyseX(int ff,int NumberOfDetector);

  double AnalyseY(int ff,int NumberOfDetector);

  double CalculatePositionX( double CalculatedStripX, correction method);

  double CalculatePositionY( double CalculatedStripY, correction method);


  reconstruction ChooseReconstruction(TString type);

  //   Calculate Strip touch using an array of Charge on Strip and Strip with Maximum Charge
     
  double HyperbolicSequentMethodX();
  double GaussianMethodX();
  double Barycentric5MethodX(); 
  double Barycentric4MethodX(); 
  double Barycentric3MethodX(); 

  double HyperbolicSequentMethodY();
  double GaussianMethodY();
     
  double Barycentric5MethodY(); 
  double Barycentric4MethodY(); 
  double Barycentric3MethodY(); 
     
     
  double CorrectedPositionX3(double Position, double a) ;
  double CorrectedPositionY3(double Position, double a) ;
  double CorrectedPositionX4(double Position, double b); 
  double CorrectedPositionY4(double Position, double b); 
    
  // X

  //   Vector of dim = multiplicity of event on all detector
  int       GetCATSDetNumberX(int i)         {return DetNumberX.at(i);}
  int       GetCATSStripX(int i)             {return StripX.at(i);}
  double    GetCATSChargeX(int i)            {return ChargeX.at(i);}
     
  int       GetCATSMultX()                {return DetNumberX.size();}
    
  //   Vector of dim = number of CATS
  double    GetCATSChargeSumX(int i)          ;
  int        GetCATSMultOverThreshX(int i)     ;
  int       GetCATSStripMaxX(int i)           ;
  // int       GetCATSDetNumberX_Position(int i) ;
  double    GetCATSPositionX(int i)           ;

  double    GetPositionOnTargetX()           {return PositionOnTargetX;}

  // Y

  //   Vector of dim = multiplicity of event on all detector
  int       GetCATSDetNumberY(int i)         {return DetNumberY.at(i);}
  int       GetCATSStripY(int i)             {return StripY.at(i);}
  double    GetCATSChargeY(int i)            {return ChargeY.at(i);}

  int       GetCATSMultY()                {return DetNumberY.size();}
    
  //   Vector of dim = number of CATS
  double    GetCATSChargeSumY(int i)         ;
  int        GetCATSMultOverThreshY(int i)    ;
  int       GetCATSStripMaxY(int i)          ;
  //  int       GetCATSDetNumberY_Position(int i);
  double    GetCATSPositionY(int i)          ;
     
  double    GetPositionOnTargetY()           {return PositionOnTargetY;}

  int       GetCATSMult()                    {return PositionX.size();}

  TVector3  GetPositionOnTarget();
  TVector3  GetBeamDirection()               {return BeamDirection;}

  ClassDef(TCATSPhysics,1)  // CATSPhysics structure
    };





namespace LOCAL_CATS
{
  //   tranform an integer to a string
  string itoa(int value);

}

#endif
