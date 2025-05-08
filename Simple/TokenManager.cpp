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
