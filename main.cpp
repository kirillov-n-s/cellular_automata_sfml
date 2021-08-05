#pragma once
#include "application.h"

int main()
{
	auto aut = new automaton(320, 180, automaton::terraforming);
	application app(aut, "", 10);
	app.run();
	delete aut;
}