#include "Universe.h"

#define GLFW_NO_GLU
#include <GL/glfw.h>

#include <mcr/Config.h>
#include <mcr/Timer.h>

#include <mcr/gfx/Camera.h>
#include <mcr/gfx/Renderer.h>
#include <mcr/gfx/MaterialManager.h>
#include <mcr/gfx/MeshLoader.h>

using namespace mcr;
using namespace gfx;

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
    virtual void onResize(const ivec2& newSize) = 0;

protected:
    Game* m_game;
};

// Game
class Game
{
public:
    Game();
    ~Game();

    void initGL();
    void run();

protected:
    GameLayer* m_layer;

private:
    static void onWindowSize(int w, int h);

    static bool s_windowResized;
    static ivec2 s_newWindowSize;
};

//////////////////////////////////////////////////////////////////////////
// Game layer stuff

class BattleScreen: public GameLayer
{
public:
    BattleScreen(Game* game): GameLayer(game) {}

    void onLoad()
    {
        m_camera.setZRange(vec2(1.f, 3000.f));
        m_camera.update();


        m_commonParameters = MaterialParameterBuffer::create(
            "Common", MaterialParameterBufferLayout()
                (MaterialParameterType::Float, "Time")
                (MaterialParameterType::Float, "DeltaTime"));

        m_sunParameters = MaterialParameterBuffer::create(
            "Sun", MaterialParameterBufferLayout()
                (MaterialParameterType::Vec3,  "Direction")
                (MaterialParameterType::Vec4,  "Color")
                (MaterialParameterType::Vec4,  "ShadowColor")
                (MaterialParameterType::Float, "Brightness"),
            MaterialParameterBuffer::Static);

        m_sunParameters->parameter("Direction")   = math::normalize(vec3(1, 1, -1));
        m_sunParameters->parameter("Color")       = vec4(1, .9f, .7f, 1);
        m_sunParameters->parameter("ShadowColor") = vec4(vec3(.5f), 1);
        m_sunParameters->parameter("Brightness")  = 3.f;

        m_mm.addParameterBuffer(m_camera.parameterBuffer());
        m_mm.addParameterBuffer(m_commonParameters);
        m_mm.addParameterBuffer(m_sunParameters);

        m_sunParameters->sync();


        if (!m_mm.fs()->attachResource("DataArena/")
        &&  !m_mm.fs()->attachResource("/usr/share/massacre/"))
        {
            debug("Can't find data directory");
            exit(1);
        }

        m_config.load(m_mm.fs()->openFile("mainconf.yaml"));

        m_turnSpeed = m_config["turn_speed"] || 60.f;
        m_velocity  = m_config["velocity"]   || 100.f;
        m_reach     = m_config["reach"]      || 780.f;

        loadArena();
        buildSky();
        buildGates();
    }

    void loadArena()
    {
        m_opaque      = m_mm.getMaterial("Materials/opaque.mtl");
        m_transparent = m_mm.getMaterial("Materials/trans.mtl");
        m_translucent = m_mm.getMaterial("Materials/leaves.mtl");
        m_flags       = m_mm.getMaterial("Materials/flags.mtl");

        MeshLoader loader(m_mm.fs());

        std::vector<Mesh> meshes;

        m_arenaBuffer = loader.load(
            MeshFileSet()
                ("Meshes/opaque.mesh")
                ("Meshes/trans.mesh")
                ("Meshes/leaves.mesh")
                ("Meshes/flags.mesh"),
            meshes);

        if (m_arenaBuffer)
        {
            m_opaqueMesh      = meshes[0];
            m_transparentMesh = meshes[1];
            m_translucentMesh = meshes[2];
            m_flagsMesh       = meshes[3];
        }
    }

    void buildSky()
    {
        m_sky = m_mm.getMaterial("Materials/sky.mtl");

        float skyVertices[] =
        {
            -50,50, 50,  0.25f,0.75f,   50,50, 50,  0.75f,0.75f,
             50,50,-50,  0.75f,0.25f,  -50,50,-50,  0.25f,0.25f,
            -50, 0, 50,  0.25f,1.00f,   50, 0, 50,  0.75f,1.00f,
             50, 0, 50,  1.00f,0.75f,   50, 0,-50,  1.00f,0.25f,
             50, 0,-50,  0.75f,0.00f,  -50, 0,-50,  0.25f,0.00f,
            -50, 0,-50,  0.00f,0.25f,  -50, 0, 50,  0.00f,0.75f
        };
        uint skyIndices[] =
        {
            0, 2, 1,  0, 3, 2,
            0, 5, 4,  0, 1, 5,
            1, 7, 6,  1, 2, 7,
            2, 9, 8,  2, 3, 9,
            3,11,10,  3, 0,11
        };

        m_skyBuffer = VertexArray::create(
            skyVertices, sizeof(skyVertices),
            skyIndices,  sizeof(skyIndices));

        m_skyBuffer->setFormat("p3f _ t2f");

        Mesh mesh = {m_skyBuffer, 0, 12, 0, 30, PrimitiveType::Triangles};
        m_skyMesh = mesh;
    }

    void buildGates()
    {
        m_quasicrystal = m_mm.getMaterial("Materials/quasicrystal.mtl");

        float spriteVertices[] =
        {
            0,0,0,0,0, 1,0,0,1,0, 1,1,0,1,1, 0,1,0,0,1,
            0,0,0,0,0, 1,0,0,1,0, 1,1,0,1,1, 0,1,0,0,1
        };
        uint spriteIndices[] = {0,1,2,0,2,3, 4,5,6,4,6,7};

        m_gateBuffer = VertexArray::create(
            spriteVertices, sizeof(spriteVertices),
            spriteIndices, sizeof(spriteIndices));

        m_gateBuffer->setFormat("p3f _ t2f");

        m_gateBuffer->transformAttribs(0, math::buildTransform(vec3(-62.5f, 0, 0), vec3(), vec3(125, 250, 0)));
        m_gateBuffer->transformAttribs(2, math::buildTransform(vec3(), vec3(), vec3(1, 2, 0)));

        m_gateBuffer->transformAttribs(0, math::buildTransform(vec3(957, 0, 5), vec3(0, -90, 0)), 0, 4);
        m_gateBuffer->transformAttribs(0, math::buildTransform(vec3(-837, 0, 22), vec3(0, 90, 0)), 4, 8);

        Mesh mesh = {m_gateBuffer, 0, 8, 0, 12, PrimitiveType::Triangles};
        m_gateMesh = mesh;
    }

    void onUnload() {}


    void onActivate()
    {
        m_pos.setY(20);
    }

    void onDeactivate() {}


    bool onRun()
    {
        m_timer.refresh();
        m_commonParameters->parameter("Time")      = m_timer.seconds();
        m_commonParameters->parameter("DeltaTime") = m_timer.dseconds();
        m_commonParameters->sync();

        handleKeys();
        render();

        return handleEvents();
    }

    void onResize(const ivec2& size)
    {
        m_renderer.setViewport(size);
        m_camera.setAspectRatio((float) size.x() / size.y());
        m_camera.update();
    }

    void render()
    {
        m_renderer.clear();

        m_camera.dumpMatrices();

        m_renderer.setActiveMaterial(m_sky);
        m_renderer.drawMesh(m_skyMesh);

        m_renderer.setActiveMaterial(m_opaque);
        m_renderer.drawMesh(m_opaqueMesh);

        m_renderer.setActiveMaterial(m_flags);
        m_renderer.drawMesh(m_flagsMesh);

        m_renderer.setActiveMaterial(m_transparent);
        m_renderer.drawMesh(m_transparentMesh);

        m_renderer.setActiveMaterial(m_translucent);
        m_renderer.drawMesh(m_translucentMesh);

        m_renderer.setActiveMaterial(m_quasicrystal);
        m_renderer.drawMesh(m_gateMesh);

        glfwSwapBuffers();
    }

    void handleKeys()
    {
        auto mv = math::normalize(vec3(
                0.f + glfwGetKey('E') - glfwGetKey('Q'), 0,
                0.f + glfwGetKey('S') - glfwGetKey('W')));

        auto yAngle = math::deg2rad * m_rot.y();
        auto ySine = sin(yAngle), yCosine = cos(yAngle);

        auto dir = mv.x() * vec3(yCosine, 0, -ySine) + mv.z() * vec3(ySine, 0, yCosine);
        auto velocity = m_velocity;


        if (!math::equals(m_reach, 0.f))
        {
            auto bias     = math::normalize(vec2(m_pos.x(), m_pos.z()));
            auto affinity = .7071068f * math::length(bias + vec2(dir.x(), dir.z()));
            auto tensity  = math::length(vec2(m_pos.x(), m_pos.z())) / m_reach;

            velocity *= std::max(0.f, 1.f - affinity * tensity);
        }

        m_pos += (float) m_timer.dseconds() * velocity * dir;

        auto turnStep = (float) m_timer.dseconds() * -m_turnSpeed;
        m_rot += vec3(0, turnStep * (glfwGetKey('D') - glfwGetKey('A')), 0);


        static const vec3 camOffset(0, 35, 0);

        auto tf = math::buildTransform(vec3(), m_rot);

        m_camera.setViewMatrix(tf, true);
        m_camera.setPosition(m_pos + camOffset * tf);
    }

    // [Esc]
    bool handleEvents()
    {
        if (glfwGetKey(GLFW_KEY_ESC))
            glfwCloseWindow();

        return !!glfwGetWindowParam(GLFW_OPENED);
    }

private:
    Config m_config;
    Timer m_timer;

    Renderer m_renderer;
    Camera m_camera;

    MaterialManager m_mm;
    rcptr<MaterialParameterBuffer> m_commonParameters, m_sunParameters;

    rcptr<Material> m_opaque, m_transparent, m_translucent, m_flags, m_sky, m_quasicrystal;

    rcptr<VertexArray> m_arenaBuffer, m_skyBuffer, m_gateBuffer;
    Mesh m_opaqueMesh, m_transparentMesh, m_translucentMesh, m_flagsMesh, m_skyMesh, m_gateMesh;

    vec3 m_pos, m_rot;
    uint m_moveFlags;
    float m_turnSpeed, m_velocity, m_reach;
};


//////////////////////////////////////////////////////////////////////////
// Game stuff

bool Game::s_windowResized = false;
ivec2 Game::s_newWindowSize;

Game::Game()
{
    g_enableLoggingToFile = true;

    initGL();

    m_layer = new BattleScreen(this);
}

Game::~Game()
{
    delete m_layer;
}


void Game::initGL()
{
    if (!glfwInit())
        debug("glfwInit failed");

#ifdef MCR_PLATFORM_MAC
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);

    if (!glfwOpenWindow(1024, 768, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
        debug("glfwOpenWindow failed");

    glfwSetWindowTitle("MassacreProto");
    glfwSetWindowSizeCallback(&Game::onWindowSize);

    GLFWvidmode desktop;
    glfwGetDesktopMode(&desktop);
    glfwSetWindowPos((desktop.Width - 1024) / 2, (desktop.Height - 768) / 2 - 30); // slightly above the center

    glfwSwapInterval(0);
    
    glewExperimental = true;

    auto glewErr = glewInit();
    if (glewErr != GLEW_OK)
        debug("glewInit failed: %s", glewGetErrorString(glewErr));

    new Renderer;
    debug("OpenGL %s", glGetString(GL_VERSION));
    debug("GLSL %s",   glGetString(GL_SHADING_LANGUAGE_VERSION));

    GLint numExts;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExts);

    debug("Extensions (%d):", numExts);

    for (int i = 0; i < numExts; ++i)
        debug("\t%s", glGetStringi(GL_EXTENSIONS, i));

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

    for (int64 frames = 0;; ++frames)
    {
        if (!m_layer->onRun())
            break;

        if (s_windowResized)
        {
            m_layer->onResize(s_newWindowSize);
            s_windowResized = false;
        }

        if (frames == fpsDisplayInterval)
        {
            frames = 0;

            timer.refresh();
            double fps = fpsDisplayInterval / timer.dseconds();

            char caption[32];
            sprintf(caption, "%.2f FPS", fps);
            glfwSetWindowTitle(caption);
        }
    }
}

void Game::onWindowSize(int w, int h)
{
    s_windowResized = true;
    s_newWindowSize.set(w, h);
}

// 8008135
//////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    Game game;
    game.run();

    return 0;
}

#ifdef MCR_PLATFORM_WINDOWS
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    std::string commandLine = GetCommandLineA();

    std::vector<std::string> arguments;
    boost::split(arguments, commandLine, boost::is_space());

    std::vector<char*> argv;
    BOOST_FOREACH (auto& arg, arguments)
    {
        boost::trim_if(arg, boost::is_any_of("\""));

        if (arg.size())
            argv.push_back(&arg[0]);
    }

    main((int) argv.size(), &argv[0]);
}
#endif
