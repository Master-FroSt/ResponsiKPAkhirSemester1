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

//inisialisasi struct
struct weapon sword = {0, 15, 1};
struct weapon claymore = {0, 5, 10};
struct enemy musuh[6];

//function prototype
void startScreen();
void endScreen();
void initMusuh();
void acakStatus(struct player* player);
void reroll(struct player* player);
void gunakanSenjata(struct player* player);
void serang(struct player* player, struct enemy* enemy, int isPlayerAttacking);
void statusMusuh(struct enemy musuh[], int lawan);

int main(){
    //Layar permulaan
    int menuAwal;
    startScreen();

    //Perulangan permainan
    int menuAkhir;
    int retry = 1;
    while (retry) {
        //inisialisasi status player secara acak
        srand(time(NULL));
        struct player pemain;
        acakStatus(&pemain);

        //Fitur pengacakan status player
        reroll(&pemain);

        //pemilihan senjata
        unsigned int perlengkapan;
        unsigned int statusPerlengkapan = 1;
        while(1){
            puts("Pilih Senjata:\n1. Sword (+15atk, +1def)\n2. Claymore(+5atk, +10def)");
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
        
        //Aksi secara turn-based
        getchar();//Menangkap masukan sebelumnya (jika ada)
        unsigned int turn = 0;
        unsigned int lawan = 0;
        initMusuh();
        while(lawan < 6) {
            printf("Status anda sekarang adalah\nHP = %d\nATK = %d\nDEF = %d\n", pemain.hp, pemain.atk, pemain.def);
            statusMusuh(musuh, lawan);
            puts("Tekan enter untuk bertarung...");
            getchar(); //Menunggu masukan untuk memulai
            while(pemain.hp > 0 && musuh[lawan].hp > 0) {
                if (turn % 2 == 0) {
                    serang(&pemain, &musuh[lawan], 1);
                } else {
                    serang(&pemain, &musuh[lawan], 0);
                }
                turn++;
            }
            if(pemain.hp < 0) break;
            if(musuh[lawan].hp < 0) lawan++;
        }

        //Penutup
        if(pemain.hp < 0) {
            puts("Anda kalah!");
            printf("Jumlah giliran yang terjadi: %u\n", turn);
            printf("Musuh yang anda kalahkan: %d\n", lawan);
        }
        endScreen();
        scanf("%d", &menuAkhir);
        switch (menuAkhir) {
            case 1:
                puts("Anda memilih menu Bermain lagi!");
                break;
            case 2:
                puts("Sampai jumpa lagi!");
                retry = 0;
                break;
            default:
                puts("Masukan tidak valid. Ulangi!");
        }
    }
    return 0;
}

void startScreen() {
    puts("Selamat Datang di permainan Tebas!");
    puts("----------------------------------");
}

void endScreen() {
    puts("Terima kasih sudah bermain Tebas!");
    puts("-------------------------------");
    puts("1. Bermain lagi");
    puts("2. Keluar");
    puts("-------------------------------");
    puts("Pilih menu: ");
}

void initMusuh() {
    srand(time(NULL));
    for (int i = 0; i < 6; i++) {
        if (i == 3 || i == 5) {
            musuh[i].hp = 1000;
            musuh[i].atk = 40;
            musuh[i].def = 20; // Ogre (boss)
        } else {
            int jenisMusuh = rand() % 2;
            if (jenisMusuh == 0) {
                musuh[i].hp = 100;
                musuh[i].atk = 25;
                musuh[i].def = 5; // Zombie
            } else {
                musuh[i].hp = 50;
                musuh[i].atk = 50;
                musuh[i].def = 0; // Witch
            }
        }
    }
}

void acakStatus(struct player* player){
    player->hp = (1000 + rand() % 1000);
    player->atk = (20 + rand() % 20);
    player->def = (0 + rand() % 20);
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

void serang(struct player* player, struct enemy* enemy, int isPlayerAttacking){
    int damage;
    if (isPlayerAttacking == 1) {
        damage = player->atk - enemy->def;
        enemy->hp -= damage;
        printf("Anda menyerang musuh dengan damage %d!\n", damage);
        if(enemy->hp > 0) {
            printf("HP musuh sekarang: %d\n", enemy->hp);
        } else {
            puts("HP musuh habis, silakan lanjut!");
        }
    } else {
        damage = enemy->atk - player->def;
        player->hp -= damage;
        printf("Musuh menyerang Anda dengan damage %d!\n", damage);
        if(player->hp > 0) {
            printf("HP Anda sekarang: %d\n", player->hp);
        } else {
            puts("HP Anda habis, anda telah dikalahkan!");
        }
    }
}

void statusMusuh(struct enemy musuh[], int lawan) {
    printf("Musuh ke-%d/6:\n", lawan+1);
    printf("HP: %d\n", musuh[lawan].hp);
    printf("ATK: %d\n", musuh[lawan].atk);
    printf("DEF: %d\n", musuh[lawan].def);
}