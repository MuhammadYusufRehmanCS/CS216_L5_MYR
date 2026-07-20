/*
    Muhammad Yusuf Rehman
    CS 216 Lab #5 Army DMA
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <new>
#include <cctype>

using namespace std;

const string STUDENT_NAME = "Muhammad Yusuf Rehman";
const string LAB_NAME = "Lab #5 Army DMA";
const string DEFAULT_CREATURE_NAME = "n/a";
const string DEFAULT_CREATURE_TYPE = "Warrior";
const string DEFAULT_ARMY_NAME = "None";
const string INVALID_MENU_MESSAGE = "Invalid menu choice.";
const string INVALID_ARMY_MESSAGE = "Invalid army name or size.";
const string INVALID_CREATURE_MESSAGE = "Invalid creature data. Default values remain unchanged.";

const int MIN_STAT = 60;
const int MAX_STAT = 200;
const int MIN_ARMY_SIZE = 5;
const int MIN_ARMY_NAME_LENGTH = 3;
const int CREATURE_TYPE_COUNT = 4;
const int MENU_CREATE_ARMY = 1;
const int MENU_QUIT = 2;
const int NAME_WIDTH = 20;
const int TYPE_WIDTH = 15;
const int NUMBER_WIDTH = 12;

enum MenuChoice
{
    CREATE_ARMY = MENU_CREATE_ARMY,
    QUIT = MENU_QUIT
};

class Creature
{
private:
    string name = DEFAULT_CREATURE_NAME;
    string type = DEFAULT_CREATURE_TYPE;
    int strength = MIN_STAT;
    int health = MIN_STAT;

public:
    Creature();
    Creature(const Creature & other);
    explicit Creature(const string & creatureName, const string & creatureType);
    Creature(const string & creatureName, const string & creatureType, int creatureStrength, int creatureHealth);
    ~Creature();

    void setName(const string & creatureName);
    void setType(const string & creatureType);
    void setStrength(int creatureStrength);
    void setHealth(int creatureHealth);
    void setCreature(const string & creatureName, const string & creatureType, int creatureStrength, int creatureHealth);

    string getName() const;
    string getType() const;
    int getStrength() const;
    int getHealth() const;
    string to_String() const;

private:
    bool isValidStat(int value) const;
};

class Army
{
private:
    string name = DEFAULT_ARMY_NAME;
    int size = 0;
    Creature ** creatures = nullptr;

public:
    Army();
    Army(const Army & other);
    explicit Army(const string & armyName, int armySize);
    ~Army();

    string getName() const;
    int getSize() const;
    void print() const;

private:
    void setArmy(const string & armyName, int armySize);
    void copyArmy(const Army & other);
    void allocateCreatures(const string & armyName, int armySize);
    void deallocateCreatures(Creature ** & creatureList, int creatureCount);
    bool isValidArmyName(const string & armyName) const;
    string randomType() const;
};

void displayMenu();
void createArmy();
void clearCin(const string & errorMessage);
string getArmyName();
int getArmySize();
int randomNumber(int minimum, int maximum);

int main()
{
    int menuChoice = 0;

    srand(static_cast<unsigned int>(time(nullptr)));

    cout << STUDENT_NAME << '\n'
         << LAB_NAME << '\n';

    do
    {
        displayMenu();
        cin >> menuChoice;

        switch (menuChoice)
        {
            case CREATE_ARMY:
                createArmy();
                break;

            case QUIT:
                cout << "Program ended.\n";
                break;

            default:
                clearCin(INVALID_MENU_MESSAGE);
        }
    } while (menuChoice != QUIT);

    return 0;
}

// Description: Initializes one creature with valid default values.
// Precondition: A Creature object is being created.
// Postcondition: The Creature object contains valid default values.
Creature::Creature()
{
    setCreature(DEFAULT_CREATURE_NAME, DEFAULT_CREATURE_TYPE, MIN_STAT, MIN_STAT);
}

// Description: Creates one creature as a copy of another creature.
// Precondition: The source Creature object is valid.
// Postcondition: The new Creature contains the same member values as the source.
Creature::Creature(const Creature & other)
{
    setCreature(other.name, other.type, other.strength, other.health);
}

// Description: Initializes a creature with a name and type plus random statistics.
// Precondition: The creature name and type are available.
// Postcondition: The Creature contains the supplied name and type with valid random statistics.
Creature::Creature(const string & creatureName, const string & creatureType)
{
    int creatureStrength = randomNumber(MIN_STAT, MAX_STAT);
    int creatureHealth = randomNumber(MIN_STAT, MAX_STAT);

    setCreature(creatureName, creatureType, creatureStrength, creatureHealth);
}

// Description: Initializes a creature with four supplied member values.
// Precondition: The supplied statistics may be valid or invalid.
// Postcondition: Valid values are stored or the initialized defaults remain unchanged.
Creature::Creature(const string & creatureName, const string & creatureType, int creatureStrength, int creatureHealth)
{
    setCreature(creatureName, creatureType, creatureStrength, creatureHealth);
}

// Description: Announces when one Creature object is destroyed.
// Precondition: A Creature object has reached the end of its lifetime.
// Postcondition: The Creature object has been destroyed.
Creature::~Creature()
{
    cout << name << ' ' << type << " destructor\n";
}

// Description: Attempts to replace the creature name.
// Precondition: The Creature object is valid.
// Postcondition: All members remain valid and the name changes when the complete record is valid.
void Creature::setName(const string & creatureName)
{
    setCreature(creatureName, type, strength, health);
}

// Description: Attempts to replace the creature type.
// Precondition: The Creature object is valid.
// Postcondition: All members remain valid and the type changes when the complete record is valid.
void Creature::setType(const string & creatureType)
{
    setCreature(name, creatureType, strength, health);
}

// Description: Attempts to replace the creature strength.
// Precondition: The Creature object is valid.
// Postcondition: All members remain valid and strength changes only when the value is valid.
void Creature::setStrength(int creatureStrength)
{
    setCreature(name, type, creatureStrength, health);
}

// Description: Attempts to replace the creature health.
// Precondition: The Creature object is valid.
// Postcondition: All members remain valid and health changes only when the value is valid.
void Creature::setHealth(int creatureHealth)
{
    setCreature(name, type, strength, creatureHealth);
}

// Description: Validates and sets all Creature member variables as one transaction.
// Precondition: The Creature object already has valid initialized member values.
// Postcondition: All supplied values are stored together or the object remains unchanged.
void Creature::setCreature(const string & creatureName, const string & creatureType, int creatureStrength, int creatureHealth)
{
    bool validStrength = isValidStat(creatureStrength);
    bool validHealth = isValidStat(creatureHealth);

    if (validStrength && validHealth)
    {
        name = creatureName;
        type = creatureType;
        strength = creatureStrength;
        health = creatureHealth;
    }
    else
    {
        cout << INVALID_CREATURE_MESSAGE << '\n';
    }
}

// Description: Returns the creature name.
// Precondition: The Creature object is valid.
// Postcondition: The creature name is returned without changing the object.
string Creature::getName() const
{
    return name;
}

// Description: Returns the creature type.
// Precondition: The Creature object is valid.
// Postcondition: The creature type is returned without changing the object.
string Creature::getType() const
{
    return type;
}

// Description: Returns the creature strength.
// Precondition: The Creature object is valid.
// Postcondition: The creature strength is returned without changing the object.
int Creature::getStrength() const
{
    return strength;
}

// Description: Returns the creature health.
// Precondition: The Creature object is valid.
// Postcondition: The creature health is returned without changing the object.
int Creature::getHealth() const
{
    return health;
}

// Description: Formats one creature as a table row.
// Precondition: The Creature object is valid.
// Postcondition: A formatted string containing all creature data is returned.
string Creature::to_String() const
{
    stringstream output;

    output << left << setw(NAME_WIDTH) << name
           << setw(TYPE_WIDTH) << type
           << right << setw(NUMBER_WIDTH) << strength
           << setw(NUMBER_WIDTH) << health;

    return output.str();
}

// Description: Checks whether one statistic is within the allowed range.
// Precondition: A statistic value is supplied.
// Postcondition: True is returned for a valid value and false otherwise.
bool Creature::isValidStat(int value) const
{
    bool isValid = value >= MIN_STAT && value <= MAX_STAT;

    return isValid;
}

// Description: Initializes an empty Army object.
// Precondition: An Army object is being created.
// Postcondition: The Army has a valid default name, zero size, and no allocated memory.
Army::Army()
{
}

// Description: Creates a deep copy of another Army object.
// Precondition: The source Army object is valid.
// Postcondition: A complete independent copy is created or a valid empty Army remains.
Army::Army(const Army & other)
{
    copyArmy(other);
}

// Description: Creates an Army using a supplied name and size.
// Precondition: The name and size may be valid or invalid.
// Postcondition: A complete Army is created or a valid empty Army remains.
Army::Army(const string & armyName, int armySize)
{
    setArmy(armyName, armySize);
}

// Description: Releases every dynamically allocated Creature and the pointer array.
// Precondition: The Army may own dynamically allocated memory.
// Postcondition: All owned memory is released and the Army is destroyed.
Army::~Army()
{
    cout << "Army " << name << " destructor in progress\n";
    deallocateCreatures(creatures, size);
    size = 0;
    name = DEFAULT_ARMY_NAME;
}

// Description: Returns the Army name.
// Precondition: The Army object is valid.
// Postcondition: The Army name is returned without changing the object.
string Army::getName() const
{
    return name;
}

// Description: Returns the number of creatures in the Army.
// Precondition: The Army object is valid.
// Postcondition: The Army size is returned without changing the object.
int Army::getSize() const
{
    return size;
}

// Description: Prints the Army name and all creatures in table format.
// Precondition: The Army object is valid.
// Postcondition: The Army data is displayed without changing the object.
void Army::print() const
{
    int index = 0;

    cout << "\nArmy: " << name << "\n\n"
         << left << setw(NAME_WIDTH) << "Name"
         << setw(TYPE_WIDTH) << "Type"
         << right << setw(NUMBER_WIDTH) << "Strength"
         << setw(NUMBER_WIDTH) << "Health" << '\n'
         << string(NAME_WIDTH + TYPE_WIDTH + NUMBER_WIDTH + NUMBER_WIDTH, '-') << '\n';

    while (index < size)
    {
        cout << creatures[index]->to_String() << '\n';
        index++;
    }
}

// Description: Validates the requested Army and creates it as one complete transaction.
// Precondition: The Army object is in a valid state.
// Postcondition: A complete new Army is stored or the original Army remains unchanged.
void Army::setArmy(const string & armyName, int armySize)
{
    bool validName = isValidArmyName(armyName);
    bool validSize = armySize >= MIN_ARMY_SIZE;

    if (validName && validSize)
    {
        allocateCreatures(armyName, armySize);
    }
    else
    {
        cout << INVALID_ARMY_MESSAGE << '\n';
    }
}

// Description: Deep-copies another Army while protecting against partial allocation.
// Precondition: The source Army object is valid.
// Postcondition: A full independent copy is stored or this Army remains valid and empty.
void Army::copyArmy(const Army & other)
{
    Creature ** temporaryCreatures = nullptr;
    int copiedCount = 0;
    bool copySuccessful = true;

    if (other.size > 0)
    {
        try
        {
            temporaryCreatures = new Creature *[other.size];

            while (copiedCount < other.size)
            {
                temporaryCreatures[copiedCount] = nullptr;
                copiedCount++;
            }

            copiedCount = 0;

            while (copiedCount < other.size)
            {
                temporaryCreatures[copiedCount] = new Creature(*other.creatures[copiedCount]);
                copiedCount++;
            }
        }
        catch (const bad_alloc &)
        {
            copySuccessful = false;
            deallocateCreatures(temporaryCreatures, copiedCount);
            cout << "Army copy failed because memory could not be allocated.\n";
        }

        if (copySuccessful)
        {
            creatures = temporaryCreatures;
            size = other.size;
            name = other.name;
        }
    }
}

// Description: Allocates a pointer array and every Creature for a requested Army.
// Precondition: The requested Army name and size are valid.
// Postcondition: The complete Army replaces the old state or the old state remains unchanged.
void Army::allocateCreatures(const string & armyName, int armySize)
{
    Creature ** temporaryCreatures = nullptr;
    int initializedCount = 0;
    int allocatedCount = 0;
    bool allocationSuccessful = true;

    try
    {
        temporaryCreatures = new Creature *[armySize];

        while (initializedCount < armySize)
        {
            temporaryCreatures[initializedCount] = nullptr;
            initializedCount++;
        }

        while (allocatedCount < armySize)
        {
            string creatureName = "Creature_" + to_string(allocatedCount + 1);
            string creatureType = randomType();

            temporaryCreatures[allocatedCount] = new Creature(creatureName, creatureType);
            allocatedCount++;
        }
    }
    catch (const bad_alloc &)
    {
        allocationSuccessful = false;
        deallocateCreatures(temporaryCreatures, allocatedCount);
        cout << "Army creation failed because memory could not be allocated.\n";
    }

    if (allocationSuccessful)
    {
        deallocateCreatures(creatures, size);
        creatures = temporaryCreatures;
        size = armySize;
        name = armyName;
    }
}

// Description: Releases a Creature pointer array and all allocated Creature objects.
// Precondition: The pointer is either nullptr or points to a DMA pointer array.
// Postcondition: Every owned Creature is deleted, the array is deleted, and the pointer is nullptr.
void Army::deallocateCreatures(Creature ** & creatureList, int creatureCount)
{
    int index = 0;

    if (creatureList != nullptr)
    {
        while (index < creatureCount)
        {
            if (creatureList[index] != nullptr)
            {
                delete creatureList[index];
                creatureList[index] = nullptr;
            }

            index++;
        }

        delete [] creatureList;
        creatureList = nullptr;
    }
}

// Description: Checks whether an Army name has at least three alphabetic characters.
// Precondition: An Army name is supplied.
// Postcondition: True is returned only when the complete name is valid.
bool Army::isValidArmyName(const string & armyName) const
{
    int index = 0;
    int nameLength = static_cast<int>(armyName.length());
    bool isValid = nameLength >= MIN_ARMY_NAME_LENGTH;

    while (index < nameLength && isValid)
    {
        isValid = isalpha(static_cast<unsigned char>(armyName[index])) != 0;
        index++;
    }

    return isValid;
}

// Description: Selects one of four allowed creature types at random.
// Precondition: The random number generator has been seeded.
// Postcondition: One valid creature type is returned.
string Army::randomType() const
{
    int typeNumber = randomNumber(0, CREATURE_TYPE_COUNT - 1);
    string creatureType = DEFAULT_CREATURE_TYPE;

    switch (typeNumber)
    {
        case 0:
            creatureType = "Warrior";
            break;

        case 1:
            creatureType = "Archer";
            break;

        case 2:
            creatureType = "Mage";
            break;

        default:
            creatureType = "Knight";
    }

    return creatureType;
}

// Description: Displays the two required menu options.
// Precondition: The output stream is available.
// Postcondition: The menu and input prompt are displayed.
void displayMenu()
{
    cout << "\n\nArmy DMA Menu:\n"
            "1 Create Army\n"
            "2 Quit\n"
            "Enter your choice: ";
}

// Description: Gets Army input, creates the Army, and prints its creatures.
// Precondition: The user selected the Create Army menu option.
// Postcondition: A complete Army is displayed or an allocation error is reported.
void createArmy()
{
    string armyName = getArmyName();
    int armySize = getArmySize();
    Army army(armyName, armySize);

    if (army.getSize() > 0)
    {
        army.print();
    }
}

// Description: Clears failed input and displays a reusable error message.
// Precondition: The input stream may be failed or contain unwanted characters.
// Postcondition: The input stream is ready for another input operation.
void clearCin(const string & errorMessage)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << errorMessage << '\n';
}

// Description: Gets a valid alphabetic Army name from the user.
// Precondition: The input stream is available.
// Postcondition: An Army name with at least three alphabetic characters is returned.
string getArmyName()
{
    string armyName = DEFAULT_ARMY_NAME;
    bool validName = false;
    int index = 0;
    int nameLength = 0;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do
    {
        cout << "Enter the army name: ";
        getline(cin, armyName);

        nameLength = static_cast<int>(armyName.length());
        validName = nameLength >= MIN_ARMY_NAME_LENGTH;
        index = 0;

        while (index < nameLength && validName)
        {
            validName = isalpha(static_cast<unsigned char>(armyName[index])) != 0;
            index++;
        }

        if (!validName)
        {
            cout << "Enter at least three alphabetic characters.\n";
        }
    } while (!validName);

    return armyName;
}

// Description: Gets a valid requested Army size from the user.
// Precondition: The input stream is available.
// Postcondition: An integer no smaller than the minimum Army size is returned.
int getArmySize()
{
    int armySize = 0;
    bool validSize = false;

    do
    {
        cout << "Enter the number of creatures: ";
        cin >> armySize;

        validSize = !cin.fail() && armySize >= MIN_ARMY_SIZE;

        if (!validSize)
        {
            clearCin("Enter a whole number of at least 5.");
        }
    } while (!validSize);

    return armySize;
}

// Description: Generates a random integer within an inclusive range.
// Precondition: Minimum is less than or equal to maximum.
// Postcondition: One random integer from minimum through maximum is returned.
int randomNumber(int minimum, int maximum)
{
    int randomValue = rand() % (maximum - minimum + 1) + minimum;

    return randomValue;
}

/*
Sample Test Run:
Muhammad Yusuf Rehman
Lab #5 Army DMA


Army DMA Menu:
1 Create Army
2 Quit
Enter your choice: ahh
Invalid menu choice.


Army DMA Menu:
1 Create Army
2 Quit
Enter your choice: 0
Invalid menu choice.


Army DMA Menu:
1 Create Army
2 Quit
Enter your choice: 3
Invalid menu choice.


Army DMA Menu:
1 Create Army
2 Quit
Enter your choice: 1
Enter the army name: AB
Enter at least three alphabetic characters.
Enter the army name: Army1
Enter at least three alphabetic characters.
Enter the army name: Alpha
Enter the number of creatures: a
Enter a whole number of at least 5.
Enter the number of creatures: -5
Enter a whole number of at least 5.
Enter the number of creatures: 0
Enter a whole number of at least 5.
Enter the number of creatures: 4
Enter a whole number of at least 5.
Enter the number of creatures: 5

Army: Alpha

Name                Type               Strength      Health
-----------------------------------------------------------
Creature_1          Warrior                  74         185
Creature_2          Mage                    118         135
Creature_3          Mage                    129         146
Creature_4          Archer                   92         179
Creature_5          Knight                  103         183
Army Alpha destructor in progress
Creature_1 Warrior destructor
Creature_2 Mage destructor
Creature_3 Mage destructor
Creature_4 Archer destructor
Creature_5 Knight destructor


Army DMA Menu:
1 Create Army
2 Quit
Enter your choice: 1
Enter the army name: Alpha
Enter the number of creatures: 5

Army: Alpha

Name                Type               Strength      Health
-----------------------------------------------------------
Creature_1          Warrior                 114          86
Creature_2          Mage                     90          84
Creature_3          Warrior                 186         130
Creature_4          Knight                  157         139
Creature_5          Archer                   75         102
Army Alpha destructor in progress
Creature_1 Warrior destructor
Creature_2 Mage destructor
Creature_3 Warrior destructor
Creature_4 Knight destructor
Creature_5 Archer destructor


Army DMA Menu:
1 Create Army
2 Quit
Enter your choice: 1
Enter the army name: Bravo
Enter the number of creatures: 8

Army: Bravo

Name                Type               Strength      Health
-----------------------------------------------------------
Creature_1          Archer                  165         190
Creature_2          Mage                    119         126
Creature_3          Archer                   97         107
Creature_4          Mage                    197         146
Creature_5          Mage                    189         165
Creature_6          Knight                  165         127
Creature_7          Knight                  163         174
Creature_8          Warrior                  88         159
Army Bravo destructor in progress
Creature_1 Archer destructor
Creature_2 Mage destructor
Creature_3 Archer destructor
Creature_4 Mage destructor
Creature_5 Mage destructor
Creature_6 Knight destructor
Creature_7 Knight destructor
Creature_8 Warrior destructor


Army DMA Menu:
1 Create Army
2 Quit
Enter your choice: 2
Program ended.
*/