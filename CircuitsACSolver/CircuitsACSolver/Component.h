#pragma once
#include <complex>
class Component
{
private:
	int pNode, nNode;
public:
	Component();
	Component(int pNode, int nNode);
	int getpNode();
	int getnNode();
	virtual std::complex<double> getR();
	virtual double getPhase();
	virtual double getMagnitude();
	virtual std::complex<double> getVal();
	virtual std::complex<double> getInvR();
	~Component();
};