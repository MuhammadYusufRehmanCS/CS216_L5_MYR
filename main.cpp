/*
    CS 216 Lab #5 Army DMA
    Muhammad Yusuf Rehman
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

const string LAB_NAME = "CS 216 Lab #5 Army DMA";
const string STUDENT_NAME = "Muhammad Yusuf Rehman";
const string DEFAULT_CREATURE_NAME = "n/a";
const string DEFAULT_CREATURE_TYPE = "Warrior";
const string DEFAULT_ARMY_NAME = "No Army";
const string INVALID_MENU_MESSAGE = "Invalid menu choice. Select from the menu above.";
const string INVALID_ARMY_MESSAGE = "Invalid army name or size. Default values remain unchanged.";
const string INVALID_CREATURE_MESSAGE = "Invalid creature data. Default values remain unchanged.";
const string MEMORY_ERROR_MESSAGE = "The army could not be created because memory allocation failed.";
const string MENU_TEXT = "\n\nArmy DMA Menu:\n1. Create Army\n2. Quit\nEnter your choice: ";

const int MIN_STAT = 60;
const int MAX_STAT = 200;
const int MIN_ARMY_SIZE = 5;
const int MIN_ARMY_NAME_LENGTH = 3;
const int NAME_WIDTH = 20;
const int TYPE_WIDTH = 15;
const int NUMBER_WIDTH = 12;
const int TABLE_WIDTH = NAME_WIDTH + TYPE_WIDTH + NUMBER_WIDTH + NUMBER_WIDTH;

enum MenuChoice
{
    CREATE_ARMY = 1,
    QUIT
};

enum CreatureType
{
    WARRIOR,
    ARCHER,
    MAGE,
    KNIGHT,
    CREATURE_TYPE_COUNT
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
    Creature(const Creature &rhs);
    Creature(const string &creatureName, const string &creatureType);
    Creature(const string &creatureName, const string &creatureType, int creatureStrength, int creatureHealth);
    ~Creature();

    void setName(const string &creatureName);
    void setType(const string &creatureType);
    void setStrength(int creatureStrength);
    void setHealth(int creatureHealth);
    void setCreature(const string &creatureName, const string &creatureType, int creatureStrength, int creatureHealth);

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
    Creature **ppCreatures = nullptr;

public:
    Army();
    Army(const Army &rhs);
    Army(const string &armyName, int armySize);
    ~Army();

    string getName() const;
    int getSize() const;
    void print() const;

private:
    void setArmy(const string &armyName, int armySize);
    void copyArmy(const Army &rhs);
    Creature **allocateCreatures(int armySize, Creature * const *ppSourceCreatures);
    void deallocateCreatures(Creature **&ppCreatureList, int creatureCount);
    bool isValidArmyName(const string &armyName) const;
    string getRandomType() const;
};

void displayMenu();
void createArmy();
void clearCin(const string &errorMessage);
string getArmyName();
int getArmySize();
int randomNumber(int minimum, int maximum);

int main()
{
    int menuChoice = 0;

    srand(static_cast<unsigned int>(time(nullptr)));

    cout << LAB_NAME << endl;
    cout << STUDENT_NAME << endl;

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
                cout << "Program ended." << endl;
                break;

            default:
                clearCin(INVALID_MENU_MESSAGE);
        }
    } while (menuChoice != QUIT);

    return 0;
}

// Description: Initializes one Creature with the required default values.
// Precondition: A Creature object is being created.
// Postcondition: The Creature contains valid default values.
Creature::Creature()
{
    setCreature(DEFAULT_CREATURE_NAME, DEFAULT_CREATURE_TYPE, MIN_STAT, MIN_STAT);
}

// Description: Creates a Creature as a copy of another Creature.
// Precondition: rhs contains a valid Creature object.
// Postcondition: The new Creature contains the same values as rhs.
Creature::Creature(const Creature &rhs)
{
    setCreature(rhs.name, rhs.type, rhs.strength, rhs.health);
}

// Description: Initializes a Creature with a name, type, and random statistics.
// Precondition: The random number generator has been seeded.
// Postcondition: The Creature contains the supplied name and type with valid random statistics.
Creature::Creature(const string &creatureName, const string &creatureType)
{
    int creatureStrength = randomNumber(MIN_STAT, MAX_STAT);
    int creatureHealth = randomNumber(MIN_STAT, MAX_STAT);

    setCreature(creatureName, creatureType, creatureStrength, creatureHealth);
}

// Description: Initializes a Creature with four supplied values.
// Precondition: The supplied values may be valid or invalid.
// Postcondition: Valid values are stored together or the initialized default values remain unchanged.
Creature::Creature(const string &creatureName, const string &creatureType, int creatureStrength, int creatureHealth)
{
    setCreature(creatureName, creatureType, creatureStrength, creatureHealth);
}

// Description: Displays a message when a Creature is destroyed.
// Precondition: A Creature object has reached the end of its lifetime.
// Postcondition: The Creature object is destroyed.
Creature::~Creature()
{
    cout << name << " " << type << " destructor" << endl;
}

// Description: Attempts to change the Creature name.
// Precondition: The Creature object contains valid values.
// Postcondition: The name changes or the complete Creature remains unchanged.
void Creature::setName(const string &creatureName)
{
    setCreature(creatureName, type, strength, health);
}

// Description: Attempts to change the Creature type.
// Precondition: The Creature object contains valid values.
// Postcondition: The type changes or the complete Creature remains unchanged.
void Creature::setType(const string &creatureType)
{
    setCreature(name, creatureType, strength, health);
}

// Description: Attempts to change the Creature strength.
// Precondition: The Creature object contains valid values.
// Postcondition: Strength changes when valid or the complete Creature remains unchanged.
void Creature::setStrength(int creatureStrength)
{
    setCreature(name, type, creatureStrength, health);
}

// Description: Attempts to change the Creature health.
// Precondition: The Creature object contains valid values.
// Postcondition: Health changes when valid or the complete Creature remains unchanged.
void Creature::setHealth(int creatureHealth)
{
    setCreature(name, type, strength, creatureHealth);
}

// Description: Validates and sets all Creature member variables as one complete record.
// Precondition: The Creature member variables already contain valid initialized values.
// Postcondition: All supplied values are stored together or no member variable changes.
void Creature::setCreature(const string &creatureName, const string &creatureType, int creatureStrength, int creatureHealth)
{
    if (isValidStat(creatureStrength) && isValidStat(creatureHealth))
    {
        name = creatureName;
        type = creatureType;
        strength = creatureStrength;
        health = creatureHealth;
    }
    else
    {
        cout << INVALID_CREATURE_MESSAGE << endl;
    }
}

// Description: Returns the Creature name.
// Precondition: The Creature object exists.
// Postcondition: The Creature remains unchanged.
string Creature::getName() const
{
    return name;
}

// Description: Returns the Creature type.
// Precondition: The Creature object exists.
// Postcondition: The Creature remains unchanged.
string Creature::getType() const
{
    return type;
}

// Description: Returns the Creature strength.
// Precondition: The Creature object exists.
// Postcondition: The Creature remains unchanged.
int Creature::getStrength() const
{
    return strength;
}

// Description: Returns the Creature health.
// Precondition: The Creature object exists.
// Postcondition: The Creature remains unchanged.
int Creature::getHealth() const
{
    return health;
}

// Description: Returns one Creature as a formatted table row.
// Precondition: The Creature object contains valid values.
// Postcondition: A formatted string is returned and the Creature remains unchanged.
string Creature::to_String() const
{
    stringstream output;

    output << left << setw(NAME_WIDTH) << name << setw(TYPE_WIDTH) << type << right << setw(NUMBER_WIDTH) << strength << setw(NUMBER_WIDTH) << health;

    return output.str();
}

// Description: Determines whether a statistic is inside the required range.
// Precondition: A statistic value is supplied.
// Postcondition: True is returned for a valid statistic and false is returned otherwise.
bool Creature::isValidStat(int value) const
{
    return value >= MIN_STAT && value <= MAX_STAT;
}