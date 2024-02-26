#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#define FOR_INSERT true
#define FOR_SEARCH false
//=======================//
// Implemented Functions //
//=======================//
template <int MAX_SIZE>
int HashTable<MAX_SIZE>::PRIMES[3] = {102523, 100907, 104659};

template <int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintLine(int tableIndex) const
{
    const HashData &data = table[tableIndex];

    // Using printf here it is easier to format
    if (data.sentinel == SENTINEL_MARK)
    {
        printf("[%03d]         : SENTINEL\n", tableIndex);
    }
    else if (data.sentinel == EMPTY_MARK)
    {
        printf("[%03d]         : EMPTY\n", tableIndex);
    }
    else
    {
        printf("[%03d] - [%03d] : ", tableIndex, data.lruCounter);
        printf("(%-5s) ", data.isCostWeighted ? "True" : "False");
        size_t sz = data.intArray.size();
        for (size_t i = 0; i < sz; i++)
        {
            if (i % 2 == 0)
                printf("[%03d]", data.intArray[i]);
            else
                printf("/%03d/", data.intArray[i]);

            if (i != sz - 1)
                printf("-->");
        }
        printf("\n");
    }
}

template <int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintTable() const
{
    printf("____________________\n");
    printf("Elements %d\n", elementCount);
    printf("[IDX] - [LRU] | DATA\n");
    printf("____________________\n");
    for (int i = 0; i < MAX_SIZE; i++)
    {
        PrintLine(i);
    }
}

//=======================//
//          TODO         //
//=======================//
template <int MAX_SIZE>
int HashTable<MAX_SIZE>::Hash(int startInt, int endInt, bool isCostWeighted)
{
    /* TODO */
    /* COMPLETE */
    int cost = isCostWeighted ? 1 : 0;
    int hash = (startInt * PRIMES[0] + endInt * PRIMES[1] + cost * PRIMES[2]) % MAX_SIZE;
    return hash;
}

template <int MAX_SIZE>
HashTable<MAX_SIZE>::HashTable()
{
    /* TODO */
    /* COMPLETE */
    for (int i = 0; i < MAX_SIZE; i++)
    {
        table[i].lruCounter = 0;
        table[i].sentinel = EMPTY_MARK;
    }
    elementCount = 0;
}

template <int MAX_SIZE>
FindResult HashTable<MAX_SIZE>::FindIndex(int startInt, int endInt, bool isCostWeighted, bool isForInserting)
{
    /* My Own Function */
    /* COMPLETE */
    int hash = Hash(startInt, endInt, isCostWeighted);
    int prob = 1;

    // For Searching look until you reach an EMPTY_MARK
    if (!isForInserting)
    {
        while (table[hash].sentinel != EMPTY_MARK)
        {
            HashData &data = table[hash];
            if (data.sentinel == OCCUPIED_MARK &&
                data.isCostWeighted == isCostWeighted &&
                data.intArray.front() == startInt &&
                data.intArray.back() == endInt)
            {
                return FindResult(hash, true);
            }

            // Quadratic probing
            hash = (hash + (2 * prob) - 1) % MAX_SIZE;
            prob++;
        }
        return FindResult(hash, false);
    }
    // For inserting look until you reach EMPTY_MARK or SENTINEL_MARK
    else
    {
        while (table[hash].sentinel == OCCUPIED_MARK)
        {
            HashData &data = table[hash];
            if (data.sentinel == OCCUPIED_MARK &&
                data.isCostWeighted == isCostWeighted &&
                data.intArray.front() == startInt &&
                data.intArray.back() == endInt)
            {
                return FindResult(hash, true);
            }

            // Quadratic probing
            hash = (hash + (2 * prob) - 1) % MAX_SIZE;
            prob++;
        }
        return FindResult(hash, false);
    }
}
template <int MAX_SIZE>
int HashTable<MAX_SIZE>::Insert(const std::vector<int> &intArray, bool isCostWeighted)
{
    /* TODO */
    /* COMPLETE */
    if (intArray.empty())
    {
        throw InvalidTableArgException();
    }

    int startInt = intArray.front();
    int endInt = intArray.back();

    // First search for the given key, if it exists in hash table
    FindResult result = FindIndex(startInt, endInt, isCostWeighted, FOR_SEARCH);
    int hash = result.index;
    bool isOccupied = result.isOccupied;

    // If that key exists in table
    if (isOccupied)
    {
        // Update the LRU counter
        table[hash].lruCounter++;
        // Return the pre-modified lruCounter
        return table[hash].lruCounter - 1;
    }
    // If that key does not exist in table insert
    else
    {
        // Call findIndex function again to get the correct place to insert
        result = FindIndex(startInt, endInt, isCostWeighted, FOR_INSERT);
        int hash = result.index;

        if (elementCount > (MAX_SIZE / CAPACITY_THRESHOLD))
            throw TableCapFullException(elementCount);
        else
        {
            HashData &data = table[hash];
            // Insert new entry data
            data.intArray = intArray;
            data.sentinel = OCCUPIED_MARK;
            // Insert new key
            data.isCostWeighted = isCostWeighted;
            data.startInt = intArray.front();
            data.endInt = intArray.back();
            // Set lruCounter
            data.lruCounter = 1;
            elementCount++;
            // Return pre-modified lruCounter that is 0
            return 0;
        }
    }
}
template <int MAX_SIZE>
bool HashTable<MAX_SIZE>::Find(std::vector<int> &intArray,
                               int startInt, int endInt, bool isCostWeighted,
                               bool incLRU)
{
    FindResult result = FindIndex(startInt, endInt, isCostWeighted, FOR_SEARCH);

    bool isOccupied = result.isOccupied;
    int hash = result.index;

    if (isOccupied)
    {
        intArray = table[hash].intArray;
        if (incLRU)
        {
            table[hash].lruCounter++;
        }
    }
    return isOccupied;
}

template <int MAX_SIZE>
void HashTable<MAX_SIZE>::InvalidateTable()
{
    /* TODO */
    /* COMPLETE */
    for (int i = 0; i < MAX_SIZE; i++)
    {
        table[i].lruCounter = 0;
        table[i].sentinel = EMPTY_MARK;
        table[i].intArray.clear();
    }
    elementCount = 0;
}

template <int MAX_SIZE>
void HashTable<MAX_SIZE>::GetMostInserted(std::vector<int> &intArray) const
{
    /* TODO */
    int MaxIndex = -1;
    int MaxLRU = 0;

    // Linear Search through table list to find the element with highest LRU
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (table[i].sentinel == OCCUPIED_MARK && table[i].lruCounter > MaxLRU)
        {
            MaxLRU = table[i].lruCounter;
            MaxIndex = i;
        }
    }

    // If a highest element exists copy data of the element to intArray
    if (MaxIndex != 1)
        intArray = table[MaxIndex].intArray;
}

template <int MAX_SIZE>
void HashTable<MAX_SIZE>::Remove(std::vector<int> &intArray,
                                 int startInt, int endInt, bool isCostWeighted)
{
    /* TODO */
    /* COMPLETE */
    FindResult result = FindIndex(startInt, endInt, isCostWeighted, FOR_SEARCH);
    int hash = result.index;
    bool isOccupied = result.isOccupied;

    if (isOccupied)
    {
        // Mark the cell SENTINEL_MARK modifiy return contents in intArray
        table[hash].sentinel = SENTINEL_MARK;
        intArray = table[hash].intArray;
        elementCount--;
        // Remove artifacts
        // table[hash].intArray.clear();
        table[hash].lruCounter = 0;
    }
}

template <int MAX_SIZE>
void HashTable<MAX_SIZE>::RemoveLRU(int lruElementCount)
{
    std::priority_queue<Pair<int, int>, std::vector<Pair<int, int>>, DoubleGreaterComparator<Pair<int, int>>> EntryMinHeap;

    // Collect entries with non-zero lruCounter
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (table[i].lruCounter > 0)
        {
            EntryMinHeap.push(Pair<int, int>(table[i].lruCounter, i));
        }
    }

    // Remove top lruElementCount items from the table
    while (lruElementCount > 0 && !EntryMinHeap.empty())
    {
        int index = EntryMinHeap.top().second;
        EntryMinHeap.pop();

        // Remove entry from the table
        table[index].sentinel = SENTINEL_MARK;
        // table[index].intArray.clear();
        table[index].lruCounter = 0;

        elementCount--;
        lruElementCount--;
    }
}

template <int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintSortedLRUEntries() const
{
    /* TODO */
    struct ComparePairs
    {
        bool operator()(const Pair<int, int> &p1, const Pair<int, int> &p2) const
        {
            if (p1.key != p2.key)
            {
                return p1.key < p2.key;
            }
            return p1.value > p2.value;
        }
    };
    std::priority_queue<Pair<int, int>, std::vector<Pair<int, int>>, ComparePairs> EntryMaxHeap;
    // std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, ComparePairs> EntryMaxHeap;

    // Collect entries with non-zero lruCounter
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (table[i].lruCounter > 0)
        {
            EntryMaxHeap.push(Pair<int, int>(table[i].lruCounter, i));
        }
    }

    // Print entries in high-to-low fashion
    while (!EntryMaxHeap.empty())
    {
        // int lruCounter = EntryMaxHeap.top().first;
        int index = EntryMaxHeap.top().second;
        EntryMaxHeap.pop();
        PrintLine(index);
    }
}

#endif // HASH_TABLE_HPP