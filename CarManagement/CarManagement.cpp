#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 5 // Maximum number of cars

// Car structure
struct car {
    char reg[10];
    char makeModel[50];
    char color[20];
    int prevOwners;
    bool reserved;
    int reservePrice;
    struct car* next;
};

struct car* start = NULL;
int count = 0;

// Check if car registration is valid
bool checkReg(const char* r) {
    if (strlen(r) != 8) return false;
    // Registration format check
    if (r[2] != 'D' || (r[3] != '1' && r[3] != '2')) return false;
    for (int i = 4; i < 8; i++) if (r[i] < '0' || r[i] > '9') return false;
    return true;
}

// Find car by registration
struct car* findCar(const char* reg) {
    struct car* temp = start;
    while (temp) {
        if (strcmp(temp->reg, reg) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

// Add a new car to the list
void add() {
    if (count >= MAX) {
        printf("No more space in the showroom.\n");
        return;
    }

    struct car* newCar = (struct car*)malloc(sizeof(struct car));
    if (!newCar) {
        printf("Memory allocation failed.\n");
        return;
    }

    while (true) {
        printf("Enter registration number (format yyDxnnnn, e.g., 23D21234): ");
        scanf_s("%9s", newCar->reg, sizeof(newCar->reg));
        if (checkReg(newCar->reg) && !findCar(newCar->reg)) {
            break;
        }
        else {
            printf("Invalid registration number or it already exists. Please try again.\n");
        }
    }

    printf("Enter Make and Model: ");
    scanf_s("%49s", newCar->makeModel, sizeof(newCar->makeModel));
    printf("Enter Color: ");
    scanf_s("%19s", newCar->color, sizeof(newCar->color));
    printf("Enter number of Previous owners (0-3): ");
    scanf_s("%d", &newCar->prevOwners);
    printf("Is the car Reserved? (1 for Yes, 0 for No): ");
    scanf_s("%d", (int*)&newCar->reserved);
    newCar->reservePrice = 0;
    if (newCar->reserved) {
        printf("Enter Reserve price: ");
        scanf_s("%d", &newCar->reservePrice);
    }

    // Add car to the list
    newCar->next = start;
    start = newCar;
    count++;
    printf("Car added successfully!\n");
}

// Sell a reserved car
void sell() {
    char reg[10];
    printf("Enter the registration number of the car to sell: ");
    scanf_s("%9s", reg);
    struct car* temp = start, * prev = NULL;
    while (temp != NULL && strcmp(temp->reg, reg) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Car not found.\n");
        return;
    }
    if (!temp->reserved) {
        printf("Car is not reserved and cannot be sold.\n");
        return;
    }
    // Remove the car from the list
    if (prev == NULL) {
        start = temp->next;
    }
    else { // The car removed is first
        prev->next = temp->next;
    }
    free(temp); // Free the memory allocated for the sold car
    count--; // Decrease the car count
    printf("Car sold successfully.\n");
}
// Main interaction menu
void mainMenu() {
    int choice;
    do {
        printf("1. Add a car\n2. Sell a car\n3. Quit\nEnter choice: ");
        scanf_s("%d", &choice);
        switch (choice) {
        case 1: add(); break;
        case 2: sell(); break;
        case 3: break;
        default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

int main() {
    mainMenu();
    return 0;
}