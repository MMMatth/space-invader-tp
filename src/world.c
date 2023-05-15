#include "../include/world.h"


void init_data(world_t * world){
    world->joueur = malloc(sizeof(sprite_t));
    init_sprite(world->joueur, SCREEN_WIDTH / 2 - SHIP_SIZE / 2, SCREEN_HEIGHT - SHIP_SIZE - 15, SHIP_SIZE, SHIP_SIZE);
    world->ligne_arrivee = malloc(sizeof(sprite_t));
    init_sprite(world->ligne_arrivee, 0, FINISH_LINE_HEIGHT, SCREEN_WIDTH, 1);
    world->meteors = malloc(sizeof(meteors_t));
    init_meteors(world, "map.txt");
    world->projectiles = malloc(sizeof(projectile_t*) * (MAX_PROJECTILE+1) );
    init_projectiles(world);
    world->vitesse = INITIAL_SPEED;
    world->chrono = 0;
    world->gameover = 0; // le jeu n'est pas fini
    world->phase = 0;

    /* ANIMATIONS */
    world->explode_animate = malloc(sizeof(animate_t*) * MAX_ANIM);
    init_animates("assets/img/explode_animate", 6, 50, world->explode_animate, 0);

    world->speed_animate = malloc(sizeof(animate_t*) * MAX_ANIM);
    init_animate("assets/img/speed_animate", 9, 100, world->speed_animate, -1);

}

void check_pos(world_t *world){ // vérifie que le joueur ne sort pas de l'écran
    if(world->joueur->x < 0){ // si le joueur sort de l'écran à gauche
        world->joueur->x = 0; // on le replace à gauche
    }
    if(world->joueur->x > SCREEN_WIDTH - SHIP_SIZE){ // si le joueur sort de l'écran à droite
        world->joueur->x = SCREEN_WIDTH - SHIP_SIZE; // on le replace à droite
    }
    if(world->joueur->y < 0){ // si le joueur sort de l'écran en haut
        world->joueur->y = 0; // on le replace en haut
    }
    if(world->joueur->y > SCREEN_HEIGHT - SHIP_SIZE){ // si le joueur sort de l'écran en bas
        world->joueur->y = SCREEN_HEIGHT - SHIP_SIZE; // on le replace en bas
    }
}

void handle_sprites_collision(world_t *world, sprite_t *sp1, sprite_t *sp2){
    if(sprites_collide(sp1, sp2)){
        world->phase = 2;
    }
}

void clean_data(world_t *world){
    free(world->joueur);
    free(world->ligne_arrivee);
    clean_projectile(world);
    clean_meteors(world);
    clean_animates(world->explode_animate);
    clean_animate(world->speed_animate);
}


void update_data(world_t *world){
    world->ligne_arrivee->y += world->vitesse;
    update_meteors(world);
    update_projectile(world);
    check_pos(world);
    handle_sprites_collision(world, world->joueur, world->ligne_arrivee);
    est_perdu(world);
    handle_projectile(world);
    update_animates(world, world->explode_animate);
    activate_speed_animate(world);
}   

int is_game_over(world_t *world){
    return world->gameover;
}

int speed_animate = 0;

void activate_speed_animate(world_t *world){
    if (world->vitesse > 3){
        if (speed_animate == 0){
            start_animate(world->speed_animate, 0, 0);
            speed_animate = 1;
        }
    }else{
        pause_animate(world->speed_animate);
        speed_animate = 0;
    }
}