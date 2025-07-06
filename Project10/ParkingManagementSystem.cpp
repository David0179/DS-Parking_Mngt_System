#include "ParkingManagementSystem.h"

#define RED "\033[1;31m" // Bright Red color for error messages
#define RESET "\033[0m"  // Reset to default color

// Constructor for Vehicle class
Vehicle::Vehicle(string reg, string owner, string make, string model, string color, string contact)
    : regNumber(reg), ownerName(owner), vehicleMake(make), vehicleModel(model),
    vehicleColor(color), ownerContact(contact), entryTime(time(0)), left(nullptr), right(nullptr) {}

// Constructor for ParkingManagementSystem
ParkingManagementSystem::ParkingManagementSystem(int cap, double rate)
    : capacity(cap), currentVehicles(0), parkingRate(rate), totalRevenue(0), vehicleBSTRoot(nullptr) {
    logFile.open("parking_Log.txt", ios::app); // Open the log file in append mode
    if (!logFile.is_open()) {
        cout << "Error: Failed to open log file." << endl; // Log file error handling
    }
}

// Destructor for ParkingManagementSystem
ParkingManagementSystem::~ParkingManagementSystem() {
    if (logFile.is_open()) {
        logFile.close(); // Close the log file if open
    }
}

// Logs events with timestamps to the log file
void ParkingManagementSystem::logEvent(const string& event) {
    if (logFile.is_open()) {
        time_t rawTime;
        time(&rawTime);
        struct tm localTimeStruct;
        localtime_s(&localTimeStruct, &rawTime);

        // Append the event with a timestamp
        logFile << "[" << std::put_time(&localTimeStruct, "%Y-%m-%d %H:%M:%S") << "] " << event << std::endl;
    }
}

// Calculates parking fee based on time parked
double ParkingManagementSystem::calculateFee(time_t entryTime) {
    time_t currentTime = time(0); // Current time
    double hoursParked = difftime(currentTime, entryTime) / 3600; // Calculate hours parked
    return round(hoursParked * parkingRate * 100) / 100; // Calculate fee and round to 2 decimals
}

// Inserts a vehicle into the Binary Search Tree (BST)
void ParkingManagementSystem::insertToBST(unique_ptr<Vehicle>& root, const string& regNumber, const string& ownerName, string& make, string& model, string& color, string& contact) {
    if (!root) {
        // If root is null, create a new vehicle node
        root = make_unique<Vehicle>(regNumber, ownerName, make, model, color, contact);
    }
    else if (regNumber < root->regNumber) {
        // Recur to the left subtree if registration number is smaller
        insertToBST(root->left, regNumber, ownerName, make, model, color, contact);
    }
    else {
        // Recur to the right subtree otherwise
        insertToBST(root->right, regNumber, ownerName, make, model, color, contact);
    }
}

// Searches for a vehicle in the BST by registration number
Vehicle* ParkingManagementSystem::searchInBST(Vehicle* root, const string& regNumber) {
    if (!root || root->regNumber == regNumber) {
        // Return the node if found or null if not found
        return root;
    }
    else if (regNumber < root->regNumber) {
        // Search in the left subtree
        return searchInBST(root->left.get(), regNumber);
    }
    else {
        // Search in the right subtree
        return searchInBST(root->right.get(), regNumber);
    }
}

// Validates the vehicle make input
bool isValidVehicleMake(const string& make) {
    if (make.empty()) {
        cout << RED << "Error: Vehicle make cannot be empty." << RESET << endl;
        return false;
    }
    for (char c : make) {
        // Check for invalid characters (only letters and spaces allowed)
        if (!isalpha(c) && c != ' ') {
            cout << RED << "Error: Vehicle make can only contain letters and spaces." << RESET << endl;
            return false;
        }
    }
    // Ensure it doesn't start or end with spaces and doesn't exceed 50 characters
    if (make.front() == ' ' || make.back() == ' ' || make.length() > 50) {
        cout << RED << "Error: Vehicle make format is invalid." << RESET << endl;
        return false;
    }
    return true;
}

// Validates the vehicle model input
bool isValidVehicleModel(const string& model) {
    if (model.empty()) {
        cout << RED << "Error: Vehicle model cannot be empty." << RESET << endl;
        return false;
    }
    for (char c : model) {
        // Check for invalid characters (letters, numbers, spaces allowed)
        if (!isalnum(c) && c != ' ') {
            cout << RED << "Error: Vehicle model can only contain letters, numbers, and spaces." << RESET << endl;
            return false;
        }
    }
    // Ensure it doesn't start or end with spaces and doesn't exceed 50 characters
    if (model.front() == ' ' || model.back() == ' ' || model.length() > 50) {
        cout << RED << "Error: Vehicle model format is invalid." << RESET << endl;
        return false;
    }
    return true;
}

// Validates the vehicle color input
bool isValidVehicleColor(const string& color) {
    if (color.empty()) {
        cout << RED << "Error: Vehicle color cannot be empty." << RESET << endl;
        return false;
    }
    for (char c : color) {
        // Check for invalid characters (only letters and spaces allowed)
        if (!isalpha(c) && c != ' ') {
            cout << RED << "Error: Vehicle color can only contain letters and spaces." << RESET << endl;
            return false;
        }
    }
    // Ensure it doesn't start or end with spaces and doesn't exceed 30 characters
    if (color.front() == ' ' || color.back() == ' ' || color.length() > 30) {
        cout << RED << "Error: Vehicle color format is invalid." << RESET << endl;
        return false;
    }
    return true;
}

// Validates the owner's contact number
bool isValidOwnerContact(const string& contact) {
    if (contact.empty()) {
        cout << RED << "Error: Owner contact cannot be empty." << RESET << endl;
        return false;
    }
    if (contact.length() < 10 || contact.length() > 15 || !all_of(contact.begin(), contact.end(), ::isdigit)) {
        // Validate length and ensure only digits
        cout << RED << "Error: Owner contact must be 10-15 digits." << RESET << endl;
        return false;
    }
    return true;
}

// Handles parking a vehicle
void ParkingManagementSystem::parkVehicle(const string& regNumber, const string& ownerName) {
    string vehicleMake, vehicleModel, vehicleColor, ownerContact;

    // Prompt and validate user input for vehicle details
    cout << "Enter Vehicle Make: ";
    cin >> vehicleMake;
    while (!isValidVehicleMake(vehicleMake)) {
        cout << "Invalid input. Please enter a valid Vehicle Make: ";
        cin >> vehicleMake;
    }

    cout << "Enter Vehicle Model: ";
    cin >> vehicleModel;
    while (!isValidVehicleModel(vehicleModel)) {
        cout << "Invalid input. Please enter a valid Vehicle Model: ";
        cin >> vehicleModel;
    }

    cout << "Enter Vehicle Color: ";
    cin >> vehicleColor;
    while (!isValidVehicleColor(vehicleColor)) {
        cout << "Invalid input. Please enter a valid Vehicle Color: ";
        cin >> vehicleColor;
    }

    cout << "Enter Owner Contact Number: ";
    cin >> ownerContact;
    while (!isValidOwnerContact(ownerContact)) {
        cout << "Invalid input. Please enter valid Owner Contact Details: ";
        cin >> ownerContact;
    }

    // Check if the vehicle is already parked
    if (vehicleHash.find(regNumber) != vehicleHash.end()) {
        cout << "\nError: Vehicle with registration number " << regNumber << " already exists in the parking lot.\n";
        return;
    }

    // Create a new vehicle instance
    auto vehicle = make_shared<Vehicle>(regNumber, ownerName, vehicleMake, vehicleModel, vehicleColor, ownerContact);

    if (currentVehicles < capacity) {
        // If parking has space, park the vehicle
        parkingStack.push(vehicle);
        vehicleHash[regNumber] = vehicle;
        insertToBST(vehicleBSTRoot, regNumber, ownerName, vehicleMake, vehicleModel, vehicleColor, ownerContact);
        currentVehicles++;
        logEvent("Parked vehicle: " + regNumber + " " + ownerName);
        cout << "\nVehicle parked successfully.\n";
    }
    else {
        // If parking is full, add to the waiting queue
        waitingQueue.push(vehicle);
        logEvent("Vehicle added to waiting queue: " + regNumber);
        cout << "\nParking is full. Vehicle added to waiting queue.\n";
    }
}



// Function to retrieve a vehicle from the parking system
void ParkingManagementSystem::retrieveVehicle(const string& regNumber) {
    // Check if the vehicle exists in the hash map
    if (vehicleHash.find(regNumber) == vehicleHash.end()) {
        cout << "\nVehicle not found in the parking lot.\n";
        return;
    }

    // Retrieve vehicle details for confirmation
    Vehicle* vehicle = searchInBST(vehicleBSTRoot.get(), regNumber);

    // Display vehicle details and request confirmation
    cout << "\nPlease confirm the vehicle details before retrieval:\n";
    struct tm localTimeStruct;
    localtime_s(&localTimeStruct, &vehicle->entryTime);
    cout << "\nVehicle found - Registration: " << vehicle->regNumber
        << ", Owner: " << vehicle->ownerName
        << ", Entry Time: " << std::put_time(&localTimeStruct, "%Y-%m-%d %H:%M:%S")
        << "\nMake: " << vehicle->vehicleMake
        << "\nModel: " << vehicle->vehicleModel
        << "\nColor: " << vehicle->vehicleColor
        << "\nOwner Contact: " << vehicle->ownerContact
        << "\n\nDo you want to proceed with retrieving this vehicle? (y/n): ";

    // User confirmation
    char userConfirmation;
    cin >> userConfirmation;

    if (userConfirmation == 'y' || userConfirmation == 'Y') {
        // Temporary stack to maintain parking stack order
        stack<shared_ptr<Vehicle>> tempStack;
        bool found = false;

        // Search for the vehicle in the parking stack
        while (!parkingStack.empty()) {
            auto vehicle = parkingStack.top();
            parkingStack.pop();

            if (vehicle->regNumber == regNumber) {
                // Calculate parking fee and update revenue
                double fee = calculateFee(vehicle->entryTime);
                totalRevenue += fee;
                currentVehicles--;
                vehicleHash.erase(regNumber);
                logEvent("Retrieved vehicle: " + regNumber + ", Fee: $" + to_string(fee));
                cout << "\nVehicle retrieved successfully. Parking fee: $" << fee << "\n";
                found = true;
                break;
            }

            // Push non-matching vehicles into the temporary stack
            tempStack.push(vehicle);
        }

        // Restore vehicles to the original parking stack
        while (!tempStack.empty()) {
            parkingStack.push(tempStack.top());
            tempStack.pop();
        }

        // Handle case where vehicle is not found
        if (!found) {
            cout << "\nVehicle not found in the parking lot.\n";
        }

        // Process waiting queue if a spot becomes available
        if (!waitingQueue.empty() && currentVehicles < capacity) {
            auto nextVehicle = waitingQueue.front();
            waitingQueue.pop();
            parkVehicle(nextVehicle->regNumber, nextVehicle->ownerName);
        }
    }
    else {
        // User cancelled the retrieval process
        cout << "\nVehicle retrieval cancelled.\n";
    }
}

// Function to search for a vehicle by its registration number
void ParkingManagementSystem::searchVehicle(const string& regNumber) {
    Vehicle* vehicle = searchInBST(vehicleBSTRoot.get(), regNumber);

    if (vehicle) {
        // Display vehicle details if found
        struct tm localTimeStruct;
        localtime_s(&localTimeStruct, &vehicle->entryTime);
        cout << "\nVehicle found - Registration: " << vehicle->regNumber
            << ", Owner: " << vehicle->ownerName
            << ", Entry Time: " << std::put_time(&localTimeStruct, "%Y-%m-%d %H:%M:%S")
            << "\nMake: " << vehicle->vehicleMake
            << "\nModel: " << vehicle->vehicleModel
            << "\nColor: " << vehicle->vehicleColor
            << "\nOwner Contact: " << vehicle->ownerContact << endl;
    }
    else {
        // Display a message if the vehicle is not found
        cout << "\nVehicle with registration number " << regNumber << " not found.\n";
    }
}

// Function to perform an advanced search based on vehicle make and model
void ParkingManagementSystem::advancedSearch(const string& make, const string& model) {
    if (!vehicleBSTRoot) {
        cout << "\nNo vehicles in the system.\n";
        return;
    }

    // Collect matching vehicles
    vector<Vehicle*> results;
    advancedSearchHelper(vehicleBSTRoot.get(), make, model, results);

    if (results.empty()) {
        // No matches found
        cout << "\nNo vehicles found with the specified filters.\n";
    }
    else {
        // Display matching vehicles
        cout << "\nSearch Results:\n";
        for (auto& vehicle : results) {
            struct tm localTimeStruct;
            localtime_s(&localTimeStruct, &vehicle->entryTime);
            cout << "\nRegistration: " << vehicle->regNumber
                << ", Owner: " << vehicle->ownerName
                << ", Entry Time: " << std::put_time(&localTimeStruct, "%Y-%m-%d %H:%M:%S")
                << "\nMake: " << vehicle->vehicleMake
                << "\nModel: " << vehicle->vehicleModel
                << "\nColor: " << vehicle->vehicleColor
                << "\nOwner Contact: " << vehicle->ownerContact
                << "\n";
        }
    }
}

// Helper function for advanced search (recursive traversal)
void ParkingManagementSystem::advancedSearchHelper(Vehicle* root, const string& make, const string& model, vector<Vehicle*>& results) {
    if (!root) return;

    // Check if vehicle matches the filters
    bool match = true;
    if (!make.empty() && root->vehicleMake != make) match = false;
    if (!model.empty() && root->vehicleModel != model) match = false;

    if (match) {
        results.push_back(root);
    }

    // Recursive traversal of left and right subtrees
    advancedSearchHelper(root->left.get(), make, model, results);
    advancedSearchHelper(root->right.get(), make, model, results);
}

// Function to display current parking status
void ParkingManagementSystem::displayStatus() {
    cout << "Vehicles Parked: " << currentVehicles << "/" << capacity << "\n";
    if (!waitingQueue.empty()) {
        cout << "\nWaiting Queue: ";
        queue<shared_ptr<Vehicle>> tempQueue = waitingQueue;
        while (!tempQueue.empty()) {
            cout << tempQueue.front()->regNumber << " ";
            tempQueue.pop();
        }
        cout << "\n";
    }
}

// Function to display all parked vehicles
void ParkingManagementSystem::displayAllParkedVehicles() {
    if (parkingStack.empty()) {
        cout << "\nNo vehicles currently parked.\n";
        return;
    }
    displayStatus();
    cout << "\nList of Parked Vehicles:\n";
    stack<shared_ptr<Vehicle>> tempStack = parkingStack;

    while (!tempStack.empty()) {
        auto vehicle = tempStack.top();
        tempStack.pop();

        struct tm localTimeStruct;
        localtime_s(&localTimeStruct, &vehicle->entryTime);

        cout << "\033[1;36mRegistration: " << vehicle->regNumber
            << ", Owner: " << vehicle->ownerName
            << ", Entry Time: " << std::put_time(&localTimeStruct, "%Y-%m-%d %H:%M:%S")
            << "\nMake: " << vehicle->vehicleMake
            << "\nModel: " << vehicle->vehicleModel
            << "\nColor: " << vehicle->vehicleColor
            << "\nOwner Contact: " << vehicle->ownerContact
            << "\033[0m" << endl;
    }
}

// Function to generate and display parking statistics
void ParkingManagementSystem::generateStatistics() {
    cout << "\nTotal revenue collected: $" << totalRevenue << "\n";
}

// Error checks for registration number
bool isValidRegNumber(const string& regNumber) {
    // Various validation checks for registration number format
    if (regNumber.length() < 3) {
        cout << RED << "Error: Registration number must be at least 3 characters long." << RESET << endl;
        return false;
    }
    if (regNumber.length() > 10) {
        cout << RED << "Error: Registration number must not exceed 10 characters." << RESET << endl;
        return false;
    }
    if (regNumber.front() == '0') {
        cout << RED << "Error: Registration number cannot start with '0'." << RESET << endl;
        return false;
    }
    for (char c : regNumber) {
        if (!isalnum(c)) {
            cout << RED << "Error: Registration number must be alphanumeric." << RESET << endl;
            return false;
        }
    }
    if (regNumber.find(' ') != string::npos) {
        cout << RED << "Error: Registration number cannot contain spaces." << RESET << endl;
        return false;
    }
    return true;
}

// Error checks for owner name
bool isValidOwnerName(const string& ownerName) {
    // Various validation checks for owner name format
    if (ownerName.empty()) {
        cout << RED << "Error: Owner name cannot be empty." << RESET << endl;
        return false;
    }
    for (char c : ownerName) {
        if (!isalpha(c) && c != ' ') {
            cout << RED << "Error: Owner name can only contain letters and spaces." << RESET << endl;
            return false;
        }
    }
    if (ownerName.front() == ' ' || ownerName.back() == ' ') {
        cout << RED << "Error: Owner name cannot start or end with a space." << RESET << endl;
        return false;
    }
    for (size_t i = 1; i < ownerName.length(); ++i) {
        if (ownerName[i] == ' ' && ownerName[i - 1] == ' ') {
            cout << RED << "Error: Owner name cannot have consecutive spaces." << RESET << endl;
            return false;
        }
    }
    if (ownerName.length() > 100) {
        cout << RED << "Error: Owner name must not exceed 100 characters." << RESET << endl;
        return false;
    }
    return true;
}

