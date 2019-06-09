#include <iostream>
#include <string>
#include <vector>

using namespace std;

int KMP(const string& S, const string& pattern) //Определяет является ли вторая строка циклическим сдвигом первой
{
    if(S.length() != pattern.length())	//Если длины не равны, то не является
        return -1;
	vector<int> pf (pattern.length());	//Вектор для значений префикс-функции шаблона
    cout << "Значения префикс функции шаблона: " << pattern << endl;
	pf[0] = 0;
    cout << "1) Для элемента " << pattern[0] << " = 0" << endl << endl;
	for (int k = 0, i = 1; i < pattern.length(); ++i) //Установим значения для pf
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

    cout << "Значения префикс функции строки поиска: " << S << endl;
	for (int k = 0, i = 0; i < 2*S.length(); ++i) //Найдем циклический сдвиг в строке S+S 
	{
   		cout << i+1 <<") Для элемента " << S[i%S.length()] << ":" << endl;
		while ((k > 0) && (pattern[k] != S[i%S.length()])) //Пока значение префикс функции больше нуля или мы не нашли одинаковых элементов
		{
			cout << "Значение предыдущей п-функции для элемента " << pattern[k-1] << " = " << pf[k-1] << endl;
			k = pf[k-1]; //Переходим к рассмотрению подпрефикса для текующего префикса 
		}
		if (pattern[k] == S[i%S.length()]) //Если нашли одинаковые элементы, то увеличиваем переменную длины вхождения pattern в S
			k++;
  		cout << "Итоговое значение п-функции для элемента " << S[i%S.length()] << " = " << k << endl << endl; 

		if (k == pattern.length()) //Если нашли полную строку pattern в S, то возращаемся 
			return (i + 1)%pattern.length();
	}
	return -1; //Не нашли pattern в S
}

int main()
{    
    setlocale(LC_ALL, "rus");
    string A;
    string B;
    cin >> A >> B;
    int result;
    if((result = KMP(A, B)) == -1)
        cout << "Циклический сдвиг не найден." << endl;
    else
        cout << "Циклический сдвиг найден со смещением " << result << endl;
    return 0;
}