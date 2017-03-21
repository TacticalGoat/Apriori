#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <iterator>
#include <string>
#include <algorithm>
using namespace std;

vector<string> transactions;
vector<map<string,int>> itemCount;
vector<map<string, int>> frequentItemsets;
map<string, double> allRules;
map<string, double>strongRules;

int min_sup = 2;
double min_conf = 0.70;

void initialize_test_values()
{
	transactions.push_back("125");
	transactions.push_back("24");
	transactions.push_back("23");
	transactions.push_back("124");
	transactions.push_back("13");
	transactions.push_back("23");
	transactions.push_back("13");
	transactions.push_back("1235");
	transactions.push_back("123");

}


void get_C1()
{
	map<string, int> tempItemCount;
	for (auto &t : transactions)
	{
		for (int i = 0; i < t.length(); i++)
		{
			if (tempItemCount.find(t.substr(i, 1)) == tempItemCount.end())
				tempItemCount[t.substr(i, 1)] = 1;
			else
				tempItemCount[t.substr(i, 1)] += 1;
		}
	}
	itemCount.push_back(tempItemCount);
}

void get_LK(int k)
{
	map<string, int> temp;
	for (auto &kv : itemCount[k])
	{
		if (kv.second >= min_sup)
		{
			temp[kv.first] = kv.second;
		}
	}
	frequentItemsets.push_back(temp);
}

string generate_candidate(string first,string second,int k)
{
	sort(first.begin(), first.end());
	sort(second.begin(), second.end());
	int length = first.length();
	if (length == 1)
		return first + second;
	else
	{
		string str_union;
		set_union(first.begin(), first.end(), second.begin(), second.end(), back_inserter(str_union));
		if (str_union.length() == k + 2)
			return str_union;
		else
			return "";
	}
}

vector<string> get_all_subsets(string str)
{
	vector<string> subsets;
	int n = str.size();
	int n1 = 1 << n;
	int flag = 0;
	for (int i = 0; i < n1; i++)
	{
		int pos = n - 1;
		int bitmask = i;
		string subset = "";
		while (bitmask > 0)
		{
			if ((bitmask & 1) == 1)
			{
				subset += str[pos];
			}
			bitmask >>= 1;
			pos--;
		}
		if (subset.length() != str.length())
		{
			sort(subset.begin(), subset.end());
			subsets.push_back(subset);
		}
	}
	sort(subsets.begin(),subsets.end());
	return subsets;
}

vector<string> get_map_keys(map<string, int> base)
{
	vector<string> retval;
	for (auto &kv : base)
	{
		retval.push_back(kv.first);
	}
	return retval;
}

int get_support(string candidate)
{
	int support = 0;
	for (auto &t : transactions)
	{
		sort(t.begin(), t.end());
		string str_union;
		set_union(t.begin(), t.end(), candidate.begin(), candidate.end(), back_inserter(str_union));
		if (str_union.compare(t) == 0)
		{
			support++;
		}
	}
	return support;
}

void get_CK(int k)
{
	map<string, int>  candidates;
	vector<string> items = get_map_keys(frequentItemsets[k]);
	if (items.size() == 0)
	{
		itemCount.push_back(candidates);
		return;
	}
	for (int i = 0; i < items.size() - 1; i++)
	{
		for (int j = i + 1; j < items.size(); j++)
		{
			string generatedCandidate = generate_candidate(items[i], items[j],k);
			if (generatedCandidate.compare("") != 0)
			{
				//cout << "Candidate:" << generatedCandidate << endl;
				int support = get_support(generatedCandidate);
				//cout << "GOT SUPPORT";
				candidates[generatedCandidate] = support;
			}
		}
	}
	itemCount.push_back(candidates);
}
string get_remaining(string item,string subset)
{
	string set_diff;
	set_difference(item.begin(), item.end(), subset.begin(), subset.end(), back_inserter(set_diff));
	return set_diff;
}

double get_confidence(string item1, string item2)
{
	double supportX, supportXY;
	int lenX, lenXY;
	string XY = item1 + item2;
	sort(XY.begin(), XY.end());
	lenX = item1.length();
	lenXY = XY.length();
	supportX = itemCount[lenX - 1][item1];
	supportXY = itemCount[lenXY - 1][XY];
	return supportXY/supportX;
}

void generateRules()
{
	for (auto freq : frequentItemsets)
	{
		for (auto kv : freq)
		{
			if (kv.first.length() > 1)
			{
				string item = kv.first;
				vector<string> subsets = get_all_subsets(item);
				for (auto subset : subsets)
				{
					if (subset.compare("") != 0)
					{
						string remaining = get_remaining(item, subset);
						string rule = subset + "->" + remaining;
						double confidence = get_confidence(subset, remaining);
						if (allRules.find(rule) == allRules.end())
						{
							allRules[rule] = confidence;
						}
					}
				}
			}
		}
	}
}

void generateStrongRules()
{
	for (auto kv : allRules)
	{
		if (kv.second >= min_conf)
		{
			strongRules[kv.first] = kv.second;
		}
	}
}

int main()
{
	initialize_test_values();
	get_C1();
	int k = 0;
	while (1)
	{
		get_LK(k);
		get_CK(k);
		if (itemCount[k+1].size() == 0)
			break;
		k++;
	}
	generateRules();
	generateStrongRules();
	cout << "ASSOCIATION RULES:" << endl;
	cout << "RULE" << "CONF" << endl;
	for (auto & kv : strongRules)
		cout << kv.first << ":" << kv.second<<endl;
	int y;
	//cout << get_support("1235");
	cin >> y;
	return 0;
}