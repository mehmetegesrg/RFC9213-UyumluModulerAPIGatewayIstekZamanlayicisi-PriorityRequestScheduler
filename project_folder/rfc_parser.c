#include "rfc_parser.h"
#include <string.h>

HttpPriority parse_rfc9213(int id, const char* header_str) {
    HttpPriority req;
    req.request_id = id;
    req.urgency = 3;             // RFC 9213'e göre varsayılan urgency değeri 3'tür
    req.incremental = false;     // Varsayılan incremental değeri false'tur
    
    // Gelen metni güvenli bir şekilde struct içine kopyala
    strncpy(req.raw_header, header_str, sizeof(req.raw_header) - 1);
    req.raw_header[sizeof(req.raw_header) - 1] = '\0';

    // "u=" parametresini bul
    const char* u_ptr = strstr(header_str, "u=");
    if (u_ptr != NULL) {
        // Karakteri sayıya çevir
        int u_val = u_ptr[2] - '0';
        if (u_val >= 0 && u_val <= 7) {
            req.urgency = u_val;
        }
    }

    // "i=" parametresini bul
    const char* i_ptr = strstr(header_str, "i=");
    if (i_ptr != NULL) {
        // RFC standartlarına göre i=?1 veya i=?0 olarak gelir
        if (i_ptr[2] == '?' && i_ptr[3] == '1') {
            req.incremental = true;
        } else if (i_ptr[2] == '?' && i_ptr[3] == '0') {
            req.incremental = false;
        }
    }

    return req;
}
