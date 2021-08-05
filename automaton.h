#pragma once
#include <random>

class automaton
{
public:
	enum type
	{
		life = 0,
		islands = 1,
		spots = 2,
		insectoid = 3,
		terraforming = 4
	};

private:
	bool* _grid;
	type _type;
	int _param; //insectoid & terraforming only
	uint32_t _iteration = 0;

	uint32_t _width;
	uint32_t _height;
	uint32_t _size;

	bool get(bool* buffer, uint32_t x, uint32_t y) const;
	bool* get_neighbors(bool* buffer, uint32_t x, uint32_t y);
	void set(uint32_t x, uint32_t y, bool value);

public:
	automaton(uint32_t width, uint32_t height, enum type type = life, int param = 8);
	~automaton();

	void operate();
	void populate();
	void clear();

	bool get(uint32_t x, uint32_t y) const;
	void flip(uint32_t x, uint32_t y);
	void flip_all();

	uint32_t width() const;
	uint32_t height() const;
	type type() const;
	uint32_t iteration() const;
};

/*
desolation landscape generation algorithm (for param = 8):
1. 20 iters
2. flip all
3. 5 iters
4. flip all
5. 10 iters
6. flip all
7. 5 iters
8. optional flip all 
*/
