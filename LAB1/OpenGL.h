#pragma once

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include "Event.h"

struct MouseWheelEventArg
{
	double value;
};

struct MouseButtonEventArg
{
	int button;
	int action;
};

struct MouseEventArg
{
	short x;
	short y;
};

struct KeyEventArg
{
	int key;
	int action;
};


class OpenGL
{
	bool render_flag;

	int window_width;
	int window_height;

	GLFWwindow* window;

	Event<OpenGL*, const MouseEventArg&> _mouseMoveEvent;
	Event<OpenGL*, const MouseWheelEventArg&> _wheelEvent;
	Event<OpenGL*, const MouseButtonEventArg&> _mouseButtonEvent;
	Event<OpenGL*, const KeyEventArg&> _keyEvent;

	static void window_size_callback(GLFWwindow* window, int width, int height);

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void PreRender();
	void resize();

	void drawAxisAndCell();
	
	double old_time = 0;

public:

	
	auto& mouseMoveEvent() { return _mouseMoveEvent; }
	auto& wheelEvent() { return _wheelEvent; }
	auto& mouseButtonEvent (){ return _mouseButtonEvent; }
	auto& keyEvent() { return _keyEvent; }

	void initWindow();


	

	void Render();

	

};