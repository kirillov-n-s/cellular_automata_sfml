#pragma once
#include <chrono>
#include <thread>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "automaton.h"

using namespace std::literals::chrono_literals;

class application
{
	std::string _title;
	sf::RenderWindow* _window;
	automaton* _automaton;

	bool _paused = true;

	const uint32_t _cell_dim = 16;
	sf::RectangleShape _cell;
	void draw(uint32_t x, uint32_t y);

	void handle_events();
	void update();
	void render();

public:
	application(automaton* automaton, const std::string& title = "");
	~application();

	void run();
};
