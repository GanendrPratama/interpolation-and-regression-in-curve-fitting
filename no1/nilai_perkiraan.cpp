#include <iostream>     // library input dan output standar cpp
#include <vector>       // library container vector
#include <cmath>        // library fungsi matematika
#include <iomanip>      // library format output angka

using namespace std;

// fungsi untuk menghitung regresi polinomial orde 3
void regresiPolinomial(const vector<double>& x, const vector<double>& y, double koefisien[4]) {
    // bagian inisialisasi. x = tahun, y = populasi
    int n = x.size(); // jumlah data "n"

    // cek apakah ukuran x dan y sama dan handling error
    if (n != y.size()) {
        cerr << "Ukuran x dan y tidak sama." << endl; // jika ukuran x dan y tidak sama
        return; // keluar dari fungsi dan program tidak melakukan perhitungan
    }

    double X[7] = {0}; // array penyimpanan jumlah x^0 sampai x^6
    double Y[4] = {0}; // array penyimpanan jumlah y * x^0 sampai y * x^3
    double B[4][5] = {0}; // matriks augmented untuk sistem persamaan linear menggunakan metode eliminasi Gauss

    // hitung jumlah X dan Y
    for (int i = 0; i < n; ++i) {
        double xi = 1.0; // x^0 = 1 pada iterasi pertama
        for (int j = 0; j <= 6; ++j) {
            if (j <= 3) Y[j] += y[i] * xi; // hitung jumlah y*x^j
            X[j] += xi; // hitung jumlah x^j
            xi *= x[i]; // reinitialize xi menjadi x^j berikutnya
        }
    }

    // susun matriks koefisien normal untuk sistem persamaan linear
    for (int i = 0; i <= 3; ++i) {
        for (int j = 0; j <= 3; ++j) {
            B[i][j] = 0; // inisialisasi elemen matriks
            for (int k = 0; k < n; ++k)
                B[i][j] += pow(x[k], i + j); // elemen baris ke-i kolom ke-j: jumlah x^(i+j)
        }
        B[i][4] = Y[i]; // last column adalah jumlah y*x^i
    }

    // eliminasi Gauss
    for (int i = 0; i <= 3; ++i) {
        for (int k = i + 1; k <= 3; ++k) {
            double t = B[k][i] / B[i][i]; // hitung rasio baris
            for (int j = 0; j <= 4; ++j)
                B[k][j] -= t * B[i][j]; // eliminasi elemen di bawah diagonal
        }
    }

    // substitusi mundur untuk mendapatkan koefisien
    for (int i = 3; i >= 0; --i) {
        koefisien[i] = B[i][4]; // inisialisasi koefisien dengan nilai dari kolom terakhir
        for (int j = i + 1; j <= 3; ++j)
            koefisien[i] -= B[i][j] * koefisien[j]; // kurangi elemen di atas diagonal
        koefisien[i] /= B[i][i]; // bagi dengan elemen diagonal untuk mendapatkan koefisien
    }
}

int main() {
    // inisialisasi data tahun, populasi, dan persentase pengguna internet
    vector<double> tahun_tahun = {2000,2001,2002,2003,2004,2007,2008,2009,2010,2011,2012,2013,2014,2017,2018};
    vector<double> populasi = {211540000,214880000,218030000,221100000,224100000,230000000,234000000,237000000,240000000,243000000,246000000,249000000,252000000,258000000,261000000};     
    vector<double> persentase_internet = {0.9, 1.1, 1.3, 1.5, 1.8, 3.5, 4.2, 5.0, 6.0, 8.0, 10.5, 15.0, 19.8, 40.0, 47.0};
    // populasi diambil dari data tahun 2000-2018

    double koef_populasi[4], koef_internet[4]; // array untuk menyimpan koefisien regresi polinomial
    
    // pemanggilan fungsi penghitung regresi untuk populasi dan pengguna internet
    regresiPolinomial(tahun_tahun, populasi, koef_populasi);
    regresiPolinomial(tahun_tahun, persentase_internet, koef_internet);

    cout << fixed << setprecision(5); // format output angka dengan 2 desimal
    cout << "=== Koefisien Regresi Populasi ===\n";
    for (int i = 0; i < 4; ++i)
        cout << "a" << i << " = " << koef_populasi[i] << endl; // menampilkan koefisien regresi populasi

    cout << "\n=== Koefisien Regresi Persentase Pengguna Internet ===\n";
    for (int i = 0; i < 4; ++i)
        cout << "b" << i << " = " << koef_internet[i] << endl; // menampilkan koefisien regresi persentase pengguna internet

    // tahun-tahun yang akan diprediksi
    vector<int> prediksi_tahun = {2005, 2006, 2015, 2016};

    cout << "\n=== Estimasi Tahun yang Hilang ===\n";
    for (int tahun : prediksi_tahun) {
        double est_populasi = koef_populasi[0] + koef_populasi[1]*tahun + koef_populasi[2]*pow(tahun,2) + koef_populasi[3]*pow(tahun,3); // estimasi populasi
        double est_persen = koef_internet[0] + koef_internet[1]*tahun + koef_internet[2]*pow(tahun,2) + koef_internet[3]*pow(tahun,3); // estimasi persentase pengguna internet

        // tampilkan hasil estimasi
        cout << "Tahun " << tahun << ":\n";
        cout << "  Populasi = " << (long long)est_populasi << endl;
        cout << "  % Pengguna Internet = " << est_persen << "%" << endl;
    }
    
    return 0;
}