#pragma once

#include "UI/View.hpp"
#include "Resources/TextureManager.hpp"
#include "Downloader/DownloadTask.hpp"
#include <memory>

namespace UI {

    class GameInfoView : public View {
    public:
        void OnEnter() override;
        void Update(float deltaTime) override;
        void Render(Graphics::Renderer& renderer) override;

    private:
        std::shared_ptr<Resources::Texture> m_background;
        std::shared_ptr<Downloader::DownloadTask> m_activeDownload;
        
        bool m_isDownloading = false;
    };

} // namespace UI
