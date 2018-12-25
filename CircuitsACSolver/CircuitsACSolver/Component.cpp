#include "Component.h"

Component::Component()
{
}

Component::Component(int pNode, int nNode)
{
	this->pNode = pNode;
	this->nNode = nNode;
}



int Component::getpNode()
{
	return pNode;
}

int Component::getnNode()
{
	return nNode;
}

std::complex<double> Component::getR()
{
	return std::complex<double>();
}

double Component::getPhase()
{
	return 0.0;
}

double Component::getMagnitude()
{
	return 0.0;
}

std::complex<double> Component::getVal()
{
	return std::complex<double>();
}

std::complex<double> Component::getInvR()
{
	return std::complex<double>();
}


Component::~Component()
{
}
