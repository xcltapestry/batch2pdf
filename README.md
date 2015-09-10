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


编译器: Visual Studio Express 2015 for Windows Desktop

  ```
batch2pdf -help
  Usage: batch2pdf command...
   -d 要批量转PDF的目录
   -n pdf文件名称,并会生成在所转目录下.
   -c cn:支持简体,en:仅支持英文,tw:支持繁体
   -o pdf文件中的目标标签名
   -n 一页总行数,默认为53
   -l 每行高度,默认为15
   -s 字体大小,默认为14
   -b 页尾字体大小,默认为10
   -h 命令说明

pdf文件效果:
  ![](https://raw.githubusercontent.com/xcltapestry/batch2pdf/master/sample/batch2pdf.png)
