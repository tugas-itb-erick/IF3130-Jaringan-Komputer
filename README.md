# CIDR Calculator
Tugas Kecil 1 IF3130 Jaringan Komputer

## Petunjuk Penggunaan
1. Ekstrak zip
2. Ketik ```make run```
3. Apabila kena error ```permission denied```, ketik ```chmod +x src/client.py``` lalu ulangi langkah 2
4. Input NIM
5. Program berhasil dirun

## Penjelasan Singkat
Program terbagi menjadi 3 phase. Berikut adalah penjelasan dari setiap phase

### Phase 1
Tugas dari phase 1 adalah mencari sebuah subnet valid untuk <i>host</i> yang disediakan. Fungsi yang diimplementasi adalah ```def getValidSubnet(host)```. Karena diminta mencari subnet yang valid (dan tidak ada batasan bagi subnet yang diinginkan), maka cara termudahnya adalah menggunakan subnet ```0.0.0.0/0``` karena subnet tersebut sudah dipastikan valid untuk semua host. 

### Phase 2
Tugas dari phase 2 adalah menghitung jumlah host valid untuk sebuah <i>subnet</i> yang disediakan (termasuk <i>broadcast</i> dan <i>gateway</i>). Fungsi yang diimplementasi adalah ```def countHosts(subnet)```. Jumlah host yang valid dapat dihitung dengan menggunakan rumus 2<sup>32-<i>CIDR Mask</i></sup>. <i>CIDR Mask</i> adalah angka yang berada pada bagian belakang tanda slash pada <i>subnet</i>. Rumus tersebut menghasilkan jumlah host yang diinginkan karena kita cukup perlu menghitung semua kemungkinan nilai bit untuk bit setelah <i>CIDR Mask</i>, yaitu dua pangkat dari panjang bit setelah <i>CIDR Mask</i>. 

### Phase 3
Tugas dari phase 3 adalah mengecek apakah suatu <i>host</i> berada pada <i>subnet</i> yang diberikan. Fungsi yang diimplementasi adalah ```def isSubnetValid(subnet, host)```. Langkah pertama yang dilakukan adalah mengkonversi <i>host</i> dan <i>subnet</i> menjadi bitstring sepanjang 32 bit. Setelah itu pengecekan dilakukan pada bit pertama sampai <i>CIDR Mask</i> untuk bitstring <i>subnet</i> dan <i>host</i>. Apabila ada nilai bit yang berbeda, artinya <i>host</i> tidak berada pada <i>subnet</i> yang diberikan. Apabila semua nilai bit sama, <i>host</i> berada pada <i>subnet</i> yang diberikan, tanpa mempedulikan bit setelah <i>CIDR Mask</i>. 
