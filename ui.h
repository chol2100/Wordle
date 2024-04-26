#ifndef UI_H
#define UI_H
#include <SFML/Graphics.hpp>
#include "game.h"
// Innehåller funktioner för att hantera användargränssnittselement, 

class Grid {
	friend class Utilities;
protected:
	// basen för antalet ruton och dess storlek
	int rows;
	int columns;
	double box_size_x;
	double box_size_y;
	double padding;

	// Färg och form på rutorna
	sf::Color gridcolor;
	sf::Color bgcolor;
	sf::Color yellow_color = sf::Color{ 249, 174, 0 };
	sf::Color green_color = sf::Color{ 70, 166, 89 };
	sf::Vector2u window_size;

	// Positionering av rutorna
	double extra_space_x;
	double extra_space_y;
	double grid_width;
	double grid_height;

	// basen för att få bokstäverna
	sf::Color lettercolor;
	std::vector<std::vector<char>>letters;
	std::vector<std::vector<sf::Color>> letter_colors;

	int count_letters = 0;
	int current_row = 0;


public:
	Grid();
	void calc_grid_pos();
	void add_letters(char letter);
	void remove_letters();
	void draw_base(sf::RenderWindow& window, std::vector<std::string> letter_responses);
	void reset_count_letters();
	void update_row_colors(const std::vector<std::string>& letter_responses);
	void game_over_screen(sf::RenderWindow& window, char result, std::string correct_word);
};

class Keyboard :public Grid {
private:
	std::vector<sf::Color> key_colors;
public:
	Keyboard();
	void draw_keyboard(sf::RenderWindow& window);
	char mouse_click(sf::Vector2i mouse_pos, sf::RenderWindow& window);
	void update_key_colors(const std::vector<std::string>& letter_responses);

};


#endif
