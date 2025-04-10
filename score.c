#include "fonction.h"


void score_collision(Mario *mario,char grille[20][40],int *score){
  when (mario->pieces += 100){
    *score += 300;
   }
   when(mario-> vie == -1){
     *score -= 200;
   }

  }

  void score_avancement(Mario *mario, int *score,char map[20][40]){
   if (mario ->x == 0){
     *score = 0;
   }
   for (int i = 0; i < ; i++){
     if(mario ->x += 1){
     *score += rand() % 100;
   	}
   }

   void sauvegarde_score(int *score){
        FILE *fichier = fopen("score.txt", "w");
        if (fichier != NULL) {
            fprintf(fichier, "%d\n", *score);
            fclose(fichier);
        } else {
            printf("Erreur lors de l'ouverture du fichier.\n");
        }

   }


  void affichage_score(Mario *mario, int *score){
    printf("Score: %p\n", score);
  }