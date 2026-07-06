#pragma once

#include <string>

namespace Network {

    class HttpClient {
    public:
        // Inicializa curl y libnet si es necesario
        static void Initialize();
        static void Terminate();

        // Realiza una petición GET bloqueante y retorna el contenido
        // Utilizado para descargas pequeñas como el TSV
        static std::string Get(const std::string& url);
    };

} // namespace Network
