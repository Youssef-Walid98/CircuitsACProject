#include "CurrentSource.h"



CurrentSource::CurrentSource()
{
}


CurrentSource::CurrentSource(int pNode, int nNode, double magnitude, double phase) :Component(pNode, nNode)
{
	this->phase = phase;
	this->magnitude = magnitude;
	this->val = std::complex<double>(magnitude * cos((pi*phase) / 180.0), magnitude * sin((pi*phase) / 180.0));
}


double CurrentSource::getPhase()
{
	return phase;
}

double CurrentSource::getMagnitude()
{
	return magnitude;
}

std::complex<double> CurrentSource::getVal()
{
	return val;
}

CurrentSource::~CurrentSource()
{
}
