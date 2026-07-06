#pragma once

namespace Application {

    class App {
    public:
        App();
        ~App();

        void Run();
        void Quit();

    private:
        void Initialize();
        void Terminate();

        bool m_running;
    };

} // namespace Application
