// standard headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "simple_mover.h"
#include "enemies.h"
#include "tile_handlers.h"
#include "loadscreen.h"
#include "images.h"

void add_goomba(uint8_t *tile) {
    simple_move_t *goomba = add_simple_mover(tile);
    
    goomba->hitbox.width = 15;
    goomba->hitbox.height = 15;
    goomba->vx = -1;
    goomba->type = GOOMBA_TYPE;
}

void add_reswob(uint8_t *tile) {
    simple_move_t *reswob = add_simple_mover(tile);
    
    reswob->hitbox.width = 31;
    reswob->hitbox.height = 39;
    reswob->vx = -1;
    reswob->type = RESWOB_TYPE;
}

boo_t *boo[MAX_BOOS];
uint8_t num_boos = 0;

void add_boo(uint8_t *tile) {
    boo_t *booooo;
    unsigned int x, y;
    
    if (num_boos > MAX_BOOS - 1) {
        return;
    }
    
    tile_to_abs_xy_pos(tile, &x, &y);
    
    booooo = boo[num_boos] = safe_malloc(sizeof(boo_t));
    
    booooo->x = x;
    booooo->y = y;
    booooo->vy = -1;
    booooo->dir = false;
    booooo->count = 0;
    num_boos++;
}

void remove_boo(uint8_t i) {
    boo_t *free_me = boo[i];
    uint8_t num_boos_less;
    
    if (!num_boos) { return; }
    
    num_boos_less = num_boos--;
    
    for(; i < num_boos_less; i++) {
        boo[i] = boo[i+1];
    }
    
    free(free_me);
}

static enum { KOOPA_GREEN, KOOPA_RED, KOOPA_GREEN_FLY, KOOPA_RED_FLY, KOOPA_BONES, SPIKE };

void add_shell_enemy(uint8_t *tile, uint8_t type) {
    simple_move_t *special = add_simple_mover(tile);
    
    special->hitbox.width = 15;
    special->hitbox.height = 26;
    special->vx = -1;
    switch(type) {
        case KOOPA_GREEN:
            special->type = KOOPA_GREEN_TYPE;
            special->smart = false;
            break;
        case KOOPA_RED:
            special->type = KOOPA_RED_TYPE;
            special->smart = true;
            break;
        case KOOPA_GREEN_FLY:
            special->type = KOOPA_GREEN_FLY_TYPE;
            goto flying_type;
        case KOOPA_RED_FLY:
            special->type = KOOPA_RED_FLY_TYPE;
flying_type:
            special->smart = true;
            special->vx = 0;
            special->vy = 1;
            special->is_flyer = true;
            break;
        case KOOPA_BONES:
            special->type = KOOPA_BONES_TYPE;
            special->smart = true;
            break;
        default: // spike type
            special->hitbox.height = 15;
            special->smart = false;
            special->type = SPIKE_TYPE;
            break;
    }
}

chomper_t *chomper[MAX_CHOMPERS];
uint8_t num_chompers = 0;

void add_chomper(uint8_t *tile, bool throws_fire) {
    chomper_t *chomp;
    unsigned int x, y;
    
    if (num_chompers > MAX_CHOMPERS - 1) {
        return;
    }
    
    tile_to_abs_xy_pos(tile, &x, &y);
    
    chomp = chomper[num_chompers] = safe_malloc(sizeof(chomper_t));
    
    chomp->x = x + TILE_WIDTH/2;
    chomp->y = y;
    chomp->start_y = y;
    chomp->vy = -2;
    chomp->throws_fire = throws_fire;
    chomp->count = 0;
    num_chompers++;
}

void remove_chomper(uint8_t i) {
    chomper_t *free_me = chomper[i];
    uint8_t num_chompers_less;
    
    if (!num_chompers) { return; }
    
    num_chompers_less = num_chompers--;
    
    for(; i < num_chompers_less; i++) {
        chomper[i] = chomper[i+1];
    }
    
    free(free_me);
}


flame_t *flame[MAX_FLAMES];
uint8_t num_flames = 0;

void add_flame(uint8_t *tile) {
    flame_t *fire;
    unsigned int x, y;
    
    if (num_flames > MAX_FLAMES - 1) {
        return;
    }
    
    tile_to_abs_xy_pos(tile, &x, &y);
    
    fire = flame[num_flames] = safe_malloc(sizeof(flame_t));
    
    fire->x = x;
    fire->y = y;
    fire->start_y = y;
    fire->vy = -15;
    fire->count = 0;
    num_flames++;
}

void remove_flame(uint8_t i) {
    flame_t *free_me = flame[i];
    uint8_t num_flames_less;
    
    if (!num_flames) { return; }
    
    num_flames_less = num_flames--;
    
    for(; i < num_flames_less; i++) {
        flame[i] = flame[i+1];
    }
    
    free(free_me);
}

thwomp_t *thwomp[MAX_THWOMPS];
uint8_t num_thwomps = 0;

void add_thwomp(uint8_t *tile) {
    thwomp_t *womp;
    unsigned int x, y;
    
    if (num_thwomps > MAX_THWOMPS - 1) {
        return;
    }
    
    tile_to_abs_xy_pos(tile, &x, &y);
    
    womp = thwomp[num_thwomps] = safe_malloc(sizeof(thwomp_t));
    
    womp->x = x + 4;
    womp->y = y;
    womp->start_y = y;
    womp->vy = 0;
    womp->count = 0;
    num_thwomps++;
}

void remove_thwomp(uint8_t i) {
    thwomp_t *free_me = thwomp[i];
    uint8_t num_thowmps_less;
    
    if (!num_thwomps) { return; }
    
    num_thowmps_less = num_thwomps--;
    
    for(; i < num_thowmps_less; i++) {
        thwomp[i] = thwomp[i+1];
    }
    
    free(free_me);
}

enemy_t *simple_enemy[MAX_SIMPLE_ENEMY];
uint8_t num_simple_enemies = 0;

enemy_t *add_simple_enemy(uint8_t *tile, uint8_t type) {
    enemy_t *enemy;
    unsigned int x, y;
    
    if (num_simple_enemies > MAX_SIMPLE_ENEMY - 1) {
        remove_simple_enemy(0);
    }
    
    tile_to_abs_xy_pos(tile, &x, &y);
    
    enemy = simple_enemy[num_simple_enemies] = safe_malloc(sizeof(enemy_t));
    
    enemy->vx = 0;
    enemy->vy = 0;
    enemy->counter = 100;
    enemy->y = y;
    enemy->x = x;
        
    switch(type) {
        case BULLET_TYPE:
            enemy->vx = -3;
            enemy->y = y + 1;
            enemy->x = x - 4;
            enemy->sprite = bullet_left;
            break;
        case SCORE_TYPE:
            enemy->counter = 16;
            enemy->sprite = bullet_left;
            break;
        case CANNONBALL_TYPE:
            enemy->sprite = cannonball_sprite;
            enemy->vx = -2;
            break;
        case LEAF_TYPE:
            enemy->y -= TILE_HEIGHT;
            enemy->counter = 16;
        case FISH_TYPE:
            enemy->vx = -1;
        default:
            break;
    }
    
    enemy->type = type;
    num_simple_enemies++;
    return enemy;
}

void remove_simple_enemy(uint8_t i) {
    enemy_t *free_me = simple_enemy[i];
    uint8_t num_simple_enemies_less;
    
    if (!num_simple_enemies) { return; }
    
    num_simple_enemies_less = num_simple_enemies--;
    
    for(; i < num_simple_enemies_less; i++) {
        simple_enemy[i] = simple_enemy[i+1];
    }
    
    free(free_me);
}

void get_enemies(void) {
    uint8_t width = tilemap.width;
    uint8_t height = tilemap.height;
    unsigned int j, delay;
    unsigned int loop = width * height;
    
    for(j = 0; j < loop; j++) {
        uint8_t *this = tilemap.map + j;
        uint8_t tile = *this;
        int8_t tmp1, tmp2;
        
        switch(tile) {
            // this case is just to avoid another function that converts coins in water to water coins
            case TILE_COIN:
                for (tmp2=-1; tmp2<2; tmp2++) {
                    int off = tmp2 * width;
                    bool at_neg_1 = tmp2 == -1;
                    for (tmp1=-1; tmp1<2; tmp1++) {
                        tile = *(this+tmp1+off);
                        if (tile == TILE_WATER || tile == TILE_WATER_COIN || (at_neg_1 && tile == TILE_WATER_TOP)) {
                            *this = TILE_WATER_COIN;
                            goto end_loops;
                        }
                    }
                }
            end_loops:
                break;
            case 0x61:
                add_simple_enemy(this, CANNONBALL_DOWN_CREATOR_TYPE);
                break;
            case 0x53:
                add_simple_enemy(this, CANNONBALL_UP_CREATOR_TYPE);
                break;
            case 0x46:
                add_simple_enemy(this, BULLET_CREATOR_TYPE);
                break;
            case TILE_E_ORIAM_START:
                tile_to_abs_xy_pos(this, (unsigned int*)&oiram.x, (unsigned int*)&oiram.y);
                if (oiram.flags & FLAG_OIRAM_BIG) {
                    oiram.y -= TILE_HEIGHT + 2;
                }
                goto set_empty_tile;
            case TILE_E_RESWOB:
                add_reswob(this);
                goto set_empty_tile;
            case TILE_E_FISH:
                add_simple_enemy(this, FISH_TYPE);
                *this = TILE_WATER;
                break;
            case TILE_E_GOOMBA:
                add_goomba(this);
                goto set_empty_tile;
            case TILE_E_SPIKE:
            case TILE_E_GREEN_KOOPA:
            case TILE_E_RED_KOOPA:
            case TILE_E_GREEN_FLY_KOOPA:
            case TILE_E_RED_FLY_KOOPA:
            case TILE_E_BONES_KOOPA:
                add_shell_enemy(this, tile - TILE_E_GREEN_KOOPA);
                goto set_empty_tile;
            case TILE_E_THWOMP:
                add_thwomp(this);
                goto set_empty_tile;
            case TILE_E_LAVA_FIREBALL:
                add_flame(this);
                *this = TILE_LAVA_TOP;
                break;
            case TILE_E_CHOMPER:
            case TILE_E_FIRE_CHOMPER:
                add_chomper(this + width, tile == TILE_E_FIRE_CHOMPER);
                goto set_empty_tile;
            case TILE_E_BOO:
                add_boo(this);
set_empty_tile:
                *this = TILE_EMPTY;
                break;
            default:
                break;
        }
    }
}
