#include "cds.h"


/* The example usage for simple key value pair. */
int32_t simple_compare(Entry, Entry);
void simple_destroy(Entry);
void usage_simple();


/* The example usage for advanced key value pair. */
typedef struct Employ_ {
    int32_t iId;
    int8_t cYear;
    int8_t cLevel;
} Employ;
int32_t advanced_compare(Entry, Entry);
void advanced_destroy(Entry);
void usage_advanced();


int main()
{
    usage_simple();
    usage_advanced();

    return 0;
}


int32_t simple_compare(Entry entSrc, Entry entTge)
{
    Pair *pPairSrc = (Pair*)entSrc;
    Pair *pPairTge = (Pair*)entTge;
    if (pPairSrc->key == pPairTge->key)
        return 0;
    return (pPairSrc->key > pPairTge->key)? 1 : (-1);
}

void simple_destroy(Entry ent)
{
    free((Employ*)ent);
    return;
}

void usage_simple()
{
    OrderedMap *pMap;

    int32_t iRtnCode = OdrMapInit(&pMap);
    if (iRtnCode != SUCC)
        return;

    /*--------------------------------------------------*
     * Set the comparison and resource clean methods.   *
     *--------------------------------------------------*/
    pMap->set_compare(pMap, simple_compare);
    pMap->set_destroy(pMap, simple_destroy);

    /*--------------------------------------------------*
     * Insert key value pairs into the map.             *
     *--------------------------------------------------*/
    Pair *pPair;
    pPair = (Pair*)malloc(sizeof(Pair));
    #if __x86_64__
        pPair->key = (Key)(int64_t)1;
        pPair->value = (Key)(int64_t)-1;
    #else
        pPair->key = (Key)1;
        pPair->value = (Key)-1;
    #endif
    pMap->put(pMap, (Entry)pPair);

    pPair = (Pair*)malloc(sizeof(Pair));
    #if __x86_64__
        pPair->key = (Key)(int64_t)3;
        pPair->value = (Key)(int64_t)-3;
    #else
        pPair->key = (Key)3;
        pPair->value = (Key)-3;
    #endif
    pMap->put(pMap, (Entry)pPair);

    pPair = (Pair*)malloc(sizeof(Pair));
    #if __x86_64__
        pPair->key = (Key)(int64_t)2;
        pPair->value = (Key)(int64_t)-2;
    #else
        pPair->key = (Key)2;
        pPair->value = (Key)-2;
    #endif
    pMap->put(pMap, (Entry)pPair);

    /*--------------------------------------------------*
     * Retrive the value by with the specified key.     *
     *--------------------------------------------------*/
    Value value;
    #if __x86_64__
        pMap->get(pMap, (Key)(int64_t)1, &value);
        assert((int64_t)value == -1);
    #else
        pMap->get(pMap, (Key)1, &value);
        assert(value == -1);
    #endif

    /*--------------------------------------------------*
     * Remove the key value pair with the specified key.*
     *--------------------------------------------------*/
    #if __x86_64__
        pMap->remove(pMap, (Key)(int64_t)2);
    #else
        pMap->remove(pMap, (Key)2);
    #endif

    int32_t iSize = pMap->size(pMap);
    assert(iSize == 2);

    OdrMapDeinit(&pMap);
    return;
}


int32_t advanced_compare(Entry entSrc, Entry entTge)
{
    Pair *pPairSrc = (Pair*)entSrc;
    Pair *pPairTge = (Pair*)entTge;

    int32_t iOrder = strcmp(pPairSrc->key, pPairTge->key);
    if (iOrder != 0)
        return (iOrder > 0)? 1 : (-1);
    return 0;
}

void advanced_destroy(Entry ent)
{
    Pair *pPair = (Pair*)ent;
    free((Employ*)pPair->value);
    free((Pair*)ent);
    return;
}

void usage_advanced()
{
    char *aName[3] = {"Alice", "Bob", "Wesker"};
    OrderedMap *pMap;

    int32_t iRtnCode = OdrMapInit(&pMap);
    if (iRtnCode != SUCC)
        return;

    /*--------------------------------------------------*
     * Set the comparison and resource clean methods.   *
     *--------------------------------------------------*/
    pMap->set_compare(pMap, advanced_compare);
    pMap->set_destroy(pMap, advanced_destroy);

    /*--------------------------------------------------*
     * Insert key value pairs into the map.             *
     *--------------------------------------------------*/
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 100;
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[0];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, (Entry)pPair);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 90;
    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[1];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, (Entry)pPair);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 80;
    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[2];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, (Entry)pPair);

    /*--------------------------------------------------*
     * Retrive the value with the specified key.        *
     *--------------------------------------------------*/
    Value value;
    pMap->get(pMap, (Key)aName[0], &value);
    assert(((Employ*)value)->iId == 1);
    assert(((Employ*)value)->cYear == 25);
    assert(((Employ*)value)->cLevel == 100);

    /*--------------------------------------------------*
     * Remove the key value pair with the specified key.*
     *--------------------------------------------------*/
    pMap->remove(pMap, (Key)aName[1]);

    int32_t iSize = pMap->size(pMap);
    assert(iSize == 2);

    OdrMapDeinit(&pMap);
    return;
}