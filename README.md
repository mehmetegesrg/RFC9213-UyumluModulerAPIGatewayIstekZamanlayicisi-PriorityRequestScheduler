# RFC 9213 Uyumlu Modüler API Gateway İstek Zamanlayıcısı

Bu proje, yüksek trafikli ağ geçitlerinde (API Gateway) gelen HTTP isteklerini **RFC 9213 (Extensible HTTP Priorities)** standartlarına göre önceliklendiren ve **Min-Heap** veri yapısı kullanarak optimize eden bir **Priority Request Scheduler** (Öncelikli İstek Zamanlayıcısı) simülasyonudur.

## 🚀 Proje Amacı
Yoğun trafik alan sistemlerde, tüm isteklerin geliş sırasına göre (FIFO) işlenmesi, kritik sistem mesajlarının veya yüksek öncelikli kullanıcı taleplerinin tıkanmasına neden olur. Bu proje, bu darboğazı aşmak için:
- Gelen istekleri standartlara göre ayrıştırır.
- Dinamik bir öncelikli kuyrukta (Priority Queue) saklar.
- En verimli algoritmik karmaşıklıkla (O(log n)) istekleri sıraya dizer.

## ✨ Temel Özellikler
- **RFC 9213 Uyumluluğu:** `u` (Urgency) ve `i` (Incremental) parametrelerini içeren HTTP öncelik header'larını parse eder.
- **Min-Heap Algoritması:** İstek ekleme ve en öncelikli isteği çekme işlemlerini $O(\\log n)$ sürede gerçekleştirir.
- **Modüler Mimari:** C dilinin sunduğu header (`.h`) ve source (`.c`) ayrımıyla, sorumlulukların ayrılması (Separation of Concerns) prensibine uygun inşa edilmiştir.
- **Dinamik Bellek Yönetimi:** `malloc` ve `realloc` kullanılarak trafik patlamalarına (spike) yanıt verebilen, `free` ile bellek sızıntısını (memory leak) sıfıra indiren bir yapı sunar.
- **Tie-Breaking:** Aynı aciliyet (urgency) değerine sahip isteklerde, sisteme giriş sırasını (request_id) baz alan lokal FIFO mantığı işletilir.

## 🏗️ Sistem Mimarisi
Proje üç ana katmandan oluşmaktadır:

1.  **Ayrıştırma Katmanı (`rfc_parser`):** Ham string verisini `HttpPriority` struct yapısına dönüştürür.
2.  **Veri Yapısı Katmanı (`priority_queue`):** Min-Heap algoritmasının kalbidir. `heapify_up` ve `heapify_down` mekanizmalarını içerir.
3.  **Yönetim Katmanı (`main`):** Gateway simülasyonunu başlatır, test senaryolarını çalıştırır ve bellek temizliğini yönetir.

## 📊 Algoritmik Analiz: Neden Min-Heap?
Projede sıradan bir dinamik dizi (Array) yerine Min-Heap tercih edilmesinin teknik nedeni performans ölçeklenebilirliğidir:

| İşlem | Sıradan Dizi (Array) | Min-Heap |
| :--- | :--- | :--- |
| **İstek Ekleme (Insert)** | $O(n)$ | $O(\\log n)$ |
| **En Acil İsteği Çekme** | $O(n)$ | $O(\\log n)$ |
| **Hafıza Kullanımı** | Sabit / Lineer | Dinamik / Logaritmik Yönetim |

*Neden?* Dizilerde araya eleman eklemek veya baştan eleman silmek, diğer tüm elemanların kaydırılmasını gerektirir. Min-Heap ise sadece ağaç derinliği boyunca işlem yaparak milyonlarca istekte bile stabil kalır.

## 🛠️ Kurulum ve Derleme
Projeyi yerel makinenizde çalıştırmak için bir C derleyicisine (GCC, Clang vb.) veya CLion gibi bir IDE'ye ihtiyacınız vardır.

### Terminal üzerinden derleme:
```bash
gcc main.c rfc_parser.c priority_queue.c -o api_gateway
./api_gateway
```

CLion / CMake ile derleme:
CMakeLists.txt dosyanıza aşağıdaki satırı eklediğinizden emin olun:
```bash
CMake
add_executable(api_gateway main.c rfc_parser.c rfc_parser.h priority_queue.c priority_queue.h)
```

📂 Dosya Yapısı
```bash
rfc_parser.h / .c: RFC 9213 parsing mantığı.

priority_queue.h / .c: Min-Heap veri yapısı ve algoritma implementasyonu.

main.c: Sistem simülasyonu ve test senaryoları.
```

📜 Geliştirici: Mehmet Ege Sarıgöl

