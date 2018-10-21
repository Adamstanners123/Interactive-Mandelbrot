#include "RenderLoop.h"

RenderLoop::RenderLoop(sf::RenderWindow* hwnd, Input* dnwh)
{
	//Initialises window and input
	m_window = hwnd;
	m_input = dnwh;

	//Load font from file
	m_font.loadFromFile("../RobotoBold.ttf");

	//Initialises selection rectangle
	m_select.setFillColor(sf::Color(0, 0, 0, 0));
	m_select.setOutlineColor(sf::Color(255, 255, 255, 255));
	m_select.setOutlineThickness(-2.f);

	m_shapeColour = sf::Color(47, 79, 79, 150);

	//Strings for controls
	string infoOne = "Use left mouse to select area to zoom into";
	string infoTwo = "Press A/D to increase/decrease resolution";
	string infoThree = "Press O/L to increase/decrease colour frequency one";
	string infoFour = "Press I/K to increase/decrease colour frequency two";
	string infoFive = "Press U/J to increase/decrease colour frequency three";
	string infoSix = "Press Up/Down to increase/decrease number of threads";
	string infoSeven = "Press R to go back to the original view";
	string infoEight = "Press Q to redraw mandelbrot set";
	
	//Initialises controls text
	m_controlsText.setCharacterSize(18);
	m_controlsText.setFont(m_font);
	m_controlsText.setString(infoOne + "\n" + infoTwo + "\n" + infoThree + "\n" + infoFour + "\n" + infoFive + "\n" + infoSix + "\n" + infoSeven + "\n" + infoEight);
	m_controlsText.setPosition(10, 5);

	//Initialises controls shape
	m_controlsShape.setSize(sf::Vector2f(m_controlsText.getGlobalBounds().width + 25, m_controlsText.getGlobalBounds().height + 25));
	m_controlsShape.setFillColor(m_shapeColour);
	m_controlsShape.setPosition(-5, -5);

	//Initialises mandlebrot info text
	m_mandlebrotInfoText.setCharacterSize(18);
	m_mandlebrotInfoText.setFont(m_font);
	m_mandlebrotInfoText.setString(std::string("Rendering parameters\n \n") + "Precision level: " + m_mbrot.getResolution() + "\n" + "Fractal rendered in 1000 ms" + "\n" + m_mbrot.getColourFrequencies() + "\n" + m_mbrot.getNumberOfThreads());
	m_mandlebrotInfoText.setPosition(5, (m_window->getSize().y - m_mandlebrotInfoText.getLocalBounds().height) + 50);

	//Initialises mandlebrot info shape
	m_mandlebrotInfoShape.setSize(sf::Vector2f(m_mandlebrotInfoText.getGlobalBounds().width + 25, m_mandlebrotInfoText.getGlobalBounds().height + 25));
	m_mandlebrotInfoShape.setPosition(m_mandlebrotInfoText.getPosition().x - 5, m_mandlebrotInfoText.getPosition().y);
	m_mandlebrotInfoShape.setFillColor(m_shapeColour);

	//Initialise loading text
	m_loadingText.setFont(m_font);
	m_loadingText.setCharacterSize(30);
	m_loadingText.setString("Loading.....");
	m_loadingText.setPosition(sf::Vector2f((m_window->getSize().x / 2.0f) - (m_loadingText.getGlobalBounds().width / 2.0f), (m_window->getSize().y / 2.0f) - (m_loadingText.getGlobalBounds().height / 2.0f)));

	//Initialise loading screen shape
	m_loadingScreenShape.setSize(sf::Vector2f(m_window->getSize()));
	m_loadingScreenShape.setFillColor(m_shapeColour);

	//Displays loading message whilst set is created
	pause();
	m_mbrot.computeMandelbrot();
}

RenderLoop::~RenderLoop()
{

}

/** Updates core data*/
void RenderLoop::update()
{
	//Update mandlebrot info text
	m_mandlebrotInfoText.setString(std::string("Rendering parameters\n") +  "Resolution: " + m_mbrot.getResolution() +
											   "\n" +  "Fractal rendered in " + m_mbrot.getLastRenderingTime() + " ms" +
										       "\n" + m_mbrot.getColourFrequencies() + 
											   m_mbrot.getNumberOfThreads());
}

/** Handles user input*/
void RenderLoop::handleInput(float dt)
{
	//Draws rectangle if left mouse is down
	if (m_input->isMouseLeftDown()) {
		drawRectangle();
	}
	//Resets mandlebrot set
	else if (m_input->isKeyDown(sf::Keyboard::R)) {
		m_mbrot.resetResolution();
		m_mbrot.computeMandelbrot();
	}
	//Decreases resolution
	else if (m_input->isKeyDown(sf::Keyboard::A)) {
		m_mbrot.decreaseResolution(dt);
	}
	//Increases resolution
	else if (m_input->isKeyDown(sf::Keyboard::D)) {
		m_mbrot.increaseResolution(dt);
	}
	//Redraws mandlebrot set
	else if (m_input->isKeyDown(sf::Keyboard::Q)) {
		pause();
		m_mbrot.computeMandelbrot();
	}
	//Increases colour frequency one
	else if (m_input->isKeyDown(sf::Keyboard::O)) {
		m_mbrot.increaseColourFrequency('a', dt);
		m_mbrot.updateColourGradient();
	}
	//Decreases colour frequency one
	else if (m_input->isKeyDown(sf::Keyboard::L)) {
		m_mbrot.decreaseColourFrequency('a', dt);
		m_mbrot.updateColourGradient();
	}
	//Increase colour frequency two
	else if (m_input->isKeyDown(sf::Keyboard::I)) {
		m_mbrot.increaseColourFrequency('b', dt);
		m_mbrot.updateColourGradient();
	}
	//Decreases colour frequency two
	else if (m_input->isKeyDown(sf::Keyboard::K)) {
		m_mbrot.decreaseColourFrequency('b', dt);
		m_mbrot.updateColourGradient();
	}
	//Increases colour frequency three
	else if (m_input->isKeyDown(sf::Keyboard::U)) {
		m_mbrot.increaseColourFrequency('c', dt);
		m_mbrot.updateColourGradient();
	}
	//Decreases colour frequency three
	else if (m_input->isKeyDown(sf::Keyboard::J)) {
		m_mbrot.decreaseColourFrequency('c', dt);
		m_mbrot.updateColourGradient();
	}
	//Increases colour frequency three
	else if (m_input->isKeyDown(sf::Keyboard::Up)) {
		m_mbrot.increaseThreads(dt);
	}
	//Decreases colour frequency three
	else if (m_input->isKeyDown(sf::Keyboard::Down)) {
		m_mbrot.decreaseThreads(dt);
	}
	//Computes new set if an area has been selected
	if (m_drawMandelbrot) {
		pause();
		eraseRectangle();
		m_mbrot.computeMandelbrot();
		m_drawMandelbrot = false;
	}

}

/** Renders drawable sprites and text to screen*/
void RenderLoop::render()
{
	beginDraw();

	//Draws text and image
	m_window->setView(m_window->getDefaultView());
	m_mbrot.render(m_window);
	m_window->draw(m_select);
	m_window->draw(m_controlsShape);
	m_window->draw(m_mandlebrotInfoShape);
	m_window->draw(m_controlsText);
	m_window->draw(m_mandlebrotInfoText);

	endDraw();
}

/** Draws selection rectangle*/
void RenderLoop::drawRectangle()
{
	//Gets mouse initial and current positions
	m_mousePosOne = sf::Vector2f(m_input->getMouseX(), m_input->getMouseY());
	m_mousePosTwo = sf::Vector2f(sf::Mouse::getPosition(*m_window));

	//Draws selection rectangle
	m_select.setPosition(m_mousePosOne);
	m_select.setSize(m_mousePosTwo - m_mousePosOne);
}

/** Erases selection rectangle*/
void RenderLoop::eraseRectangle()
{
	//Erases rectangle
	m_select.setSize(sf::Vector2f(0, 0));
}

/** Calculates mandlebrot dimensions for selected area*/
void RenderLoop::scaleZoom()
{
	//Gets current dimensions
	double left = m_mbrot.getMbrotDimensions().left;
	double right = m_mbrot.getMbrotDimensions().right;
	double top = m_mbrot.getMbrotDimensions().top;
	double bottom = m_mbrot.getMbrotDimensions().bottom;

	//Calculates scaling values
	double scaleX = (right - left) / (double)VIEW_WIDTH;
	double scaleY = (bottom - top) / (double)VIEW_HEIGHT;

	//Checks that selection rectangle is a reasonable size
	if (std::abs(m_mousePosOne.x - m_mousePosTwo.x) > 15 && std::abs(m_mousePosOne.y - m_mousePosTwo.y) > 15)
	{
		//For selection from right to left
		if (m_mousePosTwo.x < m_mousePosOne.x)
		{
			float temp = m_mousePosOne.x;
			m_mousePosOne.x = m_mousePosTwo.x;
			m_mousePosTwo.x = temp;
		}
		//For selection from bottom to top
		if (m_mousePosTwo.y < m_mousePosOne.y)
		{
			float temp = m_mousePosOne.y;
			m_mousePosOne.y = m_mousePosTwo.y;
			m_mousePosTwo.y = temp;
		}

		//Calculates new mandlebrot dimensions
		right = (double)(m_mousePosTwo.x * scaleX) + left;
		bottom = (double)(m_mousePosTwo.y * scaleY) + top;
		left = (double)(m_mousePosOne.x * scaleX) + left;
		top = (double)(m_mousePosOne.y * scaleY) + top;

		//Sets draw to true
		m_drawMandelbrot = true;

		//Sets new dimensions
		m_mbrot.setMbrotDimensions(left, right, top, bottom);
	}
}

/** Draws loading screen and gives user feedback*/
void RenderLoop::pause()
{
	//Draws loading screen
    m_window->draw(m_loadingScreenShape);
	m_window->draw(m_loadingText);
    endDraw();
}

/** Clears window for drawing*/
void RenderLoop::beginDraw()
{
	m_window->clear(sf::Color(0, 0, 0));
}

/** Display text and sprites*/
void RenderLoop::endDraw()
{
	m_window->display();
}
