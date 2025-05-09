#pragma once
#include <string>
#include <unordered_map>

using namespace std;

class TokenManager
{
public:
	int m_id = 1;
	vector<string> m_names = { "" };
	unordered_map<string, int> m_tokens;

	int addToken(const string& token);
	
	int getTokenId(const string& token);
	string getToken(int id);

	vector<string> buildPath(int base, int quote, const vector<int>& parent, bool needReverse = true);

};

