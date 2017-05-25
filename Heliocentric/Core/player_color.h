#pragma once

class PlayerColor {
public:
	enum Color {
		WHITE,
		RED,
		BLUE,
		GREEN,
		YELLOW,
		ORANGE
	};

	const static Color FIRST = WHITE;
	const static int NUM_COLORS = 6;
};
