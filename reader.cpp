#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>

using namespace std;

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

vector<string> split(const std::string &s, char delim)
{
	vector<string> retval;
	split(s, delim, back_inserter(retval));
	return retval;
}
int main()
{
	ifstream infile("input.csv");
	string line;
	while (getline(infile, line))
	{
		istringstream iss(line);
		string a, b;
		if (!(iss >> a >> b))
			break;
		cout << a << " ";
		cout << b << endl;
		istringstream iss2(b);
		vector<string> tokens = split(b, ',');
		for (auto token : tokens)
		{
			cout << token << endl;
		}

	}
	int y;
	cin >> y;
	return 0;
}