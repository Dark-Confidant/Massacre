#include "Universe.h"

#include "Timer.h"

#include "Camera.h"
#include "gfx/Context.h"
#include "gfx/renderables/Mesh.h"
#include "gfx/Renderer.h"
#include "gfx/MaterialManager.h"

#include "Object.h"
#include "components/Render.h"
#include "components/Model.h"
#include "components/Movement.h"
#include <Config.h>

using namespace mcr;

class Game;

// GameLayer, stub
class GameLayer
{
public:
    GameLayer(Game* game): m_game(game) {}
    virtual ~GameLayer() {}

    virtual void onLoad() = 0;
    virtual void onUnload() = 0;

    virtual void onActivate() = 0;
    virtual void onDeactivate() = 0;

    virtual bool onRun() = 0;

protected:
    Game* m_game;
};

// Game
class Game
{
public:
    Config    config;
    
    Game();
    ~Game();

    void initGL();
    void run();

protected:
    GameLayer* m_layer;
};

//////////////////////////////////////////////////////////////////////////
// Game layer stuff

class BattleScreen: public GameLayer
{
public:
    BattleScreen(Game* game):
    GameLayer(game),
        m_camera(),
        m_renderer(&m_camera)
    {
        memset(m_keyStates, 0, sizeof(m_keyStates));
        m_root = Object::create();
    }

    void onLoad()
    {
        m_camera.setZRange(vec2(1.f, 3000.f));
        m_camera.update();

#if defined(MCR_PLATFORM_WINDOWS)

        if (!m_mm.fs()->attachResource("DataArena/"))
        {
            debug("Can't find data directory");
            exit(1);
        };

#elif defined(MCR_PLATFORM_LINUX)

        // We should also check system wide data repository for linux
        if (!m_mm.fs()->attachResource("DataArena/")
        &&  !m_mm.fs()->attachResource("/usr/share/massacre/"))
        {
            debug("Can't find data directory");
            exit(1);
        };

#endif
        m_game->config.load("mainconf.yaml", m_mm.fs());

        loadArena();
        loadSky();
    }

    void loadArena()
    {
        auto diffuse = m_mm.getMaterial("Models/Arena/arena.mtl");
        auto trans   = m_mm.getMaterial("Models/Arena/branch.mtl");
        auto leaves  = m_mm.getMaterial("Models/Arena/leaves.mtl");

        auto diffuseAtlas = m_mm.parseAtlasTMP("Models/Arena/arena.json");
        auto transAtlas   = m_mm.parseAtlasTMP("Models/Arena/trans.json");

        auto mesh = gfx::Mesh::createFromFile(m_mm.fs()->openFile("Models/Arena/arena.mesh"));
        auto va   = const_cast<gfx::VertexArray*>(mesh->buffer()); // hack

        // flip by v
        va->transformAttribs(2, math::buildTransform(vec3(0, 1, 0), vec3(), vec3(1, -1, 0)));

        for (uint i = 0; i < mesh->numAtoms(); ++i)
        {
            auto        atom = const_cast<gfx::RenderAtom*>(mesh->atom(i));
            std::string tex  = Path::filename(atom->material->texture(0)->filename().c_str());

            bool transformBit = false;

            auto it = diffuseAtlas.find(tex);
            if (it != diffuseAtlas.end())
            {
                transformBit = true;
                atom->material = diffuse;
            }
            else
            {
                it = transAtlas.find(tex);
                if (it != transAtlas.end())
                {
                    transformBit = true;

                    if (tex.substr(0, 6) == "leaves")
                        atom->material = leaves;
                    else
                        atom->material = trans;
                }
            }

            if (transformBit)
            {
                auto tf = math::buildTransform(
                    vec3(it->second.botLeft(), 0.f),
                    vec3(),
                    vec3(it->second.size(), 0.f));

                va->transformAttribsIndexed(2, tf, atom->start, atom->size);
                continue;
            }

            atom->material->initProgram(
                diffuse->program()->shader(0),
                diffuse->program()->shader(1));
        }

        mesh->optimizeAtomsTMP();

        m_arena = Object::create(m_root);

        m_arena->addComponent<Render>()->setRenderer(&m_renderer);
        m_arena->addComponent<Model>()->setMesh(mesh);
    }

    void loadSky()
    {
        auto vert = m_mm.getShader("Shaders/sky_t0.vert");
        auto frag = m_mm.getShader("Shaders/diffuse.frag");

        auto mesh = gfx::Mesh::createFromFile(m_mm.fs()->openFile("Models/Sky/sky.mesh"));
        auto va   = const_cast<gfx::VertexArray*>(mesh->buffer()); // hack

        // flip by v
        va->transformAttribs(2, math::buildTransform(vec3(0, 1, 0), vec3(), vec3(1, -1, 0)));

        for (uint i = 0; i < mesh->numAtoms(); ++i)
        {
            gfx::Material* mtl = mesh->atom(i)->material;
            
            mtl->initProgram(vert, frag);

            mtl->set(&gfx::RenderState::depthTest, false);
            mtl->setPassHint(-1);
        }

        m_sky = Object::create(m_root);

        m_sky->addComponent<Render>()->setRenderer(&m_renderer);
        m_sky->addComponent<Model>()->setMesh(mesh);
    }

    void onUnload() {}


    void onActivate()
    {
        m_myself = Object::create(m_root);

        m_myself->addComponent<Movement>()->setVelocity(100.f);
        m_myself->transform()->setY(20);
    }

    void onDeactivate() {}


    bool onRun()
    {
        m_timer.refresh();

        handleKeys();
        m_root->update(m_timer.dmilliseconds()); // update scene
        updateCamera();
        render();

        return handleEvents();
    }

    void render()
    {
        gfx::Context::active().setRenderState(gfx::RenderState());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_camera.dumpMatrices();
        m_renderer.render();

        SDL_GL_SwapBuffers();
    }

    void handleKeys()
    {
        //static const auto turnSpeed = 70.f;
        
        double turnSpeed = m_game->config["turn_speed"];
        
        debug("turnSpeed", turnSpeed);

        byte move =  m_keyStates[SDLK_w]
                  + (m_keyStates[SDLK_s] << 1)
                  + (m_keyStates[SDLK_q] << 2)
                  + (m_keyStates[SDLK_e] << 3);

        auto turnStep = (float) m_timer.dseconds() * -turnSpeed;

        vec2 face = m_myself->movement()->face() 
                  + vec2(0, turnStep * (m_keyStates[SDLK_d] - m_keyStates[SDLK_a]));

        m_myself->movement()->setFlags(move);
        m_myself->movement()->setFace(face);

        m_myself->transform()->setRotation(vec3(face, 0));
    }

    void updateCamera()
    {
        static const vec3 camOffset(0, 35, 0);

        auto mat = m_myself->transform()->global();

        mat.scaleDown(m_myself->transform()->scale());

        m_camera.setViewMatrix(mat, true);
        m_camera.setPosition(mat.translation() + camOffset * mat);
    }

    // [Esc] and window resize
    bool handleEvents()
    {
        for (SDL_Event e; SDL_PollEvent(&e);)
        {
            switch (e.type)
            {
            case SDL_KEYDOWN:
                m_keyStates[e.key.keysym.sym] = true;
                break;

            case SDL_KEYUP:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    return false;

                m_keyStates[e.key.keysym.sym] = false;
                break;

            case SDL_VIDEORESIZE:
                SDL_SetVideoMode(e.resize.w, e.resize.h, 32, SDL_OPENGL | SDL_RESIZABLE);
                gfx::Context::active().setViewport(irect(e.resize.w, e.resize.h));
                m_camera.setAspectRatio((float) e.resize.w / e.resize.h);
                m_camera.update();
                break;

            case SDL_QUIT:
                return false;
            }
        }
        return true;
    }

private:
    Timer m_timer;
    Camera m_camera;
    
    gfx::Renderer m_renderer;
    gfx::MaterialManager m_mm;

    rcptr<Object> m_root, m_arena, m_sky;
    rcptr<Object> m_myself;

    bool m_keyStates[SDLK_LAST];
};

//////////////////////////////////////////////////////////////////////////
// Game stuff

Game::Game()
{
    g_enableLoggingToFile = true;

    initGL();
    ComRegistry::instance().startup();

    m_layer = new BattleScreen(this);
}

Game::~Game()
{
    delete m_layer;
}


void Game::initGL()
{
    auto sdlErr = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if (sdlErr == -1)
        debug("SDL_Init failed: %s", SDL_GetError());

    SDL_WM_SetCaption("MassacreProto", nullptr);

    char sdlEnv[] = "SDL_VIDEO_CENTERED=1";
    SDL_putenv(sdlEnv); // fuck you, putenv(char*)

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);

    SDL_SetVideoMode(1024, 768, 32, SDL_OPENGL | SDL_RESIZABLE);
    
    auto glewErr = glewInit();
    if (glewErr != GLEW_OK)
        debug("glewInit failed: %s", glewGetErrorString(glewErr));

    new gfx::Context;
    debug("OpenGL %s", glGetString(GL_VERSION));

    GLint numExts;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExts);

    debug("Extensions (%d):", numExts);

    for (int i = 0; i < numExts; ++i)
        debug("\t%s", glGetStringi(GL_EXTENSIONS, i));

    //glClearColor(.7f, .7f, .7f, 1);
    //glClearColor(.2f, .2f, .2f, 1);
    glClearColor(0, 0, 0, 1);
    //glPolygonOffset(-1.f, 5.f);

    const char* reqexts[] =
    {
        "GL_ARB_multisample",
        "GL_ARB_vertex_buffer_object",
        "GL_ARB_vertex_array_object",
        "GL_ARB_map_buffer_range",
        "GL_ARB_uniform_buffer_object",
        "GL_ARB_explicit_attrib_location"
    };

    BOOST_FOREACH (auto ext, reqexts)
        if (!glewIsSupported(ext))
            debug("%s: MISSING", ext);

    const struct
    {
        GLenum name;
        const char* string;
    }
    valuesOfInterest[] =
    {
        {GL_SAMPLES,                          "Samples"         },
        {GL_MAX_VERTEX_ATTRIBS,               "Attributes"      },
        {GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, "Texture units"   },
        {GL_MAX_UNIFORM_BUFFER_BINDINGS,      "Max UBO bindings"},
        {GL_MAX_UNIFORM_BLOCK_SIZE,           "Max UBO size"    },
        {GL_MAX_VERTEX_UNIFORM_BLOCKS,        "Max UBOs, vert"  },
        {GL_MAX_GEOMETRY_UNIFORM_BLOCKS,      "Max UBOs, geom"  },
        {GL_MAX_FRAGMENT_UNIFORM_BLOCKS,      "Max UBOs, frag"  }
    };

    BOOST_FOREACH (auto& voi, valuesOfInterest)
    {
        GLint val;
        glGetIntegerv(voi.name, &val);
        debug("%s: %d", voi.string, val);
    }
}

void Game::run()
{
    m_layer->onLoad();
    m_layer->onActivate();

    const int64 fpsDisplayInterval = 500; // num frames between updates
    Timer timer;

    for (int64 frames = 0; m_layer->onRun(); ++frames)
    {
        if (frames == fpsDisplayInterval)
        {
            frames = 0;

            timer.refresh();
            double fps = fpsDisplayInterval / timer.dseconds();

            char caption[32];
            sprintf(caption, "%.2f FPS", fps);
            SDL_WM_SetCaption(caption, nullptr);
        }
    }
}

// 8008135
//////////////////////////////////////////////////////////////////////////

int main()
{
    Game game;
    game.run();

    return 0;
}

#ifdef MCR_PLATFORM_WINDOWS
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    main();
}
#endif
