/* ************************************************************************** */
/*                                                                            */
/*    _   _ ____      _        header.h                                       */
/*   | | | |  _ \    / \                                                      */
/*   | | | | |_) |  / _ \      Mohammed Hrima                                 */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com                       */
/*    \___/|_| \_\/_/   \_\    created: 2023/03/11 03:08:09                   */
/*                                                                            */
/*   updated: 2026/10/20 05:13:09                                             */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Arena Arena;
struct Arena {
    unsigned char *buff;
    size_t size;
    size_t used;
    Arena *next;
};

static Arena *arena_head;
static Arena *arena_curr;

Arena *new_arena(size_t size) {
    Arena *new = calloc(1, sizeof(Arena));
    if (new == NULL) {
        fprintf(stderr, "%s:%d calloc failed\n", __FILE__, __LINE__);
        exit(1);
    }
    new->buff = calloc(size, 1);
    if (new->buff == NULL) {
        fprintf(stderr, "%s:%d calloc failed\n", __FILE__, __LINE__);
        exit(1);
    }
    new->size = size;
    return new;
}

void arena_reserve(size_t size) {
    Arena *curr = arena_curr;
    Arena *head = arena_head;
    if (curr != NULL && curr->size - curr->used >= size)
        return;

    Arena *new = new_arena((size + 16 - 1) / 16 * 16);
    if (head == NULL)
        curr = head = new;
    else
        curr = curr->next = new;

    arena_curr = curr;
    arena_head = head;
}

void *arena_alloc(size_t asked_count, size_t asked_size) {
    size_t asked = asked_count * asked_size;
    asked = (asked + 16 - 1) / 16 * 16;

    Arena *head = arena_head;
    Arena *curr = arena_curr;
    if (head == NULL || curr->used + asked >= curr->size) {
        size_t size = curr != NULL ? (curr->size << 1) : (1 << 8);
        while (size < asked)
            size <<= 1;
        Arena *new = new_arena(size);

        if (head == NULL)
            curr = head = new;
        else
            curr = curr->next = new;
    }
    void *res = curr->buff + curr->used;
    curr->used += asked;

    arena_head = head;
    arena_curr = curr;

    return res;
}

size_t arena_free() {
    size_t used = 0;
    Arena *arena = arena_head;
    while (arena != NULL) {
        used += sizeof(Arena);
        used += arena->used;
        Arena *next = arena->next;
        free(arena->buff);
        free(arena);
        arena = next;
    }
    return used;
}