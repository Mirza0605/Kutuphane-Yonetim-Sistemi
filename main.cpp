#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <ctime>
#include <algorithm>
using namespace std;

// Tek Yonlu Bagli Liste - Personel icin
struct KitapNode {
    int id;
    string isim;
    int sayfaSayisi;
    int basimYili;
    bool hasarli;
    string hasarNotu;
    KitapNode* next;
};
struct OduncKitapNode {
    int id;
    string isim;
    time_t almaZamani;
    time_t teslimZamani;
    OduncKitapNode* next;
};

OduncKitapNode* oduncHead = nullptr;



// Cift Yonlu Dairesel Liste - Kullanici icin
struct KullaniciKitap {
    int id;
    string isim;
    bool okundu;
    KullaniciKitap* next;
    KullaniciKitap* prev;
};

// Agac Yapisi - Odunc Alinan Kitaplar
struct AgacNode {
    int id;
    string isim;
    int basimYili;
    time_t almaZamani;
    AgacNode* left;
    AgacNode* right;
    string yayinevi;
    string dil;
    int sayfaSayisi;

};


// Hash Tablo - Kitap Puanlamasi
map<int, float> kitapPuanlari;

// Yorumlar
map<int, string> yorumlar;

// Queue - Talep Edilen Kitaplar
queue<pair<int, string>> talepSirasi;

// Stack - Odunc Alinan Kitaplar
stack<pair<int, string>> oduncKitaplar;

// Global kullanici koleksiyonu
KullaniciKitap* kullaniciHead = nullptr;

void oduncKitapListeyeEkle(OduncKitapNode*& head, int id, const string& isim) {
    time_t simdi = time(0);
    OduncKitapNode* yeni = new OduncKitapNode{ id, isim, simdi, 0, nullptr };

    if (!head) {
        head = yeni;
        head->next = head;  // dairesel yapý
    }
    else {
        OduncKitapNode* temp = head;
        while (temp->next != head)
            temp = temp->next;
        temp->next = yeni;
        yeni->next = head;
    }
}

void oduncKitapRaporuYazdir(OduncKitapNode* head) {
    OduncKitapNode* temp = head;
    while (temp != nullptr) {
        cout << "Kitap ID: " << temp->id << ", Isim: " << temp->isim << endl;

        char almaBuffer[80];
        strftime(almaBuffer, sizeof(almaBuffer), "%d-%m-%Y %H:%M:%S", localtime(&temp->almaZamani));
        cout << "Alma Zamani: " << almaBuffer << endl;

        if (temp->teslimZamani != 0) {
            char teslimBuffer[80];
            strftime(teslimBuffer, sizeof(teslimBuffer), "%d-%m-%Y %H:%M:%S", localtime(&temp->teslimZamani));
            cout << "Teslim Zamani: " << teslimBuffer << endl;
        }
        else {
            cout << "Teslim edilmemis." << endl;
        }

        cout << "-------------------------" << endl;
        temp = temp->next;
        break;
    }
}


void oduncKitapIadeEt(OduncKitapNode* head, int id) {
    if (!head) {
        cout << "Odunc kitap yok.\n";
        return;
    }
    OduncKitapNode* temp = head;
    do {
        if (temp->id == id) {
            temp->teslimZamani = time(0);
            cout << temp->isim << " iade edildi.\n";
            return;
        }
        temp = temp->next;
    } while (temp != head);
    cout << "Kitap bulunamadi.\n";
}

void kitapEkle(KitapNode*& head, int id, string isim, int sayfaSayisi, int basimYili) {
    KitapNode* yeni = new KitapNode{ id, isim, sayfaSayisi, basimYili, false, "", nullptr };
    if (!head) {
        head = yeni;
    }
    else {
        KitapNode* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = yeni;
    }
}
void kitapDuzenleIsmeGore(KitapNode* head, string eskiIsim, string yeniIsim) {
    while (head) {
        if (head->isim == eskiIsim) {
            head->isim = yeniIsim;
            cout << "Kitap ismi guncellendi.\n";
            return;
        }
        head = head->next;
    }
    cout << "Kitap bulunamadi.\n";
}
void kitapSilIDGore(KitapNode*& head, int id) {
    if (!head) return;
    if (head->id == id) {
        KitapNode* sil = head;
        head = head->next;
        delete sil;
        cout << "Kitap silindi.\n";
        return;
    }
    KitapNode* temp = head;
    while (temp->next && temp->next->id != id)
        temp = temp->next;
    if (temp->next) {
        KitapNode* sil = temp->next;
        temp->next = temp->next->next;
        delete sil;
        cout << "Kitap silindi.\n";
    }
    else {
        cout << "Kitap bulunamadi.\n";
    }
}
void kitaplariZamanDamgaliListele(KitapNode* head) {
    while (head) {
        cout << "ID: " << head->id << ", Isim: " << head->isim
            << ", Basim Yili: " << head->basimYili
            << ", Sayfa Sayisi: " << head->sayfaSayisi << endl;
        head = head->next;
    }
}
void kitapAraVeListele(KitapNode* head, int id) {
    while (head) {
        if (head->id == id) {
            cout << "ID: " << head->id << ", Isim: " << head->isim
                << ", Basim Yili: " << head->basimYili
                << ", Sayfa Sayisi: " << head->sayfaSayisi << endl;
            return;
        }
        head = head->next;
    }
    cout << "Kitap bulunamadi.\n";
}
int kitapSayisiGetir(KitapNode* head) {
    int sayac = 0;
    while (head) {
        sayac++;
        head = head->next;
    }
    return sayac;
}
void kitaplariSayfaSayisinaGoreSirala(KitapNode*& head) {
    if (!head || !head->next) return;
    bool swapped;
    do {
        swapped = false;
        KitapNode* current = head;
        while (current->next) {
            if (current->sayfaSayisi > current->next->sayfaSayisi) {
                swap(current->id, current->next->id);
                swap(current->isim, current->next->isim);
                swap(current->sayfaSayisi, current->next->sayfaSayisi);
                swap(current->basimYili, current->next->basimYili);
                swap(current->hasarli, current->next->hasarli);
                swap(current->hasarNotu, current->next->hasarNotu);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
    cout << "Kitaplar sayfa sayisina gore siralandi.\n";
}
void enEski5KitabiListele(KitapNode* head) {
    vector<KitapNode*> kitaplar;
    while (head) {
        kitaplar.push_back(head);
        head = head->next;
    }
    sort(kitaplar.begin(), kitaplar.end(), [](KitapNode* a, KitapNode* b) {
        return a->basimYili < b->basimYili;
        });
    cout << "En eski 5 kitap:\n";
    for (size_t i = 0; i < kitaplar.size() && i < 5; i++) {
        cout << kitaplar[i]->isim << " (" << kitaplar[i]->basimYili << ")\n";
    }
}

void kitapHasarEkle(KitapNode* head, int id, string hasarNotu) {
    while (head) {
        if (head->id == id) {
            head->hasarli = true;
            head->hasarNotu = hasarNotu;
            cout << "Hasar bilgisi eklendi.\n";
            return;
        }
        head = head->next;
    }
    cout << "Kitap bulunamadi.\n";
}
void hasarliKitaplariYazdir(KitapNode* head) {
    bool bulundu = false;
    while (head) {
        if (head->hasarli) {
            cout << "ID: " << head->id << ", Isim: " << head->isim
                << ", Not: " << head->hasarNotu << endl;
            bulundu = true;
        }
        head = head->next;
    }
    if (!bulundu) {
        cout << "Hasarli kitap yok.\n";
    }
}


void kitapListele(KitapNode* head) {
    KitapNode* temp = head;
    while (temp) {
        cout << "ID: " << temp->id << ", Isim: " << temp->isim << endl;
        temp = temp->next;
    }
}

void kullaniciKitapEkle(KullaniciKitap*& head, int id, const string& isim) {
    KullaniciKitap* yeni = new KullaniciKitap{ id, isim, false, nullptr, nullptr };
    if (!head) {
        head = yeni;
        head->next = head;
        head->prev = head;
        return;
    }
    KullaniciKitap* son = head->prev;
    son->next = yeni;
    yeni->prev = son;
    yeni->next = head;
    head->prev = yeni;
}

void kullaniciKoleksiyonListele(KullaniciKitap* head) {
    if (!head) {
        cout << "Koleksiyon bos.\n";
        return;
    }
    KullaniciKitap* temp = head;
    do {
        cout << "ID: " << temp->id << ", Isim: " << temp->isim << ", Okundu: " << (temp->okundu ? "Evet" : "Hayir") << endl;
        temp = temp->next;
    } while (temp != head);
}

AgacNode* agacEkle(AgacNode* root, int id, const string& isim, int basimYili, time_t almaZamani) {
    if (!root) return new AgacNode{ id, isim, basimYili, almaZamani, nullptr, nullptr };
    if (id < root->id)
        root->left = agacEkle(root->left, id, isim, basimYili, almaZamani);
    else
        root->right = agacEkle(root->right, id, isim, basimYili, almaZamani);
    return root;
}


void agacInorder(AgacNode* root) {
    if (root) {
        agacInorder(root->left);
        cout << "ID: " << root->id << ", Isim: " << root->isim << endl;
        agacInorder(root->right);
    }
}

void talepSirasiListele(queue<pair<int, string>> q) {
    while (!q.empty()) {
        cout << "ID: " << q.front().first << ", Isim: " << q.front().second << endl;
        q.pop();
    }
}
void rapor30Gun(AgacNode* root) {
    if (!root) return;
    rapor30Gun(root->left);

    time_t simdi = time(0);
    double farkGun = difftime(simdi, root->almaZamani) / (60 * 60 * 24);

    cout << "ID: " << root->id << ", Isim: " << root->isim;
    cout << " - Odunc Alma Suresi: " << farkGun << " gun --> ";
    if (farkGun > 30)
        cout << "30 GUNDEN FAZLA\n";
    else
        cout << "30 GUNDEN AZ\n";

    rapor30Gun(root->right);
}

void rapor1950(AgacNode* root) {
    if (!root) return;
    rapor1950(root->left);

    cout << "ID: " << root->id << ", Isim: " << root->isim
        << " - Basim Yili: " << root->basimYili << " --> ";
    if (root->basimYili < 1950)
        cout << "1950'DEN ONCE\n";
    else
        cout << "1950'DEN SONRA\n";

    rapor1950(root->right);
}

void rapor1950Ve30Gun(AgacNode* root) {
    if (!root) return;
    rapor1950Ve30Gun(root->left);

    time_t simdi = time(0);
    double farkGun = difftime(simdi, root->almaZamani) / (60 * 60 * 24);

    if (root->basimYili < 1950 && farkGun > 30) {
        cout << "ID: " << root->id << ", Isim: " << root->isim
            << " - Basim Yili: " << root->basimYili
            << ", Odunc Alma Suresi: " << farkGun << " gun\n";
    }

    rapor1950Ve30Gun(root->right);
}

void personelModul1(KitapNode*& kitaplar) {
    int secim;
    do {
        cout << "\n--- [Modul 1] Kitap Yonetimi ---\n";
        cout << "1. Kitap Ekle\n";
        cout << "2. Ismine gore kitabi duzenle\n";
        cout << "3. ID bilgisine gore kitabi sil\n";
        cout << "4. Kitaplari zaman damgalari ile birlikte listele\n";
        cout << "5. ID bilgisine gore kitap ara ve aranan kitabi listele\n";
        cout << "6. Toplam kitap sayisini getir\n";
        cout << "7. Sayfa sayisina gore kitaplari kucukten buyuge sirala\n";
        cout << "8. Basim yilina gore en eski 5 kitabi listele\n";
        cout << "9. Kitaba hasar verilmesinin fark edildigi durumda kitap hasar bilgi notu ekle\n";
        cout << "10.Hasarli kitaplari yazdirma\n";
        cout << "11.Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        if (secim == 1) {
            int id, sayfa, yil;
            string isim;
            cout << "Kitap ID: "; cin >> id;
            cin.ignore();
            cout << "Kitap Isim: "; getline(cin, isim);
            cout << "Sayfa Sayisi: "; cin >> sayfa;
            cout << "Basim Yili: "; cin >> yil;
            kitapEkle(kitaplar, id, isim, sayfa, yil);
        }

        else if (secim == 2) {
            string eskiIsim, yeniIsim;
            cin.ignore();
            cout << "Duzenlemek istediginiz kitabin mevcut ismi: "; getline(cin, eskiIsim);
            cout << "Yeni isim: "; getline(cin, yeniIsim);
            kitapDuzenleIsmeGore(kitaplar, eskiIsim, yeniIsim);
        }
        else if (secim == 3) {
            int id;
            cout << "Silinecek kitap ID: "; cin >> id;
            kitapSilIDGore(kitaplar, id);
        }
        else if (secim == 4) {
            kitaplariZamanDamgaliListele(kitaplar);
        }
        else if (secim == 5) {
            int id;
            cout << "Aranacak kitap ID: "; cin >> id;
            kitapAraVeListele(kitaplar, id);
        }
        else if (secim == 6) {
            int toplam = kitapSayisiGetir(kitaplar);
            cout << "Toplam kitap sayisi: " << toplam << endl;
        }
        else if (secim == 7) {
            kitaplariSayfaSayisinaGoreSirala(kitaplar);
        }
        else if (secim == 8) {
            enEski5KitabiListele(kitaplar);
        }
        else if (secim == 9) {
            int id;
            string hasarNotu;
            cout << "Hasarli kitap ID: "; cin >> id;
            cin.ignore();
            cout << "Hasar bilgi notu: "; getline(cin, hasarNotu);
            kitapHasarEkle(kitaplar, id, hasarNotu);
        }
        else if (secim == 10) {
            hasarliKitaplariYazdir(kitaplar);
        }
        else if (secim == 11) {
            cout << "Kitap Yonetiminden cikiliyor...\n";
            break;
        }
        else {
            cout << "Gecersiz secim!\n";
        }
    } while (true);
}

void personelModul2() {
    int secim;
    do {
        cout << "\n--- [Modul 2] Odunc Kitaplar ---\n";
        cout << "1. Odunc alinan kitaplarin raporunu olustur (Stack'ten dairesel bagli listeye aktar)\n";
        cout << "2. Iade edilen kitaplari raporla\n";
        cout << "3. Tum odunc kitaplari listele\n";
        cout << "4. Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;
        if (secim == 1) {
            if (oduncKitaplar.empty()) {
                cout << "Stack'te odunc kitap yok.\n";
            }
            else {
                while (!oduncKitaplar.empty()) {
                    int id = oduncKitaplar.top().first;
                    string isim = oduncKitaplar.top().second;
                    oduncKitapListeyeEkle(oduncHead, id, isim);
                    cout << id << " - " << isim << " odunc listeye eklendi.\n";
                    oduncKitaplar.pop();
                }
            }
        }
        else if (secim == 2) {
            int id;
            cout << "Iade edilen kitap ID: "; cin >> id;
            oduncKitapIadeEt(oduncHead, id);
        }
        else if (secim == 3) {
            oduncKitapRaporuYazdir(oduncHead);
        }
        else if (secim == 4) {
            cout << "Odunc islemlerinden cikiliyor...\n";
            break;
        }
        else {
            cout << "Gecersiz secim!\n";
        }
    } while (true);
}

void personelModul3(KitapNode*& kitaplar) {
    int secim;
    do {
        cout << "\n--- [Modul 3] Talep Edilen Kitaplar ---\n";
        cout << "1. Talep edilen yeni kitabi kuyruga ekle\n";
        cout << "2. Kuyruktaki kitabi kutuphaneye (bagli listeye) aktar\n";
        cout << "3. Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        if (secim == 1) {
            int id; string isim;
            cout << "Kitap ID: "; cin >> id;
            cin.ignore();
            cout << "Kitap Isim: "; getline(cin, isim);
            talepSirasi.push({ id, isim });
            cout << "Kitap talebi kuyruga eklendi.\n";
        }
        else if (secim == 2) {
            if (talepSirasi.empty()) {
                cout << "Kuyrukta bekleyen kitap yok.\n";
            }
            else {
                int id = talepSirasi.front().first;
                string isim = talepSirasi.front().second;
                talepSirasi.pop();

                int sayfa, yil;
                cout << "Kuyruktaki kitap bilgileri: ID=" << id << ", Isim=" << isim << endl;
                cout << "Sayfa Sayisi: "; cin >> sayfa;
                cout << "Basim Yili: "; cin >> yil;

                kitapEkle(kitaplar, id, isim, sayfa, yil);
                cout << "Kitap kutuphaneye eklendi.\n";
            }
        }
        else if (secim == 3) {
            cout << "Talep islemlerinden cikiliyor...\n";
            break;
        }
        else {
            cout << "Gecersiz secim!\n";
        }
    } while (true);
}

void personelModul4(AgacNode*& agac, OduncKitapNode* oduncHead) {
    int secim;
    do {
        cout << "\n--- [Modul 4] Agac Islemleri ---\n";
        cout << "1.Odunc alinan tum kitaplari agac yapisina ekle. Ardindan odunc alma suresi 30 gunun alti ve ustu icin rapor olustur.\n";
        cout << "2.Odunc alinan tum kitaplari agac yapisina ekle. Ardindan basim yili 1950’den once ve sonra olan kitaplari listele.\n";
        cout << "3.1950’den once olan ve odunc alma suresi 30 gunu asan kitaplar icin rapor olustur.\n";
        cout << "4.Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        if (secim == 1) {
            // Aðacý boþaltmadan tekrar ekleme yapýyoruz:
            OduncKitapNode* temp = oduncHead;
            if (!temp) {
                cout << "Odunc listesi bos!\n";
            }
            else {
                do {
                    int basimYili;
                    cout << "Kitap ID: " << temp->id << ", Isim: " << temp->isim << endl;
                    cout << "Basim Yili: "; cin >> basimYili;
                    agac = agacEkle(agac, temp->id, temp->isim, basimYili, temp->almaZamani);
                    temp = temp->next;
                } while (temp != oduncHead);
                cout << "Kitaplar agac yapisina eklendi.\n";
            }
            cout << "--- 30 GUN RAPORU ---\n";
            rapor30Gun(agac);
        }
        else if (secim == 2) {
            OduncKitapNode* temp = oduncHead;
            if (!temp) {
                cout << "Odunc listesi bos!\n";
            }
            else {
                do {
                    int basimYili;
                    cout << "Kitap ID: " << temp->id << ", Isim: " << temp->isim << endl;
                    cout << "Basim Yili: "; cin >> basimYili;
                    agac = agacEkle(agac, temp->id, temp->isim, basimYili, temp->almaZamani);
                    temp = temp->next;
                } while (temp != oduncHead);
                cout << "Kitaplar agac yapisina eklendi.\n";
            }
            cout << "--- 1950 RAPORU ---\n";
            rapor1950(agac);
        }
        else if (secim == 3) {
            cout << "--- 1950 ve 30+ GUN RAPORU ---\n";
            rapor1950Ve30Gun(agac);
        }
        else if (secim == 4) {
            cout << "Agac islemlerinden cikiliyor...\n";
            break;
        }
        else {
            cout << "Gecersiz secim!\n";
        }
    } while (true);
}

void personelModul5() {
    int secim;
    do {
        cout << "\n--- [Modul 5] Puanlama (Hash Tablosu) ---\n";
        cout << "1.Odunc alinan kitaplara puan ekle ve ardindan 4 ve uzeri olan kitaplari listele\n";
        cout << "2.Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        if (secim == 1) {
            int id;
            float puan;
            cout << "Kitap ID: "; cin >> id;
            cout << "Puan (0-5): "; cin >> puan;

            if (puan < 0 || puan > 5) {
                cout << "Gecersiz puan! 0 ile 5 arasinda olmali.\n";
            }
            else {
                kitapPuanlari[id] = puan;
                cout << "Puan kaydedildi!\n";

                cout << "--- 4 ve Üzeri Puan Alan Kitaplar ---\n";
                bool bulundu = false;
                for (const auto& entry : kitapPuanlari) {
                    if (entry.second >= 4.0) {
                        cout << "Kitap ID: " << entry.first << " - Puan: " << entry.second << endl;
                        bulundu = true;
                    }
                }
                if (!bulundu) {
                    cout << "4 ve uzeri puan alan kitap yok.\n";
                }
            }
        }
        else if (secim == 2) {
            cout << "Puanlama islemlerinden cikiliyor...\n";
            break;
        }
        else {
            cout << "Gecersiz secim!\n";
        }
    } while (true);
}

void personelMenusu(KitapNode*& kitaplar, AgacNode*& agac) {
    int secim;
    do {
        cout << "\n=== Personel Menusu ===\n";
        cout << "1.Kitap Yonetimi (Modul 1)\n";
        cout << "2.Odunc Kitaplar (Modul 2)\n";
        cout << "3.Talep Edilen Kitaplar (Modul 3)\n";
        cout << "4.Agac Islemleri (Modul 4)\n";
        cout << "5.Puanlama (Modul 5)\n";
        cout << "6.Cikis\n";
        cout << "Seciminiz:";
        cin >> secim;
        if (secim == 1) personelModul1(kitaplar);
        else if (secim == 2) personelModul2();
        else if (secim == 3) personelModul3(kitaplar);
        else if (secim == 4) personelModul4(agac, oduncHead);
        else if (secim == 5) personelModul5();
        else if (secim == 6) break;
        else cout << "Gecersiz secim!\n";
    } while (secim != 6);
}

void kullaniciAnaModul1() {
    int secim;
    do {
        cout << "\n--- [Kullanici Modul 1: Yorumlar ve Koleksiyon] ---\n";
        cout << "1. ID bilgisine gore aranan kitaba iliskin yorum yapma\n";
        cout << "2. ID bilgisine gore aranan kitaba iliskin yapilan yorumlari listeleme\n";
        cout << "3. Kullanicinin kendi kitap koleksiyonunu olusturmasi (Yalniz 1 koleksiyon olusturulabilir)\n";
        cout << "4. Kullanicinin kendi kitap koleksiyonuna kitap eklemesi\n";
        cout << "5. Kullanicinin kendi kitap koleksiyonundaki kitaplari okundu/okunmadi atamasi yapmak uzere duzenlemesi\n";
        cout << "6. Kullanicinin kitap koleksiyonunda ID’sine gore sectigi kitabi silmesi\n";
        cout << "7. Kullanicinin kitap koleksiyonundaki kitaplari yazdirmasi\n";
        cout << "8. Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        switch (secim) {
        case 1: {
            int id;
            string yorum;
            cout << "Kitap ID: "; cin >> id;
            cin.ignore();
            cout << "Yorumunuz: "; getline(cin, yorum);
            yorumlar[id] = yorum;
            cout << "Yorum eklendi.\n";
            break;
        }
        case 2: {
            if (yorumlar.empty()) {
                cout << "Henuz yorum yok.\n";
            }
            else {
                for (auto& y : yorumlar) {
                    cout << "ID: " << y.first << " Yorum: " << y.second << endl;
                }
            }
            break;
        }
        case 3: {
            if (kullaniciHead != nullptr) {
                cout << "Koleksiyon zaten olusturulmus!\n";
            }
            else {
                int id; string isim;
                cout << "Ilk Kitap ID: "; cin >> id;
                cin.ignore();
                cout << "Ilk Kitap Ismi: "; getline(cin, isim);
                kullaniciHead = new KullaniciKitap{ id, isim, false, nullptr, nullptr };
                kullaniciHead->next = kullaniciHead;
                kullaniciHead->prev = kullaniciHead;
                cout << "Koleksiyon olusturuldu.\n";
            }
            break;
        }
        case 4: {
            if (!kullaniciHead) {
                cout << "Once koleksiyon olusturun!\n";
            }
            else {
                int id; string isim;
                cout << "Yeni Kitap ID: "; cin >> id;
                cin.ignore();
                cout << "Kitap Ismi: "; getline(cin, isim);
                KullaniciKitap* yeni = new KullaniciKitap{ id, isim, false, nullptr, nullptr };
                KullaniciKitap* son = kullaniciHead->prev;
                son->next = yeni;
                yeni->prev = son;
                yeni->next = kullaniciHead;
                kullaniciHead->prev = yeni;
                cout << "Kitap eklendi.\n";
            }
            break;
        }
        case 5: {
            int id;
            cout << "Okundu olarak isaretlenecek Kitap ID: "; cin >> id;
            if (kullaniciHead) {
                KullaniciKitap* temp = kullaniciHead;
                bool bulundu = false;
                do {
                    if (temp->id == id) {
                        temp->okundu = true;
                        cout << temp->isim << " okundu olarak isaretlendi.\n";
                        bulundu = true;
                        break;
                    }
                    temp = temp->next;
                } while (temp != kullaniciHead);
                if (!bulundu) {
                    cout << "Kitap bulunamadi.\n";
                }
            }
            else {
                cout << "Koleksiyon bos!\n";
            }
            break;
        }
        case 6: {
            int id;
            cout << "Silinecek Kitap ID: "; cin >> id;
            if (kullaniciHead) {
                KullaniciKitap* temp = kullaniciHead;
                bool bulundu = false;
                do {
                    if (temp->id == id) {
                        bulundu = true;
                        if (temp == kullaniciHead && temp->next == kullaniciHead) {
                            delete temp;
                            kullaniciHead = nullptr;
                        }
                        else {
                            temp->prev->next = temp->next;
                            temp->next->prev = temp->prev;
                            if (temp == kullaniciHead) kullaniciHead = temp->next;
                            delete temp;
                        }
                        cout << "Kitap silindi.\n";
                        break;
                    }
                    temp = temp->next;
                } while (temp != kullaniciHead);
                if (!bulundu) {
                    cout << "Kitap bulunamadi.\n";
                }
            }
            else {
                cout << "Koleksiyon bos!\n";
            }
            break;
        }
        case 7: {
            if (kullaniciHead) {
                KullaniciKitap* temp = kullaniciHead;
                do {
                    cout << "ID: " << temp->id << " Isim: " << temp->isim
                        << " Okundu: " << (temp->okundu ? "Evet" : "Hayir") << endl;
                    temp = temp->next;
                } while (temp != kullaniciHead);
            }
            else {
                cout << "Koleksiyon bos!\n";
            }
            break;
        }
        case 8:
            cout << "Modul 1'den cikiliyor...\n";
            break;
        default:
            cout << "Gecersiz secim!\n";
        }
    } while (secim != 8);
}

void kullaniciAnaModul2() {
    int secim;
    stack<pair<int, string>> iadeEdilenKitaplar;  // Ýade edilen kitaplarý saklamak için

    do {
        cout << "\n--- [Kullanici Modul 2: Odunc Kitap Alimi] ---\n";
        cout << "1. Odunc almak istediginiz kitabi secin (Stack'e ekle)\n";
        cout << "2. Mevcut odunc alinan kitaplari listele (Stack'ten)\n";
        cout << "3. Odunc alinan kitabi iade et\n";
        cout << "4. Iade edilen kitaplari listele\n";
        cout << "5. Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        switch (secim) {
        case 1: {
            int id;
            string isim;
            cout << "Odunc alinacak Kitap ID: "; cin >> id;
            cin.ignore();
            cout << "Kitap Ismi: "; getline(cin, isim);
            oduncKitaplar.push({ id, isim });
            cout << "Kitap odunc alindi ve listeye eklendi.\n";
            break;
        }
        case 2: {
            if (oduncKitaplar.empty()) {
                cout << "Odunc alinan kitap yok.\n";
            }
            else {
                stack<pair<int, string>> temp = oduncKitaplar;
                cout << "Mevcut odunc kitaplar:\n";
                while (!temp.empty()) {
                    cout << "ID: " << temp.top().first << ", Isim: " << temp.top().second << endl;
                    temp.pop();
                }
            }
            break;
        }
        case 3: {
            if (oduncKitaplar.empty()) {
                cout << "Odunc alinan kitap yok, iade islemi yapilamaz.\n";
            }
            else {
                auto kitap = oduncKitaplar.top();
                oduncKitaplar.pop();
                iadeEdilenKitaplar.push(kitap);
                cout << "Kitap iade edildi: ID: " << kitap.first << ", Isim: " << kitap.second << endl;
            }
            break;
        }
        case 4: {
            if (iadeEdilenKitaplar.empty()) {
                cout << "Henuz iade edilen kitap yok.\n";
            }
            else {
                stack<pair<int, string>> temp = iadeEdilenKitaplar;
                cout << "Iade edilen kitaplar:\n";
                while (!temp.empty()) {
                    cout << "ID: " << temp.top().first << ", Isim: " << temp.top().second << endl;
                    temp.pop();
                }
            }
            break;
        }
        case 5:
            cout << "Odunc Kitap Alimi menusunden cikiliyor...\n";
            break;
        default:
            cout << "Gecersiz secim!\n";
        }
    } while (secim != 5);
}

void kullaniciAnaModul3() {
    int secim;
    do {
        cout << "\n--- [Kullanici Modul 3: Kitap Talep Et] ---\n";
        cout << "1. Kutuphane de olmayan bir kitabi talep et (Talep edilen kitaplar kuyruga eklenecektir. Ilk talep edilen kitabin ilk tedarik edilecek kitap olmasi hedeflenmektedir)\n";
        cout << "2. Talepleri yazdir\n";
        cout << "3. Akademik olarak kitap talep et (Akademik talep edilen kitaplarin onceligi 1 arttirilir ve kuyrukta dogru yere konumlanir)\n";
        cout << "4. Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        switch (secim) {
        case 1: {
            int id;
            string isim;
            cout << "Talep edilen Kitap ID: "; cin >> id;
            cin.ignore();
            cout << "Kitap Ismi: "; getline(cin, isim);
            talepSirasi.push({ id, isim });
            cout << "Kitap talebi kuyruga eklendi.\n";
            break;
        }
        case 2: {
            if (talepSirasi.empty()) {
                cout << "Bekleyen kitap talebi yok.\n";
            }
            else {
                queue<pair<int, string>> temp = talepSirasi;
                cout << "Mevcut kitap talepleri:\n";
                while (!temp.empty()) {
                    cout << "ID: " << temp.front().first << ", Isim: " << temp.front().second << endl;
                    temp.pop();
                }
            }
            break;
        }
        case 3: {
            int id;
            string isim;
            cout << "Akademik olarak talep edilen Kitap ID: "; cin >> id;
            cin.ignore();
            cout << "Kitap Ismi: "; getline(cin, isim);

            // Kuyruk geçici olarak boþaltýlýr ve yeni akademik kitap baþa eklenir
            queue<pair<int, string>> yeniKuyruk;
            yeniKuyruk.push({ id, isim });  // Akademik kitap baþa

            // Eski kuyruktaki tüm kitaplarý sona ekle
            while (!talepSirasi.empty()) {
                yeniKuyruk.push(talepSirasi.front());
                talepSirasi.pop();
            }

            talepSirasi = yeniKuyruk;

            cout << "Akademik kitap oncelikli olarak kuyruga eklendi.\n";
            cout << "--- Yeni Kuyruk ---\n";
            queue<pair<int, string>> temp = talepSirasi;
            while (!temp.empty()) {
                cout << "ID: " << temp.front().first << ", Isim: " << temp.front().second << endl;
                temp.pop();
            }
            break;
        }
        case 4:
            cout << "Kitap Talep Et menusunden cikiliyor...\n";
            break;
        default:
            cout << "Gecersiz secim!\n";
        }
    } while (secim != 4);
}
void filtreleYayinevi(AgacNode* root, const string& yayinevi) {
    if (!root) return;
    filtreleYayinevi(root->left, yayinevi);
    // Örnek: Yayinevi bilgisi yoksa isimde arayalým (örnek mantýk)
    if (root->isim.find(yayinevi) != string::npos) {
        cout << "ID: " << root->id << ", Isim: " << root->isim << " (Yayinevi: " << yayinevi << ")\n";
    }
    filtreleYayinevi(root->right, yayinevi);
}

void filtreleDil(AgacNode* root, const string& dil) {
    if (!root) return;
    filtreleDil(root->left, dil);
    // Örnek: Dil bilgisi yoksa isimde arayalým (örnek mantýk)
    if (root->isim.find(dil) != string::npos) {
        cout << "ID: " << root->id << ", Isim: " << root->isim << " (Dil: " << dil << ")\n";
    }
    filtreleDil(root->right, dil);
}

void filtreleSayfaSayisi(AgacNode* root, int sayfaSiniri, bool azMi) {
    if (!root) return;
    filtreleSayfaSayisi(root->left, sayfaSiniri, azMi);
    // Þu an sayfa sayisi yok, örnek olarak basimYili kullanalým (gerçek veri eklersen deðiþtir)
    if ((azMi && root->basimYili < sayfaSiniri) || (!azMi && root->basimYili >= sayfaSiniri)) {
        cout << "ID: " << root->id << ", Isim: " << root->isim << ", Basim Yili (Simülasyon): " << root->basimYili << endl;
    }
    filtreleSayfaSayisi(root->right, sayfaSiniri, azMi);
}

void kullaniciAnaModul4(AgacNode*& agac, OduncKitapNode* oduncHead) {
    int secim;
    do {
        cout << "\n--- [Kullanici Modul 4: Odunc Kitaplar - Agac Islemleri] ---\n";
        cout << "1. Odunc alinan kitaplari agac yapisina ekleyerek yerli ve yabanci yayinevine gore filtrele. Ardindan filtreleme sonucunu listele\n";
        cout << "2. Odunc alinan kitaplari agac yapisina ekleyerek kitaplarin diline gore filtrele (Ingilizce/Turkce). Ardindan filtreleme sonucunu listele\n";
        cout << "3. Odunc alinan kitaplari agac yapisina ekleyerek sayfa uzunlugu 1000’den az ve cok olan kitaplari filtrele ve sonucu listele\n";
        cout << "4. Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        switch (secim) {
        case 1: {
            if (!oduncHead) {
                cout << "Odunc kitap listesi bos!\n";
            }
            else {
                // Aðaca ekle
                OduncKitapNode* temp = oduncHead;
                do {
                    int basimYili;
                    cout << "Kitap ID: " << temp->id << ", Isim: " << temp->isim << endl;
                    cout << "Basim Yili: "; cin >> basimYili;
                    agac = agacEkle(agac, temp->id, temp->isim, basimYili, temp->almaZamani);
                    temp = temp->next;
                } while (temp != oduncHead);
                cout << "Tum kitaplar agaca eklendi.\n";

                string yayinevi;
                cout << "Filtrelemek istedigin yayinevi (ornegin 'Yerli' veya 'Yabanci'): ";
                cin >> yayinevi;
                cout << "--- Yayinevi Filtreleme Sonucu ---\n";
                filtreleYayinevi(agac, yayinevi);
            }
            break;
        }
        case 2: {
            if (!agac) {
                cout << "Agac bos!\n";
            }
            else {
                string dil;
                cout << "Filtrelemek istedigin dil (Ingilizce/Turkce): ";
                cin >> dil;
                cout << "--- Dil Filtreleme Sonucu ---\n";
                filtreleDil(agac, dil);
            }
            break;
        }
        case 3: {
            if (!agac) {
                cout << "Agac bos!\n";
            }
            else {
                cout << "--- 1000 Sayfa Sinirina Gore Filtreleme ---\n";
                cout << "1000'den AZ olanlar:\n";
                filtreleSayfaSayisi(agac, 1000, true);
                cout << "1000'den COK olanlar:\n";
                filtreleSayfaSayisi(agac, 1000, false);
            }
            break;
        }
        case 4:
            cout << "Agac islemlerinden cikiliyor...\n";
            break;
        default:
            cout << "Gecersiz secim!\n";
        }
    } while (secim != 4);
}

void kullaniciAnaModul5() {
    int secim;
    do {
        cout << "\n--- [Kullanici Modul 5: Kitap Puanlama] ---\n";
        cout << "1. Odunc alinan kitaplari hash tablosuna ekle ve benzer kitap onerilerini listele\n";
        cout << "2. Odunc alinan kitaplari hash tablosuna ekle, kitaplari 1 ile 5 arasinda puanla ve listele\n";
        cout << "3. Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        switch (secim) {
        case 1: {
            // Öneri mantýðý için örnek kategori map'i kullanýyoruz
            map<int, string> kitapKategorileri = {
                {101, "Bilim"},
                {102, "Roman"},
                {103, "Tarih"},
                {104, "Bilim"},
                {105, "Roman"}
            };

            int id;
            cout << "Kitap ID: "; cin >> id;

            if (kitapKategorileri.find(id) != kitapKategorileri.end()) {
                string kategori = kitapKategorileri[id];
                cout << "Kitap kategorisi: " << kategori << endl;
                cout << "--- Benzer Kitap Onerileri ---\n";
                for (const auto& k : kitapKategorileri) {
                    if (k.second == kategori && k.first != id) {
                        cout << "Kitap ID: " << k.first << " - Kategori: " << k.second << endl;
                    }
                }
            }
            else {
                cout << "Bu ID ile eslesen kategori bulunamadi.\n";
            }
            break;
        }
        case 2: {
            int id;
            float puan;
            cout << "Kitap ID: "; cin >> id;
            cout << "Puan (1-5): "; cin >> puan;

            if (puan < 1 || puan > 5) {
                cout << "Gecersiz puan! 1 ile 5 arasinda olmalidir.\n";
            }
            else {
                kitapPuanlari[id] = puan;
                cout << "Puan eklendi veya guncellendi!\n";
            }

            cout << "--- Tum Kitap Puanlari ---\n";
            if (kitapPuanlari.empty()) {
                cout << "Henuz hic kitap puani yok.\n";
            }
            else {
                for (const auto& entry : kitapPuanlari) {
                    cout << "Kitap ID: " << entry.first << " - Puan: " << entry.second << endl;
                }
            }
            break;
        }
        case 3:
            cout << "Kitap Puanlama menusunden cikiliyor...\n";
            break;
        default:
            cout << "Gecersiz secim!\n";
        }
    } while (secim != 3);
}
void kullaniciModul1() {
    int id;
    string yorum;
    cout << "Kitap ID: "; cin >> id;
    cin.ignore();
    cout << "Yorumunuz: "; getline(cin, yorum);
    yorumlar[id] = yorum;
    cout << "Yorum eklendi.\n";
}

void kullaniciModul2() {
    for (auto& y : yorumlar) {
        cout << "ID: " << y.first << " Yorum: " << y.second << endl;
    }
}

void kullaniciModul3() {
    if (kullaniciHead != nullptr) {
        cout << "Koleksiyon zaten olusturulmus!\n";
    }
    else {
        int id; string isim;
        cout << "Ilk Kitap ID: "; cin >> id;
        cin.ignore();
        cout << "Ilk Kitap Ismi: "; getline(cin, isim);
        kullaniciHead = new KullaniciKitap{ id, isim, false, nullptr, nullptr };
        kullaniciHead->next = kullaniciHead;
        kullaniciHead->prev = kullaniciHead;
        cout << "Koleksiyon olusturuldu.\n";
    }
}

void kullaniciModul4() {
    if (!kullaniciHead) {
        cout << "Once koleksiyon olusturun!\n";
    }
    else {
        int id; string isim;
        cout << "Yeni Kitap ID: "; cin >> id;
        cin.ignore();
        cout << "Kitap Ismi: "; getline(cin, isim);
        KullaniciKitap* yeni = new KullaniciKitap{ id, isim, false, nullptr, nullptr };
        KullaniciKitap* son = kullaniciHead->prev;
        son->next = yeni;
        yeni->prev = son;
        yeni->next = kullaniciHead;
        kullaniciHead->prev = yeni;
        cout << "Kitap eklendi.\n";
    }
}

void kullaniciModul5() {
    int id;
    cout << "Okundu olarak isaretlenecek Kitap ID: "; cin >> id;
    if (kullaniciHead) {
        KullaniciKitap* temp = kullaniciHead;
        do {
            if (temp->id == id) {
                temp->okundu = true;
                cout << temp->isim << " okundu olarak isaretlendi.\n";
                break;
            }
            temp = temp->next;
        } while (temp != kullaniciHead);
    }
    else {
        cout << "Koleksiyon bos!\n";
    }
}

void kullaniciModul6() {
    int id;
    cout << "Silinecek Kitap ID: "; cin >> id;
    if (kullaniciHead) {
        KullaniciKitap* temp = kullaniciHead;
        do {
            if (temp->id == id) {
                if (temp == kullaniciHead && temp->next == kullaniciHead) {
                    delete temp;
                    kullaniciHead = nullptr;
                }
                else {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                    if (temp == kullaniciHead) kullaniciHead = temp->next;
                    delete temp;
                }
                cout << "Kitap silindi.\n";
                break;
            }
            temp = temp->next;
        } while (temp != kullaniciHead);
    }
    else {
        cout << "Koleksiyon bos!\n";
    }
}

void kullaniciModul7() {
    if (kullaniciHead) {
        KullaniciKitap* temp = kullaniciHead;
        do {
            cout << "ID: " << temp->id << " Isim: " << temp->isim << " Okundu: " << (temp->okundu ? "Evet" : "Hayir") << endl;
            temp = temp->next;
        } while (temp != kullaniciHead);
    }
    else {
        cout << "Koleksiyon bos!\n";
    }
}

void kullaniciMenusu(AgacNode*& agac) {
    int secim;
    do {
        cout << "\n=== Kullanici Menusu ===\n";
        cout << "1. Modul 1: Yorumlar ve Koleksiyon Islemleri\n";
        cout << "2. Modul 2: Odunc Kitap Alimi\n";
        cout << "3. Modul 3: Kitap Talep Et\n";
        cout << "4. Modul 4: Odunc Kitaplar - Agac Islemleri\n";
        cout << "5. Modul 5: Kitap Puanlama\n";
        cout << "6. Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        switch (secim) {
        case 1:
            kullaniciAnaModul1();
            break;
        case 2:
            kullaniciAnaModul2();
            break;
        case 3:
            kullaniciAnaModul3();
            break;
        case 4:
            kullaniciAnaModul4(agac, oduncHead);
            break;
        case 5:
            kullaniciAnaModul5();
            break;
        case 6:
            cout << "Kullanici menusunden cikiliyor...\n";
            break;
        default:
            cout << "Gecersiz secim!\n";
        }
    } while (secim != 6);
}
int main() {
    KitapNode* kitaplar = nullptr;
    AgacNode* agac = nullptr;
    
    kitapEkle(kitaplar, 101, "Tutunamayanlar", 198, 1988);
    kitapEkle(kitaplar, 102, "Saatleri Ayarlama Enstitusu", 225, 1961);
    kitapEkle(kitaplar, 103, "Korkuyu Beklerken", 150, 1975);
    kitapEkle(kitaplar, 104, "Yabanci", 123, 1942);
    kitapEkle(kitaplar, 105, "Simyaci", 178, 1988);
    kitapEkle(kitaplar, 106, "Hayvan Ciftligi", 112, 1945);
    kitapEkle(kitaplar, 107, "1984", 328, 1949);
    kitapEkle(kitaplar, 108, "Savas ve Baris", 1225, 1869);
    kitapEkle(kitaplar, 109, "Anna Karenina", 864, 1877);
    kitapEkle(kitaplar, 110, "Suc ve Ceza", 671, 1866);
    kitapEkle(kitaplar, 111, "Beyaz Dis", 232, 1906);
    kitapEkle(kitaplar, 112, "Don Kisot", 863, 1605);
    kitapEkle(kitaplar, 113, "Kucuk Prens", 96, 1943);
    kitapEkle(kitaplar, 114, "Fahrenheit 451", 158, 1953);
    kitapEkle(kitaplar, 115, "Cesur Yeni Dunya", 311, 1932);
    kitapEkle(kitaplar, 116, "Utopia", 195, 1516);
    kitapEkle(kitaplar, 117, "Insan Neyle Yasar", 134, 1885);
    kitapEkle(kitaplar, 118, "Yeraltindan Notlar", 131, 1864);
    kitapEkle(kitaplar, 119, "Karamazov Kardesler", 824, 1880);
    kitapEkle(kitaplar, 120, "Karanlikta Kahkaha", 203, 1985);

    int secim;
    do {
        cout << "\n====== Kutuphane Yonetim Sistemi ======\n";
        cout << "1.Personel Girisi\n";
        cout << "2.Kullanici Girisi\n";
        cout << "3.Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        if (secim == 1) {
            personelMenusu(kitaplar, agac);
        }
        else if (secim == 2) {
            kullaniciMenusu(agac);
        }
        else if (secim == 3) {
            cout << "Cikis yapiliyor...\n";
            break;
        }
        else {
            cout << "Gecersiz secim!\n";
        }
    } while (secim != 3);

    return 0;
}