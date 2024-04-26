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

void Grid::update_row_colors(const std::vector<std::string>& letter_responses) {
	for (int i = 0; i < letter_responses.size(); i++) {
		char status = letter_responses[i][0];
		char symbol = letter_responses[i][1];
		switch (status) {
		case 'C':
			letter_colors[current_row][i] = green_color;
			break;
		case 'M':
			letter_colors[current_row][i] = yellow_color;
			break;
		case 'I':
			letter_colors[current_row][i] = sf::Color::Red;
			break;
		default:
			letter_colors[current_row][i] = bgcolor;
			break;
		}
	}
	current_row++; // Förbered för nästa rad
}

void Grid::game_over_screen(sf::RenderWindow& window, char result, std::string correct_word) {
	sf::RectangleShape popup;
	popup.setSize(sf::Vector2f(590, 300));
	popup.setPosition(80, 300);
	popup.setOutlineThickness(5);

	sf::Font font;
	font.loadFromFile("Gemsbuck 02 Bold.ttf");

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(36);
	text.setPosition(115, 375);
	//resultat T --> vinst 
	//resultat F --> förlust
	//resultat X --> spelet pågår fortfarande/inte avgjort
	if (result == 'T') {
		popup.setOutlineColor(sf::Color::Yellow);
		popup.setFillColor(sf::Color(70, 166, 89, 200));
		text.setString("You won!\n Press esc to close ");

	}
	else if (result == 'F') {
		popup.setOutlineColor(sf::Color::Yellow);
		popup.setFillColor(sf::Color::Red);
		text.setString("You Lost!\nThe correct word was " + correct_word + "\nPress esc to close");

	}
	else if (result == 'X') {
		popup.setOutlineColor(sf::Color::Transparent);
		popup.setFillColor(sf::Color::Transparent);
	}
	window.draw(popup);
	window.draw(text);
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
	
	// Omvandla global musposition till lokal i förhållande till fönstret.
	sf::Vector2i localMousePos = sf::Mouse::getPosition(window);
	if (back_button_rect.contains(localMousePos)) {
		return 'B'; // Musen klickade inom "Tillbaka"-knappens område.
	}

	else {
		return '\0'; // Ingen träff på "Tillbaka"-knappen.
	}

}

void Keyboard::update_key_colors(const std::vector<std::string>& letter_responses){
	for (auto& items : letter_responses) {
		char status = items[0];
		char symbol = items[1];
		int index = symbol - 'A';

		if (status == 'C') {
			key_colors[index] = green_color;
		}
		else if (status == 'M' && key_colors[index] != green_color) {
			key_colors[index] = yellow_color;
		}
		else if (status == 'I' && key_colors[index] != green_color && key_colors[index] != yellow_color) {
			key_colors[index] = sf::Color::Red;
		}
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

void Grid::draw_base(sf::RenderWindow& window, std::vector<std::string> letter_responses) {
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

//HÄR ÄR DEN GAMLA KEYBOARD
Keyboard::Keyboard() {
	this->rows = 3;
	this->columns = 10;
	this->box_size_x = 65;
	this->box_size_y = 85;
	this->padding = 2;
	this->gridcolor = sf::Color::White;
	this->bgcolor = sf::Color::Black;
	this->lettercolor = sf::Color::White;
	key_colors.resize(26, bgcolor);
}


void Keyboard::draw_keyboard(sf::RenderWindow& window){
	// Hämtar storleken på skärmen
	window_size = window.getSize();
	calc_grid_pos();

	// används för bokstäverna på tangentbordet
	sf::Font font;
	font.loadFromFile("Gemsbuck 02 Bold.ttf");
	std::string keyRows[3] = { "QWERTYUIOP", "ASDFGHJKL ", " ZXCVBNM  " };


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
			
			//button.setFillColor(bgcolor);
			button.setOutlineColor(gridcolor);
			button.setOutlineThickness(1);

			char currentLetter = keyRows[i][j];
			if (currentLetter >= 'A' && currentLetter <= 'Z') {
				int index = currentLetter - 'A';
				button.setFillColor(key_colors[index]);
			}
			else {
				button.setFillColor(bgcolor);
			}
			// skriver ut knapparna
			window.draw(button);

			sf::Text text;
			if (i == 2 && j == 0) text.setString("OK");
			else if (i == 2 && j == 9) text.setString(">");
			else text.setString(std::string(1, keyRows[i][j]));

			text.setFont(font);
			text.setCharacterSize(24);
			text.setFillColor(lettercolor);
			sf::FloatRect textRect = text.getLocalBounds();
			text.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
			text.setPosition(posX + box_size_x / 2.0f, posY + box_size_y / 2.0f);
			window.draw(text);


		}
	}
}
