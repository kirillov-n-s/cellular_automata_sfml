#pragma once
#include "application.h"

int main()
{
	auto aut = new automaton(160, 90);
	application app(aut, "", 20u);
	app.run();
	delete aut;
}