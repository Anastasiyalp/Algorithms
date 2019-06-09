#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool KMP(const string& S, const string& pattern, vector<int> &ent) 
{
	vector<int> pf (pattern.length());
	vector<int> :: iterator n = ent.begin();
	pf[0] = 0;
	for (int k = 0, i = 1; i < pattern.length(); ++i) 
	{
		while ((k > 0) && (pattern[i] != pattern[k]))
			k = pf[k-1];

		if (pattern[i] == pattern[k])
			k++;

		pf[i] = k;
	}

	for (int k = 0, i = 0; i < S.length(); ++i) 
	{
		while ((k > 0) && (pattern[k] != S[i]))
			k = pf[k-1];

		if (pattern[k] == S[i])
			k++;

		if (k == pattern.length())
		{
			*n = i - pattern.length() + 1; //либо продолжаем поиск следующих вхождений
			n++;
		}
	}
	return n == ent.begin();
}

int main()
{
	string pattern;
	string S;
	cin >> pattern >> S;
	vector<int> ent (S.length());
	if(KMP(S, pattern, ent))
	{
		cout << -1;
		return 0;
	}

	for(vector<int> :: iterator n = ent.begin(); n != ent.end() && !(*n == 0 && n != ent.begin()); n++)
	{
		if(n != ent.begin())
			cout << ',';
		cout << *n ;
	}
	return 0;
}