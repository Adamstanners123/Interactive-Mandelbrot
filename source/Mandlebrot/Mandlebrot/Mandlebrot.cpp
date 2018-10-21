#include "Mandlebrot.h"



Mandlebrot::Mandlebrot()
{
	//Initialise thread count, speed and elapsed time
	m_threadCap = std::thread::hardware_concurrency();
	m_threads = m_threadCap;
	m_elapsedTime = 0.0f;
	m_threadIncrementSpeed = 0.5f;
	m_resolutionIncrementSpeed = 0.05f;

	//Create image
	m_image.create((int)VIEW_WIDTH, (int)VIEW_HEIGHT);

	//Initialise colour frequencies
	m_frequencyOne = 0.3;
	m_frequencyTwo = 0.3;
	m_frequencyThree = 0.3;

	//Initialise omp locks
	omp_init_lock(&m_imageBlack_lock);
	omp_init_lock(&m_imageColour_lock);
	omp_init_lock(&m_mu_lock);
	
	//Initialise mu vector
	m_mu.resize(VIEW_WIDTH, vector<double>(VIEW_HEIGHT, 0));

	//Set aspect ratio
	m_aspectRatio = VIEW_WIDTH / VIEW_HEIGHT;

	//Initialise image rectangle
	m_imageSprite.setSize(sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));
	m_imageTexture.loadFromImage(m_image);
	m_imageSprite.setTexture(&m_imageTexture);
}

Mandlebrot::~Mandlebrot()
{
}

/** Computes mandlebrot set*/
void Mandlebrot::computeMandelbrot()
{
	//Local variables for timing and pixels
	sf::Clock timer;
	double pixelWidth, pixelHeight;

	//Apply aspect ratio to selected area
	maintainAspectRatio();

	//Calculate width and height of pixels
	pixelWidth = (m_coords.right - m_coords.left) / (double)VIEW_WIDTH;
	pixelHeight = (m_coords.bottom - m_coords.top) / (double)VIEW_HEIGHT;

	
#pragma omp parallel for schedule(dynamic) num_threads(m_threads)
	for (int x = 0; x < VIEW_WIDTH; ++x)
	{
		for (int y = 0; y < VIEW_HEIGHT; ++y)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(m_coords.left + ((x + 0.5f) * pixelWidth), m_coords.top + ((y + 0.5f) * pixelHeight));

			int iterations = 0;

			complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			while (abs(z) < 2.0 && iterations < m_max_iterations)
			{
				z = (z * z) + c;

				++iterations;
			}
			if (iterations == m_max_iterations)
			{
				//Updates mu vector and image
				m_mu[x][y] = iterations;
				m_image.setPixel(x, y, sf::Color::Black);
			}
			// z escaped within less than MAX_ITERATIONS
			// iterations. This point isn't in the set.
			else
			{
				//Updates mu vector
				m_mu[x][y] = iterations - (std::log(2) / std::log(std::abs(z)));

				//Updates image
				m_image.setPixel(x, y, colourGradient(m_mu[x][y]));
			}
		}
	}

	//Gets rendering time
	m_time = timer.getElapsedTime();
	m_imageTexture.update(m_image);
	m_imageSprite.setTexture(&m_imageTexture);
}

/** Takes in mu factor and calculates colour using sine waves*/
sf::Color Mandlebrot::colourGradient(double mu)
{
	sf::Color colourRgb;

	//Uses sine waves to calculate rgb value
	colourRgb.r = (sin(m_frequencyOne * mu + 0) * 127 + 128);
	colourRgb.b = (sin(m_frequencyTwo * mu + 2) * 127 + 128);
	colourRgb.g = (sin(m_frequencyThree * mu + 4) * 127 + 128);

	return colourRgb;
}

/** Updates colours based on new frequencies*/
void Mandlebrot::updateColourGradient()
{
#pragma omp parallel for num_threads(6)		
	for (int x = 0; x < VIEW_WIDTH; ++x)
	{
		for (int y = 0; y < VIEW_HEIGHT; ++y)
		{
			//Checks if pixel is black 
			if (m_image.getPixel(x, y) != sf::Color::Black)
			{
				//Updates image with new colouring
				m_image.setPixel(x, y, colourGradient(m_mu[x][y]));
			}
			else
			{
			//Colours pixel black
			m_image.setPixel(x, y, sf::Color::Black);
			}
		}
	}
	//Loads new image to our display rectangle
	m_imageTexture.update(m_image);
	m_imageSprite.setTexture(&m_imageTexture);
}

/** Adjusts selected area to fit aspect ratio*/
void Mandlebrot::maintainAspectRatio()
{
	//Adjusts the selected area to fit aspect ratio to 
	//avoid distortion of new image
	if ((m_coords.right - m_coords.left) < (m_coords.bottom - m_coords.top) * m_aspectRatio)
	{
		m_coords.left = (m_coords.right + m_coords.left - (m_coords.bottom - m_coords.top) * m_aspectRatio) / 2.0;
		m_coords.right = m_coords.left + (m_coords.bottom - m_coords.top) * m_aspectRatio;
	}
	else
	{
		m_coords.top = (m_coords.bottom + m_coords.top - (m_coords.right - m_coords.left) / m_aspectRatio) / 2.0;
		m_coords.bottom = m_coords.top + (m_coords.right - m_coords.left) /m_aspectRatio;
	}
}

/** Renders mandlebrot image*/
void Mandlebrot::render(sf::RenderWindow * hwnd)
{
	//Draws image
	hwnd->draw(m_imageSprite);
}

/** Resets reolution and dimensions*/
void Mandlebrot::resetResolution()
{
	//Resets resolution, dimensions and colour frquencies
	m_max_iterations = 500;

	m_coords.left = -2.0;
	m_coords.right = 0.5;
	m_coords.top = -1.15;
	m_coords.bottom = 1.15;

	m_frequencyOne = 0.3;
	m_frequencyTwo = 0.3;
	m_frequencyThree = 0.3;
}

/** Increases resolution*/
void Mandlebrot::increaseResolution(float dt)
{
	//Increment time  
	m_elapsedTime += dt;

	//If enough time has passed then 
	if (m_elapsedTime >= m_resolutionIncrementSpeed)
	{

		//Increase resolution 
		if (m_max_iterations < iterationsCap) {

			m_max_iterations += 1;

		}

		m_elapsedTime = 0.0f;
	}
}

/** Decreases resolution*/
void Mandlebrot::decreaseResolution(float dt)
{
	//Increment time  
	m_elapsedTime += dt;

	//If enough time has passed then
	if (m_elapsedTime >= m_resolutionIncrementSpeed)
	{

		//Decrease resolution
		if (m_max_iterations > 0) {

			m_max_iterations -= 1;

		}

		m_elapsedTime = 0.0f;
	}
}

/** Increases colour frequency*/
void Mandlebrot::increaseColourFrequency(char key, float dt)
{
	//Checks key and updates relevant colour frequency
	switch (key)
	{
	case 'a':
		m_frequencyOne += 0.1 * dt;
		break;
	case 'b':
		m_frequencyTwo += 0.1 * dt;
		break;
	case 'c':
		m_frequencyThree += 0.1 * dt;
		break;
	default:
		break;
	}
}

/** Decreases colour frequency*/
void Mandlebrot::decreaseColourFrequency(char key, float dt)
{
	//Checks key and updates relevant colour frequency
	switch (key)
	{
	case 'a':
		m_frequencyOne -= 0.1 * dt;
		break;
	case 'b':
		m_frequencyTwo -= 0.1 * dt;
		break;
	case 'c':
		m_frequencyThree -= 0.1 * dt;
		break;
	default:
		break;
	}
}

void Mandlebrot::increaseThreads(float dt)
{
	//Increment time  
	m_elapsedTime += dt;

	//If enough time has passed then 
	if (m_elapsedTime >= m_threadIncrementSpeed)
	{
		//Increase number of threads
		if (m_threads < m_threadCap)
		{
			m_threads += 1;
		}

		m_elapsedTime = 0.0f;
	}
}

void Mandlebrot::decreaseThreads(float dt)
{
	//Increment time  
	m_elapsedTime += dt;

	//If enough time has passed then 
	if (m_elapsedTime >= m_threadIncrementSpeed)
	{
		//Decrease number of threads
		if (m_threads > 1)
		{
			m_threads -= 1;
		}

		m_elapsedTime = 0.0f;
	}
}

/** Returns current resolution for display*/
string Mandlebrot::getResolution()
{
	//Returns resolution for performance text
	std::stringstream ss;
	ss.precision(3);
	ss << m_max_iterations;
	string str = ss.str();
	return ss.str();
}

/** Returns rendering time for display*/
string Mandlebrot::getLastRenderingTime()
{
	//Returns rendering time for performances text
	std::stringstream ss;
	ss << m_time.asMilliseconds();
	string str = ss.str();
	return ss.str();
}

/** Returns current colour frequencies for display*/
string Mandlebrot::getColourFrequencies()
{
	//Returns colour frequencies for performance text
	std::stringstream ss;
	ss.precision(3);
	ss << "Colour frequency one: " << m_frequencyOne << "\n" << "Colour frequency two: " << m_frequencyTwo << "\n" << "Colour frequency three: " << m_frequencyThree << "\n";
	string str = ss.str();
	return ss.str();
}

string Mandlebrot::getNumberOfThreads()
{
	//Returns colour frequencies for performance text
	std::stringstream ss;
	ss.precision(3);
	ss << "Number of threads: " << m_threads << "\n";
	string str = ss.str();
	return ss.str();
}








