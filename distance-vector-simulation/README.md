# Distance Vector Simulation
Tugas Besar 2 IF3130 Jaringan Komputer

## Kelompok Flabby
1. [13515057 - Erick Wijaya](https://github.com/wijayaerick)
2. [13515063 - Kezia Suhendra](https://github.com/keziasuhendra)
3. [13515111 - Catherine Almira](https://github.com/calmira)

### Petunjuk Penggunaan Program
1. Lakukan kompilasi program dan jalankan eksekusi program dengan menggunakan *command* berikut pada terminal atau *command prompt*.
		```
        make
        ```
2. Masukan input jumlah node dan jumlah edge.
3. Masukan seluruh node dan tetangganya sejumlah edge.
4. Masukan input jumlah skenario.
5. Masukan node pengirim dan penerima informasi sejumlah skenario.

### Penjelasan Program
Pertama-tama lakukan inisialisasi seluruh nilai matriks distance dengan -1 kecuali dirinya sendiri dengan 0 dan nextHop dengan -1. Kemudian baca node-node yang berhubungan (bertetangga). Node yang berhubungan diberi nilai distance 1 dan nextHop diisi dengan nilai node tetangganya. Selanjutnya baca jumlah skenario. Lakukan update nilai distance dan nextHop sebanyak jumlah skenario dengan menambahkan informasi dari vektor tersebut terhadap vektor dirinya. Kemudian print distance dan nexthop setelah dilakukan update. Ketika sebuah node mendapat informasi vektor dari tetangganya, lalu menemukan bahwa salah satu nilai cost dalam vektor yang dimiliki tetangga lebih kecil daripada cost yang ia ketahui atau cost bernilai sama tetapi nomor tetangga yang dimiliki lebih kecil, maka node akan merubah vektornya menjadi cost yang baru.

### Jawaban dari Pertanyaan
1. Apakah perbedaan dari routing protocol distance-vector dan link state? Manakah yang lebih baik untuk digunakan?
Pada distance vector, informasi untuk routing hanya dilakukan pertukaran antar tetangga yang terhubung secara langsung. Router tidak dapat melihat lebih dari node yang bertetangga langsung dengan dirinya. Sedangkan pada link state, router mengetahui semua jalur yang dapat dilalui oleh router lain pada jaringan tersebut. Link state lebih baik untuk digunakan pada internal network karena lebih robust dalam hal operation dan scalability.

2. Pada implementasinya saat ini manakah yang lebih banyak digunakan, distance-vector atau link state? Kenapa?
Link state lebih banyak digunakan dalam internal network berskala besar karena router dapat mengetahui seluruh jalur dari router lainnya sehingga setiap router dapat membuat suatu *shortest path tree* dari router itu sendiri ke router lainnya. Namun, link state lebih robust jika dioperasikan sehingga link state lebih banyak digunakan.

### Pembagian Tugas
Kami mengerjakannya bersama-sama sehingga pembagiannya menjadi sebagai berikut.
1. Erick Wijaya : Inisialisasi, update, print matriks, dan readme.
2. Kezia Suhendra : Inisialisasi, update, print matriks, dan readme.
3. Catherine Almira : Inisialisasi, update, print matriks, dan readme.
