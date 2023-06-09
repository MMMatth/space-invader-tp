#include "../include/projectile.h"



void init_projectiles(world_t * world){
    for (int i = 0; i < MAX_PROJECTILE; i++){
        world->projectiles[i] = malloc(sizeof(projectile_t)); // on alloue la memoire pour chaque projectile
        world->projectiles[i]->sprite = malloc(sizeof(sprite_t)); // on alloue la memoire pour chaque sprite 
        world->projectiles[i]->active = false; // le projectile n'est pas actif
        world->projectiles[i]->x = -1; // on initialise la position du projectile
        world->projectiles[i]->y = -1;
        world->projectiles[i]->variation = 0; // on initialise la variation de position
        // on initialise le sprite
        init_sprite(world->projectiles[i]->sprite, world->projectiles[i]->x, world->projectiles[i]->y, PROJECTILE_SIZE/2, PROJECTILE_SIZE);
    }
}

void tirer(world_t *world){
    int i = 0;
    // on cherche une place vide dans le tableau après tout les tir deja actif
    while(i < MAX_PROJECTILE && world->projectiles[i]->active){i++;}
    world->projectiles[i]->active = 1; // on passe le projectile en actif
    world->projectiles[i]->x = world->joueur->x + SHIP_SIZE / 2 - PROJECTILE_SIZE / 4; // on le place au centre du vaisseau
    world->projectiles[i]->y = world->joueur->y ; // on le place au dessus du vaisseau
    world->projectiles[i]->vitesse_y = 10; // on lui donne une vitesse
    world->projectiles[i]->vitesse_x = 0.5; 
}

void colide_btw_projectile_and_meteor(world_t *world, int index_projectile, int index_meteor){
    if (world->meteors->tab_meteor[index_meteor] != NULL){ // on verifie que le meteor existe bien
        if (sprites_collide(world->projectiles[index_projectile]->sprite, world->meteors->tab_meteor[index_meteor])){ // si il y'a collision
            start_animates(world->explode_animate, world->meteors->tab_meteor[index_meteor]->x, world->meteors->tab_meteor[index_meteor]->y); // on affiche l'animation d'explosion
            world->projectiles[index_projectile]->active = 0; // on desactive le projectile
            world->projectiles[index_projectile]->x = -1; // on le bouge loin
            world->projectiles[index_projectile]->y = -1;
            world->meteors->tab_meteor[index_meteor] = NULL; // on passe l'asteroide en NULL
        }
    }    
}

void est_dehors(projectile_t *projectile){
    if (projectile->y < 0){ // si le projectile sort de l'ecran
        projectile->active = 0; // on le desactive
    }
}


void handle_projectile(world_t *world){
    for (int i = 0; i < MAX_PROJECTILE; i++){
        if (world->projectiles[i]->active){ // on parcour les projectile actif
            est_dehors(world->projectiles[i]); // on verifie si il depace l'ecran et le desactive si c'est le cas
            for (int j = 0; j < world->meteors->nb_meteor; j++){
                colide_btw_projectile_and_meteor(world, i, j); // on verifie si il y'a collision et agit en consequence
            }
        }
    }
}

void update_projectile(world_t *world){
    for (int i = 0; i < MAX_PROJECTILE; i++){
        if (world->projectiles[i]->active){ // on parcours les projectiles actifs
            // on met a jour les coordonnées du projectile
            world->projectiles[i]->y -= world->projectiles[i]->vitesse_y; // on déplace les projectiles a une certaine vitesse
            world->projectiles[i]->variation -= world->projectiles[i]->vitesse_x;
            
            // on met a jour les coordonnées du sprite
            world->projectiles[i]->sprite->x = world->projectiles[i]->x + sin(world->projectiles[i]->variation) * 2; // on utilise sin pour que la variation varie entre -1 et 1 et on le mutiple par 2 pour augmenter l'amplitude du mouvement
            world->projectiles[i]->sprite->y = world->projectiles[i]->y;
        }
    }
}

void apply_projectile(SDL_Renderer *renderer, world_t *world, resources_t *resources){
    for (int i = 0; i < MAX_PROJECTILE; i++){ // on parcours les projectiles
        if (world->projectiles[i]->active){ // on applique le sprite si le projectile est actif
            apply_sprite(renderer, resources->laser, world->projectiles[i]->sprite);
        }
    }
}


void clean_projectile(world_t * world){
    for (int i = 0; i < MAX_PROJECTILE; i++){
        free(world->projectiles[i]->sprite); // on libere la memoire pour chaque sprite
        free(world->projectiles[i]); // on libere la memoire pour chaque projectile
    }
    free(world->projectiles); // on libere la memoire pour le tableau de projectile
}
