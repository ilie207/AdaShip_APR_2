// ComputerPlayer.cpp
#include "ComputerPlayer.h"
#include "Utils.h"
#include <iostream>

ComputerPlayer::ComputerPlayer() : Player() {
    // Additional constructor logic if needed
    std::srand(time(nullptr)); // Seed for randomization
}

void ComputerPlayer::autoSetup() {
    std::cout << "Computer is setting up its ships..." << std::endl;

    // For simplicity, auto-place ships using a random strategy
    shipBoard.autoPlaceShips();

    // Display the computer's shipboard
    std::cout << "Computer's Shipboard:" << std::endl;
    shipBoard.printBoard();
}

void ComputerPlayer::makeSmartMove(Player &opponent) {
    std::cout << "Computer is making a smart move..." << std::endl;

    // Intelligent move strategy (for simplicity, use random move)
    std::string targetCoordinate = generateRandomCoordinate();

    // Check if the coordinate has been targeted before
    while (opponent.isTargeted(targetCoordinate)) {
        targetCoordinate = generateRandomCoordinate();
    }

    // Mark the target on the opponent's shipboard
    //bool isHit = opponent.markHit(targetCoordinate);
    opponent.getTargetBoard().printBoard(); // Display the result of the computer's turn
}

bool ComputerPlayer::isValidSmartMove(int row, int col, const Player &opponent) const {
    // Add intelligence to check if the move is valid
    return row >= 0 && row < 10 && col >= 0 && col < 10 && !opponent.isTargeted(convertToCoordinate(row, col));
}

std::string ComputerPlayer::generateRandomCoordinate() const {
    // Generate a random coordinate for the computer's move
    int row = std::rand() % 10;
    int col = std::rand() % 10;
    return convertToCoordinate(row, col);
}
