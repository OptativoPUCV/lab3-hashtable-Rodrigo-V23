#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap
{
  Pair ** buckets;
  long size; //cantidad de datos/pairs en la tabla
  long capacity; //capacidad de la tabla
  long current; //indice del ultimo dato accedido
};
Pair * createPair( char * key,  void * value)
{
  Pair * new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}
long hash( char * key, long capacity)
{
  unsigned long hash = 0;
  char * ptr;
  for (ptr = key; *ptr != '\0'; ptr++)
  {
    hash += hash*32 + tolower(*ptr);
  }
  return hash%capacity;
}
int is_equal(void* key1, void* key2)
{
  if(key1 == NULL || key2 == NULL) return 0;
  if(strcmp((char*)key1,(char*)key2) == 0) return 1;
  return 0;
}
void insertMap(HashMap * mapa, char * key, void * value)
{
  long indice = hash(key, mapa -> capacity);
  while(mapa -> buckets[indice] != NULL && mapa -> buckets[indice] -> key != NULL)
  {
    if(is_equal(mapa -> buckets[indice] -> key, key))
    {
      return;
    }
    indice = (indice + 1) % mapa -> capacity;
  }
  mapa -> buckets[indice] = createPair(key, value);
  mapa -> size++;
  mapa -> current = indice;
}
void enlarge(HashMap * mapa)
{
  Pair ** old_buckets = mapa -> buckets;
  mapa -> capacity *= 2;
  mapa -> buckets = (Pair **)malloc(sizeof(Pair *) * mapa -> capacity);

  if (mapa -> buckets == NULL) {

      mapa -> capacity /= 2;
      return;
  }
  mapa -> size = 0;

  for (long i = 0; i < mapa -> capacity / 2; i++)
  {
    if (old_buckets[i] != NULL && old_buckets[i]->key != NULL)
    {
      insertMap(mapa, old_buckets[i]->key, old_buckets[i]->value);
    }
  }
  free(old_buckets);
}
HashMap * createMap(long capacity)
{
  HashMap * mapa = (HashMap *)malloc(sizeof(HashMap));

  if(mapa == NULL)
  {
    return NULL;
  }
  mapa -> buckets = (Pair **)malloc(sizeof(Pair *) * capacity);

  if(mapa -> buckets == NULL)
  {
    free(mapa);
    return NULL;
  }
  for(long i = 0; i < capacity; i++)
  {
    mapa -> buckets[i] = NULL;
  }
  mapa-> size = 0;
  mapa -> capacity = capacity;
  mapa -> current = -1;

  return mapa;
}
void eraseMap(HashMap * map,  char * key)
{    
  long index = hash(key, map->capacity);
  while (map->buckets[index] != NULL)
  {
    if (is_equal(map->buckets[index]->key, key))
    {
      map->buckets[index]->key = NULL;
      map->size--;
      return;
    }
    index = (index + 1) % map->capacity;
  }
}
Pair * searchMap(HashMap * mapa, char * key)
{
  if(mapa == NULL || key == NULL)
  {
    return NULL;
  }
  long indice = hash(key, mapa -> capacity);

  while (mapa -> buckets[indice] != NULL)
  {
    if (is_equal(mapa -> buckets[indice] -> key, key))
    {
      mapa -> current = indice;
      return mapa -> buckets[indice];
    }
    indice = (indice + 1) % mapa -> capacity;

    if(mapa -> buckets[indice] == NULL)
    {
      break;
    }
  }
  mapa -> current = indice;
  return NULL;
}
Pair * firstMap(HashMap * map)
{
  for (long i = 0; i < map->capacity; i++)
  {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
    {
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}
Pair * nextMap(HashMap * map)
{
  return NULL;
}