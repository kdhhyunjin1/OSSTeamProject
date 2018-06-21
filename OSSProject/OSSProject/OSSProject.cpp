#include <iostream>
#include <cstring>
#include <set>
#include <deque>
#include <list>
#include <queue>
#define N 593
#pragma warning (disable:4996)
using namespace std;

char Sub_info[N][30];

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int i, cnt = 0;

	for (i = 1; i <= N; i++)
	{
		int tmp_n;
		char tmp[30];
		cin >> tmp_n >> tmp;
		strcpy(Sub_info[tmp_n], tmp);
	}

	for (i = 1; i <= N; i++)
	{
		cout << Sub_info[i] << endl;
	}

	return 0;
}