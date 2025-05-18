#ifndef PLAY_HPP
#define PLAY_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits> // Required for std::numeric_limits

#include "action.hpp" // Includes Action and Inventory
#include "nodes.hpp"  // Includes Node, Option, Action, and Inventory

namespace GAME {

    class Game {
        private:
            std::string menuName;
            bool didExit = false;

            // Helpers
            int safeInput(); // More robust input handling
            void printInv(const INVENTORY::Inventory& inv) const; // Made const
            void printName() const; // Made const

        public:
            Game(std::string menuName) : menuName(menuName) {} // Member initializer list
            ~Game() = default;

            void Init();
            void Run(NODE::NodePtr rootNode, INVENTORY::Inventory& inventory);
    };

    // Implementation of Game methods
    int Game::safeInput() {
        int input;
        while (!(std::cin >> input)) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear(); // Clear the error flags
            // Ignore the rest of the invalid input from the buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        // Consume the rest of the line after reading the integer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return input;
    }

    void Game::printInv(const INVENTORY::Inventory& inv) const {
        std::cout << "--- INVENTORY ---\n";
        if (inv.items.empty()) {
            std::cout << "Inventory is empty.\n";
        } else {
            for (size_t i = 0; i < inv.items.size(); ++i) { // Use size_t for loop index
                std::cout << i + 1 << ". " << inv.items[i].name << "\n"; // Print 1-based index
            }
        }
        std::cout << "-----------------\n";
    }

    void Game::printName() const {
        std::cout << "+======";
        for (size_t i = 0; i < menuName.size(); ++i) { // Use size_t for loop index
            std::cout << "=";
        }
        std::cout << "+\n|   " << menuName << "   |\n+";
        for (size_t i = 0; i < menuName.size(); ++i) { // Use size_t for loop index
            std::cout << "=";
        }
        std::cout << "======+";
    }

    void Game::Init() {
        bool done = false;

        while (!done) {
            printName();

            std::cout << "\n1. New Game\n2. Load Game\n3. Quit\n\nEnter your choice: ";
            int input = safeInput();

            switch (input) {
                case 1:
                    done = true;
                    break;
                case 2:
                    std::cout << "[INFO] Load game not implemented yet.\n";
                    // * load game here
                    break;
                case 3:
                    didExit = true;
                    done = true;
                    break;
                default:
                    std::cout << "\nPlease enter a valid input (1-3).\n\n";
            }
        }
    }

    void Game::Run(NODE::NodePtr rootNode, INVENTORY::Inventory& inventory) {
        bool running = true;

        if (didExit) { running = false; }

        if (!didExit) {
            NODE::NodePtr currentNode = rootNode;

            while (running) {
                std::cout << "\n---------\n" << currentNode->text << "\n";

                // on enter actions
                if (currentNode->onEnterAction.type != ACTION::TYPE::NONE) {
                     // Decide the logic for on-enter actions more clearly.
                     // This current check prevents the action if *any* of the items
                     // in onEnterPickupItems are already in the inventory.
                     // Is this the intended behavior?
                    if (!inventory.hasItems(currentNode->onEnterPickupItems)) {
                        currentNode->onEnterAction.executeAction(inventory, currentNode->onEnterPickupItems, currentNode->onEnterUseItem);
                    } else {
                         // Only print this message if the action *was* a pickup action
                        if (currentNode->onEnterAction.type == ACTION::TYPE::PICKUP) {
                            std::cout << "\n[INFO] You already have some of the items trying to be picked up here.\n";
                        }
                    }
                }

                if (currentNode->isEndNode()) {
                    std::cout << "\n---------\nEnd of the game.\n";
                    running = false;
                    break;
                }

                // print all options
                for (size_t i = 0; i < currentNode->options.size(); ++i) { // Use size_t
                    std::cout << i + 1 << ". " << currentNode->options[i].text << "\n"; // Print 1-based index
                }

                int input = 0;
                bool validInput = false;
                while (!validInput) {
                    std::cout << "\n(-1 to see inventory, -2 to exit)";
                    std::cout << "\nEnter your choice: ";
                    int rawInput = safeInput(); // Use safeInput

                    if (rawInput == -1) {
                        std::cout << "\n";
                        printInv(inventory);
                        // Reprint node text and options after showing inventory
                        std::cout << "\n---------\n" << currentNode->text << "\n";
                         for (size_t i = 0; i < currentNode->options.size(); ++i) {
                            std::cout << i + 1 << ". " << currentNode->options[i].text << "\n";
                        }
                    } else if (rawInput == -2) {
                        running = false;
                        validInput = true; // Exit the input loop
                    } else {
                        input = rawInput - 1; // Adjust for 0-based indexing
                        if (input >= 0 && static_cast<size_t>(input) < currentNode->options.size()) { // Use static_cast
                            validInput = true; // Valid option selected
                        } else {
                            std::cout << "Please enter an existing option number (1 - " << currentNode->options.size() << ").";
                        }
                    }
                }

                // Check if the game is still running before processing the choice
                if (!running) {
                    break; // Exit the main game loop if the player chose to exit
                }

                // do choice actions
                auto& chosenOption = currentNode->options[input];
                if (chosenOption.useAction.type != ACTION::TYPE::NONE) {
                    // Decide the logic for option actions more clearly.
                    // This current check prevents the action if *any* of the items
                    // in chosenOption.pickupItems are already in the inventory.
                    // Is this the intended behavior?
                    if (!inventory.hasItems(chosenOption.pickupItems)) {
                        chosenOption.useAction.executeAction(inventory, chosenOption.pickupItems, chosenOption.useItem);
                    } else {
                        // Only print this message if the action *was* a pickup action
                        if (chosenOption.useAction.type == ACTION::TYPE::PICKUP) {
                            std::cout << "\n[INFO] You already have some of the items trying to be picked up by this option.\n";
                        }
                    }
                }

                currentNode = currentNode->nextNodes[input];
            }
        }
    }
}

#endif
