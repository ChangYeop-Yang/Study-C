#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

/* Function Declare */
bool BackTracking(int nIndex, int nSize);
bool Estinmate(const int nMatrix, int nIndex);
bool Promissing(int nSize, int nPosition);

/* Global Variable */
int * anArray;

int main(void)
{
	cout << "input file name? ";
	string sFileName; cin >> sFileName;

	FILE * fInput = fopen(sFileName.c_str(), "r");
	FILE * fOutput = fopen("output.txt", "wt");

	/* Integer */
	int nNumber = 0;
	fscanf(fInput, "%d", &nNumber);

	anArray = new int[nNumber + 1];
	if (Estinmate(nNumber, 1))
		for (int ii = 1; ii <= nNumber; ii++) { fprintf(fOutput, "%d\n", anArray[ii]); }

	fclose(fInput);
	fclose(fOutput);
	return 0;
}

/* Function Define */
bool BackTracking(int nIndex, int nSize)
{
	if (nIndex > nSize) { return true; }

	for (int jj = 1; jj <= nSize; jj++)
	{
		/* 해당 배열 원소에 퀸을 놓는다. */
		if (Promissing(nIndex, jj))
		{
			anArray[nIndex++] = jj;
			return BackTracking(nIndex, nSize);
		}
	}
	return false;
}
bool Estinmate(const int nMatrix, int nIndex)
{
	/* Integer */
	int nSubIndex = 0;

	/* Vector */
	vector<int> cQueenVec(nMatrix, 0);

	srand((unsigned int)time(NULL));
	do
	{
		nSubIndex = 0;

		for (int ii = 1; ii <= nMatrix; ii++)
		{
			if (Promissing(nIndex, ii)) { cQueenVec[nSubIndex++] = ii; }
		}

		if (nSubIndex != 0)
		{
			anArray[nIndex++] = cQueenVec[rand() % nSubIndex];
			if (nIndex >= (nMatrix * 0.8))
			{
				if (BackTracking(nIndex, nMatrix)) { return true; }
			}
		}
		else { nIndex = 1; }

	} while (nIndex <= nMatrix);

	return true;
}
bool Promissing(int nSize, int nPosition)
{
	/* 배열에서 퀸이 놓을 수 있는 자리를 찾는 반복문 */
	for (int kk = 1; kk < nSize; kk++)
	{
		/* 퀸의 상하좌우 위치를 확인하여 놓을 수 없는 경우 */
		if (anArray[kk] == nPosition || (abs(anArray[kk] - nPosition) == abs(kk - nSize)))
			return false;
	}

	return true;
}
