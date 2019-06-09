#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

int INF = (int) 1e9;

using namespace std;

struct Connect															//������ ������
{
	char value;
	int weight;
	bool real;
};

struct Edge																//�����
{
	char from;
	char to;
	int weight;
};

class Vertex															//�������
{
public:
	char key;
	bool visited = false;
	Connect *connects;
	size_t size_v = 0;

	void resize_con()													//������� ��������� ������� ������ ����������� �����
	{
		if(size_v%5 != 0 && size_v) return;
		Connect *tmp = new Connect[size_v + 5];
		for(int i = 0; i < size_v; i++)
			tmp[i] = connects[i];
		if(size_v) delete [] connects;
		connects = tmp;
	};
	size_t find_con(const char litter)									//������� ���������� �������
	{
		for(int i = 0; i < size_v; i++)
			if(connects[i].value == litter)
				return i;
		return 0;
	};
};

int comp(const Edge &val1, const Edge &val2)							//������� ��������� �����
{
	if(val1.from == val2.from)
		return val1.to < val2.to;
	return val1.from < val2.from;
};

class Directed_Graph{
private:
	Vertex *list;
	size_t size_l = 0;
public:
	~Directed_Graph()
	{
		for(size_t i = 0; i < size_l; i++)
			delete [] list[i].connects;
		delete [] list;
	}
	Directed_Graph()
	: list(new Vertex[3])
	{};

	void print()														//������� ������ �����
	{
		for(int i = 0; i < size_l; i++)
		{
			cout << list[i].key << " : ";
			for(int j = 0; j < list[i].size_v; j++)
				if(list[i].connects[j].real)
					cout << list[i].connects[j].value << "(" << list[i].connects[j].weight << ") ";
			cout << endl;
		}
	};

	void push(const char key, const char value, const int weight, bool real)//������� ��������� ����� �����
	{
		find_or_add(value);
		Vertex &v = list[find_or_add(key)];
		v.resize_con();
		v.connects[v.size_v].value = value;
		v.connects[v.size_v].weight = weight;
		v.connects[v.size_v].real = real;
		v.size_v++;
	};
	size_t find_or_add(const char litter) 								//������� ���� �������, ���� ��������� ����� �������
	{
		for(int i = 0; i < size_l; i++)
			if(list[i].key == litter)
				return i;
		resize_list();
		list[size_l].key = litter;
		return size_l++;
	};
	void resize_list()													//������� ��������� ������� ������ ������ � �� �������
	{
		if(size_l%3 != 0 || !size_l) return;

		Vertex *tmp = new Vertex[size_l + 3];
		
		for(int i = 0; i < size_l; i++)
		{
			tmp[i] = list[i];
			tmp[i].connects = new Connect[list[i].size_v];
			tmp[i].connects = list[i].connects;
		}
		list = tmp;
		for(int i = 0; i < size_l; i++)
			list[i].connects = tmp[i].connects;
	};
	
	void unvisit()														//������� �������� ��� ����� � ������������
	{
		for(int i = 0; i < size_l; i++)
				list[i].visited = false;
	};

	int find_flow(size_t start, size_t finish, int flow, int count_rec)				//������� ���������� ������
	{
		for(int k = 0; k < count_rec; k++)
			cout << "\t"; 
		cout << "�������� � ������� (" << list[start].key << ")." << endl;
		if(start == finish)
		{	
			for(int k = 0; k < count_rec; k++)
				cout << "\t"; 
			cout << "����� �� �����." << endl;
			return flow;												//���������� ���������� ������� �� ����
		}
		list[start].visited = true;										//�������� ����� ����������
		for(int k = 0; k < count_rec; k++)
			cout << "\t"; 
		cout << "���������� ��� ����������� ������� ��� (" << list[start].key <<"):" << endl;
		for(size_t edge = 0; edge < list[start].size_v; edge++)			//���������� ��� ����������� ����� ������� start
		{
			size_t to = find_or_add(list[start].connects[edge].value);	//������� ������ ����������� ������� � ������
			for(int k = 0; k < count_rec; k++)
				cout << "\t"; 
			cout << edge + 1 << ") ������� (" << list[to].key << ") � ���������� ���������� ������������ - " << list[start].connects[edge].weight << endl;
			if(!list[to].visited && list[start].connects[edge].weight > 0)//���� ������� �� �������� � ���������� ��� ����� �� �������, ��������
			{ 
				int min_result = find_flow(to, finish, min(flow, list[start].connects[edge].weight), count_rec + 1);//���������� ������������ ����� ����� ����������� �����
				for(int k = 0; k < count_rec; k++)
					cout << "\t";
				cout << "���������� ����� - " << min_result << endl;
				if(min_result > 0)										//� ���� ����� �� �������, ��������
				{
					for(int k = 0; k < count_rec; k++)
						cout << "\t";
					cout << "�������� �� ������� ���������� ����������� ��������� ����� �����: " << list[start].connects[edge].weight 
					<< " - " << min_result << " = " << list[start].connects[edge].weight - min_result << endl;
					list[start].connects[edge].weight -= min_result; 	//�������� ����� �� ���������� ����������� ��� ��������� �����
					//��������
					size_t con = list[to].find_con(list[start].key);	//������� ������ ��������� �����
					for(int k = 0; k < count_rec; k++)
						cout << "\t";
					cout << "��������� � �������� ������� ����� �����: " << list[to].connects[con].weight 
					<< " + " << min_result << " = " << list[to].connects[con].weight + min_result << endl;
					list[to].connects[con].weight += min_result;		//� � ��������� ���������� �����
					return min_result;									//���������� ���������
				}
			}
			else
			{
				for(int k = 0; k < count_rec; k++)
				cout << "\t"; 
				if(list[to].visited)
					cout << "������� (" << list[to].key <<") ��� ��������." << endl;
				else
					cout << "������������� ���������� �����������." << endl;
			}
		}
		return 0;														//���� �� ����� ����� �� ���� ������� ������ 0
	}
	void print_flow() 													//������� ������ ����� � �� �������
	{
		vector <Edge> vec;												//��������� ��� ����� � �������
		for(int i = 0; i < size_l; i++)
			for(int j = 0; j < list[i].size_v; j++)
				if(!list[i].connects[j].real)							//���������, ���� ����� ������
					vec.push_back({list[i].connects[j].value, list[i].key, abs(list[i].connects[j].weight < 0 ? 0 : list[i].connects[j].weight) });
		sort(vec.begin(), vec.end(), comp);								//��������� ���������� ���������� � ������� comp
		for_each(vec.begin(), vec.end(), [](const Edge& obj){cout << obj.from << " " << obj.to << " " << obj.weight << endl;});//�������
		if(!vec.empty()) vec.clear();													//������� ����������
	};
};

int main(){
    setlocale(LC_ALL, "rus");

	int N;
	char start, finish;
	cin >> N >> start >> finish;           								// ��������� ���������� �����, ��������� � �������� �������

	Directed_Graph graf;
	char from, to;
	int weight;
	for(size_t i = 0; i < N; i++)
	{
		cin >> from >> to >> weight;
		graf.push(from, to, weight, true);								//��������� �������� �����
		graf.push(to, from, 0, false);									//��������� ������ �����
	}
	cout << "������������� ����� � ���� �������:" << endl;
	graf.print();														//����� �����
	cout << endl;

    // ���������� ������������� ������
	cout << "���������� ������������� ������." << endl;
	int max_flow = 0;
	int iter_res; 
	int i = 1;
	cout << "____���� #" << i++ << endl;
	while((iter_res = graf.find_flow(graf.find_or_add(start), graf.find_or_add(finish), INF, 0)) > 0) //���� ���� ���� � �����
	{
		cout << "____���� #" << i++ << endl;
		graf.unvisit();													//������� ����� �������������
		max_flow += iter_res;											//� ��������� ������ ������� ���������
	}
	cout << max_flow << endl; 											//������� ������������ �����
	
	graf.print_flow();													//������� ����� � �� �������� 
	return 0;
}