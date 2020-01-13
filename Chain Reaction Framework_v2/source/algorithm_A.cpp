#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
#define maximumDepth 4
#define INFINITY 65535

using namespace std;

int r[] = {0, -1, 0, 1};
int c[] = {1, 0, -1, 0};

bool isCritical(Board b, int i, int j){
    if (b.get_capacity(i, j) - b.get_orbs_num(i, j) == 1)
        return true;
    return false;
}

int evaluation(Board b, int colorAI){
    int score = 0;
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 6; j++){
            if (b.get_cell_color(i, j) == colorAI){
                //If the cell is the player's color, score + 1
                score += b.get_orbs_num(i, j);
                //Calculate the score of critical opponent surrounding to the player
                int criticalOpponent = 0;
                for (int k = 0; k < 4; k++){
                    int row = i + r[k];
                    int col = j + c[k];
                    if (row >= 0 && row < 5 && col >= 0 && col < 6){
                        if (b.get_cell_color(row, col) != colorAI && b.get_cell_color(row, col) != 'w' && isCritical(b, row, col))
                            //If opponent is critical by placing lesser orbs, it get higher score
                            criticalOpponent += (5 - b.get_orbs_num(row, col));
                    }
                }
                
                if (criticalOpponent)
                    score -= criticalOpponent;
                else{
                    if (isCritical(b, i, j))
                        score += 2;
                    if (b.get_capacity(i, j) < 4)
                        score += (5 - b.get_capacity(i, j));
                }
            }
        }
    }
    return score;
}

int minimax(Board b, Player previousPlayer, int myAI, int alpha, int beta, int depth, bool isMaximizingPlayer){
    if (b.win_the_game(previousPlayer))
        if (!isMaximizingPlayer)
            return INFINITY;
        else
            return -INFINITY;
    else if (depth == 0)
        return evaluation(b, myAI);
    //Set the color of current player
    int color;
    if (previousPlayer.get_color() == 'r')
        color = 'b';
    else
        color = 'r';    
    Player player(color);
    
    if (isMaximizingPlayer){
        int bestScore = -INFINITY;
        bool prune = false;
        for (int i = 0; i < 5; i++){
            for (int j = 0; j < 6; j++){
                if (b.get_cell_color(i, j) == color || b.get_cell_color(i, j) == 'w'){
                    Board currentBoard = b;
                    currentBoard.place_orb(i, j, &player);
                    //Next round
                    int score = minimax(currentBoard, player, myAI, alpha, beta, depth - 1, false);
                    //Select maximizing step
                    if (score > bestScore)
                        bestScore = score;
                    if (bestScore > alpha)
                        alpha = bestScore;
                    if (beta <= alpha){
                        break;
                        prune = true;
                    }     
                }
            }
            if (prune)
                break;
        }
        return bestScore;
    } else{
        int bestScore = INFINITY;
        bool prune = false;
        for (int i = 0; i < 5; i++){
            for (int j = 0; j < 6; j++){
                if (b.get_cell_color(i, j) == color || b.get_cell_color(i, j) == 'w'){
                    Board currentBoard = b;
                    currentBoard.place_orb(i, j, &player);
                    //Next round
                    int score = minimax(currentBoard, player, myAI, alpha, beta, depth - 1, true);
                    //Select maximizing step
                    if (score < bestScore)
                        bestScore = score;
                    if (bestScore < beta)
                        beta = bestScore;
                    if (beta <= alpha){
                        break;
                        prune = true;
                    }
                }
            }
            if (prune)
                break;
        }
        return bestScore;
    }
}

void algorithm_A(Board board, Player player, int index[]){
    int color = player.get_color();
    int bestScore = -INFINITY;

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 6; j++){
            if (board.get_cell_color(i, j) == color || board.get_cell_color(i, j) == 'w'){
                Board currentBoard = board;
                currentBoard.place_orb(i, j, &player);
                //Next round
                int score = minimax(currentBoard, player, color, -INFINITY, INFINITY, maximumDepth, false);
                //Select maximizing step
                if (score > bestScore){
                    bestScore = score;
                    index[0] = i;
                    index[1] = j;
                }
            }
        }
    }
}
