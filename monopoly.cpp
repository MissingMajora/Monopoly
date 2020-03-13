#include <string>
#include <algorithm>    // std::shuffle
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std; 

int jail = 10;
int gtj = 30;
int size = 40;
int turns = 10000000;

struct Player{
    int tile;
    int money;
    bool goujChance;
    bool goujCom;
};

typedef void (*Cards) (Player &player);

void toGo(Player &player){
    player.money += 200;
    player.tile = 0;
}
void toIllAve(Player &player){
    player.tile = 24;
}
void toUtil(Player &player){
    if(player.tile >= 13 || player.tile <= 27){
        player.tile = 28;
    }
    else{
        player.tile = 12;
    }
}
void toStChar(Player &player){
    player.tile = 11;
}
void toRR(Player &player){
    if((player.tile >= 0 && player.tile < 5) || player.tile > 35){
        player.tile = 5;
    }else if( player.tile >=6 && player.tile <=14 ){
        player.tile = 15;
    }else if( player.tile >= 16 && player.tile <=24 ){
        player.tile = 25;
    }else{
        player.tile = 35;
    }
}
void goujChance(Player &player){
    player.goujChance = true;
}
void goujCom(Player &player){
    player.goujCom = true;
}
void backThree(Player &player){
    player.tile -= 3;
    if(player.tile == gtj){
        player.tile = jail;
    }
}
void gtjc(Player &player){
    player.tile = jail;
}
void getMoney(Player &player){
    player.money += 50;
}
void loseMoney(Player &player){
    player.money -+ 50;
}
void toReading(Player &player){
    player.tile = 5;
}
void toBoardWalk(Player &player){
    player.tile = 39;
}



void move(Player &player, int* tiles, Cards* chance, Cards* community){
    int i = 0;
    int d1 = rand()%6+1;
    int d2 = rand()%6+1;
    //printf("Rolled %d\n", d1+d2);
    if(player.tile+d1+d2 == gtj){
        player.tile= jail;
        tiles[player.tile] += 1;
        return;
    }
    if(d1 != d2){
        if(player.tile == 7 || player.tile == 22 || player.tile == 36){
            chance[rand()%16](player);
            tiles[player.tile] += 1;
        }else if (player.tile == 2 || player.tile == 17 || player.tile == 33){
            community[rand()%16](player);
            tiles[player.tile] += 1;
        }else{
            player.tile = (player.tile + d1 + d2)%size;
            tiles[player.tile] += 1;
        }
        return;
    }
    player.tile = (player.tile + d1 + d2)%size;
    tiles[player.tile] += 1;
    d1 = rand()%6+1;
    d2 = rand()%6+1;
    //printf("Rolled %d\n", d1+d2);
    if(player.tile+d1+d2 == gtj){
        player.tile= jail;
        tiles[player.tile] += 1;
        return;
    }
    if(d1 != d2){
        if(player.tile == 7 || player.tile == 22 || player.tile == 36){
            chance[rand()%16](player);
            tiles[player.tile] += 1;
        }else if (player.tile == 2 || player.tile == 17 || player.tile == 33){
            community[rand()%16](player);
            tiles[player.tile] += 1;
        }else{
            player.tile = (player.tile + d1 + d2)%size;
            tiles[player.tile] += 1;
        }
        return;
    }
    player.tile = (player.tile + d1 + d2)%size;
    tiles[player.tile] += 1;
    d1 = rand()%6+1;
    d2 = rand()%6+1;
    //printf("Rolled %d\n", d1+d2);
    if(d1 == d2){
        player.tile = jail;
        tiles[jail] += 1;
        return;
    }
    return;
}

int main(){
    printf("Starting\n");
    unsigned seed = time(0);
    srand(seed);
    int tiles[size];
    int i = 0;
    Cards chance[] = {
        toGo,
        toIllAve,
        toUtil,
        toStChar,
        toRR,
        goujChance,
        backThree,
        gtjc,
        toReading,
        toBoardWalk,
        getMoney,
        loseMoney,
        loseMoney,
        loseMoney,
        getMoney,
        getMoney,
    };
    random_shuffle(begin(chance), end(chance));
    int length = sizeof(chance) / sizeof(*chance);
    /*
    for(i=0; i<length; i++){
        printf("On iter %d this is the function pointer: %p \n", i, chance[i]);
    }
    */
    Cards community[] = {
        toGo,
        getMoney,
        loseMoney,
        getMoney,
        goujCom,
        gtjc,
        getMoney,
        getMoney,
        getMoney,
        getMoney,
        getMoney,
        loseMoney,
        getMoney,
        loseMoney,
        getMoney,
        getMoney
    };
    random_shuffle(begin(community), end(community));
    for(i=0; i<size; i++){
        tiles[i] = 0;
    }
    struct Player p1 = {0,0,false,false};
    for(i=0; i<turns; i++){
        move(p1, tiles, chance, community);
    }
    printf("Player Tile %d\n", p1.tile);
    for(i=0; i<size; i++){
        double percent = tiles[i]/(double)turns;
        printf("Percent of lands on %d: %f\n", i, percent);
    }
    for(i=0; i<size; i++){
        printf("Number of lands on %d: %d\n", i, tiles[i]);
    }
    return 1;
}