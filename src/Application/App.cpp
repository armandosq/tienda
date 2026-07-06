#include "App.hpp"
#include "Graphics/Renderer.hpp"
#include "Input/PadManager.hpp"
#include "Network/HttpClient.hpp"
#include "Downloader/DownloadManager.hpp"
#include "UI/SceneManager.hpp"
#include "UI/SplashView.hpp"
#include "UI/HomeView.hpp"
#include "UI/GameListView.hpp"
#include "UI/GameInfoView.hpp"
#include <sys/process.h>
#include <sysmodule/sysmodule.h>

namespace Application {

    App::App() : m_running(false) {}

    App::~App() {
        Terminate();
    }

    void App::Initialize() {
        // Inicializar sysmodules necesarios
        sysModuleLoad(SYSMODULE_NET);
        sysModuleLoad(SYSMODULE_PNG_DEC);
        sysModuleLoad(SYSMODULE_FREETYPE);

        Graphics::Renderer::GetInstance().Initialize();
        Input::PadManager::GetInstance().Initialize();
        Network::HttpClient::Initialize();
        Downloader::DownloadManager::GetInstance().Initialize();

        // Registrar vistas
        auto& sceneManager = UI::SceneManager::GetInstance();
        sceneManager.RegisterView("Splash", std::make_shared<UI::SplashView>());
        sceneManager.RegisterView("Home", std::make_shared<UI::HomeView>());
        sceneManager.RegisterView("GameList", std::make_shared<UI::GameListView>());
        sceneManager.RegisterView("GameInfo", std::make_shared<UI::GameInfoView>());
        
        sceneManager.SwitchTo("Splash");
    }

    void App::Terminate() {
        Downloader::DownloadManager::GetInstance().Terminate();
        Network::HttpClient::Terminate();
        
        sysModuleUnload(SYSMODULE_FREETYPE);
        sysModuleUnload(SYSMODULE_PNG_DEC);
        sysModuleUnload(SYSMODULE_NET);
    }

    void App::Run() {
        Initialize();
        m_running = true;

        // Bucle principal a 60 FPS aprox.
        while (m_running) {
            // Manejar eventos del sistema (ej. botón PS)
            sysProcessExitSpawn2();

            // Calcular deltaTime real aquí (hardcoded a 1/60 por simplicidad)
            float deltaTime = 1.0f / 60.0f;

            // Update Managers
            Input::PadManager::GetInstance().Update();
            Downloader::DownloadManager::GetInstance().Update();
            UI::SceneManager::GetInstance().Update(deltaTime);

            // Exit cond
            if (Input::PadManager::GetInstance().IsPressed(Input::Button::SELECT) && 
                Input::PadManager::GetInstance().IsPressed(Input::Button::START)) {
                Quit();
            }

            // Render
            Graphics::Renderer::GetInstance().Clear(Graphics::Color(0,0,0));
            UI::SceneManager::GetInstance().Render(Graphics::Renderer::GetInstance());
            Graphics::Renderer::GetInstance().Flip();
        }
    }

    void App::Quit() {
        m_running = false;
    }

} // namespace Application
