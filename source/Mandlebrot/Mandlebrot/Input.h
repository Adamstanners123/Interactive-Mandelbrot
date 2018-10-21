using namespace std;

#pragma once
class Input
{

private:
	struct Mouse
	{
		float x, y;
		bool left = false;

	};

public:
	Input();
	~Input();

	void setKeyDown(int key);
	void setKeyUp(int key);
	bool isKeyDown(int key);
	void setMousePosition(int lx, int ly);
	void setMouseLeftDown(bool l);
	bool isMouseLeftDown();
	float getMouseX();
	float getMouseY();


private:

	//Array of keys
	bool keys[256]{ false };

	//Contains mouse data
	Mouse mouse;

};

