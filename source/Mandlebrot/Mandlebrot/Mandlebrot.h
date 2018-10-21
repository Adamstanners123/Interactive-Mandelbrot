#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <complex>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <omp.h>

// Import things we need from the standard library
using std::complex;
using std::vector;
using std::string;

class Mandlebrot
{

private: 

	struct Dimensions {

		double left = -2.0;
		double right = 0.5;
		double top = -1.15;
		double bottom = 1.15;

	};

	//Holds the image dimensions
	Dimensions m_coords;

public:
	Mandlebrot();
	~Mandlebrot();

	void computeMandelbrot();
	sf::Color colourGradient(double mu);
	void updateColourGradient();
	void maintainAspectRatio();
	void render(sf::RenderWindow* hwnd);
	void resetResolution();
	void increaseResolution(float dt);
	void decreaseResolution(float dt);
	void increaseColourFrequency(char key, float dt);
	void decreaseColourFrequency(char key, float dt);
	void increaseThreads(float dt);
	void decreaseThreads(float dt);
	string getResolution();
	string getLastRenderingTime();
	string getColourFrequencies();
	string getNumberOfThreads();

	Dimensions getMbrotDimensions() { return m_coords; };
	void setMbrotDimensions(double left, double right, double top, double bottom) { m_coords.left = left;
																					m_coords.right = right;
																					m_coords.top = top;
																					m_coords.bottom = bottom;
																				  };
private:

	//Window for drawing
	sf::RenderWindow* m_window;

	//Max iterations(resolution)
	int m_max_iterations = 500;

	//For maintaining aspect ration of selected areas
	double m_aspectRatio;

	//Arbitrary values that control the image colouring
	float m_frequencyOne, m_frequencyTwo, m_frequencyThree;

	//For colour calculations
	vector< vector<double> > m_mu;
	
	//For getting rendering time
	sf::Time m_time;

	//The image data
	sf::Image m_image;
	sf::Texture m_imageTexture;
	sf::RectangleShape m_imageSprite;

	//Omp lock variables
	omp_lock_t m_imageBlack_lock, m_imageColour_lock, m_mu_lock;

	//Thread count
	int m_threads;
	int m_threadCap;

	//For increasing/decreasing threads and resolution
	float m_elapsedTime;
	float m_threadIncrementSpeed;
	float m_resolutionIncrementSpeed;

	int tempo;
};

