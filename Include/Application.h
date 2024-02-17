#ifndef APPLICATION
#define APPLICATION

#include <glad/glad.h>
#include <GLFW/include/glfw3.h>
#include <glm/glm.hpp>
#include <string>

#include "EngineGL.h"
#include "Scene.h"
#include "imgui/imgui.h"
#include "Logger/ImGuiLogger.h"

class Application
{
    public:
        explicit Application(int width = 1024, int height=1024, std::string name="My OpenGL Engine");
        ~Application();
        void mainLoop();
        
        // Display Imgui Overlay
        void displayOverlay(bool display, float milli, float fps);

        // Draw & Animation
        void animate(float elapsedTime);

        //KeyBoard Event
        static void keyboard_callback_glfw(GLFWwindow * window, int key, int scancode,int action, int mods);
        static void mouse_callback_glfw(GLFWwindow * window, int button, int action, int mods);
        static void mouse_wheel_callback_glfw(GLFWwindow * window, double xOffset, double yOffset);
		static void mousePos_callback_glfw(GLFWwindow* win, double mouseX, double mouseY);
		static void window_resize_callback_glfw(GLFWwindow* win, int width, int height);

		void mousePos_event(int x, int y);
        void keyboard_event(int key, int scancode, int action, int mods);
        void mouse_event(int button, int action, int mods);
        void mouse_wheel_event(double yOffset);
		void windowSize_event(int width,int height);

    private:
        static glm::vec3 projectOnSphere(glm::vec2 pos);
        glm::vec2 getNormalizedMouseCoord(glm::vec2 m) const;

        void trackballFrame();
        void translateFrameTrackball(glm::vec2 v, glm::vec2 o);
        void rotateFrameTrackBall(glm::vec2 v, glm::vec2 o);

        GLFWwindow* m_window;
        int m_height, m_width;
        std::string m_title;

        ImVec4 m_clear_color;
        bool m_show_overlay{};
        bool m_display_interface;

		EngineGL* m_engine;
        Scene* m_scene;
        bool m_anim_object;

        // Camera Attributes
        bool m_trackball, m_firstperson{}, m_translate;       // Mode
        bool mode_on{}, m_rotate, m_zoom;                     // Trackball operations
        float m_speed_factor, m_camera_speed, m_wheel;

        float angleYaw{}, anglePitch{};
        glm::vec3 camPos{}, camDir{};
        glm::vec2 oMouse{}, nMouse{};
        glm::vec2 middle{};
};

#endif