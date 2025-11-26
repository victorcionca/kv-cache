#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static const char *keys[] = {
    "first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "ninth", "tenth", "eleventh", "twelvth", "thirteenth", "fourteenth", "fifteenth", "sixteenth", "seventeenth", "eighteenth", "nineteenth", "twenty"
};


static const int num_keys = 20;

int main(int argc, char *argv[]) {
    Map *map = create_map();

    int value_size = 1000;
    unsigned char *value = malloc(value_size);
    memset(value, 'a', value_size);

    for (int i=0;i<10;i++){
        // Put keys and print LRU
        put(map, keys[i], value, value_size);
        print_lru(map);
        getchar();
    }

    // 10 random put/get operations
    for (int i=0;i<10;i++){
        int key_idx = random()%20;
        int operation = random()%2;
        if (operation == 0){
            printf("PUT %s\n", keys[key_idx]);
            put(map, keys[key_idx], value, value_size);
        }else{
            printf("GET %s: ", keys[key_idx]);
            Entry *e = get(map, keys[key_idx]);
            if (e) printf("%s\n", e->key);
            else printf("NULL\n");
        }
        print_lru(map);
        getchar();
    }
    return 0;
}
