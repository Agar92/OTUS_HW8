An util to search duplicates among files. Command line options: <br/> 
| | |
|-|-|
  -h [ --help ] |                   Produces help message. <br/>
  -i [ --include_dirs ] |          Directories to be scanned, can be multiple. Default is current folder. <br/>
  -e [ --exclude_dirs ] |          Directories to be excluded, can be multiple. <br/>
  -d [ --depth ]        |          Scan depth, 1 - all directories, 0 - current folder only. Default value is 0. <br/>
  -m [ --min_size ]     |          Minimum size of the file to be processed, in  bytes. Default value is 1. <br/>
  -f [ --file_masks ]   |          Masks for files participating in comparison, case-insensitive. <br/>
  -b [ --block_size ]   |          The size of the block to read the files with, in bytes. Default value is 256. <br/>
  -a [ --algorithm ]    |          Hash algorithm to hash file blocks. Default value is crc32. <br/>
