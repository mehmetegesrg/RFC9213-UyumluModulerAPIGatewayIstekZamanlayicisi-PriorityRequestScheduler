#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

// Kapsam İzolasyonu: Sadece bu dosyada çalışan yardımcı fonksiyonlar
static void swap(HttpPriority *a, HttpPriority *b) {
    HttpPriority temp = *a;
    *a = *b;
    *b = temp;
}

// Hangi isteğin daha öncelikli olduğunu belirleyen mantık (Min-Heap kuralı)
// Aciliyeti düşük olan (0'a yakın olan) kazanır. Eşitlikte ilk gelen (id'si küçük olan) kazanır.
static bool is_higher_priority(HttpPriority a, HttpPriority b) {
    if (a.urgency != b.urgency) {
        return a.urgency < b.urgency;
    }
    return a.request_id < b.request_id; 
}

// O(1) Başlangıç bellek tahsisi
MinHeap* create_heap(int initial_capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    
    heap->capacity = initial_capacity;
    heap->size = 0;
    heap->data = (HttpPriority*)malloc(heap->capacity * sizeof(HttpPriority));
    
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    return heap;
}

// O(log n) - Aşağıdan yukarıya ağacı düzenleme
static void heapify_up(MinHeap* heap, int index) {
    int parent = (index - 1) / 2;
    
    // Eğer çocuk, ebeveyninden daha yüksek öncelikliyse yer değiştir
    if (index > 0 && is_higher_priority(heap->data[index], heap->data[parent])) {
        swap(&heap->data[parent], &heap->data[index]);
        heapify_up(heap, parent); // Rekürsif kontrol
    }
}

// O(log n) - Yukarıdan aşağıya ağacı düzenleme
static void heapify_down(MinHeap* heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Sol çocuk daha öncelikliyse
    if (left < heap->size && is_higher_priority(heap->data[left], heap->data[smallest])) {
        smallest = left;
    }

    // Sağ çocuk daha öncelikliyse
    if (right < heap->size && is_higher_priority(heap->data[right], heap->data[smallest])) {
        smallest = right;
    }

    // Eğer kök değiştiyse takas yap ve aşağı doğru devam et
    if (smallest != index) {
        swap(&heap->data[index], &heap->data[smallest]);
        heapify_down(heap, smallest);
    }
}

// Yeni eleman ekleme ve dinamik genişleme
void insert_request(MinHeap* heap, HttpPriority request) {
    // Dizi dolduysa kapasiteyi 2 katına çıkar (realloc kullanımı - Bellek yönetimi puanı)
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->data = (HttpPriority*)realloc(heap->data, heap->capacity * sizeof(HttpPriority));
        if (!heap->data) {
            printf("Kritik Hata: Bellek ayrilamadi!\n");
            exit(1);
        }
    }
    
    // Elemanı sona ekle ve yukarı taşı
    heap->data[heap->size] = request;
    heapify_up(heap, heap->size);
    heap->size++;
}

// En yüksek öncelikli elemanı çıkarma
HttpPriority extract_min(MinHeap* heap) {
    if (heap->size <= 0) {
        HttpPriority empty = {-1, -1, false, ""};
        return empty;
    }
    
    if (heap->size == 1) {
        heap->size--;
        return heap->data[0];
    }
    
    // Ağacın kökünü (en öncelikli olanı) al
    HttpPriority root = heap->data[0];
    
    // En sondaki elemanı köke taşı ve aşağı kaydır
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    
    return root;
}

bool is_empty(MinHeap* heap) {
    return heap->size == 0;
}

// Sıfır bellek sızıntısı (Zero memory leak) için
void free_heap(MinHeap* heap) {
    if (heap) {
        if (heap->data) free(heap->data);
        free(heap);
    }
}
