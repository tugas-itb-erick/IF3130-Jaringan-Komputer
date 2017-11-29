# Distance Vector Simulation
Tugas Besar 2 IF3130 Jaringan Komputer

## Kelompok Flabby
1. 13515057 - Erick Wijaya
2. 13515063 - Kezia Suhendra
3. 13515111 - Catherine Almira

### Petunjuk Penggunaan Program

### Penjelasan Program

### Jawaban dari Pertanyaan
1. Apakah perbedaan dari routing protocol distance-vector dan link state? Manakah yang lebih baik untuk digunakan?
Pada distance vector, informasi untuk routing hanya dilakukan pertukaran antar tetangga yang terhubung secara langsung. Router tidak dapat melihat lebih dari node yang bertetangga langsung dengan dirinya. Sedangkan pada link state, router mengetahui semua jalur yang dapat dilalui oleh router lain pada jaringan tersebut. Link state lebih baik untuk digunakan karena lebih robust operation dan scalability.

2. Pada implementasinya saat ini manakah yang lebih banyak digunakan, distance-vector atau link state? Kenapa?
Link state lebih banyak digunakan karena router dapat mengetahui seluruh jalur dari router lainnya sehingga setiap router dapat membuat suatu *shortest path tree* dari router itu sendiri ke router lainnya. Namun, link state lebih robust jika dioperasikan sehingga link state lebih banyak digunakan.

### Pembagian Tugas
1. Erick Wijaya
2. Kezia Suhendra
3. Catherine Almira