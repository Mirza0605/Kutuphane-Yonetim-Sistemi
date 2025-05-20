#  Kitap Yönetim Sistemi – C++ Uygulaması

Bu proje, **Veri Yapıları** dersi kapsamında geliştirilmiş çok modüllü bir **Kitap Yönetim Sistemi** simülasyonudur. Personel ve kullanıcı rollerine sahip iki farklı arayüz üzerinden kitap yönetimi, ödünç alma, kitap puanlama ve talepleri organize etme işlemleri yapılabilir.

---

##  Kullanılan Veri Yapıları

###  Tek Yönlü Bağlı Liste
-  **Kitap listesi** bu yapıda tutulur.
- Kitap ekleme, silme, güncelleme işlemleri bu yapı üzerinde uygulanır.

###  Çift Yönlü Dairesel Liste
-  **Kullanıcı koleksiyonu** bu yapı ile modellenir.
- Koleksiyona kitap ekleme, okundu işaretleme ve silme işlemleri desteklenir.

###  Tek Yönlü Dairesel Liste
-  **Ödünç alınan kitaplar** bu yapı ile tutulur.
- Kitapların ödünç alım ve iade zamanları bu yapıyla birlikte kaydedilir.

###  İkili Arama Ağacı (BST)
-  **Odünç kitap geçmişi**, ağaç yapısına eklenerek analiz edilir.
- Raporlamalar:
  - 30 gün üzerinde ödünç alınanlar
  - 1950 öncesi basılanlar
  - Hem 1950 öncesi hem 30+ gün olanlar

###  Kuyruk (Queue)
-  **Kitap talepleri**, FIFO mantığıyla işlenir.
- Akademik talepler kuyruğun başına alınarak önceliklendirilir.

###  Yığın (Stack)
-  **Odünç alınan kitaplar**, kullanıcı tarafında bu yapıda tutulur.
- Kitaplar LIFO mantığıyla iade edilir.

###  Hash Tablolar ve Map
-  Kitap puanlama sistemi `map<int, float>` ile tutulur.
-  Kitap yorumları `map<int, string>` yapısıyla eşlenir.
