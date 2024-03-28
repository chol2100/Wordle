#include "game.h"
#include "ui.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>

// Konstruktören för Game klassen
Game::Game() {
    load_word("guessable_words.txt", guessable_words);
    load_word("solution_words.txt", possible_words);
    correct_word = pick_secret_word(possible_words);
    std::transform(correct_word.begin(), correct_word.end(), correct_word.begin(), ::toupper);
}

// Laddar ord från fil
void Game::load_word(const std::string& file_path, std::vector<std::string>& word_vector) {
    std::ifstream word_file(file_path);
    std::string word;
    while (std::getline(word_file, word)) {
        word_vector.push_back(word);
    }
}

// Väljer ett slumpmässigt ord som det "hemliga ordet"
std::string Game::pick_secret_word(std::vector<std::string>& words) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, words.size() - 1);
    return words[distr(gen)];
}

// Hanterar användarens gissning
void Game::make_guess() {
    letter_responses.clear();

    //om user_input är i guessable- ord
    if (std::find(guessable_words.begin(), guessable_words.end(), user_input) != guessable_words.end() && user_input.length()==5) {
        valid_input = true;
        for (int i = 0; i < user_input.length(); i++) {
            if (user_input[i] == correct_word[i]) {
                std::cout << "Letter correctly placed \n";
                letter_responses.push_back('C'); // Correct
                correct_letters_in_word++;
            }
            else if (correct_word.find(user_input[i]) != std::string::npos) {
                std::cout << "Correct letter but misplaced\n";
                letter_responses.push_back('M'); // Misplaced
            }
            else {
                std::cout << "No such letter in word \n";
                letter_responses.push_back('I'); // Incorrect
            }
          
        }
        if (correct_letters_in_word == 5) {
            game_continue = false;
            
        }
        else {
            game_continue = true;
            correct_letters_in_word = 0;
        }
        user_input = ""; // Återställ gissningen för nästa försök
    }
    
    else {
        std::cout << user_input << " is not a word";
        valid_input = false;
    }

    std::cout << "Your guess was: " << user_input << "\n";
    
}

// Returnerar de senaste gissningsresultaten
const std::vector<char>& Game::get_letter_responses() {
    return letter_responses;
}

// Huvudloopen för spelet
void Game::run() {
    std::cout << "The secret word is: " << correct_word << "\n";
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Wordle-like Game", sf::Style::Fullscreen);
    Grid new_grid;
    Keyboard new_keyboard;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
                    char letter = 'A' + (event.key.code - sf::Keyboard::A);
                    user_input += letter;
                    new_grid.add_letters(letter);
                }
                else if (event.key.code == sf::Keyboard::BackSpace && !user_input.empty()) {
                    user_input.pop_back();
                    new_grid.remove_letters();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (game_continue) {
                        make_guess();
                        if (valid_input) {
                            new_grid.update_row_colors(get_letter_responses()); // Uppdatera färgerna för raden baserat på gissningen
                            new_grid.reset_count_letters(); // Förbered för nästa rad
                        }
                    }
                    else {
                        // Spelet är slut - pop upp med YAY du vann eller Segt better luck next time

                    }
                    
                }
            }
            else if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i localMousePos = sf::Mouse::getPosition(window);
                char action = new_keyboard.mouse_click(localMousePos, window);
                if (action == 'B' && !user_input.empty()) {
                    user_input.pop_back();
                    new_grid.remove_letters(); // Anropar remove_letters om "Tillbaka"-knappen klickades
                }
            }
        }
        window.clear(sf::Color::Black);
        new_grid.draw_base(window, get_letter_responses()); // Rita griden med den senaste gissningen
        new_keyboard.draw_keyboard(window); // Rita tangentbordet
        window.display();
    }
}
