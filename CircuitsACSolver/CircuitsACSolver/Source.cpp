#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <armadillo>
#include <complex>
using namespace std;
using namespace arma;

void getNetlist();
void constructNodeMat();
void fillMatNoVoltages(Mat<cx_double> &A);

struct Component {
	string name;
	int node1, node2;
	double val;
};

struct nodeMat {
	string name;
	double val;
	int otherNode;
};

vector<nodeMat> Nodes[100];
int typeOfNode[100];


Component compArr[100];
double w = 100; // angular velocity of the source
int no; // number of components in netlist
int NodesNo=0;

int main() 
{
	getNetlist(); //gets the netlist from the user
	constructNodeMat(); //generates the node matrix from the netlist
	
	Mat<cx_double> susceptanceMat(NodesNo, NodesNo,fill::zeros);
	fillMatNoVoltages(susceptanceMat);
	


}


void getNetlist() {
	cin >> no;
	for (int i = 0; i < no; i++) {
		cin >> compArr[i].name >> compArr[i].node1 >> compArr[i].node2 >> compArr[i].val;
		NodesNo = max(NodesNo, max(compArr[i].node1, compArr[i].node2));
	}
}

void constructNodeMat() {
	for (int i = 1; i <= no; i++) {
		if (compArr[i].node1 == i) { 
			nodeMat temp; // temporary node
			temp.name = compArr[i].name; temp.val = compArr[i].val; temp.otherNode = compArr[i].node2;
			if (temp.name[0] == 'V' && temp.otherNode == 0 && typeOfNode[i] != 2) typeOfNode[i] = 1; // if the node contains a grounded voltage source, set vSource to 1
			if (temp.name[0] == 'V' && temp.otherNode != 0) typeOfNode[i] = 2; // if the node contains a flying voltage source, set vSource to 2
			Nodes[i].push_back(temp);
		}

		if (compArr[i].node2 == i) {
			nodeMat temp;
			temp.name = compArr[i].name; temp.val = compArr[i].val; temp.otherNode = compArr[i].node1;
			if (temp.name[0] == 'V' && temp.otherNode == 0 && typeOfNode[i] != 2) typeOfNode[i] = 1;; // if the node contains a grounded voltage source, set vSource to 1
			if (temp.name[0] == 'V' && temp.otherNode != 0) typeOfNode[i] = 2; // if the node contains a flying voltage source, set vSource to 2

			if ((temp.name)[0] == 'I' || (temp.name)[0] == 'V') { // puts the value of sources -ve since it's negative polarity
				temp.val *= -1;
			}
			Nodes[i].push_back(temp);
		}
	}
}

void fillMatNoVoltages(Mat<cx_double> &A) {
	for (int i = 1; i <= NodesNo; i++) {
		if (typeOfNode[i] != 0)
			continue;
		for (int j = 0; j < Nodes[i].size() ; j++) {
			if (Nodes[i][j].name[0] != 'I') { // if it's not a current source
				A(i,Nodes[i][j].otherNode) -= Nodes[i][j].val;
				A(i,i) += Nodes[i][j].val;
			}
		}
	}

}