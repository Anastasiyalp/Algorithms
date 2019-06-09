#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool KMP(const string& pattern, const string& S) //���������� ���������� ���������� ���������� ���� �� ������� ���������
{
	vector<int> pf (pattern.length());	//������ ��� �������� �������-������� �������
	cout << "�������� ������� ������� �������: " << pattern << endl;
	pf[0] = 0;
	cout << "1) ��� �������� " << pattern[0] << " = 0" << endl << endl;
	for (int k = 0, i = 1; i < pattern.length(); ++i)  //��������� �������� ��� pf
	{
		cout << i+1 <<") ��� �������� " << pattern[i] << ":" << endl;
		while ((k > 0) && (pattern[i] != pattern[k])) //������ ���������� ������� ��� �������� ��������� �������� pf
		{
			cout << "�������� ���������� �-������� ��� �������� " << pattern[k-1] << " = " << pf[k-1] << endl;
			k = pf[k-1];
		}
		if (pattern[i] == pattern[k]) //���� ����� �������, �� ����������� ���������� ������-�������
			k++;

		pf[i] = k; //��������� �������� ��� �������� �������-������� � �������� �� ����������
		cout << "�������� �������� �-������� ��� �������� " << pattern[i] << " = " << k << endl << endl; 
	}

	bool first = true; //����� ���� � ���������� ��������� � ������
	cout << "�������� ������� ������� ������ ������: " << S << endl;
	for(int i = 0, k = 0; i < S.length(); ++i) //������ ��������� � ������ 
	{
		cout << i+1 <<") ��� �������� " << S[i] << ":" << endl;
		while ((k > 0) && (pattern[k] != S[i])) //���� �������� ������� ������� ������ ���� ��� �� �� ����� ���������� ���������
		{
			cout << "�������� ���������� �-������� ��� �������� " << pattern[k-1] << " = " << pf[k-1] << endl;
			k = pf[k-1]; //��������� � ������������ ����������� ��� ��������� �������� 
		}
		if (pattern[k] == S[i]) //���� ����� ���������� ��������, �� ����������� ���������� ����� ��������� pattern � S
			k++;
		cout << "�������� �������� �-������� ��� �������� " << S[i] << " = " << k << endl << endl; 

		if (k == pattern.length()) //���� ����� ������ ������ pattern � S, �� ����������� 
		{
			cout << "������� ��������� � ������� : " << i - pattern.length() + 1 << endl << endl;
			first = false;
		}
	}
	return first; //�� ����� pattern � S
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