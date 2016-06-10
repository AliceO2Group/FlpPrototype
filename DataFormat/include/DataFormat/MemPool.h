#ifndef DATAFORMAT_MEMPOOL
#define DATAFORMAT_MEMPOOL


#include <atomic>


// class to create a pool of memory pages from standard allocated memory.
// lock-free mechanism for fast concurrent page request/release

class MemPool {
  public:
  MemPool(int numberOfPages, int pageSize=1024*1024, int align=1024);
  ~MemPool();
  
  void *getPage();    // thread-safe ... can be called in parallel
  void releasePage(void *page);  // thread-safe ... can be called in parallel

  int getPageSize();
  
  private:
  
  void deletePages();  // release memory allocated for data members of this object

  int numberOfPages;
  int pageSize;

  void **pageTable; // table with list of allocated pages
  std::atomic_flag *pageIsUsed; // table to flag pages in use

  std::atomic<int> lastPageIndexGet; // cache for last page index reserved, will start search of free page from there
  std::atomic<int> lastPageIndexRelease; // cache for last page index reserved, will start search of free page from there
};


#endif
