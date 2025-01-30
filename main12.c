#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define ai_DICE 5  // Number of dice to roll
#define N_DICE 5 //number of dice
#define N_CATEGORIES 13 //number of categories

// Define categories with numbers
#define ONES 1
#define TWOS 2
#define THREES 3
#define FOURS 4
#define FIVES 5
#define SIXES 6
#define THREE_OF_KIND 7
#define FOUR_OF_KIND 8
#define FULL_HOUSE 9
#define SMALL_STRAIGHT 10
#define LARGE_STRAIGHT 11
#define YAHTZEE 12
#define CHANCE 13
#define UPPER_SCORE 14
#define UPPER_BONUS 15
#define LOWER_SCORE 16
#define TOTAL 17

//ai Function prototypes
void ai_rollDice(int aidiceRoll[ai_DICE]);
void ai_displayDice(int aidiceRoll[ai_DICE]);
void ai_keepMaxValues(int aidiceRoll[ai_DICE], int aikeptDice[ai_DICE], int *aikeptCount);
void ai_selectCategory(int aikeptDice[ai_DICE], int aikeptCount, int aiplayer, int aidiceRoll[ai_DICE], int aiscorecard[15][2], char aiplayerNames[2][20]);
void ai_computerTurn(int aiplayer, int aiscorecard[15][2], char aiplayerNames[2][20]);
void ai_calculateCategoryScore(int aicategory, int aiplayer, int aidiceRoll[ai_DICE], int aiscorecard[15][2], char aiplayerNames[2][20]);
int ai_checkSpecialCategory(int aicategory , int aidiceRoll[ai_DICE]);
void ai_displayScorecard(int aiplayer, int aiscorecard[15][2], char aiplayerNames[2][20],int *aitotalScore);
// Function to display the total score
void aidisplayTotalScore(int *aitotalScore) ;
void ai_startroall();
//Global variables(ai player)
int aitotalScore;
int aiusedCategories[15] = {0}; // Array to track used categories, initialized to 0
//................................................................................................

//humen function prototypes
void GamePlay();
void startFirstDiceRoll(int player);
void secondAndThirdRoll(int player);
void selectCategory(int player);
int checkCategory(int player, int category);
void calculateCategoryScore(int player, int category);
int checkSpecialCategory(int category);
void totalUpResults(); // final score function
void calculateWinner(); // winner function
void displayMenu();// creat menu function
void displayRules();//  didpaly  function
void welcome();
void displayScorecard(int player);
void lastroll(int player);
// Global variables(humen didpaly)
int scorecard[N_CATEGORIES][10] = {0};
int diceRoll[N_DICE];
int c_Players;
char playerNames[10][100];
int isPlayingAgainstComputer = 0;
int score;
//int s_corecard[category][player];


void ai_rollDice(int aidiceRoll[ai_DICE]) {
    for (int i = 0; i < ai_DICE; i++) {
        aidiceRoll[i] = rand() % 6 + 1;  // Roll dice (1-6)
    }
}

void ai_displayDice(int aidiceRoll[ai_DICE]) {
    printf("Dice rolled: ");
    for (int i = 0; i < ai_DICE; i++) {
        printf("%d ", aidiceRoll[i]);
    }
    printf("\n");
}

void ai_keepMaxValues(int aidiceRoll[ai_DICE], int aikeptDice[ai_DICE], int *aikeptCount) {
    *aikeptCount = 0;
    int aimaxValue = 0;

    // Find the maximum value in the current roll
    for (int i = 0; i < ai_DICE; i++) {
        if (aidiceRoll[i] > aimaxValue) {
            aimaxValue = aidiceRoll[i];
        }
    }

    // Keep only the dice with the maximum value
    for (int i = 0; i < ai_DICE; i++) {
        if (aidiceRoll[i] == aimaxValue) {
            aikeptDice[*aikeptCount] = aidiceRoll[i];
            (*aikeptCount)++;
        }
    }
}

void ai_selectCategory(int aikeptDice[ai_DICE], int aikeptCount, int aiplayer, int aidiceRoll[ai_DICE], int aiscorecard[15][2], char aiplayerNames[2][20]) {
    int aicategory = -1;
    int aicounts[7] = {0};  // Array to count occurrences of each dice value (1-6)

    // Count occurrences of each value
    for (int i = 0; i < aikeptCount; i++) {
        aicounts[aikeptDice[i]]++;
    }

    // Check for specific categories
    for (int i = 1; i <= 6; i++) {
        if (aiusedCategories[i] == 0) {  // If this category hasn't been used yet
            aicategory = i;
            break;
        }
    }

    // Three of a Kind
    for (int i = 1; i <= 6; i++) {
        if (aicounts[i] >= 3 && aiusedCategories[THREE_OF_KIND] == 0) {
            aicategory = THREE_OF_KIND;
            break;
        }
    }

    // Four of a Kind
    for (int i = 1; i <= 6; i++) {
        if (aicounts[i] >= 4 && aiusedCategories[FOUR_OF_KIND] == 0) {
            aicategory = FOUR_OF_KIND;
            break;
        }
    }

    // Full House
    if (aicounts[1] >= 3 && aicounts[2] >= 2 && aiusedCategories[FULL_HOUSE] == 0) {
        aicategory = FULL_HOUSE;
    }

    // Yahtzee
    for (int i = 1; i <= 6; i++) {
        if (aicounts[i] == 5 && aiusedCategories[YAHTZEE] == 0) {
            aicategory = YAHTZEE;
            break;
        }
    }

    // Small Straight (1-2-3-4 or 2-3-4-5)
    if ((aicounts[1] > 0 && aicounts[2] > 0 && aicounts[3] > 0 && aicounts[4] > 0 && aiusedCategories[SMALL_STRAIGHT] == 0) ||
        (aicounts[2] > 0 && aicounts[3] > 0 && aicounts[4] > 0 && aicounts[5] > 0 && aiusedCategories[SMALL_STRAIGHT] == 0)) {
        aicategory = SMALL_STRAIGHT;
    }

    // Large Straight (1-2-3-4-5 or 2-3-4-5-6)
    if ((aicounts[1] > 0 && aicounts[2] > 0 && aicounts[3] > 0 && aicounts[4] > 0 && aicounts[5] > 0 && aiusedCategories[LARGE_STRAIGHT] == 0) ||
        (aicounts[2] > 0 && aicounts[3] > 0 && aicounts[4] > 0 && aicounts[5] > 0 && aicounts[6] > 0 && aiusedCategories[LARGE_STRAIGHT] == 0)) {
        aicategory = LARGE_STRAIGHT;
    }

    // If no specific category is found, use Chance (if available)
    if (aicategory == -1 && aiusedCategories[CHANCE] == 0) {
        aicategory = CHANCE;
    }

    // If a valid category is found, mark it as used
    if (aicategory != -1) {
        aiusedCategories[aicategory] = 1;  // Mark this category as used
    }

    // Print selected category number
    printf("Selected category number: %d\n", aicategory);
    ai_calculateCategoryScore(aicategory, aiplayer, aidiceRoll, aiscorecard, aiplayerNames);
}

// Implement the remaining functions here as needed. The code above should correct the main issues and help you proceed.

void ai_computerTurn(int aiplayer, int aiscorecard[15][2], char aiplayerNames[2][20]) {
    int aidiceRoll[ai_DICE];
    int aikeptDice[ai_DICE];
    int aikeptCount = 0;

    // Roll the dice for the first time
    ai_rollDice(aidiceRoll);
    ai_displayDice(aidiceRoll);

    // Keep the maximum values from the first roll
    ai_keepMaxValues(aidiceRoll, aikeptDice, &aikeptCount);

    // Continue rolling until the player decides to stop
    for (int airoll = 1; airoll < 3; airoll++) {  // Two more rolls allowed
        // Reroll the dice that are not kept
        for (int i = 0; i < ai_DICE; i++) {
            if (i >= aikeptCount) {
                aidiceRoll[i] = rand() % 6 + 1;  // Roll new dice
            } else {
                aidiceRoll[i] = aikeptDice[i];  // Keep the kept dice
            }
        }

        ai_displayDice(aidiceRoll);
        ai_keepMaxValues(aidiceRoll, aikeptDice, &aikeptCount);
    }

    // Select a category based on the final kept values
    ai_selectCategory(aikeptDice,aikeptCount,aiplayer, aidiceRoll, aiscorecard, aiplayerNames);
}

void ai_calculateCategoryScore(int aicategory, int aiplayer, int aidiceRoll[ai_DICE], int aiscorecard[15][2], char aiplayerNames[2][20]) {
    int aiscore = 0;

    // Calculate score based on category (1 to 6)
    if (aicategory >= ONES && aicategory <= SIXES) {  // Ensure ONES to SIXES are defined as 1 to 6
        for (int aidice = 0; aidice < ai_DICE; aidice++) {
            if (aidiceRoll[aidice] == aicategory) {
                aiscore += aicategory;
            }
        }

    // Three of a Kind and Four of a Kind score
    } else if (aicategory == THREE_OF_KIND || aicategory == FOUR_OF_KIND) {
        if (ai_checkSpecialCategory(aicategory,aidiceRoll)) {  // Ensure this function is implemented correctly
            for (int aidice = 0; aidice < ai_DICE; aidice++) {
                aiscore += aidiceRoll[aidice];
            }
        }

    // Full House score
    } else if (aicategory == FULL_HOUSE) {
        if (ai_checkSpecialCategory(aicategory,aidiceRoll)) {
            aiscore = 25;
        }

    // Small Straight score
    } else if (aicategory == SMALL_STRAIGHT) {
        if (ai_checkSpecialCategory(aicategory,aidiceRoll)) {
            aiscore = 30;
        }

    // Large Straight score
    } else if (aicategory == LARGE_STRAIGHT) {
        if (ai_checkSpecialCategory(aicategory,aidiceRoll)) {
            aiscore = 40;
        }

    // Yahtzee score
    } else if (aicategory == YAHTZEE) {
        if (ai_checkSpecialCategory(aicategory,aidiceRoll)) {
            aiscore = 50;
        }

    // Chance score
    } else if (aicategory == CHANCE) {
        for (int aidice = 0; aidice < ai_DICE; aidice++) {
            aiscore += aidiceRoll[aidice];
        }
    }

    // Total score and print score
    aiscorecard[aicategory - 1][aiplayer] = aiscore;  // Adjust the index according to your needs
    printf("Score for %s in category %d: %d\n", aiplayerNames[aiplayer], aicategory, aiscore);
    ai_displayScorecard(aiplayer, aiscorecard, aiplayerNames,&aitotalScore); // Call displayScorecard with parameters
}

// Placeholder for the special category check function
int ai_checkSpecialCategory(int aicategory, int aidiceRoll[ai_DICE]) {
    int counts[7] = {0};  // Array to count occurrences of each die face (1-6)

    // Count occurrences of each die face
    for (int i = 0; i < ai_DICE; i++) {
        counts[aidiceRoll[i]]++;
    }

    if (aicategory == THREE_OF_KIND) {
        // Check if there are at least 3 of any die face
        for (int i = 1; i <= 6; i++) {
            if (counts[i] >= 3) return 1;
        }
    } else if (aicategory == FOUR_OF_KIND) {
        // Check if there are at least 4 of any die face
        for (int i = 1; i <= 6; i++) {
            if (counts[i] >= 4) return 1;
        }
    } else if (aicategory == FULL_HOUSE) {
        // Check for a "Full House" condition: 3 of one number and 2 of another
        int hasThree = 0, hasTwo = 0;
        for (int i = 1; i <= 6; i++) {
            if (counts[i] == 3) hasThree = 1;
            if (counts[i] == 2) hasTwo = 1;
        }
        return hasThree && hasTwo;
    } else if (aicategory == SMALL_STRAIGHT) {
        // Check for sequences of 4 consecutive numbers
        if ((counts[1] && counts[2] && counts[3] && counts[4]) ||
            (counts[2] && counts[3] && counts[4] && counts[5]) ||
            (counts[3] && counts[4] && counts[5] && counts[6])) {
            return 1;
        }
    } else if (aicategory == LARGE_STRAIGHT) {
        // Check for sequences of 5 consecutive numbers
        if ((counts[1] && counts[2] && counts[3] && counts[4] && counts[5]) ||
            (counts[2] && counts[3] && counts[4] && counts[5] && counts[6])) {
            return 1;
        }
    } else if (aicategory == YAHTZEE) {
        // Check if all 5 dice are the same
        for (int i = 1; i <= 6; i++) {
            if (counts[i] == 5) return 1;
        }
    }

    return 0;  // Return 0 if the category is not met
}

// Display the scorecard
void ai_displayScorecard(int aiplayer, int aiscorecard[15][2], char aiplayerNames[2][20],int *aitotalScore) {
    printf("| Category          | %s     |\n", aiplayerNames[aiplayer]);
    printf("|-------------------|-------------|\n");

    // Display categories and corresponding scores
    printf("Ones                | %d          |\n", aiscorecard[ONES - 1][aiplayer]);
    printf("Twos                | %d          |\n", aiscorecard[TWOS - 1][aiplayer]);
    printf("Threes              | %d          |\n", aiscorecard[THREES - 1][aiplayer]);
    printf("Fours               | %d          |\n", aiscorecard[FOURS - 1][aiplayer]);
    printf("Fives               | %d          |\n", aiscorecard[FIVES - 1][aiplayer]);
    printf("Sixes               | %d          |\n", aiscorecard[SIXES - 1][aiplayer]);
    printf("-----------------------------------\n");

    // Calculate and display upper section sum
    int aiupperSectionSum = aiscorecard[ONES - 1][aiplayer] + aiscorecard[TWOS - 1][aiplayer] +
                          aiscorecard[THREES - 1][aiplayer] + aiscorecard[FOURS - 1][aiplayer] +
                          aiscorecard[FIVES - 1][aiplayer] + aiscorecard[SIXES - 1][aiplayer];
    printf("Upper Section Sum   | %d          |\n", aiupperSectionSum);
    printf("-----------------------------------\n");

    // Display bonus if upper section sum exceeds 63
    int aiupperBonus = (aiupperSectionSum >= 63) ? 35 : 0;
    printf("Bonus               | %d          |\n", aiupperBonus);

    // Display lower section categories
    printf("-----------------------------------\n");
    printf("Three of a Kind     | %d          |\n", aiscorecard[THREE_OF_KIND - 1][aiplayer]);
    printf("Four of a Kind      | %d          |\n", aiscorecard[FOUR_OF_KIND - 1][aiplayer]);
    printf("Full House          | %d          |\n", aiscorecard[FULL_HOUSE - 1][aiplayer]);
    printf("Small Straight      | %d          |\n", aiscorecard[SMALL_STRAIGHT - 1][aiplayer]);
    printf("Large Straight      | %d          |\n", aiscorecard[LARGE_STRAIGHT - 1][aiplayer]);
    printf("Yahtzee             | %d          |\n", aiscorecard[YAHTZEE - 1][aiplayer]);
    printf("Chance              | %d          |\n", aiscorecard[CHANCE - 1][aiplayer]);
    printf("-----------------------------------\n");

    // Calculate and display lower section sum
    int ailowerSectionSum = aiscorecard[THREE_OF_KIND - 1][aiplayer] +  aiscorecard[FOUR_OF_KIND - 1][aiplayer] +
                          aiscorecard[FULL_HOUSE - 1][aiplayer] + aiscorecard[SMALL_STRAIGHT - 1][aiplayer] +
                          aiscorecard[LARGE_STRAIGHT - 1][aiplayer] + aiscorecard[YAHTZEE - 1][aiplayer] +
                          aiscorecard[CHANCE - 1][aiplayer];
    printf("Lower Section Sum   | %d          |\n", ailowerSectionSum);

    // Calculate total score
    printf("-----------------------------------\n");
    *aitotalScore = aiupperSectionSum + aiupperBonus + ailowerSectionSum;
    //int *aifaScore = &aitotalScore;
    printf("Total Score         | %d          |\n", *aitotalScore);
    printf("-----------------------------------\n\n");
   // aidisplayTotalScore(&aitotalScore);
}
// Function to display the total score
void aidisplayTotalScore(int *aitotalScore) {
     //*aitotalScore=0;
    *aitotalScore= *aitotalScore;
    printf("computer Final Score         | %d          |\n", *aitotalScore); // Dereference the pointer to get the actual value
    //printf("-----------------------------------\n\n");
 // Dereference the pointer to get the actual value
}

void ai_startroall(){
    srand(time(NULL));
     int aiscorecard[15][2] = {0};  // Assuming 2 players and 15 categories
    char aiplayerNames[2][20] = {"Computer1", "computer"};  // Example player names
    printf("Enter your name: ");
    scanf("%s", playerNames[0]);
    printf("\n");
    for (int aiturn = 0; aiturn < 13; aiturn++) { // Example: 10 turns
        printf("-----------------------------------------------------------------------------\n");
        printf("Turn %d humen:\n", aiturn + 1);
        GamePlay();
        printf("-----------------------------------------------------------------------------\n");
        printf("Turn %d computer:\n", aiturn + 1);
        printf("-----------------------------------------------------------------------------\n");
        ai_computerTurn(1, aiscorecard, aiplayerNames); // Computer's turn
        //ai_displayScorecard(1, aiscorecard, aiplayerNames);
  }
aidisplayTotalScore(&aitotalScore);
printf("-----------------------------------------------------------------------------\n");
totalUpResults();
printf("-----------------------------------------------------------------------------\n");
aidisplayTotalScore(&aitotalScore);
printf("-----------------------------------------------------------------------------\n");
calculateWinner();
}
// Main function for demonstration
int main() {

      // Seed the random number generato
     welcome();
    int choice;
    srand(time(0));
   // srand(time(0));
   // Seed the random number generator

    do {
         //display the menu .....
        displayMenu(); //menu function .....
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
               ai_startroall();
                break;
            case 2:
                displayRules(); // display the game rules....
                break;
            case 3:
                printf("Exiting the game. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please select a valid option.\n");
        }
    } while (choice != 3);

    // Initialize the scorecard and player names
   /* int aiscorecard[15][2] = {0};  // Assuming 2 players and 15 categories
    char aiplayerNames[2][20] = {"Computer", "Player 2"};  // Example player names
 for (int aiturn = 0; aiturn < 13; aiturn++) { // Example: 10 turns
        printf("Turn %d:\n", aiturn + 1);
        ai_computerTurn(1, aiscorecard, aiplayerNames); // Computer's turn
        //ai_displayScorecard(1, aiscorecard, aiplayerNames);


    }
    */
    // Function to display the total score
    //aidisplayTotalScore(&aitotalScore);
    //totalUpResults();
    //calculateWinner();
    //aidisplayTotalScore(&aitotalScore);
 // ai_displayScorecard(1, aiscorecard, aiplayerNames, &aitotalScore);
  //printf("Final Score: %d\n", aitotalScore);

    return 0;
}
//..............................................................................................................accumulate

void GamePlay() {


    printf("\n");
    //isPlayingAgainstComputer = 1;
    c_Players = 1;  // Player and computer
    //printf("Enter your name: ");
    //scanf("%s", playerNames[0]);
    //printf("\n");
    //strcpy(playerNames[1], "Computer");  // The second player is the computer

    srand(time(NULL)); // Seed for random number generation
    for (int player = 0; player < c_Players; player++) {
             //printf("Turn %d:\n", player+ 1);
            printf("-----------------------------------------------------------------------------\n");
            startFirstDiceRoll(player);
            break;
    }
    //int player = 0;
    //startFirstDiceRoll(player);
    // GamePlay(); // Start the game

    /*for (int i = 0; i < N_CATEGORIES; i++) {
        for (int player = 0; player < c_Players; player++) {
             printf("Turn %d:\n", player+ 1);
             printf("------------------------------------------------------------------\n\n");
            startFirstDiceRoll(player);
            printf("------------------------------------------------------------------\n\n");
             printf("Turn %d:\n", player+ 1);
            printf("------------------------------------------------------------------\n\n");

            //lastroll(player);
            //secondAndThirdRoll(player);
            //selectCategory(player);
            //displayScorecard(player);
        }
    }

    //totalUpResults();
    //calculateWinner();
    */
}
//first dice rolled function......
void startFirstDiceRoll(int player) {
    /*int score=0;
    //void calculateCategoryScore(int player, int category)
    printf("|                  |  %s     |\n",playerNames[player]);
        printf("one                |  %d          |\n",score);
        printf("twos               |  %d          |\n",score);
        printf("threes             |  %d          |\n",score);
        printf("fours              |  %d          |\n",score);
        printf("fives              |  %d          |\n",score);
        printf("sixes              |  %d          |\n",score);
        printf("sum                |  %d          |\n",score);
        printf("Bonus              |  %d          |\n",score);
        printf("three of kind      |  %d          |\n",score);
        printf("four of kind       |  %d          |\n",score);
        printf("full house         |  %d          |\n",score);
        printf("small straight     |  %d          |\n",score);
        printf("large straight     |  %d          |\n",score);
        printf("chance             |  %d          |\n",score);
        printf("yahtzee            |  %d          |\n",score);
        printf("total score        |              |\n\n\n");*/

    displayScorecard(player);

    printf("........%s,start your game good luck!!!!!!!...........\n",playerNames[player]);
    printf("%s, it's your turn! Roll the dice.\n", playerNames[player]);

    // Rolled the dice.....
    for (int dice = 0; dice < N_DICE; dice++) {
        diceRoll[dice] = rand() % 6 + 1;  // Random dice rolled between 1 and 6....
    }

    // Display the rolled dice......
    printf("Rolled dice: ");
    for (int dice = 0; dice < N_DICE; dice++) {
        printf("%d ", diceRoll[dice]);
    }
    printf("\n\n");
    if (strcmp(playerNames[player], "Computer") == 0){
             secondAndThirdRoll(player);
             selectCategory(player);
             displayScorecard(player);
     }else{
        lastroll(player);
    }
}

void lastroll(int player) {
    char p;
    printf("If you want to re-roll, enter yes('y') or no('n'): ");
    scanf(" %c", &p);  // Notice the space before %c to consume any leftover newline

    printf("You entered: %c\n", p);  // Debugging print statement

    if (p == 'n') {
        //No re-roll selected, moving to selectCategory.
        selectCategory(player);
    } else if (p == 'y') {
        //"Re-roll selected, moving to secondAndThirdRoll.
        secondAndThirdRoll(player);
        selectCategory(player);
    } else {
        printf("Invalid input. Please enter 'y' for yes or 'n' for no.\n");
        lastroll(player);  // Recursive call to ask for input again
    }
}


// second roled and third dice function.......
void secondAndThirdRoll(int player) {

    if (strcmp(playerNames[player], "Computer") == 0) {
        printf("Computer re-rolling selected dice...\n");
        for (int i = 0; i < 2; i++) {
            for (int dice = 0; dice < N_DICE; dice++) {
                if (rand() % 2 == 0) {
                    diceRoll[dice] = rand() % 6 + 1;
                }
            }
            printf("Computer rolled dice: ");
            for (int dice = 0; dice < N_DICE; dice++) {
                printf("%d ", diceRoll[dice]);
            }
            printf("\n");
        }
    } else {
        printf("Re-rolling selected dice...\n");
        for (int i = 0; i < 2; i++) {
            for (int dice = 0; dice < N_DICE; dice++) {
                int reroll;
                do {
                    printf("Dice %d (%d): Enter 1 to re-roll or 0 to keep: ", dice + 1, diceRoll[dice]);
                    if (scanf("%d", &reroll) != 1) {
                        // If non-integer input is detected, clear the input buffer
                        while (getchar() != '\n'); // discard invalid input
                        reroll = -1; // Set invalid value to trigger retry
                    }

                    if (reroll != 0 && reroll != 1) {
                        printf("Invalid input. Please enter 1 to re-roll or 0 to keep.\n");
                    }
                } while (reroll != 0 && reroll != 1); // Keep asking until valid input

                if (reroll == 1) {
                    diceRoll[dice] = rand() % 6 + 1;
                }
            }
            printf("Rolled dice: ");
            for (int dice = 0; dice < N_DICE; dice++) {
                printf("%d ", diceRoll[dice]);
            }
            printf("\n");
        }
    }
}


// select The scoring categories include options.........
void selectCategory(int player) {
    int category;

    if (strcmp(playerNames[player], "Computer") == 0) {
        // Computer selects a random category............
        do {
            //random category computer......
            category = rand() % 13 + 1; // between 1 to 13....
        } while (!checkCategory(player, category));// repeat valide catagory ........

        printf("Computer selects category %d.\n", category);
    } else {
        //category input the human........
        printf("Select a category for this roll: ");
        scanf("%d", &category);

        // Check and valid if the category inpit the human......
        if (!checkCategory(player, category)) {
            printf("error input check the valid");
            printf("Category already filled or invalid. Try again.\n");
            selectCategory(player); // Recurse until a valid category is selected
            return;
        }
    }

    calculateCategoryScore(player, category);
}

int checkCategory(int player, int category) {

    // Check if the category is already used
    if (scorecard[category - 1][player] == 0) {
        return 1;
    }
    return 0;
}

void calculateCategoryScore(int player, int category) {
    int score = 0;
    //int s_corecard[category][player];
    // Calculate score based on category (1 to 6)...........
    if (category >= ONES && category <= SIXES) {
        for (int dice = 0; dice < N_DICE; dice++) {
            if (diceRoll[dice] == category) {
                score += category;
            }
        }

        //three of kind and four of kind score.................
    } else if (category == THREE_OF_KIND || category == FOUR_OF_KIND) {
        if (checkSpecialCategory(category)) {
            for (int dice = 0; dice < N_DICE; dice++) {
                score += diceRoll[dice];
            }
        }
        //full house secore...............
    } else if (category == FULL_HOUSE) {
        if (checkSpecialCategory(FULL_HOUSE)) {
            score = 25;
        }
        //small straight score.................
    } else if (category == SMALL_STRAIGHT) {
        if (checkSpecialCategory(SMALL_STRAIGHT)) {
            score = 30;
        }
        //large straight score.............
    } else if (category == LARGE_STRAIGHT) {
        if (checkSpecialCategory(LARGE_STRAIGHT)) {
            score = 40;
        }
        //yahtzee score..........................
    } else if (category == YAHTZEE) {
        if (checkSpecialCategory(YAHTZEE)) {
            score = 50;
        }
        //chance score ..................
    } else if (category == CHANCE) {
        for (int dice = 0; dice < N_DICE; dice++) {
            score += diceRoll[dice];
        }
    }
    //total score and print score..............
        scorecard[category - 1][player] = score;
        printf("Score for %s: %d\n", playerNames[player], score);
        //scorecard(s_corecard[category][player]);

}

int checkSpecialCategory(int category) {
    int counts[6] = {0};

    // Count occurrences of each dice value
    for (int dice = 0; dice < N_DICE; dice++) {
        counts[diceRoll[dice] - 1]++;
    }

    if (category == THREE_OF_KIND) {
        for (int i = 0; i < 6; i++) {
            if (counts[i] >= 3) {
                return 1;
            }
        }
    } else if (category == FOUR_OF_KIND) {
        for (int i = 0; i < 6; i++) {
            if (counts[i] >= 4) {
                return 1;
            }
        }
    } else if (category == FULL_HOUSE) {
        int threeOfKind = 0, twoOfKind = 0;
        for (int i = 0; i < 6; i++) {
            if (counts[i] == 3) threeOfKind = 1;
            if (counts[i] == 2) twoOfKind = 1;
        }
        return (threeOfKind && twoOfKind);
    } else if (category == SMALL_STRAIGHT) {
        // Check for small straight (1-2-3-4, 2-3-4-5, or 3-4-5-6)
        if ((counts[0] >= 1 && counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1) ||
            (counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1) ||
            (counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1 && counts[5] >= 1)) {
            return 1;
        }
    } else if (category == LARGE_STRAIGHT) {
        // Check for large straight (1-2-3-4-5 or 2-3-4-5-6)
        if ((counts[0] >= 1 && counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1) ||
            (counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1 && counts[5] >= 1)) {
            return 1;
        }
    } else if (category == YAHTZEE) {
        for (int i = 0; i < 6; i++) {
            if (counts[i] == 5) {
                return 1;
            }
        }
    }
    return 0;
}

//total function.............
void totalUpResults() {
    printf("\nFinal Scores:\n");
    for (int player = 0; player < c_Players; player++) {
        int totalScore = 0;
        for (int category = 0; category < N_CATEGORIES; category++) {
            totalScore += scorecard[category][player];//category score.......

        }
        //total prirnt .........
        displayScorecard( player);
        printf("%s's total score: %d\n", playerNames[player], totalScore);
    }
}

//winner function...........
void calculateWinner() {
    int highestScore = 0; //found highest score.......
    int winnerIndex = 0; //play of the index......

    //play loop.....
    for (int player = 0; player < c_Players; player++) {
        int totalScore = 0;// total score.......
        //all categories score(play)........
        for (int category = 0; category < N_CATEGORIES; category++) {
            totalScore += scorecard[category][player];
        }
        if (totalScore > highestScore) {
            highestScore = totalScore;
            winnerIndex = player; //select winner.......
        }
    }

    printf("\nThe winner is %s with a score of %d!\n", playerNames[winnerIndex], highestScore);
    printf("-----------------------------------------------------------------------------\n\n");
    printf("------------------thanks players------------------------\n");
    printf("Thank you for playing Yahtzee!\n");
    printf("-----------------------------------------------------------------------------\n\n\n\n");


}

void displayScorecard(int player) {



    printf("| Category          | %s     |\n", playerNames[player]);
    printf("|-------------------|-------------|\n");

    // Display categories and corresponding scores
    printf("Ones                | %d          |\n", scorecard[ONES - 1][player]);
    printf("Twos                | %d          |\n", scorecard[TWOS - 1][player]);
    printf("Threes              | %d          |\n", scorecard[THREES - 1][player]);
    printf("Fours               | %d          |\n", scorecard[FOURS - 1][player]);
    printf("Fives               | %d          |\n", scorecard[FIVES - 1][player]);
    printf("Sixes               | %d          |\n", scorecard[SIXES - 1][player]);
    printf("-----------------------------------\n");

    // Calculate and display upper section sum
    int upperSectionSum = scorecard[ONES - 1][player] + scorecard[TWOS - 1][player] +
                          scorecard[THREES - 1][player] + scorecard[FOURS - 1][player] +
                          scorecard[FIVES - 1][player] + scorecard[SIXES - 1][player];
    printf("Upper Section Sum   | %d          |\n", upperSectionSum);
    printf("-----------------------------------\n");

    // Display bonus if upper section sum exceeds 63
    int upperBonus = (upperSectionSum >= 63) ? 35 : 0;
    printf("Bonus               | %d          |\n", upperBonus);

    // Display lower section categories
    printf("-----------------------------------\n");
    printf("Three of a Kind     | %d          |\n", scorecard[THREE_OF_KIND - 1][player]);
    printf("Four of a Kind      | %d          |\n", scorecard[FOUR_OF_KIND - 1][player]);
    printf("Full House          | %d          |\n", scorecard[FULL_HOUSE - 1][player]);
    printf("Small Straight      | %d          |\n", scorecard[SMALL_STRAIGHT - 1][player]);
    printf("Large Straight      | %d          |\n", scorecard[LARGE_STRAIGHT - 1][player]);
    printf("Yahtzee             | %d          |\n", scorecard[YAHTZEE - 1][player]);
    printf("Chance              | %d          |\n", scorecard[CHANCE - 1][player]);
    printf("-----------------------------------\n");
    // Calculate and display lower section sum
    int lowerSectionSum = scorecard[THREE_OF_KIND - 1][player] + scorecard[FOUR_OF_KIND - 1][player] +
                          scorecard[FULL_HOUSE - 1][player] + scorecard[SMALL_STRAIGHT - 1][player] +
                          scorecard[LARGE_STRAIGHT - 1][player] + scorecard[YAHTZEE - 1][player] +
                          scorecard[CHANCE - 1][player];
    printf("Lower Section Sum   | %d          |\n", lowerSectionSum);

    // Calculate total score
    printf("-----------------------------------\n");
    int totalScore = upperSectionSum + upperBonus + lowerSectionSum;
    printf("Total Score         | %d          |\n", totalScore);
    printf("-----------------------------------\n\n");

}

void welcome(){
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n*******************************************************************************************************************************************************************************************************************");
    printf("*******************************************************************************************************************************************************************************************************************\n\n");
    printf("     \n\n\n\n\                                                                                                Welcome to Yahtzee!\n\n\n\n\n\n\n\n");
    printf("*******************************************************************************************************************************************************************************************************************");
    printf("*******************************************************************************************************************************************************************************************************************");

    getchar();  // Wait for user input to see the welcome message (you can also use _getch() if you're on Windows)

    // Clear the terminal screen based on OS
    #ifdef _WIN32
        system("cls");  // For Windows
    #else
        system("clear");  // For Linux/macOS
    #endif
}
   // display menu function......
   void displayMenu(){
    printf("\n********* YAHTZEE GAME MENU *********\n");
    printf("1. Start Game\n");
    printf("2. View Rules\n");
    printf("3. Exit\n");
    printf("*************************************\n\n");
   // return 0;
}

//  display is game rules
void displayRules(){

    printf("\n***** Yahtzee Rules *****\n");
    printf("*. The game is played with 5 dice over 13 rounds.\n");
    printf("*.Players take turns rolling the dice, and can re-roll up to 2 times per turn.\n");
    printf("*. The goal is to score the highest points by selecting categories like 'Ones', 'Three of a Kind', 'Full House', etc.\n");
    printf("*. The game ends after all 13 rounds, and the player with the highest total score wins.\n\n\n\n");
    printf("****************Upper section combinations*********************\n\n\n");
    printf("1.Ones, Twos, Threes, Fours, Fives, Sixes: Score based on the sum of the dice that show the chosen number. \n\n\n\n");
    printf("****************Lower section combinations**********************\n\n\n");
    printf("1.Three of a Kind: Score the sum of all dice if at least three of them are the same. \n");
    printf("2.Four of a Kind: Score the sum of all dice if at least four of them are the same. \n");
    printf("3.Full House: Score 25 points for a combination of three of one number and two of another.\n");
    printf("4.Small Straight: Score 30 points for four consecutive numbers. \n");
    printf("5.Large Straight: Score 40 points for five consecutive numbers. \n");
    printf("6.Yahtzee: Score 50 points for five dice showing the same number.\n");
    printf("7.Chance: Score the sum of all dice, regardless of combinatio.\n\n");

    printf("**************************\n\n");
   // return 0;
}
