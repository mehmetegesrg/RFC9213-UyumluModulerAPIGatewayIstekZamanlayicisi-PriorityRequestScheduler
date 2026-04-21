#ifndef RFC_PARSER_H
#define RFC_PARSER_H

#include <stdbool.h>

// Gelen HTTP isteğini temsil eden veri yapısı
typedef struct {
    int request_id;         // İsteğin geliş sırası (Aynı öncelikte FIFO için)
    int urgency;            // Aciliyet (0 en yüksek, 7 en düşük öncelik)
    bool incremental;       // Parçalı işleme durumu
    char raw_header[128];   // Gelen ham string (Örn: "u=3, i=?1")
} HttpPriority;

// String'i alıp struct'a çeviren fonksiyon
HttpPriority parse_rfc9213(int id, const char* header_str);

#endif // RFC_PARSER_H
