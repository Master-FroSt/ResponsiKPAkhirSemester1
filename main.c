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
void statusPemain(struct player* player);
void menungguAksi();

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
        statusPemain(&pemain);

        //Fitur pengacakan status player
        reroll(&pemain);

        //pemilihan senjata
        unsigned int perlengkapan;
        unsigned int statusPerlengkapan = 1;
        while(1){
            printf("║ 1. Sword (+15atk, +1def)\n║ 2. Claymore(+5atk, +10def)\n║ Pilih Senjata:");
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
                puts("║ Masukan tidak valid. Ulangi!");
                break;
            }
            if (statusPerlengkapan == 0) break;
        }
        gunakanSenjata(&pemain);
        menungguAksi();
        
        //Aksi secara turn-based
        getchar();//Menangkap masukan sebelumnya (jika ada)
        unsigned int turn = 0;
        unsigned int lawan = 0;
        initMusuh();
        while(lawan < 6) {
            statusPemain(&pemain);
            statusMusuh(musuh, lawan);
            menungguAksi();
            puts("⚔️ Pertarungan dimulai! ⚔️");
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
        puts("");
        puts("║ Hasil Pertarungan: ");
        printf("║ Jumlah giliran yang terjadi: %u ⏰\n", turn);
        printf("║ Musuh yang anda kalahkan: %d 👊\n", lawan);
        menungguAksi();
        endScreen();
        scanf("%d", &menuAkhir);
        switch (menuAkhir) {
            case 1:
                puts("║ Anda memilih menu Bermain lagi!");
                menungguAksi();
                break;
            case 2:
                puts("║ Sampai jumpa lagi!");
                retry = 0;
                break;
            default:
                puts("║ Masukan tidak valid. Ulangi!");
        }
    }
    return 0;
}

void startScreen() {
    puts("║ Selamat Datang di permainan Tebas!");
    puts("║ ----------------------------------");
}

void endScreen() {
    puts("║ Terima kasih sudah bermain Tebas!");
    puts("║ ---------------------------------");
    puts("║ 1. Bermain lagi");
    puts("║ 2. Keluar");
    puts("║ ---------------------------------");
    printf("║ Pilih menu: ");
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
        printf("║ Apakah Anda ingin reroll status? (%u kesempatan) (y/n): ", reroll);
        scanf(" %c", &pilihan);
        //switch untuk reroll]
        switch (pilihan) {
            case 'y':
            case 'Y':
            acakStatus(player);
            statusPemain(player);;
            reroll--;
            break;
            case 'n':
            case 'N':
            statusReroll = 0;
            break;
            default:
            puts("║ Masukan tidak valid. Ulangi!");
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
        printf("║ Anda menyerang musuh dengan damage %d! 💥\n", damage);
        if(enemy->hp > 0) {
            printf("║ HP musuh sekarang: %d 💔\n", enemy->hp);
        } else {
            puts("║ HP musuh habis, silakan lanjut! 👏");
            menungguAksi();
        }
    } else {
        damage = enemy->atk - player->def;
        player->hp -= damage;
        printf("║ Musuh menyerang Anda dengan damage %d! 🔥\n", damage);
        if(player->hp > 0) {
            printf("║ HP Anda sekarang: %d ❤️\n", player->hp);
        } else {
            puts("║ HP Anda habis, Anda telah dikalahkan! 😱");
            menungguAksi();
        }
    }
}

void statusMusuh(struct enemy musuh[], int lawan) {
    printf("    Musuh ke-%d/6:\n", lawan+1);
    printf("╔═══════════════════╗\n");
    printf("║  Status  ║  Nilai ║\n");
    printf("╠═══════════════════╣\n");
    printf("║  HP      ║  %-5d ║\n", musuh[lawan].hp);
    printf("║  ATK     ║  %-5d ║\n", musuh[lawan].atk);
    printf("║  DEF     ║  %-5d ║\n", musuh[lawan].def);
    printf("╚═══════════════════╝\n");
}

void statusPemain(struct player* player) {
    printf("  👊 Status Anda 👊\n");
    printf("╔═══════════════════╗\n");
    printf("║  Status  ║  Nilai ║\n");
    printf("╠═══════════════════╣\n");
    printf("║  HP      ║  %-5d ║\n", player->hp);
    printf("║  ATK     ║  %-5d ║\n", player->atk);
    printf("║  DEF     ║  %-5d ║\n", player->def);
    printf("╚═══════════════════╝\n");
}

void menungguAksi() {
    puts("║ Tekan enter untuk melanjutkan...");
    getchar(); //Menunggu masukan untuk memulai
}