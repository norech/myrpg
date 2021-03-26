/*
** EPITECH PROJECT, 2021
** load_vertex_array_map
** File description:
** load
*/

#include "myrpg/map.h"
#include "distract/resources.h"

static bool load_vertex_array_map_data(ig_map_t *map,
    struct iron_goat_layer *layer, struct vector_texture *textures,
    struct vector_vertex_array_data *vertexmap)
{
    if (load_vertex_array_map_get_tileset(map, layer,
                &vertexmap->tileset) == false)
        return (false);
    if (check_tileset_and_data(
        layer, &map->tilesets->data[vertexmap->tileset]) == false)
        return (false);
    load_vertex_data(textures->tileset[vertexmap->tileset],
        &MAP_RECT(map, textures, vertexmap->tileset),
        &vertexmap->vertex, layer->data->data);
    return (true);
}

static bool load_vertex_array_map_verticies(ig_map_t *map,
        struct vertex_array_map *self)
{
    self->v_vertex.size = map->layers->size;

    self->v_vertex.vec = ecalloc(sizeof(struct vector_vertex_array_data),
            map->layers->size);
    for (usize_t i = 0; i < self->v_vertex.size; i++) {
        self->v_vertex.vec[i].vertex = sfVertexArray_create();
        if (self->v_vertex.vec[i].vertex == NULL) {
            ASSERT("Vertex load", "Allocation error");
            return (false);
        }
        if (load_vertex_array_map_data(map, &map->layers->data[i],
                    &self->v_texture, &self->v_vertex.vec[i]) == false)
            return (false);
    }
    return (true);
}

static bool load_vertex_array_map_tilesets(game_t *game,
    ig_map_t *map, struct vector_texture *self, char *pathfolder)
{
    char *file = NULL;

    self->size = map->tilesets->size;
    self->tileset = ecalloc(sizeof(sfTexture *), self->size);
    for (usize_t i = 0; i < self->size; i++) {
        file = eformat("%s%s", pathfolder, map->tilesets->data[i].image);
        if (file == NULL) {
            ASSERT("Vertex load", "Allocation error");
            return (false);
        }
        self->tileset[i] = create_texture(game, file, NULL);
        if (self->tileset[i] == NULL) {
            ASSERT("Vertex load", "Allocation error");
            return (false);
        }
        FREE(file);
    }
    return (true);
}

bool load_vertex_array_map(game_t *game, ig_map_t *map,
    struct vertex_array_map *self, char *pathfolder)
{
    if (load_vertex_array_map_tilesets(game, map,
                &self->v_texture, pathfolder) == false)
        return (false);
    if (load_vertex_array_map_verticies(map, self) == false)
        return (false);
    return (true);
}