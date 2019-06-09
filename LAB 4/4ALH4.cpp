#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

bool KMP(const string& pattern) 
{
	vector<int> pf (pattern.length());
	pf[0] = 0;
	for (int k = 0, i = 1; i < pattern.length(); ++i) 
	{
		while ((k > 0) && (pattern[i] != pattern[k]))
			k = pf[k-1];

		if (pattern[i] == pattern[k])
			k++;

		pf[i] = k;
	}

	int k = 0, i = 0;
	char c;
	bool first = true;
	while(cin >> c) 
	{
		while ((k > 0) && (pattern[k] != c))
			k = pf[k-1];

		if (pattern[k] == c)
			k++;

		if (k == pattern.length())
			if(first)
			{
				cout << i - pattern.length() + 1;
				first = false;
			}else
				cout << ',' << i - pattern.length() + 1;
		i++;
	}
	return first;
}

int main()
{
	string pattern;
	cin >> pattern;
	list<int> ent;
	if(KMP(pattern))
	{
		cout << -1;
		return 0;
	}
	return 0;
}