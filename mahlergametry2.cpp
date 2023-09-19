#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <random>
#include <vector>
#include <fstream>

int generateAudioClip(std::string file_name)
{
    // Initialize random seed with the current time
    unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(static_cast<unsigned int>(seed));

    // Load the audio file (replace "mahler_symphony.wav" with your audio file)
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(file_name)) {
        std::cerr << "Error loading audio file!" << std::endl;
        return -1;
    }

    // Create a sound instance and set the buffer
    sf::Sound sound;
    sound.setBuffer(buffer);

    // Get the audio duration
    float duration = buffer.getDuration().asSeconds();

    // Calculate the random starting point (between 0 and duration - 5 seconds)
    float randomStart = static_cast<float>(std::rand()) / RAND_MAX * (duration - 5.0f);

    // Set the starting point
    sound.setPlayingOffset(sf::seconds(randomStart));

    // Play the audio for 5 seconds
    sound.play();
    sf::sleep(sf::seconds(5.0f));

    return 0;
}



void addPoints(int points)
{
    std::ifstream input("current_score.txt");

    if (input.is_open())
    {

        int best_score;
        input >> best_score;
        input.close();

        best_score += points;

        std::ofstream output("current_score.txt");
        output << best_score;
        output.close();

        std::ifstream bestinput("bestscore.txt");
        int high_score;
        bestinput >> high_score;
        bestinput.close();

        if (best_score > high_score)
        {
            high_score = best_score;
            std::ofstream bestoutput("bestscore.txt");

            bestoutput << high_score;
            bestoutput.close();
        }
        else
        {
            std::ofstream bestoutput("bestscore.txt");
            bestoutput << high_score;
            bestoutput.close();
        }



    }
  




}


void resetCurrentScore()
{
    std::ifstream inputCurrent("current_score.txt");
    std::ifstream inputBestScore("bestscore.txt");

    while (true)
    {
        if (inputCurrent.is_open() && inputBestScore.is_open())
        {
     

            int current_score;
            int high_score;

            inputCurrent >> current_score;
            inputBestScore >> high_score;
            inputCurrent.close();
            inputBestScore.close();


            std::ofstream outputCurrent("current_score.txt");
            std::ofstream outputBestScore("bestscore.txt");

            if (current_score > high_score)
            {
                outputCurrent << 0;
                outputBestScore << current_score;
                return;
            }
            else
            {
                outputCurrent << 0;
                outputBestScore << high_score;
                return;
            }
        }
    }

}

void displayCurrentScore()
{
    std::ifstream input("current_score.txt");

    if (input.is_open())
    {
        

        int best_score;
        input >> best_score;
        std::cout << best_score;

        input.close();

    
       

    }
    else
    {
        std::ofstream output("current_score.txt");
        output << 0;
        std::cout << 0;

    
    }
}

void displayHighScore()
{
    std::ifstream input("bestscore.txt");

    if (input.is_open())
    {

        int best_score;
        input >> best_score;
        std::cout << best_score;

        input.close();


    }
    else
    {
        std::ofstream output("bestscore.txt");
        output << 0;
        std::cout << 0;
    }
}





void playMahlerGame()
{
    std::vector<int> mahler_symphony_sizes = { 5,5,6,4,5,4,5,2,4,5 };


    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> symphony_choices(1, 10);
    int random_symphony = symphony_choices(mt);

    int ran_symph_size = mahler_symphony_sizes[random_symphony - 1];
    std::uniform_int_distribution<int> movement_choices(1, ran_symph_size);
    int random_movement = movement_choices(mt);


    std::string symphony_string = std::to_string(random_symphony);
    std::string movement_string = std::to_string(random_movement);

    std::string file_name = symphony_string + "." + movement_string + ".wav";

    std::cout << "Playing clip...";
    generateAudioClip(file_name);
    bool can_hint = true;


    while (true)
    {
        std::string hint_msg = can_hint == true ? "If you want hear a different section for less points, type 747.\n" : "You can no longer get a hint!\n";
        int symph_points = can_hint == true ? 5 : 2;

        std::cout << "What symphony number is this?\n" << hint_msg << std::endl;
        int symphony_guess;

        if (std::cin >> symphony_guess)
        {
            if (symphony_guess == random_symphony) //correct symph
            {
                std::cin.clear();
                std::cout << "Correct, that was Mahler Symphony No. " << random_symphony << "!\n";
                addPoints(symph_points);



                while (true)
                {
                    hint_msg = can_hint == true ? "If you want hear a different section for less points, type 747.\n" : "You can no longer get a hint!\n";
                    int move_points = can_hint == true ? 10 : 2;
                    std::cout << "What was the movement number?\n" << hint_msg << std::endl;
                    int movement_guess;

                    if (std::cin >> movement_guess)
                    {
                        if (movement_guess == random_movement) //correct move
                        {
                            std::cin.clear();
                            addPoints(move_points);

                            std::cout << "Correct again! That was Mahler Symphony No. " << random_symphony << " movement " << random_movement << "!\n";
                            std::cout << "Your current score is ";
                            displayCurrentScore();
                            std::cout << ".\nYour highest score is ";
                            displayHighScore();
                            std::cout << ".\n";

                            std::cout << "Would you like to do another (1) or go back to menu (0)?\n";
                            int selection;

                            while (true)
                            {
                                if (std::cin >> selection)
                                {
                                    switch (selection)
                                    {
                                    case 0:
                                        std::cin.clear();
                                        return;
                                    case 1:
                                        playMahlerGame();
                                        break;
                                    }
                                }
                                else
                                {
                                    std::cerr << ">:( Invalid input. Please try again and input an integer!\n";
                                    std::cin.clear();
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                }
                            }
                        }
                        else if (movement_guess == 747 && can_hint == true) //move hint
                        {
                            can_hint = false;
                            std::cout << "Playing additional clip...\n";
                            generateAudioClip(file_name);
                        }
                        else if (movement_guess != random_movement) //wrong move
                        {
                            std::cin.clear();
                            std::cout << "Incorrect! That was actually Mahler Symphony No. " << random_symphony << " movement " << random_movement << ".\n";
                            std::cout << "Your current score is ";
                            displayCurrentScore();
                            std::cout << ".\nYour highest score is ";
                            displayHighScore();
                            std::cout << ".\n";

                            std::cout << "Would you like to do another (1) or go back to menu (0)?\n";
                            int selection;

                            while (true)
                            {
                                if (std::cin >> selection)
                                {
                                    switch (selection)
                                    {
                                    case 0:
                                        return;
                                    case 1:
                                        playMahlerGame();
                                        break;
                                    }
                                }
                                else
                                {
                                    std::cerr << ">:( Invalid input. Please try again and input an integer!\n";
                                    std::cin.clear();
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                }
                            }
                        }

                    }
                    else //error
                    {
                        std::cerr << ">:( Invalid input. Please try again and input an integer!\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
            }
            else if (symphony_guess == 747 && can_hint == true) //hint
            {
                can_hint = false;
                std::cout << "Playing additional clip...\n";
                generateAudioClip(file_name);
            }
            else if (symphony_guess != random_symphony) //wrong
            {
                std::cin.clear();
                resetCurrentScore();
                std::cout << "Incorrect! That was actually Mahler Symphony No. " << random_symphony << " movement " << random_movement << ".\n";
                std::cout << "Your current score is ";
                displayCurrentScore();
                std::cout << ".\nYour highest score is ";
                displayHighScore();
                std::cout << ".\n";

                std::cout << "Would you like to do another (1) or go back to menu (0)?\n";
                int selection;

                while (true)
                {
                    if (std::cin >> selection)
                    {
                        switch (selection)
                        {
                        case 0:
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            return;
                            break;
                        case 1:
                            playMahlerGame();
                            break;
                        }
                    }
                    else
                    {
                        std::cerr << ">:( Invalid input. Please try again and input an integer!\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
            }
        }
        else //error
        {
            std::cerr << ">:( Invalid input. Please try again and input an integer!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

    



void menu()
{
    std::cout << "Initializing...\nThe Mahler Game Ver. 1.0 by Alex Huff\nPlease press enter to continue:\n\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int menuChoice;
   

    while (true)
    {
       

        std::cout << std::endl << "|Your current score is ";
        displayCurrentScore();
        std::cout << "|" << ".\n|Your highest score is ";
        displayHighScore();
        std::cout << "|.\n" << std::endl;
        std::cout << "Hello, welcome to the Mahler Guessing Game! Please input a choice:\n";
        std::cout << "0. Exit \n1. Play Game \n2. Rules\n";
      

        if (std::cin >> menuChoice)
        {
            switch (menuChoice)
            {
                case 0:
                    return;
                case 1:
                    playMahlerGame();
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                case 2:
                    std::cout << "\n\nYou will get a random 5 second section from any Mov. of any Mahler symphony.\n"
                        << "You then must answer which symphony and movement it comes from.\n" << std::endl
                        << "|\tCorrect symphony without hint: 5 points, Correct symhpony with hint: 2 points.\t\t|\n"
                        << "|\tCorrect movement without hint: 10 points, correct movement with hint: 2 points.\t\t|\n"
                        << "|\tIncorrect movement numbers do not reset your score but incorrect symphony numbers do!\t|\n"
                        << std::endl
                        << "NOTE: This app also includes the Blumine movement. To answer it, enter 5 for mov number.\n" 
                        << "      Symphony 6 Scherzo is movement 2 and Symphony 6 Andante is movement 3 :)\n" << std::endl;
                    break;
            }
        }
        else
        {
            std::cerr << ">:( Invalid input. Please input an integer!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}














int main() 
{
    menu();
}
