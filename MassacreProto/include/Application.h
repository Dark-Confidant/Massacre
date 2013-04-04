#pragma once

namespace mcr {

class Application
{
public:
    virtual void initApplication(int argc, const char** argv);
    virtual void exitApplication();

    virtual void animate();
    virtual void render();
    virtual void handleInput();
};

} // ns mcr
