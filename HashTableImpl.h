#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

//=======================//
// Implemented Functions //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::PRIMES[3] = {102523, 100907, 104659};

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintLine(int tableIndex) const
{
    const HashData& data = table[tableIndex];

    // Using printf here it is easier to format
    if(data.sentinel == SENTINEL_MARK)
    {
        printf("[%03d]         : SENTINEL\n", tableIndex);
    }
    else if(data.sentinel == EMPTY_MARK)
    {
        printf("[%03d]         : EMPTY\n", tableIndex);
    }
    else
    {
        printf("[%03d] - [%03d] : ", tableIndex, data.lruCounter);
        printf("(%-5s) ", data.isCostWeighted ? "True" : "False");
        size_t sz = data.intArray.size();
        for(size_t i = 0; i < sz; i++)
        {
            if(i % 2 == 0)
                printf("[%03d]", data.intArray[i]);
            else
                printf("/%03d/", data.intArray[i]);

            if(i != sz - 1)
                printf("-->");
        }
        printf("\n");
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintTable() const
{
    printf("____________________\n");
    printf("Elements %d\n", elementCount);
    printf("[IDX] - [LRU] | DATA\n");
    printf("____________________\n");
    for(int i = 0; i < MAX_SIZE; i++)
    {
        PrintLine(i);
    }
}

//=======================//
//          TODO         //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Hash(int startInt, int endInt, bool isCostWeighted)
{
    int h;
    
    if (isCostWeighted){
        
        h = PRIMES[0] * startInt + PRIMES[1] * endInt + PRIMES[2];
        
    }
    
    else h = PRIMES[0] * startInt + PRIMES[1] * endInt;
    
    h %= MAX_SIZE;
    
    if (h < 0) h += MAX_SIZE;
    /* TODO */
    return h;
}

template<int MAX_SIZE>
HashTable<MAX_SIZE>::HashTable()
{
    
    for (int i = 0; i < MAX_SIZE; i++){
        
        table[i].lruCounter = 0;
        table[i].sentinel = EMPTY_MARK;
        
    }
    
    elementCount = 0;
    /* TODO */
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Insert(const std::vector<int>& intArray, bool isCostWeighted)
{
    
    if (intArray.size() == 0) throw InvalidTableArgException();
    
    int hashVal = Hash(intArray.front(), intArray.back(), isCostWeighted);
    
    int placeVal = hashVal;
    int i;
    
    for (i = 1; table[placeVal].sentinel != EMPTY_MARK; i++){
        
        if (table[placeVal].isCostWeighted == isCostWeighted &&
            table[placeVal].startInt == intArray.front() &&
            table[placeVal].endInt == intArray.back() && 
            table[placeVal].intArray == intArray) {
            
            table[placeVal].lruCounter++;
            return table[placeVal].lruCounter;
            
        }
        
        else {
            
            placeVal = i * i + hashVal;
            placeVal %= MAX_SIZE;
            
        }
    }
    
    placeVal = hashVal;
    if (elementCount > MAX_SIZE / 2) throw TableCapFullException(elementCount);
    
    else {
        
        for (i = 1; table[placeVal].sentinel == OCCUPIED_MARK; i++){
            
            placeVal = i * i + hashVal;
            placeVal %= MAX_SIZE;
            
        }
        
        table[placeVal].sentinel = OCCUPIED_MARK;
        table[placeVal].isCostWeighted = isCostWeighted;
        table[placeVal].startInt = intArray.front();
        table[placeVal].endInt = intArray.back();
        table[placeVal].intArray = intArray;
        table[placeVal].lruCounter = 1;
        
        elementCount++;
        
    }
    /* TODO */
    return 0;
}

template<int MAX_SIZE>
bool HashTable<MAX_SIZE>::Find(std::vector<int>& intArray,
                               int startInt, int endInt, bool isCostWeighted,
                               bool incLRU)
{
    int hashVal = Hash(startInt, endInt, isCostWeighted);
    
    int placeVal = hashVal;
    int i;
    
    for (i = 1; table[placeVal].sentinel != EMPTY_MARK; i++){
        
        if (table[placeVal].isCostWeighted == isCostWeighted &&
            table[placeVal].startInt == startInt &&
            table[placeVal].endInt == endInt) {
            
            intArray = table[placeVal].intArray;
            if (incLRU) table[placeVal].lruCounter++;
            return true;
            
        }
        
        else {
            
            placeVal = i * i + hashVal;
            placeVal %= MAX_SIZE;
            
        }
    }
    /* TODO */
    return false;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::InvalidateTable()
{
    for (int i = 0; i < MAX_SIZE; i++){
        
        table[i].sentinel = EMPTY_MARK;
        table[i].isCostWeighted = false;
        table[i].startInt = 0;
        table[i].endInt = 0;
        table[i].intArray.clear();
        table[i].lruCounter = 0;
        
    }
    
    elementCount = 0;
    /* TODO */
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::GetMostInserted(std::vector<int>& intArray) const
{
    int count = 0;
    
    for (int i = 0; i < MAX_SIZE; i++){
        
        if (table[i].lruCounter > count){
            
            intArray = table[i].intArray;
            count = table[i].lruCounter;
            
        }
        
    }
    /* TODO */
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::Remove(std::vector<int>& intArray,
                                 int startInt, int endInt, bool isCostWeighted)
{
    int hashVal = Hash(startInt, endInt, isCostWeighted);
    
    int placeVal = hashVal;
    int i;
    
    for (i = 1; table[placeVal].sentinel != EMPTY_MARK; i++){
        
        if (table[placeVal].isCostWeighted == isCostWeighted &&
            table[placeVal].startInt == startInt &&
            table[placeVal].endInt == endInt) {
            
            intArray = table[placeVal].intArray;
            
            table[placeVal].sentinel = SENTINEL_MARK;
            table[placeVal].isCostWeighted = false;
            table[placeVal].startInt = 0;
            table[placeVal].endInt = 0;
            table[placeVal].intArray.clear();
            table[placeVal].lruCounter = 0;
            
            elementCount--;
        }
        
        else {
            
            placeVal = i * i + hashVal;
            placeVal %= MAX_SIZE;
            
        }
    }
    /* TODO */
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::RemoveLRU(int lruElementCount)
{
    int count = 999999, ind;
    
    for (int i = 0; i < MAX_SIZE; i++){
        
        if (table[i].sentinel == OCCUPIED_MARK && 
            table[i].lruCounter < count){
            
            ind = i;
            count = table[i].lruCounter;
            
        }
        
    }
    
    table[ind].sentinel = SENTINEL_MARK;
    table[ind].isCostWeighted = false;
    table[ind].startInt = 0;
    table[ind].endInt = 0;
    table[ind].intArray.clear();
    table[ind].lruCounter = 0;
    
    elementCount--;
    
    if (lruElementCount > 1) RemoveLRU(lruElementCount - 1);
    /* TODO */
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintSortedLRUEntries() const
{
    MaxPairHeap<int, int> pq;
    int ind, t, s = 0;
    for (int i = 0; i < MAX_SIZE; i++){
        
        if (table[i].sentinel == OCCUPIED_MARK){
            
            pq.push({table[i].lruCounter, i});
            s++;
            
        }
        
    }
    
    for (int i = s; i > 0; i--){
        
        ind = pq.top().value;
        pq.pop();
        PrintLine(ind);
        
    }
    /* TODO */
}

#endif // HASH_TABLE_HPP
