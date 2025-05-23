#ifndef CACHE_MANAGER_H
#define CACHE_MANAGER_H

#include <map>
#include <Arduino.h>

struct CacheEntry {
  int value;
  unsigned long timestamp;
};

class CacheManager {
public:
  CacheManager(unsigned long interval);
  int get(const String& key, bool forceRead = false);
  void setReader(int (*reader)(const String&));

private:
  std::map<String, CacheEntry> cache;
  unsigned long interval;
  int (*readerFunc)(const String&);
};

#endif