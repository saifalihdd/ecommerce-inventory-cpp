#include <iostream>
#include <limits>
#include "hash_table.hpp"
#include "avl_tree.hpp"
#include "bst.hpp"
#include "delivery_graph.hpp"
using namespace std;

int partition(vector<Produk>& arr, int low, int high) {
    double pivot = arr[high].hargaProduk;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j].hargaProduk < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<Produk>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void tampilkanProdukTerurut(const vector<Produk>& data) {
    for (const auto& p : data) {
        cout << "ID: " << p.idProduk << endl;
        cout << "Nama: " << p.namaProduk << endl;
        cout << "Harga: " << p.hargaProduk << endl;
        cout << "Stok: " << p.stokProduk << endl;
        cout << "Sales: " << p.sales << endl;
        cout << "Rating: " << p.ratingProduk << endl;
    }
}

void menuProduk(HashTable &lookup, AVLTree &avl)
{
  int pilihan;
  do
  {
    cout << "\n=== MENU PRODUK ===\n";
    cout << "1. Tambah Produk\n";
    cout << "2. Cari Produk\n";
    cout << "3. Hapus Produk\n";
    cout << "4. Tampilkan Semua Produk\n";
    cout << "5. Tampilkan semua produk By nama\n";
    cout << "6. Tampilkan Produk Urut Harga (Quick Sort)\n";    
    cout << "7. Simpan ke CSV\n";
    cout << "8. Kembali ke Menu Utama\n";

    cout << "Pilih: ";
    cin >> pilihan;

    if (cin.fail())
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Input tidak valid. Harap masukkan angka.\n";
      continue;
    }

    switch (pilihan)
    {
    case 1:
    {
      int id, stok, sales;
      double harga, rating;
      string nama;
      cout << "ID: ";
      cin >> id;
      if (lookup.search(id))
      {
        cout << "Produk dengan ID tersebut sudah ada.\n";
        break;
      }
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      cout << "Nama: ";
      getline(cin, nama);
      cout << "Harga: ";
      cin >> harga;
      cout << "Stok: ";
      cin >> stok;
      cout << "Sales: ";
      cin >> sales;
      cout << "Rating: ";
      cin >> rating;
      Produk p(id, nama, harga, stok, sales, rating);
      lookup.insert(p);
      avl.insert(p);
    }
    break;
    case 2:
    {
      int id;
      cout << "Masukkan id produk yang ingin dicari: ";
      cin >> id;
      Produk *p = lookup.search(id);
      if (p)
        cout
            << "Ditemukan: " << p->namaProduk << ", Harga: " << p->hargaProduk << "\n";
      else
        cout << "Produk tidak ditemukan.\n";
    }
    break;
    case 3:
    {
      int id;
      cout << "Masukkan id produk yang ingin dihapus: ";
      cin >> id;
      lookup.remove(id);
      cout << "Produk dihapus dari hash table (AVL tidak dihapus).\n";
    }
    break;
    case 4:
    {
      lookup.printAll();
    }
    break;
    case 5:
    {
      lookup.listByNama();
    }
    break;
    case 6:
    {
      vector<Produk> semuaProduk = lookup.getAll(); 
      if (semuaProduk.empty()) {
          cout << "Tidak ada produk yang tersedia.\n";
          break;
      }
      quickSort(semuaProduk, 0, semuaProduk.size() - 1);
      cout << "\nProduk diurutkan berdasarkan harga (Quick Sort):\n";
      tampilkanProdukTerurut(semuaProduk);
    }
    break;
    case 7:
    {
      lookup.saveToCsv("produk");
      avl.saveToCSV("riwayatProduk");
    }
    break;

    default:
      system("cls");
      break;
    }
  } while (pilihan != 8);
}

void menuRiwayat(AVLTree &avl)
{
  cout << "\n=== Riwayat Produk (Harga - Inorder) ===\n";
  avl.displayInOrder();
}

void menuPemesanan(OrderBst &orders)
{

  int pilihan;
  do
  {
    cout << "\n=== MENU PEMESANAN ===\n";
    cout << "1. Tambah Order\n"
         << "2. Tampilkan Riwayat Order\n"
         << "3. Simpan Riwayat order\n"
         << "4. Kembali ke Menu Utama\n";
    cout << "Pilihan: ";
    cin >> pilihan;

    switch (pilihan)
    {
    case 1:
    {
      int orderId, produkId, qty;
      string date;
      cout << "ID Order: ";
      cin >> orderId;
      cout << "ID Produk: ";
      cin >> produkId;
      cin.ignore();
      cout << "Tanggal (YYYY-MM-DD): ";
      getline(cin, date);
      cout << "Jumlah: ";
      cin >> qty;
      orders.insert(Order(orderId, produkId, date, qty));
    }
    break;
    case 2:

      orders.displayRiwayat();
      break;
    case 3:
      orders.saveToCsv("order");
      break;
    default:
      system("cls");
      break;
    }
  } while (pilihan != 4);
}

void menuPengiriman(DeliveryGraph &graph)
{
  int pilihan;
  string dari, ke;
  double jarak;
  do
  {
    cout << "\n=== MENU PENGIRIMAN ===\n";
    cout << "1. Tambah Rute\n";
    cout << "2. Lihat Rute (BFS)\n";
    cout << "3. Lihat Rute (DFS)\n";
    cout << "4. Lihat semua rute\n";
    cout << "5. Simpan Semua Rute\n";
    cout << "6. Kembali ke Menu Utama\n";
    cout << "Pilih: ";
    cin >> pilihan;

    switch (pilihan)
    {
    case 1:
    {
      cout << "Dari: ";
      cin >> dari;
      cout << "Ke: ";
      cin >> ke;
      cout << "Jarak dalam KM: ";
      cin >> jarak;
      graph.addEdge(dari, ke, jarak);
    }
    break;
    case 2:
    {
      cout << "Mulai dari: ";
      cin >> dari;
      graph.bfs(dari);
    }
    break;
    case 3:
    {
      cout << "Mulai dari: ";
      cin >> dari;
      graph.dfs(dari);
    }
    break;
    case 4:
      graph.displayRoutes();
      break;
    case 5:
      graph.saveToCsv("rute");
      break;
    default:
      system("cls");
      break;
    }
  } while (pilihan != 6);
}


int main()
{
  HashTable lookup;
  AVLTree riwayatProduk;
  OrderBst orderTree;
  DeliveryGraph pengiriman;

  // Muat dari CSV jika ada
  lookup.loadFromCsv("produk");
  orderTree.loadFile("order");
  riwayatProduk.loadFromCSV("riwayatProduk");
  pengiriman.loadFromCsv("rute");
  int pilihan;
  do
  {
    cout << "\n=== SISTEM E-COMMERCE ===\n";
    cout << "1. Kelola Produk\n";
    cout << "2. Lihat Riwayat Produk (AVLTree)\n";
    cout << "3. Kelola Pemesanan\n";
    cout << "4. Kelola Pengiriman\n";
    cout << "5. Keluar\n";
    cout << "Pilih: ";
    cin >> pilihan;

    switch (pilihan)
    {
    case 1:
      menuProduk(lookup, riwayatProduk);
      break;
    case 2:
      menuRiwayat(riwayatProduk);
      break;
    case 3:
      menuPemesanan(orderTree);
      break;
    case 4:
      menuPengiriman(pengiriman);
      break;
    case 5:
      cout << "Keluar dari program.\n";
      break;
    default:
      cout << "Pilihan tidak valid.\n";
    }

  } while (pilihan != 5);

  return 0;
}