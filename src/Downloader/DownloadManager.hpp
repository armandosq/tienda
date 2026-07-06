#pragma once

#include <vector>
#include <memory>
#include "Downloader/DownloadTask.hpp"

namespace Downloader {

    class DownloadManager {
    public:
        static DownloadManager& GetInstance() {
            static DownloadManager instance;
            return instance;
        }

        DownloadManager(DownloadManager const&) = delete;
        void operator=(DownloadManager const&) = delete;

        void Initialize();
        void Update(); // Call in the main GameLoop
        void Terminate();

        std::shared_ptr<DownloadTask> AddDownload(const std::string& url, const std::string& destination, const std::string& title);
        
        const std::vector<std::shared_ptr<DownloadTask>>& GetTasks() const { return m_tasks; }

    private:
        DownloadManager() : m_multi_handle(nullptr) {}
        
        CURLM* m_multi_handle;
        std::vector<std::shared_ptr<DownloadTask>> m_tasks;
    };

} // namespace Downloader
