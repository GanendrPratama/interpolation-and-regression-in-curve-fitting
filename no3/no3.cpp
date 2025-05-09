#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Function to calculate Lagrange Interpolation
double lagrangeInterpolation(const vector<int>& tahun, const vector<double>& values, int targetTahun) {
    double result = 0.0;
    int n = tahun.size();

    for (int i = 0; i < n; i++) {
        double term = values[i];
        for (int j = 0; j < n; j++) {
            if (i != j) {
                term *= (double)(targetTahun - tahun[j]) / (tahun[i] - tahun[j]);
            }
        }
        result += term;
    }

    return result;
}

// Fungsi  untuk kalkulasi regrewsi dengan dengan sigmoid mode
double Regresi(double t, double L, double k, double t0) {
    return L / (1 + exp(-k * (t - t0)));
}

int main() {
    // data untuk populasi
    vector<int> tahun_populasi = {2000, 2010, 2020, 2023};
    vector<double> populasi = {216077790, 246305322, 274814866, 281190067};

    // data untuk pengguna persen internet
    vector<int> tahun_internet = {2000, 2010, 2020, 2023};
    vector<double> internet_persen = {0.925564, 10.92, 53.7265, 69.2084};

    // estimasi populasi di 2030 menggunakan interpolasi Lagrange
    int targetTahun_population = 2030;
    double estimated_populasi = lagrangeInterpolation(tahun_populasi, populasi, targetTahun_population);
    cout << fixed << "Estimasi populasi di " << targetTahun_population << ": " << estimated_populasi << endl;

    // Parameter regresi logistik untuk persentase pengguna Internet
    double L = 100.0;  // batas atasnya (100%)
    double k = 0.2;    // laju pertumbuhan 
    double t0 = 2015;  // titik tengah 

    // Estimasi persen pengguna Internet di 2035 menggunakan regresi logistik
    int targettahuninternet = 2035;
    double estimated_persenInternet = Regresi(targettahuninternet, L, k, t0);

    cout << "Estimasi persen pengguna internet di " << targettahuninternet << ": " << estimated_persenInternet << "%" << endl;

    // Jumlah pengguna Internet di Indonesia di tahun 2035
    double estimated_populasi_2035 = lagrangeInterpolation(tahun_populasi, populasi, 2035);
    double estimated_penggunainternet = (estimated_persenInternet / 100.0) * estimated_populasi_2035;
    cout << "Estimasi pengguna internet di tahun " << targettahuninternet << ": " << estimated_penggunainternet << endl;

    return 0;
}