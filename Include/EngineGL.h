
#ifndef _ENGINE_GL_H
#define _ENGINE_GL_H

#include <map>
#include <string>
#include <stack>
#include <glad/glad.h>
#include "NodeCollector.h"
#include "Scene.h"
#include <glm/glm.hpp>


#include "FrameBufferObject.h"
#include "Display.h"

class EngineGL 
{
public:
	/**
	 * @brief	Creates an forward rendering engine
	 * @details Creates an forward rendering engine with predefined cameraGL, timers objects and a LightingModelGL
	 * @param	width as rendering frame width
	 * @param	height as rendering frame height
	 */
	EngineGL(int width, int height);
	/**
	 * @brief      Destroy an engine
	 */

	~EngineGL();
	/**
	 * @brief	Initialize the engine
	 * @return success of the initialization
	 */

	bool init();
	/**
	 * @brief	Load a Scene according to the provided filename
	 * @param	filename of a target scene in a known format (.DAE,.3DS,.OBJ)
	 * @return success of the loading operation
	 */
	bool loadScene(std::string filename);
	/**
	 * @brief	Render the scene. In the parent class, this is a forward rendering of each node of the scene
	 */
	void render();
	/**
	 * @brief	Animate the scene. This function should provide update for camera movement, node animation (by calling the animate function of each node).
	 */
	void animate(const float elapsedTime);

	/**
	 * @brief	Resize the viewport
	 * @param	w as width of the viewport
	 * @param	h as height of the viewport
	 */

	void onWindowResize(int w, int h);
	/**
	 * @brief	Get Time stamp of the last rendered frame
	 * @return timestamp of the last rendered frame
	 */
	double getFrameTime();
	/**
	 * @brief	Get width of the current frame
	 * @return width of the frame
	 */
	int getWidth() {
		return m_Width;
	};
	/**
	 * @brief	Get height of the current frame
	 * @return height of the frame
	 */
	int getHeight() {
		return m_Height;
	};

	

	/**
	 * @brief	Define the background color
	 * @param color the background color
	 */
	static void setClearColor(glm::vec4 color);

	/**
	 * @brief	Prepare the engine for rendering. Should be called at the end of init
	 * @details Set up the engine. Parse nodes using standard collectors, bind and populates the lighting model with LightNodes
	 */
	virtual void setupEngine();

	
	/**
	 * @brief	Display interface for the engine (using imgui)
	 */
	virtual void displayInterface();

protected:
	int m_Width;
	int m_Height;

	Scene* scene;
	NodeCollector* allNodes{};
    FrameBufferObject* myFBO;
	Display* display{};
};
#endif