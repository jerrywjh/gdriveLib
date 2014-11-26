/******************************************************************************************
FileName: GDocsAPI.h
Author:   Jerry Wang, 
Version: 2.0
Date:	  2012/1/17
Description:
	This is the main interface of the  Google Docs API, pure C++ implementation. 
	This is the helper class for accessing the Google Docs API, it provides much of the client-side functionalities for applications. 
	for example , loging in and out, uploading files, listing files and folders, shareing files,and so on.
*******************************************************************************************/
#pragma once
#include "stdafx.h"
#include "curl/curl.h"
#include "tinyxml.h"
#include "GDocsUser.h"

#define CHUNKSIZE		1048576		// 1MB : the resumable chunck  size of the upload/download process  
#define LEN_URI			1024			//the length for the request URL
#define UWM_PROGRESS	(WM_APP+0x131)

class GDocsEntry;

/*************************************************************
this struct is for holding the HTTP response info.   
It is used in GDocsAPI::responseWriteCallback();  and  curl's  CURLOPT_WRITEDATA 
**************************************************************/
struct MemoryStruct
{
	char* memory;
	size_t size;
};

struct ContentStruct
{
	BYTE* content;
	size_t size;
};

//this struct is used to hold file  upload/download info
struct TransferInfoStruct
{
	HANDLE hFile;  // the handle of the file to be uploaded
	UINT64 uBytesTransferred; // the number of byte already uploaded 
	HWND  hDlg;		// the window holding the progress bar.
	INT64   lStartTimeStamp;
	BOOL bIsBackground;   //if the transfer is in background
};

//global call back lock
static HANDLE *lock_cs;

/***********************************************
GDocsAPI class.
***********************************************/
class GDocsAPI   
{
public:
	GDocsAPI();  
	~GDocsAPI(void);
private:
	//prevent for copy and assignment of this object.
	GDocsAPI(const GDocsAPI &orig);
	GDocsAPI& operator = (const GDocsAPI &orig);

public:
	void SetAuthCode(LPCWSTR lpszAuthCode);
	void SetRefreshToken(LPCSTR lpszRefreshToken);
	BOOL GetTokensFromAuthCode(); 
	BOOL RefreshForAccessToken(); 
	void Logout();

	GDocsUser* GetUser();
	void GetUserInfo();   

	void GetEntryList(LPCSTR lpszUrl, TiXmlDocument* pXmlDoc);

	//resumable upload file without chunks (upload the whole file once)
	void UploadFile( LPCWSTR lpszFileFullPathName, LPCSTR lpszResumableCreateMediaUrl, HWND hMsg, BOOL bConvert, TiXmlDocument* pResponseDoc, BOOL bIsBackground);
	
	//update the file content with a new revison
	void UpdateFile( LPCWSTR lpszFileFullPathName, LPCSTR lpszResumableEditMediaUrl, LPCSTR lpszGDEtag, BOOL bCreateNewRevision, HWND hMsg, TiXmlDocument* pResponseDoc);

	void ExportFile(LPCSTR lpszContentUrl, LPCSTR lpszFileType, LPCWSTR lpszFilePath, HWND hMsg, LPCSTR lpszFormat);
	
	void CreateFolder(LPSTR lpszFolderName, LPCSTR lpszParentFolderContentLink, TiXmlDocument* pResponseDoc);

	void MoveEntry(LPCSTR lpszResourceId, LPCSTR lpszSrcFolderFeed, LPCSTR lpszDestFolderFeed, BOOL isRemovedFromSrcFolder=TRUE);

	void DeleteEntry(LPSTR lpszResourceId, BOOL isDeleted=FALSE);

	void RenameEntry(LPCSTR lpszEditLink, LPCSTR lpszNewName);

	void StarEntry(LPCSTR lpszEditLink, BOOL starOrNot);

	void RestoreEntry(LPCSTR lpszEditLink);
	
	//functions for sharing
	void GetEntryACL(LPCSTR lpszEntryId, LPCSTR lpszType, TiXmlDocument * pResponseDoc);
	void AddACLUser(LPCSTR lpszEntryId, LPCSTR lpszType, LPCSTR lpszRole, LPCSTR lpszScope, LPCSTR lpszUser);
	void UpdateACLUser(LPCSTR lpszACLEditLink, LPCSTR lpszGDEtag, LPCSTR lpszScopeType, LPCSTR lpszScopeValue, LPCSTR lpszNewRole);
	void DeleteACLUser(LPCSTR lpszACLEditLink);

private:
	GDocsUser *  m_pUser; 

	LPSTR  m_lpszClientId;    
	LPSTR  m_lpszClientSecret; 
	LPSTR  m_lpszRedirectUri;

public:
	LPSTR  m_lpszAuthCode;		//the Authorization Code returned through Google official login page
	LPSTR  m_lpszAccessToken;	//the Access Token
	LPSTR  m_lpszRefreshToken;	//the Refresh Token
	UINT	  m_uExpiresIn;			//the Expires-in of Access Token, in second.

private:
	static size_t responseWriteCallback(void *ptr, size_t size, size_t nmemb, void *stream);
	static size_t WriteToFileCallback(void *ptr, size_t size, size_t nmemb, void* straem);
	static size_t readCallback(void *ptr, size_t size, size_t nmemb, void *stream);
	static size_t readFileCallback(void *ptr, size_t size, size_t nmemb, void *stream);
	
	//callback functins for use OPENSSL in Multithreaded 
	void thread_setup(void);
	void thread_cleanup(void);
	static void locking_callback(int mode ,int type, const char * file, int line);
	
	void assertUserLoggedIn();

	void getErrMsgFromResponse(char* lpszErrXmlResponse, char* lpszErrMsg, int nSize);
	curl_slist* prepareCurl(CURL* curl,  LPCSTR lpszContentType = NULL, LPCSTR lpszIfMatch = NULL);
	void updateMetadata(LPCSTR lpszEditLink, LPCSTR lpszMetadata);
	BOOL isFileConvertable(GDocsUser* pUser, LPCWSTR lpszFileName, INT64 lFileSize);
	curl_slist* generateUploadRequestHeader(CURL* curl, LPCSTR lpszGDEtag, LPCWSTR lpszFileName, LPCSTR lpszMimeType, INT64 lFileSize);
	void initiateResumableUploadRequest(CURL* curl, LPCSTR lpszResumableCreateMediaUrl, LPCWSTR lpszFileName, INT64 lFileSize, BOOL bConvert, LPSTR lpszUploadUri);
	void uploadFileContent(CURL* curl, LPCWSTR lpszFileFullPathName, INT64 lFileSize, LPSTR lpszUploadUri, HWND hMsg, TiXmlDocument* pResponseDoc, BOOL bIsBackground);
	BOOL isParentOfEntry(GDocsEntry* pDocsEntry, LPCWSTR lpszFolderFeed);
};


