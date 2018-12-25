#include "VoltageSource.h"


VoltageSource::VoltageSource(int pNode, int nNode, double magnitude , double phase) :Component(pNode, nNode)
{
	this->phase = phase;
	this->magnitude = magnitude;
	this->val = std::complex<double>(magnitude * cos( (pi*phase)/180.0  ), magnitude * sin( (pi*phase) / 180.0 ));
}

VoltageSource::VoltageSource()
{
}


double VoltageSource::getPhase()
{
	return phase;
}

double VoltageSource::getMagnitude()
{
	return magnitude;
}

std::complex<double> VoltageSource::getVal()
{
	return val;
}

VoltageSource::~VoltageSource()
{
}
