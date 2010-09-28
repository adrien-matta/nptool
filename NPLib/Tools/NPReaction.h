#ifndef __REACTION__
#define __REACTION__
/*****************************************************************************
 * Copyright (C) 2009 	this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: matta@ipno.in2p3.fr   *
 *                                                                           *
 * Creation Date   : March 2009                                              *
 * Last update     :                                                         *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *	This class deal with Two Body transfert Reaction                         *
 *	Physical parameter (Nuclei mass) are loaded from the nubtab03.asc file   *
 *	(2003 nuclear table of isotopes mass).                                   *
 *	                                                                         *
 *	KineRelativistic: Used in NPSimulation                                   *
 *	A relativistic calculation is made to compute Light and Heavy nuclei     * 
 *	angle given the Theta CM angle.                                          *
 *	                                                                         * 
 *	ReconstructRelativistic: Used in NPAnalysis                              *
 *	A relativistic calculation is made to compute Excitation energy given the*
 *	light angle and energy in Lab frame.                                     *
 *	                                                                         *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>

#include "NPNucleus.h"

using namespace std;
//class Nucleus;

namespace NPL 
	{
		class Reaction {

		public:	//	Constructors and Destructors
			Reaction();
			Reaction(string name1, string name2, string name3, string name4, double BeamEnergy , double ExcitationEnergy, string Path );
			~Reaction();

		public:	//	Various Method
			void	SetEveryThing(string name1, string name2, string name3, string name4, double BeamEnergy, double ExcitationEnergy,string Path)	;
			void	ReadConfigurationFile(string Path)		;

		private:
			Nucleus			*fNuclei1				;	//	Beam
			Nucleus			*fNuclei2				;	//	Target
			Nucleus			*fNuclei3				;	//	Light ejectile
			Nucleus			*fNuclei4				;	//	Heavy ejectile
			double	 		fQValue				;	//	Q-value in MeV
			double	 		fBeamEnergy			;	//	Beam energy in MeV
			double	 		fThetaCM			;	//	Center-of-mass angle in radian
			double	 		fExcitation			;	//	Excitation energy in MeV
			double*			CrossSection		;	//	Differential CrossSection
			int					CrossSectionSize	;	//	Size of array containing Differention CrossSection
		   
		public:
			// Getters and Setters
			void				SetBeamEnergy						(double eBeam)	{fBeamEnergy = eBeam;initializePrecomputeVariable();}
			void				SetThetaCM							(double angle)	{fThetaCM = angle;initializePrecomputeVariable();}
			void				SetExcitation						(double exci)		{fExcitation = exci;initializePrecomputeVariable();}
			double			GetBeamEnergy() 				const		 				{return fBeamEnergy;}
			double			GetThetaCM() 						const 					{return fThetaCM;}
			double			GetExcitation() 				const 					{return fExcitation;}
			double			GetQValue() 						const 					{return fQValue;}
			Nucleus*		GetNucleus1() 					const 					{return fNuclei1;}
			Nucleus*		GetNucleus2() 					const 					{return fNuclei2;}
			Nucleus*		GetNucleus3() 					const 					{return fNuclei3;}
			Nucleus*		GetNucleus4() 					const 					{return fNuclei4;}  
			double*			GetCrossSection()				const						{return CrossSection;} 
			int					GetCrossSectionSize()		const						{return CrossSectionSize;} 




		private:	//	intern precompute variable
			void initializePrecomputeVariable();
			double m1 ;
			double m2 ;
			double m3 ;
			double m4 ;

			// center-of-mass velocity
			double WtotLab ;
			double P1 ;
			double B ;
			double G ;

			// total energy of the ejectiles in the center-of-mass
			double W3cm ;
			double W4cm ;

			// velocity of the ejectiles in the center-of-mass
			double beta3cm  ;
			double beta4cm  ;

			// Constants of the kinematics
			double K3 ;
			double K4 ;

			
		 public:	//	Kinematics
		  // Check that the reaction is alowed
		  bool CheckKinematic();
		 
		 
			//	Compute ThetaLab and EnergyLab for product of reaction
			void		KineRelativistic(	double &ThetaLab3	, 
										 	double &EnergieLab3	,
					         		 		double &ThetaLab4	,
					         		 		double &EnergieLab4	) const;
					         		 
			//	Return Excitation Energy		         		 
			double		ReconstructRelativistic(	double EnergyLab, 
													double ThetaLab) const;
			//	Return ThetaCM									
			double 		EnergyLabToThetaCM( 	double EnergyLab 		,	//	Energy Measured in lab ref
											   	double ExcitationEnergy = -500) 	//	Excitation Energy previously calculated. If no argument given, fExcitation is used
												const;
			
			//	Other	//
											   	
			//	Print private paremeter									
			void		Print() const;
		};
}
#endif
