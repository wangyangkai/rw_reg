# read or write register via /dev/mem on arm linux


```
[root@machine /home]# ./rw_reg read 0xc0010c00 0x1c
PAGE_SIZE_MASK:0x00000fff
reg base addr:0xc0010c00, range:0x0000001c
mmap() base addr:0xc0010000, range:0x00000c20
reg:0xc0010c00, val:0x00000000
reg:0xc0010c04, val:0x000006d5
reg:0xc0010c08, val:0x045b63d3
reg:0xc0010c0c, val:0x00000000
reg:0xc0010c10, val:0x00000000
reg:0xc0010c14, val:0x00000001
reg:0xc0010c18, val:0x00000000
reg:0xc0010c1c, val:0xffffffff
[root@machine /home]# 

```


```
[root@machine /home]# ./rw_reg write 0xc0010000 0xc00 0x0
PAGE_SIZE_MASK:0x00000fff
reg base addr:0xc0010000, offset:0x00000c00, value:0x00000000
mmap() base addr:0xc0010000, range:0x00000c00
set addr:0xc0010c00 val:0x00000000
get addr:0xc0010c00 val:0x00000000
[root@machine /home]# 
[root@machine /home]# hwclock 
Thu Jan  1 00:00:05 1970  0.000000 seconds
[root@machine /home]# 
[root@machine /home]# 
[root@machine /home]# ./rw_reg write 0xc0010c00 0x00 0x0
PAGE_SIZE_MASK:0x00000fff
reg base addr:0xc0010c00, offset:0x00000000, value:0x00000000
mmap() base addr:0xc0010000, range:0x00000c00
set addr:0xc0010c00 val:0x00000000
get addr:0xc0010c00 val:0x00000000
[root@machine /home]# 
[root@machine /home]# hwclock 
Thu Jan  1 00:00:07 1970  0.000000 seconds
[root@machine /home]#
```

