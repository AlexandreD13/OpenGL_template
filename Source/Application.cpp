#include <iostream>
#include <chrono>
#include <utility>

#include "Application.h"
#include <glad/glad.h>
#include "Frame.h"
#include "PhongMaterial.h"

#define STB_IMAGE_IMPLEMENTATION
#define M_PI 3.1415926535

bool goForward = false;
bool goBackward = false;
bool goLeft = false;
bool goRight = false;

using namespace std;

static void error_callback(int error, const char* description) {
	LOG_TRACE << "-------------------------------------------------------------" << std::endl;
    LOG_TRACE << "Error :" << error << "\nDescription: " << description << std::endl;
}

Application::Application(int width,int height, std::string name) : m_width(width), m_height(height), m_title(std::move(name)) {
	srand((unsigned int) time(nullptr));

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	// Load all OpenGL functions using the glfw loader function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize OpenGL context" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	try {
		ImGui_ImplGlfwGL3_Init(m_window, false);

		glfwSetMouseButtonCallback(m_window, mouse_callback_glfw);
        glfwSetScrollCallback(m_window, mouse_wheel_callback_glfw);
        glfwSetKeyCallback(m_window, keyboard_callback_glfw);
        glfwSetCharCallback(m_window, ImGui_ImplGlfwGL3_CharCallback);
		glfwSetCursorPosCallback(m_window, mousePos_callback_glfw);
		glfwSetFramebufferSizeCallback(m_window, window_resize_callback_glfw);

    } catch (const std::exception & e) {
        cerr << "Error GLFW Initialization: " << e.what() <<endl;
    }

    try {
		// Direct engine
		m_engine = new EngineGL(m_width, m_height);
		m_scene = Scene::getInstance();
		m_engine->init();
	} catch (const std::exception & e) {
		LOG_ERROR << "Error Engine Initialization: "<< e.what() <<endl;
		Logger::getInstance()->show_interface = true;
	}
    
    m_clear_color = ImColor(120, 120, 120);
	m_display_interface = true;

	m_rotate = m_zoom = m_anim_object = false;
	m_trackball = true;
    m_speed_factor = 0.025f;
	m_camera_speed = 1.f;
	m_wheel = 0.f;

	middle = glm::vec2(m_width/2,m_height/2);
	nMouse = middle;
	m_translate = false;
}

Application::~Application() {
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
    delete m_engine;
	Scene::kill();
}

void Application::displayOverlay(bool display, float milli, float seconds) {
	ImGuiStyle& style = ImGui::GetStyle();
	style.ChildWindowRounding = 8.0f;
	style.WindowRounding = 8.0f;
	style.FrameRounding = 8.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    ImGui::SetNextWindowPos(ImVec2(10, 20));

    if (!ImGui::Begin("Stats", &display,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    ImGui::Separator();    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", milli, 1/seconds);
    ImGui::Separator();

	ImGui::Text("Information:");
	ImGui::Text("Number of Effects: %d", m_scene->m_Effects.size());
	ImGui::Text("Number of Models: %d", m_scene->m_Models.size());
	ImGui::Text("Number of Nodes: %d", m_scene->m_Nodes.size());
	ImGui::Separator();

	static ImVec4 backgroundColor = ImColor(0.5f, 0.5f, 0.5f, 1.0f);
	ImGui::Text("Background Color:");
	ImGui::SameLine();

	if (ImGui::ColorButton(backgroundColor)) {
        ImGui::OpenPopup("Background Color");
    }

    if (ImGui::BeginPopup("Background Color")) {
		ImGui::ColorPicker4("", (float*) &backgroundColor);
		EngineGL::setClearColor(glm::vec4(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w));
		ImGui::EndPopup();
	}

	ImGui::Separator();
	ImGui::Text("Camera parameters:");

	ImGui::Checkbox(" : Manipulate Object Trackball", &m_trackball);
		
	ImGui::SliderFloat(": Speed Factor",&m_speed_factor, 0.001f, 0.050f);
	ImGui::SliderFloat(": Camera Speed",&m_camera_speed, 0.001f, 1.f);
    ImGui::End();

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::EndMenu();
		}

        if (ImGui::BeginMenu("Console")) {
			ImGui::MenuItem("Show Console", nullptr, &Logger::getInstance()->show_interface);
			ImGui::EndMenu();
		}
		
		ImGui::EndMainMenuBar();
	}

	m_scene->displayInterface();
	m_engine->displayInterface();

	if (Logger::getInstance()->show_interface) {
        Logger::getInstance()->Draw("Console");
    }
}

void Application::mousePos_callback_glfw(GLFWwindow* window, double mouseX, double mouseY) {
	auto* g = static_cast<Application*>(glfwGetWindowUserPointer(window));
	g->mousePos_event((int) mouseX, (int) mouseY);
}

void Application::keyboard_callback_glfw(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto* g = static_cast<Application*>(glfwGetWindowUserPointer(window));
	g->keyboard_event(key,scancode,action,mods);
}

void Application::mouse_callback_glfw(GLFWwindow* window, int button, int action, int mods) {
	auto* g = static_cast<Application*>(glfwGetWindowUserPointer(window));
	g->mouse_event(button,action,mods);
}
void Application::mouse_wheel_callback_glfw(GLFWwindow* window, double xOffset, double yOffset) {
	ImGui_ImplGlfwGL3_ScrollCallback(window,xOffset, yOffset);
	auto* g = static_cast<Application*>(glfwGetWindowUserPointer(window));
	g->mouse_wheel_event(yOffset);
}

void Application::window_resize_callback_glfw(GLFWwindow* window, int width, int height) {
	auto* g = static_cast<Application*>(glfwGetWindowUserPointer(window));
	
	if (width > 0 && height > 0) {
        g->windowSize_event(width,height);
    }
}

void Application::windowSize_event(int width, int height) {
	m_width = width;
	m_height = height;

	if (m_engine != nullptr) {
        m_engine->onWindowResize(width, height);
    }
}

void Application::keyboard_event(int key, int scancode, int action, int mods) {
	ImGuiIO& io = ImGui::GetIO();

	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(m_window, GL_TRUE); // Quit Application
    }

	if (action == GLFW_PRESS) {
		io.KeysDown[key] = true;

        switch (key) {
			case GLFW_KEY_TAB:
				m_display_interface = !m_display_interface;
				break;

			case GLFW_KEY_SPACE:
				m_trackball = !m_trackball;
				break;
				default: break;
		}
	} else if (action == GLFW_RELEASE) {
		io.KeysDown[key] = false;
	}

	if (!ImGui::GetIO().WantCaptureKeyboard) {
		if (action == GLFW_PRESS) {
			io.KeysDown[key] = true;

            switch (key) {
                case GLFW_KEY_W:
                    goForward = true;
                    break;

                case GLFW_KEY_S:
                    goBackward = true;
                    break;

                case GLFW_KEY_A:
                    goLeft = true;
                    break;

                case GLFW_KEY_D:
                    goRight = true;
                    break;

                default:
                    break;
			}
		} else if (action == GLFW_RELEASE) {
			io.KeysDown[key] = false;

            switch (key) {
                case GLFW_KEY_W:
                    goForward = false;
                    break;

                case GLFW_KEY_S:
                    goBackward = false;
                    break;

                case GLFW_KEY_A:
                    goLeft = false;
                    break;

                case GLFW_KEY_D:
                    goRight = false;
                    break;

                default:
                    break;
			}
		}
	}
}

void Application::mousePos_event(int x, int y) {
	if (m_trackball) {
		trackballFrame();
	}
}

void Application::mouse_event(int button, int action, int mods) {
	if (!ImGui::GetIO().WantCaptureMouse) {
		nMouse.x = ImGui::GetIO().MousePos.x;
		nMouse.y = ImGui::GetIO().MousePos.y;

		if (m_trackball) {
			oMouse = nMouse;

			if (action == GLFW_PRESS && button == 0) {
				m_rotate = true;
			} else if (action == GLFW_RELEASE && button == 0) {
				m_rotate = false;
				oMouse = middle;
			}

			if (action == GLFW_PRESS && button == 1) {
				m_translate = true;
			} else if (action == GLFW_RELEASE && button == 1) {
				m_translate = false;
			}
		}
	}
}

void Application::mouse_wheel_event(double yOffset) {
	if (!ImGui::GetIO().WantCaptureMouse) {
		if (m_trackball) {
            m_wheel -= (float) yOffset;
        }

		glm::vec3 dep = (float) (yOffset) * -0.5f * m_scene->camera()->frame()->convertDirTo(glm::vec3(0.0, 0.0, -1.0), m_scene->getManipulatedNode()->frame());
		m_scene->getManipulatedNode()->frame()->translate(dep);
	}
}

glm::vec2 Application::getNormalizedMouseCoord(glm::vec2 m) const {
	glm::vec2 v = glm::vec2((float)m.x / (float)m_width,1.0f - (float)m.y / (float)m_height);
	v = glm::vec2(2.0f) * v - glm::vec2(1.0f);
	return v;
}

glm::vec3 Application::projectOnSphere(glm::vec2 pos) {
	glm::vec3 v;
	v[0] = pos[0];
	v[1] = pos[1];

	if (v.x * v.x + v.y * v.y <= 0.5f) {
        v.z = sqrt(1.0f - (v.x * v.x + v.y * v.y));
    } else {
        v.z = (0.5f) / sqrt(v.x * v.x + v.y * v.y);
    }

	return v;
}

void Application::trackballFrame() {
	glm::vec2 v, o;
	nMouse.x = ImGui::GetIO().MousePos.x;
	nMouse.y = ImGui::GetIO().MousePos.y;
	
	v = getNormalizedMouseCoord(nMouse);
	o = getNormalizedMouseCoord(oMouse);
	
	if (m_rotate) {
		if (length(v - o) > 0.001) {
            rotateFrameTrackBall(v, o);
        }
	} else if (m_translate) {
		translateFrameTrackball(v, o);
	}

	oMouse = nMouse;
}

void Application::translateFrameTrackball(glm::vec2 v, glm::vec2 o) {
	glm::vec2 dis = v - o;

    Frame* f = m_scene->getManipulatedNode()->frame();
    glm::vec3 dir = m_scene->camera()->frame()->convertDirTo(glm::vec3(dis, 0.0), f);
	glm::vec3 CameraInF = m_scene->camera()->frame()->convertPtTo(glm::vec3(0.0, 0.0, 0.0), f);
	glm::vec3 ScreenDistanceInF = m_scene->camera()->frame()->convertDirTo(glm::vec3(0.0, 0.0, 1.0), f);

	f->translate((dir * glm::length(CameraInF)/glm::length(ScreenDistanceInF)));
}

void Application::rotateFrameTrackBall(glm::vec2  v, glm::vec2 o) {
	glm::vec3 v1 = projectOnSphere(v);
	glm::vec3 v2 = projectOnSphere(o);

	Frame* f = m_scene->getManipulatedNode()->frame();

	v1 = glm::normalize(v1);
	v2 = glm::normalize(v2);

	glm::vec3 rax = glm::normalize(glm::cross(v2, v1));
	if (!isnan(rax.x)) {
		float angle = acos(glm::dot(v1, v2));
		rax = glm::normalize(m_scene->camera()->frame()->convertDirTo(rax, f));
        f->rotate(rax, (float)(M_PI * angle ));
	}
}

void Application::animate(const float elapsedTime) {
	m_engine->animate(elapsedTime);
	
	if (goForward) {
        m_scene->camera()->frame()->translate(glm::vec3(0.0, 0.0, -m_camera_speed));
    } else if (goBackward) {
        m_scene->camera()->frame()->translate(glm::vec3(0.0, 0.0, m_camera_speed));
    }

	if (goRight) {
        m_scene->camera()->frame()->translate(glm::vec3(m_camera_speed, 0.0, 0.0));
    } else if (goLeft) {
        m_scene->camera()->frame()->translate(glm::vec3(-m_camera_speed, 0.0, 0.0));
    }
}

void Application::mainLoop() {
    std::chrono::time_point<std::chrono::high_resolution_clock> last_time, now_time;
	LOG_INFO << "Beginning Main Loop" << std::endl;

    while(!glfwWindowShouldClose(m_window)) {
		// Time calculation
        last_time = now_time;	
		now_time = std::chrono::high_resolution_clock::now();
   		auto elapsed_Time = now_time - last_time;
		auto elapsed_milli = (float) chrono::duration <double, milli> (elapsed_Time).count();
		auto elapsed_second = (float) chrono::duration <double, ratio<1,1> > (elapsed_Time).count();
        
        // Enlivenment handling and interface rendering
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();
        displayOverlay(m_display_interface, elapsed_milli, elapsed_second);
				
        // Rendering
		m_engine->render();

		// update		
        animate(elapsed_milli);	

		if(m_display_interface) {
            ImGui::Render();
        }

		glfwSwapBuffers(m_window);
    }

    LOG_INFO << "Ending Main Loop" << std::endl;
}