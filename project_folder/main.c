#include <stdio.h>
#include "rfc_parser.h"
#include "priority_queue.h"

int main() {
    printf("--- RFC 9213 API Gateway Oncelikli Istek Zamanlayicisi ---\n\n");

    // Başlangıç kapasitesi 2 olan bir Min-Heap oluştur. (Daha sonra realloc ile büyüyecek)
    MinHeap* scheduler = create_heap(2);

    // Gelen isteklerin simülasyonu
    const char* incoming_requests[] = {
        "u=5, i=?0", // ID: 0 (Düşük öncelik)
        "u=1, i=?1", // ID: 1 (Çok Yüksek öncelik)
        "u=3",       // ID: 2 (Orta öncelik, i varsayılan)
        "u=1, i=?0", // ID: 3 (Çok Yüksek öncelik - ID'den dolayı 1'den sonra işlenmeli)
        "u=7, i=?1", // ID: 4 (En düşük öncelik)
        "u=0, i=?1"  // ID: 5 (Acil Durum - En tepeye yerleşmeli)
    };

    int request_count = sizeof(incoming_requests) / sizeof(incoming_requests[0]);

    printf("[SISTEM] %d adet HTTP istegi Gateway'e ulasti, parse ediliyor...\n", request_count);

    // 1. Aşama: Gelen stringleri Parse et ve kuyruğa at
    for (int i = 0; i < request_count; i++) {
        HttpPriority parsed_req = parse_rfc9213(i, incoming_requests[i]);
        insert_request(scheduler, parsed_req);
        printf(" -> EKLENDI: ID: %d | Urgency: %d | Inc: %d | Ham Veri: %s\n", 
               parsed_req.request_id, parsed_req.urgency, parsed_req.incremental, parsed_req.raw_header);
    }

    printf("\n[SISTEM] Kuyruk isleniyor (Min-Heap Siralamasi ile)...\n");
    printf("------------------------------------------------------------\n");
    printf("ISLEM SIRASI | ID | URGENCY | INC | HAM VERI\n");
    printf("------------------------------------------------------------\n");

    // 2. Aşama: İstekleri kuyruktan öncelik sırasına göre çek (O(log n) çıkarma)
    int islem_sirasi = 1;
    while (!is_empty(scheduler)) {
        HttpPriority req = extract_min(scheduler);
        printf("Isleme Alindi %d | %02d |    %d    |  %d  | %s\n", 
               islem_sirasi, req.request_id, req.urgency, req.incremental, req.raw_header);
        islem_sirasi++;
    }

    // 3. Aşama: Belleği temizle (Memory Leak önlemi)
    free_heap(scheduler);
    printf("------------------------------------------------------------\n");
    printf("[SISTEM] Tüm istekler basariyla islendi. Bellek temizlendi.\n");

    return 0;
}
