#ifndef ANNULAR_CSI_UTILS_HEADER
#define ANNULAR_CSI_UTILS_HEADER

// STL
#include <vector>
// NPL
namespace NPL { class InputParser; }

namespace AnnularCsI_Utils {

/// Geometry of an annular CsI wedge
struct Geometry{
	double R_min;
	double R_max;
	double R_delta;
	double Phi_min;
	double Phi_max;
	double Phi_delta;
	double Z;
};

std::vector<Geometry> ReadConfiguration(NPL::InputParser& parser);

}

#endif
/* Local Variables:  */
/* mode: c++         */
/* End:              */
