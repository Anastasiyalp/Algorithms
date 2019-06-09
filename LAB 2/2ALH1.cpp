#include <cmath>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <stack>

#define FINISH -1
#define WAS 1
#define WASNT 0

#define type char

#define DEMO
#define SIZE 10

using namespace std;

class Directed_Graph{
private:
	type *list;
    bool *was;
	size_t size_l = 0;
	double **matrix;

public:
	Directed_Graph()
	: list(new type[SIZE]), was(new bool[SIZE]())
	{
		matrix = new double*[SIZE];
		for(int i = 0; i < SIZE; i++)
			matrix[i] = new double[SIZE]();
	}

	void print()
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

	void push(const type key, const type value, const float weight)
	{
		size_t k = find_or_add(key);
		size_t v = find_or_add(value);
		matrix[k][v] = weight;
	}

	size_t find_or_add(const type litter)
	{
		for(int i = 0; i < size_l; i++)
			if(list[i] == litter)
				return i;
		resize_list();
		list[size_l] = litter;
		return size_l++;
	}

	void resize_list()
	{
		if(size_l%SIZE != 0 || !size_l) return;

		type *new_list = new type[size_l + SIZE];
		for(int i = 0; i < size_l; i++)
			new_list[i] = list[i];
		delete [] list;
		list = new_list;

		bool *new_was = new bool[size_l + SIZE]();
		for(int i = 0; i < size_l; i++)
			new_was[i] = was[i];
		delete [] was;
		was = new_was;

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


	bool find_min(const size_t ver, size_t &min)
	{
		min = -1;
		for(int i = 0; i < size_l; i++)
			if(matrix[ver][i] && !was[i]){
				min = i;
				break;
			}
		cout << "Поиск минимального ребра для текущей вершины (" << list[ver] <<"):" << endl;
		if(min == -1)
		{
			cout << "Доступных вершин из текущей больше нет." << endl;
			return 0;
		}
		cout << "Список доступных:" << endl;
		for(int i = 0; i < size_l; i++)
		{
			if(matrix[ver][i] && !was[i])
				cout << list[i] << " (" << matrix[ver][i] << ") ";
			if(matrix[ver][i] && !was[i] && matrix[ver][i] < matrix[ver][min] && matrix[ver][i] != matrix[ver][min])
				min = i;
		}
		cout << endl << "Минимальная из всех - " << list[min] << " (" << matrix[ver][min] << ")" << endl; 
		was[min] = 1;
		return 1;
	}
    int find_way(const size_t start, const size_t finish, deque <char> &way)
	{
		cout << "Добавляем в путь вершину :" <<  list[start] << endl;
		way.push_back(list[start]);
		if(start == finish)
		{
			cout << "Путь найден." << endl;
			return FINISH;
		}
		size_t min;
		int result;
		while(find_min(start, min))
		{
			result = find_way(min, finish, way);
			if(result == FINISH)
				return FINISH;
			way.pop_back();
		}
		return 0;
	};
};

int main(){
	
    setlocale(LC_ALL, "rus");
	char start, finish;
	cin >> start >> finish;

	Directed_Graph graf;
	char from, to;
	float weight;
	while(cin >> from >> to >> weight)
	{
		if(from == '^')
			break;
		graf.push(from, to, weight);
	}

	cout << "Полученный граф :" << endl;
	graf.print();
	deque <char> way;
	cout << "Поиск пути :" << endl;
	if(!graf.find_way(graf.find_or_add(start), graf.find_or_add(finish), way))
		return 0;
	cout << endl;
	cout << "Найденный путь :" << endl;

	while(!way.empty())
	{
		cout << way.front();
		way.pop_front();
	}
	return 0;
}