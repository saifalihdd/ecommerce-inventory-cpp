#ifndef DELIVERY_GRAPH_HPP
#define DELIVERY_GRAPH_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <string>
using namespace std;
class DeliveryGraph
{
private:
  map<string, vector<pair<string, int>>> adjList;

public:
  void addEdge(const string &u, const string &v, const int &w)
  {
    adjList[u].push_back(make_pair(v, w));
    adjList[v].push_back(make_pair(u, w));
    cout << "Rute baru berhasil ditambahkan!\n";
  }

  void bfs(const string &start)
  {
    map<string, bool> visited;
    queue<string> q;

    q.push(start);
    visited[start] = true;

    cout << "Rute pengiriman dari " << start << " (BFS): ";
    while (!q.empty())
    {
      string current = q.front();
      q.pop();
      cout << current << " ";

      for (const auto &neighbor : adjList[current])
      {
        if (!visited[neighbor.first])
        {
          visited[neighbor.first] = true;
          q.push(neighbor.first);
        }
      }
    }
    cout << "\n";
  }

  void dfs(string &start)
  {
    map<string, bool> visited;
    stack<string> s;
    s.push(start);
    cout << "Rute pengiriman dari " << start << " (DFS): ";
    while (!s.empty())
    {
      string current = s.top();
      s.pop();
      if (!visited[current])
      {
        visited[current] = true;
        cout << current << " ";

        for (auto it = adjList[current].rbegin(); it != adjList[current].rend(); ++it)
        {
          if (!visited[it->first])
            s.push(it->first);
        }
      }
    }
    cout << "\n";
  }
  void displayRoutes()
  {
    cout << "Dafta semua rute pengiriman: \n";
    for (const auto &node : adjList)
    {
      cout << "Dari: " << node.first << " ke:\n";
      for (const auto &neighbor : node.second)
      {
        cout << "  - " << neighbor.first << " (jarak: " << neighbor.second << " KM)\n";
      }
    }
  }

  void saveToCsv(const string &filename)
  {
    ofstream file(filename + ".csv");
    if (!file)
    {
      cerr << "Gagal membuka file untuk ditulis.\n";
      return;
    }
    file << "KotaAsal,KotaTujuan,JarakDalamKM\n";
    for (const auto &kota : adjList)
    {
      for (const auto &neighbor : kota.second)
      {
        file << kota.first << "," << neighbor.first << "," << neighbor.second << '\n';
      }
    }
    file.close();
    cout << "Data berhasil disimpan ke " << filename + ".csv\n";
  }

  void loadFromCsv(const string &filename)
  {
    ifstream in(filename + ".csv");
    if (!in)
    {
      cerr << "Gagal membuka file untuk dibaca.\n";
      return;
    }
    string line;
    getline(in, line); // Skip header
    while (getline(in, line))
    {
      stringstream ss(line);
      string from, to, distStr;
      getline(ss, from, ',');
      getline(ss, to, ',');
      getline(ss, distStr, ',');
      int distance = stoi(distStr);

      // Avoid redundant reverse edge
      bool exists = false;
      for (const auto &neighbor : adjList[to])
      {
        if (neighbor.first == from && neighbor.second == distance)
        {
          exists = true;
          break;
        }
      }

      if (!exists)
      {
        adjList[from].push_back({to, distance});
        adjList[to].push_back({from, distance});
      }
    }
    in.close();
    cout << "Data rute berhasil dimuat dari file.\n";
  }
};

#endif