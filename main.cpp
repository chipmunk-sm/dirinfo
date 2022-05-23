/**************************************************************************************
* https://github.com/chipmunk-sm
* (C) 2021 chipmunk-sm
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
* TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************************/

#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <algorithm>
#include <codecvt>
#include <string>

#define COLOR_ERROR     12
#define COLOR_DEFAULT   7

void setErrorColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), color);
}

void setConsoleColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

std::wstring stringToWstring(const std::string t_str)
{
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.from_bytes(t_str);
}

std::wstring indentStr(std::wstring str, int n)
{
	auto ident = n - static_cast<decltype(n)>(str.length());
	return std::wstring(ident > 0 ? ident : 0, L' ') + str;
}

void PrintFileInfo(const std::filesystem::path& fileinfo)
{
	std::wstring sResult(fileinfo);
	sResult += L" [";
	try
	{
		const auto fsize = std::filesystem::file_size(fileinfo);
		sResult += indentStr(std::to_wstring(fsize), 1);
		setConsoleColor(COLOR_DEFAULT);
	}
	catch (const std::filesystem::filesystem_error& err)
	{
		sResult += L"filesystem error! " + stringToWstring(err.what()) + L" (" +
			(!err.path1().empty() ? (L" (P1:" + err.path1().wstring()) + L")" : L"") +
			(!err.path2().empty() ? (L" (P2:" + err.path2().wstring()) + L")" : L"");
		setErrorColor(COLOR_ERROR);
	}
	catch (const std::exception& ex)
	{
		sResult += L"general exception: " + stringToWstring(ex.what());
		setErrorColor(COLOR_ERROR);
	}
	sResult += L"]";
	std::wcout << sResult << std::endl;
	setConsoleColor(COLOR_DEFAULT);
}

void DirInfo(const std::filesystem::path& path)
{
	try
	{
		if (std::filesystem::exists(path))
		{
			for (const auto& fd : std::filesystem::directory_iterator(path))
			{
				if (std::filesystem::is_directory(fd.path()))
				{
					DirInfo(fd.path());
				}
				else
				{
					PrintFileInfo(fd.path());
				}
			}
		}
	}
	catch (const std::exception& ex)
	{
		setErrorColor(COLOR_ERROR);
		std::wcerr << L"Error: " << ex.what() << std::endl;
		setErrorColor(COLOR_DEFAULT);
	}
	catch (...)
	{
		setErrorColor(COLOR_ERROR);
		std::wcerr << L"Unexpected exception" << std::endl;
		setErrorColor(COLOR_DEFAULT);
	}
}


int wmain(int argc, wchar_t* argv[])
{
	/*
	for(WORD ind = 1; ind < 255; ind++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ind);
		std::cout << ind << " test console color  " << std::endl;
	}
	*/
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	std::filesystem::path path;

	if (argc > 2)
	{
		setErrorColor(COLOR_ERROR);
		std::wcerr << L"Usage: " << argv[0] << L" \"path to folder\"" << std::endl;
		setErrorColor(COLOR_DEFAULT);
		return 1;
	}
	else if (argc == 2)
	{
		path = argv[1];
	}
	else
	{
		path = std::filesystem::current_path();
	}

	if (std::filesystem::is_regular_file(path))
	{
		PrintFileInfo(path);
		return 0;
	}

	if (!std::filesystem::is_directory(path))
	{
		setErrorColor(COLOR_ERROR);
		std::wcerr << L"Error: The path is not directory, or access denied!\n[" << path << L"]" << std::endl;
		setErrorColor(COLOR_DEFAULT);
		return 1;
	}

	DirInfo(path);

	return 0;
}
