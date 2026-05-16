#ifndef PRODUK_HPP
#define PRODUK_HPP

#include <string>
using namespace std;
struct Produk
{
  int idProduk;
  string namaProduk;
  double hargaProduk;
  int stokProduk;
  int sales;
  double ratingProduk;

  Produk(int id, string nama, double harga, int stok, int sales, double rating) : idProduk(id), namaProduk(nama), hargaProduk(harga), stokProduk(stok), sales(sales), ratingProduk(rating) {}
};

struct Order
{
  int orderId;
  int produkId;
  string date;
  int qty;

  Order(int orderId = 0, int produkId = 0, string date = "", int qty = 0) : orderId(orderId), produkId(produkId), date(date), qty(qty) {}
};

#endif