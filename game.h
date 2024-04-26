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



class Game {
	friend class Grid;
private:
	std::string correct_word = "";
	std::string user_input = "";
	std::string save_guesses = "";
	bool valid_input = false;
	char word_guessed = 'X';
	int correct_letters_in_word = 0;
	int guesses_made = 0;
	char letter_response;
	std::vector<std::string> letter_responses;

	//Här kommer orden lagras
	std::vector<std::string>guessable_words;
	std::vector<std::string>possible_words;


	void load_word(const std::string& file_path, std::vector<std::string>& word_vector);
	std::string pick_secret_word(std::vector<std::string>& words);
	void make_guess();

public:
	Game();
	const std::vector<std::string>& get_letter_responses();
	void run();


};


#endif
