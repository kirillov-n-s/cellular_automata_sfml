#pragma once
#include <chrono>
#include <thread>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "automaton.h"

class application
{
	std::string _title;
	sf::RenderWindow* _window;
	automaton* _automaton;

	bool _paused = true;

	uint8_t _dark = 0;
	uint8_t _light = 0;
	void scroll_light();
	void scroll_dark();

	uint32_t _cell_dim;
	sf::RectangleShape _cell;
	void draw(uint32_t x, uint32_t y);

	void handle_events();
	void update();
	void render();

public:
	application(automaton* automaton, const std::string& title = "", uint32_t dimension = 16u, uint32_t framerate_limit = 16u);
	~application();

	void run();
};
