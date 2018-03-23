#ifndef ANNULAR_TELESCOPE_UTILS_HEADER
#define ANNULAR_TELESCOPE_UTILS_HEADER

// STL
#include <vector>
// NPL
namespace NPL { class InputParser; }

namespace AnnularTelescope_Utils {

/// Geometry of an annular telescope
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
