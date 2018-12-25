#include <iostream>
#include <armadillo>
#include <complex>
#include <string>
#include <vector>
#include "CurrentSource.h"
#include "VoltageSource.h"
#include "Resistor.h"
#include "Inductor.h"
#include "Capacitor.h"

using namespace std;
using namespace arma;

double w = 1;
double pi = 3.14159265359;

void getNetlist();
void constructNodeList();
void fillNoSource(cx_dmat &iMat, cx_dmat &sMat);
void fillGroundedSource(cx_dmat &iMat, cx_dmat &sMat);
void fillUngroundedSource(cx_dmat &iMat, cx_dmat &sMat);
void constructCurrentsList(cx_dmat &iMat, cx_dmat &sMat, cx_dmat &voltMat);

int noNodes=0;
vector<Component*> componentsList;
vector< vector<Component*> > nodeList(100);
int nodeType[100]; //// 0 -> no source //// 1 -> grounded source //// 2-> flying source
complex<double> branchCurrents[100][100];

int main() 
{
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
			branchCurrents[i][j] = complex<double>(-1, 0);
	getNetlist();
	constructNodeList();
	cx_dmat sMat (noNodes-1, noNodes-1, fill::zeros);
	cx_dmat iMat (noNodes-1, 1, fill::zeros);

	fillNoSource(iMat,sMat);
	fillGroundedSource(iMat, sMat);
	fillUngroundedSource(iMat, sMat);
	//iMat.print();
	//sMat.print();
	
	cx_dmat nodeVoltageMat = solve(sMat, iMat);

	constructCurrentsList(iMat, sMat, nodeVoltageMat);

	for (int i = 1; i < noNodes; i++)
		cout << "V(" << i << ") = " << abs(nodeVoltageMat(i - 1, 0)) << "<" << 180*arg(nodeVoltageMat(i - 1, 0))/pi << endl;


	for (int i = 0; i<100; i++)
		for (int j = 0; j < 100; j++)
		{
			if (branchCurrents[i][j] != complex<double>(-1, 0)) {
				cout << "I(" << i << "," << j << ") = " << branchCurrents[i][j] << endl;
			}
		}

	//nodeVoltageMat.print();
	system("pause");
}

void getNetlist()
{
	int no; //dummy number for number of inputs >> to be removed later <<
	cin >> no;

	for (int i = 0; i < no; i++) {
		string type;
		cin >> type;
		int pNode, nNode; double val;
		cin >> pNode >> nNode >> val;
		noNodes = max(max(pNode, nNode),noNodes);

		if (type[0] == 'I') {
			double phase;
			cin >> phase;
			Component* comp = new CurrentSource(pNode, nNode, val, phase);
			componentsList.push_back(comp);
		}
		else if (type[0] == 'V') {
			double phase;
			cin >> phase;
			Component* comp = new VoltageSource(pNode, nNode, val, phase);
			componentsList.push_back(comp);
		}
		else if (type[0] == 'R')
		{
			Component* comp = new Resistor(pNode, nNode, val);
			componentsList.push_back(comp);
		}
		else if (type[0] == 'C')
		{
			Component* comp = new Capacitor(pNode, nNode, val,w);
			componentsList.push_back(comp);
		}
		else if (type[0] == 'L')
		{
			Component* comp = new Inductor(pNode, nNode, val,w);
			componentsList.push_back(comp);
		}
	}

	noNodes++;

}

void constructNodeList()
{
	for (int i = 0; i < componentsList.size(); i++) {
		if (dynamic_cast<VoltageSource*>(componentsList[i]) != NULL) {
			if (min(componentsList[i]->getpNode(), componentsList[i]->getnNode()) == 0) // if the smallest node is 0 (source is grounded)
			{
				nodeType[componentsList[i]->getpNode()] = 1;
				nodeType[componentsList[i]->getnNode()] = 1;
			}
			else {
				nodeType[componentsList[i]->getpNode()] = 2;
				nodeType[componentsList[i]->getnNode()] = 2;
			}
		}
			nodeList[componentsList[i]->getpNode()].push_back(componentsList[i]); //pushes the component in both nodes
			nodeList[componentsList[i]->getnNode()].push_back(componentsList[i]);
	}

}

void fillNoSource( cx_dmat& iMat, cx_dmat &sMat)
{
	for (int i = 1; i < noNodes; i++) {
		if (nodeType[i] == 0)
		{
			for (int j = 0; j < nodeList[i].size(); j++) {
				if (dynamic_cast<CurrentSource*>(nodeList[i][j]) != NULL)
				{
					if (nodeList[i][j]->getpNode() == i)
						iMat(i-1, 0) += nodeList[i][j]->getVal();
					else if (nodeList[i][j]->getnNode() == i)
						iMat(i-1, 0) -= nodeList[i][j]->getVal();
				}
				else {
					int otherNode;
					if (nodeList[i][j]->getpNode() == i)
						otherNode = nodeList[i][j]->getnNode();
					else
						otherNode = nodeList[i][j]->getpNode();
					if (otherNode != 0) {
						sMat(i - 1, otherNode - 1) -= nodeList[i][j]->getInvR();
						sMat(i - 1, i - 1) += nodeList[i][j]->getInvR();
					}
					else if (otherNode == 0) {
						sMat(i - 1, i - 1) += nodeList[i][j]->getInvR();
					}
				}
			}
		}
	}
}

void fillGroundedSource(cx_dmat & iMat, cx_dmat & sMat)
{
	for (int i = 1; i < noNodes; i++) {
		if (nodeType[i] == 1)
		{
			for (int j = 0; j < nodeList[i].size(); j++) {
				if (dynamic_cast<VoltageSource*>(nodeList[i][j]) != NULL)
				{
					sMat(i-1, i-1) += complex<double>(1, 0);
					iMat(i-1, 0) += nodeList[i][j]->getVal();
				}
			}
		}
	}
}

void fillUngroundedSource(cx_dmat& iMat, cx_dmat& sMat)
{
	for (int i = 1; i < noNodes; i++) {
		if (nodeType[i] == 2)
		{
			for (int j = 0; j < nodeList[i].size(); j++) {
				if (dynamic_cast<VoltageSource*>(nodeList[i][j]) != NULL)
				{
					int PosNode = nodeList[i][j]->getpNode();
					int NegNode = nodeList[i][j]->getnNode();
					sMat(max(PosNode, NegNode) - 1, PosNode - 1) += complex<double>(1, 0);
					sMat(max(PosNode, NegNode) - 1, NegNode - 1) -= complex<double>(1, 0);
					iMat(max(PosNode, NegNode) - 1, 0) += nodeList[i][j]->getVal();

					int row = min(PosNode, NegNode);

					for (int k = 0; k < nodeList[i].size(); k++) {
						if (dynamic_cast<VoltageSource*>(nodeList[i][j]) == NULL)
						{
							int ppNode = nodeList[i][j]->getpNode(); int nnNode = nodeList[i][j]->getnNode();
							if (ppNode == PosNode && nnNode == NegNode || ppNode == NegNode && nnNode == PosNode) 
								continue;
							
							if (ppNode == PosNode) {
								sMat(row - 1, nnNode - 1) -= nodeList[i][k]->getInvR();
								sMat(row - 1, PosNode - 1) += nodeList[i][k]->getInvR();
							}
							else if (nnNode == PosNode) {
								sMat(row - 1, ppNode - 1) -= nodeList[i][k]->getInvR();
								sMat(row - 1, PosNode - 1) += nodeList[i][k]->getInvR();
							}

							if (ppNode == NegNode) {
								sMat(row - 1, nnNode - 1) -= nodeList[i][k]->getInvR();
								sMat(row - 1, NegNode - 1) += nodeList[i][k]->getInvR();
							}
							else if (nnNode == NegNode) {
								sMat(row - 1, ppNode - 1) -= nodeList[i][k]->getInvR();
								sMat(row - 1, NegNode - 1) += nodeList[i][k]->getInvR();
							}
						}
					}

				}
			}
		}
	}
}

void constructCurrentsList(cx_dmat& iMat, cx_dmat& sMat, cx_dmat& voltMat)
{
	for (int i = 1; i < noNodes; i++) {
		for (int j = 0; j < nodeList[i].size(); j++) {
			if (dynamic_cast<VoltageSource*>(nodeList[i][j]) == NULL) {
				if (dynamic_cast<CurrentSource*>(nodeList[i][j]) != NULL) {
					branchCurrents[nodeList[i][j]->getpNode()][nodeList[i][j]->getnNode()] = nodeList[i][j]->getVal(); //sets the value to the bigger node
				}
				else {
					int PosNode = nodeList[i][j]->getpNode(); int NegNode = nodeList[i][j]->getnNode();
					complex<double> smallervolt;
					complex<double> biggervolt;
					if (PosNode == 0)
					{
						biggervolt = voltMat(NegNode - 1, 0);
						smallervolt = 0;
					}
					else if (NegNode == 0)
					{
						smallervolt = 0;
						biggervolt = voltMat(PosNode - 1, 0);
					}
					else if (abs(voltMat(PosNode - 1, 0)) > abs(voltMat(NegNode - 1, 0)))
					{
						biggervolt = voltMat(PosNode - 1, 0);
						smallervolt = voltMat(NegNode - 1, 0);
					}
					else {
						biggervolt = voltMat(NegNode - 1, 0);
						smallervolt = voltMat(PosNode - 1, 0);
					}

					complex<double> dV = biggervolt - smallervolt;
					if (PosNode != 0 && NegNode != 0) {
						if (abs(voltMat(PosNode - 1, 0)) > abs(voltMat(NegNode - 1, 0)))
						{
							branchCurrents[PosNode][NegNode] = (dV / (nodeList[i][j]->getR()));
						}
						else {
							branchCurrents[NegNode][PosNode] = (dV / (nodeList[i][j]->getR()));
						}
					}
					else {
						if (PosNode == 0)
						{
							branchCurrents[NegNode][PosNode] = (dV / nodeList[i][j]->getR());
						}else
							branchCurrents[PosNode][NegNode] = (dV / nodeList[i][j]->getR());
					}
				}
			}
		}
	}

	for (int i = 1; i < noNodes; i++)
	{
		if (nodeType[i] == 1)
		{
			complex<double> sum(0, 0);
			for (int j = 0; j < 100; j++)
				if(branchCurrents[i][j] != complex<double>(-1,0) )
					sum += branchCurrents[i][j];
			for (int j = 0; j < 100; j++)
				if (branchCurrents[j][i] != complex<double>(-1, 0))
					sum -= branchCurrents[j][i];
			branchCurrents[i][0] = sum;
		}
	}
}