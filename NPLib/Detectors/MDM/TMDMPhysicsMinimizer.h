// ROOT includes
#include <vector>
#include <utility>
#include "Math/Functor.h"


class TMDMPhysics;
typedef std::vector<std::pair<bool, double> > MinParams_t;


class TMDMPhysicsMinimizer : public ROOT::Math::IMultiGenFunction {
protected:
	TMDMPhysicsMinimizer(unsigned int ndim, const TMDMPhysics* mdm);
	virtual unsigned int NDim() const { return m_NDim; }
	
public:
	virtual ~TMDMPhysicsMinimizer();	

// Need to implement these	
public:
	virtual ROOT::Math::IMultiGenFunction* Clone() const = 0;
	virtual void Initialize(MinParams_t& ipar) = 0;
private:
	virtual double DoEval(const double*) = 0;
	
protected:
	const TMDMPhysics* m_MDM;
	unsigned int m_NDim;
};

// Local Variables:
// mode: c++
// End:
