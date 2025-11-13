# 同色边 $K_4$ 在双色染边的完全图中的数量

## 问题描述与解答

### 问题：

$G$ 为 $n$ 个点的完全图，用黑白两种颜色对 $G$ 中的边进行染色，证明一定存在一种染色方式使得同色 $K_4$ 的数量至多为 $\binom{n}{4}\cdot 2^{-5}~.$

### 解答：

用概率方法可以简单地证明存在性：等概率地用黑色或白色染 $G$ 中的边，则某个 $K_4$ 为同色的概率是 $2/2^{6}=2^{-5}$，由期望的线性性，同色 $K_4$ 数量的期望为 $\binom{n}{4}\cdot 2^{-5}$，则一定存在一种染色方案使得同色 $K_4$ 数量小于等于 $\binom{n}{4}\cdot 2^{-5}$. 

概率方法只给出了存在性的证明，我们希望找到确定性的满足条件的染色方法（去随机化）：对 $G$ 中的边随便排个序，假设 $e_1,\cdots,e_{i-1}$ 已染好色， $e_i,\cdots,e_m$ 未染色，现在需要染 $e_i$ 边。对 $G$ 中每一个 $K_4$ 定义这样一个函数 $w(K_4)\in \mathbb R$，若 $K_4$ 的 $6$ 条边都未染色，则 $w(K_4)=2^{-5}$ ；若 $K_4$ 的 $6$ 条边有 $j$ 条边已经染好色，并且它们不同色，则 $w(K_4)=0$；若 $K_4$ 的 $6$ 条边有 $j$ 条边已经染好色，并且它们同色，则 $w(K_4)=2^{-(6-j)}$。

现在只有 $e_1,\cdots,e_{i-1}$ 染好了色，定义 $w_{i-1}=\sum_{K_4}w(K_4)$ 为当前染色方案的“分数”，定义 $w_i(\text{black})$ 为 $e_i$ 染成黑色后染色方案的“分数”，$w_i(\text{white})$ 为 $e_i$ 染成白色后染色方案的“分数”。通过计算有 

$$
w_{i-1}=\frac{w_i(\text{black})+w_i(\text{white})}{2}
$$

可以通过选择“分数”最小的染色方案使得 $w_{i-1}\ge w_i$，则

$$
\binom{n}{4}\cdot 2^{-5}=w_{0}\ge \cdots\ge w_m
$$

而当所有边都染好色后，任意一个 $K_4$，若为同色则 $w(K_4)=1$，否则 $w(K_4)=0$。因此 $w_m$ 正是同色 $K_4$ 的数目，因此这样确定性的算法给出了一种同色 $K_4$ 数目小于等于 $\binom{n}{4}\cdot 2^{-5}$ 的染色方案。

## 算法实现与复杂度

### 实现：

采用C语言编写，通过指针可以在更改 $e_i$ 颜色时快速找到那些会被修改 $w(K_4)$ 的 $K_4$。

首先定义常量（通过修改N可以对不同顶点数的完全图进行染色）：
```
const int N = 200;
const int K = N * (N - 1) * (N - 2) * (N - 3) / 24;

enum COLOR {INIT, BLACK, WHITE};
```

之后定义一些数据结构来存储 $G$。顶点用 $0,1,\cdots,n-1$ 表示，并且用 $n\times n$ 的矩阵edge来存储边，即 $edge[s][e] (s<e)$ 表示顶点s和顶点e之间的边。edge[s][t]中存放了一个链表node->node->...->node，每个node为一个包含(s,t)边的 $K_4$，用结构体k4_graph表示。k4_graph中包含了该 $K_4 $中6条边的顶点和颜色信息（用value_pair的结构体记录），以及它目前的分数 $w(K_4)$，用weight记录。初始化 $w_0=\binom{n}{4}\cdot 2^{-5}$，用total_weight记录：
```
struct value_pair
{
	int start, end;
	COLOR color = INIT;	
};

struct k4_graph
{
	value_pair edge_pair[6];
	double weight = pow(2, -5);
};

struct node
{
	k4_graph* k4 = NULL;
	node* next_node = NULL;	
};

node* edge[N][N];
double total_weight = K * pow(2, -5);
```
初始化好后，用上述算法进行染色。对现在要染色的边edge[s][e]，先将它设为白色，只更改edge[s][e]中node->node->...->node链表中的每个 $K_4$ 再计算它们更改的分数；再将它设为黑色，只更改edge[s][e]中node->node->...->node链表中的每个 $K_4$ 再计算它们更改的分数。选择分数更小的染色方式，更改edge[s][e]中node->node->...->node链表中的每个 $K_4$，再更改目前的分数total_weight。依次迭代下去即可，具体代码见最后面的完整代码。


### 复杂度与结果分析：

时间复杂度：

初始化创建了所有的 $K_4$ 和边，花费 $O(\binom{n}{4}+\binom{n}{2})=O(n^4)$。

每次迭代计算 $e_i$ 染色的分数时只计算了包含 $e_i$ 的 $K_4$，这样的 $K_4$ 一共有 $\binom{n-2}{2}$ 个，因此每次迭代的开销为 $O(n^2)$，一共有 $\binom{n}{2}$ 次迭代，故总共染色开销为 $O(n^4)$。

懒得手动清理指针了，如果要手动清理指针也有 $O(n^4)$ 的开销，不过不影响。

因此时间复杂度总共为 $O(n^4)$，还算不错。具体测下来，200个点的完全图染完色只需123秒，如下。

![图1](https://github.com/xhDeng19/Number-of-the-same-color-edge-K4-in-the-complete-graph/blob/main/pics/200_vertices_time.png?raw=true)

结果分析：

对n较小的完全图，可以列出算法给出的具体染色方案，如下是一个6个点完全图的染色方案，"ALG: Number of the same color k4s: 0"说明这个染色方案有0个同色的$K_4$。

![图2](https://github.com/xhDeng19/Number-of-the-same-color-edge-K4-in-the-complete-graph/blob/main/pics/6_vertices_coloring.png?raw=true)

当n逐渐增大时，可以研究算法ALG给出的染色方案的同色 $K_4$ 与期望 $\binom{n}{4}\cdot 2^{-5}$ 之间的关系，可以绘制如下的变化图。由于 $K_4$ 数量大约随 $n^4$ 增大，因此采用log和线性的尺度来呈现下图。

![图3](https://github.com/xhDeng19/Number-of-the-same-color-edge-K4-in-the-complete-graph/blob/main/pics/number_of_k4s_graph.png?raw=true)

## 完整代码
```
#include <cstdio>
#include <cmath>

const int N = 200;
const int K = N * (N - 1) * (N - 2) * (N - 3) / 24;

enum COLOR {INIT, BLACK, WHITE};

struct value_pair
{
	int start, end;
	COLOR color = INIT;	
};

struct k4_graph
{
	value_pair edge_pair[6];
	double weight = pow(2, -5);
};

struct node
{
	k4_graph* k4 = NULL;
	node* next_node = NULL;	
};

node* edge[N][N];
double total_weight = K * pow(2, -5);

void init_edge()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = i + 1; j < N; j++)
		{
			edge[i][j] = new node;
		}
	}
}

void add_k4(int i1, int i2, int i3, int i4)
{
	k4_graph* tmp_k4 = new k4_graph;
	tmp_k4->edge_pair[0].start = i1;
	tmp_k4->edge_pair[0].end = i2;
	tmp_k4->edge_pair[1].start = i1;
	tmp_k4->edge_pair[1].end = i3;
	tmp_k4->edge_pair[2].start = i1;
	tmp_k4->edge_pair[2].end = i4;
	tmp_k4->edge_pair[3].start = i2;
	tmp_k4->edge_pair[3].end = i3;
	tmp_k4->edge_pair[4].start = i2;
	tmp_k4->edge_pair[4].end = i4;
	tmp_k4->edge_pair[5].start = i3;
	tmp_k4->edge_pair[5].end = i4;
	
	node* tmp_node_i1_i2 = new node;
	tmp_node_i1_i2->k4 = tmp_k4;
	tmp_node_i1_i2->next_node = edge[i1][i2]->next_node;
	edge[i1][i2]->next_node = tmp_node_i1_i2;
	
	node* tmp_node_i1_i3 = new node;
	tmp_node_i1_i3->k4 = tmp_k4;
	tmp_node_i1_i3->next_node = edge[i1][i3]->next_node;
	edge[i1][i3]->next_node = tmp_node_i1_i3;
	
	node* tmp_node_i1_i4 = new node;
	tmp_node_i1_i4->k4 = tmp_k4;
	tmp_node_i1_i4->next_node = edge[i1][i4]->next_node;
	edge[i1][i4]->next_node = tmp_node_i1_i4;
	
	node* tmp_node_i2_i3 = new node;
	tmp_node_i2_i3->k4 = tmp_k4;
	tmp_node_i2_i3->next_node = edge[i2][i3]->next_node;
	edge[i2][i3]->next_node = tmp_node_i2_i3;
	
	node* tmp_node_i2_i4 = new node;
	tmp_node_i2_i4->k4 = tmp_k4;
	tmp_node_i2_i4->next_node = edge[i2][i4]->next_node;
	edge[i2][i4]->next_node = tmp_node_i2_i4;
	
	node* tmp_node_i3_i4 = new node;
	tmp_node_i3_i4->k4 = tmp_k4;
	tmp_node_i3_i4->next_node = edge[i3][i4]->next_node;
	edge[i3][i4]->next_node = tmp_node_i3_i4;
}

void add_all_k4()
{
	for (int i1 = 0; i1 < N; i1++)
	{
		for (int i2 = i1 + 1; i2 < N; i2++)
		{
			for (int i3 = i2 + 1; i3 < N; i3++)
			{
				for (int i4 = i3 + 1; i4 < N; i4++)
				{
					add_k4(i1, i2, i3, i4);
				}
			}
		}
	}
}

void change_edge_color_in_k4(int s, int e, k4_graph* k4, COLOR edge_color)
{
	for (int i = 0; i < 6; i++)
	{
		if (k4->edge_pair[i].start == s && k4->edge_pair[i].end == e)
		{
			k4->edge_pair[i].color = edge_color;
			return;
		}
	}
}

double calculate_k4_weight(k4_graph* k4)
{
	int white_amount = 0, black_amount = 0;
	for (int i = 0; i < 6; i++)
	{
		if (k4->edge_pair[i].color == WHITE)
		{
			white_amount++;
		}
		if (k4->edge_pair[i].color == BLACK)
		{
			black_amount++;
		}
	}
	
	if (white_amount > 0 && black_amount > 0)
	{
		return 0;
	}
	
	if (white_amount == 0 && black_amount == 0)
	{
		return pow(2, -5);
	}
	
	return pow(2, -6 + white_amount + black_amount);
}

double calculate_edge_color(int s, int e, COLOR edge_color)
{
	double color_weight = total_weight; 
	node* cur_node = edge[s][e]->next_node;
	while (cur_node)
	{
		color_weight -= cur_node->k4->weight;
		change_edge_color_in_k4(s, e, cur_node->k4, edge_color);
		color_weight += calculate_k4_weight(cur_node->k4);
		
		cur_node = cur_node->next_node;
	}
	return color_weight;
}

void set_edge_color(int s, int e, COLOR edge_color)
{
	node* cur_node = edge[s][e]->next_node;
	while (cur_node)
	{
		change_edge_color_in_k4(s, e, cur_node->k4, edge_color);
		cur_node->k4->weight = calculate_k4_weight(cur_node->k4);
		
		cur_node = cur_node->next_node;
	}
}

void color_edge(int s, int e)
{
	double white_weight = calculate_edge_color(s, e, WHITE);
	double black_weight = calculate_edge_color(s, e, BLACK);
	if (white_weight <= black_weight)
	{
		set_edge_color(s, e, WHITE);
		total_weight = white_weight;
	}
	else
	{
		set_edge_color(s, e, BLACK);
		total_weight = black_weight;
	}
}

void color_all_edges()
{
	for (int s = 0; s < N; s++)
	{
		for (int e = s + 1; e < N; e++)
		{
			color_edge(s, e);
		}
	}
}

void print_edge_color(int s, int e)
{
	printf("(%d, %d): ", s, e);
	node* cur_node = edge[s][e]->next_node;
	for (int i = 0; i < 6; i++)
	{
		if (cur_node->k4->edge_pair[i].start == s && cur_node->k4->edge_pair[i].end == e)
		{
			COLOR edge_color = cur_node->k4->edge_pair[i].color;
			if (edge_color == WHITE)
			{
				printf("white\n");
			}
			else
			{
				printf("black\n");
			}
			return;
		}
	}
}

void print_all_edges_color()
{
	for (int s = 0; s < N; s++)
	{
		for (int e = s + 1; e < N; e++)
		{
			print_edge_color(s, e);
		}
	}
}

void print_info()
{
	printf("Total number of the vertices: %d\n", N);
	printf("Total number of the k4s: %d\n", K);
	printf("Average number of the same color k4s: %lf\n", K * pow(2, -5));
	printf("ALG: Number of the same color k4s: %d\n", int(total_weight));
}

int main()
{
	init_edge();
	
	add_all_k4();
	
	color_all_edges();
	
	// print_all_edges_color();
	
	print_info();
	
	return 0;
}
```