//
// Created by Mario on 07/06/2023.
//

#ifndef SRC_IO_H
#define SRC_IO_H

#include <iostream>
#include <thread>
#include <chrono>

static const int DEFAULT_DELAY = 25;
static const std::streamsize MAX_BUFFER_SIZE = 1000;

/**
 * Prints the text gradually with a specified delay between characters.
 * @param text The text to be printed.
 * @param delayMs The delay in milliseconds between each character.
 */
void animateText(const std::string& text, int delayMs = DEFAULT_DELAY);

/**
 * Reads a single number, of value between min and max, into the variableForInput variable.
 * Loops until read is successful.
 */
void readSingleNumberInput(unsigned int& variableForInput, unsigned int min, unsigned int max,
                            const std::string& wrongInputString, int printSpeed);

#endif //SRC_IO_H
