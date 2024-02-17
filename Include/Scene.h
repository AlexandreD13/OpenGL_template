#ifndef _SCENE_
#define _SCENE_

#include "Node.h"
#include "ModelGL.h"
#include "Camera.h"
#include "Resource_mgr.hpp"
#include "Singleton.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "Logger/ImGuiLogger.h"
#include "Node.h"

class EffectGL;

const string ObjPath = "./../Objects/";
const string MaterialPath = "./../Materials/";
const string EffectPath = "./../Effects/";

class Scene : public Singleton<Scene>{
    friend class Singleton<Scene>;
public:


    Frame* frame();
    Camera* camera();

    Node* getRoot();
    Node* getNode(std::string name);


    template <class R>  R* getModel(string a)
    {
        return m_Models.get<R>(a);
    }

    template <class R> R* getNodes(string a)
    {
        return m_Nodes.get<R>(a);
    }
    template <class R> R* getEffect(string a)
    {
        return m_Effects.get<R>(a);
    }
    void releaseNode(string a);
    void releaseNode(Node *n);


    void releaseModel(string a);
    void releaseModel(ModelGL *m);

    void nextManipulatedNode();
    void manipulateNode(std::string name);

    void displayInterface();

    Node* getSceneNode();
    Node* getManipulatedNode();

    void resizeViewport(int w, int h);


    int getViewportWidth() { return m_width; }
    int getViewportHeight() { return m_height; }

    Resource_mgr<Node> m_Nodes;
    Resource_mgr<EffectGL> m_Effects;
    Resource_mgr<ModelGL> m_Models;
    bool needupdate;

private:
    Scene();
    ~Scene();

    Node* m_Root;
    Node *m_Scene;

    int m_width, m_height;

    Camera* current_Camera;
    Node* current_ManipulatedNode;

};



#endif