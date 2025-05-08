// Simple.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <iomanip>
#include "TokenManager.h"

using namespace std;

struct Edge {
    int base, quote;
    float ask, bid;

    Edge(int _base, int _quote, float _ask, float _bid)
        : base(_base), quote(_quote), ask(_ask), bid(_bid) {
    }
};

float bestAskRoute(int base, int quote, vector<vector<Edge>>& adj, vector<string>& path);
float bestBidRoute(int base, int quote, vector<vector<Edge>>& adj, vector<string>& path);

void printPath(const vector<string>& path) {
    if (path.empty())
        return;

    cout << path[0];
    for (int i = 1; i < path.size(); i++) {
        cout << "->" << path[i];
    }
    cout << endl;
}

// manage tokens by id
TokenManager mgr;


int main()
{
    ifstream in("input.txt");

    if (!in.is_open()) {
        cerr << "ERROR open file" << endl;
        return 0;
    }


    string base, quote;
    int n;
    vector<vector<Edge>> adj;

    in >> base >> quote;
    in >> n;
    int idBase = mgr.addToken(base);
    int idQuote = mgr.addToken(quote);

    string _b, _q;
    float ask, bid;
    for (int i = 0; i < n; i++) {
        in >> _b >> _q;
        in >> ask >> bid;
        if (ask == 0 || bid == 0) {
            cerr << " > invalid ask & bid value" << endl;
            continue;
        }

        int idB = mgr.addToken(_b);
        int idQ = mgr.addToken(_q);
        adj.resize(mgr.m_id);

        adj[idB].push_back(Edge(idB, idQ, ask, bid));
        adj[idQ].push_back(Edge(idQ, idB, 1.0 / bid, 1.0 / ask));
    }

    // Dijkstra
    vector<string> path;
    float bestAsk = bestAskRoute(idBase, idQuote, adj, path);
    printPath(path);
    cout << bestAsk << endl;

    float bestBid = bestBidRoute(idBase, idQuote, adj, path);
    printPath(path);
    cout << bestBid << endl;
        
    in.close();

    return 0;
}


vector<string> buildPath(int base, int quote, const vector<int>& parent) {
    vector<string> path;
    int cur = quote;
    while (cur != base) {
        path.push_back(mgr.getToken(cur));
        cur = parent[cur];
    }
    path.push_back(mgr.getToken(base));
    return path;
}

float bestAskRoute(int base, int quote, vector<vector<Edge>>& adj, vector<string>& path) {

    int n = mgr.m_id;

    vector<int> parent(n);
    vector<float> prices(n, FLT_MAX);
    prices[base] = 1;

    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<>> pq;
    pq.push({ 1.0, base });

    while (!pq.empty()) {
        auto node = pq.top(); pq.pop();
        float cost = node.first;
        int token = node.second;
        if (token == quote) {
            break;
        }

        if (cost > prices[token])
            continue;

        for (auto& e : adj[token]) {
            float price = cost * e.ask;
            if (price >= prices[e.quote])
                continue;

            prices[e.quote] = price;
            pq.push({ price, e.quote });
            parent[e.quote] = node.second;
        }
    }
    path = buildPath(base, quote, parent);

    return prices[quote];
}

float bestBidRoute(int base, int quote, vector<vector<Edge>>& adj, vector<string>& path) {

    int n = mgr.m_id;

    vector<int> parent(n);
    vector<float> prices(n, 0);
    prices[base] = 1;

    priority_queue<pair<float, int>> pq;
    pq.push({ 1.0, base });

    while (!pq.empty()) {
        auto node = pq.top(); pq.pop();
        if (node.second == quote) {
            break;
        }

        for (auto& e : adj[node.second]) {
            float price = node.first * e.bid;
            if (price <= prices[e.quote])
                continue;

            prices[e.quote] = price;
            pq.push({ price, e.quote });
            parent[e.quote] = node.second;
        }
    }
    path = buildPath(base, quote, parent);

    return prices[quote];
}
