#pragma once

#include <string>
#include <atomic>
#include <curl/curl.h>

namespace Downloader {

    enum class DownloadState {
        IDLE,
        DOWNLOADING,
        PAUSED,
        COMPLETED,
        ERROR,
        CANCELLED
    };

    class DownloadTask {
    public:
        DownloadTask(const std::string& url, const std::string& destination);
        ~DownloadTask();

        void Start();
        void Pause();
        void Resume();
        void Cancel();

        void Update(); // Called periodically

        // Getters
        std::string GetTitle() const { return m_title; }
        void SetTitle(const std::string& title) { m_title = title; }
        
        float GetProgress() const;
        double GetSpeed() const;
        double GetTimeRemaining() const;
        DownloadState GetState() const { return m_state; }

        static size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *userdata);
        static int ProgressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

    private:
        std::string m_url;
        std::string m_destination;
        std::string m_title;

        CURL* m_curl;
        FILE* m_file;
        
        DownloadState m_state;
        
        double m_downloadedBytes;
        double m_totalBytes;
        double m_speedBytesPerSec;
        double m_timeRemainingSecs;
        
        long m_resumeOffset;
        
        void PerformCurlAsync(); // Simulated async/non-blocking or handled via multi handle in manager
    };

} // namespace Downloader
