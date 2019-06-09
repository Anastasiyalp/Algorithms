#include <cmath>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <stack>

#define FINISH 0
#define WAS 1
#define WASNT 0

#define type int

#define DEMO
#define SIZE 10

using namespace std;

struct Vertex //��������� �������
{
	type value;
	double weight;
	type from;
};

class Directed_Graph{ //����� �����
private:
	type *list; //������ ������
	size_t size_l = 0; //������ ����� ������
	double **matrix; //������� ���������

public:
	Directed_Graph() //������������� ����� 
	: list(new type[SIZE])
	{
		matrix = new double*[SIZE];
		for(int i = 0; i < SIZE; i++)
			matrix[i] = new double[SIZE]();
	}

	void print() //���������� �����
	{
		for(int i = 0; i < size_l; i++)
		{
			cout << list[i] << " : ";
			for(int j = 0; j < size_l; j++)
				if(matrix[i][j] != 0)
					cout << list[j] << "(" << matrix[i][j] << ") ";
			cout << endl;
		}
	}

	void push(const type key, const type value, const float weight) //���������� ������� 
	{
		size_t k = find_or_add(key);
		size_t v = find_or_add(value);
		matrix[k][v] = weight;
	}

	size_t find_or_add(const type litter)//���������� ��� ���������� ������� ������� 
	{
		for(int i = 0; i < size_l; i++)
			if(list[i] == litter)
				return i;
		resize_list();
		list[size_l] = litter;
		return size_l++;
	}

	void resize_list() //��������� ������� ������ ������
	{
		if(size_l%SIZE != 0 || !size_l) return;

		type *new_list = new type[size_l + SIZE];
		for(int i = 0; i < size_l; i++)
			new_list[i] = list[i];
		delete [] list;
		list = new_list;

		double **new_matrix = new double*[size_l + SIZE];
		for(int i = 0; i < size_l + SIZE; i++)
			new_matrix[i] = new double[size_l + SIZE]();

		for(int i = 0; i < size_l; i++)
		{
			for(int j = 0; j < size_l; j++)
				new_matrix[i][j] = matrix[i][j];
			delete [] matrix[i];
		}
		delete [] matrix;
		matrix = new_matrix;
	};

	bool in_arr(type c, vector <Vertex> &arr) //���������� ���������� � ���������� ������� � �������
	{
		for(int i = 0; i < arr.size(); i++)
			if(arr[i].value == c)
				return 1;
		return 0;
	};

	size_t index_in_arr(type c, vector <Vertex> &arr) //���������� ������ ���������� ������� � �������
	{
		for(size_t i = 0; i < arr.size(); i++)
			if(arr[i].value == c)
				return i;
		return 0;
	};

	size_t min_arr(vector <Vertex> &arr, size_t finish) //���������� ����������� ������ � ������� � ������������ � ����������
	{
		size_t min = 0;
		for(int i = 0; i < arr.size(); i++)
		{
			if(arr[i].weight + abs(arr[i].value - list[finish]) < arr[min].weight + abs(arr[min].value - list[finish]))
				min	= i;
			if(arr[i].weight + abs(arr[i].value - list[finish]) == arr[min].weight + abs(arr[min].value - list[finish]))
				if(abs(arr[i].value - list[finish]) <  abs(arr[min].value - list[finish]))
					min = i;
		}
		return min;		
	}

	void find_way(size_t start, size_t finish, stack <type> &way) //������������ ���� ���������� �*
	{
		#ifdef DEMO
			cout <<	"� ��������� ����������� ����� ������ (" << list[start] << "), ������ ������������� ������ ���� ����."  << endl;
		#endif

		vector <Vertex> arr_wait;//��������� �������
		vector <Vertex> arr_past;//������������� �������

		arr_wait.push_back({list[start], 0, 0});//��������� ������ �������
		
		Vertex X, Y; 
		while(!arr_wait.empty())//���� ���� ������� � ������ ��������
		{

			vector <Vertex>::iterator it = arr_wait.begin();
			size_t min = min_arr(arr_wait, finish);//������� ����������� �������
			advance(it, min);
			X = arr_wait.at(min);//�������� � � 
			#ifdef DEMO
				cout << "������ �� ������������ :" << endl;
				for(int i = 0; i < arr_wait.size(); i++) 
					cout << arr_wait[i].value << " - F(" << arr_wait[i].weight  + abs(arr_wait[i].value - list[finish]) << "), ";
				cout << endl;
			#endif
			arr_wait.erase(it); //��������� ������� � �� ������� ������ �� ��������
		
			#ifdef DEMO
				cout << "���������� ����� �� ������ �� ������������: " << X.value << endl;
				cout << "�������� � ���." << endl << endl;	
			#endif
		
			arr_past.push_back(X); //�������� ������� � � ������ ������������� ������
			if(X.value == list[finish]) //���� ����� �� ������, �����������
				break;

			#ifdef DEMO
				cout << "����������� ������� ��� (" << X.value << "):" << endl; 
			#endif

			for(int i = 0; i < size_l; i++)//���������� ����������� ������� ��� �
			{
				
				size_t index_x = find_or_add(X.value);//������� ������ �
				if(matrix[index_x][i] != 0) //���� ���������� ����
				{
					#ifdef DEMO
						cout << list[i] << " - G(" << matrix[index_x][i] + X.weight << ") H(" << abs(list[i] - list[finish]) << ") F(";
						cout << matrix[index_x][i] + X.weight + abs(list[i] - list[finish]) << ")  - "; 
					#endif
					Y = {list[i], matrix[index_x][i] + X.weight, X.value}; //������� ������� ��� �������
					if(in_arr(Y.value, arr_past)) //���� ������� ��� � ���������� ������, �� ����������
					{
						#ifdef DEMO
							cout << "������� ��� ��������� � ������������� ������, ���������� ��." << endl; 
						#endif

						continue;
					}
					if(!in_arr(Y.value, arr_wait)) //���� ������� ��� � ������ �� ��������, �� ��������� �� ����
					{
						#ifdef DEMO
							cout << "������� ��� ������������� ������ � � ������ ��������. ��������� �� � ������ �� ������������." << endl; 
						#endif

						arr_wait.push_back(Y);
					}
					else //����� ��������� � � ������ �� ��������
					{
						#ifdef DEMO
							cout << "������� ��� ������������� ������ �� ���� ������ ��������." << endl; 
						#endif

						Vertex &old_Y = arr_wait[index_in_arr(Y.value, arr_wait)];
						if(Y.weight < old_Y.weight)
						{
							#ifdef DEMO
								cout << "����� ���� �� ������� ����� �����������. �������� ���������� ��." << endl; 
							#endif

							old_Y.weight = Y.weight;
							old_Y.from = Y.from;
						}
						#ifdef DEMO
						else
							cout << "����� ���� �� ������� �� ����� �����������, ������ �� ������." << endl; 
						#endif

					}
				}
			}
		}
		if(!in_arr(list[finish], arr_past)) //���� �� ����� �� ������, �� �����������
			return;
		
		#ifdef DEMO
			cout << "������� ������." << endl;
		#endif
		
		Vertex *ptr = &X;
		while(ptr->value != list[start])//������� ����
		{
			way.push(ptr->value);
			ptr = &arr_past[index_in_arr(ptr->from, arr_past)];
		}
		way.push(ptr->value);
	}

};
//������� ��������� ���� � ���.
int main(){
     setlocale(LC_ALL, "rus");

	type start, finish;
	cin >> start >> finish;

	Directed_Graph graf;
	type from, to;
	double weight;
	while(cin >> from >> to >> weight)//�������� ������
	{
		if(from == '^')
			break;
		graf.push(from, to, weight);
	}
	#ifdef DEMO
		cout << "���������� ���� :" << endl;
		graf.print();
		cout << endl;
	#endif
	stack <type> way;
	graf.find_way(graf.find_or_add(start), graf.find_or_add(finish), way);

	#ifdef DEMO
		cout << "��������� ���� �� " << start << " �� " << finish << " : ";
	#endif
	while(!way.empty())
	{
		cout << way.top() <<" ";
		way.pop();
	}
	cout << endl;

	return 0;
}