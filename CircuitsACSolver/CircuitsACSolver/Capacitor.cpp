#include "Capacitor.h"



Capacitor::Capacitor()
{
}

Capacitor::Capacitor(int node1, int node2, double capacitance, double w):Component(node1,node2)
{
	this->capacitance = capacitance;
	this->XC = std::complex<double>(0,(-1)*(1 / (w * capacitance)));
}

std::complex<double> Capacitor::getR()
{
	return XC;
}

std::complex<double> Capacitor::getInvR()
{
	return (std::complex<double>(1, 0) / XC);
}

Capacitor::~Capacitor()
{
}
