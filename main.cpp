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
