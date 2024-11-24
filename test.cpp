#include "game_of_life.h"
#include <iostream>

using namespace std;
using namespace GOL;

int main(int argc, char const *argv[])
{
    GameOfLife game("starter.txt");
    cout << game;
    game += 105;
    game.SetLiveCell('L');
    cout << game;
    game -=100;
    cout << game;
    game -=1;
    return 0;
}
