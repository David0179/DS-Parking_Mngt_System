#pragma once

#include <iostream>
#include <stack>
#include <queue>
#include <unordered_map>
#include <string>
#include <memory>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

// Structure to represent a vehicle node in the Binary Search Tree (BST)
struct Vehicle {
    string regNumber;        // Vehicle registration number
    string ownerName;        // Name of the vehicle owner
    string vehicleMake;      // Make of the vehicle (e.g., Toyota, Honda)
    string vehicleModel;     // Model of the vehicle (e.g., Corolla, Civic)
    string vehicleColor;     // Color of the vehicle
    string ownerContact;     // Contact number of the owner
    time_t entryTime;        // Timestamp when the vehicle was parked
    unique_ptr<Vehicle> left; // Pointer to the left child node in the BST
    unique_ptr<Vehicle> right; // Pointer to the right child node in the BST

    // Constructor to initialize a Vehicle instance
    Vehicle(string reg, string owner, string make, string model, string color, string contact);
};

// Class to manage the parking system
class ParkingManagementSystem {
private:
    stack<shared_ptr<Vehicle>> parkingStack;  // Stack to represent vehicles in the parking lot
    queue<shared_ptr<Vehicle>> waitingQueue;  // Queue to manage vehicles waiting for a parking slot
    unordered_map<string, shared_ptr<Vehicle>> vehicleHash; // Hash map for fast lookup by registration number
    unique_ptr<Vehicle> vehicleBSTRoot;       // Root of the BST for storing vehicle data
    int capacity;                             // Maximum parking capacity
    int currentVehicles;                      // Current number of vehicles in the parking lot
    double parkingRate;                       // Parking rate per hour
    double totalRevenue;                      // Total revenue generated
    ofstream logFile;                         // Log file to record parking events

    // Logs events to a file for tracking system activity
    void logEvent(const string& event);

    // Calculates the parking fee based on the entry time
    double calculateFee(time_t entryTime);

    // Inserts a vehicle into the BST
    void insertToBST(unique_ptr<Vehicle>& root, const string& regNumber, const string& ownerName, string& make, string& model, string& color, string& contact);

    // Searches for a vehicle in the BST by its registration number
    Vehicle* searchInBST(Vehicle* root, const string& regNumber);

public:
    // Constructor to initialize the parking system with capacity and rate
    ParkingManagementSystem(int cap, double rate);

    // Destructor to clean up resources
    ~ParkingManagementSystem();

    // Parks a vehicle in the system
    void parkVehicle(const string& regNumber, const string& ownerName);

    // Retrieves a vehicle from the parking lot
    void retrieveVehicle(const string& regNumber);

    // Searches for a vehicle by its registration number
    void searchVehicle(const string& regNumber);

    // Displays the current status of the parking lot
    void displayStatus();

    // Displays all parked vehicles in the system
    void displayAllParkedVehicles();

    // Generates statistics such as total revenue and vehicle count
    void generateStatistics();

    // Returns the current number of vehicles in the parking lot
    int getCurrentVehicleCount();

    // Performs an advanced search for vehicles based on make and model
    void advancedSearch(const string& make, const string& model);

    // Helper function for advanced search (traverses the BST)
    void advancedSearchHelper(Vehicle* root, const string& make, const string& model, vector<Vehicle*>& results);
};

// Validates the format of a registration number
bool isValidRegNumber(const string& regNumber);

// Validates the format of an owner's name
bool isValidOwnerName(const string& ownerName);
