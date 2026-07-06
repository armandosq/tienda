#include "DownloadManager.hpp"
#include <iostream>

namespace Downloader {

    void DownloadManager::Initialize() {
        m_multi_handle = curl_multi_init();
    }

    void DownloadManager::Terminate() {
        if (m_multi_handle) {
            curl_multi_cleanup(m_multi_handle);
            m_multi_handle = nullptr;
        }
    }

    std::shared_ptr<DownloadTask> DownloadManager::AddDownload(const std::string& url, const std::string& destination, const std::string& title) {
        auto task = std::make_shared<DownloadTask>(url, destination);
        task->SetTitle(title);
        
        // En un entorno de PS3 multi-hilo o manejado asíncronamente con libcurl multi:
        // Aquí deberíamos atar el curl_easy handle de DownloadTask al curl_multi_handle
        // Por la limitación de encapsulamiento actual, usaremos una versión simplificada
        // de curl_easy para DownloadTask, y DownloadManager lo orquesta.
        
        m_tasks.push_back(task);
        task->Start();
        
        return task;
    }

    void DownloadManager::Update() {
        if (!m_multi_handle) return;
        
        // Aquí se realizaría el tick asíncrono de libcurl multi (curl_multi_perform)
        // para descargar en background sin bloquear los 60 FPS del juego.
        
        // int still_running = 0;
        // curl_multi_perform(m_multi_handle, &still_running);
        
        // Para limpiar las tareas canceladas o finalizadas
        for (auto it = m_tasks.begin(); it != m_tasks.end(); ) {
            if ((*it)->GetState() == DownloadState::CANCELLED || (*it)->GetState() == DownloadState::COMPLETED) {
                // Remove from multi_handle and erase
                it = m_tasks.erase(it);
            } else {
                ++it;
            }
        }
    }

} // namespace Downloader
