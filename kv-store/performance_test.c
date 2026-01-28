#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define VALUE_SIZE 10

static const char *keys[] = {
    "zero",
    "first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "ninth", "tenth", "eleventh", "twelvth", "thirteenth", "fourteenth", "fifteenth", "sixteenth", "seventeenth", "eighteenth", "nineteenth"
};

static const char *string_tenths[] = {
    "twenty", "thirty", "fourty", "fifty", "sixty", "seventy", "eighty","ninety"
};

static const int num_keys = 20;

static char* number_to_word(int nnumber, char *number) {
    if (nnumber < 20) return keys[nnumber];
    int thousands = nnumber/1000;
    int hundreds = (nnumber%1000)/100;
    int tenths = (nnumber%100)/10;
    int digits = nnumber%10;
    int number_len = 0;
    if (thousands > 0){
        sprintf(number, "%sthousand", keys[thousands]);
        number_len += strlen(keys[thousands])+8;
    }
    if (hundreds > 0){
        sprintf(number, "%shundred", keys[hundreds]);
        number_len += strlen(keys[hundreds])+7;
    }
    if (tenths >= 2) {
        sprintf(number+number_len, "%s", string_tenths[tenths-2]);
        number_len += strlen(string_tenths[tenths-2]);
    }
    if (digits > 0){
        sprintf(number+number_len, "%s", keys[digits]);
        number_len += strlen(keys[digits]);
    }
    number[number_len] = 0;
    return number;
}

int main(int argc, char *argv[]) {
    // Build a large set of keys that will fill the table
    // Pre-generate so we don't incur delays at test time
    char *table_keys[MAP_CAPACITY];
    for (int i=0;i<MAP_CAPACITY;i++){
        table_keys[i] = malloc(50);
        number_to_word(i, table_keys[i]);
    }
    int value_length = VALUE_SIZE;
    unsigned char *value = malloc(value_length);
    memset(value, 'a', value_length);

    Map *map = create_map();

    // Performance evaluation starts here

    struct timeval start_fill, end_fill, end_query;

    gettimeofday(&start_fill, NULL);

    // Fill in the table
    for (int i=0;i<MAP_CAPACITY;i++){
        put(map, table_keys[i], value, value_length);
    }

    gettimeofday(&end_fill, NULL);

    // Query the table randomly
    // We will generate a new key list to recreate similar conditions to put,
    // where the key array has a good chance of being in the cache, and there
    // is no random access penalty
    for (int i=0;i<MAP_CAPACITY;i++){
        number_to_word(random()%MAP_CAPACITY, table_keys[i]);
    }
    for (int i=0;i<MAP_CAPACITY;i++){
        //int key_num = random()%MAP_CAPACITY;
        get(map, table_keys[i]);
    }

    gettimeofday(&end_query, NULL);

    printf("Done running. Fill %ld. Query %ld\n",
           (end_fill.tv_sec-start_fill.tv_sec)*1000000+(end_fill.tv_usec-start_fill.tv_usec),
           (end_query.tv_sec-end_fill.tv_sec)*1000000+(end_query.tv_usec-end_fill.tv_usec));
}
