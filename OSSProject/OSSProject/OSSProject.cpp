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

struct ANSWER
{
	int trans_ans, time_ans;
}Answer[100]; // 환승한 역과 시간이 걸린 것을 담을 정답 구조체 생성.

char Sub_info[N][30]; // 역 이름 넣을 배열
int Sub_chk[N]; // 이미 지나온 역의 시간 저장
int Sub_matrix[N][N]; //역 사이의 시간 넣을 2차원 인접행렬
int Sta_end[5]; // 끝나는 역 저장(환승역일 경우 여러개가 도착점이 될 수 있으므로 역이 최대 4개까지 겹치므로 배열 5개로 생성)
deque<deque<int> > Sta_Trans; // 이중 deque으로 환승하는 역 저장 (deque은 stl library에서 제공하는 container 쉽게 말해 크기를 마음대로 정할 수 있는 배열이라고 생각하면 됨.)
int min_time=987654321; // 최소시간 처음에 큰 정수로 해야 나중에 더 작은 숫자가 나와도 교체가능.
queue<DATA> Q; //DATA형으로 queue 생성 (queue는 작년에 했으니 아신다고 생각하겠습니다. FILO방식의 container)

bool compare(const ANSWER &i, const ANSWER &j) //이건 sort할 때 필요함.
{
	return i.time_ans < j.time_ans; //시간 기준 오름차순 정렬
}

int main()
{
	FILE *fp;
	fp = fopen("input.txt", "r"); //파일 입출력으로 일단 input.txt의 정보를 쭉 받아옴.
	Sta_Trans.push_back(deque<int>()); //w를 1부터 시작할 건데 deque은 push_back 했을 때 0부터 시작하니까 처음에 일단 하나 추가해놔서 0은 비워놓고 w가 늘어남에 따라 얘도 1부터 시작하게 하려는 사전작업(별로 안 중요함. 그냥 넘겨도 됨.)

	int i, cnt = 0, trans_cnt = 0, w=0;
	char Ch_Sta_start[30], Ch_Sta_end[30]; //출발역 이름, 도착역 이름.
	int n; //역 이름 다 넣고 그 다음 줄에 역끼리 상관관계가 몇 개인지 적어주세요. 참고용에 의하면 마지막 592 종합운동장역 다음에 역끼리 상관관계가 몇 개인지 적어주시면 됩니다.

	for (i = 1; i <= N-1; i++)
	{
		int tmp_n;
		char tmp[30];
		fscanf(fp, "%d%s", &tmp_n, tmp);
		strcpy(Sub_info[tmp_n], tmp); //역 정보에 역 이름 넣음.
	}//역 이름 넣음

	fscanf(fp, "%d", &n);
	for (i = 1; i <= n; i++)
	{
		int a, b, c;//임시 int형
		fscanf(fp, "%d%d%d", &a, &b, &c); // 역, 역, 시간
		Sub_matrix[a][b] = c;
		Sub_matrix[b][a] = c; //양방향으로 시간 적음. 아 참고로 응암순환은 거지같아서 구산->응암 쪽은 따로 추가해야돼요.
	}
	fclose(fp); //여기서 input.txt는 닫음. 모든 정보는 이미 저장한 상태.

	cout << "출발역을 적어주세요: ";
	cin >> Ch_Sta_start;
	for (i = 1; i <= N; i++) //역 처음부터 끝까지
	{
		DATA a;//임시 DATA형
		if (strcmp(Sub_info[i], Ch_Sta_start) == 0) //역 이름과 출발하는 역이 같다면
		{
			w++; //방법이 늘어났다는 뜻. w=1이면 가는 첫 번째 방법, w=2이면 두 번째 방법.
			a.Sta_now = i; //출발역에 숫자(출발역이 환승역일 경우 출발하는 곳이 여러 곳으로 봐야되서 꼭 전부 돌아야 됨.
			a.time = 0; //출발할 때는 0분(초)
			a.way = w; //출발하는 방향 기준으로 방법이 늘어났다는 걸 w를 추가하면서 알려줄거임.
			Q.push(a); //이 구조체를 queue에 push함.
			Sta_Trans.push_back(deque<int>()); // 이중 deque에서 먼저 행을 추가함.
			Sta_Trans[w].push_back(i); // w의 행에서 출발역을 추가함. (push_back은 배열에서 뒤에 한 칸을 추가하는 것임.)
			Sub_chk[i] = 0; //각각의 역에 여기까지 시간이 얼마나 걸렸는지 저장.
		}
	}

	cout << "도착역을 적어주세요: ";
	cin >> Ch_Sta_end;
	
	while (!Q.empty()) //여기서 BFS 시작(중요, 이 while 부분이 핵심 of 핵심)
	{
		int now, now_time;// 임시로 현재 역과 시간을 담을 변수 선언.

		now = Q.front().Sta_now; //지금 역을 담을 임시변수
		now_time = Q.front().time; //지금 시간을 담을 임시변수
		double min_time_limit = double(min_time)*1.3; //최소시간한계 최소시간이 없으니까 모든 노선을 다 돌아서 시간이 좀 오래걸리는 것 같아 이거 넣으면 조금 줄어들거임. 역시 중요하지 않음.


		for (i = 1; i <= N; i++)
		{
			if (Sub_matrix[now][i] > 0 && (double(now_time + Sub_matrix[now][i]) <= double(Sub_chk[i])*1.3 || Sub_chk[i] == 0) && double(now_time + Sub_matrix[now][i])<=min_time_limit) // 두 역 사이의 시간이 0이 아니고 시간배열이 0이거나 그 역의 시간한계를 넘지 않고 최소 시간 한계를 넘지 않는다면(중요)
			{

				if (strcmp(Sub_info[i], Ch_Sta_end) == 0) //도착할 역과 목표한 역이 같다면
				{
					if (strcmp(Sub_info[i], Sub_info[Sta_Trans[Q.front().way].back()]) == 0) continue; //만약 도착한 역이 이 전에 찍어놨던 역과 같다면 넘김.(광운대역 계속 찍혔던 버그를 픽스하기 위해 추가함. 버그를 어떻게 고쳤는지 이런 것도 써주면 좋음.)
					Sta_Trans[Q.front().way].push_back(i); //환승역 마지막에 목표역 추가
					Answer[++trans_cnt].trans_ans = Q.front().way; //정답 구조체에서 환승부분에 지금 몇 번째 방법인지 저장.
					Answer[trans_cnt].time_ans = Q.front().time; //정답 구조체에서 시간 부분에 현재 방법에서 시간이 얼마나 걸렸는지 저장.
					min_time = (min_time < now_time + Sub_matrix[now][i]) ? min_time : now_time + Sub_matrix[now][i]; //도착했을 때 최소시간이면 갱신하고 아니면 그냥 냅둠. 삼항연산자 사용함.
					if (Sub_chk[i] == 0) Sub_chk[i] = now_time + Sub_matrix[now][i]; //지금 처음왔으면 역에 걸린 시간에 지금까지 걸린 시간을 넣음.
					else Sub_chk[i] = (Sub_chk[i] < now_time + Sub_matrix[now][i]) ? Sub_chk[i] : now_time + Sub_matrix[now][i]; //이미 왔었다면 더 적은 시간을 넣음. 위의 최소시간과 거의 똑같음.
				}

				else
				{
					DATA a; //임시 DATA형 선언
					a.Sta_now = i; //도착역을 Sta_now에 저장.
					a.time = now_time + Sub_matrix[now][i]; //시간을 a.time에 저장
					if (strcmp(Sub_info[a.Sta_now], Sub_info[now]) == 0) //만약 지금 역과 이전 역이 같다면 (환승했다는 의미)
					{
						w++; //방법이 추가됨
						a.way = w; //way에 넣음.
						Sta_Trans.push_back(Sta_Trans[Q.front().way]); //방법이 추가됐으니 환승역을 넣을 이중 deque도 행을 추가함.(위에서 push_back(deque<int>())과 다른 점은 이렇게 하면 이전에 있던 환승역 정보를 다 갖다 넣을 수 있음. Q.front().way의 의미를 생각해보면 알 수 있음.)

						Sta_Trans[w].push_back(i); //이전 환승역들 다 넣어놓은 그 행에 지금 역 추가.
					}
					else a.way = Q.front().way; //환승하지 않았다면 그냥 방법도 그대로 넣음.
					if(Sub_chk[i]==0) Sub_chk[i] = Q.front().time; //위에 역 시간과 똑같음.
					else Sub_chk[i] = (Sub_chk[i] < Q.front().time) ? Sub_chk[i] : Q.front().time; //역시 똑같음.
					Q.push(a); //queue에 추가함.
				}
			}
		}

		Q.pop(); // 다 진행하고 나면 맨 앞의 queue를 pop함.
	}

	sort(Answer + 1, Answer + trans_cnt + 1, compare); //stl sort를 통해서 정답 구조체를 시간으로 sort함.

	for (i = 1; i <= trans_cnt; i++)
	{
		int tmp = Answer[i].trans_ans; //방법을 임시변수에 저장.
		for (int j = 0; j < Sta_Trans[tmp].size(); j++) //size()는 지금 덱의 크기를 출력하는 메소드임. 그래서 방법행의 갯수를 따라 for문이 진행됨.
		{
			cout << Sub_info[Sta_Trans[tmp][j]] << ' '; //방법행에 숫자가 있는 걸 역 정보에서 참조해서 역 이름으로 출력.
		}
		cout << "걸리는 시간: " << Answer[i].time_ans<<"분"<<endl; //걸리는 시간 출력.
	}
	
	
	system("pause");

	return 0;
}