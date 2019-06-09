#include <iostream>
#include <string>
#include <vector>

using namespace std;

int KMP(const string& S, const string& pattern) //���������� �������� �� ������ ������ ����������� ������� ������
{
    if(S.length() != pattern.length())	//���� ����� �� �����, �� �� ��������
        return -1;
	vector<int> pf (pattern.length());	//������ ��� �������� �������-������� �������
    cout << "�������� ������� ������� �������: " << pattern << endl;
	pf[0] = 0;
    cout << "1) ��� �������� " << pattern[0] << " = 0" << endl << endl;
	for (int k = 0, i = 1; i < pattern.length(); ++i) //��������� �������� ��� pf
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

    cout << "�������� ������� ������� ������ ������: " << S << endl;
	for (int k = 0, i = 0; i < 2*S.length(); ++i) //������ ����������� ����� � ������ S+S 
	{
   		cout << i+1 <<") ��� �������� " << S[i%S.length()] << ":" << endl;
		while ((k > 0) && (pattern[k] != S[i%S.length()])) //���� �������� ������� ������� ������ ���� ��� �� �� ����� ���������� ���������
		{
			cout << "�������� ���������� �-������� ��� �������� " << pattern[k-1] << " = " << pf[k-1] << endl;
			k = pf[k-1]; //��������� � ������������ ����������� ��� ��������� �������� 
		}
		if (pattern[k] == S[i%S.length()]) //���� ����� ���������� ��������, �� ����������� ���������� ����� ��������� pattern � S
			k++;
  		cout << "�������� �������� �-������� ��� �������� " << S[i%S.length()] << " = " << k << endl << endl; 

		if (k == pattern.length()) //���� ����� ������ ������ pattern � S, �� ����������� 
			return (i + 1)%pattern.length();
	}
	return -1; //�� ����� pattern � S
}

int main()
{    
    setlocale(LC_ALL, "rus");
    string A;
    string B;
    cin >> A >> B;
    int result;
    if((result = KMP(A, B)) == -1)
        cout << "����������� ����� �� ������." << endl;
    else
        cout << "����������� ����� ������ �� ��������� " << result << endl;
    return 0;
}