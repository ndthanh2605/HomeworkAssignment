// Expanded.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include "TokenManager.h"

struct Edge {
    int u, v;
    double rate, cap;

    Edge(int b, int q, double r, double c) : u(b), v(q), rate(r), cap(c) {}
};

double cappedBellmanFord(int base, double qty, int quote, vector<Edge>& edges, vector<string>& path);
double revCappedBellmanFord(int base, double qty, int quote, vector<Edge>& edges, vector<string>& path);

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
    double qty;
    int n;
    vector<Edge> edges;

    in >> base >> quote >> qty;
    in >> n;
    int idBase = mgr.addToken(base);
    int idQuote = mgr.addToken(quote);

    int cnt;
    string _b, _q;
    double _ask, _bid, _qty;
    for (int i = 0; i < n; i++) {
        in >> _b >> _q;
        int idB = mgr.addToken(_b);
        int idQ = mgr.addToken(_q);

        in >> cnt;
        for (int j = 0; j < cnt; j++) {
            in >> _ask >> _qty;
            if (_ask == 0 || _qty == 0)
                continue;
            edges.push_back(Edge(idQ, idB, 1.0 / _ask, _qty));
        }
        in >> cnt;
        for (int j = 0; j < cnt; j++) {
            in >> _bid >> _qty;
            if (_bid == 0 || _qty == 0)
                continue;
            edges.push_back(Edge(idB, idQ, _bid, _qty));
        }
    }
    // done read input

    // Bellman-Ford with capacity
    vector<string> path;
    double bestAskAmount = revCappedBellmanFord(idBase, qty, idQuote, edges, path);
    printPath(path);
    cout << (bestAskAmount / qty) << endl;

    double bestBidAmount = cappedBellmanFord(idBase, qty, idQuote, edges, path);
    printPath(path);
    cout << (bestBidAmount / qty) << endl;



    in.close();

    return 0;
}

double cappedBellmanFord(int base, double qty, int quote, vector<Edge>& edges, vector<string>& path) {

    int n = mgr.m_id;
    vector<int> parent(n);
    vector<double> amount(n, 0);
    amount[base] = qty;

    // relax n - 1 times
    for (int i = 1; i < n; i++) {
        bool relaxed = false;
        for (auto& e : edges) {
            if (amount[e.u] == 0)
                continue;

            double _amount = amount[e.u] * e.rate;
            if (_amount > e.cap)
                continue;
            if (_amount > amount[e.v]) {
                amount[e.v] = amount[e.u] * e.rate;
                parent[e.v] = e.u;
                relaxed = true;
            }
        }

        if (!relaxed)
            break;
    }
    path = mgr.buildPath(base, quote, parent, false);

    return amount[quote];
}

double revCappedBellmanFord(int base, double qty, int quote, vector<Edge>& edges, vector<string>& path) {

    int n = mgr.m_id;
    vector<int> parent(n);
    vector<double> amount(n, DBL_MAX);
    amount[base] = qty;

    // relax n - 1 times
    for (int i = 1; i < n; i++) {
        bool relaxed = false;
        for (auto& e : edges) {
            if (amount[e.v] == DBL_MAX)
                continue;

            double _amount = amount[e.v] / e.rate;
            if (_amount > e.cap)
                continue;
            if (_amount < amount[e.u]) {
                amount[e.u] = amount[e.v] / e.rate;
                parent[e.u] = e.v;
                relaxed = true;
            }
        }

        if (!relaxed)
            break;
    }
    
    path = mgr.buildPath(base, quote, parent);

    return amount[quote];
}