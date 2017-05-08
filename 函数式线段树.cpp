#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>
#include<algorithm>
#include<set>
#include<vector>
using namespace std;

struct node
{
	node *son[2];
	int section_min, section_max;
	int key;
	node() { son[1] = son[0] = NULL; key = 0; }				//Key�Ǵ����ж��ٸ��ӣ�
};
node memory[500500];
int memory_now = -1;
node* ne()
{
	memory_now++;
	return &memory[memory_now];
}

struct functional_tree
{
	node *root[100100];				//���ô������ʱ�����غϰ�
	int basetime;					//�������ľ���ʱ�䣬ʱ����һά�ġ�
	int section;					//�߶����Ĵ�С�Ͻ磬�½�Ĭ����1��

	void init(int si)				//�߶�����С
	{
		root[0] = ne() ;
		root[0]->section_min = 1;
		root[0]->section_max = si;
		section = si;
		basetime = 0;
		build(root[0]);
	}

	void build(node *right_now)		//��ϯ���Ĺ������ݹ鹹����while ѭ������Ҳ����)
	{
		if (right_now->section_min == right_now->section_max) return;
		int mid = (right_now->section_max + right_now->section_min) / 2;
		right_now->son[0] = new node;
		right_now->son[0]->section_min = right_now->section_min;
		right_now->son[0]->section_max = mid;

		right_now->son[1] = new node;
		right_now->son[1]->section_min = mid + 1;
		right_now->son[1]->section_max = right_now->section_max;
		build(right_now->son[0]);
		build(right_now->son[1]);
		return;
	}

	void time_add(int ch)			//����[ch,ch]��һ·�ϼ�1��ch��change_section����˼
	{
		basetime++;
		root[basetime] = ne();
		root[basetime]->section_max = section;
		root[basetime]->section_min = 1;		//r�ĳ�ʼ����Ҳ����baseʱ��ĳ�ʼ����
		node *p, *r;							//p��past�������ȥ������һʱ��,r��rightnow����������
		r = root[basetime];
		p = root[basetime - 1];
		r->key = p->key + 1;
		while (r->section_max > r->section_min)		//��Ȼ���Ǻ�������˵������������ǻ���ʱ���������
		{
			int mid = (r->section_max + r->section_min) / 2;
			if (ch > mid)								//ch�Ǽӵĵ㣡��Ӧ�����ұ�
			{
				r->son[0] = p->son[0];					//���̳й�ȥ��ʱ��
				r->son[1] = ne();					//�½�һ���ұ߽ڵ�
				r->son[1]->section_max = p->son[1]->section_max;
				r->son[1]->section_min = p->son[1]->section_min;
				r->son[1]->key = p->son[1]->key + 1;
				r = r->son[1];
				p = p->son[1];
			}
			else										//ch�����
			{
				r->son[1] = p->son[1];					//���̳й�ȥ��ʱ��
				r->son[0] = ne();					//�½�һ���ұ߽ڵ�
				r->son[0]->section_max = p->son[0]->section_max;
				r->son[0]->section_min = p->son[0]->section_min;
				r->son[0]->key = p->son[0]->key + 1;
				r = r->son[0];
				p = p->son[0];
			}
		}
		return;
	}

	int k_small(int mi, int ma, int k)		//������ʱ�̣����䣩�ڵ�kС
	{
		node *p = root[mi - 1], *r = root[ma];
		if (r->key - p->key < k)return -233;		//����ķ���������ʱ����
		while (r->section_max != r->section_min)
		{
			int temp = r->son[0]->key - p->son[0]->key;		//������Ԫ�ظ�����
			if (k <= temp)							//�������д���K��Ԫ�ش���
			{
				p = p->son[0];
				r = r->son[0];
			}
			else									//��������û����ô��Ԫ�أ�ת�Ƶ���������k��Сtemp
			{
				k = k - temp;
				p = p->son[1];
				r = r->son[1];
			}
		}
		return r->section_max;		//��ʱ�϶�����ˣ��������ѭ��
	}

	/*���������ʱ�������ԣ�������״���鴦������ǰ׺�Ͳ���������Ը������䣬���ӶȻ���n*logn*logn ���ڵ�ʱ�临�Ӷ���n*lgn */
};


int a[100100], b[100100];
functional_tree t;
int main()
{
	ios::sync_with_stdio(false);
	int n, m;
	cin >> n >> m;
	t.init(n);
	for (int i = 1; i <= n; i++)
	{
		cin >> a[i];
		b[i] = a[i];
	}
	sort(b + 1, b + n + 1);
	for (int i = 1; i <= n; i++)	
	{
		int l = 1, r = n;
		while (l < r)
		{
			int mid = (l + r + 1) / 2;
			if (a[i] < b[mid]) r = mid - 1;
			else l = mid;
		}								//��ʱ�̶���l
		t.time_add(l);
	}
	for (int i = 1; i <= m; i++)
	{
		int l, r, k;
		cin >> l >> r >> k;
		cout << b[t.k_small(l, r, k)] << endl;
	}
	//	system("pause");
	return 0;
}