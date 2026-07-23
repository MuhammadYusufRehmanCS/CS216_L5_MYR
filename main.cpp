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
// Description: Initializes an empty Army with valid default member values.
// Precondition: An Army object is being created.
// Postcondition: The Army has a default name, zero size, and no allocated memory.
Army::Army()
{
}

// Description: Creates a deep copy of another Army.
// Precondition: rhs contains a valid Army object.
// Postcondition: A complete independent copy is created or a valid empty Army remains.
Army::Army(const Army &rhs)
{
    copyArmy(rhs);
}

// Description: Creates an Army from a supplied name and size.
// Precondition: The supplied name and size may be valid or invalid.
// Postcondition: A complete Army is created or the initialized default values remain unchanged.
Army::Army(const string &armyName, int armySize)
{
    setArmy(armyName, armySize);
}

// Description: Releases all memory owned by an Army.
// Precondition: The Army may own dynamically allocated memory.
// Postcondition: Every allocated Creature and the pointer array are released.
Army::~Army()
{
    cout << "Army " << name << " destructor in progress" << endl;
    deallocateCreatures(ppCreatures, size);
}

// Description: Returns the Army name.
// Precondition: The Army object exists.
// Postcondition: The Army remains unchanged.
string Army::getName() const
{
    return name;
}

// Description: Returns the number of Creatures in the Army.
// Precondition: The Army object exists.
// Postcondition: The Army remains unchanged.
int Army::getSize() const
{
    return size;
}

// Description: Prints the Army name and every Creature in table format.
// Precondition: The Army object contains valid member values.
// Postcondition: The Army is displayed and remains unchanged.
void Army::print() const
{
    cout << "\nArmy: " << name << endl;
    cout << endl;
    cout << left << setw(NAME_WIDTH) << "Name" << setw(TYPE_WIDTH) << "Type" << right << setw(NUMBER_WIDTH) << "Strength" << setw(NUMBER_WIDTH) << "Health" << endl;
    cout << string(TABLE_WIDTH, '-') << endl;

    for (int index = 0; index < size; ++index)
    {
        cout << ppCreatures[index]->to_String() << endl;
    }
}

// Description: Validates and replaces an Army as one complete transaction.
// Precondition: The Army member variables already contain valid initialized values.
// Postcondition: A complete requested Army is stored or no member variable changes.
void Army::setArmy(const string &armyName, int armySize)
{
    Creature **ppNewCreatures = nullptr;

    if (isValidArmyName(armyName) && armySize >= MIN_ARMY_SIZE)
    {
        ppNewCreatures = allocateCreatures(armySize, nullptr);

        if (ppNewCreatures != nullptr)
        {
            deallocateCreatures(ppCreatures, size);
            ppCreatures = ppNewCreatures;
            name = armyName;
            size = armySize;
        }
    }
    else
    {
        cout << INVALID_ARMY_MESSAGE << endl;
    }
}

// Description: Deep-copies another Army while protecting against partial allocation.
// Precondition: rhs contains a valid Army object.
// Postcondition: A complete independent copy is stored or this Army remains valid and empty.
void Army::copyArmy(const Army &rhs)
{
    Creature **ppNewCreatures = nullptr;

    if (rhs.size > 0)
    {
        ppNewCreatures = allocateCreatures(rhs.size, rhs.ppCreatures);

        if (ppNewCreatures != nullptr)
        {
            ppCreatures = ppNewCreatures;
            name = rhs.name;
            size = rhs.size;
        }
    }
}

// Description: Allocates one pointer array and either creates or copies every Creature.
// Precondition: armySize is positive; ppSourceCreatures is nullptr for random Creatures or points to a complete source Army.
// Postcondition: A complete pointer array is returned or all partial memory is released and nullptr is returned.
Creature **Army::allocateCreatures(int armySize, Creature * const *ppSourceCreatures)
{
    Creature **ppNewCreatures = nullptr;
    int allocatedCount = 0;

    try
    {
        ppNewCreatures = new Creature *[armySize]{nullptr};

        for (allocatedCount = 0; allocatedCount < armySize; ++allocatedCount)
        {
            if (ppSourceCreatures == nullptr)
            {
                string creatureName = "Creature_" + to_string(allocatedCount + 1);
                string creatureType = getRandomType();

                ppNewCreatures[allocatedCount] = new Creature(creatureName, creatureType);
            }
            else
            {
                ppNewCreatures[allocatedCount] = new Creature(*ppSourceCreatures[allocatedCount]);
            }
        }

        return ppNewCreatures;
    }
    catch (const bad_alloc &)
    {
        deallocateCreatures(ppNewCreatures, allocatedCount);
        cout << MEMORY_ERROR_MESSAGE << endl;
    }

    return nullptr;
}

// Description: Releases a Creature pointer array and all Creature objects it owns.
// Precondition: ppCreatureList is nullptr or points to a dynamically allocated pointer array.
// Postcondition: All owned memory is released and ppCreatureList is set to nullptr.
void Army::deallocateCreatures(Creature **&ppCreatureList, int creatureCount)
{
    if (ppCreatureList != nullptr)
    {
        for (int index = 0; index < creatureCount; ++index)
        {
            delete ppCreatureList[index];
            ppCreatureList[index] = nullptr;
        }

        delete [] ppCreatureList;
        ppCreatureList = nullptr;
    }
}

// Description: Determines whether an Army name contains at least three alphabetic characters only.
// Precondition: An Army name is supplied.
// Postcondition: True is returned for a valid name and false is returned otherwise.
bool Army::isValidArmyName(const string &armyName) const
{
    bool validName = static_cast<int>(armyName.length()) >= MIN_ARMY_NAME_LENGTH;

    for (int index = 0; index < static_cast<int>(armyName.length()) && validName; ++index)
    {
        validName = isalpha(static_cast<unsigned char>(armyName[index])) != 0;
    }

    return validName;
}

// Description: Returns one randomly selected Creature type.
// Precondition: The random number generator has been seeded.
// Postcondition: One of the four allowed Creature type strings is returned.
string Army::getRandomType() const
{
    CreatureType creatureType = static_cast<CreatureType>(randomNumber(WARRIOR, CREATURE_TYPE_COUNT - 1));
    string typeName = DEFAULT_CREATURE_TYPE;

    switch (creatureType)
    {
        case WARRIOR:
            typeName = "Warrior";
            break;

        case ARCHER:
            typeName = "Archer";
            break;

        case MAGE:
            typeName = "Mage";
            break;

        case KNIGHT:
            typeName = "Knight";
            break;

        default:
            typeName = DEFAULT_CREATURE_TYPE;
    }

    return typeName;
}

// Description: Displays the required Army menu.
// Precondition: The output stream is available.
// Postcondition: The menu and input prompt are displayed.
void displayMenu()
{
    cout << MENU_TEXT;
}

// Description: Gets Army input, creates one Army, and asks the Army to print itself.
// Precondition: The user selected Create Army.
// Postcondition: A complete Army is displayed or an allocation error is reported.
void createArmy()
{
    string armyName = getArmyName();
    int armySize = getArmySize();
    Army army(armyName, armySize);

    army.print();
}

// Description: Clears failed input and displays a reusable error message.
// Precondition: The input stream may be failed or contain unwanted characters.
// Postcondition: The input stream is ready for another input operation.
void clearCin(const string &errorMessage)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << errorMessage << endl;
}

// Description: Gets a valid alphabetic Army name from the user.
// Precondition: The input stream is available.
// Postcondition: An Army name containing at least three alphabetic characters is returned.
string getArmyName()
{
    string armyName = DEFAULT_ARMY_NAME;
    bool validName = false;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do
    {
        cout << "Enter the army name: ";
        getline(cin, armyName);

        validName = static_cast<int>(armyName.length()) >= MIN_ARMY_NAME_LENGTH;

        for (int index = 0; index < static_cast<int>(armyName.length()) && validName; ++index)
        {
            validName = isalpha(static_cast<unsigned char>(armyName[index])) != 0;
        }

        if (!validName)
        {
            cout << "Enter at least three alphabetic characters." << endl;
        }
    } while (!validName);

    return armyName;
}

// Description: Gets a valid requested Army size from the user.
// Precondition: The input stream is available.
// Postcondition: A whole number no smaller than the minimum Army size is returned.
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

// Description: Generates a random integer inside an inclusive range.
// Precondition: minimum is less than or equal to maximum.
// Postcondition: One integer from minimum through maximum is returned.
int randomNumber(int minimum, int maximum)
{
    return rand() % (maximum - minimum + 1) + minimum;
}

/*
CS 216 Lab #5 Army DMA
Muhammad Yusuf Rehman


Army DMA Menu:
1. Create Army
2. Quit
Enter your choice: okay
Invalid menu choice. Select from the menu above.


Army DMA Menu:
1. Create Army
2. Quit
Enter your choice: 0
Invalid menu choice. Select from the menu above.


Army DMA Menu:
1. Create Army
2. Quit
Enter your choice: 3
Invalid menu choice. Select from the menu above.


Army DMA Menu:
1. Create Army
2. Quit
Enter your choice: 1
Enter the army name: AB
Enter at least three alphabetic characters.
Enter the army name: Army1
Enter at least three alphabetic characters.
Enter the army name: Alpha
Enter the number of creatures: a
Enter a whole number of at least 5.
Enter the number of creatures: 5

Army: Alpha

Name                Type               Strength      Health
-----------------------------------------------------------
Creature_1          Knight                  143         117
Creature_2          Mage                    181         197
Creature_3          Knight                   90         125
Creature_4          Archer                   90         170
Creature_5          Mage                    169         178
Army Alpha destructor in progress
Creature_1 Knight destructor
Creature_2 Mage destructor
Creature_3 Knight destructor
Creature_4 Archer destructor
Creature_5 Mage destructor


Army DMA Menu:
1. Create Army
2. Quit
Enter your choice: 1
Enter the army name: Bravo
Enter the number of creatures: 8

Army: Bravo

Name                Type               Strength      Health
-----------------------------------------------------------
Creature_1          Warrior                 141         101
Creature_2          Knight                  163         180
Creature_3          Mage                    172         153
Creature_4          Archer                  185         158
Creature_5          Mage                    189          76
Creature_6          Archer                   82          71
Creature_7          Mage                     80         137
Creature_8          Knight                  143         138
Army Bravo destructor in progress
Creature_1 Warrior destructor
Creature_2 Knight destructor
Creature_3 Mage destructor
Creature_4 Archer destructor
Creature_5 Mage destructor
Creature_6 Archer destructor
Creature_7 Mage destructor
Creature_8 Knight destructor


Army DMA Menu:
1. Create Army
2. Quit
Enter your choice: 2
Program ended.
*/