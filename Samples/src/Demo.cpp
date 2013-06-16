#define GLFW_NO_GLU
#include <GL/glfw.h>

#include <mcr/Config.h>
#include <mcr/Timer.h>
#include <mcr/Debug.h>

#include <mcr/gfx/Camera.h>
#include <mcr/gfx/Renderer.h>
#include <mcr/gfx/mtl/Manager.h>
#include <mcr/gfx/geom/MeshManager.h>
#include <mcr/gfx/geom/mem/NaiveMemory.h>

using namespace mcr;
using namespace gfx;

//////////////////////////////////////////////////////////////////////////
// Scene

struct Scene
{
    struct
    {
        rcptr<mtl::Material>
            opaque,
            transparent,
            translucent,
            flags,
            sky,
            quasicrystal;
    }
    materials;

    struct
    {
        geom::Mesh
            opaque,
            transparent,
            translucent,
            flags,
            sky,
            gates;
    }
    meshes;


    void load(mtl::Manager& mtlm, geom::MeshManager& meshm)
    {
        auto fs = mtlm.fs();

        auto sun = mtl::ParamBuffer::create(
            "Sun", mtl::ParamLayout()
                .addVec3("Direction")
                .addVec4("Color")
                .addVec4("ShadowColor")
                .addFloat("Brightness"),
            mtl::ParamBuffer::Static);

        mtlm.addParamBuffer(sun);

        sun->param("Direction")   = math::normalize(vec3(-1, -1, 1));
        sun->param("Color")       = vec4(1, .9f, .7f, 1);
        sun->param("ShadowColor") = vec4(vec3(.5f), 1);
        sun->param("Brightness")  = 3.f;
        sun->sync();

        materials.opaque       = mtlm.getMaterial("Materials/opaque.mtl");
        materials.transparent  = mtlm.getMaterial("Materials/trans.mtl");
        materials.translucent  = mtlm.getMaterial("Materials/leaves.mtl");
        materials.flags        = mtlm.getMaterial("Materials/flags.mtl");

        materials.sky          = mtlm.getMaterial("Materials/sky.mtl");
        materials.quasicrystal = mtlm.getMaterial("Materials/quasicrystal.mtl");


        meshm.loadStatic(fs->openReader("Meshes/opaque.mesh"), meshes.opaque);
        meshm.loadStatic(fs->openReader("Meshes/trans.mesh"),  meshes.transparent);
        meshm.loadStatic(fs->openReader("Meshes/leaves.mesh"), meshes.translucent);
        meshm.loadStatic(fs->openReader("Meshes/flags.mesh"),  meshes.flags);

        meshm.loadStatic(fs->openReader("Meshes/sky.mesh"),    meshes.sky);
        meshm.loadStatic(fs->openReader("Meshes/gates.mesh"),  meshes.gates);
    }

    void render(Renderer& renderer)
    {
        renderer.clear();

        renderer.setActiveMaterial(materials.sky);
        renderer.drawMesh(meshes.sky);

        renderer.setActiveMaterial(materials.opaque);
        renderer.drawMesh(meshes.opaque);

        renderer.setActiveMaterial(materials.flags);
        renderer.drawMesh(meshes.flags);

        renderer.setActiveMaterial(materials.transparent);
        renderer.drawMesh(meshes.transparent);

        renderer.setActiveMaterial(materials.translucent);
        renderer.drawMesh(meshes.translucent);

        renderer.setActiveMaterial(materials.quasicrystal);
        renderer.drawMesh(meshes.gates);
    }
};


//////////////////////////////////////////////////////////////////////////
// App

class Demo
{
public:
    Demo()
    {
        if (!m_mtlm.fs()->setRoot("DataArena/")
        &&  !m_mtlm.fs()->setRoot("/usr/share/massacre/"))
        {
            debug("Can't find data directory");
            exit(1);
        }

        m_config.load(m_mtlm.fs()->openReader("mainconf.yaml"));

        m_config.query("velocity",   m_velocity,  200.f);
        m_config.query("turn_speed", m_turnSpeed, 60.f);


        m_camera.setZRange(vec2(10.f, 3000.f));
        m_camera.update();
        m_pos.setY(20);

        m_commonParams = mtl::ParamBuffer::create(
            "Common", mtl::ParamLayout()
                .addFloat("Time")
                .addFloat("DeltaTime"));

        m_mtlm.addParamBuffer(m_camera.paramBuffer());
        m_mtlm.addParamBuffer(m_commonParams);

        m_timer.start();
        m_scene.load(m_mtlm, m_meshm);
        m_timer.refresh();

        debug("Scene load time: %f seconds", m_timer.seconds());
    }

    void run()
    {
        while (handleEvents())
        {
            handleKeys();

            m_timer.refresh();
            m_commonParams->param("Time")      = m_timer.seconds();
            m_commonParams->param("DeltaTime") = m_timer.dseconds();
            m_commonParams->sync();

            m_camera.dumpMatrices();
            m_scene.render(m_renderer);

            glfwSwapBuffers();

            measureFps();

            if (s_windowSize != m_renderer.viewport().size())
            {
                m_renderer.setViewport(s_windowSize);
                m_camera.setAspectRatio((float) s_windowSize.x() / s_windowSize.y());
                m_camera.update();
            }
        }
    }

    static void measureFps()
    {
        static int64 s_frames = 0;
        static Timer s_timer;

        const int64 updateInterval = 500;

        if (++s_frames % updateInterval == 0)
        {
            s_timer.refresh();
            double fps = updateInterval / s_timer.dseconds();

            char caption[32];
            sprintf(caption, "%.2f FPS", fps);
            glfwSetWindowTitle(caption);
        }
    }

    static void resizeWindow(int w, int h)
    {
        s_windowSize.set(w, h);
    }

    void handleKeys()
    {
        auto rotationControl = 0.f + glfwGetKey('A') - glfwGetKey('D');

        auto movementControl = math::normalize(vec3(
                0.f + glfwGetKey('E') - glfwGetKey('Q'), 0,
                0.f + glfwGetKey('S') - glfwGetKey('W')));

        movementControl *= math::buildTransform(vec3(), m_rot);


        auto dtime = (float) m_timer.dseconds();

        m_pos += dtime * m_velocity * movementControl;
        m_rot += vec3(0, dtime * m_turnSpeed * rotationControl, 0);


        static const vec3 camOffset(0, 35, 0);

        auto rotation = math::buildTransform(vec3(), m_rot);

        m_camera.setViewMatrix(rotation, true);
        m_camera.setPosition(m_pos + camOffset * rotation);
    }

    // [Esc]
    bool handleEvents()
    {
        if (glfwGetKey(GLFW_KEY_ESC))
            glfwCloseWindow();

        return !!glfwGetWindowParam(GLFW_OPENED);
    }

private:
    static ivec2            s_windowSize;

    Config                  m_config;
    Timer                   m_timer;

    Renderer                m_renderer;
    Camera                  m_camera;

    mtl::Manager            m_mtlm;
    rcptr<mtl::ParamBuffer> m_commonParams;
    geom::MeshManager       m_meshm;
    Scene                   m_scene;

    vec3                    m_pos, m_rot;
    float                   m_velocity, m_turnSpeed;
};

ivec2 Demo::s_windowSize;

// 8008135
//////////////////////////////////////////////////////////////////////////


void initWindow(int w, int h, GLFWwindowsizefun onResize)
{
    if (!glfwInit())
        debug("glfwInit failed");

#ifdef MCR_PLATFORM_MAC
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef _DEBUG
    glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#endif

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);

    if (!glfwOpenWindow(w, h, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
        debug("glfwOpenWindow failed");

    glfwSetWindowTitle("MassacreProto");
    glfwSetWindowSizeCallback(onResize);

    GLFWvidmode desktop;
    glfwGetDesktopMode(&desktop);
    glfwSetWindowPos((desktop.Width - w) / 2, (desktop.Height - h) / 2 - 30); // slightly above the center

    glfwSwapInterval(0);
}

int main()
{
    g_enableLoggingToFile = true;

    initWindow(1024, 768, &Demo::resizeWindow);

    Demo app;
    app.run();
}