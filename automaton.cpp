#include "automaton.h"

bool automaton::get(bool* buffer, uint32_t x, uint32_t y) const
{
	return buffer[(y * _width + x) % _size];
}

bool* automaton::get_neighbors(bool* buffer, uint32_t x, uint32_t y)
{
	bool* res = new bool[8];

	res[0] = get(buffer, x - 1, y - 1);
	res[1] = get(buffer, x + 0, y - 1);
	res[2] = get(buffer, x + 1, y - 1);

	res[3] = get(buffer, x - 1, y + 0);
	res[4] = get(buffer, x + 1, y + 0);

	res[5] = get(buffer, x - 1, y + 1);
	res[6] = get(buffer, x + 0, y + 1);
	res[7] = get(buffer, x + 1, y + 1);

	return res;
}

automaton::automaton(uint32_t width, uint32_t height, enum type type, int param)
	: _width(width), _height(height), _size(width * height), _type(type), _param(param)
{
	_grid = new bool[_size];
	std::memset(_grid, 0, sizeof(bool) * _size);
}

automaton::~automaton()
{
	delete[] _grid;
}

void automaton::operate()
{
	bool* buffer = new bool[_size];
	for (int i = 0; i < _size; i++)
		buffer[i] = _grid[i];
	for (int x = 0; x < _width; x++)
	{
		for (int y = 0; y < _height; y++)
		{
			auto neighbors = get_neighbors(buffer, x, y);
			int count = 0;
			for (int i = 0; i < 8; i++)
				count += neighbors[i];
			bool state;
			switch (_type)
			{
			case life:
				state = get(buffer, x, y) ? count == 2 || count == 3 : count == 3;
				break;
			case continents:
				state = count <= 4;
				break;
			case islands:
				state = count <= 6;
				break;
			case insectoid:
				state = count > _param || count < 2;
				break;
			case desolation:
				state = count > 5;
				break;
			}
			set(x, y, state);
			delete[] neighbors;
		}
	}
	delete[] buffer;
	_iteration++;
}

void automaton::populate()
{
	std::random_device dev;
	std::mt19937 eng(dev());

	int a = 0;
	int b = 1;
	switch (_type)
	{
	case automaton::islands:
		b = 4;
		break;
	case automaton::desolation:
		b = _param;
		break;
	}

	std::uniform_int_distribution<int> distrib(a, b);
	for (int y = 0; y < _height; y++)
		for (int x = 0; x < _width; x++)
			set(x, y, (bool)std::max(distrib(eng), 0));
}

void automaton::clear()
{
	for (int x = 0; x < _width; x++)
		for (int y = 0; y < _height; y++)
			set(x, y, false );
}

bool automaton::get(uint32_t x, uint32_t y) const
{
	return _grid[(y * _width + x) % _size];
}

void automaton::set(uint32_t x, uint32_t y, bool value)
{
	_grid[(y * _width + x) % _size] = value;
}

void automaton::flip(uint32_t x, uint32_t y)
{
	_grid[(y * _width + x) % _size] ^= true;
}

uint32_t automaton::width() const
{
	return _width;
}

uint32_t automaton::height() const
{
	return _height;
}

enum automaton::type automaton::type() const
{
	return _type;
}

uint32_t automaton::iteration() const
{
	return _iteration;
}
