#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <ctime>


using namespace std;
ofstream fout("cppstudio.txt");

struct Square{ //��������� ��������
	int x;
	int y;
	int length;
};
int multiplicity(int N){//�������� ���������
	if(N%2 == 0 && N!=2)
		return 2;
	if(N%3 == 0 && N!=3)
		return 3;
	if(N%5 == 0 && N!=5)
		return 5;
	return 1;
}
void demo(int **arr, int N){//���������� ��������
	fout << "������� ���������:" << endl;
	for(int m = 0; m < N; m++){
		for(int l = 0; l < N; l++)
			fout << arr[m][l]<< ' ';
		fout << endl;
	}
	fout << endl;
}
void fill_square(int **arr, int x , int y, int length_square){//������������ �������
	for(int i = 0; i < length_square; i++)
		for(int j = 0; j < length_square; j++)
			arr[x+i][y+j] = length_square;
}
void clear_square(int **arr, int x, int y, int length_square){//������� �������
	for(int i = 0; i < length_square; i++)
		for(int j = 0; j < length_square; j++)
			arr[x+i][y+j] = 0;
}
int more_square(int **arr, int x, int y, int length_square){//��������� ����������� �������� ������� ������������ ����� 
	for(int i = 0; i < length_square; i++)
		if(arr[x + length_square - 1][y + i] || arr[x + i][y + length_square - 1])
			return 1;
	return 0;	
}
void stack_copy(stack <Square> * sqrs, stack <Square> * sqrs_copy){//���������� �����
	while(!sqrs->empty()){
		sqrs_copy->push(sqrs->top());
		sqrs->pop();
	}
}
int find_empty_cell(int **arr, int &x, int &y, int N){ // ���������� ������ ������
	for(x = 0; x < N; x++){
		for(y = 0; y < N - 1; y++)
			if(arr[x][y] == 0)
				break;
		if(arr[x][y] == 0)
			break;
	}
	if(x == N) x = N-1;
	if(arr[x][y] != 0)
		return 1;
	return 0;
}
void empty_stack(stack <Square> *sqrs){
	while(!sqrs->empty())
		sqrs->pop();
}

int find_min_sqrs(int limit, stack <Square> *sqrs, int **arr, int N, int count_rec, int scale){ //������� ���������� ����������� ������������� ��������

	if(limit < count_rec) //������� ������ �� �������
	{
		fout << endl << endl << "����� �� �������." << endl;
		return -1;
	}
	else
		fout << "�������� ��������� �������." << endl;

	demo(arr, N); //����� ������������� ��������

	int x, y;
	if(find_empty_cell(arr, x, y, N)) // ���������� ������ ������
	{
		fout << "������ ������ �� ��������." << endl;
		return 0;
	}
	else
		fout << "������� ������ ������: � = " << x << ", y = "<< y << endl;
	stack <Square> sqrs_tmp; //��������� ���� ��������� 
	stack <Square> sqrs_max; //�������� ���� ���������


	int distance_to_the_border = (N-x > N-y) ? (N-y-!arr[0][0]) : (N-x-!arr[0][0]); //������ ���������� �� �������
	fout << "���������� �� ������ = " << distance_to_the_border << endl;
	int length_square, k_min = limit + 1, k, need_length = 1;
	fout << "������� ���� ��������:" << endl;
	for(length_square = 1; length_square <= distance_to_the_border; length_square++){ //��������� �� ����� ��������
		fout << "������� �����: " << length_square << endl;
		fout << "�������� �������. ";
		fill_square(arr, x, y, length_square); //��������� �������

		k = find_min_sqrs(limit, &sqrs_tmp, arr, N, count_rec+1, scale) + 1;//������ ���������� ����������� ���������

		if(k < k_min && k != 0){ //���� ������ ������������� � ������� ����������� ��������� �������� ���� ���������
			fout << "����� ������������� �����, ��� " << limit + 1 << ". �������� ���." << endl;
			k_min = k; //��������� ����������� ��������������
			need_length = length_square;
			while(!sqrs_max.empty()){
				sqrs_max.pop();
			}
			stack_copy(&sqrs_tmp, &sqrs_max);
		}
		else //����� ������� ��������� ����
		{
			fout << "���� ������� ������������� ��-�����, ������� ���� ���������." << endl;
			empty_stack(&sqrs_tmp);
		}
		fout << "������ ������� ������� = " << length_square << " � ����� � = " << x << ", y = " << y << "." << endl;
		clear_square(arr, x, y, length_square); //������� ������������ �������

		demo(arr, N);

		if(distance_to_the_border - length_square - 1 < 0) break;
		if(more_square(arr, x, y, length_square + 1)) // ���� ������ �������� ������� ��������, �������
		{
			fout << "������� �������� �������� �� ����������." << endl;
			break;
		}
	}
	while(!sqrs->empty())
		sqrs->pop();
	stack_copy(&sqrs_max, sqrs); //�������� ���� � ������� ����
	sqrs->push({x*scale+1, y*scale+1, need_length*scale}); //� ��������� ������� �������
	return k_min;
}

int main(){
    setlocale(LC_ALL, "rus");
	int N, min_k;
	fout << "������� ����� ��������, ��� �������� �� ������ �������� ����������� �������������" << endl;
	cin >> N;

	unsigned int time1 = clock();

	if(!(N>=2 && N<=40)){
		fout << "������, ������� �������� �����." << endl; return 0;
	}


	stack <Square> sqrs;
	int mul = multiplicity(N);
	int limit = 6.37 * sqrt(sqrt((mul == 1) ? N : mul)); //������ ���������� �������� ��� ���������� ���������
	int **arr ;

	if(mul!=1){
		arr = new int *[mul];
		for(int i = 0; i < mul; i++)
			arr[i] = new int[mul];
		for(int m = 0; m < mul; m++)
			for(int l = 0; l < N; l++)
				arr[m][l] = 0;
		min_k = find_min_sqrs(limit, &sqrs, arr, mul, 0, N/mul);
	}
	else {
		arr = new int *[N];
		for(int i = 0; i < N; i++)
			arr[i] = new int[N];
		for(int m = 0; m < N; m++)
			for(int l = 0; l < N; l++)
				arr[m][l] = 0;
//		int half = N-N/2;
//		fill_square(arr, 0, half);
//		fill_square(arr, half, half - 1);
//		fill_square(arr, 0, half - 1);

		min_k = find_min_sqrs(limit/* - 3*/, &sqrs, arr, N, 0, 1)/* + 3*/;

//		sqrs.push({half + 1, 1, half - 1});
//		sqrs.push({1, half + 1, half - 1});
//		sqrs.push({1, 1, half});
	}

	fout << "���������� = " << min_k << endl;
	Square tmp;
	fout << "����������� ������������� ���������:" << endl;
	for(int j = 0; j < min_k; j++){
		tmp = sqrs.top();
		fout << j + 1 << ") x = " << tmp.x << ", y = " << tmp.y << ", ����� = " << tmp.length << endl;
		sqrs.pop();
	}

	unsigned int time2 = clock();
	fout << "TIME : " << time2-time1 << endl;

	delete [] arr;
	return 0;
	fout.close();
}
