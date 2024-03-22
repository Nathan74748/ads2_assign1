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

//sell a reserved car
void sell() {
    char reg[10];
    printf("Enter the registration number of the car to sell: ");
    scanf_s("%9s", reg, (unsigned)_countof(reg));
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
    // remove the car from the list
    if (prev == NULL) {
        start = temp->next;
    }
    else {
        prev->next = temp->next;
    }
    free(temp);
    count--;
    printf("Car sold successfully.\n");
}

void reserveUnreserveCar() {
    char reg[10];
    int choice;
    printf("1 for reserve, 2 for unreserve: ");
    scanf_s("%d", &choice, sizeof(choice)); // get user choice

    printf("Enter car reg: ");
    scanf_s("%9s", reg, sizeof(reg)); // get reg

    struct car* c = findCar(reg); // find car
    if (!c) {
        printf("Car not found\n");
        return; 
    }

    if (choice == 1) { // reserve
        if (c->reserved) {
            printf("Already reserved\n");
            return; // check if already reserved
        }
        printf("Reserve price (500-1500): ");
        scanf_s("%d", &c->reservePrice, sizeof(c->reservePrice)); // get reserve price
        if (c->reservePrice < 500 || c->reservePrice > 1500) {
            printf("Invalid price\n");
            return;
        }
        c->reserved = true; // set reserved
        printf("Car reserved\n");
    }
    else if (choice == 2) { // unreserve
        if (!c->reserved) {
            printf("Not reserved\n");
            return;
        }
        c->reserved = false; 
        c->reservePrice = 0; // reset price
        printf("Car unreserved\n");
    }
    else {
        printf("Invalid choice\n"); // invalid choice
    }
}


void viewCars() {
    int choice;
    printf("Choose option:\n1. View all cars\n2. View non-reserved cars\n3. View cars by make\n4. View cars by color\nChoice: ");
    scanf_s("%d", &choice, sizeof(choice));
    getchar(); // clear buffer

    struct car* temp = start;
    char input[50]; // for storing user input for make or color

    switch (choice) {
    case 1: // View all cars
        while (temp) {
            printf("%s, %s, %s, %d previous owner(s), %s, Reserve Price: %d\n",
                temp->reg, temp->makeModel, temp->color, temp->prevOwners,
                temp->reserved ? "Reserved" : "Not Reserved", temp->reservePrice);
            temp = temp->next;
        }
        break;
    case 2: // view non-reserved cars
        while (temp) {
            if (!temp->reserved) {
                printf("%s, %s, %s, %d previous owner(s)\n", temp->reg, temp->makeModel, temp->color, temp->prevOwners);
            }
            temp = temp->next;
        }
        break;
    case 3: // view cars by make
        printf("Enter make to view: ");
        scanf_s("%49s", input, sizeof(input));
        while (temp) {
            if (strcmp(temp->makeModel, input) == 0) {
                printf("%s, %s, %s, %d previous owner(s)\n", temp->reg, temp->makeModel, temp->color, temp->prevOwners);
            }
            temp = temp->next;
        }
        break;
    case 4: // View cars by color
        printf("Enter color to view: ");
        scanf_s("%19s", input, sizeof(input));
        while (temp) {
            if (strcmp(temp->color, input) == 0) {
                printf("%s, %s, %s, %d previous owner(s)\n", temp->reg, temp->makeModel, temp->color, temp->prevOwners);
            }
            temp = temp->next;
        }
        break;
    default:
        printf("Invalid choice\n");
    }
}

// Function to view a specific car
void viewSpecificCar() {
    char reg[10];
    printf("Enter car registration to view: ");
    scanf_s("%9s", reg, sizeof(reg));
    getchar(); // clear buffer

    struct car* car = findCar(reg);
    if (car) {
        printf("Registration: %s, Make & Model: %s, Color: %s, Previous Owners: %d, %s, Reserve Price: %d\n",
            car->reg, car->makeModel, car->color, car->prevOwners,
            car->reserved ? "Reserved" : "Not Reserved", car->reservePrice);
    }
    else {
        printf("Car not found.\n");
    }
}

// Function to update car details
void updateCarDetails() {
    char reg[10];
    printf("Enter the registration number of the car to update: ");
    scanf_s("%9s", reg, sizeof(reg));
    getchar(); // clean buffer after input

    struct car* carToUpdate = findCar(reg);
    if (!carToUpdate) {
        printf("Car not found.\n");
        return;
    }

    // update make and model
    printf("Enter new Make and Model: ");
    scanf_s("%49s", carToUpdate->makeModel, sizeof(carToUpdate->makeModel));
    getchar(); // clear buffer after input

    // update Color
    printf("Enter new Color: ");
    scanf_s("%19s", carToUpdate->color, sizeof(carToUpdate->color));
    getchar(); // clear buffer after input

    printf("Enter number of Previous Owners (0-3): ");
    scanf_s("%d", &carToUpdate->prevOwners);
    getchar(); // clear buffer after input


    printf("Is the car Reserved? (1 for Yes, 0 for No): ");
    scanf_s("%d", (int*)&carToUpdate->reserved);
    getchar(); // clear buffer after input

    if (carToUpdate->reserved) {
        printf("Enter new Reserve Price: ");
        scanf_s("%d", &carToUpdate->reservePrice);
        getchar(); // clean buffer after input
    }
    else {
        carToUpdate->reservePrice = 0; // reset reserve price if car is not reserved
    }

    printf("Car details updated successfully.\n");
}


void loadCarsFromFile() {
    // Open file.
    FILE* file = fopen("car.dat", "rb");
    if (!file) {

        printf("Car list doesn't exist. You will need to input cars.\n");
        return;
    }
    struct car tmpCar;
    while (fread(&tmpCar, sizeof(struct car), 1, file)) {
        struct car* newCar = (struct car*)malloc(sizeof(struct car));
        *newCar = tmpCar;
        newCar->next = start; 
        start = newCar;
        count++; 
    }
    fclose(file); 
    printf("System has been populated with cars from the data file.\n");
}


void saveCarsToFile() {

    FILE* file = fopen("car.dat", "wb");
    if (!file) {
        // cant add if file won't open.
        printf("Failed to open file for writing.\n");
        return;
    }
    struct car* temp = start;
    // add each car to file.
    while (temp) {
        fwrite(temp, sizeof(struct car), 1, file); // save car.
        temp = temp->next; // next car.
    }
    fclose(file); 
    printf("All cars have been saved to the file.\n");
}

// Main  menu
void mainMenu() {
    int choice;
    do {
        printf("1. Add car\n2. Sell car\n3. Reserve/Unreserve\n4. View cars\n5. View a specific car\n6. Update Car Details\n7. Quit\nChoice: ");
        scanf_s("%d", &choice, sizeof(choice));
        switch (choice) {
        case 1: add(); break;
        case 2: sell(); break;
        case 3: reserveUnreserveCar(); break;
        case 4: viewCars(); break;
        case 5: viewSpecificCar(); break;
        case 6: updateCarDetails(); break; // special feasture
        case 7: break; // Exit
        default: printf("Wrong choice\n");
        }
    } while (choice != 7);
}


int main() {
    loadCarsFromFile();// load at the start 
    mainMenu();
    saveCarsToFile();
    return 0;
}