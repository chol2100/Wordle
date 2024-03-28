#include "ui.h"
#include <iostream>
#include "game.h"

Grid::Grid() {
	this->rows = 6;
	this->columns = 5;
	this->box_size_x = 110;
	this->box_size_y = 110;
	this->padding = 2;
	this->gridcolor = sf::Color::White;
	this->bgcolor = sf::Color::Black;
	this->lettercolor = sf::Color::White;
	this->grid_width = 0.0; // Tillfälligt värde, ändras efter specifika objekt. Anger bredden på alla rutor tsm
	this->grid_height = 0.0; // Tillfälligt värde, ändras efter specifika objekt
	this->extra_space_x = 0.0;
	this->extra_space_y = 0.0;

	letters.resize(rows, std::vector<char>(columns, ' '));
	letter_colors.resize(rows, std::vector<sf::Color>(columns, bgcolor));
	
}

void Grid::reset_count_letters() {
	count_letters = 0;
	//current_row++; // ökar varje gång användaren lägger gilltigt ord och trycker OK
}

void Grid::update_row_colors(const std::vector<char>& letter_responses) {
	for (size_t j = 0; j < letter_responses.size(); ++j) {
		switch (letter_responses[j]) {
		case 'C':
			letter_colors[current_row][j] = green_color;
			break;
		case 'M':
			letter_colors[current_row][j] = yellow_color;
			break;
		case 'I':
			letter_colors[current_row][j] = sf::Color::Red;
			break;
		default:
			letter_colors[current_row][j] = bgcolor;
			break;
		}
	}
	current_row++; // Förbered för nästa rad
}

void Grid::add_letters(char letter) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			// Hittar första lediga platsen och placerar bokstaven
			if (letters[i][j] == ' ' && count_letters <= 4) {
				letters[i][j] = letter;
				count_letters++;
				return; // Avslutar funktionen efter att ha placerat bokstaven
			}
		}
	}
}


void Grid::remove_letters() {

	// Börja på sista rutan
	for (int i = (rows - 1); i >= 0; i--) {
		for (int j = (columns - 1); j >= 0; j--) {
			if (letters[i][j] != ' ' && i == current_row) {
				letters[i][j] = ' ';
				count_letters--;
				return;
			}
		}
	}
}

char Keyboard::mouse_click(sf::Vector2i mouse_pos, sf::RenderWindow& window) {

	double backBtnPosX = extra_space_x + 9 * (box_size_x + padding); // För "Tillbaka"-knappens X-position.
	double backBtnPosY = extra_space_y * 1.9 + 2 * (box_size_y + padding); // För "Tillbaka"-knappens Y-position, justera om nödvändigt.

	sf::Rect<int> back_button_rect(backBtnPosX, backBtnPosY, box_size_x, box_size_y);
	//sf::Rect<int>ok_button_rect()
	// Omvandla global musposition till lokal i förhållande till fönstret.
	sf::Vector2i localMousePos = sf::Mouse::getPosition(window);

	if (back_button_rect.contains(localMousePos)) {
		return 'B'; // Musen klickade inom "Tillbaka"-knappens område.
	}

	else {
		return '\0'; // Ingen träff på "Tillbaka"-knappen.
	}

}



void Grid::calc_grid_pos() {
	double window_width = window_size.x;
	double window_height = window_size.y;

	grid_width = columns * (box_size_x + padding);
	grid_height = rows * (box_size_y + padding);
	// Allt hamnar i mitten mitten
	extra_space_x = (window_width - grid_width) / 2;
	extra_space_y = (window_height - grid_height) / 2;
}

void Grid::draw_base(sf::RenderWindow& window, std::vector<char> letter_responses) {
	// Hämtar storleken på skärmen
	window_size = window.getSize();
	// Räkna ut positioneringen för rutnätet
	calc_grid_pos();

	sf::Font font;
	font.loadFromFile("Gemsbuck 02 Bold.ttf");

	double adjusted_extra_space_y = extra_space_y * 0.25;
	for (int i = 0; i < rows; i++) { // en rad är ett ord, en kolumn är en bokstav
		for (int j = 0; j < columns; j++) {
			// bestämmer den horisontella placeringen
			double posX = extra_space_x + j * (box_size_x + padding);
			// bestämmer den vertikala placeringen
			double posY = adjusted_extra_space_y + i * (box_size_y + padding);

			sf::RectangleShape square(sf::Vector2f(box_size_x, box_size_y));
			square.setPosition(posX, posY);
			
			square.setFillColor(letter_colors[i][j]);			
			square.setOutlineColor(gridcolor);
			square.setOutlineThickness(1);

			//Skriver ut rutnät utan knappar
			window.draw(square);

			// Förbered och visa text
			sf::Text text;
			text.setFont(font);  // Se till att denna font är korrekt inställd och laddad
			text.setString(std::string(1, letters[i][j]));  // Skapa en sträng från char
			text.setCharacterSize(24);
			text.setFillColor(lettercolor);

			// Centrera texten inom rutan
			sf::FloatRect textRect = text.getLocalBounds();
			text.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
			text.setPosition(posX + box_size_x / 2.0f, posY + box_size_y / 2.0f);

			window.draw(text);
		}
	}
}

Keyboard::Keyboard() {
	this->rows = 3;
	this->columns = 10;
	this->box_size_x = 65;
	this->box_size_y = 85;
	this->padding = 2;
	this->gridcolor = sf::Color::White;
	this->bgcolor = sf::Color::Black;
	this->lettercolor = sf::Color::White;
}

void Keyboard::draw_keyboard(sf::RenderWindow& window) {
	// Hämtar storleken på skärmen
	window_size = window.getSize();
	calc_grid_pos();

	// används för bokstäverna på tangentbordet
	sf::Font font;
	font.loadFromFile("Gemsbuck 02 Bold.ttf");

	std::string top_keys = "QWERTYUIOP";
	std::string middle_keys = "ASDFGHJKL ";
	std::string bottom_keys = "  ZXCVBNM  ";

	//Skriver ut knapparna
	double adjusted_extra_space_y = extra_space_y * 1.9;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			// bestämmer den horisontella placeringen
			double posX = extra_space_x + j * (box_size_x + padding);
			// bestämmer den vertikala placeringen
			double posY = adjusted_extra_space_y + i * (box_size_y + padding);

			sf::RectangleShape button(sf::Vector2f(box_size_x, box_size_y));
			button.setPosition(posX, posY);
			
			button.setFillColor(bgcolor);
			button.setOutlineColor(gridcolor);
			button.setOutlineThickness(1);

			// skriver ut knapparna
			window.draw(button);

			// Lägger till bokstäver som ska på knapparna
			sf::Text text;
			text.setFont(font);
			text.setCharacterSize(24);
			text.setFillColor(lettercolor);

			//Knapparna längst upp
			if (i == 0) {
				text.setString(top_keys[j]);
			}

			// Knapaprna i mitten
			else if (i == 1) {
				text.setString(middle_keys[j]);
			}

			//Knapparna längst ner
			else if (i == 2) {
				if (j == 0) {
					text.setString("OK");
				}
				else if (j == 9) {
					text.setString("<");
				}
				else {
					text.setString(bottom_keys[j]);
				}

			}
			sf::FloatRect textRect = text.getLocalBounds();
			text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			text.setPosition(sf::Vector2f(posX + box_size_x / 2.0f, posY + box_size_y / 2.0f));
			window.draw(text); // skriver ut bokstäverna på knapparna

		}
	}
}
