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
    int u, v;
    double w;

    Edge(int b, int q, double rate) : u(b), v(q) {
        w = -log(rate);
    }
};

double bellmanFord(int base, int quote, vector<Edge>& edges, vector<string>& path);
double bestBidRoute(int base, int quote, vector<Edge>& edges, vector<string>& path);

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
    vector<Edge> edges;

    in >> base >> quote;
    in >> n;
    int idBase = mgr.addToken(base);
    int idQuote = mgr.addToken(quote);

    string _b, _q;
    double ask, bid;
    for (int i = 0; i < n; i++) {
        in >> _b >> _q;
        in >> ask >> bid;
        if (ask == 0 || bid == 0) {
            cerr << " > invalid ask & bid value" << endl;
            continue;
        }

        int idB = mgr.addToken(_b);
        int idQ = mgr.addToken(_q);
        
        edges.push_back(Edge(idB, idQ, bid));
        edges.push_back(Edge(idQ, idB, 1.0 / ask));
    }

    // Bellman-Ford
    vector<string> path;
    double bestAsk = bellmanFord(idQuote, idBase, edges, path);
    printPath(path);
    cout << (bestAsk == DBL_MAX? bestAsk : exp(bestAsk)) << endl;

    double bestBid = bellmanFord(idBase, idQuote, edges, path);
    printPath(path);
    cout << (bestBid == DBL_MAX ? bestBid : exp(-bestBid)) << endl;
        
    in.close();

    return 0;
}


vector<string> buildPath(int base, int quote, const vector<int>& parent) {
    vector<string> path;
    int cur = quote;
    int cnt = 0;
    while (cur != base) {
        path.push_back(mgr.getToken(cur));
        cur = parent[cur];
        cnt++;
        if (cnt == parent.size() * 2)   //  break if exist circle
            break;
    }
    path.push_back(mgr.getToken(base));
    reverse(path.begin(), path.end());
    return path;
}

double bellmanFord(int base, int quote, vector<Edge>& edges, vector<string>& path) {

    int n = mgr.m_id;

    vector<int> parent(n);
    vector<double> prices(n, DBL_MAX);
    prices[base] = 0;

    // relax all tokens
    for (int i = 1; i < n; i++) {       // token id in range [1..(m_id - 1)]
        bool relaxed = false;
        for (auto& e : edges) {
            if (prices[e.u] != DBL_MAX && prices[e.u] + e.w < prices[e.v]) {
                prices[e.v] = prices[e.u] + e.w;
                parent[e.v] = e.u;
                relaxed = true;
            }
        }
        if (!relaxed && i > 1)
            break;
    }

    // run once again for detect negative circle
    for (int i = 1; i < n; i++) {
        bool relaxed = false;
        for (auto& e : edges) {
            if (prices[e.u] != DBL_MAX && prices[e.u] + e.w < prices[e.v]) {
                cout << " > NEGATIVE CIRCLE EXISTS!" << endl;
                relaxed = true;
                break;
            }
        }
        if (relaxed)
            break;
    }

    path = buildPath(base, quote, parent);

    return prices[quote];
}
