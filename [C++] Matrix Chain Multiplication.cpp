#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

/* Define */
#define SIZE 1001
#define INF 999999999L

/* Declare Function */
void MinMult(int * mWeigth, int mSize);
void PrintOrder(int num1, int num2);
pair<int, int> MinPath(const vector<pair<int, int>> & mVec);

/* Integer Array */
int mArray[SIZE][SIZE];
int mArrayIndex[SIZE][SIZE];

int main(void)
{
	/* Integer */
	int * mWeight = new int[SIZE];

	/* Init Array */
	for (int ii = 1; ii < SIZE; ii++)
		mArray[ii][ii] = mArrayIndex[ii][ii] = 0;
	
	/* Input Weight Array */
	for (int ii = 0; ii < SIZE; ii++)
		cin >> mWeight[ii];

	/* MinMult Function */
	MinMult(mWeight, SIZE);
	/* Order Print Function */
	PrintOrder(1, SIZE-1);

	cout << endl;

	return 0;
}

/* Define Function */
void MinMult(int * mWeigth, int mSize)
{
	/* Vector */
	vector<pair<int, int>> mVector;

	for (int ii = 1; ii < mSize; ii++)
	{
		for (int jj = ii - 1; jj > 0; jj--)
		{
			mVector.clear();
			for (int kk = jj; kk < ii; kk++)
			{ mVector.push_back(make_pair(mArray[jj][kk] + mArray[kk + 1][ii] + (mWeigth[jj - 1] * mWeigth[kk] * mWeigth[ii]), kk)); }
			
			const pair<int, int> mTemp_Pair = MinPath(mVector);
			mArray[jj][ii] = mTemp_Pair.first;
			mArrayIndex[jj][ii] = mTemp_Pair.second;
		}
	}

	/* Output */
	cout << "① GOAL Value (mArray[1][n]) = " << mArray[1][mSize-1] << endl;
	cout << "② GOAL Index (mArrayIndex[1][n]) = " << mArrayIndex[1][mSize-1] << endl;
}
void PrintOrder(int num1, int num2)
{
	if (num1 == num2) { cout << 'A' << num1; return; }
	else
	{
		const int mTemp = mArrayIndex[num1][num2];
		cout << '(';
		PrintOrder(num1, mTemp);
		PrintOrder(mTemp + 1, num2);
		cout << ')';
	}
}
pair<int, int> MinPath(const vector<pair<int, int>> & mVec)
{
	/* Integer */
	int mMin = INF;
	int mSize = mVec.size();
	int mMarker = 0;

	for (int ii = 0; ii < mSize; ii++)
	{ if (mMin > mVec[ii].first) { mMin = mVec[ii].first; mMarker = mVec[ii].second; } }

	return make_pair(mMin, mMarker);
}
