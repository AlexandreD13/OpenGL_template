#include "EngineGL.h"
#include "Scene.h"
#include "BaseMaterial.h"
#include "PhongMaterial.h"
#include "RotationMaterial.h"

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
    auto const src_str = [source]() {
        switch (source)
        {
            case GL_DEBUG_SOURCE_API: return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER: return "OTHER";
            default:return "";
        }
    }();

    auto const type_str = [type]() {
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR: return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "MARKER";
            case GL_DEBUG_TYPE_OTHER: return "OTHER";
            default:return "";
        }
    }();

    auto const severity_str = [severity]() {
        switch (severity) {
            case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
            case GL_DEBUG_SEVERITY_LOW: return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
            default:return "";
        }
    }();
    LOG_INFO << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}



EngineGL::EngineGL(int width, int height)
{
    m_Width = width;
    m_Height = height;

    myFBO = NULL;

    scene = Scene::getInstance();
    scene->resizeViewport(m_Width, m_Height);

}

EngineGL::~EngineGL()
{
}


void EngineGL::setupEngine()
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, m_Width, m_Height);
    setClearColor(glm::vec4(0.5,0.5,0.5,1.0));

    this->allNodes = new NodeCollector();

    allNodes->collect(scene->getRoot());

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(message_callback, nullptr);

    LOG_INFO << "initialisation complete" << std::endl;
}

bool EngineGL::init()
{
    LOG_INFO << "Initializing Scene" << std::endl;

    BaseMaterial* sphereMaterial = new BaseMaterial("Sphere");
    RotationMaterial* rotationMaterial = new RotationMaterial("Rotation");
    PhongMaterial* phongMaterial = new PhongMaterial("Phong");
    PhongMaterial* phongMaterialWall = new PhongMaterial("PhongWall");

    Node* bunny = scene->getNode("Bunny");
    bunny->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Bunny.obj"));
    bunny->frame()->scale(glm::vec3(30.0));
    bunny->setMaterial(phongMaterial);
    scene->getSceneNode()->adopt(bunny);

    Node* sol = scene->getNode("Sol");
    sol->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Wall.obj"));
    sol->setMaterial(phongMaterialWall);
    sol->frame()->translate(glm::vec3(0.0, -2.3, 0.0));
    scene->getSceneNode()->adopt(sol);

    Node* A = scene->getNode("A");
    bunny->adopt(A);

    Node* lumiere = scene->getNode("Light");
    lumiere->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Sphere.obj"));
    lumiere->setMaterial(sphereMaterial);
    lumiere->frame()->translate(glm::vec3(0.5, 0.0, 0.0));
    lumiere->frame()->scale(glm::vec3(0.1));

    A->adopt(lumiere);
    A->setMaterial(rotationMaterial);

    setupEngine();
    return(true);
}


void EngineGL::render ()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (unsigned int i = 0; i < allNodes->nodes.size(); i++)
        allNodes->nodes[i]->render();
}

void EngineGL::animate (const float elapsedTime)
{
    // Animate each node
    for (unsigned int i = 0; i < allNodes->nodes.size(); i++)
    {
        allNodes->nodes[i]->animate(elapsedTime);
    }

    // Update Camera Buffer
    scene->camera()->updateBuffer();
}

void EngineGL::onWindowResize(int w, int h)
{
    m_Width = w;
    m_Height = h;
    glViewport(0, 0, w, h);
    float ratio = (float)w / (float)h;

    scene->resizeViewport(w,h);
    scene->camera()->setPerspectiveProjection(glm::radians(45.0f), ratio, 1.0f, 2000.0f);

}

void EngineGL::setClearColor(glm::vec4 color)
{
    glClearColor(color.x, color.y, color.z,color.w);
}
void EngineGL::displayInterface()
{
    if (myFBO)
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("FBOs"))
            {
                ImGui::MenuItem(myFBO->getName().c_str(), NULL, &(myFBO->show_interface));
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        myFBO->displayInterface();
    }

}