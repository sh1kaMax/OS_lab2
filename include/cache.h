#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <windows.h>

using namespace std;

struct CacheEntry {
    HANDLE fd;
    size_t offset;
    bool writtenOnDisk;
    vector<char> data;

    bool operator==(const CacheEntry& other) const;
};

struct pair_hash {
    template<typename T1, typename T2>
    size_t operator()(const pair<T1, T2>& p) const {
        return hash<T1>()(p.first) ^ hash<T2>()(p.second);
    }
};

class LRUCache {
private:
    size_t capacity;

    list<CacheEntry> cacheList;
    unordered_map<pair<HANDLE, size_t>, list<CacheEntry>::iterator, pair_hash> cacheMap;
public:
    explicit LRUCache(size_t cap);

    void put(const CacheEntry& entry);
    bool get(HANDLE fd, size_t offset, vector<char>& data);
    bool check(HANDLE fd, size_t offset);
    bool update(HANDLE fd, size_t offset, vector<char> data);
    ssize_t getSize(HANDLE fd, size_t offset);
    bool setWrittenOnDiskTrue(HANDLE fd, size_t offset);
};

#endif
