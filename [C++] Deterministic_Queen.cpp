#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

/* DEFINE */
#define SIZE 1001

/* Global Variable */
int anArray[SIZE];

/* Function Declare */
void BackTracking(int nIndex, int nSize);
bool Promissing(int nSize);

int main(void)
{
	cout << "input file name? ";

	string sFileName; cin >> sFileName;
	FILE * fInput = fopen(sFileName.c_str(), "r");

	int nNumber = 0;
	fscanf(fInput, "%d", &nNumber);

	BackTracking(0, nNumber);

	fclose(fInput);

	return 0;
}

/* Function Define */
void BackTracking(int nIndex, int nSize)
{
	for (int jj = 0; jj < nSize; jj++)
	{
		/* 해당 배열 원소에 퀸을 놓는다. */
		anArray[nIndex] = jj;
		if (Promissing(nIndex))
		{
			/* 퀸의 최적해를 찾은 경우 */
			if (nIndex == nSize - 1)
			{
				FILE * fOutput = fopen("output.txt", "wt");
				for (int ii = 0; ii < nSize; ii++)
				{ fprintf(fOutput, "%d\n", anArray[ii]); } 
				fclose(fOutput); exit(true);
			}
			/* 퀸의 최적해를 찾지 못한 경우 */
			else { BackTracking(nIndex + 1, nSize); }
		}
	}
}
bool Promissing(int nSize)
{
	/* 배열에서 퀸이 놓을 수 있는 자리를 찾는 반복문 */
	for (int kk = 0; kk < nSize; kk++)
	{
		/* 퀸의 상하좌우 위치를 확인하여 놓을 수 없는 경우 */
		if (anArray[kk] == anArray[nSize] || (abs(anArray[kk] - anArray[nSize]) == abs(kk - nSize)))
			return false;
	}

	return true;
}
