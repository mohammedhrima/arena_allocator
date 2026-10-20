/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 21:02:32 by mhrima            #+#    #+#             */
/*   Updated: 2023/11/28 21:05:29 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

// Counts the words of a text. Every word copy and every list node comes from
// the arena: nothing is freed one by one, a single arena_free() ends it all.

typedef struct Word Word;
struct Word {
    char *text;
    int count;
    Word *next;
};

// a copy of s[0..len) in the arena; the memory is zeroed, so it ends with '\0'
char *arena_strndup(const char *s, size_t len) {
    char *copy = arena_alloc(len + 1, 1);
    memcpy(copy, s, len);
    return copy;
}

// one more of the word s[0..len): its count goes up, or it joins the list
Word *count_word(Word *list, const char *s, size_t len) {
    for (Word *w = list; w != NULL; w = w->next) {
        if (strlen(w->text) == len && strncmp(w->text, s, len) == 0) {
            w->count++;
            return list;
        }
    }
    Word *w = arena_alloc(1, sizeof(Word));
    w->text = arena_strndup(s, len);
    w->count = 1;
    w->next = list;
    return w;
}

int main(void) {
    const char *text = "the cat sat on the mat and the dog sat on the cat";

    arena_reserve(1024); // room for the whole batch: it all lands in one block
    Word *words = NULL;
    for (const char *s = text; *s != '\0';) {
        while (*s == ' ')
            s++;
        const char *e = s;
        while (*e != '\0' && *e != ' ')
            e++;
        if (e > s)
            words = count_word(words, s, e - s);
        s = e;
    }

    for (Word *w = words; w != NULL; w = w->next)
        printf("%-4s %d\n", w->text, w->count);

    // no free per word or per node: one call releases everything
    printf("memory used: %zu bytes\n", arena_free());
    return 0;
}
