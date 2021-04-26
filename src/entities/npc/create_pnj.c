/*
** EPITECH PROJECT, 2020
** pnj
** File description:
** Entity
*/

#include "stdlib.h"
#include "distract/game.h"
#include "distract/entity.h"
#include "distract/resources.h"
#include "distract/graphics.h"
#include "distract/debug.h"
#include "SFML/Window.h"
#include "SFML/Graphics.h"
#include "myrpg/entities.h"
#include "myrpg/asset.h"
#include "myrpg/define.h"
#include "myrpg/pnj.h"
#include "myrpg/util.h"

static bool creat_loader_part(game_t *game, pnj_t **pnj)
{
    struct json pnj_conf = {0};
    ig_map_t map = get_game_state(game)->map;
    char *file = NULL;

    if (load_property_string(map.properties, &file, "pnj_files",
        "could not load map property in pnj.c") == false)
        return (false);
    D_ASSERT(json_parser(&pnj_conf, file),
        false, "could not json parse pnj_conf", false);
    if (load_pnj_data_loop(&pnj_conf, &(*pnj)->pnj_vector) == false) {
        destroy_json(&pnj_conf);
        return (false);
    }
    destroy_json(&pnj_conf);
    return (true);
}

bool set_pnj_loop(game_t *game, pnj_t **pnj)
{
    sfTexture *texture;

    for (usize_t i = 0; i < (*pnj)->pnj_vector->size; i++) {
        texture = create_texture(game, (*pnj)->pnj_vector->data[i].file_path,
            NULL);
        (*pnj)->pnj_vector->data[i].rect_size =
            VEC2U((*pnj)->pnj_vector->data[i].rect.width,
            (*pnj)->pnj_vector->data[i].rect.height);
        D_ASSERT(texture, NULL, "could not load pnj texture", false);
        (*pnj)->pnj_vector->data[i].sprite = create_sprite(texture,
            &(*pnj)->pnj_vector->data[i].rect);
        sfSprite_setPosition((*pnj)->pnj_vector->data[i].sprite,
            (*pnj)->pnj_vector->data[i].pos);
    }
    return (true);
}

bool create_pnj(game_t *game UNUSED, entity_t *entity)
{
    pnj_t *pnj = ecalloc(sizeof(pnj_t), 1);

    D_ASSERT(pnj, NULL, "error on ecalloc on pnj", false);
    if (!creat_loader_part(game, &pnj) || !set_pnj_loop(game, &pnj))
        return (false);
    pnj->clock = create_pausable_clock(game);
    D_ASSERT(pnj->clock, NULL, "could not create pnj clock", false);
    pnj->entity = entity;
    pnj->entity->z = 1000;
    entity->instance = pnj;
    return (true);
}

void destroy_pnj(game_t *game UNUSED, entity_t *entity)
{
    pnj_t *pnj = entity->instance;

    destroy_pausable_clock(pnj->clock);
    if (pnj->pnj_vector != NULL)
        pnj->pnj_vector->clear(&pnj->pnj_vector);
    free(pnj);
}

void draw_pnj(game_t *game UNUSED, entity_t *entity)
{
    pnj_t *pnj = entity->instance;
    sfIntRect rect;

    tick_pausable_clock(pnj->clock);
    for (usize_t i = 0; i < pnj->pnj_vector->size; i++)
        sfRenderWindow_drawSprite(game->window,
            pnj->pnj_vector->data[i].sprite, NULL);
    if (pnj->clock->time > 0.4) {
        for (usize_t i = 0; i < pnj->pnj_vector->size; i++) {
            rect = sfSprite_getTextureRect(pnj->pnj_vector->data[i].sprite);
            rect.left += pnj->pnj_vector->data[i].rect_size.x;
            if (rect.left == (int)(pnj->pnj_vector->data[i].rect_size.x *
                (pnj->pnj_vector->data[i].frame)))
                rect.left = 0;
            sfSprite_setTextureRect(pnj->pnj_vector->data[i].sprite, rect);
        }
        restart_pausable_clock(pnj->clock);
    }
}