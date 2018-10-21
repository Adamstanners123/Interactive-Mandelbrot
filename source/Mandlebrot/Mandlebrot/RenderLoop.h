#pragma once
#include "Input.h"
#include "Mandlebrot.h"

class RenderLoop {

public: 
	RenderLoop(sf::RenderWindow* hwnd, Input* dnwh);
	~RenderLoop();

	void handleInput(float dt);
	void update();
	void render();
	void drawRectangle();
	void eraseRectangle();
	void scaleZoom();
	void pause();

private:
	void beginDraw();
	void endDraw();

	//Input and window
	sf::RenderWindow* m_window;
	Input* m_input;

	//Mandlebrot isntance
	Mandlebrot m_mbrot;

	//Performance and controls text
	sf::Font m_font;
	sf::Text m_controlsText;
	sf::Text m_mandlebrotInfoText;
	sf::Text m_loadingText;
	sf::RectangleShape m_controlsShape;
	sf::RectangleShape m_mandlebrotInfoShape;
	sf::RectangleShape m_loadingScreenShape;
	sf::Color m_shapeColour;

	//For drawing new set
	double m_mouseX, m_mouseY;
	bool m_drawMandelbrot = false;

	//Selection rectangle and mouse variables
	sf::RectangleShape m_select;
	sf::Vector2f m_mousePosOne, m_mousePosTwo;

};

