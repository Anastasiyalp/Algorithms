#include <iostream>
#include <cmath>
#include <stack>
#include <ctime>

using namespace std;

struct Square{
	int x;
	int y;
	int length;
};
int multiplicity(int N){
	if(N%2 == 0)
		return 2;
	if(N%3 == 0)
		return 3;
	return 0;
}
void demo(int **arr,int N){
	for(int m = 0; m < N; m++){
		for(int l = 0; l < N; l++)
			cout << arr[m][l]<< ' ';
		cout << endl;
	}
	cout << endl;
}
void fill_square(int **arr, int x ,int y, int length_square){
	for(int i = 0; i < length_square; i++)
		for(int j = 0; j < length_square; j++)
			arr[x+i][y+j] = length_square;
}
void stack_copy(stack <Square> * sqrs, stack <Square> * sqrs_copy){
	while(!sqrs->empty()){
		sqrs_copy->push(sqrs->top());
		sqrs->pop();
	}
}
void find_empty_cell(int **arr, int &x, int &y, int N){
	for(x = 0; x < N; x++){
		for(y = 0; y < N - 1; y++)
			if(arr[x][y] == 0)
				break;
		if(arr[x][y] == 0)
			break;
	}
	if(x == N) x = N-1;
}
int find_min_sqrs(int  limit, stack <Square> *sqrs,int **arr, int N, int count_rec){

	if(limit <= count_rec) return 0;

	demo(arr, N);

	int x, y;
	find_empty_cell(arr, x, y, N);
	if(arr[x][y] !=0 ) return 0;

	stack <Square> sqrs_tmp;
	stack <Square> sqrs_max;

	int distance_to_the_border = (N-x > N-y) ? (N-y-!arr[0][0]) : (N-x-!arr[0][0]);

	int length_square, was = 0, k_min = N*N, k, need_length = 1;

	for(length_square = 1; length_square <= distance_to_the_border; length_square++){
		was = 0;

		fill_square(arr, x ,y, length_square);

		k = find_min_sqrs(limit, &sqrs_tmp, arr, N, count_rec+1)+1;
		if(k < k_min){
			k_min = k;
			need_length = length_square;
			while(!sqrs_max.empty())
				sqrs_max.pop();
			stack_copy(&sqrs_tmp, &sqrs_max);
		}

		for(int i = 0; i < length_square; i++)
			for(int j = 0; j < length_square; j++)
				arr[x+i][y+j] = 0;

		demo(arr, N);

		if(distance_to_the_border - length_square - 1 < 0) break;
		int next_length_square = length_square + 1;
		for(int i = 0; i < next_length_square; i++){
			if(arr[x + next_length_square - 1][y + i] || arr[x + i][y + next_length_square - 1])
				{was++; break;}
		}
		if(was) break;
	}
	while(!sqrs->empty())
		sqrs->pop();
	stack_copy(&sqrs_max, sqrs);
	sqrs->push({x+1, y+1, need_length});
	return k_min;
}

int main(){
	int N, min_k;
	cout << "Введите длину квадрата, для которого вы хотите получить минимальное распределение" << endl;
	cin >> N;

	unsigned int time1 = clock();

	if(!(N>=2 && N<=40)){
		cout << "Ошибка, введено неверное число." << endl; return 0;
	}

	int **arr = new int *[N];
	for(int i = 0; i < N; i++)
		arr[i] = new int[N];
	for(int m = 0; m < N; m++)
		for(int l = 0; l < N; l++)
			arr[m][l] = 0;
	stack <Square> sqrs;
	int limit = 6.5 * sqrt(sqrt(N)); //Задаем предельную величину для количества квадратов

	switch(multiplicity(N)) {
		case 2:{
			min_k = 4;
			int half = N/2;
			sqrs.push({half + 1, half + 1, half});
			sqrs.push({1, half + 1, half});
			sqrs.push({half + 1, 1, half});
			sqrs.push({1, 1, half});
			break;
		}
		case 3:{
			min_k = 6;
			int third = N/3;
			sqrs.push({third*3, third*3, third});
			sqrs.push({third*3, 1, third});
			sqrs.push({1, third*3, third});
			sqrs.push({third*2, 1, third});
			sqrs.push({1, third*2, third});
			sqrs.push({1, 1, third*2});
			break;
		}
		default:{
			int half = N-N/2;
			fill_square(arr, 0, 0, half);
			fill_square(arr, half, 0, half - 1);
			fill_square(arr, 0, half, half - 1);

			demo(arr, N);

			int quarter = half/2;
			fill_square(arr, N - N/4 -1, N - N/4 -1, half - quarter);
			fill_square(arr, N - half, N - quarter, quarter);
			fill_square(arr, N - quarter, N - half, quarter);

			demo(arr, N);

			min_k = find_min_sqrs(limit, &sqrs, arr, N, 0) + 6;
			sqrs.push({N - N/4, N - N/4, half - quarter});
			sqrs.push({N - half + 1, N - quarter + 1, quarter});
			sqrs.push({N - quarter + 1, N - half + 1, quarter});

			sqrs.push({half + 1, 1, half - 1});
			sqrs.push({1, half + 1, half - 1});
			sqrs.push({1, 1, half});
		}
	}

	Square tmp;
	cout << "Минимальное распределение квадратов:"
	for(int j = 0; j < min_k; j++){
		tmp = sqrs.top();
		cout << "x = " << tmp.x << ", y = " << tmp.y << ", длина = " << tmp.length << endl;
		sqrs.pop();
	}
	cout << "Количество = " << min_k << endl;

	unsigned int time2 = clock();
	cout << "TIME : " << time2-time1 << endl;

	for(int i = 0; i < N; i++)
			delete [] arr[i];
	delete [] arr;
	return 0;
}
