#pragma once
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

string real_calculate(float x, float y, float z, string s)
{
	while (s.find('x') != string::npos)
	{
		s.replace(s.find('x'), 1, to_string(x));
	}
	while (s.find('y') != string::npos)
	{
		s.replace(s.find('y'), 1, to_string(y));
	}
	while (s.find('z') != string::npos)
	{
		s.replace(s.find('z'), 1, to_string(z));
	}
	while (s.find('(') != string::npos)
	{
		int c = 0;
		for (int i = s.find('('); i < s.size(); i++)
		{
			if (s[i] == '(')
			{
				c++;
			}
			if (s[i] == ')')
			{
				c--;
				if (c == 0)
				{
					string s1;
					s1.assign(s, s.find('(') + 1, i - s.find('(') - 1);
					s.replace(s.find('('), i - s.find('(') + 1, real_calculate(x, y, z, s1));
				}
			}
		}
	}
	string N1;
	string N2;
	float n1;
	float n2;
	int k, j, l;
	while (s.find("--") != string::npos)
	{
		if (s.find("--") != 0)
		{
			s.replace(s.find("--"), 2, "+");
		}
		else
		{
			s.replace(s.find("--"), 2, "");
		}
	}
	while (s.string::find("+-") != string::npos)
	{
		s.replace(s.string::find("+-"), 2, "-");
	}
	if (s[0] == '-')
	{
		s[0] = '~';
	}
	while (s.find('^') != string::npos)
	{
		n1 = 1;
		n2 = 1;
		k = s.find('^');
		j = k - 1;
		while (j >= 0 && s[j] != '*' && s[j] != '/' && s[j] != '^' && s[j] != '+' && s[j] != '-')
		{
			j--;
		}
		N1.assign(s, j + 1, k - j);
		if (N1[0] == '~')
		{
			n1 *= -1;
			N1.erase(0, 1);
		}
		n1 *= stof(N1);
		l = k + 1;
		while (l != s.size() && s[l] != '*' && s[l] != '/' && s[l] != '^' && s[l] != '+' && s[l] != '-')
		{
			l++;
		}
		N2.assign(s, k + 1, l - k - 1);
		if (N2[0] == '~')
		{
			n2 *= -1;
			N2.erase(0, 1);
		}
		n2 *= stof(N2);
		s = real_calculate(x, y, z, s.replace(j + 1, l - j - 1, "(" + to_string(pow(n1, n2)) + ")"));
	}
	while (s.find('*') != string::npos)
	{
		n1 = 1;
		n2 = 1;
		k = s.find('*');
		j = k - 1;
		while (j >= 0 && s[j] != '*' && s[j] != '/' && s[j] != '^' && s[j] != '+' && s[j] != '-')
		{
			j--;
		}
		N1.assign(s, j + 1, k - j);
		if (N1[0] == '~')
		{
			n1 *= -1;
			N1.erase(0, 1);
		}
		n1 *= stof(N1);
		l = k + 1;
		while (l != s.size() && s[l] != '*' && s[l] != '/' && s[l] != '^' && s[l] != '+' && s[l] != '-')
		{
			l++;
		}
		N2.assign(s, k + 1, l - k - 1);
		if (N2[0] == '~')
		{
			n2 *= -1;
			N2.erase(0, 1);
		}
		n2 *= stof(N2);
		s = real_calculate(x, y, z, s.replace(j + 1, l - j - 1, "(" + to_string(n1 * n2) + ")"));
	}
	while (s.find('/') != string::npos)
	{
		n1 = 1;
		n2 = 1;
		k = s.find('/');
		j = k - 1;
		while (j >= 0 && s[j] != '*' && s[j] != '/' && s[j] != '^' && s[j] != '+' && s[j] != '-')
		{
			j--;
		}
		N1.assign(s, j + 1, k - j);
		if (N1[0] == '~')
		{
			n1 *= -1;
			N1.erase(0, 1);
		}
		n1 *= stof(N1);
		l = k + 1;
		while (l != s.size() && s[l] != '*' && s[l] != '/' && s[l] != '^' && s[l] != '+' && s[l] != '-')
		{
			l++;
		}
		N2.assign(s, k + 1, l - k - 1);
		if (N2[0] == '~')
		{
			n2 *= -1;
			N2.erase(0, 1);
		}
		n2 *= stof(N2);
		s = real_calculate(x, y, z, s.replace(j + 1, l - j - 1, "(" + to_string(n1 / n2) + ")"));
	}
	while (s.find('+') != string::npos)
	{
		n1 = 1;
		n2 = 1;
		k = s.find('+');
		j = k - 1;
		while (j >= 0 && s[j] != '*' && s[j] != '/' && s[j] != '^' && s[j] != '+' && s[j] != '-')
		{
			j--;
		}
		N1.assign(s, j + 1, k - j);
		if (N1[0] == '~')
		{
			n1 *= -1;
			N1.erase(0, 1);
		}
		n1 *= stof(N1);
		l = k + 1;
		while (l != s.size() && s[l] != '*' && s[l] != '/' && s[l] != '^' && s[l] != '+' && s[l] != '-')
		{
			l++;
		}
		N2.assign(s, k + 1, l - k - 1);
		if (N2[0] == '~')
		{
			n2 *= -1;
			N2.erase(0, 1);
		}
		n2 *= stof(N2);
		s = real_calculate(x, y, z, s.replace(j + 1, l - j - 1, "(" + to_string(n1 + n2) + ")"));
	}
	while (s.rfind('-') != string::npos && s.rfind('-') != 0)
	{
		n1 = 1;
		n2 = 1;
		k = s.rfind('-');
		j = k - 1;
		while (j >= 0 && s[j] != '*' && s[j] != '/' && s[j] != '^' && s[j] != '+' && s[j] != '-')
		{
			j--;
		}
		N1.assign(s, j + 1, k - j);
		if (N1[0] == '~')
		{
			n1 *= -1;
			N1.erase(0, 1);
		}
		n1 *= stof(N1);
		l = k + 1;
		while (l != s.size() && s[l] != '*' && s[l] != '/' && s[l] != '^' && s[l] != '+' && s[l] != '-')
		{
			l++;
		}
		N2.assign(s, k + 1, l - k - 1);
		if (N2[0] == '~')
		{
			n2 *= -1;
			N2.erase(0, 1);
		}
		n2 *= stof(N2);
		s = real_calculate(x, y, z, s.replace(j + 1, l - j - 1, "(" + to_string(n1 - n2) + ")"));
	}
	return s;
}

float calculate(float x, float y, float z, string s)
{
	string s1 = real_calculate(x, y, z, s);
	if (s1[0] == '~')
	{
		s1.replace(0, 1, "-");
	}
	return stof(s1);
}

bool isTrue(float x, float y, float z, string s)
{
	bool is_less = 0;
	bool is_more = 0;
	bool is_equal = 0;
	string s1, s2;
	if (s.find("<") != string::npos)
	{
		is_less = 1;
	}
	if (s.find(">") != string::npos)
	{
		is_more = 1;
	}
	if (s.find("=") != string::npos)
	{
		is_equal = 1;
	}
	if (is_less && is_equal)
	{
		s1.assign(s, 0, s.find("<"));
		s2.assign(s, s.find("=") + 1, s.size() - s.find("=") - 1);
		return calculate(x, y, z, s1) <= calculate(x, y, z, s2);
	}
	else if (is_more && is_equal)
	{
		s1.assign(s, 0, s.find(">"));
		s2.assign(s, s.find("=") + 1, s.size() - s.find("=") - 1);
		return calculate(x, y, z, s1) >= calculate(x, y, z, s2);
	}
	else if (is_less)
	{
		s1.assign(s, 0, s.find("<"));
		s2.assign(s, s.find("<") + 1, s.size() - s.find("<") - 1);
		return calculate(x, y, z, s1) < calculate(x, y, z, s2);
	}
	else if (is_more)
	{
		s1.assign(s, 0, s.find(">"));
		s2.assign(s, s.find(">") + 1, s.size() - s.find(">") - 1);
		return calculate(x, y, z, s1) > calculate(x, y, z, s2);
	}
	else if (is_equal)
	{
		s1.assign(s, 0, s.find("="));
		s2.assign(s, s.find("=") + 1, s.size() - s.find("=") - 1);
		return calculate(x, y, z, s1) == calculate(x, y, z, s2);
	}
}