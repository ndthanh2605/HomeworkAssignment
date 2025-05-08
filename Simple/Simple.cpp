// Simple.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <iomanip>

using namespace std;

struct Edge {
    string base, quote;
    float ask, bid;

    Edge(const string& _base, const string& _quote, float _ask, float _bid)
        : base(_base), quote(_quote), ask(_ask), bid(_bid) {
    }
};

float bestAskRoute(const string& base, const string& quote, unordered_map<string, vector<Edge>>& adj, vector<string>& path);
float bestBidRoute(const string& base, const string& quote, unordered_map<string, vector<Edge>>& adj, vector<string>& path);

void printPath(const vector<string>& path) {
    cout << path[0];
    for (int i = 1; i < path.size(); i++) {
        cout << "->" << path[i];
    }
    cout << endl;
}

int main()
{
    ifstream in("input.txt");

    if (in.is_open()) {
        string base, quote;
        int n;
        unordered_map<string, vector<Edge>> adj;

        in >> base >> quote;
        in >> n;

        string _b, _q;
        float ask, bid;
        for (int i = 0; i < n; i++) {
            in >> _b >> _q;
            in >> ask >> bid;
            if (ask == 0 || bid == 0) {
                cerr << " > invalid ask & bid value" << endl;
                continue;
            }
            adj[_b].push_back(Edge(_b, _q, ask, bid));
            adj[_q].push_back(Edge(_q, _b, 1.0 / bid, 1.0 / ask));
        }

        // Dijkstra
        vector<string> path;
        float bestAsk = bestAskRoute(base, quote, adj, path);
        printPath(path);
        cout << bestAsk << endl;

        float bestBid = bestBidRoute(base, quote, adj, path);
        printPath(path);
        cout << bestBid << endl;
        
        in.close();
    }
    else {
        cerr << "ERROR open file" << endl;
    }

    return 0;
}


vector<string> buildPath(const string& base, const string& quote, unordered_map<string, string>& parent) {
    vector<string> path;
    string cur = quote;
    while (cur != base) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back(base);
    return path;
}

float bestAskRoute(const string& base, const string& quote, unordered_map<string, vector<Edge>>& adj, vector<string>& path) {

    unordered_map<string, string> parent;
    unordered_map<string, float> prices;
    for (auto& itr : adj) {
        prices[itr.first] = FLT_MAX;
    }
    prices[base] = 1;

    priority_queue<pair<float, string>, vector<pair<float, string>>, greater<>> pq;
    pq.push({ 1.0, base });

    while (!pq.empty()) {
        auto node = pq.top(); pq.pop();
        if (node.second == quote) {
            break;
        }

        for (auto& e : adj[node.second]) {
            float price = node.first * e.ask;
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

float bestBidRoute(const string& base, const string& quote, unordered_map<string, vector<Edge>>& adj, vector<string>& path) {

    unordered_map<string, string> parent;
    unordered_map<string, float> prices;
    for (auto& itr : adj) {
        prices[itr.first] = 0;
    }
    prices[base] = 1;

    priority_queue<pair<float, string>> pq;
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
