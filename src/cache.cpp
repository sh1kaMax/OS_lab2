#include "cache.h"

bool CacheEntry::operator==(const CacheEntry& other) const {
    return fd == other.fd && offset == other.offset;
}

LRUCache::LRUCache(size_t cap) : capacity(cap) {}

void LRUCache::put(const CacheEntry& entry) {
    auto key = make_pair(entry.fd, entry.offset);

    if (cacheMap.find(key) != cacheMap.end()) {
        cacheList.erase(cacheMap[key]);
    } else if (cacheList.size() == capacity) {
        CacheEntry last = cacheList.back();
        auto lastKey = make_pair(last.fd, last.offset);
        cacheMap.erase(lastKey);
        cacheList.pop_back();
    }

    cacheList.push_front(entry);
    cacheMap[key] = cacheList.begin();
}

bool LRUCache::get(HANDLE fd, size_t offset, vector<char>& data) {
    auto key = make_pair(fd, offset);

    if (cacheMap.find(key) == cacheMap.end()) {
        return false;
    }

    auto it = cacheMap[key];
    data = it->data;

    if (cacheList.begin() != it) {
        CacheEntry newEntry = *it;
        cacheList.erase(it);
        cacheList.push_front(newEntry);
        cacheMap[key] = cacheList.begin();
    }
    return true;
}

bool LRUCache::check(HANDLE fd, size_t offset) {
    auto key = make_pair(fd, offset);
    
    return cacheMap.find(key) != cacheMap.end();
}

bool LRUCache::update(HANDLE fd, size_t offset, vector<char> data) {
    auto key = make_pair(fd, offset);

    if (cacheMap.find(key) == cacheMap.end()) {
        return false;
    }

    auto it = cacheMap[key];
    it->data.insert(it->data.end(), data.begin(), data.end());
    it->writtenOnDisk = false;

    return true;
}

ssize_t LRUCache::getSize(HANDLE fd, size_t offset) {
    auto key = make_pair(fd, offset);

    if (cacheMap.find(key) == cacheMap.end()) {
        return -1;
    }

    return cacheMap[key]->data.size();
}

bool LRUCache::setWrittenOnDiskTrue(HANDLE fd, size_t offset) {
    auto key = make_pair(fd, offset);

    if (cacheMap.find(key) == cacheMap.end()) {
        return false;
    }

    cacheMap[key]->writtenOnDisk = true;
    return true;
}