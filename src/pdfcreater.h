/**
Protject: batch2pdf
File: pdfcreater
Desc: pdf工具类 
Author: Xiong Chuan Liang (xcl_168@aliyun.com)
Date: 2015-9-9
*/
#ifndef PDFCREATER_H_
#define PDFCREATER_H_

#include <vector>
#include <array>

#include "hpdf.h"

namespace pdf {

	namespace config {

		const std::array<std::string, 15> FILTER_FILEEXT = { ".c",".cpp",".cc",".h",".hpp",".go",".java",".py", \
			",lua",".erl",".pl",".rs",".js",".php", ".rb" };
			
		enum LANGUAGE {
			EN = 0,
			CN = 1,
			TW = 2
		};

		static LANGUAGE LANG_TYPE = LANGUAGE::CN;
		void SetConfigLanguage(config::LANGUAGE langType);

		static int PAGE_LINENUM = 53;  //一页可显示的总行数
		static int LINE_HEIGHT = 15;  //行高	
		static int PAGE_FONT_SIZE = 14; // const HPDF_REAL
		static int FOOTBAR_FONT_SIZE = 10;

		void SetConfigPageLineNum(int pageLineNum);
		void SetConfigLineHeight(int lineHeight);
		void SetConfigPageFontSize(int pageFontSize);
		void SetConfigFootbarFontSize(int footbarFontSize);

		static const int ROWID_WIDTH = 5;
		static const int TEXT_INDENT = 5;   // 左边缩进

		static const char* FILE_OUTLINE = "files";
		static const char* AUTHOR = " Xiong Chuan Liang (xcl_168@aliyun.com)";
			
	}

	namespace merge {

		 void FileCreator( const std::string path,
								const std::string destFileName = "xcl.pdf",
								const std::string outline = "files");
	}

}  // namespace pdf

#endif  // PDFCREATER_H_