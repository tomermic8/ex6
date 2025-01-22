#include "ex6.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

# define INT_BUFFER 128
#define zero 0
#define one 1
#define two 2
#define three 3
#define four 4
#define five 5
#define six 6
#define seven 7
#define ten 10
#define onetwo 1.2
#define onefive 1.5


// ================================================
// Basic struct definitions from ex6.h assumed:
//   PokemonData { int id; char *name; PokemonType TYPE; int hp; int attack; EvolutionStatus CAN_EVOLVE; }
//   PokemonNode { PokemonData* data; PokemonNode* left, *right; }
//   OwnerNode   { char* ownerName; PokemonNode* pokedexRoot; OwnerNode *next, *prev; }
//   OwnerNode* ownerHead;
//   const PokemonData pokedex[];
// ================================================

// --------------------------------------------------------------
// 1) Safe integer reading
// --------------------------------------------------------------
// Function that convert enum to string
const char *starterToString(Starter starter) {
    switch (starter) {
        case Bulbasaur: return "Bulbasaur";
        case Charmander: return "Charmander";
        case Squirtle: return "Squirtle";
        default: return "Unknown";
    }
}

const char *getEvolutionStatusName(EvolutionStatus status) {
    return (status == CAN_EVOLVE) ? "Yes" : "No";
}



PokemonNode *search_pokemon(PokemonNode *root, int id) {//get the root and serch pokemon
    while (root->data->id != id) {
        if (root->data->id != id && root->left == NULL && root->right == NULL) {
            printf("we didnt find pokemon");
            return NULL;
        }
        if (root->data->id < id) {
            root = root->right;
        } else {
            root = root->left;
        }
    }

    return root;
}

void fight(PokemonNode *fight1, PokemonNode *fight2) {//get two pokemons and calcaulate their stregth' then fight
    float score1 = (fight1->data->attack * onefive) + (fight1->data->hp * onetwo);
    float score2 = (fight2->data->attack * onefive) + (fight2->data->hp * onetwo  );
    printf("Pokemon 1: %s (Score = %.2f)\n", fight1->data->name, score1);
    printf("Pokemon 2: %s (Score = %.2f)\n", fight2->data->name, score2);
    if (score1 > score2) {
        printf("%s wins!\n", fight1->data->name);
    }
    if (score2 > score1) {
        printf("%s wins!\n", fight2->data->name);
    }
    if (score1 == score2) {
        printf("It's a tie!\n");
    }
}

void preOrderTraversal(PokemonNode *root) {//till 117 its pre in and post order
    preOrderGeneric(root, printPokemonNode);
}

void preOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    if (root == NULL) {
        return;
    }

    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n", root->data->id,
           root->data->name, getTypeName(root->data->TYPE), root->data->hp, root->data->attack,
           getEvolutionStatusName(root->data->CAN_EVOLVE));
    preOrderGeneric(root->left, visit);
    preOrderGeneric(root->right, visit);
}

void inOrderTraversal(PokemonNode *root) {
    inOrderGeneric(root, printPokemonNode);
}

void inOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    if (root == NULL) {
        return;
    }
    inOrderGeneric(root->left, visit);
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n", root->data->id,
           root->data->name, getTypeName(root->data->TYPE), root->data->hp, root->data->attack,
           getEvolutionStatusName(root->data->CAN_EVOLVE));
    inOrderGeneric(root->right, visit);
}

void postOrderTraversal(PokemonNode *root) {
    postOrderGeneric(root, printPokemonNode);
}

void postOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    if (root == NULL) {
        return;
    }
    postOrderGeneric(root->left, visit);
    postOrderGeneric(root->right, visit);
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n", root->data->id,
           root->data->name, getTypeName(root->data->TYPE), root->data->hp, root->data->attack,
           getEvolutionStatusName(root->data->CAN_EVOLVE));
}




PokemonNode *deletePokemonreal(PokemonNode *root, int id) {
    // when empty tree
    if (root == NULL) {
        printf("Pokemon with ID %d not found.\n", id);
        return NULL;
    }

    //search pok by id
    if (id < root->data->id) {
        root->left = deletePokemonreal(root->left, id);
    } else if (id > root->data->id) {
        root->right = deletePokemonreal(root->right, id);
    }
    //we found pok to del
    else {
        printf("Removing Pokemon %s (ID %d).\n", root->data->name, id);

        //case 1 no leaves
        if (root->left == NULL && root->right == NULL) {
            free(root->data->name);
            free(root->data);
            free(root);
            return NULL;
        }
        // case 2 one child
        else if (root->left == NULL) {
            PokemonNode *temp = root->right;
            free(root->data->name);
            free(root->data);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            PokemonNode *temp = root->left;
            free(root->data->name);
            free(root->data);
            free(root);
            return temp;
        }
        //case 3 two children
        else {
           //find sucs
            PokemonNode *successor = root->right;
            while (successor->left != NULL) {
                successor = successor->left;
            }

            free(root->data->name);

            // copy detailes
            root->data->id = successor->data->id;
            root->data->name = successor->data->name;
            root->data->TYPE = successor->data->TYPE;
            root->data->hp = successor->data->hp;
            root->data->attack = successor->data->attack;
            root->data->CAN_EVOLVE = successor->data->CAN_EVOLVE;

            // del sucs
            root->right = deletePokemonreal(root->right, successor->data->id);
        }
    }
    return root;
}


OwnerNode *search_pokadex(OwnerNode *head, char *str) {//search the pokadex be owner name
    OwnerNode *cur = head;
    while (cur->next != head) {
        if (strcmp(str, cur->ownerName) == zero) {
            return cur;
        }
        cur = cur->next;
    }
    if (strcmp(str, cur->ownerName) == zero) {
        return cur;
    }
    return NULL;
}

PokemonNode *addPokemon(PokemonNode *owner, PokemonNode *added) {
    // Base case - if we've reached a NULL node, this is where we insert
    if (owner == NULL) {
        added->left = NULL;
        added->right = NULL;
        printf("Pokemon %s (ID %d) added.", added->data->name, added->data->id);
        return added;
    }
    // If the id of the added pokemon is smaller , go left recursively
    if (added->data->id < owner->data->id) {
        owner->left = addPokemon(owner->left, added);
    }
    // If the id of the added pokemon is bigger , go right recursively
    else if (added->data->id > owner->data->id) {
        owner->right = addPokemon(owner->right, added);
    }
    // Equal id is not allowed
    else {
        printf("Pokemon with ID %d is already in the Pokedex. No changes made.", added->data->id);
        free(added->data);
        free(added);
        return owner;
    }
    return owner;
}

PokemonNode *addPokemon_merge(PokemonNode *owner, PokemonNode *added) {

    if (owner == NULL) {
        added->left = NULL;
        added->right = NULL;
        return added;
    }

    if (added->data->id < owner->data->id) {
        owner->left = addPokemon_merge(owner->left, added);
    }

    else if (added->data->id > owner->data->id) {
        owner->right = addPokemon_merge(owner->right, added);
    }

    else {
        free(added->data);
        free(added);
        return owner;
    }
    return owner;
}

OwnerNode *createOwner(OwnerNode *ownerHead) {
    if (ownerHead == NULL) {
       //make the first node
        OwnerNode *node = (OwnerNode *) malloc(sizeof(OwnerNode));
        if (node == NULL) {
            printf("Failed to allocate memory for PokemonNode\n");
            return NULL;
        }
        node->next = node;
        node->prev = node;
        printf("Your name: ");
        node->ownerName = getDynamicInput();
        if (node->ownerName == NULL) {
            free(node);
            return NULL;
        }
        printf("Choose Starter:\n1. Bulbasaur\n2. Charmander\n3. Squirtle\n");

        int selection=zero;
        selection = readIntSafe("Your choice: ");

        printf("New Pokedex created for %s with starter %s.", node->ownerName, starterToString(selection));
        //מדפיס מה שצריך
        node->pokedexRoot = malloc(sizeof(PokemonNode)); //create bin tree
        if (!node->pokedexRoot) {
            free(node->ownerName);
            free(node);
            return NULL;
        }
        // aloccate values
        node->pokedexRoot->left = NULL;
        node->pokedexRoot->right = NULL;
        node->pokedexRoot->data = malloc(sizeof(PokemonData));
        if (!node->pokedexRoot->data) {
            free(node->pokedexRoot);
            free(node->ownerName);
            free(node);
            return NULL;
        }

        //get pok by what user picked
        if (selection == two) {
            selection = four;
        }
        if (selection == three) {
            selection = seven;
        }

        //give values
        node->pokedexRoot->data->id = pokedex[selection - one].id;
        node->pokedexRoot->data->name = pokedex[selection - one].name;
        node->pokedexRoot->data->attack = pokedex[selection - one].attack;
        node->pokedexRoot->data->hp = pokedex[selection - one].hp;
        node->pokedexRoot->data->TYPE = pokedex[selection - one].TYPE;
        node->pokedexRoot->data->CAN_EVOLVE = pokedex[selection - one].CAN_EVOLVE;
        ownerHead = node;
        return ownerHead;
    }

    printf("Your name: ");

    char *str = getDynamicInput();
    OwnerNode *check = search_pokadex(ownerHead, str);
    if (check != NULL) {
        printf("Owner '%s' already exists. Not creating a new Pokedex.", str);
        free(str);
        return ownerHead;
    }


    OwnerNode *temp = ownerHead->next; //else when we have already one pokadex at least. we will do the same jist its not the first place
    while (temp->next != ownerHead) {
        temp = temp->next;
    }
    temp->next = malloc(sizeof(OwnerNode));
    if (!temp->next) {
        printf("Failed to allocate memory for PokemonNode\n");
        free(str);
        return NULL;
    }
    //place for new node
    temp->next->prev = temp;
    temp->next->next = ownerHead;
    ownerHead->prev = temp->next;


    temp = temp->next; //point the last

    temp->ownerName = str;

    printf("Choose Starter:\n1. Bulbasaur\n2. Charmander\n3. Squirtle\n");
    int selection=zero;
    selection = readIntSafe("Your choice: ");

    printf("New Pokedex created for %s with starter %s.", temp->ownerName, starterToString(selection));

    temp->pokedexRoot = malloc(sizeof(PokemonNode));
    if (!temp->pokedexRoot) {
        free(temp->ownerName);
        free(temp);

        return NULL;
    }

    temp->pokedexRoot->left = NULL;
    temp->pokedexRoot->right = NULL;
    temp->pokedexRoot->data = malloc(sizeof(PokemonData));
    if (!temp->pokedexRoot->data) {
        free(temp->pokedexRoot);
        free(temp->ownerName);
        free(temp);

        return NULL;
    }


    if (selection == two) {
        selection = four;
    }
    if (selection == three) {
        selection = seven;
    }


    temp->pokedexRoot->data->id = pokedex[selection - one].id;
    temp->pokedexRoot->data->name = pokedex[selection - one].name;
    temp->pokedexRoot->data->attack = pokedex[selection - one].attack;
    temp->pokedexRoot->data->hp = pokedex[selection - one].hp;
    temp->pokedexRoot->data->TYPE = pokedex[selection - one].TYPE;
    temp->pokedexRoot->data->CAN_EVOLVE = pokedex[selection - one].CAN_EVOLVE;

    temp = ownerHead;

    return ownerHead;
}


void trimWhitespace(char *str) {
    // Remove leading spaces/tabs/\r
    int start = zero;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > zero) {
        int idx = zero;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int) strlen(str);
    while (len > zero && (str[len - one] == ' ' || str[len - one] == '\t' || str[len - one] == '\r')) {
        str[--len] = '\0';
    }
}

char *myStrdup(const char *src) {
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char *dest = (char *) malloc(len + one);
    if (!dest) {
        printf("Memory allocation failed in myStrdup.\n");
        return NULL;
    }
    strcpy(dest, src);
    return dest;
}

int readIntSafe(const char *prompt) {
    char buffer[INT_BUFFER];
    int value;
    int success = zero;

    while (!success) {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0) {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char *endptr;
        value = (int) strtol(buffer, &endptr, ten);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0') {
            printf("Invalid input.\n");
        } else {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

// --------------------------------------------------------------
// 2) Utility: Get type name from enum
// --------------------------------------------------------------
const char *getTypeName(PokemonType type) {
    switch (type) {
        case GRASS:
            return "GRASS";
        case FIRE:
            return "FIRE";
        case WATER:
            return "WATER";
        case BUG:
            return "BUG";
        case NORMAL:
            return "NORMAL";
        case POISON:
            return "POISON";
        case ELECTRIC:
            return "ELECTRIC";
        case GROUND:
            return "GROUND";
        case FAIRY:
            return "FAIRY";
        case FIGHTING:
            return "FIGHTING";
        case PSYCHIC:
            return "PSYCHIC";
        case ROCK:
            return "ROCK";
        case GHOST:
            return "GHOST";
        case DRAGON:
            return "DRAGON";
        case ICE:
            return "ICE";
        default:
            return "UNKNOWN";
    }
}

// --------------------------------------------------------------
// Utility: getDynamicInput (for reading a line into malloc'd memory)
// --------------------------------------------------------------
char *getDynamicInput() {
    char *input = NULL;
    size_t size = 0, capacity = 1;
    input = (char *) malloc(capacity);
    if (!input) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (size + 1 >= capacity) {
            capacity *= 2;
            char *temp = (char *) realloc(input, capacity);
            if (!temp) {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char) c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}

// Function to print a single Pokemon node
void printPokemonNode(PokemonNode *node) {
    if (!node)
        return;
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           node->data->id,
           node->data->name,
           getTypeName(node->data->TYPE),
           node->data->hp,
           node->data->attack,
           (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
}

// --------------------------------------------------------------
// Display Menu
// --------------------------------------------------------------
void displayMenu(OwnerNode *owner) {//present the menu for display
    int choice=zero;
    if (!owner->pokedexRoot) {
        printf("Pokedex is empty.\n");
        return;
    }

    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");


    choice= readIntSafe("Your choice: ");


    switch (choice) {
        case one:
            displayBFS(owner->pokedexRoot);

            break;
        case two:
            preOrderTraversal(owner->pokedexRoot);
            break;
        case three:
            inOrderTraversal(owner->pokedexRoot);
            break;
        case four:
            postOrderTraversal(owner->pokedexRoot);
            break;
        case five:
            displayAlphabetical(owner->pokedexRoot);

            break;
        default:
            printf("Invalid choice.\n");
    }
}

// --------------------------------------------------------------
// Sub-menu for existing Pokedex
// --------------------------------------------------------------

void add_details(PokemonNode *add, int id) {//add detailes for pok
    add->data = malloc(sizeof(PokemonData));
    add->left = NULL; // חסר
    add->right = NULL; // חסר
    add->data->id = pokedex[id - one].id;
    add->data->name = pokedex[id - one].name;
    add->data->hp = pokedex[id - one].hp;
    add->data->attack = pokedex[id - one].attack;
    add->data->TYPE = pokedex[id - one].TYPE;
    add->data->CAN_EVOLVE = pokedex[id - one].CAN_EVOLVE;
}

void enterExistingPokedexMenu() {
    OwnerNode *cur = ownerHead; //point the head
    int size_of_owner = one; //count the owners


    // list owners
    printf("\nExisting Pokedexes:\n");
    // you need to implement a few things here :)

    while (cur->next != ownerHead) {//count owners and print

        printf("%d. %s\n", size_of_owner, cur->ownerName);
        size_of_owner++;
        cur = cur->next;
    }
    printf("%d. %s\n", size_of_owner, cur->ownerName);

    cur = ownerHead;
    int num_of_node;
    num_of_node = readIntSafe("Choose a Pokedex by number: ");

    for (int i = one; i < num_of_node; i++) {//cur will point the pokadex we chose

        cur = cur->next;
    }

    printf("\nEntering %s's Pokedex...\n", cur->ownerName);
    int search_fight1;
    int search_fight2;
    int subChoice;
    int pokemon_evolve;
    int idb;
    int id;
    do {
        printf("\n-- %s's Pokedex Menu --\n", cur->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");


        subChoice = readIntSafe("Your choice: ");


        switch (subChoice) {
            case one:
                PokemonNode *add = (PokemonNode *) malloc(sizeof(PokemonNode)); //point the root
                if (add == NULL) {
                    printf("Memory allocation failed.\n");
                    break;
                }


            id = readIntSafe("Enter ID to add: ");
                add_details(add, id);
                cur->pokedexRoot = addPokemon(cur->pokedexRoot, add);

                break;
            case two:
                if (cur->pokedexRoot == NULL) {//for case of empty pokadex
                    printf("Pokedex is empty.\n");
                    break;
                }
                displayMenu(cur);


                break;
            case three:

                if (cur->pokedexRoot == NULL) {
                    printf("No Pokemon to release.\n");
                    break;
                }

            idb = readIntSafe("Enter Pokemon ID to release: ");


                cur->pokedexRoot = deletePokemonreal(cur->pokedexRoot, idb);
                break;
            case four:
                if (cur->pokedexRoot == NULL) {
                    printf("Pokedex is empty.\n");
                    break;
                }



            search_fight1 = readIntSafe("Enter ID of the first Pokemon: ");
            search_fight2 = readIntSafe("Enter ID of the second Pokemon: ");


                PokemonNode *serch_1 = search_pokemon(cur->pokedexRoot, search_fight1);
                PokemonNode *serch_2 = search_pokemon(cur->pokedexRoot, search_fight2);

                if (serch_1 == NULL || serch_2 == NULL) {//when we didnt find 1 pok or 2
                    printf("Pokemon not found.\n");
                } else {
                    fight(serch_1, serch_2);
                }


                break;
            case five:
                if (cur->pokedexRoot == NULL) {
                    printf("Cannot evolve. Pokedex empty.\n");
                    break;
                }


            pokemon_evolve = readIntSafe("Enter ID of Pokemon to evolve: ");

                PokemonNode *evolve = search_pokemon(cur->pokedexRoot, pokemon_evolve); //מחפש את הפוקימון שרוצים לפתח
                if (evolve == NULL) {
                    printf("No Pokemon with ID %d found.\n", pokemon_evolve);
                } else {
                    if (evolve->data->CAN_EVOLVE == CAN_EVOLVE) {
                        //check if can evolve
                        PokemonNode *next_evolve = search_pokemon(cur->pokedexRoot, pokemon_evolve + one);
                        //checl if evolve vers is in the tree
                        if (next_evolve != NULL) {
                            //so delete
                            cur->pokedexRoot = deletePokemonreal(cur->pokedexRoot, pokemon_evolve);
                            printf(
                                "Evolution ID %d (%s) already in the Pokedex. Releasing %s (ID %d).Removing Pokemon %s (ID %d)."
                                , pokedex[pokemon_evolve].id, pokedex[pokemon_evolve].name,
                                pokedex[pokemon_evolve - one].name, pokedex[pokemon_evolve - one].id,
                                pokedex[pokemon_evolve - one].name, pokedex[pokemon_evolve - one].id);
                        } else {
                            //when no evolve vers found
                            printf("Pokemon evolved from %s (ID %d) ", evolve->data->name, evolve->data->id);
                            evolve->data->attack = pokedex[pokemon_evolve].attack;
                            evolve->data->TYPE = pokedex[pokemon_evolve].TYPE;
                            evolve->data->CAN_EVOLVE = pokedex[pokemon_evolve].CAN_EVOLVE;
                            evolve->data->id = pokedex[pokemon_evolve].id;
                            evolve->data->hp = pokedex[pokemon_evolve].hp;
                            evolve->data->name = pokedex[pokemon_evolve].name;
                            printf("to %s (ID %d).", evolve->data->name, evolve->data->id);
                        }
                    } else {
                        printf("%s (ID %d) cannot evolve.\n", evolve->data->name, evolve->data->id);
                    }
                }
                break;
            case 6:
                printf("Back to Main Menu.\n");
                subChoice = 6;
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (subChoice != 6);
}

OwnerNode *deletepokadex(OwnerNode *ownerHead) {

    OwnerNode *cur = ownerHead;
    int size_of_owner = one;

    // list owners
    printf("\n=== Delete a Pokedex ===\n");
    // you need to implement a few things here :)

    while (cur->next != ownerHead) {

        printf("%d. %s\n", size_of_owner, cur->ownerName); //print the poxadex
        size_of_owner++;
        cur = cur->next;
    }
    printf("%d. %s\n", size_of_owner, cur->ownerName);

    cur = ownerHead;


    int num_of_node = readIntSafe("Choose a Pokedex to delete by number: ");

    for (int i = one; i < num_of_node; i++) {//point the pokadex we want

        cur = cur->next;
    }
    printf("Deleting %s's entire Pokedex...\nPokedex deleted.\n",cur->ownerName);
    if (cur == ownerHead) {
        //if we want to del the first pokadex and there is 1

        if (ownerHead->next == ownerHead && ownerHead->prev == ownerHead) {
            cur->pokedexRoot = delete_t(cur->pokedexRoot);
            free(cur->ownerName);
            free(cur);
            return NULL;
        } else {
            ownerHead = ownerHead->next;
            cur->pokedexRoot = delete_t(cur->pokedexRoot);
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            free(cur->ownerName);
            free(cur);
            return ownerHead;
        }
    } else {
        //כשלא רוצים למחוק את הראשון
        cur->pokedexRoot = delete_t(cur->pokedexRoot);
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        free(cur->ownerName);
        free(cur);
        return ownerHead;
    }
}

PokemonNode *delete_t(PokemonNode *root) {//delete the tree of pokadex we chose
    if (root == NULL) {
        return NULL;
    }

    root->left = delete_t(root->left);
    root->right = delete_t(root->right);

    free(root->data->name);
    free(root->data);
    free(root);

    return NULL;
}

OwnerNode *sortbyowners(OwnerNode *head) {
    OwnerNode *cur = head;
    OwnerNode *sort = head;
    int size_of_owner = one;

    while (cur->next != head) {
        size_of_owner++;
        cur = cur->next;
    }
    cur = cur->next;
    OwnerNode *min = head;

    while (cur->next != head) {//search for the abc min owner

        if (strcmp(min->ownerName, cur->next->ownerName) > zero) {
            //אם הקר קטן יותר
            min = cur->next;
        }
        cur = cur->next;
    }

    sort = min;
    cur = min->next; //connect the new list to min tnd siconect from current list
    sort->prev->next = sort->next;
    sort->next->prev = sort->prev;
    sort->next = sort; //
    sort->prev = sort;
//that was all to find the dirst owner now we will run till one before the last

    OwnerNode *headsort = sort;

    for (int i = one; i <= size_of_owner - two; i++) {

        min = cur;
        head = cur;
        while (head != cur->next) {

            if (strcmp(min->ownerName, cur->next->ownerName) > zero) {

                min = cur->next;
            }
            cur = cur->next;
        }

        if (strcmp(min->ownerName, cur->ownerName) > zero) {

            min = cur;
        }

//we will connect the node to the new list and disconnect from current list

        cur = min->next;
        min->next->prev = min->prev;
        min->prev->next = min->next;
        sort->next = min;
        min->prev = sort;
        sort = sort->next;
    }//now we finished so close the new list so head poind last and opos
    sort->next = cur;
    cur->prev = sort;
    cur->next = headsort;
    headsort->prev = cur;

    return headsort;
}


NodeArray *createQueue(int initialCapacity) {
    // create the queue
    NodeArray *queue = (NodeArray *) malloc(sizeof(NodeArray));
    if (queue == NULL) {
        exit(one);
    }
    // Create a starting size for the queue
    queue->nodes = (PokemonNode **) malloc(initialCapacity * sizeof(PokemonNode *)); //שיניתי
    if (queue->nodes == NULL) {
        exit(one);
    }
    // initialize the queue's data
    queue->capacity = initialCapacity;
    queue->front = zero;
    queue->rear = zero;
    queue->size = zero;
    return queue;
}

void reallocTheQueue(NodeArray *queue) {
    // Pointer to the array
    PokemonNode **old_nodes = queue->nodes;
    // Double the capacity
    queue->capacity = queue->capacity * two;
    // Create a bigger array
    queue->nodes = (PokemonNode **) malloc(queue->capacity * sizeof(PokemonNode *));
    if (queue->nodes == NULL) {
        exit(one);
    }
    // Copy the pokemons from the old array
    for (int i = queue->front; i < queue->rear; i++) {
        queue->nodes[i] = old_nodes[i];
    }
    // Free the pointer to the old array
    free(old_nodes);
}

void addNode(NodeArray *array, PokemonNode *node) {
    // Check if the array is full
    if (array->size == array->capacity) {
        // Double the capacity of the array
        array->capacity *= two;
        array->nodes = realloc(array->nodes, array->capacity * sizeof(PokemonNode *));
        if (!array->nodes) {
            exit(one);
        }
    }

    // Add the node to the array and increase its size by 1
    array->nodes[array->size++] = node;
}


void getInTheQueue(NodeArray *queue, PokemonNode *node) {
    // Check if the queue is full
    if (queue->front == queue->rear) {
        reallocTheQueue(queue);
    }
    // Get the pokemon to the end of the queue
    queue->nodes[queue->rear] = node;
    queue->rear++;
    queue->size++;
}

PokemonNode *getOutFromQueue(NodeArray *queue) {
    // Check if the queue is empty
    if (queue->size == zero) {
        return NULL;
    }
    // Set a pointer to the start of the queue- the pokemon we want to remove by FIFO
    PokemonNode *node = queue->nodes[queue->front];
    // Now the front of the line is the pokemon that comes after the one we want to remove
    queue->front++;
    // Queue's size reduces by 1 because it is now starting from a different position
    queue->size--;
    return node;
}

void displayBFS(PokemonNode *root) {
    // First, check that the root is not empty
    if (root == NULL) {
        return;
    }
    // Create the queue with a starting capacity of 10 pokemons
    NodeArray *queue = createQueue(ten);
    // Get the root in the queue
    getInTheQueue(queue, root);


    while (queue->size != zero) {
        // Get pokemon out of the queue and print its details
        PokemonNode *cnode = getOutFromQueue(queue);
        printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
               cnode->data->id,
               cnode->data->name,
               getTypeName(cnode->data->TYPE),
               cnode->data->hp,
               cnode->data->attack,
               getEvolutionStatusName(cnode->data->CAN_EVOLVE));
        // If the pokemon we removed have childrens, get them in the queue
        if (cnode->left != NULL) {
            getInTheQueue(queue, cnode->left);
        }
        if (cnode->right != NULL) {
            getInTheQueue(queue, cnode->right);
        }
    }
    // Free all the dynamic allocated structs and pointers
    free(queue->nodes);
    free(queue);
}

void mergepokadex(PokemonNode *roottoremove, PokemonNode *roottoadd) {//do the same of bfs fiest but not print just add every pok to second pokadex

    if (roottoremove == NULL) {
        return;
    }

    NodeArray *queue = createQueue(ten);

    getInTheQueue(queue, roottoremove);


    while (queue->size != zero) {

        PokemonNode *cnode = getOutFromQueue(queue);

        PokemonNode *left = cnode->left;
        PokemonNode *right = cnode->right;

        cnode->left = NULL;
        cnode->right = NULL;

        addPokemon_merge(roottoadd, cnode);//add the pokemon from q to the first pokadex

        if (left != NULL) {
            getInTheQueue(queue, left);
        }
        if (right != NULL) {
            getInTheQueue(queue, right);
        }
    }



    free(queue->nodes);
    free(queue);
    roottoremove = NULL;
}


void print_foward(OwnerNode *root, int times) {//when wwe want to print f
    OwnerNode *cur = root;
    for (int i = zero; i < times; i++) {
        printf("[%d] %s\n", i + one, cur->ownerName);
        cur = cur->next;
    }
}

void printbackwards(OwnerNode *root, int times) {//when we want to print b
    OwnerNode *cur = root;
    for (int i = zero; i < times; i++) {
        printf("[%d] %s\n", i + one, cur->ownerName);
        cur = cur->prev;
    }
}

void collectAll(PokemonNode *root, NodeArray *array) {//for display alph
    if (root == NULL) {
        return;
    }
    collectAll(root->left, array);
    addNode(array, root);
    collectAll(root->right, array);
}

int compareByNameNode(const void *a, const void *b) {
    PokemonNode *nodeA = *(PokemonNode **) a;
    PokemonNode *nodeB = *(PokemonNode **) b;
    return strcmp(nodeA->data->name, nodeB->data->name);
}

void displayAlphabetical(PokemonNode *root) {
    // check if the tree is empty
    if (root == NULL) {
        return;
    }
    // Create QUEUE for the alphabetical sort
    NodeArray *array = createQueue(ten);
    // Get the all the pokemons into the array
    collectAll(root, array);

    // compare the data by alphabetic sort with qsort and the function: compareByName
    qsort(array->nodes, array->size, sizeof(PokemonNode *), compareByNameNode);

    // Print the data
    for (int i = zero; i < array->size; i++) {
        PokemonNode *node = array->nodes[i];
        printPokemonNode(node);
    }

    // Free dynamic allocated memory
    free(array->nodes);
    free(array);
}

void freeall(OwnerNode *head) {
    if (head == NULL) {
        return; // no need to release
    }

    OwnerNode *current = head;
    while (current->next != head) {
        //delete all bin trees
        delete_t(current->pokedexRoot);
        current = current->next;
    }
    delete_t(current->pokedexRoot); //delete first bin tree
    current = head;
    OwnerNode *next = head;
    while (current->next != head) {
        next = current->next;
        free(current->ownerName);
        free(current);
        current = next;
    }
    free(current->ownerName);
    free(current);
}

// --------------------------------------------------------------
// Main Menu
// --------------------------------------------------------------
void mainMenu() {
    int choice123;
    char *merge1;
    char *merge2;
    char *back_or_foward;
    int times;
    do {


        printf("\n=== Main Menu ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. Exit\n");

        choice123 = readIntSafe("Your choice: ");
        switch (choice123) {
            case one:

                ownerHead = createOwner(ownerHead);
                break;
            case two:
                if (ownerHead == NULL) {
                    printf("No existing Pokedexes.\n");
                } else {
                    enterExistingPokedexMenu();
                }

                break;
            case three:

                if (ownerHead == NULL) {
                    printf("No existing Pokedexes to delete.\n");
                } else {
                    ownerHead = deletepokadex(ownerHead);
                }
                break;
            case four:
                if (ownerHead == NULL || ownerHead->next == ownerHead) {//when no enough pokadexes to merge
                    printf("Not enough owners to merge.\n");
                } else {
                    printf("\n=== Merge Pokedexes ===\nEnter name of first owner: ");
                    merge1 = getDynamicInput();
                    OwnerNode *first_merge = search_pokadex(ownerHead, merge1);
                    printf("Enter name of second owner: ");
                    merge2 = getDynamicInput();
                    OwnerNode *second_merge = search_pokadex(ownerHead, merge2);
                    if (first_merge != NULL && second_merge != NULL) {//if we found pokadexes to merge
                        mergepokadex(second_merge->pokedexRoot, first_merge->pokedexRoot);
                        printf("Merging %s and %s...\nMerge completed.\nOwner '%s' has been removed after merging.",
                               first_merge->ownerName
                               , second_merge->ownerName, second_merge->ownerName);

                        if (second_merge == ownerHead) {
                            ownerHead = ownerHead->next;
                        }


                        second_merge->prev->next = second_merge->next;
                        second_merge->next->prev = second_merge->prev;


                        free(merge1);
                        free(merge2);

                        free(second_merge->ownerName);
                        free(second_merge);
                    } else {//if we didnt find
                        free(merge1);
                        free(merge2);
                        printf("One or both owners not found.");
                    }
                }

                break;
            case five:

                if (ownerHead == NULL || ownerHead->next == ownerHead) {

                    printf("0 or 1 owners only => no need to sort.\n");
                    break;
                } else {
                    ownerHead = sortbyowners(ownerHead);
                    printf("Owners sorted by name.\n");
                }
                ownerHead = sortbyowners(ownerHead);
                break;
            case six:
                if (ownerHead == NULL) {

                    printf("No owners.\n");
                    break;
                }

                printf("Enter direction (F or B): ");
                back_or_foward = getDynamicInput();



            times = readIntSafe("How many prints? ");

                if (strcmp(back_or_foward, "F") == zero || strcmp(back_or_foward, "f") == zero) {
                    print_foward(ownerHead, times);
                }
                if (strcmp(back_or_foward, "B") == zero || strcmp(back_or_foward, "b") == zero) {
                    printbackwards(ownerHead, times);
                }
            free(back_or_foward);
                break;
            case seven:
                freeall(ownerHead);
                ownerHead = NULL;
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid.\n");
        }
    } while (choice123 != seven);
}


int main() {
    ownerHead = NULL;
    mainMenu();


    return zero;
}
