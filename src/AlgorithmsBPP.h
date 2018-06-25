#ifndef ALGORTIHMSBPP_H
#define ALGORTIHMSBPP_H

#include <string>
#include <deque>
#include <iostream>
#include <algorithm>

#include "Element.h"
#include "Algorithm.h"
#include "Correction.h"

using namespace std;

class AlgorithmsBPP
{
public:
	void setB(int inputB);
	void setC(int inputC);
	void setDebug(bool inputDebug);
	void setPrintLvl(int lvl);
	void setCorrectionItertionCount(int count);
	void setP(int value);
	void setQ(int value);

	bool getDebug();
	int getB();
	int getC();
	int getPrintLvl();
	int getCorrectionItertionCount();
	int getP();
	int getQ();

	deque<deque<Element>> BFD(deque<Element> input);
	deque<deque<Element>> BFDB(deque<Element> input);
	deque<deque<Element>> BinFFDB(deque<Element> input);
	deque<deque<Element>> BinBFIB(deque<Element> input);
	deque<deque<Element>> BinFFSLB(deque<Element> input, bool versionPrim1, bool versionPrim2, Correction withCorrection);
	deque<deque<Element>> BinFFAWB(deque<Element> input, bool withBinFFSLsort, Correction withCorrection);
	deque<deque<Element>> BinFFAWP2B(deque<Element> input, Correction withCorrection);
	deque<deque<Element>> correct(Correction type, deque<deque<Element>> input, Algorithm alg);

private:
	int paramB = 0;
	int paramC = 0;
	int correctionIterationCount = 0;
	int correctionP = 0;
	int correctionQ = 0;
	int sizeLp = 0;
	int sizeLn = 0;
	int startSize = 0;
	int endSize = 0;
	int sumElementsOfBin = 0;
	int printLvl = 0;

	bool alternativeVersion = false;
	bool alternativeSort = false;
	bool debug = true;

	deque<deque<Element>> solution;
	deque<Element> L;

	void returnFragmentToDeque(Element element, deque<Element>* input, bool sorted, bool decrasing);
	void returnFragmentToDequeBinFFAWB(Element element);
	void returnFragmentToDequeBinFFSLB(Element element);

	deque<deque<Element>> correctFunction(deque<deque<Element>> input, Algorithm function, int baseProbabilty, int bonus);

	void printSolution(deque<deque<Element>> solution);
	void printBin(deque<Element> solution);
	void printInfoAboutAddElement(int element, int firstPartOfElement);

	int getOptimalSolutionSize(deque<deque<Element>> input);
	void calculateSizeOfListOfElements();
	void calculateSizeOfSolution();

	bool packElementv1(Element element, int elementNumber, deque<Element>* binSolution, Algorithm function);
	bool packElementv2(Element element, int *elementNumber, deque<Element>* binSolution, Algorithm function);

	bool checkFirstCondition();
	bool checkSecondCondition();
	bool checkThirdCondition();
	void prepareBinFFSLB(deque<Element> *input, bool withAlternativeVersion, bool withAlternativeSort);
	deque<Element> prepareListBinFFSLB(deque<Element> *input);

	void prepareBinFFAWP2B(deque<Element> *input);

	void prepareBinFFAWB(deque<Element> *input, bool withFFSLsort);

	void prepareBinBFIB(deque<Element> *input);

	static bool compareElement(Element i, Element j);
	bool compareBinFFAWB(Element i, Element j);

	struct Compare {
		Compare(AlgorithmsBPP *c) 
		{
			engine = c;
		}
		bool operator () (const Element & i1, const Element & i2) 
		{
			return engine->compareBinFFAWB(i1, i2);
		} 
		AlgorithmsBPP* engine;
	};
};
#endif

