import numpy as np
import matplotlib.pyplot as plt

# =============================
# KOEFISIEN REGRESI POLINOMIAL ORDE 3
# =============================

# Koefisien populasi (dari hasil program C++)
a0_pop = -10182056898958.94922
a1_pop = 15178883958.25156
a2_pop = -7543763.53367
a3_pop = 1249.94434

# Koefisien pengguna internet (dari hasil program C++)
b0_net = 0.55347
b1_net = 0.82056
b2_net = -0.17983
b3_net = 0.01555

# =============================
# DEFINISI FUNGSI REGRESI
# =============================
def poly_pop(x):
    return a0_pop + a1_pop * x + a2_pop * x**2 + a3_pop * x**3

def poly_net(x):
    return b0_net + b1_net * x + b2_net * x**2 + b3_net * x**3

# =============================
# DATA ASLI
# =============================
years = np.array([2000,2001,2002,2003,2004,2007,2008,2009,2010,2011,2012,2013,2014,2017,2018])

pop = np.array([211540000,214880000,218030000,221100000,224100000,
                230000000,234000000,237000000,240000000,243000000,
                246000000,249000000,252000000,258000000,261000000])

internet = np.array([0.9, 1.1, 1.3, 1.5, 1.8,
                     3.5, 4.2, 5.0, 6.0, 8.0,
                     10.5, 15.0, 19.8, 40.0, 47.0])

# =============================
# ESTIMASI TAHUN YANG HILANG
# =============================
missing_years = np.array([2005, 2006, 2015, 2016])
pop_estimated = [226468653, 229259233, 253330546, 256026641]
net_estimated = [-2.65715, -1.96461, 28.17787, 33.06986]

# =============================
# PLOTTING
# =============================
x_plot = np.linspace(2000, 2020, 300)

fig, axs = plt.subplots(2, 1, figsize=(10, 10))

# Grafik Populasi
axs[0].scatter(years, pop, color='blue', label='Data Asli Populasi')
axs[0].plot(x_plot, poly_pop(x_plot), color='red', label='Regresi Populasi')
axs[0].scatter(missing_years, pop_estimated, color='black', marker='x', s=80, label='Estimasi Tahun Hilang')
axs[0].set_xlabel('Tahun')
axs[0].set_ylabel('Populasi')
axs[0].set_title('Estimasi Populasi Indonesia')
axs[0].legend()
axs[0].grid()

# Grafik Pengguna Internet
axs[1].scatter(years, internet, color='green', label='Data Asli Pengguna Internet')
axs[1].plot(x_plot, poly_net(x_plot - 2000), color='orange', label='Regresi Pengguna Internet')
axs[1].scatter(missing_years, poly_net(missing_years - 2000), color='black', marker='x', s=80, label='Estimasi Tahun Hilang')
axs[1].set_xlabel('Tahun')
axs[1].set_ylabel('Persentase (%)')
axs[1].set_title('Estimasi Pengguna Internet Indonesia')
axs[1].legend()
axs[1].grid()

# Tata letak dan simpan
plt.tight_layout()
plt.savefig("hasil_estimasi.png")
print("Gambar disimpan sebagai 'hasil_estimasi.png'")