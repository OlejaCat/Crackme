#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <random>
#include <sstream>
#include <iostream>

#include "patch.hpp"


class MatrixEffect 
{
	private:
		struct Symbol 
		{
			sf::Text text;
			float speed;
		};

		std::vector<Symbol> symbols;
		sf::Font font;
		std::mt19937 rng;
		std::string charSet;
		std::uniform_real_distribution<float> speedDist;

	public:
		MatrixEffect(unsigned int width, unsigned int height) 
			: rng(std::random_device()()), 
			speedDist(50.f, 200.f) {
				if (!font.loadFromFile("arial.ttf")) {
					std::cerr << "Error loading font!" << std::endl;
				}
				charSet = "01!@#$%^&*()_+-=[]{}|;:',.<>?/~`";

				std::uniform_int_distribution<int> charDist(0, charSet.size()-1);

				for (int i = 0; i < 50; ++i) {
					Symbol s;
					s.text.setFont(font);
					s.text.setString(std::string(1, charSet[charDist(rng)]));
					s.text.setCharacterSize(24);
					s.text.setFillColor(sf::Color(0, 255, 0, 150));
					s.text.setPosition(i * 30, static_cast<float>(rand() % height));
					s.speed = speedDist(rng);
					symbols.push_back(s);
				}
			}

		void update(float dt) {
			for (auto& s : symbols) {
				s.text.move(0, s.speed * dt);
				if (s.text.getPosition().y > 600) {
					s.text.setPosition(s.text.getPosition().x, -20.f);
					s.speed = speedDist(rng);
				}
			}
		}

		void draw(sf::RenderWindow& window) {
			for (auto& s : symbols) {
				window.draw(s.text);
			}
		}
};

int main() 
{
	sf::err().rdbuf(NULL);

	sf::RenderWindow window(sf::VideoMode(800, 600), "Patch");
	window.setVerticalSyncEnabled(false);
	MatrixEffect matrixEffect(800, 600);

	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) 
	 {
	 std::cerr << "Error loading font!" << std::endl;
		return EXIT_FAILURE;
	 }

	sf::Music music;
	if (!music.openFromFile("audio/idea.wav"))
	{
		std::cerr << "Error loadnig music!" << std::endl;
		return EXIT_FAILURE;	
	}
	music.setLoopPoints({sf::milliseconds(500), sf::seconds(4)});
	music.play();

	// Input field
	sf::RectangleShape inputBox(sf::Vector2f(400, 40));
	inputBox.setFillColor(sf::Color(0, 20, 0, 200));
	inputBox.setOutlineThickness(2);
	inputBox.setOutlineColor(sf::Color::Green);
	inputBox.setPosition(200, 300);

	// Crack button
	sf::RectangleShape button(sf::Vector2f(120, 40));
	button.setFillColor(sf::Color(0, 20, 0, 200));
	button.setOutlineThickness(2);
	button.setOutlineColor(sf::Color::Green);
	button.setPosition(340, 400);

	sf::Text buttonText("CRACK!", font, 24);
	buttonText.setPosition(350, 405);
	buttonText.setFillColor(sf::Color::Green);

	sf::Text inputText("", font, 24);
	inputText.setPosition(210, 305);
	inputText.setFillColor(sf::Color::Green);

	std::string inputString;
	sf::Clock cursorClock;
	bool showCursor = true;
	sf::Clock frameClock;

	while (window.isOpen()) 
	{
		float dt = frameClock.restart().asSeconds();
		sf::Event event;

		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed)
				window.close();

			// Text input handling
			if (event.type == sf::Event::TextEntered) 
			{
				if (event.text.unicode == '\b' && !inputString.empty()) 
				{
					inputString.pop_back();
				}
				else if (event.text.unicode < 255 && 
						event.text.unicode != '\r' && 
						event.text.unicode != '\t') {
					inputString += static_cast<char>(event.text.unicode);
				}
			}

			// Button click handling
			if (event.type == sf::Event::MouseButtonPressed) 
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) 
				{	
					char *cstr = inputString.data();
					patchInputFile(cstr);
				}
			}
		}

		// Update matrix effect
		matrixEffect.update(dt);

		// Cursor blinking
		if (cursorClock.getElapsedTime().asSeconds() > 0.5) {
			showCursor = !showCursor;
			cursorClock.restart();
		}

		// Update input text
		inputText.setString(inputString + (showCursor ? "_" : ""));

		// Drawing
		window.clear(sf::Color::Black);
		matrixEffect.draw(window);

		// Draw interface elements
		window.draw(inputBox);
		window.draw(button);
		window.draw(buttonText);
		window.draw(inputText);

		window.display();
	}

	return 0;
}
