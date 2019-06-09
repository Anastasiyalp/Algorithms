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

struct Vertex //Структура вершины
{
	type value;
	double weight;
	type from;
};

class Directed_Graph{ //Класс графа
private:
	type *list; //Список вершин
	size_t size_l = 0; //Размер этого списка
	double **matrix; //Матрица смежности

public:
	Directed_Graph() //Инициализация графа 
	: list(new type[SIZE])
	{
		matrix = new double*[SIZE];
		for(int i = 0; i < SIZE; i++)
			matrix[i] = new double[SIZE]();
	}

	void print() //Распечатка графа
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

	void push(const type key, const type value, const float weight) //Добавление вершины 
	{
		size_t k = find_or_add(key);
		size_t v = find_or_add(value);
		matrix[k][v] = weight;
	}

	size_t find_or_add(const type litter)//Нахождение или добавление индекса вершины 
	{
		for(int i = 0; i < size_l; i++)
			if(list[i] == litter)
				return i;
		resize_list();
		list[size_l] = litter;
		return size_l++;
	}

	void resize_list() //Изменение размера списка вершин
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

	bool in_arr(type c, vector <Vertex> &arr) //Возвращает информацию о нахождении символа в массиве
	{
		for(int i = 0; i < arr.size(); i++)
			if(arr[i].value == c)
				return 1;
		return 0;
	};

	size_t index_in_arr(type c, vector <Vertex> &arr) //Возвращает индекс нахождения символа в массиве
	{
		for(size_t i = 0; i < arr.size(); i++)
			if(arr[i].value == c)
				return i;
		return 0;
	};

	size_t min_arr(vector <Vertex> &arr, size_t finish) //Возвращает минимальный символ в массиве в соответствии в эвристикой
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

	void find_way(size_t start, size_t finish, stack <type> &way) //Находиждение путь алгоритмом А*
	{
		#ifdef DEMO
			cout <<	"В ожидающие добавляется точка старта (" << list[start] << "), список рассмотренных вершин пока пуст."  << endl;
		#endif

		vector <Vertex> arr_wait;//Ожидаемые вершины
		vector <Vertex> arr_past;//Просмотренные вершины

		arr_wait.push_back({list[start], 0, 0});//Добавляем первую вершину
		
		Vertex X, Y; 
		while(!arr_wait.empty())//Пока есть вершины в списке ожидания
		{

			vector <Vertex>::iterator it = arr_wait.begin();
			size_t min = min_arr(arr_wait, finish);//Достаем минимальный элемент
			advance(it, min);
			X = arr_wait.at(min);//Помещаем в Х 
			#ifdef DEMO
				cout << "Список на рассмотрение :" << endl;
				for(int i = 0; i < arr_wait.size(); i++) 
					cout << arr_wait[i].value << " - F(" << arr_wait[i].weight  + abs(arr_wait[i].value - list[finish]) << "), ";
				cout << endl;
			#endif
			arr_wait.erase(it); //Извлекаем элемент Х из массива вершин на ожидание
		
			#ifdef DEMO
				cout << "Наименьшая точка из списка на рассмотрение: " << X.value << endl;
				cout << "Перейдем к ней." << endl << endl;	
			#endif
		
			arr_past.push_back(X); //Помещаем элемент Х в массив просмотренных вершин
			if(X.value == list[finish]) //Если дошли до финиша, взвращаемся
				break;

			#ifdef DEMO
				cout << "Инцидентные вершины для (" << X.value << "):" << endl; 
			#endif

			for(int i = 0; i < size_l; i++)//Перебираем инцидентные вершины для Х
			{
				
				size_t index_x = find_or_add(X.value);//Находим индекс Х
				if(matrix[index_x][i] != 0) //Если существует путь
				{
					#ifdef DEMO
						cout << list[i] << " - G(" << matrix[index_x][i] + X.weight << ") H(" << abs(list[i] - list[finish]) << ") F(";
						cout << matrix[index_x][i] + X.weight + abs(list[i] - list[finish]) << ")  - "; 
					#endif
					Y = {list[i], matrix[index_x][i] + X.weight, X.value}; //Создаем элемент для массива
					if(in_arr(Y.value, arr_past)) //Если вершина уже в пройденном списке, то продолжаем
					{
						#ifdef DEMO
							cout << "Вершина уже находится в рассмотренном списке, пропускаем ее." << endl; 
						#endif

						continue;
					}
					if(!in_arr(Y.value, arr_wait)) //Если вершины нет в списке на ожидание, то добавляем ее туда
					{
						#ifdef DEMO
							cout << "Вершины нет рассмотренном списке и в списке ожидания. Добавляем ее в список на рассмотрение." << endl; 
						#endif

						arr_wait.push_back(Y);
					}
					else //Иначе обновляем У в списке на ожидание
					{
						#ifdef DEMO
							cout << "Вершины нет рассмотренном списке но есть списке ожидания." << endl; 
						#endif

						Vertex &old_Y = arr_wait[index_in_arr(Y.value, arr_wait)];
						if(Y.weight < old_Y.weight)
						{
							#ifdef DEMO
								cout << "Новый путь до вершины лучше предыдущего. Заменяем предыдущий им." << endl; 
							#endif

							old_Y.weight = Y.weight;
							old_Y.from = Y.from;
						}
						#ifdef DEMO
						else
							cout << "Новый путь до вершины не лучше предыдущего, ничего не делаем." << endl; 
						#endif

					}
				}
			}
		}
		if(!in_arr(list[finish], arr_past)) //Если не дошли до финиша, то заканчиваем
			return;
		
		#ifdef DEMO
			cout << "Маршрут найден." << endl;
		#endif
		
		Vertex *ptr = &X;
		while(ptr->value != list[start])//Находим путь
		{
			way.push(ptr->value);
			ptr = &arr_past[index_in_arr(ptr->from, arr_past)];
		}
		way.push(ptr->value);
	}

};
//Вывести стоимости пути в алг.
int main(){
     setlocale(LC_ALL, "rus");

	type start, finish;
	cin >> start >> finish;

	Directed_Graph graf;
	type from, to;
	double weight;
	while(cin >> from >> to >> weight)//Считыние данных
	{
		if(from == '^')
			break;
		graf.push(from, to, weight);
	}
	#ifdef DEMO
		cout << "Полученный граф :" << endl;
		graf.print();
		cout << endl;
	#endif
	stack <type> way;
	graf.find_way(graf.find_or_add(start), graf.find_or_add(finish), way);

	#ifdef DEMO
		cout << "Найденный путь от " << start << " до " << finish << " : ";
	#endif
	while(!way.empty())
	{
		cout << way.top() <<" ";
		way.pop();
	}
	cout << endl;

	return 0;
}