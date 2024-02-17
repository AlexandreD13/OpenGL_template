
#pragma once

#include "GLProgramPipeline.h"

#include "FrameBufferObject.h"
#include "Scene.h"

class EffectGL
{
public :


    //Constructeur-Destructeur

    /**
        @brief class constructor
        @param name : effect name for identification purposes
    */
    EffectGL(string name="");

    /**
        Class destructor
    */
    ~EffectGL();

    //Méthodes

    /**
        @brief Virtual method to apply the effect to a src FBO, targeting a target FBO (NULL for direct display). In theory, passing (and binding) the FBO as parameters each frame is not the most efficient method but it improves usability and code readability for IMN401
        @param src : source FBO
        @param target : target FBO
    */
    virtual void apply(FrameBufferObject *src,FrameBufferObject *target);


    virtual void animate( const float elapsedTime);


    string getName() { return m_Name; };




    //Interface for ImGUI
    bool show_interface;
    virtual void displayInterface() {};

protected :
    std::string m_Name;
    GLProgramPipeline* m_ProgramPipeline;
    Scene* scene;
    ModelGL* quad;

    /**
    @brief Bind the pipeline and draw a quad covering viewport in 2D.
    */
    virtual void drawQuad();

};