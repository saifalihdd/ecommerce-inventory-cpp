#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include "avl_tree.hpp"
#include "struct.hpp"
using namespace std;

const int table_size = 100;

class HashTable
{
private:
  vector<list<Produk>> table;

  int hash(int key)
  {
    return key % table_size;
  }

public:
  HashTable()
  {
    table.resize(table_size);
  }
  void insert(const Produk &produk)
  {
    if (search(produk.idProduk))
    {
      cout << "Produk dengan ID " << produk.idProduk << " sudah ada.\n";
      return;
    }
    int index = hash(produk.idProduk);
    table[index].push_back(produk);
  }

  Produk *search(int id)
  {
    int index = hash(id);
    for (auto &prod : table[index])
    {
      if (prod.idProduk == id)
        return &prod;
    }
    return nullptr;
  }

  void update(int id, const Produk &updateProduk)
  {
    int index = hash(id);
    for (auto &prod : table[index])
    {
      if (prod.idProduk == id)
      {
        prod = updateProduk;
        return;
      }
    }
  }

  void remove(int id)
  {
    int index = hash(id);
    table[index].remove_if([id](const Produk &p)
                           { return p.idProduk == id; });
  }

  vector<Produk> getAll() const {
    vector<Produk> all;
    for (const auto& bucket : table) {
        for (const auto& prod : bucket) {
            all.push_back(prod);
        }
    }
    return all;
}


  void printAll() const
  {
    for (const auto &bucket : table)
    {
      for (const auto &prod : bucket)
      {
        cout << "ID: " << prod.idProduk << " | Nama: " << prod.namaProduk << " | Harga: " << prod.hargaProduk
             << " | Stok: " << prod.stokProduk << " | Sales: " << prod.sales
             << " | Rating: " << prod.ratingProduk << "\n";
      }
    }
  }

  vector<Produk> getSemuaProdukBerdasarkanNama() const
  {
    vector<Produk> all;
    for (const auto &bucket : table)
    {
      for (const auto &prod : bucket)
      {
        all.push_back(prod);
      }
    }
    sort(all.begin(), all.end(), [](const Produk &a, const Produk &b)
         { return a.namaProduk < b.namaProduk; });

    return all;
  }
  void listByNama()
  {
    vector<Produk> listBerdasarkanNama = getSemuaProdukBerdasarkanNama();
    for (const auto &prod : listBerdasarkanNama)
    {
      cout << "Nama: " << prod.namaProduk << " | ID: " << prod.idProduk
           << " | Harga: " << prod.hargaProduk << " | Stok: " << prod.stokProduk
           << " | Sales: " << prod.sales << " | Rating: " << prod.ratingProduk << "\n";
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

    file << "ID,Nama,Harga,Stok,Sales,Rating\n";
    for (const auto &bucket : table)
    {
      for (const auto &prod : bucket)
      {
        file << prod.idProduk << "," << prod.namaProduk << "," << prod.hargaProduk << ","
             << prod.stokProduk << "," << prod.sales << "," << prod.ratingProduk << "\n";
      }
    }

    file.close();
    cout << "Data berhasil disimpan ke " << filename + ".csv\n";
  }

  void loadFromCsv(const string &filename)
  {
    ifstream file(filename + ".csv");
    if (!file)
    {
      cerr << "Gagal membuka file " << filename << ".csv\n";
      return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line))
    {
      stringstream ss(line);
      string idStr, nama, hargaStr, stokStr, salesStr, ratingStr;

      getline(ss, idStr, ',');
      getline(ss, nama, ',');
      getline(ss, hargaStr, ',');
      getline(ss, stokStr, ',');
      getline(ss, salesStr, ',');
      getline(ss, ratingStr, ',');

      if (idStr.empty() || nama.empty() || hargaStr.empty() || stokStr.empty() || salesStr.empty() || ratingStr.empty())
      {
        cerr << "Baris tidak lengkap: " << line << "\n";
        continue;
      }

      try
      {
        int id = stoi(idStr);
        double harga = stod(hargaStr);
        int stok = stoi(stokStr);
        int sales = stoi(salesStr);
        double rating = stod(ratingStr);

        Produk p(id, nama, harga, stok, sales, rating);
        insert(p);
      }
      catch (const exception &e)
      {
        cerr << "Gagal parsing baris: " << line << "\n";
        cerr << "Error: " << e.what() << "\n";
      }
    }

    file.close();
    cout << "Data berhasil dimuat dari " << filename + ".csv\n";
  }
};
#endif