#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include "struct.hpp"
using namespace std;

struct AVLNode
{
  Produk data;
  AVLNode *left;
  AVLNode *right;
  int height;

  AVLNode(const Produk &p) : data(p), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree
{
private:
  AVLNode *root;
  int getHeight(AVLNode *node)
  {
    return node ? node->height : 0;
  }
  int getBalance(AVLNode *node)
  {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
  }

  AVLNode *leftrotate(AVLNode *x)
  {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    return y;
  }

  AVLNode *rightrotate(AVLNode *y)
  {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    return x;
  }

  AVLNode *insert(AVLNode *node, const Produk &key)
  {
    if (!node)
      return new AVLNode(key);
    if (key.hargaProduk < node->data.hargaProduk)
    {
      node->left = insert(node->left, key);
    }
    else if (key.hargaProduk > node->data.hargaProduk)
    {
      node->right = insert(node->right, key);
    }
    else
    {
      return node;
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && key.hargaProduk < node->left->data.hargaProduk)
    {
      return rightrotate(node);
    }
    if (balance < -1 && key.hargaProduk > node->right->data.hargaProduk)
    {
      return leftrotate(node);
    }
    if (balance > 1 && key.hargaProduk > node->left->data.hargaProduk)
    {
      node->left = leftrotate(node->left);
      return rightrotate(node);
    }
    if (balance < -1 && key.hargaProduk < node->right->data.hargaProduk)
    {
      node->right = rightrotate(node->right);
      return leftrotate(node);
    }

    return node;
  }

  void inOrder(AVLNode *node)
  {
    if (node)
    {
      inOrder(node->left);
      cout << "[" << node->data.namaProduk << "] Harga: " << node->data.hargaProduk << ", Stok: " << node->data.stokProduk << "\n";
      inOrder(node->right);
    }
  }
  void saveInOrderToCSV(AVLNode *node, ofstream &file)
  {
    if (!node)
      return;

    saveInOrderToCSV(node->left, file);
    file << node->data.idProduk << ","
         << node->data.namaProduk << ","
         << node->data.hargaProduk << ","
         << node->data.stokProduk << ","
         << node->data.sales << ","
         << node->data.ratingProduk << "\n";
    saveInOrderToCSV(node->right, file);
  }

public:
  AVLTree() : root(nullptr) {}
  void insert(const Produk &key)
  {
    root = insert(root, key);
  }

  void displayInOrder()
  {
    cout << "Produk berdasarkan harga (in-order):\n";
    inOrder(root);
  }

  void saveToCSV(const string &filename)
  {
    ofstream file(filename + ".csv");
    if (!file)
    {
      cerr << "Gagal membuka file untuk ditulis.\n";
      return;
    }

    // Tulis header
    file << "ID,Nama,Harga,Stok,Sales,Rating\n";
    saveInOrderToCSV(root, file);
    file.close();
  }
  void loadFromCSV(const string &filename)
  {
    ifstream file(filename + ".csv");
    if (!file)
    {
      cerr << "Gagal membuka file untuk dibaca.\n";
      return;
    }

    string line;
    getline(file, line); // Lewati header

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

      Produk p(stoi(idStr), nama, stod(hargaStr), stoi(stokStr), stoi(salesStr), stod(ratingStr));
      insert(p);
    }

    file.close();
    cout << "Data berhasil dimuat dari " << filename + ".csv\n";
  }
};

#endif