#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//deklarasi struct
struct player{
    int hp;
    int atk;
    int def;
    struct weapon* senjata;
};
struct weapon{
    int hpBonus;
    int atkBonus;
    int defBonus;
};
struct enemy{
    int hp;
    int atk;
    int def;
};

//function prototype
void acakStatus(struct player* player);
void reroll(struct player* player);
void gunakanSenjata(struct player* player);

int main(){
    //inisialisasi bonus weapon
    struct weapon sword = {0, 10, 1};
    struct weapon claymore = {0, 5, 10};

    //inisialisasi status enemy
    struct enemy zombie = {100, 5, 5};
    struct enemy witch = {50, 25, 1};
    struct enemy org = {1000, 10, 20};

    //inisialisasi status player secara acak
    srand(time(NULL));
    struct player pemain;
    acakStatus(&pemain);
    printf("Status anda adalah\nHP = %d\nATK = %d\nDEF = %d\n", pemain.hp, pemain.atk, pemain.def);

    //Fitur pengacakan status player
    reroll(&pemain);

    //pemilihan senjata
    unsigned int perlengkapan;
    unsigned int statusPerlengkapan = 1;
    while(1){
        puts("Pilih Senjata:\n1. Sword\n2. Claymore");
        scanf("%d", &perlengkapan);
        switch (perlengkapan) {
            case 1:
            pemain.senjata = &sword;
            statusPerlengkapan = 0;
            break;
            case 2:
            pemain.senjata = &claymore;
            statusPerlengkapan = 0;
            break;
            default:
            puts("Masukan tidak valid. Ulangi!");
            break;
        }
        if (statusPerlengkapan == 0) break;
    }
    gunakanSenjata(&pemain);
    printf("Status anda adalah\nHP = %d\nATK = %d\nDEF = %d\n", pemain.hp, pemain.atk, pemain.def);

    //Penutup
    printf("Hello World");
    return 0;
}

void acakStatus(struct player* player){
    player->hp = (100 + rand() % 100);
    player->atk = (10 + rand() % 10);
    player->def = (10 + rand() % 10);
}

void reroll(struct player* player){
    unsigned int reroll = 3;
    unsigned int statusReroll = 1;
    char pilihan;
    while (1) {
        printf("Apakah Anda ingin reroll status? (%u kesempatan) (y/n): ", reroll);
        scanf(" %c", &pilihan);
        //switch untuk reroll]
        switch (pilihan) {
            case 'y':
            case 'Y':
            acakStatus(player);
            printf("Status baru Anda adalah\nHP = %d\nATK = %d\nDEF = %d\n", player->hp, player->atk, player->def);
            reroll--;
            break;
            case 'n':
            case 'N':
            statusReroll = 0;
            break;
            default:
            puts("Masukan tidak valid. Ulangi!");
            break;
        }
        if (reroll == 0 || statusReroll == 0) break;
    }
}

void gunakanSenjata(struct player* player){
    player->hp += player->senjata->hpBonus;
    player->atk += player->senjata->atkBonus;
    player->def += player->senjata->defBonus;
}