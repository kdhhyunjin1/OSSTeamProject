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

struct ANSWER
{
	int trans_ans, time_ans;
}Answer[100]; // ȯ���� ���� �ð��� �ɸ� ���� ���� ���� ����ü ����.

char Sub_info[N][30]; // �� �̸� ���� �迭
int Sub_chk[N]; // �̹� ������ ���� �ð� ����
int Sub_matrix[N][N]; //�� ������ �ð� ���� 2���� �������
int Sta_end[5]; // ������ �� ����(ȯ�¿��� ��� �������� �������� �� �� �����Ƿ� ���� �ִ� 4������ ��ġ�Ƿ� �迭 5���� ����)
deque<deque<int> > Sta_Trans; // ���� deque���� ȯ���ϴ� �� ���� (deque�� stl library���� �����ϴ� container ���� ���� ũ�⸦ ������� ���� �� �ִ� �迭�̶�� �����ϸ� ��.)
int min_time=987654321; // �ּҽð� ó���� ū ������ �ؾ� ���߿� �� ���� ���ڰ� ���͵� ��ü����.
queue<DATA> Q; //DATA������ queue ���� (queue�� �۳⿡ ������ �ƽŴٰ� �����ϰڽ��ϴ�. FILO����� container)

bool compare(const ANSWER &i, const ANSWER &j) //�̰� sort�� �� �ʿ���.
{
	return i.time_ans < j.time_ans; //�ð� ���� �������� ����
}

int main()
{
	FILE *fp;
	fp = fopen("input.txt", "r"); //���� ��������� �ϴ� input.txt�� ������ �� �޾ƿ�.
	Sta_Trans.push_back(deque<int>()); //w�� 1���� ������ �ǵ� deque�� push_back ���� �� 0���� �����ϴϱ� ó���� �ϴ� �ϳ� �߰��س��� 0�� ������� w�� �þ�� ���� �굵 1���� �����ϰ� �Ϸ��� �����۾�(���� �� �߿���. �׳� �Ѱܵ� ��.)

	int i, cnt = 0, trans_cnt = 0, w=0;
	char Ch_Sta_start[30], Ch_Sta_end[30]; //��߿� �̸�, ������ �̸�.
	int n; //�� �̸� �� �ְ� �� ���� �ٿ� ������ ������谡 �� ������ �����ּ���. ����뿡 ���ϸ� ������ 592 ���տ�忪 ������ ������ ������谡 �� ������ �����ֽø� �˴ϴ�.

	for (i = 1; i <= N-1; i++)
	{
		int tmp_n;
		char tmp[30];
		fscanf(fp, "%d%s", &tmp_n, tmp);
		strcpy(Sub_info[tmp_n], tmp); //�� ������ �� �̸� ����.
	}//�� �̸� ����

	fscanf(fp, "%d", &n);
	for (i = 1; i <= n; i++)
	{
		int a, b, c;//�ӽ� int��
		fscanf(fp, "%d%d%d", &a, &b, &c); // ��, ��, �ð�
		Sub_matrix[a][b] = c;
		Sub_matrix[b][a] = c; //��������� �ð� ����. �� ����� ���ϼ�ȯ�� �������Ƽ� ����->���� ���� ���� �߰��ؾߵſ�.
	}
	fclose(fp); //���⼭ input.txt�� ����. ��� ������ �̹� ������ ����.

	cout << "��߿��� �����ּ���: ";
	cin >> Ch_Sta_start;
	for (i = 1; i <= N; i++) //�� ó������ ������
	{
		DATA a;//�ӽ� DATA��
		if (strcmp(Sub_info[i], Ch_Sta_start) == 0) //�� �̸��� ����ϴ� ���� ���ٸ�
		{
			w++; //����� �þ�ٴ� ��. w=1�̸� ���� ù ��° ���, w=2�̸� �� ��° ���.
			a.Sta_now = i; //��߿��� ����(��߿��� ȯ�¿��� ��� ����ϴ� ���� ���� ������ ���ߵǼ� �� ���� ���ƾ� ��.
			a.time = 0; //����� ���� 0��(��)
			a.way = w; //����ϴ� ���� �������� ����� �þ�ٴ� �� w�� �߰��ϸ鼭 �˷��ٰ���.
			Q.push(a); //�� ����ü�� queue�� push��.
			Sta_Trans.push_back(deque<int>()); // ���� deque���� ���� ���� �߰���.
			Sta_Trans[w].push_back(i); // w�� �࿡�� ��߿��� �߰���. (push_back�� �迭���� �ڿ� �� ĭ�� �߰��ϴ� ����.)
			Sub_chk[i] = 0; //������ ���� ������� �ð��� �󸶳� �ɷȴ��� ����.
		}
	}

	cout << "�������� �����ּ���: ";
	cin >> Ch_Sta_end;
	
	while (!Q.empty()) //���⼭ BFS ����(�߿�, �� while �κ��� �ٽ� of �ٽ�)
	{
		int now, now_time;// �ӽ÷� ���� ���� �ð��� ���� ���� ����.

		now = Q.front().Sta_now; //���� ���� ���� �ӽú���
		now_time = Q.front().time; //���� �ð��� ���� �ӽú���
		double min_time_limit = double(min_time)*1.3; //�ּҽð��Ѱ� �ּҽð��� �����ϱ� ��� �뼱�� �� ���Ƽ� �ð��� �� �����ɸ��� �� ���� �̰� ������ ���� �پ�����. ���� �߿����� ����.


		for (i = 1; i <= N; i++)
		{
			if (Sub_matrix[now][i] > 0 && (double(now_time + Sub_matrix[now][i]) <= double(Sub_chk[i])*1.3 || Sub_chk[i] == 0) && double(now_time + Sub_matrix[now][i])<=min_time_limit) // �� �� ������ �ð��� 0�� �ƴϰ� �ð��迭�� 0�̰ų� �� ���� �ð��Ѱ踦 ���� �ʰ� �ּ� �ð� �Ѱ踦 ���� �ʴ´ٸ�(�߿�)
			{

				if (strcmp(Sub_info[i], Ch_Sta_end) == 0) //������ ���� ��ǥ�� ���� ���ٸ�
				{
					if (strcmp(Sub_info[i], Sub_info[Sta_Trans[Q.front().way].back()]) == 0) continue; //���� ������ ���� �� ���� ������ ���� ���ٸ� �ѱ�.(����뿪 ��� ������ ���׸� �Ƚ��ϱ� ���� �߰���. ���׸� ��� ���ƴ��� �̷� �͵� ���ָ� ����.)
					Sta_Trans[Q.front().way].push_back(i); //ȯ�¿� �������� ��ǥ�� �߰�
					Answer[++trans_cnt].trans_ans = Q.front().way; //���� ����ü���� ȯ�ºκп� ���� �� ��° ������� ����.
					Answer[trans_cnt].time_ans = Q.front().time; //���� ����ü���� �ð� �κп� ���� ������� �ð��� �󸶳� �ɷȴ��� ����.
					min_time = (min_time < now_time + Sub_matrix[now][i]) ? min_time : now_time + Sub_matrix[now][i]; //�������� �� �ּҽð��̸� �����ϰ� �ƴϸ� �׳� ����. ���׿����� �����.
					if (Sub_chk[i] == 0) Sub_chk[i] = now_time + Sub_matrix[now][i]; //���� ó�������� ���� �ɸ� �ð��� ���ݱ��� �ɸ� �ð��� ����.
					else Sub_chk[i] = (Sub_chk[i] < now_time + Sub_matrix[now][i]) ? Sub_chk[i] : now_time + Sub_matrix[now][i]; //�̹� �Ծ��ٸ� �� ���� �ð��� ����. ���� �ּҽð��� ���� �Ȱ���.
				}

				else
				{
					DATA a; //�ӽ� DATA�� ����
					a.Sta_now = i; //�������� Sta_now�� ����.
					a.time = now_time + Sub_matrix[now][i]; //�ð��� a.time�� ����
					if (strcmp(Sub_info[a.Sta_now], Sub_info[now]) == 0) //���� ���� ���� ���� ���� ���ٸ� (ȯ���ߴٴ� �ǹ�)
					{
						w++; //����� �߰���
						a.way = w; //way�� ����.
						Sta_Trans.push_back(Sta_Trans[Q.front().way]); //����� �߰������� ȯ�¿��� ���� ���� deque�� ���� �߰���.(������ push_back(deque<int>())�� �ٸ� ���� �̷��� �ϸ� ������ �ִ� ȯ�¿� ������ �� ���� ���� �� ����. Q.front().way�� �ǹ̸� �����غ��� �� �� ����.)

						Sta_Trans[w].push_back(i); //���� ȯ�¿��� �� �־���� �� �࿡ ���� �� �߰�.
					}
					else a.way = Q.front().way; //ȯ������ �ʾҴٸ� �׳� ����� �״�� ����.
					if(Sub_chk[i]==0) Sub_chk[i] = Q.front().time; //���� �� �ð��� �Ȱ���.
					else Sub_chk[i] = (Sub_chk[i] < Q.front().time) ? Sub_chk[i] : Q.front().time; //���� �Ȱ���.
					Q.push(a); //queue�� �߰���.
				}
			}
		}

		Q.pop(); // �� �����ϰ� ���� �� ���� queue�� pop��.
	}

	sort(Answer + 1, Answer + trans_cnt + 1, compare); //stl sort�� ���ؼ� ���� ����ü�� �ð����� sort��.

	for (i = 1; i <= trans_cnt; i++)
	{
		int tmp = Answer[i].trans_ans; //����� �ӽú����� ����.
		for (int j = 0; j < Sta_Trans[tmp].size(); j++) //size()�� ���� ���� ũ�⸦ ����ϴ� �޼ҵ���. �׷��� ������� ������ ���� for���� �����.
		{
			cout << Sub_info[Sta_Trans[tmp][j]] << ' '; //����࿡ ���ڰ� �ִ� �� �� �������� �����ؼ� �� �̸����� ���.
		}
		cout << "�ɸ��� �ð�: " << Answer[i].time_ans<<"��"<<endl; //�ɸ��� �ð� ���.
	}
	
	
	system("pause");

	return 0;
}