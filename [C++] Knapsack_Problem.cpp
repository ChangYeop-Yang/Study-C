#include <vector>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstdlib>
#include <iostream>
using namespace std;

/* TYPEDEF */
typedef pair<int, int> JAM;

/* Integer */
unsigned int nItemTotal = 0;
unsigned int nBagTotal = 0;
unsigned int nMaxProfit = 0;

/* Declear Function */
void BackTracking(vector<JAM> cDataVec, vector<bool> & cBestVec, vector<bool> & cTirialVec, int nPosition = -1, int nWeight = 0, int nProfit = 0);
bool Promissing(const int nBagWeight, const int nBagTotal);

int main(void)
{
	cout << "input file name? ";
	string sFileName; cin >> sFileName;

	FILE * fInput = fopen(sFileName.c_str(), "r");
	FILE * fOutput = fopen("output.txt", "w");

	fscanf(fInput, "%d %d", &nItemTotal, &nBagTotal);

	/* Vector */
	vector<JAM> cDataJamVec(nItemTotal);
	vector<bool> cTirialVec(nItemTotal);
	vector<bool> cBestJamVec;

	/* JAM INPUT */
	for (int ii = 0, nInput1 = 0, nInput2 = 0; ii < nItemTotal; ii++)
	{
		/* Profit - Weight */
		fscanf(fInput, "%d %d", &nInput1, &nInput2);
		cDataJamVec[ii] = make_pair(nInput1, nInput2);
	}

	/* Output */
	BackTracking(cDataJamVec, cBestJamVec, cTirialVec);
	fprintf(fOutput, "%d\n", nMaxProfit);

	for (const bool bValue : cBestJamVec)
	{ fprintf(fOutput, "%d\n", bValue); }

	fclose(fInput);
	fclose(fOutput);

	return 0;
}

/* Define Function */
void BackTracking(vector<JAM> cDataVec, vector<bool> & cBestVec, vector<bool> & cTirialVec, int nPosition, int nWeight, int nProfit)
{
	/* 현재 배낭의 무게가 총 무게에 대해서 초과하지 않는 경우 */
	if (Promissing(nWeight, nBagTotal))
	{
		if (nPosition == nItemTotal - 1)
		{
			if (nMaxProfit < nProfit) { nMaxProfit = nProfit; cBestVec.assign(cTirialVec.begin(), cTirialVec.end()); }
		}
		else
		{
			/* 배낭에 보석을 넣는 경우 */
			cTirialVec[nPosition + 1] = true;
			BackTracking(cDataVec, cBestVec, cTirialVec, nPosition + 1, cDataVec[nPosition + 1].second + nWeight, cDataVec[nPosition + 1].first + nProfit);

			/* 배당에 보석을 넣지 않는 경우 */
			cTirialVec[nPosition + 1] = false;
			BackTracking(cDataVec, cBestVec, cTirialVec, nPosition + 1, nWeight, nProfit);
		}
	}
}

bool Promissing(const int nBagWeight, const int nBagTotal)
{
	if (nBagWeight > nBagTotal) { return false; }
	return true;
}
