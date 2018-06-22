#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <deque>
#include <Windows.h>
#define N 593 //�� ����
#pragma warning (disable:4996)
using namespace std;

struct DATA
{
	int Sta_now, time, way; //���� ������, �ð�, �� ��° ���
};

char Sub_info[N][30]; // �� �̸� ���� �迭
int Sub_chk[N]; // �̹� ������ ���� �ð� ����
int Sub_matrix[N][N]; //�� ������ �ð� ���� 2���� �������
int Sta_end[5]; // ������ �� ����(ȯ�¿��� ��� �������� �������� �� �� �����Ƿ� ���� �ִ� 4������ ��ġ�Ƿ� �迭 5���� ����)
deque<deque<int> > Sta_Trans; // ���� deque���� ȯ���ϴ� �� ����
int Trans_ans[100], Time_ans[100]; // ��� ����, �ð� ����
int min_time=987654321; // �ּҽð� ó���� ū ������ �ؾ� ���߿� �� ���� ���ڰ� ���͵� ��ü����.
queue<DATA> Q; //DATA������ queue ����

int main()
{
	FILE *fp;
	fp = fopen("input.txt", "r");
	Sta_Trans.push_back(deque<int>());

	int i, cnt = 0, trans_cnt = 0, w=0;
	char Ch_Sta_start[30], Ch_Sta_end[30];
	int n; //�� �̸� �� �ְ� �� ���� �ٿ� ������ ������谡 �� ������ �����ּ���. ����뿡 ���ϸ� ������ 592 ���տ�忪 ������ ������ ������谡 �� ������ �����ֽø� �˴ϴ�.

	for (i = 1; i <= N-1; i++)
	{
		int tmp_n;
		char tmp[30];
		fscanf(fp, "%d%s", &tmp_n, tmp);
		strcpy(Sub_info[tmp_n], tmp);
	}//�� �̸� ����

	fscanf(fp, "%d", &n);
	for (i = 1; i <= n; i++)
	{
		int a, b, c;//�ӽ� int��
		fscanf(fp, "%d%d%d", &a, &b, &c);
		Sub_matrix[a][b] = c;
		Sub_matrix[b][a] = c; //��������� �ð� ����. �� ����� ���ϼ�ȯ�� �������Ƽ� ����->���� ���� ���� �߰��ؾߵſ�.
	}
	fclose(fp);

	cout << "��߿��� �����ּ���: ";
	cin >> Ch_Sta_start;
	for (i = 1; i <= N; i++) //�� ó������ ������
	{
		DATA a;//�ӽ� DATA��
		if (strcmp(Sub_info[i], Ch_Sta_start) == 0) //�� �̸��� ����ϴ� ���� ���ٸ�
		{
			w++;
			a.Sta_now = i; //��߿��� ����(��߿��� ȯ�¿��� ��� ����ϴ� ���� ���� ������ ���ߵǼ� �� ���� ���ƾ� ��.
			a.time = 0; //����� ���� 0��(��)
			a.way = w;
			Q.push(a); //�� ����ü�� queue�� push��.
			Sta_Trans.push_back(deque<int>());
			Sta_Trans[w].push_back(i);
			Sub_chk[i] = 0;
		}
	}

	cout << "�������� �����ּ���: ";
	cin >> Ch_Sta_end;
	
	while (!Q.empty())
	{
		int now, now_time;// �ӽ÷� ���� ���� �ð��� ���� ���� ����.

		now = Q.front().Sta_now;
		now_time = Q.front().time;


		for (i = 1; i <= N; i++)
		{
			if (Sub_matrix[now][i] > 0 && (double(now_time + Sub_matrix[now][i]) <= double(Sub_chk[i])*1.3 || Sub_chk[i] == 0)) // �� �� ������ �ð��� 0�� �ƴϰ� �ð��迭�� 0�̰ų� �� ���� �ð��Ѱ踦 ���� ������
			{

				if (strcmp(Sub_info[i], Ch_Sta_end) == 0)
				{
					if (strcmp(Sub_info[i], Sub_info[Sta_Trans[Q.front().way].back()]) == 0) continue;
					Sta_Trans[Q.front().way].push_back(i);
					Trans_ans[++trans_cnt] = Q.front().way;
					Time_ans[trans_cnt] = Q.front().time;
					if (Sub_chk[i] == 0) Sub_chk[i] = now_time + Sub_matrix[now][i];
					else Sub_chk[i] = (Sub_chk[i] < now_time + Sub_matrix[now][i]) ? Sub_chk[i] : now_time + Sub_matrix[now][i];
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
						Sta_Trans.push_back(Sta_Trans[Q.front().way]);

						Sta_Trans[w].push_back(i);
					}
					else a.way = Q.front().way;
					if(Sub_chk[i]==0) Sub_chk[i] = Q.front().time;
					else Sub_chk[i] = (Sub_chk[i] < Q.front().time) ? Sub_chk[i] : Q.front().time;
					Q.push(a);
				}
			}
		}

		Q.pop(); // �� �����ϰ� ���� �� ���� queue�� pop��.
	}

	for (i = 1; i <= trans_cnt; i++)
	{
		int tmp = Trans_ans[i];
		for (int j = 0; j < Sta_Trans[tmp].size(); j++)
		{
			cout << Sub_info[Sta_Trans[tmp][j]] << ' ';
		}
<<<<<<< HEAD
		cout << "�ɸ� �ð�: " << Time_ans[i]<<"��"<<endl;
=======
		cout << "�ɸ� �ð�: " << Time_ans[i] / 60 << "�ð� " << Time_ans[i] % 60 << "��" << endl;
>>>>>>> 6a5a814984fcb899e0961e22ca810bcae4e086c2
	}
	
	system("pause");

	return 0;
}