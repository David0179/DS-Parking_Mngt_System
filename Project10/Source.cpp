
/*  
  Made by: 
       Daud 230499
       Faria 230477
       Aqsa 230489
*/

#include <iostream>
#include <limits>
#include <string>
#include "ParkingManagementSystem.h"

using namespace std;

// Color codes for console output
const string RED_TEXT = "\033[31m";    // Red text for errors or warnings
const string RESET_TEXT = "\033[0m";  // Reset to default text color

// Function to display the menu with car ASCII art
void displayMenuWithCar() {
    // Define colors for menu background, text, and decorative elements
    const string BACKGROUND_COLOR = "\033[48;5;17m"; // Dark Blue background
    const string TEXT_COLOR = "\033[38;5;81m";      // Light Blue text
    const string RESET_COLOR = "\033[0m";           // Reset to default
    const string BORDER_COLOR = "\033[38;5;226m";  // Yellow for border
    const string CAR1_COLOR = "\033[38;5;202m";    // Orange car

    // Top border for the menu
    cout << BACKGROUND_COLOR << BORDER_COLOR;
    cout << "**************************************************************************************************" << RESET_COLOR << endl;

    // Menu header with car designs
    cout << BACKGROUND_COLOR << TEXT_COLOR
        << "                " << CAR1_COLOR << ".--------." << RESET_COLOR
        << BACKGROUND_COLOR << TEXT_COLOR << "    Welcome to the Parking Management System"
        << BACKGROUND_COLOR << TEXT_COLOR << "       " << CAR1_COLOR << " .--------.          " << RESET_COLOR << endl;

    cout << BACKGROUND_COLOR << TEXT_COLOR
        << "       " << CAR1_COLOR << "_______/_____|_____\\____" << RESET_COLOR
        << BACKGROUND_COLOR << TEXT_COLOR << "                                     "
        << CAR1_COLOR << "  _______/_____|_____\\____    " << RESET_COLOR << endl;

    cout << BACKGROUND_COLOR << TEXT_COLOR
        << "      " << CAR1_COLOR << "O      _     |     _      O" << RESET_COLOR
        << BACKGROUND_COLOR << TEXT_COLOR << "            Parking Menu"
        << BACKGROUND_COLOR << TEXT_COLOR << "            "
        << CAR1_COLOR << "O      _     |     _      O  " << RESET_COLOR << endl;

    cout << BACKGROUND_COLOR << TEXT_COLOR
        << "       " << CAR1_COLOR << "'--(_)-------(_)--'------'" << RESET_COLOR
        << BACKGROUND_COLOR << TEXT_COLOR << "                                    "
        << CAR1_COLOR << "'--(_)-------(_)--'------'   " << RESET_COLOR << endl;

    // Display menu options
    cout << BACKGROUND_COLOR << BORDER_COLOR;
    cout << "**************************************************************************************************" << RESET_COLOR << endl;
    cout << BACKGROUND_COLOR << TEXT_COLOR << "                                  1. Park a Vehicle                                               " << RESET_TEXT << endl;
    cout << BACKGROUND_COLOR << TEXT_COLOR << "                                  2. Retrieve a Vehicle                                           " << RESET_TEXT << endl;
    cout << BACKGROUND_COLOR << TEXT_COLOR << "                                  3. Search for a Vehicle                                         " << RESET_TEXT << endl;
    cout << BACKGROUND_COLOR << TEXT_COLOR << "                                  4. Apply Filter                                                 " << RESET_TEXT << endl;
    cout << BACKGROUND_COLOR << TEXT_COLOR << "                                  5. Display Parking Lot                                          " << RESET_TEXT << endl;
    cout << BACKGROUND_COLOR << TEXT_COLOR << "                                  6. Generate Statistics                                          " << RESET_TEXT << endl;
    cout << BACKGROUND_COLOR << TEXT_COLOR << "                                  7. Exit                                                         " << RESET_TEXT << endl;

    // Bottom border for the menu
    cout << BACKGROUND_COLOR << BORDER_COLOR;
    cout << "**************************************************************************************************" << RESET_COLOR << endl;

    // Prompt the user for input
    cout << BACKGROUND_COLOR << TEXT_COLOR << "                             Enter your choice (1-7): " << RESET_COLOR;
}

int main() {
    // Initialize the Parking Management System with capacity and rate
    ParkingManagementSystem pms(5, 10.0); // Capacity: 5, Rate: $10/hour

    int choice; // Variable to store user's menu choice

    do {
        system("cls"); // Clear the screen (use "clear" for Linux/Mac)
        displayMenuWithCar(); // Display the main menu

        // Input validation for menu choice
        while (!(cin >> choice) || choice < 1 || choice > 7) {
            cin.clear(); // Clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << RED_TEXT << "Invalid choice. Please enter a number between 1 and 7: " << RESET_TEXT;
        }

        // Handle user's menu choice
        switch (choice) {
        case 1: { // Option to park a vehicle
            string regNumber, ownerName;
            cout << "\nEnter Vehicle Details:\n";

            cout << "   Registration Number: ";
            cin >> regNumber;

            // Validate registration number format
            while (!isValidRegNumber(regNumber)) {
                cout << RED_TEXT << "Invalid Registration Number. Please enter a valid Number: " << RESET_TEXT;
                cin >> regNumber;
            }

            cout << "   Owner Name: ";
            cin >> ownerName;

            // Validate owner name format
            while (!isValidOwnerName(ownerName)) {
                cout << RED_TEXT << "Invalid Owner Name. Please enter a valid name: " << RESET_TEXT;
                cin >> ownerName;
            }

            // Park the vehicle
            pms.parkVehicle(regNumber, ownerName);
            break;
        }
        case 2: { // Option to retrieve a vehicle
            string regNumber;
            cout << "\nEnter the Registration Number of the Vehicle to Retrieve: ";
            cin >> regNumber;

            // Validate registration number format
            while (!isValidRegNumber(regNumber)) {
                cout << RED_TEXT << "Invalid Registration Number. Please enter again: " << RESET_TEXT;
                cin >> regNumber;
            }

            // Retrieve the vehicle
            pms.retrieveVehicle(regNumber);
            break;
        }
        case 3: { // Option to search for a specific vehicle
            string regNumber;
            cout << "\nEnter the Registration Number of the Vehicle to Search: ";
            cin >> regNumber;

            // Validate registration number format
            while (!isValidRegNumber(regNumber)) {
                cout << RED_TEXT << "Invalid Registration Number. Please enter again: " << RESET_TEXT;
                cin >> regNumber;
            }

            // Search for the vehicle
            pms.searchVehicle(regNumber);
            break;
        }
        case 4: { // Option to apply a filter (advanced search)
            string make, model;

            cout << "\nEnter vehicle make (or press Enter to skip): ";
            cin.ignore(); // Clear newline from previous input
            getline(cin, make);

            cout << "Enter vehicle model (or press Enter to skip): ";
            getline(cin, model);

            // Check if both make and model are empty
            if (!make.empty() || !model.empty()) {
               
                pms.advancedSearch(make, model);
            }
            else {
                cout << "No inputs provided. Skipping advanced search." << endl;
            }

            break;
        }
        case 5: // Option to display all parked vehicles
            pms.displayAllParkedVehicles();
            break;

        case 6: // Option to generate parking statistics
            pms.generateStatistics();
            break;

        case 7: // Option to exit the program
            cout << "\nExiting... Thank you for using the Parking Management System!\n";
            break;

        default: // Handle invalid menu options
            cout << RED_TEXT << "\nInvalid choice. Please try again.\n" << RESET_TEXT;
            break;
        }

        // Wait for user to press a key before continuing
        cout << "\nPress any key to continue...";
        cin.ignore();
        cin.get();
    } while (choice != 7); // Loop until the user chooses to exit

    return 0;
}
