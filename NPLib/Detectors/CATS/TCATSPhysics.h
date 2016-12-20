#ifndef __CATSCAL__
#define __CATSCAL__
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
#include <TRandom1.h>
#include <TRandom2.h>
#include <TRandom3.h>
//   NPLib
#include "TCATSData.h"
#include "TCATSSpectra.h"
#include "NPVDetector.h"
#include "NPCalibrationManager.h"
#include "NPDetectorFactory.h"
#include "NPInputParser.h"
// forward declaration
class TCATSSpectra;
using namespace std ;

class TCATSPhysics : public TObject, public NPL::VDetector
{

  public:   //   Constructor and Destructor
    TCATSPhysics();
    ~TCATSPhysics();

  private:   //   Root Input and Output tree classes

    TCATSData*       m_EventData;//!
    TCATSData*       m_PreTreatedData;//!
    TCATSPhysics*    m_EventPhysics;//!

  public :
    //   Vector of dim = multiplicity of event on all detector
    vector<int>      DetNumberX; 
    vector<int>      StripX;
    vector<double>   ChargeX; 

    //   Vector of dim = number of CATS
    vector<int>      StripMaxX;
    vector<double>   ChargeMaxX;
    vector<int>      DetMaxX;

    //   Vector of dim = multiplicity of event on all detector
    vector<int>      DetNumberY; 
    vector<int>      StripY;
    vector<double>   ChargeY;

    //   Vector of dim = number of CATS  
    vector<int>      StripMaxY;
    vector<double>   ChargeMaxY;
    vector<int>      DetMaxY;

    //   Vector of dim = number of CATS
    vector<double>   PositionX;
    vector<double>   PositionY;
    vector<double>   StripNumberX;
    vector<double>   StripNumberY;
    vector<double>   PositionZ;
    vector<double>	 QsumX;
    vector<double>	 QsumY;
    double           PositionOnTargetX;
    double           PositionOnTargetY;

    TVector3         BeamDirection;//!

    // Vector of Charge Array (one for each CATS fired)
    vector< vector<double> > Buffer_X_Q;//!
    vector< vector<double> > Buffer_Y_Q;//!

  private :
    vector< vector< vector<double> > >   StripPositionX;//!
    vector< vector< vector<double> > >   StripPositionY;//!
    vector<double>                       StripPositionZ;//!  
    int m_NumberOfCATS;
    double m_TargetAngle; //!
    double m_TargetThickness; //!

    // Those two vector contain a pointer to the reconstruction function that should be used for each detector 
    // Method take as argument a vector<double> representing the Charge distribution and uint giving the Strip with Max Q
    // It returns a double, corresponding to the position in Strip unit
    // Methods are declared in the CATS namespace outside the class
    // CINT can't handle such complex vector, so the ifndef part is there to hide it away 
    #ifndef __CINT__
    vector<double(*)(vector<double>&,int&)> ReconstructionFunctionX;//!
    vector<double(*)(vector<double>&,int&)> ReconstructionFunctionY;//!
    #endif /* __CINT __ */
  
  public:
    // Set the reconstruction Method used for the CATS plane
    void SetReconstructionMethod(unsigned int CATSNumber, string XorY, string MethodName);

  private : 
    //   Map of activated channel
    map< int, vector<bool> > m_XChannelStatus;//!
    map< int, vector<bool> > m_YChannelStatus;//! 
    //   Map of inverted channel
    map< int, vector<int> > m_CATSXInversion;//!
    map< int, vector<int> > m_CATSYInversion;//! 

  public:   // Output data of interest
    //   for a CATS
    void SetTargetAngle(double m_TargetAngle) {m_TargetAngle = m_TargetAngle;}
    void SetTargetThickness(double m_TargetThickness) {m_TargetThickness = m_TargetThickness;}


    //   Remove bad channel, calibrate the data and apply threshold
    void PreTreat();

    //   Activated associated Branches and link it to the private member DetectorData address
    //   In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
    void InitializeRootInputRaw() ;

    //   Activated associated Branches and link it to the private member DetectorPhysics address
    //   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
    void InitializeRootInputPhysics() ;

    //   Create associated branches and associated private member DetectorPhysics address
    void InitializeRootOutput() ;

    //   Clear The PreTeated object
    void ClearPreTreatedData()   {m_PreTreatedData->Clear();}

    void BuildPhysicalEvent();

    void BuildSimplePhysicalEvent();

    // Method related to the TSpectra classes, aimed at providing a framework for online applications
    // Instantiate the Spectra class and the histogramm throught it
    void InitSpectra();
    // Fill the spectra hold by the spectra class
    void FillSpectra();
    // Used for Online mainly, perform check on the histo and for example change their color if issues are found
    void CheckSpectra();
    // Used for Online only, clear all the spectra hold by the Spectra class
    void ClearSpectra();


    //   Those two method all to clear the Event Physics or Data
    void ClearEventPhysics() {Clear();}      
    void ClearEventData()    {m_EventData->Clear();}    

    void  Clear();
    void  Clear(const Option_t*) {};  

    // Give and external TCATSData object to TCATSPhysics, needed for online analysis
    void SetRawDataPointer(TCATSData* rawDataPointer) {m_EventData = rawDataPointer;}

    //   Return false if the channel is disabled by user
    bool IsValidChannel(const string DetectorType, const int Detector , const int channel);
    void InitializeStandardParameter();
    void AddParameterToCalibrationManager();
    void ReadAnalysisConfig();
    void ReadConfiguration(NPL::InputParser);
    void AddCATS(TVector3 C_X1_Y1, TVector3 C_X28_Y1, TVector3 C_X1_Y28, TVector3 C_X28_Y28);

  public:
    TVector3 GetBeamDirection();
    TVector3 GetPositionOnTarget();

    double GetPositionOnTargetX()  {return PositionOnTargetX;}  
    double GetPositionOnTargetY()  {return PositionOnTargetY;}


  private: // Spectra Class   
    TCATSSpectra*      m_Spectra;//! 

  public: 
    void WriteSpectra() ;//!

  public: // Spectra Getter
    map< string , TH1*> GetSpectra();

    public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
     ClassDef(TCATSPhysics,1)  // CATSPhysics structure
};


namespace CATS_LOCAL{
  // The reconstruction methods need to be outside the class for practical purpose
  // of dealing with the function pointer
  double AnalyticHyperbolicSecant(vector<double>& ChargeArray,int& StripMax);
  double FittedHyperbolicSecant(vector<double>& ChargeArray,int& StripMax);
  double AnalyticGaussian(vector<double>& ChargeArray,int& StripMax);
  double Centroide(vector<double>& ChargeArray,int& StripMax); 


  //   tranform an integer to a string
  string itoa(int value);

  double fCATS_X_Q(const TCATSData* Data, const int i);
  double fCATS_Y_Q(const TCATSData* Data, const int i);
  bool fCATS_Threshold_X(const TCATSData* Data, const int i);
  bool fCATS_Threshold_Y(const TCATSData* Data, const int i);
  double fCATS_Ped_X(const TCATSData* m_EventData, const int i);
  double fCATS_Ped_Y(const TCATSData* m_EventData, const int i);
}

#endif
