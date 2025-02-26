//
// Created by Mario on 07/06/2023.
//

#include "IO.h"

void animateText(const std::string& text, int delayMs)
{

    char prev = ' ';

    for (unsigned int i = 0; i < text.length(); i++)
    {
        char character = text[i];

        int realDelayMs = delayMs;
        if( character == ' ' )
            realDelayMs *= 3;
        else if( character == '.' && prev != '.' )
            realDelayMs *= 27;
        else if( character == '\n' && prev != '.' )
            realDelayMs *= 37;

        // add newline if suitable
        bool addedNewline = false;

        if(     (prev == '.' || prev == '!' || prev == '?')
            &&  (character != '\n' && character != '.' && character != '?' && character != '!') )
        {
            addedNewline = true;
            std::cout << std::endl << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(37));
        }

        if(!(addedNewline && character == ' ')){
            std::cout << character << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(realDelayMs));
        }

        prev = character;
    }

    // add a final new space if suitable
    if(     (prev == '.' || prev == '!' || prev == '?') )
        std::cout << std::endl << std::flush;
}

void readSingleNumberInput(unsigned int& variableForInput, unsigned int min, unsigned int max,
                           const std::string& wrongInputString, int printSpeed)
{
    using std::cin;

    while (true)
    {
        cin >> variableForInput;

        if (cin.fail() || variableForInput < min || variableForInput > max)
        {
            animateText("\n" + wrongInputString + "\n\n", printSpeed);
            cin.clear();
            cin.ignore(MAX_BUFFER_SIZE, '\n');
        }
        else
        {
            break;
        }
    }

    // Clear the input buffer up to a certain limit
    cin.ignore(MAX_BUFFER_SIZE, '\n');
}

