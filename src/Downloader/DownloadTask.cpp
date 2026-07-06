#include "DownloadTask.hpp"
#include <sys/stat.h>
#include <iostream>

namespace Downloader {

    DownloadTask::DownloadTask(const std::string& url, const std::string& destination)
        : m_url(url), m_destination(destination), m_title("Unknown"), m_curl(nullptr), m_file(nullptr),
          m_state(DownloadState::IDLE), m_downloadedBytes(0), m_totalBytes(0), 
          m_speedBytesPerSec(0), m_timeRemainingSecs(0), m_resumeOffset(0) {
    }

    DownloadTask::~DownloadTask() {
        if (m_curl) curl_easy_cleanup(m_curl);
        if (m_file) fclose(m_file);
    }

    size_t DownloadTask::WriteCallback(void *ptr, size_t size, size_t nmemb, void *userdata) {
        DownloadTask* task = static_cast<DownloadTask*>(userdata);
        if (task->m_state == DownloadState::PAUSED || task->m_state == DownloadState::CANCELLED) {
            return CURL_WRITEFUNC_PAUSE; // Pause or stop
        }
        size_t written = fwrite(ptr, size, nmemb, task->m_file);
        return written;
    }

    int DownloadTask::ProgressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
        DownloadTask* task = static_cast<DownloadTask*>(clientp);
        
        if(dltotal > 0) {
            task->m_totalBytes = static_cast<double>(dltotal) + task->m_resumeOffset;
            task->m_downloadedBytes = static_cast<double>(dlnow) + task->m_resumeOffset;
            
            curl_easy_getinfo(task->m_curl, CURLINFO_SPEED_DOWNLOAD_T, &task->m_speedBytesPerSec);
            
            if(task->m_speedBytesPerSec > 0) {
                task->m_timeRemainingSecs = (task->m_totalBytes - task->m_downloadedBytes) / task->m_speedBytesPerSec;
            }
        }
        
        if (task->m_state == DownloadState::CANCELLED) {
            return 1; // Abort transfer
        }
        
        return 0;
    }

    void DownloadTask::Start() {
        if (m_state == DownloadState::DOWNLOADING) return;

        m_curl = curl_easy_init();
        if(!m_curl) return;

        // Verificar si el archivo ya existe para reanudar
        struct stat file_info;
        if(stat(m_destination.c_str(), &file_info) == 0) {
            m_resumeOffset = file_info.st_size;
            m_file = fopen(m_destination.c_str(), "ab");
        } else {
            m_file = fopen(m_destination.c_str(), "wb");
            m_resumeOffset = 0;
        }

        if(!m_file) {
            m_state = DownloadState::ERROR;
            return;
        }

        curl_easy_setopt(m_curl, CURLOPT_URL, m_url.c_str());
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, ProgressCallback);
        curl_easy_setopt(m_curl, CURLOPT_XFERINFODATA, this);
        curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);

        if (m_resumeOffset > 0) {
            curl_easy_setopt(m_curl, CURLOPT_RESUME_FROM_LARGE, (curl_off_t)m_resumeOffset);
        }

        m_state = DownloadState::DOWNLOADING;
        // In a real PS3 app, this would be added to curl_multi in DownloadManager
        // For now, this is a structure ready for multi interface.
    }

    void DownloadTask::Pause() {
        if (m_state == DownloadState::DOWNLOADING) {
            m_state = DownloadState::PAUSED;
            if (m_file) {
                fflush(m_file);
                fclose(m_file);
                m_file = nullptr;
            }
        }
    }

    void DownloadTask::Resume() {
        if (m_state == DownloadState::PAUSED || m_state == DownloadState::ERROR) {
            Start(); // Reiniciar con el offset actual
        }
    }

    void DownloadTask::Cancel() {
        m_state = DownloadState::CANCELLED;
        if (m_file) {
            fclose(m_file);
            m_file = nullptr;
        }
        remove(m_destination.c_str());
    }

    float DownloadTask::GetProgress() const {
        if (m_totalBytes == 0) return 0.0f;
        return static_cast<float>(m_downloadedBytes / m_totalBytes);
    }
    
    double DownloadTask::GetSpeed() const {
        return m_speedBytesPerSec;
    }
    
    double DownloadTask::GetTimeRemaining() const {
        return m_timeRemainingSecs;
    }

    void DownloadTask::Update() {
        // Here we could step the multi handle if we managed curl internally per task
    }

} // namespace Downloader
