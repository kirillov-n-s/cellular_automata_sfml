#include "application.h"

void application::draw(uint32_t x, uint32_t y)
{
	_cell.setPosition({ (float)(x * _cell_dim), (float)(y * _cell_dim) });
	//auto alive = sf::Color(226, 214, 207), dead = sf::Color(72, 57, 32);
	//auto alive = sf::Color(0xd9, 0xa5, 0xb3), dead = sf::Color(0x31, 0x68, 0x79);
	//auto alive = sf::Color(0xff, 0x48, 0x48), dead = sf::Color(0x31, 0x68, 0x79);
	//auto alive = sf::Color(0xed, 0x8e, 0x7c), dead = sf::Color(0x54, 0x43, 0x6b);
	//auto alive = sf::Color(0xed, 0x8e, 0x7c), dead = sf::Color(0x00, 0x36, 0x38);
	auto alive = sf::Color(0xed, 0x8e, 0x7c), dead = sf::Color(0x31, 0x68, 0x79);
	_cell.setFillColor(_automaton->get(x, y) ? alive : dead);
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

application::application(automaton* automaton, const std::string& title)
	: _automaton(automaton), _title(title)
{
	auto width = _automaton->width() * _cell_dim;
	auto height = _automaton->height() * _cell_dim;

	_window = new sf::RenderWindow(sf::VideoMode(width, height), _title);
	//_window->setVerticalSyncEnabled(true);
	_window->setFramerateLimit(15u);

	_cell = sf::RectangleShape({ (float)_cell_dim, (float)_cell_dim });
	_cell.setOutlineColor(sf::Color::Black);
	_cell.setOutlineThickness(-2.f);
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
		_window->setTitle(_title + " [FPS: " + std::to_string(fps) + "]");
	}
}
