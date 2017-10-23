#ifndef HAVE_MDM_PHYS_MIN
#define HAVE_MDM_PHYS_MIN

// ROOT includes
#include <vector>
#include <utility>
#include "Math/Functor.h"


class TMDMPhysics;
typedef std::vector<std::pair<bool, double> > MinParams_t;


class TMDMPhysicsMinimizer : public ROOT::Math::IMultiGenFunction {
protected:
	TMDMPhysicsMinimizer(unsigned int ndim, TMDMPhysics* mdm);
	virtual unsigned int NDim() const { return m_NDim; }
	
public:
	virtual ~TMDMPhysicsMinimizer();	

// Need to implement these	
public:
	virtual ROOT::Math::IMultiGenFunction* Clone() const = 0;
	virtual void Initialize(MinParams_t& ipar) = 0;
private:
	virtual double DoEval(const double*) const = 0;
	
protected:
	TMDMPhysics* m_MDM;
	unsigned int m_NDim;
};

#endif

// Local Variables:
// mode: c++
// End:
