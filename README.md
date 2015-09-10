batch2pdf
========
 将源码工程文件批量转到一个pdf中，并自动生成文件标签以便跳转。
-------
```C++

Desc: 批量将指定工程目录下(含子目录)的源码文件生成到一个pdf文件中.
      并会自动依目录和文件名生成pdf目录标签以便跳转.

    代码中限定仅支持如下类型文件:
   ".c",".cpp",".cc",".h",".hpp",".go",".java",".py", 
   ",lua",".erl",".pl",".rs",".js",".php", ".rb"

    使用开源库:https://github.com/libharu
    编译和配置可参考: http://blog.csdn.net/xcl168/article/details/48229079


Example:
  batch2pdf -dE:\c\wdt-master\ -nwdt.pdf  -owdt_20150909
  batch2pdf -dD:\c\leveldb-master\ -nleveldb.pdf -cen -oleveldb

  batch2pdf -dD:\c\rocksdb-master\ -nRocksDB源码_20150909.pdf -oRocksDB

  ```

  ![](https://raw.githubusercontent.com/xcltapestry/batch2pdf/master/sample/batch2pdf.png)
