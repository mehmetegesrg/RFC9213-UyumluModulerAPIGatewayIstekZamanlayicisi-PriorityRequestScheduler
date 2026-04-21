#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "rfc_parser.h"

// Min-Heap yapısının kalbi
typedef struct {
    HttpPriority* data; // Dinamik dizi pointer'ı
    int capacity;       // Toplam kapasite
    int size;           // Mevcut eleman sayısı
} MinHeap;

// Bellek Yönetimi ve Temel Fonksiyon Prototipleri
MinHeap* create_heap(int initial_capacity);
void free_heap(MinHeap* heap);

// O(log n) karmaşıklığına sahip Kuyruk İşlemleri
void insert_request(MinHeap* heap, HttpPriority request);
HttpPriority extract_min(MinHeap* heap);
bool is_empty(MinHeap* heap);

#endif // PRIORITY_QUEUE_H
