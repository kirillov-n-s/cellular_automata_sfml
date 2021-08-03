#pragma once
#include <random>

class automaton
{
public:
	enum type
	{
		life = 0,
		formation = 1,
		insectoid = 2
	};

private:
	bool* _grid;
	type _type;
	int _param; //insectoid only

	uint32_t _width;
	uint32_t _height;
	uint32_t _size;

	bool get(bool* buffer, uint32_t x, uint32_t y) const;
	bool* get_neighbors(bool* buffer, uint32_t x, uint32_t y);

public:
	automaton(uint32_t width, uint32_t height, type type = life, int param = 4);
	~automaton();

	void operate();
	void populate();
	void clear();

	bool get(uint32_t x, uint32_t y) const;
	void set(uint32_t x, uint32_t y, bool value);
	void flip(uint32_t x, uint32_t y);

	uint32_t width() const;
	uint32_t height() const;
};

