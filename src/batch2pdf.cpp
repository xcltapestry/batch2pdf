/**
Protject: batch2pdf
Desc: 批量将指定目录下(含子目录)的文件生成到一个pdf文件中.
      并会自动依目录和文件名生成pdf目录标签以便跳转.

	  代码中限定仅支持如下类型文件:
	 ".c",".cpp",".cc",".h",".hpp",".go",".java",".py", 
	 ",lua",".erl",".pl",".rs",".js",".php", ".rb"

	  使用开源库:https://github.com/libharu
	  编译和配置可参考: http://blog.csdn.net/xcl168/article/details/48229079

Author: Xiong Chuan Liang (xcl_168@aliyun.com)
Date: 2015-9-9
Example:
	batch2pdf -dE:\soft_c\wdt-master\ -nwdt.pdf  -owdt_20150909
	batch2pdf -dD:\soft_c\leveldb-master\ -nleveldb.pdf -cen -oleveldb

	batch2pdf -dD:\soft_c\rocksdb-master\ -nRocksDB源码_20150909.pdf -oRocksDB
*/


#include <iostream>
#include <string>
#include "pdfcreater.h"

#if defined(_WIN32) || defined(_WIN64)
	#pragma comment(lib,"..\\x64\\Release\\LIBHPDF.lib")
	#pragma warning (disable: 4996)  
#endif

static void Usage() {
	const char usage = 
	fprintf(
		stderr,
		"Usage: batch2pdf command...\n"
		"   -d 要批量转PDF的目录\n"
		"   -n pdf文件名称,并会生成在所转目录下.\n"
		"   -c cn:支持简体,en:仅支持英文,tw:支持繁体\n"
		"   -o pdf文件中的目标标签名\n"
		"   -n 一页总行数,默认为53\n"
		"   -l 每行高度,默认为15\n"
		"   -s 字体大小,默认为14\n"
		"   -b 页尾字体大小,默认为10\n"
		"   -h 命令说明\n"
		);
}

int main(int argc, char **argv)
{
	std::string path = "./";  // -d
	std::string destFileName = "xcl.pdf"; // -n
	std::string outline = "files"; //-o
	std::string temp,arg,argValue;

	for (int i = 1; i < argc; i++) {
		temp = argv[i];
		arg = temp.substr(0, 2);
		argValue = temp.substr(2);
		if (argValue.empty())continue;

		if (arg.compare("-d") == 0) {
			path = argValue;
		}
		else if (arg.compare("-n") == 0) {
			destFileName = argValue;
		}
		else if (arg.compare("-f") == 0) {
			if (argValue.compare("en") == 0) {
			   pdf::config::SetConfigLanguage(pdf::config::LANGUAGE::EN);
			}
			else if (argValue.compare("cn") == 0) {
				pdf::config::SetConfigLanguage(pdf::config::LANGUAGE::CN);
			}
			else if (argValue.compare("tw") == 0) {
				pdf::config::SetConfigLanguage(pdf::config::LANGUAGE::TW);
			}
		}
		else if (arg.compare("-o") == 0) {
			outline = argValue;
		}
		else if (arg.compare("-n") == 0) {
			pdf::config::SetConfigPageLineNum(std::stoi(argValue));
		}
		else if (arg.compare("-l") == 0) {
			pdf::config::SetConfigLineHeight(std::stoi(argValue));
		}
		else if (arg.compare("-s") == 0) {
			pdf::config::SetConfigPageFontSize(std::stoi(argValue));
		}
		else if (arg.compare("-b") == 0) {
			pdf::config::SetConfigFootbarFontSize(std::stoi(argValue));
		}
		else if (arg.compare("-h") == 0) {
			Usage();
		}
	}
	
	//批量将目录下的文件生成到一个pdf文件中
	pdf::merge::FileCreator(path, destFileName, outline);
		
	// 调用方法一
    // pdf::merge::FileCreator(R"(D:\pdf\demo\)", "zj.pdf"); 

	// 调用方法二
	//pdf::merge::FileCreator(R"(D:\pdf\demo\)", "zj.pdf", "demo"); 

	return 0;
}
