#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <deque>
#include <Windows.h>
#define N 593 //역 갯수
#pragma warning (disable:4996)
using namespace std;

struct DATA
{
	int Sta_now, time, way; //현재 역정보, 시간, 몇 번째 방법
};

char Sub_info[N][30]; // 역 이름 넣을 배열
int Sub_chk[N]; // 이미 지나온 역인지 확인
int Sub_matrix[N][N]; //역 사이의 시간 넣을 2차원 인접행렬
int Sta_end[5]; // 끝나는 역 저장(환승역일 경우 여러개가 도착점이 될 수 있으므로 역이 최대 4개까지 겹치므로 배열 5개로 생성)
deque<deque<int> > Sta_Trans; // 이중 deque으로 환승하는 역 저장
int Trans_ans[100], Time_ans[100]; // 방법 저장, 시간 저장
int min_time = 987654321; // 최소시간 처음에 큰 정수로 해야 나중에 더 작은 숫자가 나와도 교체가능.
queue<DATA> Q; //DATA형으로 queue 생성

int main()
{
	FILE *fp;
	fp = fopen("input.txt", "r");
	Sta_Trans.push_back(deque<int>());

	int i, cnt = 0, trans_cnt = 0, w = 0;
	char Ch_Sta_start[30], Ch_Sta_end[30];
	int n; //역 이름 다 넣고 그 다음 줄에 역끼리 상관관계가 몇 개인지 적어주세요. 참고용에 의하면 마지막 592 종합운동장역 다음에 역끼리 상관관계가 몇 개인지 적어주시면 됩니다.

	for (i = 1; i <= N - 1; i++)
	{
		int tmp_n;
		char tmp[30];
		fscanf(fp, "%d%s", &tmp_n, tmp);
		strcpy(Sub_info[tmp_n], tmp);
	}//역 이름 넣음

	fscanf(fp, "%d", &n);
	for (i = 1; i <= n; i++)
	{
		int a, b, c;//임시 int형
		fscanf(fp, "%d%d%d", &a, &b, &c);
		Sub_matrix[a][b] = c;
		Sub_matrix[b][a] = c; //양방향으로 시간 적음. 아 참고로 응암순환은 거지같아서 구산->응암 쪽은 따로 추가해야돼요.
	}
	fclose(fp);

	cout << "출발역을 적어주세요: ";
	cin >> Ch_Sta_start;
	for (i = 1; i <= N; i++) //역 처음부터 끝까지
	{
		DATA a;//임시 DATA형
		if (strcmp(Sub_info[i], Ch_Sta_start) == 0) //역 이름과 출발하는 역이 같다면
		{
			w++;
			a.Sta_now = i; //출발역에 숫자(출발역이 환승역일 경우 출발하는 곳이 여러 곳으로 봐야되서 꼭 전부 돌아야 됨.
			a.time = 0; //출발할 때는 0분(초)
			a.way = w;
			Q.push(a); //이 구조체를 queue에 push함.
			Sta_Trans.push_back(deque<int>());
			Sta_Trans[w].push_back(i);
			Sub_chk[i] = 1;
		}
	}

	cout << "도착역을 적어주세요: ";
	cin >> Ch_Sta_end;

	while (!Q.empty())
	{
		int now, now_time;// 임시로 현재 역과 시간을 담을 변수 선언.

		double min_time_limit = double(min_time)*1.3; //최소시간 한계. 임시로 min_time의 1.3배로 설정함. 회의를 통해 교체가능. 이 시간이 넘으면 더 이상 진행하지 않음.
		now = Q.front().Sta_now;
		now_time = Q.front().time;


		for (i = 1; i <= N; i++)
		{
			if (Sub_matrix[now][i]>0 && Sub_chk[i] == 0 && (now_time + Sub_matrix[now][i] <= min_time_limit)) // 두 역 사이의 시간이 0이 아니고 체킹배열이 0이고 최소시간 한계를 넘지 않는다면
			{

				if (strcmp(Sub_info[i], Ch_Sta_end) == 0)
				{

					if (min_time > now_time + Sub_matrix[now][i])
					{
						min_time = now_time + Sub_matrix[now][i];
					}
					Sta_Trans[Q.front().way].push_back(i);
					Trans_ans[++trans_cnt] = Q.front().way;
					Time_ans[trans_cnt] = Q.front().time;
				}

				else
				{
					DATA a;
					a.Sta_now = i;
					a.time = now_time + Sub_matrix[now][i];
					if (strcmp(Sub_info[a.Sta_now], Sub_info[now]) == 0)
					{
						w++;
						a.way = w;
						/*for (int j = 0; j < Sta_Trans[Q.front().way].size(); j++) // 여긴 해결.
						{
						Sta_Trans[w].push_back(Sta_Trans[Q.front().way][j]);

						}*/
						Sta_Trans.push_back(Sta_Trans[Q.front().way]);

						Sta_Trans[w].push_back(i);
					}
					else a.way = Q.front().way;
					Q.push(a);
				}
				Sub_chk[i] = 1;
			}
		}

		Q.pop(); // 다 진행하고 나면 맨 위의 queue를 pop함.
	}

	for (i = 1; i <= trans_cnt; i++)
	{
		int tmp = Trans_ans[i];
		for (int j = 0; j < Sta_Trans[tmp].size(); j++)
		{
			cout << Sub_info[Sta_Trans[tmp][j]] << ' ';
		}
		cout << "걸린 시간: " << Time_ans[i] << endl;
	}


	system("pause");

	return 0;
}