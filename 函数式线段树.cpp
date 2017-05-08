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
	node() { son[1] = son[0] = NULL; key = 0; }				//Key是代表有多少个子？
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
	node *root[100100];				//作用大概是与时间轴重合吧
	int basetime;					//经历过的绝对时间，时间是一维的。
	int section;					//线段树的大小上界，下届默认是1吧

	void init(int si)				//线段树大小
	{
		root[0] = ne() ;
		root[0]->section_min = 1;
		root[0]->section_max = si;
		section = si;
		basetime = 0;
		build(root[0]);
	}

	void build(node *right_now)		//主席树的构建，递归构建（while 循环好像也可以)
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

	void time_add(int ch)			//会让[ch,ch]和一路上加1，ch是change_section的意思
	{
		basetime++;
		root[basetime] = ne();
		root[basetime]->section_max = section;
		root[basetime]->section_min = 1;		//r的初始化，也就是base时间的初始化，
		node *p, *r;							//p是past，代表过去，是上一时刻,r是rightnow，代表现在
		r = root[basetime];
		p = root[basetime - 1];
		r->key = p->key + 1;
		while (r->section_max > r->section_min)		//虽然不是很想这样说，但这个代码是基于时间的连续性
		{
			int mid = (r->section_max + r->section_min) / 2;
			if (ch > mid)								//ch是加的点！它应该在右边
			{
				r->son[0] = p->son[0];					//左侧继承过去的时刻
				r->son[1] = ne();					//新建一个右边节点
				r->son[1]->section_max = p->son[1]->section_max;
				r->son[1]->section_min = p->son[1]->section_min;
				r->son[1]->key = p->son[1]->key + 1;
				r = r->son[1];
				p = p->son[1];
			}
			else										//ch在左边
			{
				r->son[1] = p->son[1];					//左侧继承过去的时刻
				r->son[0] = ne();					//新建一个右边节点
				r->son[0]->section_max = p->son[0]->section_max;
				r->son[0]->section_min = p->son[0]->section_min;
				r->son[0]->key = p->son[0]->key + 1;
				r = r->son[0];
				p = p->son[0];
			}
		}
		return;
	}

	int k_small(int mi, int ma, int k)		//求两个时刻（区间）内第k小
	{
		node *p = root[mi - 1], *r = root[ma];
		if (r->key - p->key < k)return -233;		//出错的返还，测试时候用
		while (r->section_max != r->section_min)
		{
			int temp = r->son[0]->key - p->son[0]->key;		//左子树元素个数。
			if (k <= temp)							//左子树有大于K个元素存在
			{
				p = p->son[0];
				r = r->son[0];
			}
			else									//左子数并没有那么多元素，转移到右子树。k减小temp
			{
				k = k - temp;
				p = p->son[1];
				r = r->son[1];
			}
		}
		return r->section_max;		//此时肯定相等了，否则会死循环
	}

	/*如果脱离了时间连续性，利用树状数组处理，来求前缀和并相减，可以更改区间，复杂度会是n*logn*logn 现在的时间复杂度是n*lgn */
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
		}								//此时固定出l
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