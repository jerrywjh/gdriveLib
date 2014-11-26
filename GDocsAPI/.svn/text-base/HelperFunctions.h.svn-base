#pragma once
#include "stdafx.h"
#include <string>

using namespace std;


/***************************************************
This class holds static helper functions for this project.
****************************************************/
class HelperFunctions
{
public:
	static void		GetFileNameFromFilePath(LPCWSTR lpszFilePath, LPWSTR lpszFileName, size_t size); 
	static INT64	GetFileSize(LPCWSTR lpszFilePath) ; 
	static INT64	GetCurrentTimeStamp();
	static void		GetFileExtFromFileName(LPCWSTR lpszFileName, LPWSTR lpszFileExt, size_t uSize);
	static void		GetFileNameWithoutExt(LPCWSTR lpszFileName, LPWSTR lpszPureName, size_t size);
	static LPCSTR	GetMimeTypeFromFileName(LPCWSTR lpszFileName);

	static LPSTR	ConvertToUpper(LPSTR szStr);
	static LPSTR	ConvertToLower(LPSTR szStr);

	static LPWSTR	ConvertToUpper(LPWSTR szStr);
	static LPWSTR	ConvertToLower(LPWSTR szStr);
	
	static void	GetFolderIdFromFolderFeed(LPCSTR lpszFolderFeed, LPSTR lpszFolderId, size_t size);
	
	//convert the number to  string format like: "123,456,345".
	static void	NumberToBuffer   (unsigned   long   n,   WCHAR   *buffer,   int   digits_per_segment,  WCHAR   separator);

	static BOOL	DeleteDirectory(LPCWSTR lpszDir);
	static BOOL	IsFolderExist(LPCWSTR lpszDir);
	static string&   StringReplace(string&   str, const string&  old_value, const  string&  new_value);

	static BOOL CanBeConvertedToDocument(LPCWSTR lpszFileName);
	static BOOL CanBeConvertedToSpreadsheet(LPCWSTR lpszFileName);
	static BOOL CanBeConvertedToPresentation(LPCWSTR lpszFileName);
	static BOOL CanBeConvertedToDrawing(LPCWSTR lpszFileName);

	static VOID GetFormattedLocalTime(LPCSTR lpszOriginalTime, LPWSTR lpszFormattedLocalTime,  UINT uSize);
};
