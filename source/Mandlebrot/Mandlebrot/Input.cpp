#include "Input.h"

Input::Input()
{
}


Input::~Input()
{
}

/** Sets key down*/
void Input::setKeyDown(int key)
{
	keys[key] = true;
}

/** Sets key up*/
void Input::setKeyUp(int key)
{
	keys[key] = false;
}

/** Checks if key is pressed*/
bool Input::isKeyDown(int key)
{
	return keys[key];
}

/** Sets mouse position*/
void Input::setMousePosition(int lx, int ly)
{
	mouse.x = lx;
	mouse.y = ly;
}

/** Sets mouse left down*/
void Input::setMouseLeftDown(bool l)
{
	mouse.left = l;

}

/** Checks if mouse left is down*/
bool Input::isMouseLeftDown()
{
	return mouse.left;
}

/** Returns mouse x position*/
float Input::getMouseX()
{
	return mouse.x;
}

/** Returns mouse y position*/
float Input::getMouseY()
{
	return mouse.y;
}





