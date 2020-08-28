#include "Window.hpp"

#include "events/Mouse.hpp"
#include "events/Keyboard.hpp"
#include "events/WindowEvent.hpp"

namespace Monarch {

Window::Window(const char* title, int _width, int _height){
    data.width = _width;
    data.height = _height;

    glewExperimental = true;
    if (!glfwInit()){
        fprintf( stderr, "Failed to initialize GLFW\n");
        return;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( _width, _height, title, NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, &data);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }

    setGLFWEventCallbacks();
	disableCursor();
}

void Window::update() const {
    glfwWaitEventsTimeout(0.016);
    //glViewport(0, 0, width, height);
    glfwSwapBuffers(window);
	if (!data.cursorEnabled) glfwSetCursorPos(window, data.width/2, data.height/2);
}

void Window::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::destroy() const {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::setGLFWEventCallbacks(){
    glfwSetWindowSizeCallback(window, [](GLFWwindow* _window, int width, int height)
		{
			WindowData& data  = *(WindowData*)glfwGetWindowUserPointer(_window);
			data.width = width;
			data.height = height;

			WindowResizedEvent event(width, height);
			data.callback(event);
		});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* _window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
			WindowClosedEvent event;
			data.callback(event);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* _window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, false);
					data.callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.callback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.callback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* _window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.callback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(window, [](GLFWwindow* _window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

			MouseScrolledEvent event(xOffset, yOffset);
			data.callback(event);
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* _window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.callback(event);
		}
	);
}	


void Window::disableCursor(){
    data.cursorEnabled = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Window::enableCursor(){
    data.cursorEnabled = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}

}
