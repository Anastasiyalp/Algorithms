#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

int INF = (int) 1e9;

using namespace std;

struct Connect															//Список связей
{
	char value;
	int weight;
	int unweight;
};

struct Edge																//Ребро
{
	char from;
	char to;
	int weight;
};

class Vertex															//Вершина
{
public:
	char key;
	bool visited = false;
	Connect *connects;
	size_t size_v = 0;

	void resize_con()													//Функция изменения размера списка инцидентных ребер
	{
		if(size_v%3 != 0 && size_v) return;
		Connect *tmp = new Connect[size_v + 3];
		for(int i = 0; i < size_v; i++)
			tmp[i] = connects[i];
		delete [] connects;
		connects = tmp;
	};
	size_t find_con(const char litter)									//Функция нахождения вершины
	{
		for(int i = 0; i < size_v; i++)
			if(connects[i].value == litter)
				return i;
		return 0;
	};
};

int comp(const Edge &val1, const Edge &val2)							//Функция сравнения ребер
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
	Directed_Graph()
	: list(new Vertex[10])
	{};

	void print()														//Функция вывода графа
	{
		for(int i = 0; i < size_l; i++)
		{
			cout << list[i].key << " : ";
			for(int j = 0; j < list[i].size_v; j++)
				cout << list[i].connects[j].value << "(" << list[i].connects[j].weight << ") ";
			cout << endl;
		}
	};

	void push(const char key, const char value, const int weight, bool)//Функция добавляет новое ребро
	{
		find_or_add(value);
		Vertex &v = list[find_or_add(key)];
		v.resize_con();
		v.connects[v.size_v].value = value;
		v.connects[v.size_v].weight = weight;
		v.connects[v.size_v].unweight = 0;
		v.size_v++;
	};
	size_t find_or_add(const char litter) 								//Функция либо находит, либо добавляет новую вершину
	{
		for(int i = 0; i < size_l; i++)
			if(list[i].key == litter)
				return i;
		resize_list();
		list[size_l].key = litter;
		return size_l++;
	};
	void resize_list()													//Функция изменения размера списка вершин с их ребрами
	{
		if(size_l%10 != 0 || !size_l) return;

		Vertex *tmp = new Vertex[size_l + 10];
		for(int i = 0; i < size_l; i++)
		{
			tmp[i] = list[i];
				tmp[i].connects = new Connect[list[i].size_v];
			for(int j = 0; j < list[i].size_v; j++)
				tmp[i].connects[j] = list[i].connects[j];
		}
		for(int i = 0; i < size_l; i++)
			delete [] list[i].connects;
		delete [] list;
		list = tmp;
	};
	
	void unvisit()														//Функция обращает все ребра в непройденные
	{
		for(int i = 0; i < size_l; i++)
				list[i].visited = false;
	};

	int find_flow(size_t start, size_t finish, int flow)				//Функция нахождения потока
	{
		if(start == finish) return flow;								//возвращяем полученный минимум на пути
		list[start].visited = true;										//помечаем ребро посещенным
		for(size_t edge = 0; edge < list[start].size_v; edge++)			//Перебираем все инцидентные ребра вершине start
		{
			size_t to = find_or_add(list[start].connects[edge].value);	//Находим индекс инцидентной вершины в списке
			if(!list[to].visited && list[start].connects[edge].weight > 0)//Если вершина не посещена и остаточный вес ребра не нулевой, проходим
			{
				int min_result = find_flow(to, finish, min(flow, list[start].connects[edge].weight));//Полученный максимальный поток через минимальное ребро
				if(min_result > 0) 										// И если поток не нулевой, проходим
				{
					list[start].connects[edge].weight -= min_result; 	//Вычитаем поток из пропускной способности для реального ребра
					list[start].connects[edge].unweight += min_result;	//И к обратному прибавляем поток
					return min_result;									//Возвращаем результат
				}
			}
		}
		return 0;														//если не нашли поток из этой вершины вернем 0
	}
	void print_flow() 													//Функция печати ребер с их потоком
	{
		vector <Edge> vec;												//Контейнер для ребер с потоком
		for(int i = 0; i < size_l; i++)
			for(int j = 0; j < list[i].size_v; j++)
			{
				int add_weight = list[i].connects[j].weight < 0 ? 0 : list[i].connects[j].unweight;
				vec.push_back({list[i].key, list[i].connects[j].value, add_weight });
			}
		sort(vec.begin(), vec.end(), comp);								//Сортируем содержимое контейнера с помощью comp
		for_each(vec.begin(), vec.end(), [](const Edge& obj){cout << obj.from << " " << obj.to << " " << obj.weight << endl;});//Выводим
		vec.clear();													//Очистка контейнера
	};
};

int main(){
	int N;
	char start, finish;
	cin >> N >> start >> finish;           								// считываем количество ребер, начальную и конечную вершины

	Directed_Graph graf;
	char from, to;
	int weight;
	for(size_t i = 0; i < N; i++)
	{
		cin >> from >> to >> weight;
		graf.push(from, to, weight, true);								//Добавляем реальное ребро
		graf.push(from, to, weight, false);
	}

//	graf.print();														//Вывод графа
//	cout << endl;

    // Нахождение максимального потока
	int max_flow = 0;
	int iter_res; 
	while((iter_res = graf.find_flow(graf.find_or_add(start), graf.find_or_add(finish), INF)) > 0) //Пока есть путь в графе
	{
		graf.unvisit();													//Пометим ребра непройденными
		max_flow += iter_res;											//К итоговому потоку добавим найденный
	}
	cout << max_flow << endl; 											//Выводим максимальный поток
	
	graf.print_flow();													//Выводим ребра с их потоками 
	return 0;
}