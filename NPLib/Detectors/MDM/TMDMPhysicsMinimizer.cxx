#include "TMDMPhysicsMinimizer.h"


TMDMPhysicsMinimizer::TMDMPhysicsMinimizer(unsigned int ndim, 
																					 TMDMPhysics* mdm):
	m_NDim(ndim), 
	m_MDM(mdm),
	m_FixedThetaX(true),
	m_FixedThetaY(true),
	m_FixedEkin(true),
	m_InitialThetaX(0.),
	m_InitialThetaY(0.),
	m_InitialEkin(0.){
}

TMDMPhysicsMinimizer::~TMDMPhysicsMinimizer(){
}


