#include "CacheManager.h"

CacheManager::CacheManager(unsigned long interval) {
  this->interval = interval;
  readerFunc = nullptr;
}

void CacheManager::setReader(int (*reader)(const String&)) {
  readerFunc = reader;
}

int CacheManager::get(const String& key, bool forceRead) {
  unsigned long now = millis();

  if (!cache.count(key) || forceRead || (now - cache[key].timestamp > interval)) {
    if (readerFunc) {
      int value = readerFunc(key);
      cache[key] = {value, now};
    }
  }

  return cache[key].value;
}