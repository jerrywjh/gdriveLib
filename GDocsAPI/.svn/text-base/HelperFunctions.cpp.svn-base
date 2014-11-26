#include "StdAfx.h"
#include <time.h>
#include <sys/timeb.h>
#include <assert.h>
#include <math.h>
#include "HelperFunctions.h"


//files with these extensions can be converted to Google Document format when uploaded
const char ExtensionsForDocument[][8] =  
{
	"TXT",
	"HTML",
	"HTM",
	"DOC",
	"DOCX",
	"RTF",
	"ODT",
	"SXW"
};

//files with these extensions can be converted to Google Spreadsheet format when uploaded
const char ExtensionsForSpreadsheet[][8] =  
{
	"CSV",
	"TSV",
	"TAB",
	"XLS",
	"XLSX",
	"ODS"
};

//files with these extensions can be converted to Google Spreadsheet format when uploaded
const char ExtensionsForPresentation[][8] =  
{
	"PPT",
	"PPS",
	"PPTX"
};

//files with these extensions can be converted to Google Drawing format when uploaded
const char ExtensionsForDrawing[][8] =
{
	"WMF"
};



//上传文件时不同扩展名对应的mimetype, 
//这个列表要根据google docs上传的实验反复来校对才行 
//前面一直到"WMF"的值是根据 http://code.google.com/apis/documents/faq.html#WhatKindOfFilesCanIUpload 设置的
//剩余部分基本是根据http://en.wikipedia.org/wiki/Mimetype来设置的
const char MimeTypes[][2][128] =
{	
	{"***",	"application/octet-stream"},
	{"CSV",	"text/csv"},
	{"TSV",	"text/tab-separated-values"},
	{"TAB",	"text/tab-separated-values"},
	{"HTML","text/html"},
	{"HTM",	"text/html"},
	{"DOC",	"application/msword"},
	{"DOCX","application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
	{"ODS",	"application/x-vnd.oasis.opendocument.spreadsheet"},
	{"ODT",	"application/vnd.oasis.opendocument.text"},
	{"RTF",	"application/rtf"},
	{"SXW",	"application/vnd.sun.xml.writer"},                               
	{"TXT",	"text/plain"},
	{"XLS",	"application/vnd.ms-excel"},
	{"XLSX",	"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
	{"PDF",	"application/pdf"},
	{"PPT",	"application/vnd.ms-powerpoint"},
	{"PPS",	"application/vnd.ms-powerpoint"},
	{"PPTX",	"application/vnd.openxmlformats-officedocument.presentationml.presentation"},
	{"WMF"	"image/x-wmf"},												
	{"ATOM","application/atom+xml"},
	{"XML",	"application/xml"},          
	{"JSON",	"application/json"},
	{"JS",		"application/javascript"},
	{"OGG",	"application/ogg"},   
	{"PS",	"application/postscript"},
	{"WOFF","application/x-woff"},
	{"XHTML","application/xhtml+xml"},
	{"XHT",	"application/xhtml+xml"},
	{"ZIP",	"application/zip"},
	{"GZ",	"application/x-gzip"},
	{"RAR",	"application/rar"},
	{"RM",	"application/vnd.rn-realmedia"},
	{"RMVB","application/vnd.rn-realmedia-vbr"},
	{"SWF",	"application/x-shockwave-flash"},
	{"AU",	"audio/basic"},
	{"SND",	"audio/basic"},
	{"MID",	"audio/mid"},
	{"RMI",	"audio/mid"},
	{"MP3",	"audio/mpeg"},
	{"AIF",	"audio/x-aiff"},
	{"AIFC",	"audio/x-aiff"},
	{"AIFF",	"audio/x-aiff"},
	{"M3U",	"audio/x-mpegurl"},
	{"RA",	"audio/vnd.rn-realaudio"},
	{"RAM",	"audio/vnd.rn-realaudio"},
	{"WAV",	"audio/x-wave"},
	{"WMA", "audio/x-ms-wma"},
	{"M4A",	"audio/x-m4a"}, 
	{"BMP",	"image/bmp"},
	{"GIF",	"image/gif"},
	{"JPE",	"image/jpeg"},
	{"JPEG",	"image/jpeg"},
	{"JPG",	"image/jpeg"},
	{"JFIF",	"image/jpeg"},
	{"PNG",	"image/png"},
	{"SVG",	"image/svg+xml"},
	{"TIF",	"image/tiff"},
	{"TIFF",	"image/tiff"},
	{"ICO",	"image/vnd.microsoft.icon"},
	{"CSS",	"text/css"},
	{"BAS",	"text/plain"},
	{"C",		"text/plain"},
	{"H",		"text/plain"},
	{"RTX",	"text/richtext"},
	{"MP2",	"video/mpeg"},
	{"MPA",	"video/mpeg"},
	{"MPE",	"video/mpeg"},
	{"MPEG",	"video/mpeg"},
	{"MPG",	"video/mpeg"},
	{"MPV2",	"video/mpeg"},
	{"MOV",	"video/quicktime"},
	{"QT",	"video/quicktime"},
	{"LSF",	"video/x-la-asf"},
	{"LSX",	"video/x-la-asf"},
	{"ASF",	"video/x-ms-asf"},
	{"ASR",	"video/x-ms-asf"},
	{"ASX",	"video/x-ms-asf"},
	{"AVI",	"video/x-msvideo"},
	{"3GP",	"video/3gpp"},
	{"3GPP",	"video/3gpp"},
	{"3G2",	"video/3gpp2"},
	{"MOVIE","video/x-sgi-movie"},
	{"MP4",	"video/mp4"},
	{"WMV",	"video/x-ms-wmv"},
	{"WEBM","video/webm"},
	{"M4V",	"video/x-m4v"},
	{"FLV",	"video/x-flv"}
};

/*get the right fileName from the whole file path.
[in] szFilePath:   the path of the file
[out] fileName:    the output result of file name.
[in] uSize:  size of the fileName buffer.
*/
void HelperFunctions::GetFileNameFromFilePath(LPCWSTR lpszFilePath, LPWSTR lpszFileName, size_t size)  
{
	//if the last charactor of the file path is '/' or '\' , get rid of it.
	size_t lengthOfFilePath = wcslen(lpszFilePath);		 
	if (lengthOfFilePath <= 0)
		throw std::runtime_error("the filePath can not be empty");
	WCHAR *filePathCopy = new WCHAR[lengthOfFilePath+1](); 
	wcscpy_s(filePathCopy, lengthOfFilePath+1, lpszFilePath);	
	WCHAR* pLasCHAR = filePathCopy+lengthOfFilePath -1;
	if (* pLasCHAR == L'/' || *pLasCHAR ==L'\\')
		*pLasCHAR = L'\0';  

	for (size_t i=0;i<lengthOfFilePath-1; i++)
	{
		--pLasCHAR;
		if ( *pLasCHAR == L'/' || *pLasCHAR == L'\\' )
			break;
	}
	wcscpy_s(lpszFileName, size, ++pLasCHAR);
	delete[] filePathCopy;
}

/* get the length of a file, using windows API , 
this function is valid for large file which is more than 2G bytes. 
(howerver, we should forbid uploading files whose size is more than 2GB)
*/
INT64 HelperFunctions::GetFileSize(LPCWSTR szFilePath)
{
	LARGE_INTEGER sizeOfFile;
	HANDLE hFile = CreateFile(szFilePath, 
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						0,
						0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
	//	DWORD dwErr = ::GetLastError();
		throw std::runtime_error("Is this file opened by another app? ");
	}
	if (! GetFileSizeEx( hFile , &sizeOfFile) )
	{
		CloseHandle(hFile);
		throw std::runtime_error("get file size failed. Note: only windows2000 and above are supported.");
	}
	CloseHandle(hFile);
	return sizeOfFile.QuadPart;
}


/*get the millisecond count since midnight , January 1, 1970, UTC.
*/
INT64 HelperFunctions::GetCurrentTimeStamp()
{
	struct _timeb currentTime;
	_ftime64_s(&currentTime);

	return (currentTime.time*1000 +currentTime.millitm);
}


/********************************************************************
get the filename extension (like exe, dll, txt, doc) from the filename.
[in] szFileName:   the filename of the file
[out] szFileExt:    the extension name of file .
[in] size:  size of the szFileExt buffer.

if the filename do not have an extension, like filename "abc",  the szFileExt will be empty string;
*******************************************************************************/
void HelperFunctions::GetFileExtFromFileName(LPCWSTR szFileName, LPWSTR szFileExt, size_t uSize)  
{
	size_t uLengthOfFileName = wcslen(szFileName);		 

	if (uLengthOfFileName <= 0)
		throw std::runtime_error("the file name can not be empty");


	const WCHAR* pLasWCHAR = szFileName+uLengthOfFileName -1;       
	
	bool hasExt = false;
	for (size_t i=0;i<uLengthOfFileName-1; i++)
	{
		--pLasWCHAR;
		if ( *pLasWCHAR == L'.' )
		{
			hasExt = true;
			break;
		}
	}
		
	memset(szFileExt, 0, uSize); 
	if (hasExt)
	{
		if (wcslen(pLasWCHAR) < uSize)  //prevent copy overflow error
		{
			wcscpy_s(szFileExt, uSize, ++pLasWCHAR);
		}
	}
}

/******************************************************************************
Return the filename without the extension, Like for "abc.txt" , return "abc",  for "xyz" return "xyz"
Param:
	[in]lpszFileName: the file name with extension
	[out] lpszPureName: the file name without extension
	[in] size: the size of lpszPureName buffer.

********************************************************************************/
void	HelperFunctions::GetFileNameWithoutExt(LPCWSTR lpszFileName, LPWSTR lpszPureName, size_t size)
{
	if (lpszFileName == NULL)
	{
		lpszPureName = NULL;
		return;
	}
	if (wcschr(lpszFileName,L'.') == NULL)
	{
		wcscpy_s(lpszPureName, size, lpszFileName);
		return ;
	}

	wcscpy_s(lpszPureName, size, lpszFileName);
	WCHAR* pLastChar = lpszPureName + wcslen(lpszPureName) -1;
	if (*pLastChar ==L'.')
		*pLastChar =L'\0';
}
	
/***********************************************************************************
Get the file's corrresponding MIME Type, The file's mimetype is based on it's file extensions 
Params:
	[in] lpszFileName:  the name of the file , Not the full path

return:
	 the mime type string of the file.   if the file extension does not match any , return "application/octet-stream"

NOTE: the user do NOT need to release the returned string.
*************************************************************************************/
LPCSTR	HelperFunctions::GetMimeTypeFromFileName(LPCWSTR lpszFileName)
{
	WCHAR szFileExt[32]={0};
	HelperFunctions::GetFileExtFromFileName(lpszFileName, szFileExt, 32);
	
	CHAR  szMBFileExt[64]={0};
	WideCharToMultiByte(CP_UTF8,0, szFileExt, -1, szMBFileExt, 64, NULL, NULL);
	
	ConvertToUpper(szMBFileExt);


	for (int i = 0; i < sizeof (MimeTypes) / sizeof (MimeTypes[0]); i++)
	{
		if (strcmp(MimeTypes[i][0],szMBFileExt) == 0)
		{
			return MimeTypes[i][1];
		}
	} 
	return MimeTypes[0][1];   //if no match, return "application/octet-stream"
}


/**************************************
convert the string to Upper case

the return value is the converted uppercase string.
******************************************/
LPSTR HelperFunctions::ConvertToUpper(LPSTR szSource)
{
	assert(szSource != NULL);
	
	LPSTR szString = szSource;
	
	while(*szString)
	{
		*szString =(CHAR)toupper(*szString);
		szString ++;
	}
	return szSource;
} 

/****************************************************************
Convert the string to lower case
 the return value is the converted lowercase string.
******************************************************************/
LPSTR HelperFunctions::ConvertToLower(LPSTR szSource)
{
	assert(szSource != NULL);
	
	LPSTR szString = szSource;
	
	while(*szString)
	{
		*szString = (CHAR)tolower(*szString);
		szString ++;
	}
	return szSource;
}

/**************************************
convert the string to Upper case
the return value is the converted uppercase string.
******************************************/
LPWSTR HelperFunctions::ConvertToUpper(LPWSTR szSource)
{
	assert(szSource != NULL);
	
	LPWSTR szString = szSource;
	
	while(*szString)
	{
		*szString =(WCHAR)towupper(*szString);
		szString ++;
	}
	return szSource;
} 

/****************************************************************
Convert the string to lower case
 the return value is the converted lowercase string.
******************************************************************/
LPWSTR HelperFunctions::ConvertToLower(LPWSTR szSource)
{
	assert(szSource != NULL);
	
	LPWSTR szString = szSource;
	
	while(*szString)
	{
		*szString = (WCHAR)towlower(*szString);
		szString ++;
	}
	return szSource;
}



//根据FolderFeed取得FolderId,如果是根目录，不做任何操作
void	HelperFunctions::GetFolderIdFromFolderFeed(LPCSTR lpszFolderFeed, LPSTR lpszFolderId, size_t uSize)
{
	const char* p = strstr(lpszFolderFeed, "/folder%3A");
	if (p)
	{
		strcpy_s(lpszFolderId, uSize, p+10);
		char * q = strstr(lpszFolderId, "/contents");
		*q='\0';
	}
}

//convert number to  "12,345,678" format string.
void   HelperFunctions::NumberToBuffer   (unsigned   long   n,   WCHAR   *buffer,   int   digits_per_segment,  WCHAR   separator)   
{   
				unsigned   long     multiple;       /*   the   multiple   of   10   to   use   */   
                long   int               cursegment;   /*   largest   power   of   multiple   <   n   */   
                WCHAR                 *p;                     /*   insertion   pointer   into   buffer   */   
                long   int               digits;           /*   current   segment   */   
                int                         nsegments;     /*   number   of   three   digit   segments   */   
                WCHAR                 formatstr[32];   /*   the   format   string   for   each   segment   */   

                /*   Set   up   our   parameters.   */   
                p   =   buffer;   
                *p   =  L'\0 ';   
                multiple   =  (ULONG)pow((float)10,   digits_per_segment);   

                /*   Set   up   a   printf-style   format   string   to   handle   zero   padding     *   each   segment   (i.e.,   formatstr   =   "%03d ").             */   
                swprintf(formatstr, 32,  L"%%0%dd ",   digits_per_segment);   

                /*   Figure   out   how   many   segments   we   have   in   n.   */   
                nsegments   =   ((int)log10((double)n))   /   digits_per_segment;   

                if   (nsegments   >   0)   
                {   
                                /*    `n ' '   requires   more   than   one   segment,   so   handle   each   
                                  *   segment   separately   within   a   loop.   We   have   to   handle   the   
                                  *   first   segment   separately   since   it   doesn 't   require   zero   
                                  *   padding.   We   also   prime   ``cursegment ' '   as   the   highest   
                                  *   power   of   ``multiple ' '   that   is   less   than   or   equal   to   
                                  *   ``n ' '   here.   
                                  */   
                                cursegment   =  (LONG)pow((float)multiple,   nsegments);   
                                digits   =   n   /   cursegment;   
                                p   +=   swprintf(p,   L"%ld ",   digits);   
                                *p++   =   separator;   
                                n   -=   (digits   *   cursegment);   
                                cursegment   /=   multiple;   
                                --nsegments;   

                                /*   Spin   for   the   remaining   powers   of   1000   within   n.   */   
                                while   (nsegments   !=   0)   
                                {   
                                                digits   =   n   /   cursegment;   
                                                p   +=   swprintf(p,   formatstr,   digits);   
                                                *p++   =   separator;   
                                                n   -=   (digits   *   cursegment);   
                                                cursegment   /=   multiple;   
                                                --nsegments;   
                                }   
                                /*   Place   the   final   trailing   digits   on.   */   
                                swprintf(p,   formatstr,   n);   
                }   
                else   
                {   
                                /*   n   only   requires   one   segment   so   no   leading   zeros   are     necessary.                              */   
                                swprintf(p,   L"%ld ",   n);   
                }   
}   



//delete the specific folder
BOOL  HelperFunctions::DeleteDirectory(LPCWSTR lpszDir)
{
	 if (!IsFolderExist(lpszDir))  
	 {
		return TRUE;  //如果文件夹不存在，相当于删除成功！！
	 }
	 
	  int len = wcslen(lpszDir);
	  WCHAR *pszFrom = new TCHAR[len+2];
	  wcscpy_s(pszFrom, len+2, lpszDir);
	  pszFrom[len] = 0;
	  pszFrom[len+1] = 0;
	  
	  SHFILEOPSTRUCT fileop;
	  fileop.hwnd   = NULL;    // no status display
	  fileop.wFunc  = FO_DELETE;  // delete operation
	  fileop.pFrom  = pszFrom;  // source file name as double null terminated string
	  fileop.pTo    = NULL;    // no destination needed
	  fileop.fFlags = FOF_NOCONFIRMATION|FOF_SILENT;  // do not prompt the user
	  
//	  if(!noRecycleBin)
//	    fileop.fFlags |= FOF_ALLOWUNDO;

	  fileop.fAnyOperationsAborted = FALSE;
	  fileop.lpszProgressTitle     = NULL;
	  fileop.hNameMappings         = NULL;

	  int ret = ::SHFileOperation(&fileop);
	  delete [] pszFrom;  
	  return (ret == 0);
}

//Is the folder exist
BOOL  HelperFunctions::IsFolderExist(LPCWSTR lpszDir)
{
	    WIN32_FIND_DATA  wfd;
	    BOOL rValue = FALSE;
	    HANDLE hFind = ::FindFirstFile(lpszDir, &wfd);
	    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	    {
		rValue = TRUE;   
	    }
	    ::FindClose(hFind);
	    
	    return rValue;
}

//替换字符串中某段字符为新的值
string&   HelperFunctions::StringReplace(string&   str, const  string&  old_value, const  string&  new_value)   
{   
	for(string::size_type   pos(0);   pos!=string::npos;   pos+=new_value.length())   
	{   
		if(   (pos=str.find(old_value,pos))!=string::npos   )   
			str.replace(pos,old_value.length(),new_value);   
		else   
			break;   
	}   
	return   str;   
}   

/**************************************************
If the uploading file can be converted to Google Document format, return TRUE
only file with extensions in ExtensionsForDocument[][] can be converted
NOTE: the extensions above can be modified in the future
param:
	lpszFileName: the name of the file, not the full path
***************************************************/
BOOL HelperFunctions::CanBeConvertedToDocument(LPCWSTR lpszFileName)
{

	WCHAR szFileExt[32]={0};
	HelperFunctions::GetFileExtFromFileName(lpszFileName, szFileExt, 32);

	CHAR  szMBFileExt[64]={0};
	WideCharToMultiByte(CP_UTF8,0, szFileExt, -1, szMBFileExt, 64, NULL, NULL);
	
	ConvertToUpper(szMBFileExt);

	for (int i=0; i<sizeof(ExtensionsForDocument)/sizeof(ExtensionsForDocument[0]); i++)
	{
		if (strcmp(ExtensionsForDocument[i], szMBFileExt) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}


/**************************************************
If the uploading file can be converted to Google Spreadsheet format, return TRUE
only file with extensions in ExtensionsForSpreadsheet[][] can be converted
NOTE: the extensions above can be modified in the future
param:
	lpszFileName: the name of the file, not the full path
***************************************************/
BOOL HelperFunctions::CanBeConvertedToSpreadsheet(LPCWSTR lpszFileName)
{

	WCHAR szFileExt[32]={0};
	HelperFunctions::GetFileExtFromFileName(lpszFileName, szFileExt, 32);

	CHAR  szMBFileExt[64]={0};
	WideCharToMultiByte(CP_UTF8,0, szFileExt, -1, szMBFileExt, 64, NULL, NULL);
	
	ConvertToUpper(szMBFileExt);

	for (int i=0; i<sizeof(ExtensionsForSpreadsheet)/sizeof(ExtensionsForSpreadsheet[0]); i++)
	{
		if (strcmp(ExtensionsForSpreadsheet[i], szMBFileExt) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/**************************************************
If the uploading file can be converted to Google Presentation format, return TRUE
only file with extensions in ExtensionsForPresentation[][] can be converted
NOTE: the extensions above can be modified in the future
param:
	lpszFileName: the name of the file, not the full path
***************************************************/
BOOL HelperFunctions::CanBeConvertedToPresentation(LPCWSTR lpszFileName)
{

	WCHAR szFileExt[32]={0};
	HelperFunctions::GetFileExtFromFileName(lpszFileName, szFileExt, 32);

	CHAR  szMBFileExt[64]={0};
	WideCharToMultiByte(CP_UTF8,0, szFileExt, -1, szMBFileExt, 64, NULL, NULL);
	
	ConvertToUpper(szMBFileExt);

	for (int i=0; i<sizeof(ExtensionsForPresentation)/sizeof(ExtensionsForPresentation[0]); i++)
	{
		if (strcmp(ExtensionsForPresentation[i], szMBFileExt) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}



/**************************************************
If the uploading file can be converted to Google Drawing format, return TRUE
only file with extensions in ExtensionsForDrawing[][] can be converted
NOTE: the extensions above can be modified in the future
param:
	lpszFileName: the name of the file, not the full path
***************************************************/
BOOL HelperFunctions::CanBeConvertedToDrawing(LPCWSTR lpszFileName)
{

	WCHAR szFileExt[32]={0};
	HelperFunctions::GetFileExtFromFileName(lpszFileName, szFileExt, 32);

	CHAR  szMBFileExt[64]={0};
	WideCharToMultiByte(CP_UTF8,0, szFileExt, -1, szMBFileExt, 64, NULL, NULL);
	
	ConvertToUpper(szMBFileExt);

	for (int i=0; i<sizeof(ExtensionsForDrawing)/sizeof(ExtensionsForDrawing[0]); i++)
	{
		if (strcmp(ExtensionsForDrawing[i], szMBFileExt) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/********************************************************************
Convert the universal time "2010-05-24T15:09:21.687Z"  (ISO8601)  to LOCAL time format "2010-05-24 19:09:21"
Params:
	lpszOriginalTime: the UTC time (ISO 86001 format)  :   ANSI format
	lpszFormattedLocalTime:  the local time with format "2010-05-24 19:09:21"  : WideChar format!
	uSize : the size of lpszFormattedLocalTime buffer

**********************************************************************/
VOID HelperFunctions::GetFormattedLocalTime(LPCSTR lpszOriginalTime, LPWSTR lpszFormattedLocalTime,  UINT uSize)
{
	BOOL bSuccess = TRUE;
	TIME_ZONE_INFORMATION tzi;

	::GetTimeZoneInformation(&tzi);

	SYSTEMTIME stUniversal;

	CHAR  szYear[5]={0};
	CHAR  szMonth[3]={0};
	CHAR  szDay[3]={0};
	CHAR  szHour[3]={0};
	CHAR  szMinute[3]={0};
	CHAR  szSecond[3]={0};
	CHAR  szMilliSecond[4]={0};

	lstrcpynA(szYear, lpszOriginalTime, 5);
	lstrcpynA(szMonth, lpszOriginalTime+5,3);
	lstrcpynA(szDay, lpszOriginalTime+8,3);
	lstrcpynA(szHour, lpszOriginalTime+11,3);
	lstrcpynA(szMinute, lpszOriginalTime+14,3);
	lstrcpynA(szSecond, lpszOriginalTime+17,3);
	lstrcpynA(szMilliSecond, lpszOriginalTime+20, 4);


	
	stUniversal.wYear = atoi(szYear);
	if ( stUniversal.wYear<1000 || stUniversal.wYear>3000)
		bSuccess = FALSE;

	stUniversal.wMonth = atoi(szMonth);
	if ( stUniversal.wMonth<=0  || stUniversal.wMonth>12 )
		bSuccess = FALSE;

	stUniversal.wDay = atoi(szDay);
	if (stUniversal.wDay<=0  || stUniversal.wDay >31)
		bSuccess = FALSE;

	stUniversal.wHour = atoi(szHour);
	if (stUniversal.wHour<0 || stUniversal.wHour>24)
		bSuccess = FALSE;
	
	stUniversal.wMinute = atoi(szMinute);
	if (stUniversal.wMinute<0 || stUniversal.wMinute>60)
		bSuccess = FALSE;

	stUniversal.wSecond = atoi(szSecond);
	if (stUniversal.wSecond<0 || stUniversal.wSecond>60)
		bSuccess = FALSE;

	stUniversal.wMilliseconds = atoi(szMilliSecond);

	if (bSuccess)
	{

		SYSTEMTIME stLocal;
		::SystemTimeToTzSpecificLocalTime(&tzi, &stUniversal, &stLocal);

		swprintf_s(lpszFormattedLocalTime, uSize, L"%d-%02d-%02d %02d:%02d:%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
	}
	else
	{
		::MultiByteToWideChar(CP_UTF8, 0, lpszOriginalTime, -1, lpszFormattedLocalTime, uSize);
	}
}
