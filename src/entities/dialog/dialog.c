/*
** EPITECH PROJECT, 2020
** npc
** File description:
** Entity
*/

#include <SFML/Window/Event.h>
#include <SFML/Window/Keyboard.h>
#include <stdio.h>
#include "stdlib.h"
#include "distract/game.h"
#include "distract/entity.h"
#include "distract/resources.h"
#include "distract/graphics.h"
#include "distract/debug.h"
#include "distract/util.h"
#include "SFML/Window.h"
#include "SFML/Graphics.h"
#include "myrpg/entities.h"
#include "myrpg/asset.h"
#include "myrpg/define.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Types.h>
#include "distract/math.h"

bool create_dialog(game_t *game UNUSED, entity_t *entity)
{
    dialog_t *dialog = dcalloc(sizeof(dialog_t), 1);
    sfIntRect rect = IRECT(0, 0, 16, 16);
    sfTexture *texture = create_texture(game, "asset/icon/talk.png", &rect);
    entity_t *dialogbox = get_entity(game, DIALOGBOX);

    D_ASSERT(dialog, NULL, "Cannot create dialog", false);
    D_ASSERT(texture, NULL, "Cannot create dialog texture", false);
    D_ASSERT(dialogbox, NULL, "Cannot get dialogbox entity for dialog", false);
    dialog->entity = entity;
    dialog->clock = create_pausable_clock(game);
    dialog->sprite = create_sprite(texture, NULL);
    dialog->box = dialogbox->instance;
    D_ASSERT(dialog->clock, NULL, "Cannot create dialog clock", false);
    D_ASSERT(dialog->sprite, NULL, "Cannot create dialog sprite", false);
    entity->instance = dialog;
    entity->z = 100;
    return (true);
}

void destroy_dialog(game_t *game UNUSED, entity_t *entity)
{
    dialog_t *dialog = entity->instance;

    sfSprite_destroy(dialog->sprite);
    destroy_pausable_clock(dialog->clock);
    free(dialog);
}

void update_dialog(game_t *game UNUSED, entity_t *entity)
{
    dialog_t *dialog = entity->instance;

    sfSprite_setPosition(dialog->sprite, entity->pos);
    tick_pausable_clock(dialog->clock);
}

void draw_npc(game_t *game UNUSED, entity_t *entity)
{
    dialog_t *dialog = entity->instance;
    entity_t *heroentity = get_entity(game, HERO);
    sfVector2f pos;

    if (dialog->hero == NULL) {
        dialog->hero = heroentity->instance;
        return;
    }
    pos = dialog->hero->entity->pos;
    if (v2fdistance(&pos, &entity->pos) < 50)
        sfRenderWindow_drawSprite(game->window, dialog->sprite, NULL);
}

bool handle_npc_events(game_t *game UNUSED,
    entity_t *entity UNUSED, sfEvent *event UNUSED)
{
    dialog_t *dialog = entity->instance;
    entity_t *heroentity = get_entity(game, HERO);
    sfVector2f pos;

    if (dialog->hero == NULL) {
        dialog->hero = heroentity->instance;
        return (false);
    }
    pos = dialog->hero->entity->pos;
    if (v2fdistance(&pos, &entity->pos) >= 50)
        return (false);
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeySpace
        && !dialog->box->is_visible
        && dialog->clock->time > 0.02f) {
        show_dialog(dialog);
        dialog->clock->time = 0;
        return (true);
    }
    return (false);
}