#include <queue>
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;

/* Define */
#define SIZE 31
#define INF 987654321
#define BAN 9000

/* Inline Function */
template <class T>
void CopyArray(T * anArray1, T * anArray2, const int nMatrix)
{ for (int ii = 0; ii < nMatrix; ii++) { anArray1[ii] = anArray2[ii]; } }

/* Struct */
typedef struct node
{
	/* Integer */
	int nLevel = 0;
	int nBound = 0;

	/* Integer Array */
	int anPath[SIZE];
	bool anPass[SIZE];

	node() { for (int ii = 0; ii < SIZE; ii++) { anPath[ii] = -1; anPass[ii] = false; } }
} NODE;

typedef struct compare
{
	bool operator() (const NODE sBound1, const NODE sBound2) { return sBound1.nBound < sBound2.nBound; }
} CMP;

/* Global Variable */
int anArray[SIZE][SIZE];
priority_queue<NODE, vector<NODE>, CMP> cPQ;

/* Declear Function */
int TSP(int * cBast, const int nMatrix);
int Bound(NODE & sU, const int nMatrix);
int Length(NODE & nU, const int nMatrix);

int main(void)
{
	cout << "input file name? ";
	string sFileName; cin >> sFileName;

	/* FILE */
	FILE * fInput = fopen(sFileName.c_str(), "rt");
	FILE * fOutput = fopen("output.txt", "wt");

	/* Integer */
	int nMatrix = 0, anBastArray[SIZE];
	fscanf(fInput, "%d", &nMatrix);

	/* Input */
	for (int ii = 0; ii < nMatrix; ii++)
	{ 
		for (int jj = 0, nTemp = 0; jj < nMatrix; jj++) 
		{ 
			fscanf(fInput, "%d", &nTemp);
			anArray[ii][jj] = nTemp == BAN ? 0 : nTemp;
		}
	}

	/* Min Output */
	fprintf(fOutput, "%d\n", TSP(anBastArray, nMatrix));

	/* Path Output */
	for (int ii = 0; ii < nMatrix; ii++)
	{ fprintf(fOutput, "%d\n", anBastArray[ii] + 1); }
	fprintf(fOutput, "%d\n", 1);

	fclose(fOutput);
	fclose(fInput);

	return 0;
}

/* Define Function */
int TSP(int * cBest, const int nMatrix)
{
	/* Node */
	NODE sU, sV;

	/* Init Node Value */
	sV.nLevel = 0;
	sV.anPath[0] = 0;
	sV.anPass[0] = true;
	sV.nBound = Bound(sV, nMatrix);

	cPQ.push(sV);

	/* Integer */
	int nMinLength = INF;

	while (!cPQ.empty())
	{
		sV = cPQ.top(); cPQ.pop();

		if (sV.nBound < nMinLength)
		{
			sU.nLevel = sV.nLevel + 1; /* sU를 sV에 자식노드에 넣는다. */
			for (int ii = 1; ii < nMatrix; ii++)
			{
				/* sV에 ii번째 노드가 없는 경우 */
				if (sV.anPass[ii] == false)
				{
					CopyArray(sU.anPath, sV.anPath, nMatrix);
					CopyArray(sU.anPass, sV.anPass, nMatrix);

					/* ii번째 노드를 마지막 경로에 추가한다. */
					sU.anPath[sU.nLevel] = ii;
					sU.anPass[ii] = true;

					/* 다음 노드가 최종 목적지 노드인지 검사 */
					if (sU.nLevel == nMatrix - 2)
					{
						int nRemind = 1;
						for (nRemind = 1; nRemind < nMatrix; nRemind++)
						{ if (sU.anPass[nRemind] == false) { break; } }

						sU.anPath[nMatrix - 1] = nRemind;
						sU.anPass[nRemind] = true;

						int nMin = Length(sU, nMatrix);
						if (nMin < nMinLength)
						{
							nMinLength = nMin;
							CopyArray(cBest, sU.anPath, nMatrix);
						}
					}
					else
					{
						sU.nBound = Bound(sU, nMatrix);
						if (sU.nBound < nMinLength) { cPQ.push(sU); }
					}
				}
			}
		}
	}

	return nMinLength;
}
int Bound(NODE & sU, const int nMatrix)
{
	/* Node */
	NODE sFirstNode, sLastNode;

	/* Integer */
	int nBound = 0, nTerm = -1;

	/* 해당 정점의 경로에 대한 마지막 원소를 구분하는 반복문 */
	for (int ii = 1; sU.anPath[ii] != -1; ii++)
	{
		/* 지나 온 정점사이의 거리 가중치 누적 */
		nBound += anArray[sU.anPath[ii - 1]][sU.anPath[ii]];

		/* 마지막 노드를 제외한 지나 온 정점사이의 거리 가중치 누적 */
		sLastNode.anPath[ii - 1] = sU.anPath[ii - 1];
		sLastNode.anPass[sU.anPath[ii - 1]] = true;

		/* 첫 노드를 제외한 지나온 정점사이의 거리 가중치 누적 */
		sFirstNode.anPath[ii - 1] = sU.anPath[ii];
		sFirstNode.anPass[sU.anPath[ii]] = true;

		nTerm = sU.anPath[ii];
	}

	for (int ii = 0, nTemp = 0; ii < nMatrix; ii++)
	{
		nTemp = INF;
		if (sLastNode.anPass[ii] == false)
		{
			for (int jj = 0; jj < nMatrix; jj++)
			{
				/* 최종 정점이 첫 정점인 경우 */
				if (ii == nTerm && jj == 0) { continue; }
				if (sFirstNode.anPass[jj] == false && anArray[ii][jj] != 0)
				{ if (nTemp > anArray[ii][jj]) { nTemp = anArray[ii][jj]; } }
			}
			nBound += nTemp;
		}
	}

	return nBound;
}
int Length(NODE & nU, const int nMatrix)
{
	/* Integer */
	int nLength = 0, nIndex = 1;

	/* 지나 온 정점사이의 거리 가중치 누적 반복문 */
	for (nIndex = 1; nIndex < nMatrix; nIndex++)
	{ nLength += anArray[nU.anPath[nIndex - 1]][nU.anPath[nIndex]]; }

	/* 마지막 정점에서 첫 정점으로 돌아오는 가중치 합 */
	nLength += anArray[nU.anPath[nIndex - 1]][nU.anPath[0]];

	return nLength;
}
