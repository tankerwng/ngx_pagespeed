/*
 * Copyright 2013 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Author: jmarantz@google.com (Joshua Marantz)

#ifndef NET_INSTAWEB_UTIL_PUBLIC_COMPRESSED_CACHE_H_
#define NET_INSTAWEB_UTIL_PUBLIC_COMPRESSED_CACHE_H_

#include "net/instaweb/util/public/basictypes.h"
#include "net/instaweb/util/public/cache_interface.h"
#include "net/instaweb/util/public/string.h"

namespace net_instaweb {

class Histogram;
class SharedString;
class Statistics;
class Variable;

// Compressed cache adapter.
class CompressedCache : public CacheInterface {
 public:
  // Does not takes ownership of cache or stats.
  CompressedCache(CacheInterface* cache, Statistics* stats);
  virtual ~CompressedCache();

  static void InitStats(Statistics* stats);

  virtual void Get(const GoogleString& key, Callback* callback);
  virtual void Put(const GoogleString& key, SharedString* value);
  virtual void Delete(const GoogleString& key);
  virtual const char* Name() const { return name_.c_str(); }
  virtual CacheInterface* Backend() { return cache_; }
  virtual bool IsBlocking() const { return cache_->IsBlocking(); }
  virtual bool IsHealthy() const;
  virtual void ShutDown();

  // Total number of times we did a fetch from the underlying cache and
  // it failed to uncompress due to corruption.
  int64 CorruptPayloads() const;

  // Total number of uncompressed bytes inserted since the cache was
  // started.
  int64 OriginalSize() const;

  // Total number of compressed bytes inserted since the cache was
  // started.
  int64 CompressedSize() const;

 private:
  CacheInterface* cache_;
  GoogleString name_;
  Histogram* compressed_cache_savings_;
  Variable* corrupt_payloads_;
  Variable* original_size_;
  Variable* compressed_size_;

  DISALLOW_COPY_AND_ASSIGN(CompressedCache);
};

}  // namespace net_instaweb

#endif  // NET_INSTAWEB_UTIL_PUBLIC_COMPRESSED_CACHE_H_
