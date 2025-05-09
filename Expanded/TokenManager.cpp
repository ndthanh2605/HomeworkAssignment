#include "TokenManager.h"

int TokenManager::addToken(const string& token)
{
	if (!m_tokens.count(token)) {
		m_tokens[token] = m_id++;
		m_names.push_back(token);
	}

	return m_tokens[token];
}

int TokenManager::getTokenId(const string& token)
{
	if (m_tokens.count(token))
		return m_tokens[token];

	return 0;
}

string TokenManager::getToken(int id)
{
	if (id < m_names.size())
		return m_names[id];

	return "";
}

vector<string> TokenManager::buildPath(int base, int quote, const vector<int>& parent, bool needReverse)
{
	vector<string> path;
	int cur = quote;
	int cnt = 0;
	while (cur != base) {
		path.push_back(getToken(cur));
		cur = parent[cur];
		cnt++;
		if (cnt == parent.size() * 2)   //  break if exist circle
			break;
	}
	path.push_back(getToken(base));
	if (needReverse)
		reverse(path.begin(), path.end());
	return path;
}
