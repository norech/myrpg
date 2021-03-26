/*
** EPITECH PROJECT, 2021
** map
** File description:
** map
*/


#ifndef __MY_RPG__MAP__H__
    #define __MY_RPG__MAP__H__

    #include <iron_goat/deser.h>
    #include <SFML/Graphics.h>
    #include "distract/game.h"

    struct vector_vertex_array_data {
        usize_t tileset;
        sfVertexArray *vertex;
    };

    struct vector_vertex_array {
        struct vector_vertex_array_data *vec;
        size_t size;
    };

    struct vector_texture {
        sfTexture **tileset;
        size_t size;
    };

    typedef struct vertex_array_map {
        struct vector_vertex_array v_vertex;
        struct vector_texture v_texture;
    } vertex_map_t;

    typedef struct map_rect {
        uint64_t mapx;
        uint64_t mapy;
        uint64_t tilex;
        uint64_t tiley;
    } map_rect_t;

    struct tile_id_rot {
        u16_t rot;
        u64_t id;
    };

    struct tile_id_rot get_real_tile_id_and_rotation(u64_t id);
    void draw_vertex_map(sfRenderWindow *window,
        struct vertex_array_map *self);

    #define DEFAULT_RENDERSTATE(texturedata) \
        (sfRenderStates){ \
            .blendMode = sfBlendAlpha, \
            .shader = NULL, \
            .transform = sfTransform_Identity, \
            .texture = texturedata \
        }

    #define MAP_RECT(map, textures, tilesetv) \
        (map_rect_t){map->height, map->width, \
            map->tilesets->data[tilesetv].tilewidth, \
            map->tilesets->data[tilesetv].tileheight}

    bool load_vertex_array_map(game_t *game, ig_map_t *map,
        struct vertex_array_map *self, char *pathfolder);

    // Integrity checks
    bool load_vertex_array_map_get_tileset(ig_map_t *map,
        struct iron_goat_layer *layer, usize_t *tileset);
    bool check_tileset_and_data(struct iron_goat_layer *layer,
        struct iron_goat_tileset *tileset);

    void load_vertex_data(sfTexture *tileset, map_rect_t *size,
        sfVertexArray **vertex, u64_t *data);

#endif