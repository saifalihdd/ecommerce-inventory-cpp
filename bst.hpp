#ifndef BST_HPP
#define BST_HPP
#include "struct.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct OrderNode
{
  Order data;
  OrderNode *left;
  OrderNode *right;

  OrderNode(const Order &data) : data(data), left(nullptr), right(nullptr) {}
};

class OrderBst
{
private:
  OrderNode *root;

  OrderNode *insert(OrderNode *node, const Order &order)
  {
    if (!node)
      return new OrderNode(order);
    if (order.orderId < node->data.orderId)
      node->left = insert(node->left, order);
    else
      node->right = insert(node->right, order);
    return node;
  }

  void inOrder(OrderNode *node)
  {
    if (!node)
      return;
    inOrder(node->left);
    cout << "OrderID: " << node->data.orderId
         << ", ProdukID: " << node->data.produkId
         << ", Date: " << node->data.date
         << ", Qty: " << node->data.qty << "\n";
    inOrder(node->right);
  }

  void saveInOrder(OrderNode *node, ofstream &file)
  {
    if (!node)
      return;
    saveInOrder(node->left, file);
    file << node->data.orderId << ","
         << node->data.produkId << ","
         << node->data.date << ","
         << node->data.qty << "\n";
    saveInOrder(node->right, file);
  }

public:
  OrderBst() : root(nullptr) {}

  void insert(const Order &order)
  {
    root = insert(root, order);
    cout << "Pesanan berhasil ditambahkan.\n";
  }

  void displayRiwayat()
  {
    cout << "Riwayat pesanan:\n";
    inOrder(root);
  }

  void saveToCsv(const string &filename)
  {
    ofstream file(filename + ".csv");
    if (!file)
    {
      cerr << "Gagal membuka file untuk ditulis.\n";
      return;
    }

    file << "OrderID,ProdukID,Date,Qty\n";
    saveInOrder(root, file);
    file.close();
    cout << "Riwayat pemesanan berhasil disimpan ke " << filename << ".csv\n";
  }

  void loadFile(const string &filename)
  {
    ifstream in(filename + ".csv");
    if (!in)
    {
      cerr << "Gagal membuka file untuk dibaca\n";
      return;
    }

    string line;
    getline(in, line); // Skip header
    while (getline(in, line))
    {
      stringstream ss(line);
      string orderIdStr, produkIdStr, dateStr, qtyStr;

      getline(ss, orderIdStr, ',');
      getline(ss, produkIdStr, ',');
      getline(ss, dateStr, ',');
      getline(ss, qtyStr, ',');

      if (orderIdStr.empty() || produkIdStr.empty() || dateStr.empty() || qtyStr.empty())
      {
        cerr << "Baris tidak valid: " << line << "\n";
        continue;
      }

      try
      {
        int orderId = stoi(orderIdStr);
        int produkId = stoi(produkIdStr);
        int qty = stoi(qtyStr);

        Order p(orderId, produkId, dateStr, qty);
        insert(p);
      }
      catch (exception &e)
      {
        cerr << "Gagal parsing baris: " << line << "\n";
        cerr << "Error: " << e.what() << "\n";
      }
    }

    in.close();
    cout << "Riwayat pesanan berhasil dimuat dari " << filename << ".csv\n";
  }
};

#endif
