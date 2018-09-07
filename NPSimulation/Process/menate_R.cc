//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
// menate_R.cc 
//
// Description - contains member functions declared in menate_R.hh
//
// started on 6 May 2008
//
// Written by: Brian Roeder, Postdoc, LPC Caen, Texas A&M Univ. 
// email - broeder@comp.tamu.edu
//
// -Kinematics processes in PostStepDoIt translated from FORTRAN code
//  "MENATE" by P. Desesquelles et al., NIM A 307 (1991) 366.
// -MeanFreePath and Reaction Selection based on discussions with JL Lecouey
// -Uses cross sections from MENATE, but these can be modified by changing
//  the data files.
// -Modifications : Uses non-isotropic angular distributions for
//                  other processes.
//---------------------------------------------------------------------
// Version Comments - (other changes/fixes noted in menate_R.hh)
//  
// 23 April 2008 - version 1.0 -> working version -> by Brian Roeder
// modified functions and cross section read-in functions
// added all reactions considered in original MENATE including:
// n+H -> n+p elastic
// n+12C -> n+12C elastic
// n+12C -> n'+12C+gamma(4.4MeV)
// n+12C -> a+9Be
// n+12C -> n'+ 3a
// n+12C -> p + 12B
// n+12C -> n' + p + 11B (breakup)
// n+12C -> n' + n + 11C (breakup)
// All reactions considered together for MeanFreePath and
// reaction probability consideration
// **** This version includes fix in "Available_Eng" for
// Alpha2 in process 12C(n,n')3a (bug in FORTRAN version of MENATE)
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// 19 May 2008 - new version with modifications - BTR
//             - Modified angular distributions for n+p, n+12C elastic
// other changes listed in menate_R.hh.
//------------------------------------------------------------------
// 2011-2012 Zach Kohley Modifications
// To allow for the Sweeper magnet and sweeper chamber to be included
// in the simulation needed to add Al and Fe materials into Menate_R
// only elastic and non-elastic process are included.  Non-elastic are
// essentially absorption (neutron is killed).
// Angular distribution for 30, 50 and 70 MeV are included for the elastic
// scattering of neutron off Fe and Al.
//-----------------------------------------------------------------------
//
// 6 Sept 2018
// Added to NPTool package by Greg Christian (TAMU), gchristian@tamu.edu
// Code inherited from MoNA-simulation package,
// https://github.com/baumann3141/MoNA-simulation (cloned 6 Sept 2018, 10:20 AM CDT
//
// Changes Made w.r.t. MoNA-simulation version.  Changes were required to make menate_R.cc
// compile within the NPSimulation framework
//    1.  Added "using namespace CLHEP" to top of file
//    2.  Added explicit std:: before call to std::isnan
//    3.  Changed G4ParticleTable::GetParticleTable()->GetIon(...) to
//        G4IonTable::GetIonTable()->GetIon(...).  The former is not any
//        longer available for GEANT v10.0 and up.  Also required adding
//        #include "G4IonTable.hh" to menate_R.hh
//    4.  Changed menate_R::ReadCrossSectionFile() to look in
//        $NPTOOL/Inputs/CrossSection/MENATE_R
//    5.  Local variable ElementName changed to ElementName_ to get rid of warning
//    6.  Commented declaration of DiffNorm variable (use was already commented)
//    7.  Changed inner XEV -> XEV1 in Evaporate_Eng, to get rid of compiler warning
//    8.  Got rid of previousStepSize in GetMeanFreePath (remove unused variable warning)
//    9.  Got rid of declaration of unused T_Feel variable (in 2 places)
//    10. Using Thomass Baumann's / Nathan Frank's modified 12C(n,np)11Be cross section file
//        (fixes mistake in the original)
//

#include "menate_R.hh"
#include <cmath>
#include <iomanip>
using namespace CLHEP;

menate_R::menate_R(const G4String& processName) : G4VDiscreteProcess(processName)
{
  Pi = CLHEP::pi;
  Two_Pi = 2.*Pi;

  AMass_Material = 12.; // Atomic Mass of C12 for certain inelastic reactions
                        // If Used with materials other than C12, need to
                        // give atomic Mass here or in PostStepDoIt Method (as for NE213)

  CalcMeth="ORIGINAL";

  G4cout << "Constructor for menate_R process was called! " << G4endl;
  G4cout << "A non-relativistic model for n - scattering ";
  G4cout << "on carbon and hydrogen or materials composed of it (e.g. NE213)" << G4endl;
  G4cout << "You are using MENATE_R version 1.4, finished 19 May 2008 - BTR" << G4endl;

  Setup_FeElastic_AngDist();
  Setup_AlElastic_AngDist();
  
  const G4MaterialTable* theMaterialList = G4Material::GetMaterialTable();
  G4int NumberOfMaterials = G4Material::GetNumberOfMaterials(); 

  // Read Each Material to get List of Elements
  H_Switch = false;
  C_Switch = false;
  Fe_Switch = false;
  Al_Switch = false;

  for(G4int i=0; i<NumberOfMaterials; i++)
    {
      // Searches to see if Carbon and Hydrogen defined.
      // Can add other elements to this process later if needed.  

      const G4Material* theMaterial = (*theMaterialList)[i];
      G4String MaterialName = theMaterial->GetName();
      const G4ElementVector* theElementList = theMaterial->GetElementVector();
      G4int NumberOfElements = theMaterial->GetNumberOfElements();

      for(G4int j=0;j<NumberOfElements; j++)
	{
	  if(j == 0)
	    {G4cout << "Reading Elements in Material : " << MaterialName << G4endl;}

	  const G4Element* theElement = (*theElementList)[j];
	  G4String theElementName = theElement->GetName();

	  if(theElementName == "Hydrogen" || theElementName == "H")
	    { H_Switch = true; } // Hydrogen defined 
	  else if(theElementName == "Carbon" || theElementName == "C")
	    { C_Switch = true; } // Carbon defined
	  else if(theElementName == "Iron" || theElementName == "Fe")
	    { Fe_Switch = true; } // Iron defined
	  else if(theElementName == "Aluminum" || theElementName == "Al")
	    { Al_Switch = true; } // Al defined
	}
    }

  // Load Cross Sections if Element is Found

	  if(H_Switch == true)
	    { ReadCrossSectionFile("Hydrogen1_el.dat",theHydrogenXS);}
	  
	  if(C_Switch == true)
	    { 
	      // Load all Carbon Elastic and Inelastic Cross Sections
	      ReadCrossSectionFile("Carbon12_el.dat",theCarbonXS); 
	      ReadCrossSectionFile("Carbon12_nng4_4.dat",theC12NGammaXS);
	      ReadCrossSectionFile("Carbon12_na9Be.dat",theC12ABe9XS);
	      ReadCrossSectionFile("Carbon12_np12B.dat",theC12NPB12XS);
//	      ReadCrossSectionFile("Carbon12_nnp11B.dat",theC12NNPB11XS);
				ReadCrossSectionFile("MODIFIED_12Cnnp11B.dat",theC12NNPB11XS);
	      ReadCrossSectionFile("Carbon12_2n11C.dat",theC12N2NC11XS);
	      ReadCrossSectionFile("Carbon12_nn3a.dat",theC12NN3AlphaXS);
	    }

	  if(Fe_Switch == true){
	    ReadCrossSectionFile("Iron56_el.dat",theIronElasXS);
	    ReadCrossSectionFile("Iron56_nonel.dat",theIronNonelasXS);
	  }

	  if(Al_Switch == true){
	    ReadCrossSectionFile("Al27_el.dat",theAlElasXS);
	    ReadCrossSectionFile("Al27_nonel.dat",theAlNonelasXS);
	  }
	    
  G4cout << "Finished Building Cross Section Table! " << G4endl;
}


menate_R::~menate_R()
{;}


G4double menate_R::Absolute(G4double Num)
{
  if(Num < 0.)
    { Num *= -1.;}

  return Num;
}

G4double menate_R::SIGN(G4double A1, G4double B2)
{
  // Does FORTRAN sign command
  // Return "A1" with the sign of B2

  if(B2 >= 0.)
    {A1 = Absolute(A1);}
  else
    {A1 = -1.*Absolute(A1);}
  return A1;
}

void menate_R::ReadCrossSectionFile(G4String FileName, CrossSectionClass* theReactionXS)
{
  if(!getenv("NPTOOL")) 
	{
		G4cerr << "Please set NPTOOL environment variable!" << G4endl;
		exit(1);
	}
  G4String  DirName = G4String(getenv("NPTOOL")) + "/Inputs/CrossSection/MENATE_R";

  FileName = DirName+"/"+FileName;

  G4String ElementName_;
  G4int NumberOfLines = 0;

  std::fstream theFile;
  theFile.open(FileName, std::fstream::in );
  theFile >> NumberOfLines; 

  if(theFile.good())
    {
      theFile >> ElementName_;

      G4cout << "Loading Data For : " << ElementName_ << " , FileName = " << FileName << G4endl;
      for(G4int k=0; k<NumberOfLines; k++)
	{
	  G4double theEnergy;
	  G4double theCrossSection;
	  theFile >> theEnergy >> theCrossSection;
	  
	  if(FileName.contains("Iron") || FileName.contains("Al27") ){
	    theReactionXS[k].SetNumberOfLines(NumberOfLines);
	    theReactionXS[k].SetElementName(ElementName_);
	    theReactionXS[k].SetKinEng(theEnergy*eV);
	    theReactionXS[k].SetTotalCrossSection(theCrossSection*barn);
	    //G4cout << theReactionXS[k].GetKinEng()/MeV << "  " << theReactionXS[k].GetTotalCrossSection()/barn << G4endl;
	  } else{
	    theReactionXS[k].SetNumberOfLines(NumberOfLines);
	    theReactionXS[k].SetElementName(ElementName_);
	    theReactionXS[k].SetKinEng(theEnergy*MeV);
	    theReactionXS[k].SetTotalCrossSection(theCrossSection*barn);
	  }

	  /*//zwk adjusting cross sections
	  if(FileName.contains("nng4")){
	    theReactionXS[k].SetTotalCrossSection(50*theCrossSection*barn);
	    G4cout << "nng4 cross section enhanced :) !!" << G4endl;
	  }
	  else theReactionXS[k].SetTotalCrossSection(theCrossSection*barn); */
	  //theReactionXS[k].DumpData();
	}
      G4cout << "Successfully Loaded !" << G4endl;
      theFile.close();
    }
  else
    {
      G4cerr << "File = " << FileName << " not found or in improper format." << G4endl;
      enum G4ExceptionSeverity severity;
      severity = JustWarning;
      G4Exception("Program aborted in menate::ReadCrossSectionFile() method!","",severity,"");
    }
} 


G4double menate_R::GetCrossSection(G4double KinEng, CrossSectionClass* theReactionXS)
{
  G4double CrossSection=0.;
  G4int NumberOfLines;

      NumberOfLines = theReactionXS[0].GetNumberOfLines();	
      
      if(KinEng > (theReactionXS[NumberOfLines-1].GetKinEng()))
	{ 
        G4cout << "The Neutron Energy is higher than the Total Cross Section DataFile!" << G4endl;
	G4cout << "Using last cross section available!!!!!!!!" << G4endl;
	G4cout << "  Name: " << theReactionXS[0].GetElementName() << G4endl;
	G4cout << "  Max XS Energy: " << theReactionXS[NumberOfLines-1].GetKinEng() << G4endl;
	//G4cerr << "Setting Cross Section = 0 !!!!!!!!!!!!!!!!!!!!!!!" << G4endl;
	//return 0.;
	//zwk use last cross section rather than nothing
	return theReactionXS[NumberOfLines-1].GetTotalCrossSection();
	}
  
      for(G4int k=0; k<NumberOfLines; k++)
	{
	  if((theReactionXS[k].GetKinEng()) == KinEng)
	  { 
	    CrossSection = theReactionXS[k].GetTotalCrossSection();
	    //G4cout << CrossSection/barn << " barn" << G4endl;
	    return CrossSection;
	  }
	  else if ((theReactionXS[k].GetKinEng()) > KinEng)
	  {
	    //G4cout << "Calculating average Cross Section (no exact match)" << G4endl;
	    //G4cout << KinEng/MeV << G4endl;
	    G4double LowEng = (theReactionXS[k-1].GetKinEng());
	    G4double HighEng = (theReactionXS[k].GetKinEng());
	    G4double LowerXS = (theReactionXS[k-1].GetTotalCrossSection());
	    G4double UpperXS = (theReactionXS[k].GetTotalCrossSection()); 
	    CrossSection = GetXSInterpolation(KinEng,LowEng,HighEng,LowerXS,UpperXS);
	    //G4cout << CrossSection/barn << " barn" << G4endl;
	    return CrossSection;
	  }
	}
	 
  return CrossSection;
}

G4double menate_R::GetXSInterpolation(G4double KinEng, G4double LowEng, G4double HighEng, 
                                         G4double LowXS, G4double HighXS)
{
  G4double slope = (HighXS-LowXS)/(HighEng-LowEng);
  G4double y_inter = HighXS-(slope*HighEng);

  G4double Interpol_XS = y_inter + (slope*KinEng);

  return Interpol_XS;
}

G4ThreeVector menate_R::GenMomDir(G4ThreeVector MomDirIn, G4double theta, G4double phi)
{
  // Generates final momentum direction in frame of initial particle
  // follows routine of "MEN_COMPUTE_DIRECTION"
  // Accounts for initial direction of neutron not parallel to z-axis

  // Variables used in calculation

  G4double SinTh = sin(theta);
  G4double CosTh = cos(theta);
  G4double SinPh = sin(phi);
  G4double CosPh = cos(phi);
  G4double CONS = sqrt( Absolute((1. - pow(MomDirIn[2],2))) );

  G4ThreeVector MomDirOut;
  MomDirOut[0] = SinTh*CosPh;
  MomDirOut[1] = SinTh*SinPh;
  MomDirOut[2] = CosTh;

  if(CONS > 1e-8)
    {
      // If CONS > 1e-8, not on z-axis, so need to change frames as below:
      MomDirOut[0] = (SinTh*CosPh*MomDirIn[0]*MomDirIn[2]/CONS) +
	             (SinTh*SinPh*MomDirIn[1]/CONS) +
	             (CosTh*MomDirIn[0]);
      MomDirOut[1] = (SinTh*CosPh*MomDirIn[1]*MomDirIn[2]/CONS) -
	             (SinTh*SinPh*MomDirIn[0]/CONS) +
	             (CosTh*MomDirIn[1]);
      MomDirOut[2] = -(SinTh*CosPh*CONS) + CosTh*MomDirIn[2];
    }

  G4double Norm = sqrt(pow(MomDirOut[0],2)+pow(MomDirOut[1],2)+pow(MomDirOut[2],2));
  // G4double DiffNorm = Absolute(1.-Norm);

  if(Norm != 1.)
    {
      //zwk commented out
      //only occured once when running millions of events
      // if(DiffNorm >= 1e-6)
      // {G4Exception(" Momentum Direction not a unit vector in menate_R::GenMomDir(): Aborted!");}

      // Fixes slight norm diff due to rounding to avoid G4 error

      MomDirOut[0] /= Norm;
      MomDirOut[1] /= Norm;
      MomDirOut[2] /= Norm;

    }

  return MomDirOut;
}

G4double menate_R::ShareGammaEngC12(G4double Available_Eng)
{
  // 24 Apr. 2008 - This function translated from original FORTRAN "MENATE_R"
  // Bleeds excitation energy of C12* into 400 keV gamma rays unless there
  // is enough energy to create the 4.439 MeV (2+->g.s.) gamma. 
  // Addition of this function effects neutron det. efficiency in region
  // between 5-11 MeV (lowers eff. by a few percent).

  // 19 May 2008 - Removed counter for 400 keV gamma rays - BTR

  Num_gamma_4439k = 0;

  G4double Remaining_Eng = Available_Eng;

  while(Remaining_Eng > 4.439*MeV)
    {
      Num_gamma_4439k++;
      Remaining_Eng -= 4.439*MeV;
    }

 G4double Tot_Gamma_Eng = static_cast<G4double>(Num_gamma_4439k)*4.439*MeV;

 return Tot_Gamma_Eng;
}


G4double menate_R::Evaporate_Eng(G4double AMass,G4double Available_Eng)
{
  // Calculate an energy included between 0 and Available_Eng following a probability
  // density dp/de = e/n*exp(-e/t) --- copied from MENATE_R fortran code
  
  G4double Evap_Eng = 0.;

  G4double Temperature = sqrt(8.*Available_Eng/AMass);

  if(Temperature == 0.)
	{ 
		Evap_Eng = 0.;
		return Evap_Eng; 
	}

  G4double ALEA = G4UniformRand();
  G4double XEV = Available_Eng/Temperature;

  if(XEV < 1e-4)
	{ Evap_Eng = 0.; }
  else
	{
		G4double ZNORM = 1. - (1.-(1.+XEV)*exp(-XEV))*ALEA;
		if(ZNORM == 0.)
		{
			Evap_Eng = 0.;
			return Evap_Eng;
		} 
		G4double YEV = XEV*ALEA;
		G4double XEV1;

		do{
			XEV1 = YEV;
			YEV = log((1.+XEV1)/ZNORM);
			if( XEV1 == 0.)
	    {
	      Evap_Eng = 0.;
	      return Evap_Eng;
	    }
		}
		while(Absolute((YEV/XEV1)-1.) > 1e-6);

		Evap_Eng = YEV*Temperature;
	}

  if(Evap_Eng >= Available_Eng)
	{ Evap_Eng = Available_Eng; }

  return Evap_Eng;
}


void menate_R::SetMeanFreePathCalcMethod(G4String Method)
{
  CalcMeth = Method;
  G4cout << "The MeanFreePath and Reaction Calculations method is set to : " << CalcMeth << G4endl;
}


G4String menate_R::ChooseReaction()
{
  // Chooses Reaction that is used in PostStepDoIt 
  G4String theReaction = "NoReaction";

  if(H_Switch == true && C_Switch == false && Fe_Switch==false && Al_Switch==false)
    { theReaction = "N_P_elastic"; }
  else if(C_Switch == true || Fe_Switch == true || Al_Switch==true)
    {
     // Get number of probabilities considered
     // Only include if Cross Section > 0 !
     G4double ProbSigma[12];
     G4double ProbLimit[12];
     if(H_Switch == false){
       ProbSigma[0] = 0.;
       ProbLimit[0] = 0.;
     }
     if(C_Switch == false){
       for(int p=1; p<8; p++){ProbSigma[p]=0.; ProbLimit[0]=0.;}
     }
     if(Fe_Switch == false){
       ProbSigma[8]=0; ProbLimit[8]=0;
       ProbSigma[9]=0; ProbLimit[9]=0;
     }
     if(Al_Switch == false){
       ProbSigma[10]=0; ProbLimit[10]=0;
       ProbSigma[11]=0; ProbLimit[11]=0;
     }

     for(G4int i=0; i<12; i++)
       {
	 ProbSigma[i] = ProbDistPerReaction[i]/ProbTot;
	 
	 if( i == 0 )
	   { ProbLimit[i] = ProbSigma[i]; }
	 else
	   { ProbLimit[i] = ProbLimit[i-1] + ProbSigma[i]; }
	 //G4cout << "ProbSigma " << i << " is : " << ProbSigma[i] << " ProbLimit = " << ProbLimit[i] << G4endl;
       }

    // Set Reaction Type
    // H(n,np) - N_P_elastic
    // 12C(n,n) - N_C12_elastic
    // 12C(n,n'gamma)12C - N_C12_NGamma
    // 12C(n,alpha)9Be - N_C12_A_Be9
    // 12C(n,p)12B     - N_C12_P_B12
    // 12C(n,np)11B    - N_C12_NNP_B11
    // 12C(n,2n)11C    - N_C12_N2N_C11
    // 12C(n,n')3alpha - N_C12_NN3Alpha
     //Fe (n,n) - N_Fe_elastic
     //Fe (n,x) - N_Fe_nonelastic
     //Al (n,n) - N_Al_elastic
     //Al (n,x) - N_Al_nonelastic

     while(theReaction == "NoReaction")
       {
	 // Loop through reactions until one is chosen.
	 // If Prob = 0. for a Reaction, disregarded
	 G4double RecRandnum = G4UniformRand();
	 if(RecRandnum < ProbLimit[0] && ProbSigma[0] > 0.)
	   { theReaction = "N_P_elastic"; }
	 if(RecRandnum >= ProbLimit[0] && RecRandnum < ProbLimit[1] && ProbSigma[1] > 0.)
	   { theReaction = "N_C12_elastic"; }
	 if(RecRandnum >= ProbLimit[1] && RecRandnum < ProbLimit[2] && ProbSigma[2] > 0.)
	   { theReaction = "N_C12_NGamma"; }
	 if(RecRandnum >= ProbLimit[2] && RecRandnum < ProbLimit[3] && ProbSigma[3] > 0.)
	   { theReaction = "N_C12_A_Be9"; } 
	 if(RecRandnum >= ProbLimit[3] && RecRandnum < ProbLimit[4] && ProbSigma[4] > 0.)
	   { theReaction = "N_C12_P_B12"; }
	 if(RecRandnum >= ProbLimit[4] && RecRandnum < ProbLimit[5] && ProbSigma[5] > 0.)
	   { theReaction = "N_C12_NNP_B11"; } 
	 if(RecRandnum >= ProbLimit[5] && RecRandnum < ProbLimit[6] && ProbSigma[6] > 0.)
	   { theReaction = "N_C12_N2N_C11"; } 
	 if(RecRandnum >= ProbLimit[6] && RecRandnum < ProbLimit[7] && ProbSigma[7] > 0.)
	   { theReaction = "N_C12_NN3Alpha"; }
	 if(RecRandnum >= ProbLimit[7] && RecRandnum < ProbLimit[8] && ProbSigma[8] > 0.)
	   { theReaction = "N_Fe_elastic"; }
	 if(RecRandnum >= ProbLimit[8] && RecRandnum < ProbLimit[9] && ProbSigma[9] > 0.)
	   { theReaction = "N_Fe_nonelastic"; }
	 if(RecRandnum >= ProbLimit[9] && RecRandnum < ProbLimit[10] && ProbSigma[10] > 0.)
	   { theReaction = "N_Al_elastic"; }
	 if(RecRandnum >= ProbLimit[10] && RecRandnum < ProbLimit[11] && ProbSigma[11] > 0.)
	   { theReaction = "N_Al_nonelastic"; }
       }

 
     //     G4cout << "***************Reaction Chosen was : " << theReaction << G4endl;
   }
 return theReaction;
}

//-----------------------------------------------------------
// Angular Distribution generators - Added 6 May 2008 BTR
//-----------------------------------------------------------
G4double menate_R::NP_AngDist(G4double NEng)
{
  //NP scattering from DEMONS by Reese, Yariv and Sailor et al.
  //originally written by Stanton
  //based on equation - a + 3b cos(theta)
  // adjusted "30." in original routine with "29.", including
  // minimum energy for "dip" to 29 MeV - BTR 7 May 2008.

  G4double CosCM = 0.;

  if(NEng < 29.*MeV)
    {
      CosCM = 2.*G4UniformRand()-1.;
      return CosCM;
    }

 //      a = 3.0/(3.0+rMax)
 //      b = rMax*a/3.0
 //      RAT = a/(a+b)

  G4double rMax = NEng/29.-1.;
  G4double RAT = 1./(1.+(rMax/3.));
  G4double ran1, ran2, absRan2;

  ran1 = G4UniformRand();
  ran2 = 2.*G4UniformRand()-1.;
  absRan2 = Absolute(ran2);

   if(absRan2 < RAT)
    {CosCM = 2.0*ran1-1.0;}
  else
    {CosCM = SIGN(pow(ran1,(1./3.)),ran2);}

 return CosCM;
}

G4double menate_R::NC12_DIFF(G4double NEng)
{
  // Generates Angular Distribution for n+12C scattering
  // in center of mass system. Kinematics in PostStepDoIt
  // convert result to lab angle. 

  // Uses parameterization in DEMONS by Reese, Yariv and Sailor et al.

  // n+12C cross section is considered isotropic for NEng < 7.35 MeV.
  // The orignal "FitParam" was tweaked to 0.5 to provide a better fit
  // to the C12 diffractive peak at forward angles between 7.3 and 70 MeV.
  // For NEng>70MeV, FitParam is returned to original value 
  // (1.17 fits 96 MeV) with a linear increase (shown below).
  // Fits ang dist data up to 150 MeV.
  // Assume a "global" split between diffractive and non-diffractive
  // cross sections at 0.9.

  // References for n+12C angular distributions (elastic) include :
  // Z.P Chen et al. - J. Phys. G: Nucl. Part. Phys. 31, 1249 (2005).
  // T. Kaneko et al. - Phys. Rev. C 46, 298 (1992).   
  // Z.M. Chen et al. - J. Phys. G: Nucl. Part. Phys. 19, 877 (1993).
  // J.S. Petler et al. - Phys. Rev. C 32, 673 (1985).
  // P. Mermod et al. - Phys. Rev. C 74, 054002 (2006).
  // J.H. Osborne et al. - Phys. Rev. C 70, 054613 (2004).

  // Modifications added by Brian Roeder, LPC Caen, 19 May 2008.

  G4double CosCM = 0.;
  G4double DiffSigma;
  G4double FitParam = 0.5;
  if(NEng >= 70.*MeV)
    {FitParam = 0.021613*NEng-0.90484;} // Fit to AngDist Data

  if(NEng < 7.35*MeV)
    {
      CosCM = 1.-2.*G4UniformRand();
      return CosCM;
    }
  else
    {
      G4double rand = G4UniformRand();
      if(rand > 0.9)
	{CosCM = 1.-2.*G4UniformRand();}
      else
	{
	  DiffSigma = GetCrossSection(NEng,theCarbonXS)/barn;	  
	  do
	    {
	      G4double rand2 = 0.;
	      rand2 = G4UniformRand();
	      //CosCM = 1.0+log(rand2)/1.17/DiffSigma/NEng;
	      CosCM = 1.0+log(rand2)/FitParam/DiffSigma/NEng;
	    }
	  while(CosCM < -1. || CosCM > 1.);	 	  
	}
    }

  return CosCM;
}


void menate_R::Setup_AlElastic_AngDist(){


  const int num = 146;

  //30MeV-------
  double dist_30[num]={
    -1.000000+0, 1.143092e-2, -9.990482e-1, 1.124223e-2, -9.961947e-1, 1.069678e-2,
    -9.914449e-1, 9.853178e-3, -9.848078e-1, 8.801133e-3, -9.762960e-1, 7.649630e-3,
    -9.659258e-1, 6.513742e-3, -9.537170e-1, 5.500872e-3, -9.396926e-1, 4.698089e-3,
    -9.238795e-1, 4.163449e-3, -9.063078e-1, 3.920835e-3, -8.870108e-1, 3.959841e-3,
    -8.660254e-1, 4.239958e-3, -8.433914e-1, 4.698323e-3, -8.191520e-1, 5.259733e-3,
    -7.933533e-1, 5.847400e-3, -7.660444e-1, 6.392955e-3, -7.372773e-1, 6.844546e-3,
    -7.071068e-1, 7.172467e-3, -6.755902e-1, 7.371493e-3, -6.427876e-1, 7.460832e-3,
    -6.087614e-1, 7.481616e-3, -5.735764e-1, 7.493423e-3, -5.372996e-1, 7.569377e-3,
    -5.000000e-1, 7.791201e-3, -4.617486e-1, 8.243935e-3, -4.226183e-1, 9.009845e-3,
    -3.826834e-1, 1.016157e-2, -3.420201e-1, 1.175351e-2, -3.007058e-1, 1.381233e-2,
    -2.588190e-1, 1.632611e-2, -2.164396e-1, 1.923550e-2, -1.736482e-1, 2.242820e-2,
    -1.305262e-1, 2.574168e-2, -8.715574e-2, 2.897774e-2, -4.361939e-2, 3.193267e-2,
    0.000000+0, 3.444537e-2, 4.361939e-2, 3.646164e-2, 8.715574e-2, 3.811006e-2,
    1.305262e-1, 3.977790e-2, 1.736482e-1, 4.217155e-2, 2.164396e-1, 4.633969e-2,
    2.588190e-1, 5.363686e-2, 3.007058e-1, 6.560642e-2, 3.420201e-1, 8.376855e-2,
    3.826834e-1, 1.093183e-1, 4.226183e-1, 1.427545e-1, 4.617486e-1, 1.834979e-1,
    5.000000e-1, 2.295749e-1, 5.372996e-1, 2.774898e-1, 5.735764e-1, 3.224090e-1,
    6.087614e-1, 3.587789e-1, 6.427876e-1, 3.814766e-1, 6.755902e-1, 3.875034e-1,
    7.071068e-1, 3.781310e-1, 7.372773e-1, 3.612692e-1, 7.660444e-1, 3.536586e-1,
    7.933533e-1, 3.823776e-1, 8.191520e-1, 4.850586e-1, 8.433914e-1, 7.082584e-1,
    8.660254e-1, 1.103586+0, 8.870108e-1, 1.721515+0, 9.063078e-1, 2.603299+0,
    9.238795e-1, 3.771875+0, 9.396926e-1, 5.223208+0, 9.537170e-1, 6.919739+0,
    9.659258e-1, 8.787858+0, 9.762960e-1, 1.072018+1, 9.848078e-1, 1.258416+1,
    9.914449e-1, 1.423525+1, 9.961947e-1, 1.553436+1, 9.990482e-1, 1.636534+1,
    1.000000+0, 1.665131+1 };

  hAl_AngDist_30MeV = new TH1F("hAl_AngDist_30MeV","hAl_AngDist_30MeV",300, -1.1, 1.1);
  for(int i=0; i<num; i++){
    double angle = dist_30[i];
    double prob = dist_30[i+1];
    //G4cout << i << " " << angle << " " << prob << G4endl;
    i++;
    double bin = hAl_AngDist_30MeV->FindBin(angle);
    hAl_AngDist_30MeV->SetBinContent(bin, prob);
  }


  //50MeV-----
  double dist_50[num]={
    -1.000000+0, 6.709124e-4, -9.990482e-1, 6.603906e-4, -9.961947e-1, 6.304766e-4,
    -9.914449e-1, 5.858351e-4, -9.848078e-1, 5.332814e-4, -9.762960e-1, 4.805843e-4,
    -9.659258e-1, 4.350849e-4, -9.537170e-1, 4.024467e-4, -9.396926e-1, 3.857654e-4,
    -9.238795e-1, 3.852123e-4, -9.063078e-1, 3.982495e-4, -8.870108e-1, 4.203689e-4,
    -8.660254e-1, 4.461890e-4, -8.433914e-1, 4.707079e-4, -8.191520e-1, 4.904874e-4,
    -7.933533e-1, 5.045640e-4, -7.660444e-1, 5.149420e-4, -7.372773e-1, 5.265894e-4,
    -7.071068e-1, 5.469609e-4, -6.755902e-1, 5.851284e-4, -6.427876e-1, 6.506459e-4,
    -6.087614e-1, 7.522636e-4, -5.735764e-1, 8.966486e-4, -5.372996e-1, 1.087158e-3,
    -5.000000e-1, 1.322876e-3, -4.617486e-1, 1.598021e-3, -4.226183e-1, 1.902000e-3,
    -3.826834e-1, 2.220344e-3, -3.420201e-1, 2.536745e-3, -3.007058e-1, 2.836444e-3,
    -2.588190e-1, 3.110933e-3, -2.164396e-1, 3.363524e-3, -1.736482e-1, 3.614936e-3,
    -1.305262e-1, 3.907434e-3, -8.715574e-2, 4.305532e-3, -4.361939e-2, 4.891377e-3,
    0.000000+0, 5.753370e-3, 4.361939e-2, 6.967889e-3, 8.715574e-2, 8.576699e-3,
    1.305262e-1, 1.056473e-2, 1.736482e-1, 1.284832e-2, 2.164396e-1, 1.528370e-2,
    2.588190e-1, 1.770778e-2, 3.007058e-1, 2.001875e-2, 3.420201e-1, 2.229245e-2,
    3.826834e-1, 2.491798e-2, 4.226183e-1, 2.871318e-2, 4.617486e-1, 3.496300e-2,
    5.000000e-1, 4.531408e-2, 5.372996e-1, 6.146544e-2, 5.735764e-1, 8.463770e-2,
    6.087614e-1, 1.148850e-1, 6.427876e-1, 1.504327e-1, 6.755902e-1, 1.873634e-1,
    7.071068e-1, 2.201177e-1, 7.372773e-1, 2.432984e-1, 7.660444e-1, 2.551966e-1,
    7.933533e-1, 2.630871e-1, 8.191520e-1, 2.897874e-1, 8.433914e-1, 3.801505e-1,
    8.660254e-1, 6.053457e-1, 8.870108e-1, 1.062217+0, 9.063078e-1, 1.865062+0,
    9.238795e-1, 3.128298+0, 9.396926e-1, 4.940641+0, 9.537170e-1, 7.334422+0,
    9.659258e-1, 1.025697+1, 9.762960e-1, 1.355285+1, 9.848078e-1, 1.696543+1,
    9.914449e-1, 2.016259+1, 9.961947e-1, 2.278521+1, 9.990482e-1, 2.450986+1,
    1.000000+0, 2.511166+1 };

  hAl_AngDist_50MeV = new TH1F("hAl_AngDist_50MeV","hAl_AngDist_50MeV",300, -1.1, 1.1);
  for(int i=0; i<num; i++){
    double angle = dist_50[i];
    double prob = dist_50[i+1];
    //G4cout << i << " " << angle << " " << prob << G4endl;
    i++;
    double bin = hAl_AngDist_50MeV->FindBin(angle);
    hAl_AngDist_50MeV->SetBinContent(bin, prob);
  }


  //70MeV-----
  double dist_70[num]={
    -1.000000+0, 3.282137e-6, -9.990482e-1, 3.320258e-6, -9.961947e-1, 3.435988e-6,
    -9.914449e-1, 3.627566e-6, -9.848078e-1, 3.879130e-6, -9.762960e-1, 4.156161e-6,
    -9.659258e-1, 4.410307e-6, -9.537170e-1, 4.586605e-6, -9.396926e-1, 4.627784e-6,
    -9.238795e-1, 4.480618e-6, -9.063078e-1, 4.114588e-6, -8.870108e-1, 3.553527e-6,
    -8.660254e-1, 2.905423e-6, -8.433914e-1, 2.372443e-6, -8.191520e-1, 2.238375e-6,
    -7.933533e-1, 2.843672e-6, -7.660444e-1, 4.551534e-6, -7.372773e-1, 7.691386e-6,
    -7.071068e-1, 1.246622e-5, -6.755902e-1, 1.883831e-5, -6.427876e-1, 2.643986e-5,
    -6.087614e-1, 3.455596e-5, -5.735764e-1, 4.220604e-5, -5.372996e-1, 4.833323e-5,
    -5.000000e-1, 5.212627e-5, -4.617486e-1, 5.349607e-5, -4.226183e-1, 5.366340e-5,
    -3.826834e-1, 5.568937e-5, -3.420201e-1, 6.468955e-5, -3.007058e-1, 8.747101e-5,
    -2.588190e-1, 1.314276e-4, -2.164396e-1, 2.026794e-4, -1.736482e-1, 3.036667e-4,
    -1.305262e-1, 4.308042e-4, -8.715574e-2, 5.733049e-4, -4.361939e-2, 7.146489e-4,
    0.000000+0, 8.379677e-4, 4.361939e-2, 9.356725e-4, 8.715574e-2, 1.021972e-3,
    1.305262e-1, 1.144510e-3, 1.736482e-1, 1.389505e-3, 2.164396e-1, 1.873184e-3,
    2.588190e-1, 2.714392e-3, 3.007058e-1, 3.989167e-3, 3.420201e-1, 5.679383e-3,
    3.826834e-1, 7.640669e-3, 4.226183e-1, 9.625927e-3, 4.617486e-1, 1.139963e-2,
    5.000000e-1, 1.295828e-2, 5.372996e-1, 1.482605e-2, 5.735764e-1, 1.832085e-2,
    6.087614e-1, 2.561511e-2, 6.427876e-1, 3.937859e-2, 6.755902e-1, 6.185808e-2,
    7.071068e-1, 9.347093e-2, 7.372773e-1, 1.313941e-1, 7.660444e-1, 1.691515e-1,
    7.933533e-1, 1.987067e-1, 8.191520e-1, 2.166963e-1, 8.433914e-1, 2.358658e-1,
    8.660254e-1, 3.010805e-1, 8.870108e-1, 5.064869e-1, 9.063078e-1, 1.006997+0,
    9.238795e-1, 2.014608+0, 9.396926e-1, 3.770019+0, 9.537170e-1, 6.484900+0,
    9.659258e-1, 1.026029+1, 9.762960e-1, 1.500126+1, 9.848078e-1, 2.035945+1,
    9.914449e-1, 2.573888+1, 9.961947e-1, 3.038484+1, 9.990482e-1, 3.354666+1,
    1.000000+0, 3.466892+1 };

  hAl_AngDist_70MeV = new TH1F("hAl_AngDist_70MeV","hAl_AngDist_70MeV",300, -1.1, 1.1);
  for(int i=0; i<num; i++){
    double angle = dist_70[i];
    double prob = dist_70[i+1];
    //G4cout << i << " " << angle << " " << prob << G4endl;
    i++;
    double bin = hAl_AngDist_70MeV->FindBin(angle);
    hAl_AngDist_70MeV->SetBinContent(bin, prob);
  }



}

void menate_R::Setup_FeElastic_AngDist(){

  const int num = 146;

  //30MeV-------
  double dist_30[num]={
    -1.000000e+0, 6.864330e-2, -9.990482e-1, 6.799924e-2, -9.961947e-1, 6.606658e-2,
    -9.914449e-1, 6.285744e-2, -9.848078e-1, 5.841792e-2, -9.762960e-1, 5.286584e-2,
    -9.659258e-1, 4.642469e-2, -9.537170e-1, 3.944496e-2, -9.396926e-1, 3.240171e-2,
    -9.238795e-1, 2.585606e-2, -9.063078e-1, 2.038231e-2, -8.870108e-1, 1.646921e-2,
    -8.660254e-1, 1.441510e-2, -8.433914e-1, 1.424461e-2, -8.191520e-1, 1.567324e-2,
    -7.933533e-1, 1.813968e-2, -7.660444e-1, 2.090887e-2, -7.372773e-1, 2.322993e-2,
    -7.071068e-1, 2.451358e-2, -6.755902e-1, 2.448392e-2, -6.427876e-1, 2.326010e-2,
    -6.087614e-1, 2.133853e-2, -5.735764e-1, 1.947210e-2, -5.372996e-1, 1.847261e-2,
    -5.000000e-1, 1.898707e-2, -4.617486e-1, 2.130970e-2, -4.226183e-1, 2.528576e-2,
    -3.826834e-1, 3.033819e-2, -3.420201e-1, 3.561518e-2, -3.007058e-1, 4.022124e-2,
    -2.588190e-1, 4.346884e-2, -2.164396e-1, 4.508347e-2, -1.736482e-1, 4.530519e-2,
    -1.305262e-1, 4.486399e-2, -8.715574e-2, 4.483712e-2, -4.361939e-2, 4.643046e-2,
    0.000000e+0, 5.073712e-2, 4.361939e-2, 5.852223e-2, 8.715574e-2, 7.006881e-2,
    1.305262e-1, 8.508392e-2, 1.736482e-1, 1.026739e-1, 2.164396e-1, 1.213783e-1,
    2.588190e-1, 1.392980e-1, 3.007058e-1, 1.543629e-1, 3.420201e-1, 1.647997e-1,
    3.826834e-1, 1.698153e-1, 4.226183e-1, 1.704198e-1, 4.617486e-1, 1.701846e-1,
    5.000000e-1, 1.755779e-1, 5.372996e-1, 1.954684e-1, 5.735764e-1, 2.394566e-1,
    6.087614e-1, 3.149690e-1, 6.427876e-1, 4.235233e-1, 6.755902e-1, 5.571453e-1,
    7.071068e-1, 6.964176e-1, 7.372773e-1, 8.118647e-1, 7.660444e-1, 8.699422e-1,
    7.933533e-1, 8.440213e-1, 8.191520e-1, 7.290353e-1, 8.433914e-1, 5.566144e-1,
    8.660254e-1, 4.061517e-1, 8.870108e-1, 4.064349e-1, 9.063078e-1, 7.237915e-1,
    9.238795e-1, 1.535223e+0, 9.396926e-1, 2.989307e+0, 9.537170e-1, 5.162009e+0,
    9.659258e-1, 8.018149e+0, 9.762960e-1, 1.139005e+1, 9.848078e-1, 1.498265e+1,
    9.914449e-1, 1.840853e+1, 9.961947e-1, 2.124884e+1, 9.990482e-1, 2.312784e+1,
    1.000000e+0, 2.378523e+1};

  hFe_AngDist_30MeV = new TH1F("hFe_AngDist_30MeV","hFe_AngDist_30MeV",300, -1.1, 1.1);
  for(int i=0; i<num; i++){
    double angle = dist_30[i];
    double prob = dist_30[i+1];
    //G4cout << i << " " << angle << " " << prob << G4endl;
    i++;
    double bin = hFe_AngDist_30MeV->FindBin(angle);
    hFe_AngDist_30MeV->SetBinContent(bin, prob);
  }


  //50MeV-----
  double dist_50[num]={
    -1.000000e+0, 3.691942e-3, -9.990482e-1, 3.578426e-3, -9.961947e-1, 3.262122e-3,
    -9.914449e-1, 2.808433e-3, -9.848078e-1, 2.304621e-3, -9.762960e-1, 1.835324e-3,
    -9.659258e-1, 1.461301e-3, -9.537170e-1, 1.208336e-3, -9.396926e-1, 1.068867e-3,
    -9.238795e-1, 1.014110e-3, -9.063078e-1, 1.010602e-3, -8.870108e-1, 1.034141e-3,
    -8.660254e-1, 1.076051e-3, -8.433914e-1, 1.140557e-3, -8.191520e-1, 1.236001e-3,
    -7.933533e-1, 1.364980e-3, -7.660444e-1, 1.518547e-3, -7.372773e-1, 1.677278e-3,
    -7.071068e-1, 1.818750e-3, -6.755902e-1, 1.928005e-3, -6.427876e-1, 2.006146e-3,
    -6.087614e-1, 2.073045e-3, -5.735764e-1, 2.162751e-3, -5.372996e-1, 2.313195e-3,
    -5.000000e-1, 2.554042e-3, -4.617486e-1, 2.896975e-3, -4.226183e-1, 3.331733e-3,
    -3.826834e-1, 3.829010e-3, -3.420201e-1, 4.349491e-3, -3.007058e-1, 4.856640e-3,
    -2.588190e-1, 5.330337e-3, -2.164396e-1, 5.777631e-3, -1.736482e-1, 6.238028e-3,
    -1.305262e-1, 6.779185e-3, -8.715574e-2, 7.482344e-3, -4.361939e-2, 8.417194e-3,
    0.000000e+0, 9.613124e-3, 4.361939e-2, 1.103728e-2, 8.715574e-2, 1.259760e-2,
    1.305262e-1, 1.418379e-2, 1.736482e-1, 1.574889e-2, 2.164396e-1, 1.740999e-2,
    2.588190e-1, 1.951995e-2, 3.007058e-1, 2.264527e-2, 3.420201e-1, 2.739634e-2,
    3.826834e-1, 3.411133e-2, 4.226183e-1, 4.249017e-2, 4.617486e-1, 5.138349e-2,
    5.000000e-1, 5.900207e-2, 5.372996e-1, 6.375657e-2, 5.735764e-1, 6.569038e-2,
    6.087614e-1, 6.807300e-2, 6.427876e-1, 7.826655e-2, 6.755902e-1, 1.067589e-1,
    7.071068e-1, 1.635344e-1, 7.372773e-1, 2.520196e-1, 7.660444e-1, 3.625788e-1,
    7.933533e-1, 4.694480e-1, 8.191520e-1, 5.360762e-1, 8.433914e-1, 5.328226e-1,
    8.660254e-1, 4.670148e-1, 8.870108e-1, 4.188167e-1, 9.063078e-1, 5.690737e-1,
    9.238795e-1, 1.200984e+0, 9.396926e-1, 2.659844e+0, 9.537170e-1, 5.266500e+0,
    9.659258e-1, 9.198724e+0, 9.762960e-1, 1.437487e+1, 9.848078e-1, 2.038495e+1,
    9.914449e-1, 2.650916e+1, 9.961947e-1, 3.183898e+1, 9.990482e-1, 3.547902e+1,
    1.000000e+0, 3.677275e+1};

  hFe_AngDist_50MeV = new TH1F("hFe_AngDist_50MeV","hFe_AngDist_50MeV",300, -1.1, 1.1);
  for(int i=0; i<num; i++){
    double angle = dist_50[i];
    double prob = dist_50[i+1];
    //G4cout << i << " " << angle << " " << prob << G4endl;
    i++;
    double bin = hFe_AngDist_50MeV->FindBin(angle);
    hFe_AngDist_50MeV->SetBinContent(bin, prob);
  }


  //70MeV-----
  double dist_70[num]={
    -1.000000e+0, 1.983425e-6, -9.990482e-1, 1.965798e-6, -9.961947e-1, 1.905855e-6,
    -9.914449e-1, 1.789217e-6, -9.848078e-1, 1.608414e-6, -9.762960e-1, 1.374194e-6,
    -9.659258e-1, 1.121800e-6, -9.537170e-1, 9.156061e-7, -9.396926e-1, 8.487982e-7,
    -9.238795e-1, 1.028369e-6, -9.063078e-1, 1.543452e-6, -8.870108e-1, 2.426820e-6,
    -8.660254e-1, 3.620830e-6, -8.433914e-1, 4.955293e-6, -8.191520e-1, 6.152640e-6,
    -7.933533e-1, 6.884876e-6, -7.660444e-1, 6.894530e-6, -7.372773e-1, 6.152684e-6,
    -7.071068e-1, 5.003299e-6, -6.755902e-1, 4.240556e-6, -6.427876e-1, 5.065765e-6,
    -6.087614e-1, 8.855034e-6, -5.735764e-1, 1.670012e-5, -5.372996e-1, 2.880136e-5,
    -5.000000e-1, 4.395150e-5, -4.617486e-1, 5.943709e-5, -4.226183e-1, 7.165837e-5,
    -3.826834e-1, 7.762420e-5, -3.420201e-1, 7.715392e-5, -3.007058e-1, 7.507574e-5,
    -2.588190e-1, 8.212670e-5, -2.164396e-1, 1.131184e-4, -1.736482e-1, 1.816766e-4,
    -1.305262e-1, 2.925482e-4, -8.715574e-2, 4.346801e-4, -4.361939e-2, 5.800715e-4,
    0.000000e+0, 6.933060e-4, 4.361939e-2, 7.530309e-4, 8.715574e-2, 7.790938e-4,
    1.305262e-1, 8.502975e-4, 1.736482e-1, 1.093673e-3, 2.164396e-1, 1.633476e-3,
    2.588190e-1, 2.510481e-3, 3.007058e-1, 3.613760e-3, 3.420201e-1, 4.690171e-3,
    3.826834e-1, 5.484783e-3, 4.226183e-1, 5.996965e-3, 4.617486e-1, 6.725686e-3,
    5.000000e-1, 8.681963e-3, 5.372996e-1, 1.297100e-2, 5.735764e-1, 1.997476e-2,
    6.087614e-1, 2.857775e-2, 6.427876e-1, 3.626075e-2, 6.755902e-1, 4.084107e-2,
    7.071068e-1, 4.379671e-2, 7.372773e-1, 5.349841e-2, 7.660444e-1, 8.508892e-2,
    7.933533e-1, 1.537562e-1, 8.191520e-1, 2.613914e-1, 8.433914e-1, 3.833791e-1,
    8.660254e-1, 4.699469e-1, 8.870108e-1, 4.791521e-1, 9.063078e-1, 4.493164e-1,
    9.238795e-1, 5.943887e-1, 9.396926e-1, 1.373549e+0, 9.537170e-1, 3.465226e+0,
    9.659258e-1, 7.589794e+0, 9.762960e-1, 1.418895e+1, 9.848078e-1, 2.306574e+1,
    9.914449e-1, 3.316772e+1, 9.961947e-1, 4.268799e+1, 9.990482e-1, 4.953558e+1,
    1.000000e+0, 5.203243e+1 };

  hFe_AngDist_70MeV = new TH1F("hFe_AngDist_70MeV","hFe_AngDist_70MeV",300, -1.1, 1.1);
  for(int i=0; i<num; i++){
    double angle = dist_70[i];
    double prob = dist_70[i+1];
    //G4cout << i << " " << angle << " " << prob << G4endl;
    i++;
    double bin = hFe_AngDist_70MeV->FindBin(angle);
    hFe_AngDist_70MeV->SetBinContent(bin, prob);
  }


  //+++ ---
}


G4double menate_R:: AlElastic_AngDist(G4double NEng){
  double ang_CM = 0;

  if(NEng<=15*MeV) ang_CM = G4UniformRand();
  else if(NEng<=40*MeV ) ang_CM = hAl_AngDist_30MeV->GetRandom();
  else if(NEng<=60*MeV ) ang_CM = hAl_AngDist_50MeV->GetRandom();
  else if(NEng>60*MeV ) ang_CM = hAl_AngDist_70MeV->GetRandom();

  return ang_CM;
}

G4double menate_R:: FeElastic_AngDist(G4double NEng){
  double ang_CM = 0;

  if(NEng<=15*MeV) ang_CM = G4UniformRand();
  else if(NEng<=40*MeV ) ang_CM = hFe_AngDist_30MeV->GetRandom();
  else if(NEng<=60*MeV ) ang_CM = hFe_AngDist_50MeV->GetRandom();
  else if(NEng>60*MeV ) ang_CM = hFe_AngDist_70MeV->GetRandom();

  return ang_CM;
}
//e-e-e-e-e------------------------------------------------------------------
//----------------------------------------------------------------------
// The Below are the GEANT4:: G4VDiscreteProcess class methods that must
// be overridden to run this process in G4.
//----------------------------------------------------------------------
//----------------------------------------------------------------------

 
G4bool menate_R::IsApplicable(const G4ParticleDefinition& particle)
{ 
// returns "true" if this model can be applied to the particle-type
  //G4cout << "IsApplicable loaded!" << G4endl;

  if (particle == *(       G4Neutron::Neutron()       )) 
    {
      //G4cout << "Found the Neutron ! " << G4endl;
      return true;
    }
  else
    {   enum G4ExceptionSeverity severity;
        severity = JustWarning;
        G4Exception("Model \"menate_R\" can only be used for neutron scattering!","",severity,"");}
  return false;
}


G4double menate_R::GetMeanFreePath(const G4Track& aTrack, G4double, // previousStepSize, 
                                      G4ForceCondition* condition) 
{
  //G4cout << "menate_R GetMeanFreePath called!" << G4endl;
  
  const G4Step* theStep = aTrack.GetStep();
  G4StepPoint* thePoint = theStep->GetPreStepPoint();
  G4String theVolume =  thePoint->GetPhysicalVolume()->GetName();
 
  /*
  if(theVolume == "target")  
    {*condition = Forced;}
  else
    {*condition = NotForced;}
  */  

  *condition = NotForced;
  
//  G4double previousStepSize = 0.1*mm;

  const G4DynamicParticle* projectile = aTrack.GetDynamicParticle();
  G4double theKinEng = projectile->GetKineticEnergy();

  if(theKinEng < 1e-4*MeV)
  {return DBL_MAX;} // Energy too low for this model!

  // Now read the medium to get mean free path at this energy!

 const G4Material* theMaterial = aTrack.GetMaterial();

 // GetVecNbOfAtomsPerVolume() returns (NumberOfAtoms/mm3)! 
 const G4double* N_Per_Volume = theMaterial->GetVecNbOfAtomsPerVolume();

 // Get List of Elements in Material, also get number!
 const G4ElementVector* theElementList = theMaterial->GetElementVector();
 G4int NumberOfElements = theMaterial->GetNumberOfElements();

 // Only do MeanFreePath calculation if in a material
 // that has Hydrogen or Carbon !
 H_Switch = false;
 C_Switch = false;
 Fe_Switch = false;
 Al_Switch = false;

 for(G4int ne=0;ne<NumberOfElements;ne++)
   {
     G4Element* theElement = (*theElementList)[ne];

     //G4cout << "The Element is : " << theElement->GetName() << G4endl;
     Z = static_cast<int>(theElement->GetZ());
     A = static_cast<int>(theElement->GetN());  // returns Atomic Weight
     //G4cout << Z << " " << A << G4endl;

     //G4cout << theMaterial->GetName() << G4endl;
     if(Z == 1 && A == 1)
       { 
	 H_Switch = true; 
	 Num_H = N_Per_Volume[ne];
       }
     else if(Z == 6 && A == 12)
       { 
	 C_Switch = true; 
	 Num_C = N_Per_Volume[ne];
       }
     else if(Z == 26){
       Fe_Switch = true;
       //G4cout << "Inside IRON !!" << G4endl;
       Num_Fe = N_Per_Volume[ne];
     }
     else if(Z == 13){
       Al_Switch = true;
       //G4cout << "Inside IRON !!" << G4endl;
       Num_Al = N_Per_Volume[ne];
     }
   }

 if(H_Switch == false && C_Switch == false && Fe_Switch == false && Al_Switch == false)
   { return DBL_MAX; }

 // Calculate Total Mean Free Path (From Total Cross Section and Density)
 // Does not decide reaction --- PostStepDoIt decides reaction if n-scattering occurs

 if(CalcMeth == "ORIGINAL")
   {
     G4int k_tot = 0;

     if(H_Switch == true)
       {
	 // Returns Cross Section in Barns, pre-loaded in constructor
	 Sigma = GetCrossSection(theKinEng,theHydrogenXS);
	 //G4cout << "Sigma =  " << Sigma/barn << G4endl; 
	 ProbDistPerReaction[0] = Sigma*Num_H;
	 k_tot = 1;
       }
     else
       {
	 // Clears array element
	 ProbDistPerReaction[0] = 0.;
       }

     if(C_Switch == true)
       {
	 Sigma = GetCrossSection(theKinEng,theCarbonXS);
	 ProbDistPerReaction[1] = Sigma*Num_C;	
	 Sigma = GetCrossSection(theKinEng,theC12NGammaXS);
	 ProbDistPerReaction[2] = Sigma*Num_C;
	 Sigma = GetCrossSection(theKinEng,theC12ABe9XS);
	 ProbDistPerReaction[3] = Sigma*Num_C;
	 Sigma = GetCrossSection(theKinEng,theC12NPB12XS);
	 ProbDistPerReaction[4] = Sigma*Num_C;
	 Sigma = GetCrossSection(theKinEng,theC12NNPB11XS);
	 ProbDistPerReaction[5] = Sigma*Num_C;
	 Sigma = GetCrossSection(theKinEng,theC12N2NC11XS);
	 ProbDistPerReaction[6] = Sigma*Num_C;
	 Sigma = GetCrossSection(theKinEng,theC12NN3AlphaXS);
	 ProbDistPerReaction[7] = Sigma*Num_C;

	 k_tot = 8;
       }
     else
       {
	 // Clears array element
	 for(G4int i=1;i<8;i++)
	   { ProbDistPerReaction[i] = 0.; }
       }

     if(Fe_Switch == true)
       {
	 Sigma = GetCrossSection(theKinEng,theIronElasXS);
	 ProbDistPerReaction[8] = Sigma*Num_Fe;	
	 Sigma = GetCrossSection(theKinEng,theIronNonelasXS);
	 ProbDistPerReaction[9] = Sigma*Num_Fe;       

	 k_tot = 10;
       }
     else
       {
	 // Clears array element
	 for(G4int i=8;i<10;i++)
	   { ProbDistPerReaction[i] = 0.; }
       }

     if(Al_Switch == true)
       {
	 Sigma = GetCrossSection(theKinEng,theAlElasXS);
	 ProbDistPerReaction[10] = Sigma*Num_Al;	
	 Sigma = GetCrossSection(theKinEng,theAlNonelasXS);
	 ProbDistPerReaction[11] = Sigma*Num_Al;       

	 k_tot = 12;
       }
     else
       {
	 // Clears array element
	 for(G4int i=10;i<12;i++)
	   { ProbDistPerReaction[i] = 0.; }
       }


     ProbTot = 0.;  // Reset Probtot

     for(G4int k=0; k<k_tot; k++)
       {ProbTot += ProbDistPerReaction[k];}
    
     if(ProbTot > 1.e-6)
       {
	 G4double total_mean_free_path = 1/(ProbTot);
	 //G4cout << "The mean free path was : " << total_mean_free_path/cm << " cm" << G4endl; 
	 return total_mean_free_path;
       }
   }
 else if(CalcMeth == "MENATE")
   {
     // MENATE MeanFreePath method is as follows:
     // Adds up total cross section for hydrogen and carbon and then uses molecular weight,
     // density of NE213 to calculate MeanFreePath

     // 30 April 2008 - BTR - After comparison with program DECOI, determined the difference
     // between the ORIGINAL mean free path calculation above and the MENATE calculation
     // is that the MENATE calculation lacks a factor 1.213 that is multiplied to the n-p
     // total cross section. The 1.213 factor comes from the ratio of Carbon to Hydrogen in 
     // NE213. This disagreement is "fixed" in the original MENATE FORTRAN code by mulitiplying
     // the Hydrogren cross sections in the data file by a factor of 1.2. 
     // I have left here the original method in the FORTRAN MENATE code for mean free path
     // calculation for comparison with the ORIGINAL (standard) method. Note that the methods
     // do not agree unless the n-p total cross sections in the data file are muliplied by 1.2
     // as in the FORTRAN version data files. 

     G4String MaterialName = theMaterial->GetName();
     if(MaterialName == "NE213")
       { AMass_Material = 13.213; }
     else
       { AMass_Material = theMaterial->GetA(); }

     // Get Cross Section Sum at this energy
     ProbDistPerReaction[0] = (GetCrossSection(theKinEng,theHydrogenXS))/barn;
     ProbDistPerReaction[1] = (GetCrossSection(theKinEng,theCarbonXS))/barn;	
     ProbDistPerReaction[2] = (GetCrossSection(theKinEng,theC12NGammaXS))/barn;
     ProbDistPerReaction[3] = (GetCrossSection(theKinEng,theC12ABe9XS))/barn;
     ProbDistPerReaction[4] = (GetCrossSection(theKinEng,theC12NPB12XS))/barn;
     ProbDistPerReaction[5] = (GetCrossSection(theKinEng,theC12NNPB11XS))/barn;
     ProbDistPerReaction[6] = (GetCrossSection(theKinEng,theC12N2NC11XS))/barn;
     ProbDistPerReaction[7] = (GetCrossSection(theKinEng,theC12NN3AlphaXS))/barn;

     // Add up Cross Sections
     ProbTot = 0.;  // Reset Probtot

     for(G4int k=0; k<8; k++)
       {ProbTot += ProbDistPerReaction[k];}

     G4double NE213dens = 0.874;
     G4double NumAvagadro = 0.60221367;   // Num Avagadro*conversion to barns
     G4double Constant = 10.*AMass_Material/NumAvagadro/NE213dens;    
     G4double total_mean_free_path = Constant/ProbTot;
     //G4cout << "The mean free path was : " << total_mean_free_path/cm << " cm" << G4endl; 
	
     if(total_mean_free_path < DBL_MAX)
       {return total_mean_free_path;}
      
   }
 else if(CalcMeth == "carbon_el_only" && C_Switch == true)
   {
     // Added 7 May 2007 - For carbon elastic scattering only
      // Clears array elements
	 for(G4int i=0;i<8;i++)
	   { ProbDistPerReaction[i] = 0.; }
	 Sigma = GetCrossSection(theKinEng,theCarbonXS);
	 ProbDistPerReaction[1] = Sigma*Num_C;
	 ProbTot = ProbDistPerReaction[1];
	 if(ProbTot > 1.e-6)
	   {
	     G4double total_mean_free_path = 1/(ProbTot);
	     //G4cout << "The mean free path was : " << total_mean_free_path/cm << " cm" << G4endl; 
	     return total_mean_free_path;
	   }
   }

 return DBL_MAX;
}


G4VParticleChange* menate_R::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
  // Now we tell GEANT what to do if MeanFreePath condition is satisfied!
  // Overrides PostStepDoIt function in G4VDiscreteProcess

  const G4DynamicParticle* projectile = aTrack.GetDynamicParticle();
  //const G4ParticleDefinition* particle = projectile->GetDefinition();

  const G4Material* theMaterial = aTrack.GetMaterial();
  G4String MaterialName = theMaterial->GetName();

  G4double KinEng_Int = projectile->GetKineticEnergy();
  G4ThreeVector MomDir_Int = projectile->GetMomentumDirection();
  G4double GlobalTime = aTrack.GetGlobalTime(); 
  G4ThreeVector thePosition = aTrack.GetPosition();
  G4TouchableHandle theTouchable = aTrack.GetTouchableHandle();


  //zwk Found that Genat4 gets hung up randomly with a position of (nan,nan,nan)
  //Therefore if this happens reset position to previous set
  if(std::isnan(thePosition.x())||std::isnan(thePosition.y())||std::isnan(thePosition.z())){ 
    //G4cout << "MENATE_R: Current position is (nan,nan,nan): resetting to presteppoint" << G4endl;
    G4cout << aTrack.GetMaterial()->GetName() << G4endl;
    G4cout << aTrack.GetNextMaterial()->GetName() << G4endl;
    G4cout << aStep.GetPreStepPoint()->GetPosition().z()/cm << G4endl;
    G4cout << aStep.GetPostStepPoint()->GetPosition().z()/cm << G4endl;
    G4cout << aStep.GetPreStepPoint()->GetMaterial()->GetName() << G4endl;
    G4cout << aStep.GetPostStepPoint()->GetMaterial()->GetName() << G4endl;

    thePosition = aStep.GetPreStepPoint()->GetPosition();

  }

  /*
  // These variables not used...
  G4double LocalTime = aTrack.GetLocalTime();
  G4double ProperTime = aTrack.GetProperTime();
  G4double theProjMass = projectile->GetMass();
  */

  //zwk changed from 10-5 MeV
 if(KinEng_Int < 0.5e-3*MeV)
   {
     //G4cout << "Neutron Energy = " << KinEng_Int << G4endl;
     //G4cout << "Do Nothing, Kill Event! " << G4endl;
    aParticleChange.ProposeTrackStatus(fStopAndKill);
    aParticleChange.ProposeEnergy(0.);
    aParticleChange.ProposePosition(thePosition);
    return pParticleChange;
   } 

 if(MaterialName == "NE213")
   { AMass_Material = 13.213; }

  // Define Reaction -
  // Chooses Reaction based on ProbDistPerReaction Vector defined in GetMeanFreePath()

 G4String ReactionName = ChooseReaction();

  //
  //*******MENATE Reaction Kinematics*************
  //
    
      //zwk modification  Bool_t fwrdANGLES = true means use cos_theta_iso=pow(G4uniformRand,1/4), so becomes very forward focused rather than isotropic
      G4bool fwrdANGLES = false;

if(ReactionName == "N_P_elastic")
  {

    G4double mass_ratio = 939.6/938.3;  // Mass ratio N/P

    G4double cos_theta = 0.;
    G4double theta_cm = 0.;
    G4double theta_N  = 0.;
    G4double phi_N = 0.;
    G4double T_N = 0.;
    G4ThreeVector MomDir_N;

    G4double cos_theta_P = 0.;
    G4double theta_P = 0.;
    G4double phi_P = 0.;
    G4double T_P = 0.;
    G4ThreeVector MomDir_P;

    // *** 6 May 2008 - Added n-p angular distribution as in DEMONS - BTR
    // Made slight adjustment to routine (changed "30" to "29") to improve 
    // agreement with data.
    // Calculate recoil energy as in Knoll          

    cos_theta = NP_AngDist(KinEng_Int); // cosine of cm_angle
    theta_cm = acos(cos_theta);

    // Following Knoll, calculate energy for proton first
    cos_theta_P = sqrt((1.-cos_theta)/2.);
    theta_P = acos(cos_theta_P);
    phi_P = 2.*Pi*G4UniformRand();

    T_P = KinEng_Int*pow(cos_theta_P,2);

    MomDir_P = GenMomDir(MomDir_Int,theta_P,phi_P);

    // scattered neutron ----------------
    // angles changed from cm to lab as in Marian and Young
    theta_N = atan(sin(theta_cm)/(cos_theta+mass_ratio));
    phi_N = phi_P+Pi;

    T_N = KinEng_Int - T_P;    

    MomDir_N = GenMomDir(MomDir_Int,theta_N,phi_N);
  
    // Generate a Secondary Neutron
    G4DynamicParticle* theSecNeutron = new G4DynamicParticle;
    G4ParticleDefinition* theSecNeutronDefinition = G4Neutron::Neutron();
    theSecNeutron->SetDefinition(theSecNeutronDefinition);
    theSecNeutron->SetKineticEnergy(T_N);
    theSecNeutron->SetMomentumDirection(MomDir_N);

    // Generate a Secondary Proton
    G4DynamicParticle* theSecProton = new G4DynamicParticle;
    G4ParticleDefinition* theSecProtonDefinition = G4Proton::Proton();
    theSecProton->SetDefinition(theSecProtonDefinition);
    theSecProton->SetKineticEnergy(T_P);
    theSecProton->SetMomentumDirection(MomDir_P);
   
    // Kill the Parent Neutron -----------------------
    aParticleChange.ProposeTrackStatus(fStopAndKill);
    aParticleChange.ProposeEnergy(0.);
    aParticleChange.ProposePosition(thePosition);
  
 // Set final tracks in motion!
    G4Track* theNTrack = new G4Track(theSecNeutron,GlobalTime,thePosition);
    theNTrack->SetTouchableHandle(theTouchable);
    G4Track* thePTrack = new G4Track(theSecProton,GlobalTime,thePosition);
    thePTrack->SetTouchableHandle(theTouchable);

    aParticleChange.SetNumberOfSecondaries(2);
    aParticleChange.AddSecondary(theNTrack);
    aParticleChange.AddSecondary(thePTrack);
  
    // G4cout << "Made it to the end ! " << G4endl;
  
  }
 else if(ReactionName == "N_C12_elastic")
   {
     G4double theta_N  = 0.;
     G4double phi_N = 0.;

     G4double T_N = 0.;
     G4double T_C12el = 0.;
     G4ThreeVector MomDir_N;
     G4ThreeVector MomDir_C12;

     // For n,12C elastic scattering, follows model of Knoll
     // Original MENATE distribution was isotropic in CM
     // G4double cos_sq_theta = G4UniformRand(); 

     // Calls diffractive angular distribution as in DEMONS
     G4double cos_theta_cm = NC12_DIFF(KinEng_Int);

     // Convert according to Knoll
     G4double cos_theta_lab = sqrt((1.-cos_theta_cm)/2.);

     G4double cos_sq_theta = pow(cos_theta_lab,2);
    
     // .2840237              =4*Mc12*Mn/(Mc12+Mn)^2
     T_C12el = 0.2840237*KinEng_Int*cos_sq_theta;

     T_N = KinEng_Int - T_C12el;

     if(T_N > 1.e-5)
       {
	 G4double arg = (sqrt(KinEng_Int)-sqrt(12.*T_C12el*cos_sq_theta))/sqrt(T_N);
	 if(arg > 1.)
	   {arg = 1.;}
	 theta_N = acos(arg);
         phi_N = 2.*Pi*G4UniformRand();
       }

     MomDir_N = GenMomDir(MomDir_Int,theta_N,phi_N);

     // Carbon angles
     G4double theta_C12 = acos(sqrt(cos_sq_theta));
     G4double phi_C12 = phi_N+Pi;

     MomDir_C12 = GenMomDir(MomDir_Int,theta_C12,phi_C12);
    
     // Generate a Secondary Neutron
     G4DynamicParticle* theSecNeutron = new G4DynamicParticle;
     G4ParticleDefinition* theSecNeutronDefinition = G4Neutron::Neutron();
     theSecNeutron->SetDefinition(theSecNeutronDefinition);
     theSecNeutron->SetKineticEnergy(T_N);
     theSecNeutron->SetMomentumDirection(MomDir_N);

     // Generate a Secondary C12
     G4DynamicParticle* theSecC12 = new G4DynamicParticle;
     // GetIon() Method works whether particle exists in physicslist or not. 
     // Arguements are GetIon(Charge,Mass,ExcitationEng)
     G4ParticleDefinition* theSecC12Definition = G4IonTable::GetIonTable()->GetIon(6,12,0.);
     theSecC12->SetDefinition(theSecC12Definition);
     theSecC12->SetKineticEnergy(T_C12el);
     theSecC12->SetMomentumDirection(MomDir_C12);
   
     // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     // Set final tracks in motion!
     G4Track* theNTrack = new G4Track(theSecNeutron,GlobalTime,thePosition);
     theNTrack->SetTouchableHandle(theTouchable);
     G4Track* theC12Track = new G4Track(theSecC12,GlobalTime,thePosition);
     theC12Track->SetTouchableHandle(theTouchable);

     aParticleChange.SetNumberOfSecondaries(2);
     aParticleChange.AddSecondary(theNTrack);
     aParticleChange.AddSecondary(theC12Track);
  
    // G4cout << "Made it to the end ! " << G4endl;
   }
 else if(ReactionName == "N_C12_NGamma")
   {
     // 24 Apr. 2008 - BTR - This version replaces version in previous
     // menate.cc to 12C(n,n'gamma) reaction in orig FORTRAN MENATE

     G4double Available_Eng;

     if(KinEng_Int >= 8.5*MeV)
       { Available_Eng = 8.5*MeV*(G4UniformRand()); } // Max available Eng = 8.5 MeV ?
     else
       { Available_Eng = KinEng_Int*G4UniformRand(); }
     

     //G4double Total_Gamma_Eng = ShareGammaEngC12(Available_Eng);
     //Changed such that one 4.4 MeV gamma is always produced if the
     //nng4 reaction is called.
     //if the energy is >4.43 MeV then 1 gamma is produced.
     G4double enough_gamma_eng=0*MeV;
     if(KinEng_Int >= 4.5*MeV) enough_gamma_eng = 5.0*MeV;
     G4double Total_Gamma_Eng = ShareGammaEngC12(enough_gamma_eng);

     G4ThreeVector MomDir_N;
     G4ThreeVector MomDir_C12;

     // N'

     Available_Eng = 0.9230769*KinEng_Int-Total_Gamma_Eng;
     if(Available_Eng <= 0.)
       {Available_Eng = 0.;} // = 0 in Fortran code - BTR 

     G4double cos_theta_iso = 1.-(2.*G4UniformRand());
     //zwk
     if(fwrdANGLES) cos_theta_iso = pow(G4UniformRand(),1./4.0);

     G4double T_N = KinEng_Int/169.+0.9230769*Available_Eng + 
                    (0.1478106*sqrt(KinEng_Int*Available_Eng))*cos_theta_iso;
     G4double theta_N = acos(cos_theta_iso);
     G4double phi_N = 2.*Pi*G4UniformRand();

     MomDir_N = GenMomDir(MomDir_Int,theta_N,phi_N);
 
     // C12*

     G4double T_C12 = KinEng_Int - T_N - Total_Gamma_Eng;
     if(T_C12 <= 0.)
       {T_C12 = 0.;}
     G4double theta_C12 = Pi - theta_N;
     G4double phi_C12 = Pi + phi_N;

     MomDir_C12 = GenMomDir(MomDir_Int,theta_C12,phi_C12);

     // Generate a Secondary Neutron
     G4DynamicParticle* theSecNeutron = new G4DynamicParticle;
     G4ParticleDefinition* theSecNeutronDefinition = G4Neutron::Neutron();
     theSecNeutron->SetDefinition(theSecNeutronDefinition);
     theSecNeutron->SetKineticEnergy(T_N);
     theSecNeutron->SetMomentumDirection(MomDir_N);

     // Generate a Secondary C12*
     G4DynamicParticle* theSecC12 = new G4DynamicParticle;
     // GetIon() Method works whether particle exists in physicslist or not. 
     // Arguements are GetIon(Charge,Mass,ExcitationEng)
     G4ParticleDefinition* theSecC12Definition = G4IonTable::GetIonTable()->GetIon(6,12,0.);
     theSecC12->SetDefinition(theSecC12Definition);
     theSecC12->SetKineticEnergy(T_C12);
     theSecC12->SetMomentumDirection(MomDir_C12);

     // 4.439 MeV Gamma - emitted isotropically from reaction point..
     // -- 24 Apr 2008 - BTR - With 8.5 MeV limit given at start of 
     //    MENATE function for this reaction, can not produce more
     //    than one 4.439 MeV gamma. 
     // - 19 May 2008 - BTR
     // --- Removed 400 keV gamma ray counter from original MENATE code

     G4int NumberOfSec = 2+Num_gamma_4439k;
     aParticleChange.SetNumberOfSecondaries(NumberOfSec);

     //G4cout << Available_Eng/MeV << G4endl;
     //G4cout << "Num gammas: " << Num_gamma_4439k << G4endl;
	 if(Num_gamma_4439k > 0)
	   {	     
	     G4double T_Gamma = 4.439*MeV;
	     G4double theta_Gamma = acos(1.-(2.*G4UniformRand()));
	     G4double phi_Gamma = 2.*Pi*G4UniformRand();
	     
	     G4ThreeVector MomDir_Gamma;
	     MomDir_Gamma = GenMomDir(MomDir_Int,theta_Gamma,phi_Gamma);

	     // Generate a Secondary Gamma (4.439 MeV)
	     G4DynamicParticle* theSecGamma = new G4DynamicParticle;
	     G4ParticleDefinition* theGammaDefinition = G4Gamma::Gamma();
	     theSecGamma->SetDefinition(theGammaDefinition);
	     theSecGamma->SetKineticEnergy(T_Gamma);
	     theSecGamma->SetMomentumDirection(MomDir_Gamma);

	     aParticleChange.SetNumberOfSecondaries(2+Num_gamma_4439k);

	     G4Track* theGammaTrack = new G4Track(theSecGamma,GlobalTime,thePosition);
	     theGammaTrack->SetTouchableHandle(theTouchable);
	     aParticleChange.AddSecondary(theGammaTrack);
	   }
    
     // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     // Set final tracks in motion!
     G4Track* theNTrack = new G4Track(theSecNeutron,GlobalTime,thePosition);
     theNTrack->SetTouchableHandle(theTouchable);
     G4Track* theC12Track = new G4Track(theSecC12,GlobalTime,thePosition);
     theC12Track->SetTouchableHandle(theTouchable);
     
     aParticleChange.AddSecondary(theNTrack);
     aParticleChange.AddSecondary(theC12Track);
     
    // G4cout << "Made it to the end ! " << G4endl;
 
   }
 else if(ReactionName == "N_C12_A_Be9")
   {
     // Copied from MENATE
     // Reaction can not occur if incoming neutron energy is below 6.176 MeV
     // Q Value = - 5.701 MeV

     // Uses formula for C12 recoil to calculate direction of 9Be

     G4ThreeVector MomDir_Alpha;
     G4ThreeVector MomDir_Be9;

     G4double Q_value = -5.701*MeV;
     // 0.9230769 = Mass_C12/(Mass_N+Mass_C12) - converts to CM frame energy
     G4double Available_Eng = 0.9230769*KinEng_Int+Q_value;

     if(Available_Eng <= 0.)
       { Available_Eng = 0.; }

     // the alpha

     G4double cos_theta_iso = 1.-(2.*G4UniformRand());
     //zwk
     if(fwrdANGLES) cos_theta_iso = pow(G4UniformRand(),1./4.0);

     // 0.0236686 = Malpha*MassN/(MassC12+MassN)^2
     // 0.6923077 = MBe9/(MassN+MassC12)
     // 0.2560155 = 2.*sqrt(Mn*MBe9*Malpha/(MassC12+MassN)^3)

     G4double T_Alpha = 0.0236686*KinEng_Int+0.6923077*Available_Eng+
                        (0.2560155*sqrt(KinEng_Int*Available_Eng))*cos_theta_iso;

     G4double theta_Alpha = 0.;
     G4double phi_Alpha = 0.;

     if(T_Alpha > 1.e-5)
       {
	 // 0.1538462 = sqrt(Malpha*Mn)/(Mn+Mc12)
	 // 0.8320503 = sqrt(MBe9/(MassC12+MassN))

	 G4double Arg = (0.1538462*sqrt(KinEng_Int)+0.8320503*sqrt(Available_Eng)*cos_theta_iso)/(sqrt(T_Alpha));

	 if(Arg > 1.)
	   { Arg = 1.; }

	 theta_Alpha = acos(Arg);
	 phi_Alpha = 2.*Pi*G4UniformRand();
       }
  
     MomDir_Alpha = GenMomDir(MomDir_Int,theta_Alpha,phi_Alpha);
 
     // Be9 - Need to check kinematics - maybe not right - 18 Apr 2008, BTR

     G4double T_Be9 = KinEng_Int + Q_value - T_Alpha;
     G4double theta_Be9 = Pi - theta_Alpha;
     G4double phi_Be9 = Pi + phi_Alpha;

     MomDir_Be9 = GenMomDir(MomDir_Int,theta_Be9,phi_Be9);

     // Generate a Secondary Alpha
     G4DynamicParticle* theSecAlpha = new G4DynamicParticle;
     G4ParticleDefinition* theSecAlphaDefinition = G4Alpha::Alpha();
     theSecAlpha->SetDefinition(theSecAlphaDefinition);
     theSecAlpha->SetKineticEnergy(T_Alpha);
     theSecAlpha->SetMomentumDirection(MomDir_Alpha);

     // Generate a Secondary Be9
     G4DynamicParticle* theSecBe9 = new G4DynamicParticle;
     // GetIon() Method works whether particle exists in physicslist or not. 
     // Arguements are GetIon(Charge,Mass,ExcitationEng)
     G4ParticleDefinition* theSecBe9Definition = G4IonTable::GetIonTable()->GetIon(4,9,0.);
     theSecBe9->SetDefinition(theSecBe9Definition);
     theSecBe9->SetKineticEnergy(T_Be9);
     theSecBe9->SetMomentumDirection(MomDir_Be9);

    // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     // Set final tracks in motion!
     G4Track* theAlphaTrack = new G4Track(theSecAlpha,GlobalTime,thePosition);
     theAlphaTrack->SetTouchableHandle(theTouchable);
     G4Track* theBe9Track = new G4Track(theSecBe9,GlobalTime,thePosition);
     theBe9Track->SetTouchableHandle(theTouchable);

     aParticleChange.SetNumberOfSecondaries(2);
     aParticleChange.AddSecondary(theAlphaTrack);
     aParticleChange.AddSecondary(theBe9Track);
  
     /*
     //---Test Momentum Conservation----
     G4cout << "\n\n------------------"<< G4endl;

     //incoming neutron
     G4DynamicParticle *p1 = (G4DynamicParticle*) aTrack.GetDynamicParticle();
     G4ThreeVector vec1 = p1->GetMomentum();
     G4cout << "p1, px: " << vec1.x()/MeV << "  py: " << vec1.y()/MeV << " pz: " << vec1.z()/MeV << "  mag: " << vec1.mag()/MeV << G4endl;
     G4cout << "  Total Mom: " << p1 ->GetTotalMomentum()/MeV << "  TotalE: " << p1->GetTotalEnergy()/MeV << "  KE: " << p1->GetKineticEnergy()/MeV << G4endl;


     G4DynamicParticle *p2 = (G4DynamicParticle*) theAlphaTrack->GetDynamicParticle();
     G4ThreeVector vec2 = p2->GetMomentum();
     G4cout << "p2, px: " << vec2.x()/MeV << "  py: " << vec2.y()/MeV << " pz: " << vec2.z()/MeV << "  mag: " << vec2.mag()/MeV << G4endl;
     G4cout << "  Total Mom: " << p2->GetTotalMomentum()/MeV << "  TotalE: " << p2->GetTotalEnergy()/MeV << "  KE: " << p2->GetKineticEnergy()/MeV << G4endl;


     G4DynamicParticle *p3 = (G4DynamicParticle*) theBe9Track->GetDynamicParticle();
     G4ThreeVector vec3 = p3->GetMomentum();
     G4cout << "p3, px: " << vec3.x()/MeV << "  py: " << vec3.y()/MeV << " pz: " << vec3.z()/MeV << "  mag: " << vec3.mag()/MeV << G4endl;
     G4cout << "  Total Mom: " << p3->GetTotalMomentum()/MeV << "  TotalE: " << p3->GetTotalEnergy()/MeV << "  KE: " << p3->GetKineticEnergy()/MeV << G4endl;
     //-----------
     */

    // G4cout << "Made it to the end ! " << G4endl;
   }
 else if(ReactionName == "N_C12_P_B12")
   {
     // Charge exchange Reaction copied from MENATE
     G4double Q_value = -12.587*MeV;

     G4ThreeVector MomDir_P;
     G4ThreeVector MomDir_B12;

     G4double Available_Eng = 0.9230769*KinEng_Int+Q_value;

     if(Available_Eng < 0.)
       { Available_Eng = 0.; }

     // the proton

     G4double cos_theta_iso = 1.-(2.*G4UniformRand());
     //zwk
     if(fwrdANGLES) cos_theta_iso = pow(G4UniformRand(),1./4.0);

     //1/169. = Mp*Mn/(Mn+MC12)^2
     // .9230769 = MB12/(Mn+MC12)
     // .1478106 = 2*sqrt(Mn*MB12*Mp/(Mc12+Mn)^3)

     G4double T_P = KinEng_Int/169. + 0.9230769*Available_Eng + 
                    (0.1478106*sqrt(KinEng_Int*Available_Eng))*cos_theta_iso;
     
     G4double theta_P = 0.;
     G4double phi_P = 0.;

     if(T_P > 1.e-5)
       {
	 // 1/13. = sqrt(Mp*Mn)/(Mn+Mc)
	 // 0.9607689 = sqrt(MB12/(Mn+MC12))

	 G4double Arg = (sqrt(KinEng_Int)/13. + 0.9607689*sqrt(Available_Eng)*cos_theta_iso)/(sqrt(T_P)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_P = acos(Arg);
	 phi_P = 2.*Pi*G4UniformRand();
       }

     MomDir_P = GenMomDir(MomDir_Int,theta_P,phi_P);

 // B12 - Need to check kinematics - maybe not right - 18 Apr 2008, BTR

     G4double T_B12 = KinEng_Int + Q_value - T_P;
     G4double theta_B12 = Pi - theta_P;
     G4double phi_B12 = Pi + phi_P;

     MomDir_B12 = GenMomDir(MomDir_Int,theta_B12,phi_B12);

     // Generate a Secondary Proton
     G4DynamicParticle* theSecProton = new G4DynamicParticle;
     G4ParticleDefinition* theSecPDefinition = G4Proton::Proton();
     theSecProton->SetDefinition(theSecPDefinition);
     theSecProton->SetKineticEnergy(T_P);
     theSecProton->SetMomentumDirection(MomDir_P);

     // Generate a Secondary B12
     G4DynamicParticle* theSecB12 = new G4DynamicParticle;
     // GetIon() Method works whether particle exists in physicslist or not. 
     // Arguements are GetIon(Charge,Mass,ExcitationEng)
     G4ParticleDefinition* theSecB12Definition = G4IonTable::GetIonTable()->GetIon(5,12,0.);
     theSecB12->SetDefinition(theSecB12Definition);
     theSecB12->SetKineticEnergy(T_B12);
     theSecB12->SetMomentumDirection(MomDir_B12);

    // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     // Set final tracks in motion!
     G4Track* thePTrack = new G4Track(theSecProton,GlobalTime,thePosition);
     thePTrack->SetTouchableHandle(theTouchable);
     G4Track* theB12Track = new G4Track(theSecB12,GlobalTime,thePosition);
     theB12Track->SetTouchableHandle(theTouchable);

     aParticleChange.SetNumberOfSecondaries(2);
     aParticleChange.AddSecondary(thePTrack);
     aParticleChange.AddSecondary(theB12Track);
  
    // G4cout << "Made it to the end ! " << G4endl;
   }
 else if(ReactionName == "N_C12_NNP_B11")
   {
     // Reaction copied from MENATE
     // Treated as :
     // n + 12C ---> 12C* + n'
     // 12C*    ---> 11B + p

     G4double Q_value = -15.956*MeV;

     G4ThreeVector MomDir_N;
     G4ThreeVector MomDir_P;
     G4ThreeVector MomDir_B11;

     // Scattered neutron (n')
     G4double Available_Eng = 0.9230769*KinEng_Int+Q_value;

     if(Available_Eng < 0.)
       { Available_Eng = 1.e-3; }

     G4double Eng_Evap1 = Evaporate_Eng(AMass_Material,Available_Eng);

     G4double cos_theta_iso = 1.-2.*G4UniformRand();
     //zwk
     if(fwrdANGLES) cos_theta_iso = pow(G4UniformRand(),1./4.0);

     // 1/169. = (Mn/(Mn+Mc))^2
     // 0.1538462 = 2Mn/(Mn+Mc)

     G4double T_N = KinEng_Int/169. + Eng_Evap1 + (0.1538462*sqrt(KinEng_Int*Eng_Evap1))*cos_theta_iso;
     
     G4double theta_N = 0.;
     G4double phi_N = 0.;

     if(T_N > 1.e-5)
       {
	 // 1/13. = Mn/(Mn+Mc)

	 G4double Arg = (sqrt(KinEng_Int)/13. + (sqrt(Eng_Evap1)*cos_theta_iso))/(sqrt(T_N)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_N = acos(Arg);
	 phi_N = 2.*Pi*G4UniformRand();
	 //G4cout << "12C(n,np)11B angle of neutron: " << theta_N*180/3.14 << G4endl;
       }

     MomDir_N = GenMomDir(MomDir_Int,theta_N,phi_N);

     // Scattered 12C*
     // 1/13. = Mn(Mc+Mn)
     // 1.0833333 = (Mc+Mn)/Mc

     G4double T_C12Star = KinEng_Int/13. + 1.0833333*Eng_Evap1 - T_N;

     G4double theta_C12Star = 0.;
     G4double phi_C12Star = phi_N+Pi;

     if(T_C12Star > 1.e-5)
       {
	 // 0.2664694 = sqrt(Mn*Mc)/(Mn+Mc)
	 // 0.2886751 = sqrt(Mn/Mc)
	 G4double Arg = (0.2664694*sqrt(KinEng_Int) - (0.2886751*sqrt(Eng_Evap1)*cos_theta_iso))/(sqrt(T_C12Star)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_C12Star = acos(Arg);
       }
     
     // Now the proton is emitted in the frame of the scattered C12*
     // At the end, the angles are put back into the frame of the original neutron
 
     Available_Eng = KinEng_Int + Q_value - T_N - T_C12Star;

     if(Available_Eng < 0.)
       { Available_Eng = 0.; }

     cos_theta_iso = 1.-(2.*G4UniformRand());

     // 1/12. = Mp/MC12
     // .9166667 = MB11/MC12
     // .5527708 = 2*sqrt(MB11*Mp)/Mc

     G4double T_P = T_C12Star/12. + 0.9166667*Available_Eng + 
                    (0.5527708*sqrt(Available_Eng*T_C12Star))*cos_theta_iso;
     
     G4double theta_C12_P = 0.;
     G4double phi_C12_P = 0.;

     if(T_P > 1.e-5)
       {
	 // 0.2886751 = sqrt(Mp/Mc)
	 // 0.9574271 = sqrt((MB11)/(MC12))

	 G4double Arg = (0.2886751*sqrt(T_C12Star) + 0.9574271*sqrt(Available_Eng)*cos_theta_iso)/(sqrt(T_P)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_C12_P = acos(Arg);
	 phi_C12_P = 2.*Pi*G4UniformRand();
       }

     // Now Start With Z-Axis and then go to incoming neutron frame ?
     //G4ThreeVector MomDir_ZAxis = G4ThreeVector(0.,0.,1.);

     G4ThreeVector MomDir_ZAxis = MomDir_Int;
     G4ThreeVector MomDir_C12Star = GenMomDir(MomDir_ZAxis,theta_C12Star,phi_C12Star);

     MomDir_P = GenMomDir(MomDir_C12Star,theta_C12_P,phi_C12_P);

     // B11 - Need to check kinematics - maybe not right - 18 Apr 2008, BTR

     G4double T_B11 = KinEng_Int + Q_value - T_N - T_P; // Remaining Eng
     if(T_B11 < 0.)
       {T_B11 = 1e-9;}

     G4double theta_B11 = Pi - theta_C12_P;
     G4double phi_B11 = Pi + phi_C12_P;

     MomDir_B11 = GenMomDir(MomDir_Int,theta_B11,phi_B11);

// Generate a Secondary Neutron
     G4DynamicParticle* theSecNeutron = new G4DynamicParticle;
     G4ParticleDefinition* theSecNDefinition = G4Neutron::Neutron();
     theSecNeutron->SetDefinition(theSecNDefinition);
     theSecNeutron->SetKineticEnergy(T_N);
     theSecNeutron->SetMomentumDirection(MomDir_N);

// Generate a Secondary Proton
     G4DynamicParticle* theSecProton = new G4DynamicParticle;
     G4ParticleDefinition* theSecPDefinition = G4Proton::Proton();
     theSecProton->SetDefinition(theSecPDefinition);
     theSecProton->SetKineticEnergy(T_P);
     theSecProton->SetMomentumDirection(MomDir_P);

     // Generate a Secondary B11
     G4DynamicParticle* theSecB11 = new G4DynamicParticle;
     // GetIon() Method works whether particle exists in physicslist or not. 
     // Arguements are GetIon(Charge,Mass,ExcitationEng)
     G4ParticleDefinition* theSecB11Definition = G4IonTable::GetIonTable()->GetIon(5,11,0.);
     theSecB11->SetDefinition(theSecB11Definition);
     theSecB11->SetKineticEnergy(T_B11);
     theSecB11->SetMomentumDirection(MomDir_B11);

    // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     // Set final tracks in motion!
     G4Track* theNTrack = new G4Track(theSecNeutron,GlobalTime,thePosition);
     theNTrack->SetTouchableHandle(theTouchable);
     G4Track* thePTrack = new G4Track(theSecProton,GlobalTime,thePosition);
     thePTrack->SetTouchableHandle(theTouchable);
     G4Track* theB11Track = new G4Track(theSecB11,GlobalTime,thePosition);
     theB11Track->SetTouchableHandle(theTouchable);

     aParticleChange.SetNumberOfSecondaries(3);
     aParticleChange.AddSecondary(theNTrack);
     aParticleChange.AddSecondary(thePTrack);
     aParticleChange.AddSecondary(theB11Track);
  
    // G4cout << "Made it to the end ! " << G4endl;
   }
 else if(ReactionName == "N_C12_N2N_C11")
   {
     // Reaction copied from MENATE
     // Treated as :
     // n + 12C ---> 12C* + n'
     // 12C*    ---> 11C + n

     G4double Q_value = -18.721*MeV;

     G4ThreeVector MomDir_N1;
     G4ThreeVector MomDir_N2;
     G4ThreeVector MomDir_C11;

     // Scattered neutron (n')
     G4double Available_Eng = 0.9230769*KinEng_Int+Q_value;

     if(Available_Eng < 0.)
       { Available_Eng = 1.e-3; }

     G4double Eng_Evap1 = Evaporate_Eng(AMass_Material,Available_Eng);

     G4double cos_theta_iso = 1.-(2.*G4UniformRand());
     //zwk
     if(fwrdANGLES) cos_theta_iso = pow(G4UniformRand(),1./4.0);

     // 1/169. = (Mn/(Mn+Mc))^2
     // 0.1538462 = 2Mn/(Mn+Mc)

     G4double T_N1 = KinEng_Int/169. + Eng_Evap1 + (0.1538462*sqrt(KinEng_Int*Eng_Evap1))*cos_theta_iso;
     
     G4double theta_N1 = 0.;
     G4double phi_N1 = 0.;

     if(T_N1 > 1.e-5)
       {
	 // 1/13. = Mn/(Mn+Mc)

	 G4double Arg = (sqrt(KinEng_Int)/13. + (sqrt(Eng_Evap1)*cos_theta_iso))/(sqrt(T_N1)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_N1 = acos(Arg);
	 phi_N1 = 2.*Pi*G4UniformRand();
       }

     MomDir_N1 = GenMomDir(MomDir_Int,theta_N1,phi_N1);

     // Scattered 12C*
     // 1/13. = Mn(Mc+Mn)
     // 1.0833333 = (Mc+Mn)/Mc

     G4double T_C12Star = KinEng_Int/13. + 1.0833333*Eng_Evap1 - T_N1;

     G4double theta_C12Star = 0.;
     G4double phi_C12Star = phi_N1+Pi;

     if(T_C12Star > 1.e-5)
       {
	 // 0.2664694 = sqrt(Mn*Mc)/(Mn+Mc)
	 // 0.2886751 = sqrt(Mn/Mc)
	 G4double Arg = (0.2664694*sqrt(KinEng_Int) - (0.2886751*sqrt(Eng_Evap1)*cos_theta_iso))/(sqrt(T_C12Star)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_C12Star = acos(Arg);
       }
     
     // Now neutron2 is emitted in the frame of the scattered C12*
     // At the end, the angles are put back into the frame of the original neutron
 
     Available_Eng = KinEng_Int + Q_value - T_N1 - T_C12Star;

     if(Available_Eng < 0.)
       { Available_Eng = 1.e-3; }

     G4double Eng_Evap2 = Evaporate_Eng(AMass_Material,Available_Eng);

     cos_theta_iso = 1.-(2.*G4UniformRand());

     // .5773503 = 2*sqrt(Mn/Mc)

     G4double T_N2 = T_C12Star/12. + Eng_Evap2 + 
                    (0.5773503*sqrt(T_C12Star*Eng_Evap2))*cos_theta_iso;
     
     G4double theta_C12_N2 = acos(cos_theta_iso);
     G4double phi_C12_N2 = 2.*Pi*G4UniformRand();
	 
     // Now Start With Z-Axis and then go to incoming neutron frame ?
     //G4ThreeVector MomDir_ZAxis = G4ThreeVector(0.,0.,1.);
    
     G4ThreeVector MomDir_ZAxis = MomDir_Int;
     G4ThreeVector MomDir_C12Star = GenMomDir(MomDir_ZAxis,theta_C12Star,phi_C12Star);

     MomDir_N2 = GenMomDir(MomDir_C12Star,theta_C12_N2,phi_C12_N2);

     // C11 - Need to check kinematics - maybe not right - 18 Apr 2008, BTR

     G4double T_C11 = KinEng_Int + Q_value - T_N1 - T_N2; // Remaining Eng
     if(T_C11 <= 0.)
       {T_C11 = 1e-9;}

     G4double theta_C11 = Pi - theta_C12_N2;
     G4double phi_C11 = Pi + phi_C12_N2;

     MomDir_C11 = GenMomDir(MomDir_Int,theta_C11,phi_C11);

// Generate a Secondary Neutron1
     G4DynamicParticle* theSecNeutron1 = new G4DynamicParticle;
     G4ParticleDefinition* theSecN1Definition = G4Neutron::Neutron();
     theSecNeutron1->SetDefinition(theSecN1Definition);
     theSecNeutron1->SetKineticEnergy(T_N1);
     theSecNeutron1->SetMomentumDirection(MomDir_N1);

// Generate a Secondary Neutron2
     G4DynamicParticle* theSecNeutron2 = new G4DynamicParticle;
     G4ParticleDefinition* theSecN2Definition = G4Neutron::Neutron();
     theSecNeutron2->SetDefinition(theSecN2Definition);
     theSecNeutron2->SetKineticEnergy(T_N2);
     theSecNeutron2->SetMomentumDirection(MomDir_N2);

     // Generate a Secondary C11
     G4DynamicParticle* theSecC11 = new G4DynamicParticle;
     // GetIon() Method works whether particle exists in physicslist or not. 
     // Arguements are GetIon(Charge,Mass,ExcitationEng)
     G4ParticleDefinition* theSecC11Definition = G4IonTable::GetIonTable()->GetIon(6,11,0.);
     theSecC11->SetDefinition(theSecC11Definition);
     theSecC11->SetKineticEnergy(T_C11);
     theSecC11->SetMomentumDirection(MomDir_C11);

    // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     // Set final tracks in motion!
     G4Track* theN1Track = new G4Track(theSecNeutron1,GlobalTime,thePosition);
     theN1Track->SetTouchableHandle(theTouchable);
     G4Track* theN2Track = new G4Track(theSecNeutron2,GlobalTime,thePosition);
     theN2Track->SetTouchableHandle(theTouchable);
     G4Track* theC11Track = new G4Track(theSecC11,GlobalTime,thePosition);
     theC11Track->SetTouchableHandle(theTouchable);

     aParticleChange.SetNumberOfSecondaries(3);
     aParticleChange.AddSecondary(theN1Track);
     aParticleChange.AddSecondary(theN2Track);
     aParticleChange.AddSecondary(theC11Track);

     /*
     G4double T_N_Sum = T_N1+T_N2;
     G4cout << "n, 2n C11 event! " << G4endl;
     G4cout << "T_N1 = " << T_N1 << "T_N2 = " << T_N2 << " , T_Sum = " << T_N_Sum << G4endl;
     G4cout << "Total Eng Final State : " << T_N_Sum+T_C11 << " , Eng_Int+Qvalue = " << KinEng_Int+Q_value << G4endl; 
     */     
    // G4cout << "Made it to the end ! " << G4endl;
   }
else if(ReactionName == "N_C12_NN3Alpha")
   {
     // Reaction copied from MENATE
     // Treated as :
     // n + 12C ---> 12C* + n'
     // 12C*    ---> 8Be + alpha1
     // 8Be*    ---> alpha2+alpha3

     G4double Q_value = -7.275*MeV;

     G4ThreeVector MomDir_N;
     G4ThreeVector MomDir_Alpha1;
     G4ThreeVector MomDir_Alpha2;
     G4ThreeVector MomDir_Alpha3;

     // Scattered neutron (n')
     G4double Available_Eng = 0.9230769*KinEng_Int+Q_value;

     if(Available_Eng < 0.)
       { Available_Eng = 1.e-3; }

     G4double Eng_Evap1 = Evaporate_Eng(AMass_Material,Available_Eng);

     G4double cos_theta_iso = 1.-(2.*G4UniformRand());
     //zwk
     if(fwrdANGLES) cos_theta_iso = pow(G4UniformRand(),1./4.0);

     // 1/169. = (Mn/(Mn+Mc))^2
     // 0.1538462 = 2Mn/(Mn+Mc)

     G4double T_N = KinEng_Int/169. + Eng_Evap1 + (0.1538462*sqrt(KinEng_Int*Eng_Evap1))*cos_theta_iso;
     
     G4double theta_N = 0.;
     G4double phi_N = 0.;

     if(T_N > 1.e-5)
       {
	 // 1/13. = Mn/(Mn+Mc)

	 G4double Arg = (sqrt(KinEng_Int)/13. + (sqrt(Eng_Evap1)*cos_theta_iso))/(sqrt(T_N)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_N = acos(Arg);
	 phi_N = 2.*Pi*G4UniformRand();
       }

     MomDir_N = GenMomDir(MomDir_Int,theta_N,phi_N);

     // Scattered 12C*
     // 1/13. = Mn(Mc+Mn)
     // 1.0833333 = (Mc+Mn)/Mc

     G4double T_C12Star = KinEng_Int/13. + 1.0833333*Eng_Evap1 - T_N;

     G4double theta_C12Star = 0.;
     G4double phi_C12Star = phi_N+Pi;

     if(T_C12Star > 1.e-5)
       {
	 // 0.2664694 = sqrt(Mn*Mc)/(Mn+Mc)
	 // 0.2886751 = sqrt(Mn/Mc)
	 G4double Arg = (0.2664694*sqrt(KinEng_Int) - (0.2886751*sqrt(Eng_Evap1)*cos_theta_iso))/(sqrt(T_C12Star)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_C12Star = acos(Arg);
       }
     
     // Now Alpha1 is emitted in the frame of the scattered C12*
     // At the end, the angles are put back into the frame of the original neutron
 
     Available_Eng = (KinEng_Int + Q_value - T_N - T_C12Star)*G4UniformRand();

     if(Available_Eng < 0.)
       { Available_Eng = 0.; }

     cos_theta_iso = 1.-(2.*G4UniformRand());

     // 1./3. = MA1/MC12
     // 0.6666667 = MBe8/MC12
     // 0.9428090 = 2*sqrt(Malpha*M8Be)/Mc12)

     G4double T_Alpha1 = T_C12Star/3. + 0.6666667*Available_Eng + 
                    (0.9428090*sqrt(Available_Eng*T_C12Star))*cos_theta_iso;
     
     G4double theta_Alpha1 = 0.;
     G4double phi_Alpha1 = 0.;

     if(T_Alpha1 > 1.e-5)
       {
	 // 0.5773503 = sqrt(Malpha/MC12)
	 // 0.8164966 = sqrt((MBe8)/(MC12))

	 G4double Arg = (0.5773503*sqrt(T_C12Star) + 0.8164966*sqrt(Available_Eng)*cos_theta_iso)/(sqrt(T_Alpha1)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_Alpha1 = acos(Arg);
	 phi_Alpha1 = 2.*Pi*G4UniformRand();
       }

     // Now Start With Z-Axis and then go to incoming neutron frame ?
     //  G4ThreeVector MomDir_ZAxis = G4ThreeVector(0.,0.,1.);
     G4ThreeVector MomDir_ZAxis = MomDir_Int;
     G4ThreeVector MomDir_C12Star = GenMomDir(MomDir_ZAxis,theta_C12Star,phi_C12Star);

     MomDir_Alpha1 = GenMomDir(MomDir_C12Star,theta_Alpha1,phi_Alpha1);

     // Be8* Kinematics --- Particle decayed into the other alphas
     // Angles in frame of scattered C12*

     G4double T_Be8Star = T_C12Star + Available_Eng - T_Alpha1; // Remaining Eng
    
     G4double theta_Be8Star = 0.;
     G4double phi_Be8Star = phi_Alpha1+Pi;

     if(T_Be8Star > 1.e-5)
       {
	 // 0.5773503 = sqrt(Malpha/MC12)
	 // 0.8164966 = sqrt(M8Be/MC12)
	 G4double Arg = (0.8164966*sqrt(T_C12Star) - (0.5773503*sqrt(Available_Eng)*cos_theta_iso))/(sqrt(T_Be8Star)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_Be8Star = acos(Arg);
       }

     G4ThreeVector MomDir_Be8Star = GenMomDir(MomDir_C12Star,theta_Be8Star,phi_Be8Star);

     // Now Decay 8Be g.s. into 2 Alphas
     // Q_value = 0.092*MeV

     G4double Q_valueBe8 = 0.092*MeV;
     Available_Eng = KinEng_Int+Q_value+Q_valueBe8-T_N-T_Alpha1-T_Be8Star; // Changed 23 Apr 2008 - BTR

     if(Available_Eng < 0.)
       {Available_Eng = 0.;}

     cos_theta_iso = 1.-(2.*G4UniformRand());

     // 0.5 = MA2/Be8
 
     G4double T_Alpha2 = 0.5*(T_Be8Star+Available_Eng) + 
                    (sqrt(Available_Eng*T_Be8Star))*cos_theta_iso;
      if(T_Alpha2 < 0.)
       {T_Alpha2 = 1e-9;}

     G4double theta_Alpha2 = 0.;
     G4double phi_Alpha2 = 0.;

     if(T_Alpha2 > 1.e-5)
       {
	 // 0.7071068 = sqrt(Malpha/MBe8)
	
	 G4double Arg = (0.7071068*(sqrt(T_Be8Star) + sqrt(Available_Eng)*cos_theta_iso))/(sqrt(T_Alpha2)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_Alpha2 = acos(Arg);
	 phi_Alpha2 = 2.*Pi*G4UniformRand();
       }

     MomDir_Alpha2 = GenMomDir(MomDir_Be8Star,theta_Alpha2,phi_Alpha2);

     // Alpha3 emitted in frame of Be8

     G4double T_Alpha3 = T_Be8Star + Available_Eng - T_Alpha2;
    if(T_Alpha3 < 0.)
       {T_Alpha3 = 1e-9;}
  
     G4double theta_Alpha3 = 0.;
     G4double phi_Alpha3 = phi_Alpha2+Pi;

     if(T_Alpha3 > 1.e-5)
       {
	 // 0.7071068 = sqrt(Malpha/MBe8)
	
	 G4double Arg = (0.7071068*(sqrt(T_Be8Star) - sqrt(Available_Eng)*cos_theta_iso))/(sqrt(T_Alpha3)); 
	 if( Arg > 1.)
	   { Arg = 1.; }
	 theta_Alpha3 = acos(Arg);
       }

     MomDir_Alpha3 = GenMomDir(MomDir_Be8Star,theta_Alpha3,phi_Alpha3);

// Generate a Secondary Neutron
     G4DynamicParticle* theSecNeutron = new G4DynamicParticle;
     G4ParticleDefinition* theSecNDefinition = G4Neutron::Neutron();
     theSecNeutron->SetDefinition(theSecNDefinition);
     theSecNeutron->SetKineticEnergy(T_N);
     theSecNeutron->SetMomentumDirection(MomDir_N);

// Generate a Secondary Alpha1
     G4DynamicParticle* theSecAlpha1 = new G4DynamicParticle;
     G4ParticleDefinition* theSecA1Definition = G4Alpha::Alpha();
     theSecAlpha1->SetDefinition(theSecA1Definition);
     theSecAlpha1->SetKineticEnergy(T_Alpha1);
     theSecAlpha1->SetMomentumDirection(MomDir_Alpha1);

// Generate a Secondary Alpha2
     G4DynamicParticle* theSecAlpha2 = new G4DynamicParticle;
     G4ParticleDefinition* theSecA2Definition = G4Alpha::Alpha();
     theSecAlpha2->SetDefinition(theSecA2Definition);
     theSecAlpha2->SetKineticEnergy(T_Alpha2);
     theSecAlpha2->SetMomentumDirection(MomDir_Alpha2);

// Generate a Secondary Alpha3
     G4DynamicParticle* theSecAlpha3 = new G4DynamicParticle;
     G4ParticleDefinition* theSecA3Definition = G4Alpha::Alpha();
     theSecAlpha3->SetDefinition(theSecA3Definition);
     theSecAlpha3->SetKineticEnergy(T_Alpha3);
     theSecAlpha3->SetMomentumDirection(MomDir_Alpha3);

    
    // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     // Set final tracks in motion!
     G4Track* theNTrack = new G4Track(theSecNeutron,GlobalTime,thePosition);
     theNTrack->SetTouchableHandle(theTouchable);
     G4Track* theA1Track = new G4Track(theSecAlpha1,GlobalTime,thePosition);
     theA1Track->SetTouchableHandle(theTouchable);
     G4Track* theA2Track = new G4Track(theSecAlpha2,GlobalTime,thePosition);
     theA2Track->SetTouchableHandle(theTouchable);
     G4Track* theA3Track = new G4Track(theSecAlpha3,GlobalTime,thePosition);
     theA3Track->SetTouchableHandle(theTouchable);

     aParticleChange.SetNumberOfSecondaries(4);
     aParticleChange.AddSecondary(theNTrack);
     aParticleChange.AddSecondary(theA1Track);
     aParticleChange.AddSecondary(theA2Track);
     aParticleChange.AddSecondary(theA3Track);

     /*
     G4double T_A_Sum = T_Alpha1+T_Alpha2+T_Alpha3;

     G4cout << "n, n'3alpha event! " << G4endl;
     G4cout << "T_N = " << T_N << G4endl;
     G4cout << "A1 = " << T_Alpha1 << " ,A2 = " << T_Alpha2 << " , A3 = " << T_Alpha3 << ", A_Sum = " << T_A_Sum << G4endl;
     G4cout << "Total Eng Final State : " << T_A_Sum+T_N << " , Eng_Int-Qvalue = " << KinEng_Int-7.275+0.092 << G4endl; 
     */
    // G4cout << "Made it to the end ! " << G4endl;
   }
 else if(ReactionName == "N_Fe_elastic")
   {
     G4double theta_N  = 0.;
     G4double phi_N = 0.;

     G4double T_N = 0.;
//     G4double T_Feel = 0.;
     G4ThreeVector MomDir_N;
     G4ThreeVector MomDir_Fe;

     /*
     //Just going to use isotropic distribution in CM
     //However good consider using C12 elastic scattering, may be similar
     G4double cos_sq_theta = G4UniformRand(); 

     // Calls diffractive angular distribution as in DEMONS
     //G4double cos_theta_cm = NC12_DIFF(KinEng_Int);
     // Convert according to Knoll
     //G4double cos_theta_lab = sqrt((1.-cos_theta_cm)/2.);
     //G4double cos_sq_theta = pow(cos_theta_lab,2);
    
     // .070177              =4*Mfe*Mn/(Mfe+Mn)^2
     T_Feel = 0.07017*KinEng_Int*cos_sq_theta;

     T_N = KinEng_Int - T_Feel;

     if(T_N > 1.e-5)
       {
	 G4double arg = (sqrt(KinEng_Int)-sqrt(12.*T_Feel*cos_sq_theta))/sqrt(T_N);
	 if(arg > 1.)
	   {arg = 1.;}
	 theta_N = acos(arg);
         phi_N = 2.*Pi*G4UniformRand();
       }

     MomDir_N = GenMomDir(MomDir_Int,theta_N,phi_N);

     // Carbon angles
     G4double theta_Fe = acos(sqrt(cos_sq_theta));
     G4double phi_Fe = phi_N+Pi;

     MomDir_Fe = GenMomDir(MomDir_Int,theta_Fe,phi_Fe);
     */


     double cos_theta_cm = FeElastic_AngDist(KinEng_Int); // cosine of cm_angle
     if(cos_theta_cm > 1) cos_theta_cm = 1;
     double theta_cm = acos(cos_theta_cm); // of neutron
     
     // Following Knoll, calculate energy for proton first
     double cos_theta_lab_Fe = sqrt((1.-cos_theta_cm)/2.);
     double theta_lab_Fe = acos(cos_theta_lab_Fe);
     double phi_Fe = 2.*Pi*G4UniformRand();
     
     double T_Fe = KinEng_Int*pow(cos_theta_lab_Fe,2)*4*56/pow(57.,2.);
     
     MomDir_Fe = GenMomDir(MomDir_Int,theta_lab_Fe,phi_Fe);
     
     // scattered neutron ----------------
     // angles changed from cm to lab as in Marian and Young
     theta_N = atan(sin(theta_cm)/(cos_theta_cm+(1/56)) );
     phi_N = phi_Fe+Pi;
     
     T_N = KinEng_Int - T_Fe;    
     
     MomDir_N = GenMomDir(MomDir_Int,theta_N,phi_N);
     
     // Generate a Secondary Neutron
     G4DynamicParticle* theSecNeutron = new G4DynamicParticle;
     G4ParticleDefinition* theSecNeutronDefinition = G4Neutron::Neutron();
     theSecNeutron->SetDefinition(theSecNeutronDefinition);
     theSecNeutron->SetKineticEnergy(T_N);
     theSecNeutron->SetMomentumDirection(MomDir_N);

     // This is where you would generate the recoil Fe nucleus
     //However for our purpose (Fe is only in sweeper) we don't
     //need to keep track.
     //G4DynamicParticle* theSecC12 = new G4DynamicParticle;
     // GetIon() Method works whether particle exists in physicslist or not. 
     // Arguements are GetIon(Charge,Mass,ExcitationEng)
     //G4ParticleDefinition* theSecC12Definition = G4IonTable::GetIonTable()->GetIon(6,12,0.);
     //theSecC12->SetDefinition(theSecC12Definition);
     //theSecC12->SetKineticEnergy(T_C12el);
     //theSecC12->SetMomentumDirection(MomDir_C12);
   
     // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     // Set final tracks in motion!
     G4Track* theNTrack = new G4Track(theSecNeutron,GlobalTime,thePosition);
     theNTrack->SetTouchableHandle(theTouchable);
     //G4Track* theC12Track = new G4Track(theSecC12,GlobalTime,thePosition);
     //theC12Track->SetTouchableHandle(theTouchable);

     aParticleChange.SetNumberOfSecondaries(1);
     aParticleChange.AddSecondary(theNTrack);
     //aParticleChange.AddSecondary(theC12Track);
  
    // G4cout << "Made it to the end ! " << G4endl;
     //G4cout << "Finished Elastic Scatter of Fe" << G4endl;
   }
 else if(ReactionName == "N_Fe_nonelastic")
   {

     //For our purposes we are going to treat the non-elastic Fe interactions as absorption of the neutrons
     //the idea is that after the interaction the neutron would be slowed and may not make it into the timing gate
     //furthermore we don't have enought information (beyond the cross sections) to know how to handle the 
     //non-elastic processes.

   
     // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     aParticleChange.SetNumberOfSecondaries(0);
     //aParticleChange.AddSecondary(theNTrack);
     //aParticleChange.AddSecondary(theC12Track);
  
    // G4cout << "Made it to the end ! " << G4endl;
     //G4cout << "Finished Fe Non-Elastic Process" << G4endl;
   }
 else if(ReactionName == "N_Al_elastic")
   {

    G4double theta_N  = 0.;
     G4double phi_N = 0.;

     G4double T_N = 0.;
//     G4double T_Feel = 0.;
     G4ThreeVector MomDir_N;
     G4ThreeVector MomDir_Al;

     /*
     //Just going to use isotropic distribution in CM
     //However good consider using C12 elastic scattering, may be similar
     G4double cos_sq_theta = G4UniformRand(); 

     // Calls diffractive angular distribution as in DEMONS
     //G4double cos_theta_cm = NC12_DIFF(KinEng_Int);
     // Convert according to Knoll
     //G4double cos_theta_lab = sqrt((1.-cos_theta_cm)/2.);
     //G4double cos_sq_theta = pow(cos_theta_lab,2);
    
     // .070177              =4*Mfe*Mn/(Mfe+Mn)^2
     T_Feel = 0.07017*KinEng_Int*cos_sq_theta;

     T_N = KinEng_Int - T_Feel;

     if(T_N > 1.e-5)
       {
	 G4double arg = (sqrt(KinEng_Int)-sqrt(12.*T_Feel*cos_sq_theta))/sqrt(T_N);
	 if(arg > 1.)
	   {arg = 1.;}
	 theta_N = acos(arg);
         phi_N = 2.*Pi*G4UniformRand();
       }

     MomDir_N = GenMomDir(MomDir_Int,theta_N,phi_N);

     // Carbon angles
     G4double theta_Fe = acos(sqrt(cos_sq_theta));
     G4double phi_Fe = phi_N+Pi;

     MomDir_Fe = GenMomDir(MomDir_Int,theta_Fe,phi_Fe);
     */


     double cos_theta_cm = AlElastic_AngDist(KinEng_Int); // cosine of cm_angle
     if(cos_theta_cm > 1) cos_theta_cm = 1;
     double theta_cm = acos(cos_theta_cm); // of neutron
     
     // Following Knoll, calculate energy for proton first
     double cos_theta_lab_Al = sqrt((1.-cos_theta_cm)/2.);
     double theta_lab_Al = acos(cos_theta_lab_Al);
     double phi_Al = 2.*Pi*G4UniformRand();
     
     double T_Al = KinEng_Int*pow(cos_theta_lab_Al,2)*4*27/pow(28.,2);

     //G4cout << "Al theta: " << cos_theta_cm << " " << theta_cm << " " << cos_theta_lab_Al << " " << theta_lab_Al << G4endl;
     //G4cout << "Al KE: " << T_Al/MeV << " Init_N KE: " << KinEng_Int/MeV << G4endl;
     
     MomDir_Al = GenMomDir(MomDir_Int,theta_lab_Al,phi_Al);
     
     // scattered neutron ----------------
     // angles changed from cm to lab as in Marian and Young
     theta_N = atan(sin(theta_cm)/(cos_theta_cm+(1/27)) );
     phi_N = phi_Al+Pi;
     
     T_N = KinEng_Int - T_Al;    
     
     MomDir_N = GenMomDir(MomDir_Int,theta_N,phi_N);
     
     // Generate a Secondary Neutron
     G4DynamicParticle* theSecNeutron = new G4DynamicParticle;
     G4ParticleDefinition* theSecNeutronDefinition = G4Neutron::Neutron();
     theSecNeutron->SetDefinition(theSecNeutronDefinition);
     theSecNeutron->SetKineticEnergy(T_N);
     theSecNeutron->SetMomentumDirection(MomDir_N);

     // This is where you would generate the recoil Fe nucleus
     //However for our purpose (Fe is only in sweeper) we don't
     //need to keep track.
     //G4DynamicParticle* theSecC12 = new G4DynamicParticle;
     // GetIon() Method works whether particle exists in physicslist or not. 
     // Arguements are GetIon(Charge,Mass,ExcitationEng)
     //G4ParticleDefinition* theSecC12Definition = G4IonTable::GetIonTable()->GetIon(6,12,0.);
     //theSecC12->SetDefinition(theSecC12Definition);
     //theSecC12->SetKineticEnergy(T_C12el);
     //theSecC12->SetMomentumDirection(MomDir_C12);
   
     // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     // Set final tracks in motion!
     G4Track* theNTrack = new G4Track(theSecNeutron,GlobalTime,thePosition);
     theNTrack->SetTouchableHandle(theTouchable);
     //G4Track* theC12Track = new G4Track(theSecC12,GlobalTime,thePosition);
     //theC12Track->SetTouchableHandle(theTouchable);

     aParticleChange.SetNumberOfSecondaries(1);
     aParticleChange.AddSecondary(theNTrack);
     //aParticleChange.AddSecondary(theC12Track);
  
    // G4cout << "Made it to the end ! " << G4endl;
     //G4cout << "Finished Elastic Scatter of Fe" << G4endl;
   }
 else if(ReactionName == "N_Al_nonelastic")
   {

     //For our purposes we are going to treat the non-elastic Fe interactions as absorption of the neutrons
     //the idea is that after the interaction the neutron would be slowed and may not make it into the timing gate
     //furthermore we don't have enought information (beyond the cross sections) to know how to handle the 
     //non-elastic processes.

   
     // Kill the Parent Neutron -----------------------
     aParticleChange.ProposeTrackStatus(fStopAndKill);
     aParticleChange.ProposeEnergy(0.);
     aParticleChange.ProposePosition(thePosition);
  
     aParticleChange.SetNumberOfSecondaries(0);
     //aParticleChange.AddSecondary(theNTrack);
     //aParticleChange.AddSecondary(theC12Track);
  
    // G4cout << "Made it to the end ! " << G4endl;
     //G4cout << "Finished Fe Non-Elastic Process" << G4endl;
   }


 return pParticleChange;
}
