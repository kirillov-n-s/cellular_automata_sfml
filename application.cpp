#include "application.h"

void application::scroll_light()
{
	_light++;
	if (_light > 7)
		_light = 0;
}

void application::scroll_dark()
{
	_dark++;
	if (_dark > 5)
		_dark = 0;
}

void application::draw(uint32_t x, uint32_t y)
{
	_cell.setPosition({ (float)(x * _cell_dim), (float)(y * _cell_dim) });
	sf::Color light, dark;

	switch (_light)
	{
	case 0:
		light = { 0xff, 0xda, 0xb9 };
		break;
	case 1:
		light = { 0xbc, 0xff, 0xb9 };
		break;
	case 2:
		light = { 0x96, 0xba, 0xff };
		break;
	case 3:
		light = { 0xd9, 0xa5, 0xb3 };
		break;
	case 4:
		light = { 0xed, 0x8e, 0x7c };
		break;
	case 5:
		light = { 0xe1, 0xe8, 0xeb };
		break;
	case 6:
		light = { 0xec, 0xa3, 0xf5 };
		break;
	case 7:
		light = { 0xd7, 0x97, 0x71 };
		break;
	}

	switch (_dark)
	{
	case 0:
		dark = { 0x54, 0x43, 0x68 };
		break;
	case 1:
		dark = { 0x32, 0x52, 0x88 };
		break;
	case 2:
		dark = { 0x00, 0x36, 0x38 };
		break;
	case 3:
		dark = { 0x7b, 0x11, 0x3a };
		break;
	case 4:
		dark = { 0x4a, 0x39, 0x33 };
		break;
	case 5:
		dark = { 0x08, 0x20, 0x32 };
		break;
	}

	_cell.setFillColor(_automaton->get(x, y) ? light : dark);
	_window->draw(_cell);
}

void application::handle_events()
{
	sf::Event event;
	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			_window->close();

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Space:
				_paused ^= true;
				break;
			case sf::Keyboard::Enter:
				_automaton->populate();
				break;
			case sf::Keyboard::RShift:
				_automaton->clear();
				break;
			case sf::Keyboard::RControl:
				if (_paused)
					_automaton->operate();
				break;
			case sf::Keyboard::LControl:
				_automaton->flip_all();
				break;
			case sf::Keyboard::LAlt:
				scroll_light();
				break;
			case sf::Keyboard::RAlt:
				scroll_dark();
				break;
			case sf::Keyboard::Escape:
				_window->close();
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			auto coords = sf::Mouse::getPosition(*_window) / (int)_cell_dim;
			_automaton->flip(coords.x, coords.y);
		}
	}
}

void application::update()
{
	_automaton->operate();
}

void application::render()
{
	_window->clear(sf::Color::Black);
	for (int x = 0; x < _automaton->width(); x++)
		for (int y = 0; y < _automaton->height(); y++)
			draw(x, y);
	_window->display();
}

application::application(automaton* automaton, const std::string& title, uint32_t dimension, uint32_t framerate_limit)
	: _automaton(automaton), _title(title)
{
	_cell_dim = dimension;
	auto width = _automaton->width() * _cell_dim;
	auto height = _automaton->height() * _cell_dim;

	_window = new sf::RenderWindow(sf::VideoMode(width, height), _title);
	if (!framerate_limit)
		_window->setVerticalSyncEnabled(true);
	else
		_window->setFramerateLimit(framerate_limit);

	_cell = sf::RectangleShape({ (float)_cell_dim, (float)_cell_dim });
	_cell.setOutlineColor(sf::Color::Black);
	_cell.setOutlineThickness(-2.f);

	if (_title == "")
		switch (_automaton->type())
		{
		case automaton::life:
			_title = "Conway's Game Of Life";
			break;
		case automaton::islands:
			_title = "Islands (Formation pattern, neighbor limit: 4, 50% initial coverage)";
			break;
		case automaton::spots:
			_title = "Spots (Formation pattern, neighbor limit: 6, 80% initial coverage)";
			break;
		case automaton::insectoid:
			_title = "Insectoid";
			break;
		case automaton::terraforming:
			_title = "Terraforming";
			break;
		}
}

application::~application()
{
	delete _window;
}

void application::run()
{
	while (_window->isOpen())
	{
		std::chrono::system_clock clock;
		auto then = clock.now();

		handle_events();
		if (!_paused)
			update();
		render();

		uint64_t fps = 1000u / (std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - then)).count();
		_window->setTitle(_title + " [FPS: " + std::to_string(fps) + "]" + " [Iteration: " + std::to_string(_automaton->iteration()) + "]");
	}
}
