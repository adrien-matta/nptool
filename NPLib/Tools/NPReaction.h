#ifndef __REACTION__
#define __REACTION__

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
			Nucleus			*fNoy1				;	//	Beam
			Nucleus			*fNoy2				;	//	Target
			Nucleus			*fNoy3				;	//	Light ejectile
			Nucleus			*fNoy4				;	//	Heavy ejectile
			double	 		fQValue				;	//	Q-value in MeV
			double	 		fBeamEnergy			;	//	Beam energy in MeV
			double	 		fThetaCM			;	//	Center-of-mass angle in radian
			double	 		fExcitation			;	//	Excitation energy in MeV
			double*			CrossSection		;	//	Differential CrossSection
			int				CrossSectionSize	;	//	Size of array containing Differention CrossSection
		   
		public:
			// Getters and Setters
			void				SetBeamEnergy		(double efais)	{fBeamEnergy = efais;}
			void				SetThetaCM			(double angle)	{fThetaCM = angle;}
			void				SetExcitation		(double exci)	{fExcitation = exci;}
			double				GetBeamEnergy() 	const		 	{return fBeamEnergy;}
			double				GetThetaCM() 		const 			{return fThetaCM;}
			double				GetExcitation() 	const 			{return fExcitation;}
			double				GetQValue() 		const 			{return fQValue;}
			Nucleus*			GetNucleus1() 		const 			{return fNoy1;}
			Nucleus*			GetNucleus2() 		const 			{return fNoy2;}
			Nucleus*			GetNucleus3() 		const 			{return fNoy3;}
			Nucleus*			GetNucleus4() 		const 			{return fNoy4;}  
			double*				GetCrossSection()	const			{return CrossSection;} 
			int					GetCrossSectionSize()		const			{return CrossSectionSize;} 

			//	Kinematics	//
		   
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
											   	double ExcitationEnergy ) 	//	Excitation Energy previously calculated. If no argument given, fExcitation is used
												const;
			
			//	Other	//
											   	
			//	Print private paremeter									
			void		Print() const;
		};
}
#endif
