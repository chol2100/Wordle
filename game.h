#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include "ui.h"
//Spellogik
// Innehåller funktioner för att hantera spellogiken, inklusive att slumpa det hemliga ordet, hantera användarens gissningar 
// och avgöra om spelet är vunnet eller förlorat.
// Denna modul kan också innehålla datastrukturer för att hålla det hemliga ordet och användarens gissningar.


class Game  {
	friend class Grid;
private:
	std::string correct_word = "";
	std::string user_input = "";
	bool valid_input = false;
	bool game_continue = true;
	int correct_letters_in_word = 0;
	int current_row = 0;
	char letter_response;
	std::vector<char> letter_responses;

	//Här kommer orden lagras
	std::vector<std::string>guessable_words;
	std::vector<std::string>possible_words;


	void load_word(const std::string& file_path, std::vector<std::string>& word_vector);
	std::string pick_secret_word(std::vector<std::string>& words);
	void make_guess();

public:
	Game();
	const std::vector<char>& get_letter_responses();
	void run();

};


#endif
