#include "AlgorithmsBPP.h"

void AlgorithmsBPP::setB(int inputB)
{
	paramB = inputB;
}
void AlgorithmsBPP::setC(int inputC)
{
	paramC = inputC;
}
int AlgorithmsBPP::getB()
{
	return paramB;
}
int AlgorithmsBPP::getC()
{
	return paramC;
}
void AlgorithmsBPP::setDebug(bool inputDebug)
{
	debug = inputDebug;
}
bool AlgorithmsBPP::getDebug()
{
	return debug;
}
int AlgorithmsBPP::getPrintLvl()
{
	return printLvl;
}
void AlgorithmsBPP::setPrintLvl(int lvl)
{
	printLvl = lvl;
}
int AlgorithmsBPP::getCorrectionItertionCount()
{
	return correctionIterationCount;
}
void AlgorithmsBPP::setCorrectionItertionCount(int count)
{
	correctionIterationCount = count;
}
int AlgorithmsBPP::getP()
{
	return correctionP/10;
}
void AlgorithmsBPP::setP(int value)
{
	correctionP = value*10;
}
int AlgorithmsBPP::getQ()
{
	return correctionQ/10;
}
void AlgorithmsBPP::setQ(int value)
{
	correctionQ = value*10;
}

bool AlgorithmsBPP::compareElement(Element i, Element j)
{
	return i.getValue() > j.getValue();
}

bool AlgorithmsBPP::compareBinFFAWB(Element i, Element j)
{
	if (i.getValue() < paramB || j.getValue() <paramB)
	{
		return i.getValue() > j.getValue();
	}
	if (paramB >= paramC / 5 && paramB <= paramC / 3 && ((i.getValue() > paramC - paramB && i.getValue() <= paramC) ||
		(j.getValue() > paramC - paramB && j.getValue() <= paramC)))
	{
		return i.getValue() < j.getValue();
	}

	if (paramB == 0)
		return i.getValue() > j.getValue();

	double tmpI = (double)i.getValue() / (int)(i.getValue() / paramB);
	double tmpJ = (double)j.getValue() / (int)(j.getValue() / paramB);
	if (tmpI != tmpJ)
		return tmpI > tmpJ;
	else
		return i.getValue() < j.getValue();
}

int AlgorithmsBPP::getOptimalSolutionSize(deque<deque<Element>> solution)
{
	int size = 0;
	for (unsigned int i = 0; i < solution.size(); ++i)
	{
		for (unsigned int j = 0; j < solution[i].size(); j++)
			size += solution[i][j].getValue();
	}

	int optimal = size / paramC;
	if (size % paramC != 0)
		++optimal;

	return optimal;
}

void AlgorithmsBPP::printSolution(deque<deque<Element>> solution)
{
	if (printLvl < 1)
		return;

	for (unsigned int i = 0; i < solution.size(); ++i)
	{
		int sum = 0;
		for (unsigned int j = 0; j < solution.at(i).size(); ++j)
		{
			cout << solution.at(i).at(j).getValue() << " ";
			sum += solution.at(i).at(j).getValue();
		}
		cout << " - " << sum << endl;
	}
	cout << endl;
}

void AlgorithmsBPP::printBin(deque<Element> solution)
{
	if (printLvl < 1)
		return;
	int sum = 0;
	for (unsigned int i = 0; i < solution.size(); ++i)
	{
		cout << solution.at(i).getValue() << " ";
		sum += solution.at(i).getValue();
	}
	cout << " - " << sum << endl;
}

deque<deque<Element>> AlgorithmsBPP::correct(Correction type, deque<deque<Element>> input, Algorithm function)
{
	if (type == Correction::NO)
		return input;


	int optimalSolutionSize = getOptimalSolutionSize(input);
	if (input.size() == optimalSolutionSize)
		return input;

	deque<deque<Element>> bestSolution = input;
	deque<deque<Element>> result;

	bool firstTry = true;

	for (int i = 0; i < correctionIterationCount || type == Correction::BASE; ++i)
	{
		if (firstTry)
		{
			result = correctFunction(bestSolution, function, 0, 0);
			firstTry = false;
		}
		else
			result = correctFunction(bestSolution, function, correctionP, i * correctionQ);

		if (result.size() < bestSolution.size())
		{
			bestSolution = result;
			i = 0;
			if (bestSolution.size() == optimalSolutionSize)
				return bestSolution;
		}

		if (type == Correction::BASE)
			break;
	}
	return bestSolution;
}

deque<deque<Element>> AlgorithmsBPP::correctFunction(deque<deque<Element>> input, Algorithm function, int baseProbabilty, int bonus)
{
	int oldSize = input.size();

	deque<Element> newInput;
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		int size = 0;
		for (unsigned int j = 0; j < input.at(i).size(); ++j)
		{
			size += input.at(i).at(j).getValue();
		}
		int probability;
		if (baseProbabilty == 0 && bonus == 0)
			probability = (std::rand() % 1000);
		else
			probability = 0;

		if (size != paramC && (i != input.size() - 1 || i == input.size() - 1 && newInput.size() != 0))
		{
			for (unsigned int j = 0; j < input.at(i).size(); ++j)
				newInput.push_back(input.at(i).at(j));

			input.erase(input.begin() + i);
			--i;
		}
		else if (probability < baseProbabilty + bonus)
		{
			for (unsigned int j = 0; j < input.at(i).size(); ++j)
				newInput.push_back(input.at(i).at(j));

			input.erase(input.begin() + i);
			--i;
		}
	}

	if (newInput.size() == 0)
		return input;

	sort(newInput.begin(), newInput.end(), compareElement);

	AlgorithmsBPP engine;

	engine.setB(paramB);
	engine.setC(paramC);

	if (function == Algorithm::BinFFSLB)
	{
		deque<deque<Element>> result = engine.BinFFSLB(newInput, alternativeVersion, alternativeSort, Correction::NO);
		input.insert(input.end(), result.begin(), result.end());
		return input;
	}
	else if (function == Algorithm::BinFFAWP2B)
	{
		deque<deque<Element>> result = engine.BinFFAWB(newInput, false, Correction::NO);
		input.insert(input.end(), result.begin(), result.end());
		return input;
	}
	else if (function == Algorithm::BinFFAWB)
	{
		deque<deque<Element>> result = engine.BinFFAWB(newInput, false, Correction::NO);
		input.insert(input.end(), result.begin(), result.end());
		return input;
	}
	else if (function == Algorithm::BinBFIB)
	{
		deque<deque<Element>> result = engine.BinBFIB(newInput);
		input.insert(input.end(), result.begin(), result.end());
		return input;
	}
	else
		return input;
}

void AlgorithmsBPP::calculateSizeOfListOfElements()
{
	startSize = 0;
	for (unsigned int i = 0; i < L.size(); ++i)
		startSize += L[i].getValue();
}

void AlgorithmsBPP::calculateSizeOfSolution()
{
	endSize = 0;
	for (unsigned int i = 0; i < solution.size(); ++i)
	{
		int binSize = 0;
		for (unsigned int j = 0; j < solution[i].size(); j++)
		{
			endSize += solution[i][j].getValue();
			binSize += solution[i][j].getValue();
		}
		if (debug && binSize > paramC)
		{
			cout << "ERROR in calculateSizeOfSolution, binSize > paramC" << endl;
			exit(-1);
		}
	}
}

deque<Element> AlgorithmsBPP::prepareListBinFFSLB(deque<Element> *input)
{
	sizeLp = 0;
	sizeLn = 0;
	sort(input->begin(), input->end(), compareElement);
	int maxElement = input->front().getValue();
	deque<Element> result;
	while (!input->empty() && input->back().getValue() < 2 * paramB)
	{
		result.push_front(input->back());
		input->pop_back();
		++sizeLn;
	}

	if (maxElement / 3 >= paramB || !alternativeSort)
	{
		while (!input->empty())
		{
			result.push_back(input->back());
			input->pop_back();
			++sizeLp;
		}
	}
	else
	{
		while (!input->empty())
		{
			result.push_back(input->front());
			input->pop_front();
			++sizeLp;
		}
	}
	return result;
}

void AlgorithmsBPP::printInfoAboutAddElement(int element, int firstPartOfElement)
{
	if (printLvl < 2)
		return;

	if (firstPartOfElement == 0)
		cout << "Dodano element o rozmiarze " << element << endl;
	else
	{
		cout << "Wziêto element o rozmiarze " << element;
		cout << " Dodano fragment o rozmiarze " << firstPartOfElement;
		cout << " Zwrocono fragment o rozmiarze " << element - firstPartOfElement << endl;
	}
}

bool AlgorithmsBPP::packElementv1(Element element, int elementNumber, deque<Element>* binSolution, Algorithm function)
{
	int freeSpaceInBin = paramC - sumElementsOfBin;

	if (freeSpaceInBin >= element.getValue())
	{
		printInfoAboutAddElement(element.getValue(), 0);

		binSolution->push_back(element);
		sumElementsOfBin += element.getValue();

		if (element.getValue() >= 2 * paramB)
			--sizeLp;
		else
			--sizeLn;
		L.erase(L.begin() + elementNumber);
		return true;
	}
	else if (freeSpaceInBin < element.getValue() && freeSpaceInBin != 0 && paramB <= freeSpaceInBin  && element.getValue() >= 2 * paramB)
	{
		Element firstPartOfElement(element.getIndex(), min(element.getValue() - paramB, paramC - sumElementsOfBin));

		if (debug && firstPartOfElement.getValue() < paramB)
		{
			cout << "ERROR in packElementv1, frstPartOfElement.getValue() < paramB" << endl;
			exit(-1);
		}

		printInfoAboutAddElement(element.getValue(), firstPartOfElement.getValue());

		sumElementsOfBin += firstPartOfElement.getValue();
		binSolution->push_back(firstPartOfElement);
		L.erase(L.begin() + elementNumber);
		--sizeLp;

		Element secondPartOfElement(element.getIndex(), element.getValue() - firstPartOfElement.getValue());

		switch (function)
		{
		case Algorithm::BinFFSLB:
			returnFragmentToDequeBinFFSLB(secondPartOfElement);
			break;
		case Algorithm::BinFFAWB:
		case Algorithm::BinFFAWP2B:
			returnFragmentToDequeBinFFAWB(secondPartOfElement);
			break;
		case Algorithm::BinFFD:
			returnFragmentToDeque(secondPartOfElement, &L, false, false);
			break;
		case Algorithm::BFD:
			returnFragmentToDeque(secondPartOfElement, &L, true, true);
			break;
		default:
			break;
		}

		if (secondPartOfElement.getValue() >= 2 * paramB)
			++sizeLp;
		else
			++sizeLn;
		return true;
	}
	return false;
}

bool AlgorithmsBPP::packElementv2(Element element, int* elementNumber, deque<Element>* binSolution, Algorithm function)
{
	int freeSpaceInBin = paramC - sumElementsOfBin;
	if (element.getValue() == freeSpaceInBin || element.getValue() <= freeSpaceInBin - paramB)
	{
		printInfoAboutAddElement(element.getValue(), 0);

		binSolution->push_back(element);
		sumElementsOfBin += element.getValue();

		if (element.getValue() >= 2 * paramB)
			--sizeLp;
		else
			--sizeLn;

		L.erase(L.begin() + *elementNumber);

		if (alternativeVersion)
			*elementNumber = 0;

		return true;
	}
	else if (element.getValue() > freeSpaceInBin - paramB && element.getValue() < freeSpaceInBin && element.getValue() >= 2 * paramB)
	{

		Element firstPartOfElement(element.getIndex(), element.getValue() - paramB);

		if (firstPartOfElement.getValue() < paramB)
		{
			cout << "ERROR in packElementv1, frstPartOfElement.getValue() < paramB" << endl;
			exit(-1);
		}

		printInfoAboutAddElement(element.getValue(), firstPartOfElement.getValue());

		sumElementsOfBin += firstPartOfElement.getValue();
		binSolution->push_back(firstPartOfElement);
		L.erase(L.begin() + *elementNumber);
		--sizeLp;
		Element secondPartOfElement(element.getIndex(), element.getValue() - firstPartOfElement.getValue());

		returnFragmentToDequeBinFFSLB(secondPartOfElement);
		if (element.getValue() - firstPartOfElement.getValue() >= 2 * paramB)
			++sizeLp;
		else
			++sizeLn;

		++(*elementNumber);
		return true;
	}
	else if (element.getValue() < freeSpaceInBin + paramB && element.getValue() > freeSpaceInBin && element.getValue() >= 2 * paramB && freeSpaceInBin > 2 * paramB)
	{

		Element firstPartOfElement(element.getIndex(), freeSpaceInBin - paramB);

		if (firstPartOfElement.getValue() < paramB)
		{
			cout << "ERROR in packElementv1, frstPartOfElement.getValue() < paramB" << endl;
			exit(-1);
		}

		printInfoAboutAddElement(element.getValue(), firstPartOfElement.getValue());

		sumElementsOfBin += firstPartOfElement.getValue();
		binSolution->push_back(firstPartOfElement);
		L.erase(L.begin() + *elementNumber);
		--sizeLp;
		Element secondPartOfElement(element.getIndex(), element.getValue() - firstPartOfElement.getValue());

		returnFragmentToDequeBinFFSLB(secondPartOfElement);
		if (element.getValue() - firstPartOfElement.getValue() >= 2 * paramB)
			++sizeLp;
		else
			++sizeLn;

		++(*elementNumber);
		return true;
	}
	else if (element.getValue() >= freeSpaceInBin + paramB && freeSpaceInBin >= paramB && element.getValue() >= 2 * paramB)
	{
		Element firstPartOfElement(element.getIndex(), freeSpaceInBin);
		if (firstPartOfElement.getValue() < paramB)
		{
			cout << "ERROR in packElementv1, frstPartOfElement.getValue() < paramB" << endl;
			exit(-1);
		}

		sumElementsOfBin += firstPartOfElement.getValue();
		binSolution->push_back(firstPartOfElement);
		L.erase(L.begin() + *elementNumber);
		--sizeLp;

		printInfoAboutAddElement(element.getValue(), firstPartOfElement.getValue());

		Element secondPartOfElement(element.getIndex(), element.getValue() - firstPartOfElement.getValue());

		returnFragmentToDequeBinFFSLB(secondPartOfElement);
		if (element.getValue() - firstPartOfElement.getValue() >= 2 * paramB)
			++sizeLp;
		else
			++sizeLn;

		++(*elementNumber);
		return true;
	}
	return false;
}

bool AlgorithmsBPP::checkFirstCondition()
{
	return L.front().getValue() < 2 * paramB && L.front().getValue() > paramC - paramB && L.front().getValue() <= paramC - sumElementsOfBin;
}

bool AlgorithmsBPP::checkSecondCondition()
{
	return sizeLp == 0;
}

bool AlgorithmsBPP::checkThirdCondition()
{
	if (paramC - sumElementsOfBin < 2 * paramB && (sizeLp == 0 || sizeLn == 0))
	{
		if ((sizeLp == 0 && L.front().getValue() < paramC - sumElementsOfBin + paramB && L.back().getValue() > paramC - sumElementsOfBin - paramB) ||
			(sizeLn == 0 && L.front().getValue() > paramC - sumElementsOfBin - paramB && L.back().getValue() < paramC - sumElementsOfBin + paramB))
		{
			return true;
		}
	}
	else if (paramC - sumElementsOfBin < 2 * paramB &&
		L.front().getValue() < paramC - sumElementsOfBin + paramB &&
		L.at(sizeLn - 1).getValue() > paramC - sumElementsOfBin - paramB &&
		L.at(sizeLn).getValue() > paramC - sumElementsOfBin - paramB &&
		L.back().getValue() < paramC - sumElementsOfBin + paramB)
	{
		return true;
	}
	return false;
}

void AlgorithmsBPP::prepareBinFFSLB(deque<Element> *input, bool withAlternativeVersion, bool withAlternativeSort)
{
	solution.clear();
	alternativeSort = withAlternativeSort;
	alternativeVersion = withAlternativeVersion;
	if (printLvl > 0)
		cout << endl;
	L = prepareListBinFFSLB(input);
	if (debug)
		calculateSizeOfListOfElements();
}

deque<deque<Element>> AlgorithmsBPP::BinFFSLB(deque<Element> input, bool versionPrim1, bool versionPrim2, Correction withCorrection)
{
	prepareBinFFSLB(&input, versionPrim1, versionPrim2);
	for (int bin = 0; !L.empty(); ++bin)
	{
		deque<Element> binSolution;
		sumElementsOfBin = 0;
		int elementNumber = 0;
		bool firstCondtion = false, secondCondtion = false, thirdCondition = false;
		while (sumElementsOfBin < paramC  && elementNumber < L.size())
		{
			Element element = L.at(elementNumber);

			firstCondtion = checkFirstCondition();
			secondCondtion = checkSecondCondition();
			thirdCondition = checkThirdCondition();

			if (firstCondtion)
			{
				packElementv1(element, elementNumber, &binSolution, Algorithm::BinFFSLB);
				break;
			}
			if (secondCondtion || thirdCondition)
				break;

			if (!packElementv2(element, &elementNumber, &binSolution, Algorithm::BinFFSLB))
				++elementNumber;
		}


		while (sumElementsOfBin < paramC && !L.empty())
		{
			int bestElemenet = 0;
			int bestFreeSpace = paramC;
			for (unsigned int i = 0; i < L.size(); ++i)
			{
				if (paramC - sumElementsOfBin >= L.at(i).getValue())
				{
					if ((paramC - sumElementsOfBin - L.at(i).getValue() < bestFreeSpace))
					{
						bestElemenet = i;
						bestFreeSpace = paramC - sumElementsOfBin - L.at(i).getValue();
					}
				}
				else if (paramC - sumElementsOfBin < L.at(i).getValue() && paramB <= paramC - sumElementsOfBin  && L.at(i).getValue() >= 2 * paramB)
				{
					if ((paramC - sumElementsOfBin - min(L.at(i).getValue() - paramB, paramC - sumElementsOfBin) < bestFreeSpace))
					{
						bestElemenet = i;
						bestFreeSpace = paramC - sumElementsOfBin - min(L.at(i).getValue() - paramB, paramC - sumElementsOfBin);
					}
				}

				if (bestFreeSpace == 0)
					break;
			}
			elementNumber = bestElemenet;
			Element element = L.at(elementNumber);

			if (!packElementv1(element, elementNumber, &binSolution, Algorithm::BinFFSLB))
				break;
		}

		printBin(binSolution);

		solution.push_back(binSolution);
	}

	if (debug)
		calculateSizeOfSolution();

	if (debug && startSize != endSize)
	{
		cout << "ERROR in BinFFSL, tartSize != endSize" << endl;
		exit(-1);
	}

	deque<deque<Element>> result;

	result = correct(withCorrection, solution, Algorithm::BinFFSLB);

	if (result.size() < solution.size() && result.size() != 0)
		return result;
	else
		return solution;
}

void AlgorithmsBPP::prepareBinFFAWP2B(deque<Element> *input)
{
	solution.clear();
	alternativeVersion = false;
	alternativeSort = false;
	if (printLvl > 0)
		cout << endl;
	std::sort(input->begin(), input->end(), Compare(this));
	L = *input;
	if (debug)
		calculateSizeOfListOfElements();
}

deque<deque<Element>> AlgorithmsBPP::BinFFAWP2B(deque<Element> input, Correction withCorrection)
{
	prepareBinFFAWP2B(&input);
	for (int bin = 0; !L.empty(); ++bin)
	{
		deque<Element> binSolution;
		sumElementsOfBin = 0;
		int elementNumber = 0;
		bool firstCondtion = false, secondCondtion = false, thirdCondition = false;
		while (sumElementsOfBin < paramC  && elementNumber < L.size())
		{
			Element element = L.at(elementNumber);

			firstCondtion = element.getValue() != 0 && element.getValue() > paramC - paramB && element.getValue() <= paramC - sumElementsOfBin;

			secondCondtion = checkSecondCondition();

			thirdCondition = true;
			for (unsigned int i = 0; i < L.size(); ++i)
			if (L.at(i).getValue() <= paramC - sumElementsOfBin - paramB || L.at(i).getValue() >= paramC - sumElementsOfBin + paramB)
				thirdCondition = false;

			thirdCondition = thirdCondition && paramC - sumElementsOfBin < 2 * paramB;

			if (firstCondtion)
			{
				if (paramB < paramC / 3 && alternativeVersion)
					break;

				packElementv1(element, elementNumber, &binSolution, Algorithm::BinFFAWP2B);
				break;
			}
			if (secondCondtion || thirdCondition)
				break;

			if (!packElementv2(element, &elementNumber, &binSolution, Algorithm::BinFFAWP2B))
				++elementNumber;
		}

		while (sumElementsOfBin < paramC && !L.empty())
		{
			int bestElemenet = 0;
			int bestFreeSpace = paramC;
			for (unsigned int i = 0; i < L.size(); ++i)
			{
				if (paramC - sumElementsOfBin >= L.at(i).getValue())
				{
					if ((paramC - sumElementsOfBin - L.at(i).getValue() < bestFreeSpace))
					{
						bestElemenet = i;
						bestFreeSpace = paramC - sumElementsOfBin - L.at(i).getValue();
					}
				}
				else if (paramC - sumElementsOfBin < L.at(i).getValue() && paramB <= paramC - sumElementsOfBin  && L.at(i).getValue() >= 2 * paramB)
				{
					if ((paramC - sumElementsOfBin - min(L.at(i).getValue() - paramB, paramC - sumElementsOfBin) < bestFreeSpace))
					{
						bestElemenet = i;
						bestFreeSpace = paramC - sumElementsOfBin - min(L.at(i).getValue() - paramB, paramC - sumElementsOfBin);
					}
				}

				if (bestFreeSpace == 0)
					break;
			}
			elementNumber = bestElemenet;
			Element element = L.at(elementNumber);

			if (!packElementv1(element, elementNumber, &binSolution, Algorithm::BinFFAWP2B))
				break;
		}

		printBin(binSolution);

		solution.push_back(binSolution);
	}
	if (debug)
		calculateSizeOfSolution();

	if (debug && startSize != endSize)
	{
		cout << "ERROR in BinFFAWP2, startSize != endSize" << endl;
		exit(-1);
	}

	deque<deque<Element>> result;

	result = correct(withCorrection, solution, Algorithm::BinFFAWP2B);

	if (result.size() < solution.size() && result.size() != 0)
		return result;
	else
		return solution;
}

void AlgorithmsBPP::returnFragmentToDequeBinFFSLB(Element element)
{
	if (debug && element.getValue() < AlgorithmsBPP::paramB)
	{
		cout << "ERROR in returnFragmentToDequeFFSL, element.getValue() < AlgorithmsBPP::paramB" << endl;
		exit(-1);
	}

	std::deque<Element>::iterator it = L.begin();
	while (it != L.end() && it->getValue() < 2 * paramB)
	{
		if (element.getValue() > it->getValue())
		{
			L.insert(it, element);
			return;
		}
		++it;
	}
	if (paramB <= paramC / 3 || !alternativeSort)
	{
		while (it != L.end())
		{
			if (element.getValue() < it->getValue())
			{
				L.insert(it, element);
				return;
			}
			++it;
		}
		L.push_back(element);
	}
	else
	{
		while (it != L.end() && element.getValue() >= 2 * paramB)
		{
			if (element.getValue() > it->getValue())
			{
				L.insert(it, element);
				return;
			}
			++it;
		}
		L.insert(it, element);
	}
}

void AlgorithmsBPP::prepareBinBFIB(deque<Element> *input)
{
	solution.clear();

	while (!input->empty())
	{
		L.push_back(input->at(input->size() - 1));
		input->pop_back();
	}
	if (debug)
		calculateSizeOfListOfElements();
}

deque<deque<Element>> AlgorithmsBPP::BinBFIB(deque<Element> input)
{
	prepareBinBFIB(&input);

	for (int bin = 0; L.size() > 0; ++bin)
	{
		deque<Element> binSolution;
		int sumElementsOfBin = 0;
		int elementNumber = 0;

		while (sumElementsOfBin < paramC && elementNumber < L.size())
		{
			int potentialFreeSpace = paramC;
			unsigned int element;
			int potentialBestElement;
			for (element = 0; element + 1 < L.size() && L.at(element + 1).getValue() <= paramC - sumElementsOfBin; ++element);
			potentialFreeSpace = paramC - sumElementsOfBin - L.at(element).getValue();
			potentialBestElement = element;
			for (; element + 1 < L.size() && L.at(element).getValue() < paramC - sumElementsOfBin + paramB; ++element);


			Element firstPartOfElement(L.at(element).getIndex(), min(L.at(element).getValue() - paramB, paramC - sumElementsOfBin));

			if ((potentialFreeSpace  < 0 || potentialFreeSpace > paramC - sumElementsOfBin - firstPartOfElement.getValue()) && (paramC - sumElementsOfBin < L.at(element).getValue() && paramB <= paramC - sumElementsOfBin  && L.at(element).getValue() >= 2 * paramB))
			{
				Element oldElement = L.at(element);
				sumElementsOfBin += firstPartOfElement.getValue();
				binSolution.push_back(firstPartOfElement);
				L.erase(L.begin() + element);
				Element secondPartOfElement(oldElement.getIndex(), oldElement.getValue() - firstPartOfElement.getValue());

				returnFragmentToDeque(secondPartOfElement, &L, true, false);
			}
			else if (paramC - sumElementsOfBin >= L.at(potentialBestElement).getValue())
			{
				binSolution.push_back(L.at(potentialBestElement));
				sumElementsOfBin += L.at(potentialBestElement).getValue();
				L.erase(L.begin() + potentialBestElement);
			}
			else
				break;

		}

		solution.push_back(binSolution);
	}

	if (debug)
		calculateSizeOfSolution();

	if (debug && startSize != endSize)
	{
		cout << "ERROR in BinBFI, startSize != endSize" << endl;
		exit(-1);
	}

	return solution;
}

deque<deque<Element>> AlgorithmsBPP::BinFFDB(deque<Element> input)
{
	solution.clear();
	L = input;
	if (debug)
		calculateSizeOfListOfElements();
	if (printLvl > 0)
		cout << endl;

	for (int bin = 0; L.size() > 0; ++bin)
	{
		deque<Element> binSolution;
		sumElementsOfBin = 0;
		unsigned int elementNumber = 0;

		while (sumElementsOfBin < paramC && elementNumber < L.size())
		{
			for (; elementNumber < L.size() && sumElementsOfBin < paramC; ++elementNumber)
			{
				if (packElementv1(L.at(elementNumber), elementNumber, &binSolution, Algorithm::BinFFD))
				{
					break;
				}
			}

		}
		printBin(binSolution);
		solution.push_back(binSolution);
	}

	if (debug)
		calculateSizeOfSolution();

	if (debug && startSize != endSize)
	{
		cout << "ERROR at BinFFD, startSize != endSize" << endl;
		exit(-1);
	}

	return solution;
}

deque<deque<Element>> AlgorithmsBPP::BFD(deque<Element> input)
{
	deque<deque<Element>> solution;
	deque<int> sumsOfBins;
	int result = 0;
	bool added = false;

	while (!input.empty())
	{
		added = false;
		for (unsigned int i = 0; i < solution.size(); ++i)
		{
			if (paramC - sumsOfBins.at(i) >= input.at(0).getValue())
			{
				sumsOfBins[i] += input.at(0).getValue();
				solution[i].push_back(input.at(0));
				input.erase(input.begin());
				added = true;
				break;
			}
		}
		if (!added)
		{
			deque<Element> bin;
			bin.push_front(input.at(0));
			solution.push_back(bin);
			sumsOfBins.push_back(input.at(0).getValue());
			input.erase(input.begin());
		}
	}
	return solution;
}

deque<deque<Element>> AlgorithmsBPP::BFDB(deque<Element> input)
{
	deque<deque<Element>> solution;
	deque<int> sumsOfBins;

	bool added = false;

	while (!input.empty())
	{
		added = false;
		for (unsigned int i = 0; i < solution.size(); ++i)
		{
			if (paramC - sumsOfBins.at(i) >= input.at(0).getValue())
			{
				sumsOfBins[i] += input.at(0).getValue();
				solution[i].push_back(input.at(0));
				input.erase(input.begin());
				added = true;
				break;
			}
			else if (paramC - sumsOfBins.at(i) >= paramB && paramC - sumsOfBins.at(i) != 0 && input.at(0).getValue() >= 2 * paramB)
			{
				Element firstPartOfElement(input.at(0).getIndex(), min(input.at(0).getValue() - paramB, paramC - sumsOfBins[i]));

				sumsOfBins[i] += firstPartOfElement.getValue();
				solution[i].push_back(firstPartOfElement);

				Element secondPartOfElement(input.at(0).getIndex(), input.at(0).getValue() - firstPartOfElement.getValue());

				returnFragmentToDeque(secondPartOfElement, &input, true, true);
				input.erase(input.begin());
				added = true;
				break;
			}
		}
		if (!added)
		{
			deque<Element> bin;
			bin.push_front(input.at(0));
			solution.push_back(bin);
			sumsOfBins.push_back(input.at(0).getValue());
			input.erase(input.begin());
		}
	}

	return solution;

}

void AlgorithmsBPP::prepareBinFFAWB(deque<Element> *input, bool withBinFFSLsort)
{
	solution.clear();

	if (printLvl > 0)
		cout << endl;
	
	if (withBinFFSLsort)
		L = prepareListBinFFSLB(input);
	else
	{
		std::sort(input->begin(), input->end(), Compare(this));
		L = *input;
	}

	calculateSizeOfListOfElements();
}

deque<deque<Element>> AlgorithmsBPP::BinFFAWB(deque<Element> input, bool withBinFFSLsort, Correction withCorrection)
{
	prepareBinFFAWB(&input, withBinFFSLsort);

	int numberOfFirstSmallElement;

	for (int bin = 0; L.size() > 0; ++bin)
	{
		deque<Element> binSolution;
		sumElementsOfBin = 0;
		int elementNumber = 0;

		while (sumElementsOfBin < paramC && elementNumber < L.size())
		{
			Element element = L.at(elementNumber);
			bool firstChoose = true;
			int bestFreeSpace = paramC;
			int bestElement = elementNumber;
			int actualElemenet = elementNumber;

			bool splitedElement = false;
			bool wholeElemenet = false;

			if (!((paramC - sumElementsOfBin >= element.getValue()) || (paramC - sumElementsOfBin < element.getValue() && paramB <= paramC - sumElementsOfBin && element.getValue() >= 2 * paramB)))
			{
				++elementNumber;
				continue;
			}
			int tmpElementNumber = elementNumber;
			do
			{
				element = L.at(tmpElementNumber);
				if (paramC - sumElementsOfBin >= element.getValue())
				{
					if (paramC - sumElementsOfBin - element.getValue() >= paramB  && firstChoose)
					{
						bestElement = tmpElementNumber;
						break;
					}
					else if (paramC - sumElementsOfBin - element.getValue() < bestFreeSpace)
					{
						bestFreeSpace = paramC - sumElementsOfBin - element.getValue();
						bestElement = tmpElementNumber;
					}
				}
				else if (paramC - sumElementsOfBin < element.getValue() && paramB <= paramC - sumElementsOfBin && element.getValue() >= 2 * paramB)
				{
					Element firstPartOfElement(element.getIndex(), min(element.getValue() - paramB, paramC - sumElementsOfBin));

					int potentialFreeSpace = paramC - (sumElementsOfBin + firstPartOfElement.getValue());

					if (potentialFreeSpace < bestFreeSpace)
					{
						bestElement = tmpElementNumber;
						bestFreeSpace = potentialFreeSpace;
					}

				}
				firstChoose = false;
				++tmpElementNumber;
			} while (bestFreeSpace < paramB && bestFreeSpace > 0 && tmpElementNumber < L.size() - 1 && sumElementsOfBin != 0);

			element = L.at(bestElement);

			packElementv1(element, bestElement, &binSolution, Algorithm::BinFFAWB);

			if (!firstChoose && paramC - sumElementsOfBin != 0 && !L.empty())
			{
				for (numberOfFirstSmallElement = 0; numberOfFirstSmallElement < L.size(); ++numberOfFirstSmallElement)
				{
					if (L.at(numberOfFirstSmallElement).getValue() <= paramC - sumElementsOfBin)
						break;
				}
				if (numberOfFirstSmallElement != L.size())
					elementNumber = numberOfFirstSmallElement;
			}

		}

		printBin(binSolution);

		solution.push_back(binSolution);
	}

	if (debug)
		calculateSizeOfSolution();

	if (debug && startSize != endSize)
	{
		cout << "ERROR in BinFFAWB, startSize != endSize" << endl;
		exit(-1);
	}

	deque<deque<Element>> correctionResult;
	correctionResult = correct(withCorrection, solution, Algorithm::BinFFAWB);

	if (correctionResult.size() < solution.size() && correctionResult.size() != 0)
		return correctionResult;
	else
		return solution;
}

void AlgorithmsBPP::returnFragmentToDequeBinFFAWB(Element element)
{
	if (debug && element.getValue() < paramB)
	{
		cout << "ERROR in returnFragmentToDequeBinFFAWB, element.getValue() < paramB" << endl;
		exit(-1);
	}
	std::deque<Element>::iterator it = L.begin();
	while (it != L.end())
	{
		if (compareBinFFAWB(element, *it))
		{
			L.insert(it, element);
			return;
		}
		++it;
	}
	L.push_back(element);

}

void AlgorithmsBPP::returnFragmentToDeque(Element element, deque<Element>* input, bool sorted, bool decrasing)
{
	if (debug && element.getValue() < AlgorithmsBPP::paramB)
	{
		cout << "ERROR in returnFragmentToDequeBinFFAWB, element.getValue() < paramB" << endl;
		exit(-1);
	}
	if (sorted)
	{
		std::deque<Element>::iterator it = input->begin();
		while (it != input->end())
		{
			if (element.getValue() > it->getValue() && decrasing)
			{
				input->insert(it, element);
				return;
			}
			else if (element.getValue() < it->getValue() && !decrasing)
			{
				input->insert(it, element);
				return;
			}
			++it;
		}
		input->push_back(element);
	}
	else
		input->push_front(element);
}