#ifndef HAVE_MDM_PHYS_MIN
#define HAVE_MDM_PHYS_MIN

// ROOT includes
#include <vector>
#include <utility>
#include "Math/Functor.h"


class TMDMPhysics;

class TMDMPhysicsMinimizer : public ROOT::Math::IMultiGenFunction {
protected:
	TMDMPhysicsMinimizer(unsigned int ndim, TMDMPhysics* mdm);
	
public:
	virtual unsigned int NDim() const { return m_NDim; }
	virtual ~TMDMPhysicsMinimizer();	

// Need to implement these	
public:
	// Clone the current class
	virtual ROOT::Math::IMultiGenFunction* Clone() const = 0;
	// Initialize input parameters to RAYTRACE
	// Should set the m_Fixed<...> and m_Initial<...> values
	// for each of theta x, theta y, ekin (at target location)
	virtual void Initialize() = 0;
// End to implement
public:
	bool   GetFixedThetaX()   { return m_FixedThetaX   ;}
	bool   GetFixedThetaY()   { return m_FixedThetaY   ;}
	bool   GetFixedEkin()     { return m_FixedEkin     ;}
	double GetInitialThetaX() { return m_InitialThetaX ;}
	double GetInitialThetaY() { return m_InitialThetaY ;}
	double GetInitialEkin()   { return m_InitialEkin   ;}
private:
	// Define the function to minimize
	// Parameters are length n input, where n is the number
	// of free parameters, in the following order:
	// [0] target x
	// [1] target y
	// [2] target ekin
	//   Note this is a _rank ordering_ of non-fixed parameters.
	//   For example if target y is fixed, then the order is:
	//      x[0] = target x
	//      x[1] = ekin
	virtual double DoEval(const double* x) const = 0;
	
protected:
	TMDMPhysics* m_MDM;
	unsigned int m_NDim;
	bool   m_FixedThetaX;
	bool   m_FixedThetaY;
	bool   m_FixedEkin;
	double m_InitialThetaX;
	double m_InitialThetaY;
	double m_InitialEkin;
};

#endif

// Local Variables:
// mode: c++
// End:
