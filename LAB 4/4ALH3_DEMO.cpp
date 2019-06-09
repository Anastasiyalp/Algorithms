#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool KMP(const string& pattern, const string& S) //Возвращает логическую переменную означающую была ли найдена подстрока
{
	vector<int> pf (pattern.length());	//Вектор для значений префикс-функции шаблона
	cout << "Значения префикс функции шаблона: " << pattern << endl;
	pf[0] = 0;
	cout << "1) Для элемента " << pattern[0] << " = 0" << endl << endl;
	for (int k = 0, i = 1; i < pattern.length(); ++i)  //Установим значения для pf
	{
		cout << i+1 <<") Для элемента " << pattern[i] << ":" << endl;
		while ((k > 0) && (pattern[i] != pattern[k])) //Найдем одинаковый элемент для текущего используя значения pf
		{
			cout << "Значение предыдущей п-функции для элемента " << pattern[k-1] << " = " << pf[k-1] << endl;
			k = pf[k-1];
		}
		if (pattern[i] == pattern[k]) //Если такой нашелся, то увеличиваем переменную префик-функции
			k++;

		pf[i] = k; //Установим значение для элемента префикс-функции в значение ее переменной
		cout << "Итоговое значение п-функции для элемента " << pattern[i] << " = " << k << endl << endl; 
	}

	bool first = true; //Несет инфу о нахождении подстроки в строке
	cout << "Значения префикс функции строки поиска: " << S << endl;
	for(int i = 0, k = 0; i < S.length(); ++i) //Найдем подстроку в строке 
	{
		cout << i+1 <<") Для элемента " << S[i] << ":" << endl;
		while ((k > 0) && (pattern[k] != S[i])) //Пока значение префикс функции больше нуля или мы не нашли одинаковых элементов
		{
			cout << "Значение предыдущей п-функции для элемента " << pattern[k-1] << " = " << pf[k-1] << endl;
			k = pf[k-1]; //Переходим к рассмотрению подпрефикса для текующего префикса 
		}
		if (pattern[k] == S[i]) //Если нашли одинаковые элементы, то увеличиваем переменную длины вхождения pattern в S
			k++;
		cout << "Итоговое значение п-функции для элемента " << S[i] << " = " << k << endl << endl; 

		if (k == pattern.length()) //Если нашли полную строку pattern в S, то возращаемся 
		{
			cout << "Найдена подстрока в позиции : " << i - pattern.length() + 1 << endl << endl;
			first = false;
		}
	}
	return first; //Не нашли pattern в S
}

int main()
{
    setlocale(LC_ALL, "rus");

	string pattern;
	string S;
	cin >> pattern >> S;
	if(KMP(pattern, S))
		cout << -1;
	return 0;
}