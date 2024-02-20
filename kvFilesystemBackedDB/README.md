# A toy key-value DB

1. Data

* Data is stored as file name=key, file data=value
    * keys are 255 bytes length <= because NAME_MAX is 255
    * value is unlimited <= limited by file size
2. API
* Data is manipulated using http-based REST API 
    * insert/update a single key/value pair <= write a file to the disk
    * query a single key, returning its value <= locate/read a file on the disk
    * list the keys <= returning the full directory listing
    * delete a key <= delete file
3. Caching
* of newly written data <= vfs cache
* of keys [inode/direntry cache], of values
* size of the cache is configurable via sysctl
    * vm.vfs_cache_pressure = 50 <= kernel will prefer inode and dentry caches over the page cache
    * vm.dirty_background_ratio = 10 <= 10% of the data/dirty pages will stay in the memory
    * there are more settings
        * vm.dirty_background_ratio = 10
        * vm.dirty_background_bytes = 0
        * vm.dirty_ratio = 20
        * vm.dirty_bytes = 0
        * vm.dirty_expire_centisecs = 3000
        * vm.dirty_writeback_centisecs = 500
    * added bonus: they live in harmony with the rest of the system wrt. memory usage