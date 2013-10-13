#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define STRING_LENGTH 32
#define SQUARES 9

typedef char board_type[SQUARES];

const char empty = ' ';
const int infinity = 10; // larger than any score
const int max_moves = SQUARES; // max moves in a game

int total_nodes; // nodes searched

// array with the 8 possible ways to win (3 squares in a row)
#define POSSIBLE_WINS 8
const int three_in_a_row[POSSIBLE_WINS][3] = {
    {0, 1, 2}, // h. 1st row
    {3, 4 ,5}, // h. 2nd row
    {6 ,7 ,8}, // h. 3rd row
    {0, 3, 6}, // v. 1st col
    {1, 4, 7}, // v. 2nd col
    {2, 5, 8}, // v. 3rd col
    {0, 4, 8}, // back diagonal
    {2, 4, 6}  // forward diagonal
};

// array for heuristics
const int heuristic_array[4][4] = {
    {   0, -10, -100, -1000},
    {  10,   0,    0,     0},
    { 100,   0,    0,     0},
    {1000,   0,    0,     0}
};

typedef struct {
    int square;
    int heuristic;
} move_heuristic_type;

// init the board with all empty spaces
void init(board_type board) {
    int i;
    for(i = 0; i<SQUARES; i++)
        board[i] = empty;
}

// if a player has won, return the winner
// if it is a tie, return 'T'
// if it still hasn't finished, then return empty
char winner(board_type board) {
    int i;
    for(i=0; i<POSSIBLE_WINS; i++) {
        char winner = board[three_in_a_row[i][0]];
        if(winner != empty && winner == board[three_in_a_row[i][1]]
           && winner == board[three_in_a_row[i][2]])
            return winner;
    }
    
    for(i=0; i<SQUARES; i++) {
        if(board[i] == empty)
            return empty;
    }
    
    return 'T';
}

// returns the other player
char other_player(char player) {
    return player == 'X' ? 'O' : 'X';
}

// inserts the player mark on board indexed by the square
void play(board_type board, int square, char player) {
    board[square] = player;
}

// prints the current board
void print_board(board_type board) {
    int i;
    
    for(i=0; i < 3; i++) {
        int j;
        // first line
        for(j=0; j<3; j++) {
            if(board[(i*3)+j] == 'X')
                printf("\\/ %c", (j<2) ? '|' : empty);
            else if(board[(i*3)+j] == 'O')
                printf("/\\ %c", (j<2) ? '|' : empty);
            else if(board[(i*3)+j] == empty)
                printf("   %c", (j<2) ? '|' : empty);
        }
        putchar('\n');
        
        // second line
        for(j=0; j<3; j++) {
            if(board[(i*3)+j] == 'X')
                printf("/\\ %c", (j<2) ? '|' : empty);
            else if(board[(i*3)+j] == 'O')
                printf("\\/ %c", (j<2) ? '|' : empty);
            else if(board[(i*3)+j] == empty)
                printf("   %c", (j<2) ? '|' : empty);
        }
        putchar('\n');
        
        // draw the floor
        if(i<2)
            printf("--- --- ---");
        
        putchar('\n');
    }
    putchar('\n');
}

// return the heuristic value used to determine the order
// in which the children of a node are searched.
// this increases speed and efficiency
int evaluate(board_type board, char player) {
    int i;
    int heuristic = 0;
    
    for(i=0; i<POSSIBLE_WINS; i++) {
        int j;
        int players = 0, other_players = 0;
        
        for(j=0; j<3; j++) {
            char piece = board[three_in_a_row[i][j]];
            if(piece == player)
                players++;
            else if(piece == other_player(player))
                other_players++;
        }
        
        heuristic += heuristic_array[players][other_players];
    }
    return heuristic;
}

// return the score of the best move found for a board
// the best square is stored in *square
// note - recursive function, searches nodes and childs
int best_move(board_type board, char player, int *square, int move_num, int alpha, int beta) {
    int best_square = -1;
    int moves = 0;
    int i;
    move_heuristic_type move_heuristic[SQUARES]; // an array of 9 heuristc structs
    
    total_nodes++; // times we call best_move
    
    // we find calculate the heuristic value of each move
    // and sort the structs in descending order
    // this way, we speed things up a bit
    for(i=0; i<SQUARES; i++) {
        // if the square is available for play
        if(board[i] == empty) {
            int heuristic;
            int j;
            
            play(board, i, player); // we play the empty square
            heuristic = evaluate(board, player); // we get the heuristic value
            play(board, i, empty); // we reverse the play
            
            // now we must sort the picked up structs in descending order
            for(j = moves-1; j>=0 && move_heuristic[j].heuristic < heuristic; j--) {
                move_heuristic[j+1].heuristic = move_heuristic[j].heuristic;
                move_heuristic[j+1].square = move_heuristic[j].square;
            }
            move_heuristic[j+1].heuristic = heuristic;
            move_heuristic[j+1].square = i;
            moves++;
        }
        // we can store the structs and use qsort here instead.
    }
    
    for(i=0; i<moves; i++) {
        int score;
        int sq = move_heuristic[i].square;
        char w;
        
        // we make a move and get the score
        play(board, sq, player);
        
        w = winner(board);
        if(w == 'X')
            score = (max_moves + 1) - move_num;
        else if(w == 'O')
            score = move_num - (max_moves + 1);
        else if(w == 'T')
            score = 0;
        else // haven't finished board
            score = best_move(board, other_player(player), square, move_num + 1, alpha, beta); // we recurse
        
        // reverse the play
        play(board, sq, empty);
        
        // now we prune alpha-beta
        if(player == 'X') {
            if(score >= beta) { // cut off
                *square = sq;
                return score;
            } else if(score > alpha) { // we found a better alpha (alpha only increases)
                alpha = score;
                best_square = sq;
            }
        } else {
            if(score <= alpha) { // cut off
                *square = sq;
                return score;
            } else if(score < beta) { // we found that oponent has a best worse beta (beta only decreases)
                beta = score;
                best_square = sq;
            }
        }
    }
    
    *square = best_square;
    if(player == 'X')
        return alpha;
    else
        return beta;
}

// i can remove this

/*
void describe(int score, char computer) {
    if(score < 0) {
        if(computer == 'X')
            printf("You will win.\n");
        else
            printf("I will win by move %d\n", max_moves + score + 1);
    }
    else if(score == 0) {
        printf("I can guarantee at least a tie.\nIf you do an error, I will win.\n");
    }
    else if(score > 0) {
        if(computer == 'X')
            printf("I will win by move %d\n", max_moves - score +1);
        else
            printf("You will win.\n");
    }
}
*/

void move(board_type board, char player, int move_num, bool humans_only, char computer) {
    int square;
    
    if(humans_only) {
        do {
            printf("What is your move?: ");
            fflush(stdin);
            scanf("%d", &square);
            square--;
        } while(board[square] != empty);
        play(board, square, player);
    } else {
        if(player == computer) {
            total_nodes = 0;
            //describe(best_move(board, computer, &square, move_num, -infinity, infinity), computer);
            best_move(board, computer, &square, move_num, -infinity, infinity);
            //printf("Analyzed %d nodes\n", total_nodes);
            //getchar(); getchar();
            play(board, square, computer);
        } else {
            do {
                printf("What is your move?: ");
                fflush(stdin);
                scanf("%d", &square);
                square--;
            } while(board[square] != empty);
            play(board, square, other_player(computer));
        }
    }
}

void player_vs_computer(void) {
    // for now, ignore choosing different players
    char player = 'X';
    char ans;
    board_type board;
    int move_num = 1; // first move
    char computer;
    
    system("cls");
    // initialize board
    init(board);
    
    // who will move first?
    printf("Do you want to move first? (y/n): ");
    fflush(stdin);
    scanf("%c", &ans);
    
    if(toupper(ans) == 'Y')
        computer = 'O';
    else
        computer = 'X';
    
    // while !finished
    while(winner(board) == empty) {
        system("cls");
        printf("Match: Computer vs. Player\n\n");
        print_board(board);
        move(board, player, move_num, false, computer);
        player = other_player(player);
        move_num++;
    }
    system("cls");
    printf("Match: Computer vs. Player\n\n");
    print_board(board);
    
    if(winner(board) != 'T')
        printf("Winner is %c!\n", winner(board));
    else
        printf("Draw!\n");
    
}

void player_vs_player(void) {
    char x_name[STRING_LENGTH];
    char o_name[STRING_LENGTH];
    board_type board;
    int move_num = 1;
    char player = 'X';
    
    system("cls");
    memset(x_name, '\0', STRING_LENGTH);
    memset(o_name, '\0', STRING_LENGTH);
    
    init(board);
    
    printf("Enter the name of player 'X': ");
    fflush(stdin);
    scanf("%32s", &x_name);
    printf("Enter the name of player 'O': ");
    fflush(stdin);
    scanf("%32s", &o_name);
    
    while(winner(board) == empty) {
        system("cls");
        printf("Match: Player vs. Player\n\n");
        print_board(board);
        if(player == 'X')
            printf("It is turn for %s!\n", x_name);
        else
            printf("It is turn for %s!\n", o_name);
        
        move(board, player, move_num, true, '\0');
        player = other_player(player);
        move_num++;
    }
    system("cls");
    printf("Match: Player vs. Player\n\n");
    print_board(board);
    
    if(winner(board) != 'T')
        printf("Winner is %s!\n", winner(board) == 'X' ? x_name : o_name);
    else
        printf("Draw!\n");
    
}

void how_to_play(void) {
    printf(
        "Box settings for tic tac toe:\n\n"
        
        "   |   |   \n"
        "  1| 2 |3  \n"
        "--- --- ---\n"
        "  4| 5 |6  \n"
        "--- --- ---\n"
        "  7| 8 |9  \n"
        "   |   |   \n\n"
        
        "You must first select if you want to play with another player\n"
        "or if you want to play with the computer.\n\n"
        
        "If you choose Player vs. Player, you must enter each player's name.\n"
        "If you choose Player vs. Computer, you must select if you want to go\n"
        "first or second.\n"
        "X always goes first.\n\n"
        
        "While playing you will be required to enter a move.\n"
        "Acceptable moves are shown above.\n\n"
    );
}

int main(void)
{
    int choice, game_choice;
    char ans;
    
    main_menu:
    system("cls");
    printf("What do you want to do?\n1. Play\n2. How to play\n\n");
    
    printf("Enter your choice: ");
    fflush(stdin);
    scanf("%d", &choice);
    
    switch(choice) {
        case 2:
            system("cls");
            how_to_play();
            printf("Play now? <press enter>");
            getchar(); getchar();
        
        case 1:
            // start game... also collect info for what game we want to play
            do {
                system("cls");
                printf(
                    "What do you want to play?\n"
                    "1. Player vs Player\n"
                    "2. Player vs Computer\n\n"
                );
                
                choose_game:
                printf("Your choice: ");
                fflush(stdin);
                scanf("%d", &game_choice);
                
                switch(game_choice) {
                    case 1:
                        player_vs_player();
                        break;
                    case 2:
                        player_vs_computer();
                        break;
                    default:
                        printf("Invalid choice! Please try again\n");
                        goto choose_game;
                }
                
                printf("Do you want to play again? (y/n): ");
                fflush(stdin);
                scanf("%c", &ans);
            } while(toupper(ans) == 'Y');
            
            goto main_menu;
            break;
        
        default:
            system("cls");
            printf("Enter a valid option! (1 or 2)\n\n");
            goto main_menu;
    }
    
    return 0;
}

// if you want to make the computer able to determine if it is playing for
// x or o, you should change values in player_vs_computer and in move.
// i'm not sure if I must change values also in best_move.

