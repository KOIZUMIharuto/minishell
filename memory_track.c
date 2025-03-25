#include <minishell.h>

t_mem_track *g_memory_list = NULL;

void *tracked_malloc(size_t size, const char *file, int line)
{
    void *ptr = malloc(size);
    
    t_mem_track *new_track = malloc(sizeof(t_mem_track));
    if (new_track) {
        new_track->ptr = ptr;
        new_track->file = strdup(file);
        new_track->line = line;
        new_track->next = g_memory_list;
        g_memory_list = new_track;
    }
    
    return ptr;
}

void tracked_free(void *ptr, const char *file, int line)
{
    if (!ptr) return;
    
    t_mem_track **curr = &g_memory_list;
    while (*curr) {
        if ((*curr)->ptr == ptr) {
            t_mem_track *to_free = *curr;
            *curr = (*curr)->next;
            free(to_free->file);
            free(to_free);
            break;
        }
        curr = &(*curr)->next;
    }
    
    free(ptr);
}

void print_memory_leaks(void)
{
    t_mem_track *curr = g_memory_list;
    int leak_count = 0;
    
    while (curr) {
        printf("LEAK: %p allocated at %s:%d\n", curr->ptr, curr->file, curr->line);
        curr = curr->next;
        leak_count++;
    }
    
    if (leak_count == 0)
        printf("No memory leaks detected\n");
    else
        printf("Total leaks: %d\n", leak_count);
} 