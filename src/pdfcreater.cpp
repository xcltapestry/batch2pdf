#include "pdfcreater.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <experimental/filesystem>
#include <fstream>

#include <setjmp.h>
#include "hpdf.h"

#include <windows.h>

namespace fs = std::experimental::filesystem;

namespace pdf {

	namespace config {

		void SetConfigLanguage(const config::LANGUAGE langType) {
			LANG_TYPE = langType;
		}

		void SetConfigConvert(config::CONVERT convertType) {
			CONVERT_TYPE = convertType;
		}

		void SetConfigPageLineNum(const int pageLineNum) {
			PAGE_LINENUM = pageLineNum;
		}
		void SetConfigLineHeight(const int lineHeight) {
			LINE_HEIGHT = lineHeight;
		}
		void SetConfigPageFontSize(const int pageFontSize) {
			PAGE_FONT_SIZE = pageFontSize;
		}
		void SetConfigFootbarFontSize(const int footbarFontSize) {
			FOOTBAR_FONT_SIZE = footbarFontSize;
		}

	}

	namespace util {

		std::vector<std::string> split(const std::string &s, char delim) {
			std::stringstream buffer(s);
			std::vector<std::string> vec;
			std::string ln;
		
			while (std::getline(buffer, ln, delim)) {
				vec.push_back(ln);
			}
			return vec;
		}

		std::vector<std::string>listDir(const std::string path) {
			std::tr2::sys::path mypath = path;
			std::vector<std::string> files;
			std::string fext;
			for (fs::path p : fs::recursive_directory_iterator(mypath)) {
				if (p.extension().empty())continue;
				fext = p.extension().string();
				transform(fext.begin(), fext.end(), fext.begin(), tolower);

				for (std::string ext : config::FILTER_FILEEXT) {
					if (!ext.compare(fext)) files.push_back(p.string());
				}
			}
			return files;
		}

		void ConvertUtf8ToGBK(std::string& strGBK, std::string strUtf8)
		{
			int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.c_str(), -1, NULL, 0);
			unsigned short * wszGBK = new unsigned short[len + 1];
			memset(wszGBK, 0, len * 2 + 2);
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.c_str(), -1, (LPWSTR)wszGBK, len);
			len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
			char *szGBK = new char[len + 1];
			memset(szGBK, 0, len + 1);
			WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
			strGBK = szGBK;
			delete[] szGBK;
			delete[] wszGBK;
		}

		std::string getFileContents(const std::string fileName) noexcept{
			std::ifstream infile;
			infile.open(fileName);
			if (!infile.is_open())return "";

			std::stringstream buffer;
			buffer << infile.rdbuf();
			infile.close();
			infile.clear();

			if (config::CONVERT_TYPE == config::CONVERT::UTF8TOGBK) {
				std::string g;
				std::string x = buffer.str();
				ConvertUtf8ToGBK(g, x);
				return g;
			}
			else {
				return (buffer.str());
			}
		}
		
		int calcPageNum(const int total) {
			int pageLns = config::PAGE_LINENUM;
			if (pageLns == 0 || total == 0 || total < pageLns) return 1;
			return (total % pageLns != 0) ? total / pageLns + 1 : total / pageLns;
		} //end calcPageNum
		
		std::string rowidWidth(const int id) {
			std::ostringstream buffer;
			buffer << std::setfill(' ') << std::setw(config::ROWID_WIDTH) << id << " " << std::endl;
			return buffer.str();
		}

	} //end namespace util

	namespace merge {

		/////////////////////////////////////////////////////////////////
		jmp_buf env;

#ifdef HPDF_DLL
		void  __stdcall
#else
		void
#endif
			error_handler(HPDF_STATUS   error_no,
				HPDF_STATUS   detail_no,
				void         *user_data)
		{
			printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
				(HPDF_UINT)detail_no);
			longjmp(env, 1);
		}
		/////////////////////////////////////////////////////////////////

		int fileWriter(HPDF_Doc  pdf,
					 HPDF_Outline root, 
					 HPDF_Font font, 
					 const std::string curFile, 
					 const std::vector< std::string> vec) {
					
			auto func_footbar = [](std::string fileName)->std::string {
				std::ostringstream buffer;				
				buffer << "  File:" << fileName.c_str() <<"   Autor:XCL ";
				return buffer.str();
			};

			auto func_pageOutline = [&](HPDF_Page page,std::string curFile){
				HPDF_Outline outline;
				HPDF_Destination dst;
				outline = HPDF_CreateOutline(pdf, root, curFile.c_str(), NULL);
				dst = HPDF_Page_CreateDestination(page);
				HPDF_Destination_SetXYZ(dst, 0, HPDF_Page_GetHeight(page), 1);// 1 = 默认百分比100
				HPDF_Outline_SetDestination(outline, dst);
			};
		
				HPDF_Page page;							
				int curID = 0;	
				int pageNum = util::calcPageNum(vec.size());
				for (int i = 0; i < pageNum; i++) {
					page = HPDF_AddPage(pdf);					
					if (i == 0)func_pageOutline(page,curFile);								
					HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

					HPDF_REAL height = HPDF_Page_GetHeight(page);
					HPDF_REAL width = HPDF_Page_GetWidth(page);
					HPDF_UINT x, y;
				
				    {
						HPDF_RGBColor c = HPDF_Page_GetRGBFill(page);
						HPDF_Page_SetRGBStroke(page, 0.0, 0.5, 0);
						HPDF_Page_SetFontAndSize(page, font, config::FOOTBAR_FONT_SIZE);
						HPDF_Page_SetLineWidth(page, 0.5);
						HPDF_Page_MoveTo(page, 0, 13);
						HPDF_Page_LineTo(page, width, 16);
						HPDF_Page_Stroke(page);
						
						HPDF_Page_SetRGBFill(page, 0.0, 0,0.5);
						HPDF_Page_BeginText(page);						
						HPDF_Page_MoveTextPos(page, 40, config::FOOTBAR_FONT_SIZE - 5);
						HPDF_Page_ShowText(page, func_footbar(curFile).c_str());
						HPDF_Page_EndText(page);					
						HPDF_Page_SetRGBFill(page, c.r, c.g, c.b);
					};

					HPDF_Page_SetFontAndSize(page, font, config::PAGE_FONT_SIZE); 

					x = 0;
					y = height - config::LINE_HEIGHT;
					int ln = 0;
					for (ln = curID; ln < curID + config::PAGE_LINENUM; ln++) {

						if (ln == vec.size())break;
						if (ln == curID) y = height - config::LINE_HEIGHT * 2;
					
						HPDF_Page_BeginText(page);
						HPDF_Page_MoveTextPos(page, config::ROWID_WIDTH, y - 2);
						HPDF_Page_ShowText(page, util::rowidWidth(ln + 1).c_str());
						HPDF_Page_ShowText(page, vec[ln].c_str());
						HPDF_Page_EndText(page);
						y -= config::LINE_HEIGHT;
					} //end for
					curID = ln;
				}

			return 0;
		} //end func

		void FileCreator(const std::string path,
						const std::string destFileName,
						const std::string outline ) {
			std::vector<std::string> files = util::listDir(path);
			if (files.size() <= 0)return;
			std::sort(std::begin(files), std::end(files));
			///////////////////////////
			HPDF_Doc  pdf;
			pdf = HPDF_New(error_handler, NULL);
			if (!pdf) {
				std::cout<<"error: cannot create PdfDoc object"<<std::endl;
				return;
			}

			if (setjmp(env)) {
				HPDF_Free(pdf);
				return;
			}

			/*
			const char *font_name1;
			const char *font_name2;
			const char *font_name3;
			font_name1 = HPDF_LoadTTFontFromFile(pdf, "C:\\Windows\\Fonts\\Arial.ttf", HPDF_TRUE);
			font_name2 = HPDF_LoadTTFontFromFile(pdf, R"(C:\Windows\Fonts\simsunb.ttf)", HPDF_TRUE);
			font_name3 = HPDF_LoadTTFontFromFile2(pdf, "C:\\Windows\\Fonts\\simsun.ttc", 1, HPDF_TRUE);
			*/

			HPDF_Outline root;
			root = HPDF_CreateOutline(pdf, NULL, outline.c_str(), NULL);
			HPDF_Outline_SetOpened(root, HPDF_TRUE);
			HPDF_SetInfoAttr(pdf, HPDF_INFO_AUTHOR, config::AUTHOR);
			HPDF_SetInfoAttr(pdf, HPDF_INFO_TITLE, path.c_str());

			HPDF_Font font;
			switch (config::LANG_TYPE)
			{
			case pdf::config::CN:
				HPDF_UseCNSFonts(pdf);
				HPDF_UseCNSEncodings(pdf);
				font = HPDF_GetFont(pdf, "SimHei", "GB-EUC-H");  // SimSun  SimHei
				break;
			case pdf::config::TW:
				HPDF_UseCNTFonts(pdf);
				HPDF_UseCNTEncodings(pdf);
				font = HPDF_GetFont(pdf, "MingLiU", "ETen-B5-H");
				break;
			default:
				font = HPDF_GetFont(pdf, "Helvetica", NULL);
				break;
			}

			///////////////////////////			
			std::string contents;
			std::vector<std::string> vec;
			for (std::string file : files) {
				contents = util::getFileContents(file);
				vec = util::split(contents, '\n');
				fileWriter(pdf, root, font, file.substr(path.length()), vec);
			}
			///////////////////////////			
			std::string destFile(path);
			destFile.append("/");
			destFile.append(destFileName);
			HPDF_SaveToFile(pdf, destFile.c_str());
			HPDF_Free(pdf);
		}//end FileCreator()

	} //end namespace merge......

} // end namespace pdf