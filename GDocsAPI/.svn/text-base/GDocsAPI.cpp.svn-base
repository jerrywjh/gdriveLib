/****************************************************************
This is the main file for Google Docs API functions
Author: Jerry wang 
All right reserved.
*************************************************************/
#include "StdAfx.h"
#include "openssl/crypto.h"
#include "GDocsEntry.h"
#include "GDocsAPI.h"
#include "cJSON.h"
#include "HelperFunctions.h"
#include "GDocsException.h"

//define Progress Message (User-defined Message);
extern volatile LONG g_bCancelTransfering;   

//login errorCode
const char loginErrCode[][32] =
{
	"BadAuthentication",
	"NotVerified",
	"TermsNotAgreed",
	"Unknown",
	"AccountDeleted",
	"AccountDisabled",
	"ServiceDisabled",
	"ServiceUnavailable"
};
// Error code description
const char loginErrCodeDes[][128]=
{
	"The login request used a username or password that is not recognized. ",
	"The account email address has not been verified.",
	"The user has not agreed to terms.",
	"The error is unknown or unspecified",
	"The user account has been deleted.",
	"The user account has been disabled.",
	"The user's access to the specified service has been disabled.",
	"The service is not available; try again later"
};

/************************************************************************************
OpenSSL不是线程安全的，因此在multi-threaded下面用HTTPS/SSL会出错。下面的方法是为了解决这个问题的。
*******************************************************************************************/
void GDocsAPI::thread_setup(void)
{
	int i;
	
	lock_cs = (HANDLE*)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(HANDLE));
	for (i=0; i<CRYPTO_num_locks(); i++)
	{
		lock_cs[i] = ::CreateMutex(NULL, FALSE, NULL);
	}
	
	CRYPTO_set_locking_callback((void(*)(int, int, const char* , int))GDocsAPI::locking_callback);
	/*id callback already defined in Windows*/
}

void GDocsAPI::thread_cleanup(void)
{
	CRYPTO_set_locking_callback(NULL);
	for (int i=0; i<CRYPTO_num_locks(); i++)
		::CloseHandle(lock_cs[i]);
	
	OPENSSL_free(lock_cs);
}


void GDocsAPI::locking_callback(int mode, int type, const char* /*file*/, int /*line*/)
{
	if (mode & CRYPTO_LOCK)
	{
		::WaitForSingleObject(lock_cs[type], INFINITE);
	}
	else
	{
		::ReleaseMutex(lock_cs[type]);
	}
}


/****************************************************************
call back static function to receive the information of HTTP response.
this fuction can be use for receiving response information for ANY HTTP request!
    ***void *realloc(  void *memblock,   size_t size ); The contents of the block are unchanged up to the shorter of the new and old sizes, although the new block can be in a different location. 
*****************************************************************/
 size_t GDocsAPI::responseWriteCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t realSize= size*nmemb;
	struct MemoryStruct * mem = (struct MemoryStruct*)stream;  

	if (mem->memory)
		mem->memory = (char*) realloc(mem->memory, mem->size + realSize+1);  
	else
		mem->memory = (char*) malloc(mem->size+realSize+1);

	if (mem->memory)
	{
		memcpy(&(mem->memory[mem->size]), ptr, realSize);
		mem->size += realSize;
		mem->memory[mem->size]=0;
	}
	return realSize;
}

/**************************************************************************
Write the response to file call back
*************************************************************************/
 size_t GDocsAPI::WriteToFileCallback(void *ptr, size_t size, size_t nmemb, void *stream)
 {
	if (g_bCancelTransfering)
	{
		return CURLE_WRITE_ERROR;
	}
	
	struct TransferInfoStruct*  pDownloadInfo = (struct TransferInfoStruct *)stream;
	
	size_t blockSize = size*nmemb;
	
	HANDLE hFile =  pDownloadInfo->hFile;
	
	DWORD dwWritten =0;
	if (!WriteFile(hFile, ptr, blockSize,  &dwWritten, NULL))
	{
		return CURLE_WRITE_ERROR;
	}
	else if (dwWritten)
	{
		pDownloadInfo->uBytesTransferred += dwWritten;
		INT64 lStartTimeStamp = pDownloadInfo->lStartTimeStamp;
		INT64 lCurrentTimeStamp = HelperFunctions::GetCurrentTimeStamp();
		
		int nSpeed =0;
		if (lCurrentTimeStamp > lStartTimeStamp)
		{
			nSpeed = (INT)((INT64)pDownloadInfo->uBytesTransferred/(lCurrentTimeStamp - lStartTimeStamp));
		}
		
		//PostMessage??SendMessage
		if (pDownloadInfo->hDlg)
		{
//			::PostMessage(pDownloadInfo->hDlg, UWM_PROGRESS, (WPARAM)pDownloadInfo->uBytesTransferred, (LPARAM) nSpeed);
			::SendMessage(pDownloadInfo->hDlg, UWM_PROGRESS, (WPARAM)pDownloadInfo->uBytesTransferred, (LPARAM) nSpeed);

		}
	}
	return blockSize;	
 }
 
 /****************************************************************
 //read call back function, For HTTP PUT command
 ********************************************************************/
 size_t GDocsAPI::readCallback(void *ptr, size_t size, size_t nmemb, void *stream)
 {
 	struct MemoryStruct * mem = (struct MemoryStruct*)stream;
	int iSize = mem->size<size*nmemb? mem->size: size*nmemb;  //min(mem->size, size*nmemb) 
	
	if (iSize >0)
	{
		memcpy(ptr, mem->memory, iSize);
		mem->memory = mem->memory + iSize;
		mem->size -= iSize;
		return iSize;
	}
	else
		return 0;
 }

 
 /*******************************************************************************
 The Callback function for GDocs file upload.

 ******************************************************************************/
size_t GDocsAPI::readFileCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	struct TransferInfoStruct* pUploadInfo = (struct TransferInfoStruct*)stream;
	BOOL bIsBackground = pUploadInfo->bIsBackground;

	// 目前前台仍然只采用一个workerthread来传输文件，有文件在传输的时候，不能增加新的传输(upload/download),y因此可以用一个全局变量来控制
	//对于后台的传输，如"backup to Google docs" "Share with..." "Convert ..." ,不可以cancle!
	if(g_bCancelTransfering & (!bIsBackground) ) 
	{
		return CURL_READFUNC_ABORT;
	}
	

	size_t  blockSize = size*nmemb;	

	HANDLE hFile = pUploadInfo->hFile;
	DWORD dwBytesRead = 0;
	if (!ReadFile(hFile, ptr, blockSize, &dwBytesRead, NULL))
	{
		return CURL_READFUNC_ABORT;
	}
	else if (dwBytesRead)
	{
		//关于速度等的信息，直接在主界面线程中处理，这里只需要传递回去bytesUploaded就可以了。
		pUploadInfo->uBytesTransferred += dwBytesRead;
		INT64 lStartTimeStamp = pUploadInfo->lStartTimeStamp;
		INT64 lCurrentTimeStamp = HelperFunctions::GetCurrentTimeStamp();
		
		int nSpeed =0;
		if (lCurrentTimeStamp > lStartTimeStamp)
		{
			nSpeed =(INT)(pUploadInfo->uBytesTransferred/(lCurrentTimeStamp - lStartTimeStamp));
		}
			
		//Send the progress info message to the Message Dialog, send "speed" and "uBytesTransferred" as params 
		//::SendMessage(pUploadInfo->hDlg, UWM_PROGRESS, (WPARAM)pUploadInfo->uBytesTransferred, (LPARAM)nSpeed);   //send user-defined message to progress bar dialog

		::PostMessage(pUploadInfo->hDlg, UWM_PROGRESS, (WPARAM)pUploadInfo->uBytesTransferred, (LPARAM)nSpeed);   //send user-defined message to progress bar dialog

		return dwBytesRead;   //return the acutal number of bytes readed.
	}
	return 0;  //end of file, no more data left to deliver
}


/********************************************************************* 
GDocsAPI constructor:
	Global libcurl initialisaztion, Not thread safe, so must be called in main thread before any subthread started.

Exception: 
	
********************************************************************/
GDocsAPI::GDocsAPI()
	 :m_lpszClientId("848488221964.apps.googleusercontent.com"),
	  m_lpszClientSecret("32LZDMJfg0fzmYu_6xP-uAMC"),
	  m_lpszRedirectUri("urn:ietf:wg:oauth:2.0:oob"),
	  m_lpszAccessToken(NULL),
	  m_lpszRefreshToken(NULL),
	  m_lpszAuthCode(NULL),
	  m_uExpiresIn(3500),
	  m_pUser(new GDocsUser())
{
	CURLcode curlCode= curl_global_init(CURL_GLOBAL_ALL);

	if (curlCode !=0 )
	{
		throw std::runtime_error("HTTP library  initialization failed.");  	
	}
	
	thread_setup();  //this is used for SSL in multi-threaded enviroment.
}

//Destructor
GDocsAPI::~GDocsAPI(void)
{ 
	if (m_lpszAuthCode)
	{
		delete[] m_lpszAuthCode;
		m_lpszAuthCode = NULL;
	}

	if (m_lpszRefreshToken)
	{
		delete[] m_lpszRefreshToken;
		m_lpszRefreshToken = NULL;
	}

	if (m_lpszAccessToken)
	{
		delete[] m_lpszAccessToken;
		m_lpszAccessToken = NULL;
	}

	delete m_pUser;

	//clean up the SSL thread setup in thread_setup();
	thread_cleanup();
	//golbal cleanup
	curl_global_cleanup();
}

/********************************************************************
Logout():   logs a user out by ending the current session.  
clear all 
*********************************************************************/
void GDocsAPI::Logout()
{
	if (m_lpszAuthCode)
	{
		delete[] m_lpszAuthCode;
		m_lpszAuthCode = NULL;
	}

	if (m_lpszRefreshToken)
	{
		delete[] m_lpszRefreshToken;
		m_lpszRefreshToken = NULL;
	}

	if (m_lpszAccessToken)
	{
		delete[] m_lpszAccessToken;
		m_lpszAccessToken = NULL;
	}
	
	delete m_pUser;  //delete do not need to check NULL.   ???
}


/****************************************************
Check if the user has logged in, (access token is not NULL)
if not, exception throwed.
***************************************************/
void GDocsAPI::assertUserLoggedIn()
{
	if (m_lpszAccessToken == NULL )
	{
		throw std::runtime_error("The user must be logged in.");
	}
}


/***************************************************
Set the m_lpszAuthCode variable (Autorization Code)
Param:
	lpszAuthCode: the WideChar format authcode from the browser's title
****************************************************/
void GDocsAPI::SetAuthCode(LPCWSTR lpszAuthCode)
{
	if (NULL  == lpszAuthCode)
	{
		return;
	}

	if (m_lpszAuthCode)
	{
		delete [] m_lpszAuthCode;
		m_lpszAuthCode = NULL;
	}

	int nDim = wcslen(lpszAuthCode)+1;
	m_lpszAuthCode  = new char[nDim]();
	::WideCharToMultiByte(CP_UTF8, 0, lpszAuthCode, -1, m_lpszAuthCode, nDim, NULL, NULL);
}

/******************************************************
set the value of m_lpszRefreshToken 
******************************************************/
void GDocsAPI::SetRefreshToken(LPCSTR lpszRefreshToken)
{
	if (NULL == lpszRefreshToken)
	{
		return;
	}
	if (m_lpszRefreshToken)
	{
		delete[] m_lpszRefreshToken;
		m_lpszRefreshToken = NULL;
	}

	int nDim = strlen(lpszRefreshToken)+1;
	m_lpszRefreshToken = new char[nDim]();
	strcpy_s(m_lpszRefreshToken, nDim, lpszRefreshToken);
}

/***********************************************************
Get Access Token and Refresh Token from the authorization code,
Set the value of m_lpszAccessToken &  m_lpszRefreshToken

return:
	TRUE for success, FALSE for failed.
***********************************************************/
BOOL GDocsAPI::GetTokensFromAuthCode()
{
	if (m_lpszAuthCode == NULL)
		return FALSE;

	CURL* curl = curl_easy_init();
	if (!curl)
		return FALSE;

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //  do not verify certificate
	curl_easy_setopt(curl, CURLOPT_URL,  "https://accounts.google.com/o/oauth2/token");  

	char szPostBody[1024] ={0};
	strcpy_s(szPostBody, "code=");
	strcat_s(szPostBody, m_lpszAuthCode);
	strcat_s(szPostBody, "&client_id=");
	strcat_s(szPostBody, m_lpszClientId);
	strcat_s(szPostBody, "&client_secret=");
	strcat_s(szPostBody, m_lpszClientSecret);
	strcat_s(szPostBody, "&redirect_uri=");
	strcat_s(szPostBody, m_lpszRedirectUri);
	strcat_s(szPostBody, "&grant_type=authorization_code");

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szPostBody);

	//Set write callback function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);    
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	//perform request
	CURLcode res = curl_easy_perform(curl);
	
	if ( res != CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (curl)
			curl_easy_cleanup(curl);

		return FALSE;
	}

	if ( NULL == responseBuffer.memory)
	{
		if (curl)
			curl_easy_cleanup(curl);

		return FALSE;
	}

	//get access token, refresh token from JSON response
	BOOL bSuccess = TRUE;

	cJSON* pJsonRoot = cJSON_Parse(responseBuffer.memory);
	if ( pJsonRoot)
	{
		//AccessToken
		cJSON* pAccessTokenJson = cJSON_GetObjectItem(pJsonRoot, "access_token");
		if (pAccessTokenJson)
		{
			if (m_lpszAccessToken)
				delete[] m_lpszAccessToken;

			int nDim = strlen(pAccessTokenJson->valuestring)+1;
			m_lpszAccessToken = new char[nDim]();
			strcpy_s(m_lpszAccessToken, nDim, pAccessTokenJson->valuestring);
		}
		else
		{
			bSuccess = FALSE;
		}

		//RefreshToken
		cJSON* pRefreshTokenJson = cJSON_GetObjectItem(pJsonRoot, "refresh_token");
		if (pRefreshTokenJson)
		{
			if (m_lpszRefreshToken)
				delete[] m_lpszRefreshToken;

			int nDim = strlen(pRefreshTokenJson->valuestring)+1;
			m_lpszRefreshToken = new char[nDim]();
			strcpy_s(m_lpszRefreshToken, nDim, pRefreshTokenJson->valuestring);
		}
		else
		{
			bSuccess= FALSE;
 		}

		//Expires-In 
		cJSON* pExpiresInJson = cJSON_GetObjectItem(pJsonRoot, "expires_in");
		if (pExpiresInJson)
		{
			m_uExpiresIn = pExpiresInJson->valueint;
		}
	
		cJSON_Delete(pJsonRoot);  //free the cJSON object
	}
	else
	{
		bSuccess = FALSE;
	}

	//free resources
	if (responseBuffer.memory)
		free(responseBuffer.memory);

	if (curl)
		curl_easy_cleanup(curl);

	return bSuccess;
}

/*********************************************************************************
Get the new Access Token using the Refresh Token

when the access token expires, HTTP GET/POST will get the following error:
		HTTP/1.1 401 Unauthorized
		status: 401
		content-length: 199
		x-xss-protection: 1; mode=block
		x-content-type-options: nosniff
		x-google-cache-control: remote-fetch
		expires: Sat, 24 Dec 2011 08:21:23 GMT
		server: GSE
		via: HTTP/1.1 GWA
		cache-control: private, max-age=0
		date: Sat, 24 Dec 2011 08:21:23 GMT
		x-frame-options: SAMEORIGIN
		content-type: text/html; charset=UTF-8
		www-authenticate: AuthSub realm="http://www.google.com/accounts/AuthSubRequest"
		-content-encoding: gzip
		<HTML> 
		<HEAD> <TITLE>Token invalid - Invalid AuthSub token.</TITLE> </HEAD> 
		<BODY BGCOLOR="#FFFFFF" TEXT="#000000"> <H1>Token invalid - Invalid AuthSub token.</H1> <H2>Error 401</H2> </BODY> 
		</HTML>

关于刷新： 每隔 expired -1000秒的时候刷新access token (互斥） 在ontimer事件中
如果api出现 401上述错误， （比如用户休眠后） 应该再次请求刷新access token.

***********************************************************************/

BOOL  GDocsAPI::RefreshForAccessToken()
{
	if (m_lpszRefreshToken == NULL)
		return FALSE;

	CURL* curl = curl_easy_init();
	if (!curl)
		return FALSE;

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //  do not verify certificate
	curl_easy_setopt(curl, CURLOPT_URL,  "https://accounts.google.com/o/oauth2/token");  

	char szPostBody[1024]={0};
	strcpy_s(szPostBody, "client_id=");
	strcat_s(szPostBody, m_lpszClientId);
	strcat_s(szPostBody, "&client_secret=");
	strcat_s(szPostBody, m_lpszClientSecret);
	strcat_s(szPostBody, "&refresh_token=");
	strcat_s(szPostBody, m_lpszRefreshToken);
	strcat_s(szPostBody, "&grant_type=refresh_token");

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szPostBody);

	//Set write callback function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);    
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	//perform request
	CURLcode res = curl_easy_perform(curl);
	if ( res != CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (curl)
			curl_easy_cleanup(curl);

		return FALSE;
	}
	if (NULL == responseBuffer.memory)
	{
		if (curl)
			curl_easy_cleanup(curl);

		return FALSE;
	}

	//get the Access Token from JSON response
	BOOL bSuccess = TRUE;

	cJSON* pJsonRoot = cJSON_Parse(responseBuffer.memory);

	if ( pJsonRoot)
	{
		//access token 
		cJSON* pAccessTokenJson = cJSON_GetObjectItem(pJsonRoot, "access_token");
		if (pAccessTokenJson)
		{
			if (m_lpszAccessToken)
				delete[] m_lpszAccessToken;

			int nDim = strlen(pAccessTokenJson->valuestring)+1;
			m_lpszAccessToken = new char[nDim]();
			strcpy_s(m_lpszAccessToken, nDim, pAccessTokenJson->valuestring);
		}
		else
		{
			bSuccess = FALSE;
		}
	
		//expires in
		cJSON* pExpiresInJson = cJSON_GetObjectItem(pJsonRoot, "expires_in");
		if (pExpiresInJson)
		{
			m_uExpiresIn = pExpiresInJson->valueint;
		}	

		cJSON_Delete(pJsonRoot);
	}
	else
	{
		bSuccess = FALSE;
	}

	//free resources
	if (responseBuffer.memory)
		free(responseBuffer.memory);

	if (curl)
		curl_easy_cleanup(curl);

	return bSuccess;
}


/************************************************************************************
If a GDocs API request failed, usually a XML-format error message is returned.
In most cases, we need to  throw an Exception with the error information to notify the user
that this request is failed for what reason.
This method is used to get the error message from the XML file.

param: 
	[in] lpszErrXmlResponse: the Xml-format Error message; 
	[out] lpszErrMsg: the string for the exception we are going to throw. it's size should be 512 or above
	[in] nSize: the size of the lpszErrMsg buffer, usually 512.
A typical Error Response message is like this:
<errors xmlns='http://schemas.google.com/g/2005'>
	<error>
		<domain>GData</domain>
		<code>ResourceNotFoundException</code>
		<internalReason>Invalid document id: folder:0B7CmbVXdOi7mZjhiYjNiNGItN33TY2MS00ZDFlLWEzYWMtOTFhOWU3NDI5MjFm</internalReason>
	</error>
</errors>
**********************************************************************************/
void GDocsAPI::getErrMsgFromResponse(char* lpszErrXmlResponse, char* lpszErrMsg, int nSize)
{	
	TiXmlDocument* pErrDoc = new TiXmlDocument();
	pErrDoc->Parse(lpszErrXmlResponse, 0, TIXML_ENCODING_UTF8);

	TiXmlElement *pRootEle = pErrDoc->FirstChildElement("errors");
	if (pRootEle)
	{
		TiXmlElement *pErrEle = pRootEle->FirstChildElement("error");
		if (pErrEle)
		{
			TiXmlElement* pErrCodeEle = pErrEle->FirstChildElement("code");
			if (pErrCodeEle)
				strcpy_s(lpszErrMsg, nSize, pErrCodeEle->GetText());

			strcat_s(lpszErrMsg, nSize, " : ");

			TiXmlElement* pErrReasonEle = pErrEle->FirstChildElement("internalReason");
			if (pErrReasonEle)
				strcat_s(lpszErrMsg, nSize, pErrReasonEle->GetText()); 
		}
	}
	else
	{
		lpszErrMsg =" Operation failed for unknown reason.";
	}
	delete pErrDoc;
}

/*************************************************************************
Prepare curl handle for the following http request.
Set the libCurl HTTP request 's header, Setup the debug information, Disable libCurl Certificate verifaction

params:
	[in]curl:  the handle of curl
	[in]lpszContentType: the Content Type of the http request. like "application/atom+xml" ,   NULL for default, 
		in most request, just ignore this param and let libcurl choose the right Content-Type.
	[in]lpszIfMatch:   
			NULL : this header is not required, this is the default value
			*: 	ignore match or not
			Entry-Id:  do it only when match

return:  curl_slist *    the linked list of header string, which must be freed after curl_easy_perform();  //for example: curl_slist_free_all(slist);
**************************************************************************/
curl_slist* GDocsAPI::prepareCurl(CURL* curl,  LPCSTR lpszContentType, LPCSTR lpszIfMatch)
{
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //  do not verify certificate

	struct curl_slist * headers= NULL;
	headers = curl_slist_append(headers, "GData-Version: 3.0");

	//add authorization header
	int iAuthLen = strlen("Authorization: Bearer ") +strlen(m_lpszAccessToken) +8;
	CHAR* szAuthToken = new CHAR[iAuthLen]();
	strcpy_s(szAuthToken, iAuthLen, "Authorization: Bearer ");
	strcat_s(szAuthToken, iAuthLen, m_lpszAccessToken);

	headers = curl_slist_append(headers, szAuthToken);
	delete [] szAuthToken;
	
	if (lpszContentType)
	{
		char szContentType[256] ={0};
		strcpy_s(szContentType, "Content-Type:");
		strcat_s(szContentType, lpszContentType);
		headers = curl_slist_append(headers, szContentType);
	}
	
	if (lpszIfMatch)
	{
		char szIfMatch[256]={0};
		strcpy_s(szIfMatch, "If-Match: ");
		strcat_s(szIfMatch, lpszIfMatch);
		headers = curl_slist_append(headers, szIfMatch);
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	return headers;
}

/***************************************************************************************
Update the meta data of the specific entry,
like: rename the entry, starred/unstarred , view/unview the entry,
params:
	[in] lpszEditLink:  The edit link of the entry, like:href="https://docs.google.com/feeds/default/private/full/document%3A19QlFP5je6Suh7OwnuoyC2Kru5id6UIGMBeaFwbdYQo0"
	[in] lpszMetadata:  like 
		"<title>abc.jpb</title>",   
		"<category scheme="http://schemas.google.com/g/2005/labels" term="http://schemas.google.com/g/2005/labels#viewed" label="viewed"/>"
		<category scheme="http://schemas.google.com/g/2005/labels" term="http://schemas.google.com/g/2005/labels#starred" label="starred"/>
 
***************************************************************************************/
void  GDocsAPI::updateMetadata(LPCSTR lpszEditLink, LPCSTR lpszMetadata)
{
	assertUserLoggedIn(); 

	if (!lpszEditLink || strlen(lpszEditLink)==0)
		throw std::runtime_error("Invalid Entry .");

	CURL* curl = curl_easy_init();
	curl_slist * headerList = prepareCurl(curl, "application/atom+xml", "*");

	curl_easy_setopt(curl,CURLOPT_UPLOAD, 1L);   //use PUT method

	curl_easy_setopt(curl, CURLOPT_URL, lpszEditLink);

	//xml body, set read functon
	int nDim = strlen(lpszMetadata) +384;
	char* lpszBody =  new char[nDim]();
	strcpy_s(lpszBody, nDim, "<?xml version='1.0' encoding='UTF-8'?>");
	strcat_s(lpszBody,  nDim, "<entry xmlns=\"http://www.w3.org/2005/Atom\"  xmlns:docs=\"http://schemas.google.com/docs/2007\"  xmlns:gd=\"http://schemas.google.com/g/2005\">");
	strcat_s(lpszBody,  nDim, lpszMetadata);
	strcat_s(lpszBody,  nDim, "</entry>");

	struct MemoryStruct readBuffer;
	readBuffer.memory = lpszBody;
	readBuffer.size = strlen(lpszBody);

	curl_easy_setopt(curl, CURLOPT_READFUNCTION, readCallback);
	curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&readBuffer);

	//set the response write function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);   
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	CURLcode res = curl_easy_perform(curl);
	delete [] lpszBody;

	if (res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(" Operation Failed, Probably due to connection problem.");
	}

	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
	if (statusCode >=400)  //error happens!
	{
		char szErrMsg[512] ={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);
		
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(szErrMsg);
	}

	if (responseBuffer.memory)
		free(responseBuffer.memory);
	if (headerList)
		curl_slist_free_all(headerList);
	if (curl)
		curl_easy_cleanup(curl);
}

/********************************************
get the current user object
************************************************/
GDocsUser* GDocsAPI::GetUser()
{
	return m_pUser;
}


/*****************************************************************
Get the general information about the current Google user, including email, name, qutoa ...
And update  m_pUser object!
Actually, this function can be viewed as  "UpdateUserInfo"
****************************************************************/
void GDocsAPI::GetUserInfo()
{
	assertUserLoggedIn();

	CURL* curl = curl_easy_init();
	if (!curl)
		throw std::runtime_error("Connection failed");

	//set the header and other options, headerList should be freed after curl_easy_perform()
	curl_slist * headerList = prepareCurl(curl);
	
	curl_easy_setopt(curl, CURLOPT_URL, "https://docs.google.com/feeds/metadata/default");

	//Set write callback function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);    
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	CURLcode res= curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headerList)
			curl_slist_free_all(headerList);	//free hader slist
		
		if (curl)
			curl_easy_cleanup(curl);		//free curl handle 

		m_pUser->SetQuotaBytesTotal(1024*1024*1024);
		m_pUser->SetQuotaBytesUsed(0);
		m_pUser->SetQuotaBytesUsedInTrash(0);
		m_pUser->SetMaxUploadSizeForDocument(26214400 );
		m_pUser->SetMaxUploadSizeForSpreadsheet(10485760);
		m_pUser->SetMaxUploadSizeForPresentation(10485760);
		m_pUser->SetMaxUploadSizeForDrawing(2097152);
		m_pUser->SetMaxUploadSizeForPdf(10737418240);
		m_pUser->SetMaxUploadSizeForFile(10737418240);
		
		return;
	}

	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

	if (statusCode <400)  //succeed
	{
		TiXmlDocument* pXmlDoc = new TiXmlDocument();
		pXmlDoc->Parse(responseBuffer.memory, 0, TIXML_ENCODING_UTF8);  

		//get the general user information from xml response
		TiXmlElement* pEntryElement = pXmlDoc->FirstChildElement("entry");
		if (pEntryElement)
		{
			TiXmlElement* pAuthorElement = pEntryElement->FirstChildElement("author");
			if (pAuthorElement)
			{
				TiXmlElement* pEmailElement = pAuthorElement->FirstChildElement("email");
				if (pEmailElement)
				{
					m_pUser->SetEmail( pEmailElement->GetText() );
				}

				TiXmlElement* pNameElement = pAuthorElement->FirstChildElement("name");
				if (pNameElement)
				{
					m_pUser->SetName( pNameElement->GetText() );
				}
			}

			TiXmlElement* pQuotaBytesTotalElement = pEntryElement->FirstChildElement("gd:quotaBytesTotal");
			if (pQuotaBytesTotalElement)
			{
				m_pUser->SetQuotaBytesTotal( _atoi64(pQuotaBytesTotalElement->GetText()) );
			}

			TiXmlElement* pQuotaBytesUsedElement = pEntryElement->FirstChildElement("gd:quotaBytesUsed");
			if (pQuotaBytesUsedElement)
			{
				m_pUser->SetQuotaBytesUsed( _atoi64(pQuotaBytesUsedElement->GetText()) );
			}

			TiXmlElement* pQuotaBytesUsedInTrashElement = pEntryElement->FirstChildElement("docs:quotaBytesUsedInTrash");
			if (pQuotaBytesUsedInTrashElement)
			{
				m_pUser->SetQuotaBytesUsedInTrash( _atoi64(pQuotaBytesUsedInTrashElement->GetText()) );
			}

			TiXmlElement* pMaxUploadSizeElement = pEntryElement->FirstChildElement("docs:maxUploadSize");  
			for ( pMaxUploadSizeElement; pMaxUploadSizeElement; pMaxUploadSizeElement=pMaxUploadSizeElement->NextSiblingElement("docs:maxUploadSize") )
			{
				if (strcmp(pMaxUploadSizeElement->Attribute("kind"),"document") == 0)
				{
					m_pUser->SetMaxUploadSizeForDocument( _atoi64(pMaxUploadSizeElement->GetText()) );
				}

				if (strcmp(pMaxUploadSizeElement->Attribute("kind"),"spreadsheet") == 0)
				{
					m_pUser->SetMaxUploadSizeForSpreadsheet( _atoi64(pMaxUploadSizeElement->GetText()) );
				}

				if (strcmp(pMaxUploadSizeElement->Attribute("kind"),"presentation") == 0)
				{
					m_pUser->SetMaxUploadSizeForPresentation( _atoi64(pMaxUploadSizeElement->GetText()) );
				}

				if (strcmp(pMaxUploadSizeElement->Attribute("kind"),"drawing") == 0)
				{
					m_pUser->SetMaxUploadSizeForDrawing( _atoi64(pMaxUploadSizeElement->GetText()) );
				}

				if (strcmp(pMaxUploadSizeElement->Attribute("kind"),"pdf") == 0)
				{
					m_pUser->SetMaxUploadSizeForPdf( _atoi64(pMaxUploadSizeElement->GetText()) );
				}

				if (strcmp(pMaxUploadSizeElement->Attribute("kind"),"file") == 0)
				{
					m_pUser->SetMaxUploadSizeForFile( _atoi64(pMaxUploadSizeElement->GetText()) );
				}
			}
		}

		delete pXmlDoc;

		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headerList)
			curl_slist_free_all(headerList);	//free hader slist
		
		if (curl)
			curl_easy_cleanup(curl);		//free curl handle 
	}
	else  //error happend， use the default value
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headerList)
			curl_slist_free_all(headerList);	//free hader slist
		
		if (curl)
			curl_easy_cleanup(curl);		//free curl handle 
	}
}


/*************************************************************************************
Get  all entries of different type in a specific folder 
This function  can be used to retrieve different entries. 
parameters:
	[in] lpszUrl:  FOR all enties in ROOT: https://docs.google.com/feeds/default/private/full?showfolders=true
				for next page: https://docs.google.com/feeds/default/private/full?start-key=EAEasdfldsjfkjfdfsldf, 由,<feed> 's next link决定
				for folder: https://docs.google.com/feeds/default/private/full/folder%2Afolder_id/content   (不需要showfolders=true)
				for all spreadsheet: https://docs.google.com/feeds/default/private/full/-/spreadsheet
				for all starred PDF: https://docs.google.com/feeds/default/private/full/-/pdf/starred 
				for filename contain "qian qian": https://docs.google.com/feeds/default/private/full?title=qian+qian"
				.....
				.....
	[out] pXmlDoc:  the tinyXml doc objetct


google docs returns 100 entry list for each request, which means the entries in pXmlDoc is no more than 100.
so if the user want all the whole entrylist, he should check the returned pXmlDoc to see if "start-key" link exist.
usually it should be done in a loop operation.
**************************************************************************************/
void  GDocsAPI::GetEntryList(LPCSTR lpszUrl, TiXmlDocument* pXmlDoc)
{	
	assertUserLoggedIn();  

	CURL* curl = curl_easy_init(); 
	if (!curl)
		throw std::runtime_error("Connection failed");  	

	//set the header and other options, headerList should be freed after curl_easy_perform()
	curl_slist * headerList = prepareCurl(curl);
	
	
	//char* encodedUrl = curl_easy_escape(curl, lpszUrl,  0);
	curl_easy_setopt(curl, CURLOPT_URL, lpszUrl);
	//curl_free(encodedUrl);

	//Set write callback function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);    
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	CURLcode res= curl_easy_perform(curl);
	if (res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headerList)
			curl_slist_free_all(headerList);	//free hader slist
		
		if (curl)
			curl_easy_cleanup(curl);		//free curl handle 

		throw std::runtime_error("Get entry list failed");
	}

	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

	if (statusCode <400)  //succeed
	{
		pXmlDoc->Clear(); 
		pXmlDoc->Parse(responseBuffer.memory, 0, TIXML_ENCODING_UTF8);  

		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headerList)
			curl_slist_free_all(headerList);	//free hader slist
		
		if (curl)
			curl_easy_cleanup(curl);		//free curl handle 
	}
	else  //error happend
	{
		char szErrMsg[512]={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);

		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headerList)
			curl_slist_free_all(headerList);	//free hader slist
		
		if (curl)
			curl_easy_cleanup(curl);		//free curl handle 
	
		throw std::runtime_error(szErrMsg);
	}
}

/************************************************************
there are different size limitations to convert to document, spreadsheet, presentation,drawing files 
if the uploading file cannot be converted to corresponding type, return FALSE, else return TRUE
params:
	pUser: the current login user
	lpszFileName: the name of the uploading file, not the full path
	lFileSize: the size of the uploading file
****************************************************************/
BOOL GDocsAPI::isFileConvertable(GDocsUser* pUser, LPCWSTR lpszFileName, INT64 lFileSize)
{
	if (HelperFunctions::CanBeConvertedToDocument(lpszFileName))
	{
		if (lFileSize <= pUser->GetMaxUploadSizeForDocument())
		{
			return TRUE;
		}
	}

	if (HelperFunctions::CanBeConvertedToSpreadsheet(lpszFileName))
	{
		if (lFileSize <= pUser->GetMaxUploadSizeForSpreadsheet())
		{
			return  TRUE;
		}
	}

	if (HelperFunctions::CanBeConvertedToPresentation(lpszFileName))
	{
		if (lFileSize <= pUser->GetMaxUploadSizeForPresentation())
		{
			return TRUE;
		}
	}

	if (HelperFunctions::CanBeConvertedToDrawing(lpszFileName))
	{
		if (lFileSize <= pUser->GetMaxUploadSizeForDrawing())
		{
			return TRUE;
		}
	}
	return FALSE;
}

/***************************************************************************
Generate the upload HTTP Post header for the curl request
params:
	[in] curl:  the curl handle
	[in] lpszGDEtag: the gd:etag for the original entry to be overwrite. For first-time upload/normal upload, this is NULL.   for file updating, it should be "*" now (temperary due to a bug in google docs api)
	[in] lpszFileName: the name of the uploading file , not the path.  For file updating, this is not necessary, it should be  NULL , f 
	[in] Mimetype, like "application/pdf"
	[in] lFileSize: the size of the uploading file

return:  the header linked list, which must be freed after curl_easy_perform() .  like: curl_slist_free_all( headers)
*****************************************************************************/
curl_slist* GDocsAPI::generateUploadRequestHeader(CURL* curl, LPCSTR lpszGDEtag,  LPCWSTR lpszFileName, LPCSTR lpszMimeType, INT64 lFileSize)
{
	struct curl_slist * headers= NULL;
	headers = curl_slist_append(headers, "GData-Version:3.0");

	//add authorization header
	int iAuthLen = strlen("Authorization: Bearer ") +strlen(m_lpszAccessToken) +8;
	CHAR* szAuthToken = new CHAR[iAuthLen]();
	strcpy_s(szAuthToken, iAuthLen, "Authorization: Bearer ");
	strcat_s(szAuthToken, iAuthLen, m_lpszAccessToken);
	headers = curl_slist_append(headers, szAuthToken);
	delete [] szAuthToken;

	if (lpszGDEtag)
	{
		CHAR szIfMatch[256]={0};
		strcpy_s(szIfMatch, "If-Match:");
		strcat_s(szIfMatch, lpszGDEtag);
		headers = curl_slist_append(headers, szIfMatch);
	}

	//add Content-Length header : "Content-Length:0"; use Slug to define the filename(Title)
	headers = curl_slist_append(headers, "Content-Length:0");

	//add Content-Type header
	CHAR szContentType[256]={0};  //like:  "Content-Type:application/pdf
	if (lpszMimeType!= NULL)
	{
		strcpy_s(szContentType, "Content-Type: ");
		strcat_s(szContentType, lpszMimeType);
	}
	headers = curl_slist_append(headers, szContentType);

	//add  slug file name header
	if (lpszFileName)
	{
		CHAR szSlug[MAX_PATH] ={0};
		//convert to UTF_8 format, and encoded
		CHAR szUTF8FileName[MAX_PATH]={0};
		WideCharToMultiByte(CP_UTF8, 0, lpszFileName, -1, szUTF8FileName, MAX_PATH, NULL, NULL);
		char* szEncodedFileName = curl_easy_escape(curl, szUTF8FileName, strlen(szUTF8FileName));   //need to be freed later
		strcpy_s(szSlug, "Slug: ");
		if (strlen(szEncodedFileName)>250)   //truncate if the filename is too long
		{
			*(szEncodedFileName+250) =0;
		}
		strcat_s(szSlug, szEncodedFileName);
		headers = curl_slist_append(headers, szSlug);
		curl_free(szEncodedFileName); //free the encoded string
	}

	//add X-Upload-Content-Type:
	CHAR szXUploadContentType[256]={0};
	if (lpszMimeType != NULL)
	{
		strcpy_s(szXUploadContentType, "X-Upload-Content-Type:");
		strcat_s(szXUploadContentType, lpszMimeType);
	}
	else
	{
		strcpy_s(szXUploadContentType, "X-Upload-Content-Type: application/octet-stream");
	}
	headers = curl_slist_append(headers, szXUploadContentType);

	//Add X-upload-content-length
	CHAR szXUploadContentLength[64]={0};
	sprintf_s(szXUploadContentLength, sizeof(szXUploadContentLength), "X-Upload-Content-Length: %I64d", lFileSize);
	headers = curl_slist_append(headers, szXUploadContentLength);

	return headers;
}

/**********************************************************************
Initiate a resumable upload session, send an HTTP Post request to the "resumable-create-media" link
No file content is uploaded in this request,  file content are transferred in the following request
Param:
	[in] curl : the curl handle
	[in] lpszResumableCreateMediaUrl: the link url to request
	[in] bConvert : TRUE or FALSE
	[out] lpszUploadUri:  the result upload-uri for the following file content uploading

	Exception: if this failed,  runtime_error exception throwed


**********************************************************************/
void GDocsAPI::initiateResumableUploadRequest(CURL* curl, LPCSTR lpszResumableCreateMediaUrl, LPCWSTR lpszFileName, INT64 lFileSize, BOOL bConvert, LPSTR lpszUploadUri)
{		
	CHAR szContentType[256]={0};  //like:  "Content-Type:application/pdf
	LPCSTR szMimeType =  HelperFunctions::GetMimeTypeFromFileName(lpszFileName); 
	strcpy_s(szContentType, "Content-Type: ");
	strcat_s(szContentType, szMimeType);

	curl_slist* headers = generateUploadRequestHeader(curl, NULL,  lpszFileName, szMimeType, lFileSize);   //the headers need to be freed after curl_easy_perform()
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //  do not verify certificate
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	if (bConvert)
	{
		curl_easy_setopt(curl, CURLOPT_URL, lpszResumableCreateMediaUrl);
	}
	else
	{
		int nDim = strlen(lpszResumableCreateMediaUrl)+32;
		char* pszPostUrl = new char[nDim]();
		strcpy_s(pszPostUrl, nDim, lpszResumableCreateMediaUrl);
		strcat_s(pszPostUrl, nDim,"?convert=false");
		curl_easy_setopt(curl,CURLOPT_URL, pszPostUrl);
		delete [] pszPostUrl;
	}

	//set the response write function to get the response HEADER
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, GDocsAPI::responseWriteCallback);   
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&responseBuffer);
	 
	CURLcode res = curl_easy_perform(curl);
	if ( res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headers)
			curl_slist_free_all(headers); 
		
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error("Failed to upload file, upload request failed.");
	}

	//get response info..	Google returns 200 for sucessful file resumable uploda request. format: "200 OK  Location:<upload_uri>"
	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

	if (statusCode == 200 )
	{
		char* pszUri = strstr(responseBuffer.memory, "Location:");
		if (pszUri)
		{
			pszUri +=10;
			char * pszUriEnd = strstr(pszUri, "\r\n");
			if (pszUriEnd)
			{
				pszUriEnd[0] = 0;
			}

			strcpy_s(lpszUploadUri, LEN_URI, pszUri);
			
			if (responseBuffer.memory)
				free(responseBuffer.memory);
			
			if (headers)
				curl_slist_free_all(headers); 

			//if (curl)					//do not cleanup this curl Handle, it will be continuelly used in the content upload request.
			//	curl_easy_cleanup(curl);
		}
		else
		{
			if (responseBuffer.memory)
				free(responseBuffer.memory);

			if (headers)
				curl_slist_free_all(headers); 

			if (curl)
				curl_easy_cleanup(curl);

			throw std::runtime_error("File upload failed: no upload uri is generated.");
		}
	}
	else
	{
		char szErrMsg[512]={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);
		
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headers)
			curl_slist_free_all(headers); 

		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(szErrMsg);
	}
}

/*********************************************************************************************
upload the file content by PUT to the URI got from initiateResumableUploadRequest(),this function is called after initiateResumableUploadRequest()
Upload the whole file once, not in chunks,   For chunks upload, see uploadFileContentInChunks()

Param:
	[in] curl: the curl handle, this handle should be a clean one.  
	[in] lpszFileFullPathName:  the path of the uploading file
	[in] lpszUploadUri:  the upload uri for this HTTP PUT operation
	[in] hMsg:  the handle of the progress message window
	[out] pResponseDoc: the XML result of the new uploaded entry
	[in] bIsBackground:  TRUE: uploading in bacground without any progress window. 

**********************************************************************************************/
VOID GDocsAPI::uploadFileContent(CURL* curl, LPCWSTR lpszFileFullPathName, INT64 lFileSize, LPSTR lpszUploadUri, HWND hMsg, TiXmlDocument* pResponseDoc, BOOL bIsBackground)
{
	//open the file for reading
	HANDLE hFile = ::CreateFileW(lpszFileFullPathName, GENERIC_READ,  
									FILE_SHARE_READ, NULL, OPEN_EXISTING, 
									NULL, NULL); 
	if (hFile == INVALID_HANDLE_VALUE)
	{
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error("open file failed!");
	}

	//add header
	struct curl_slist* headers = NULL;  

	//Content-Length
	char szContentLength[64] ={0};
	sprintf_s(szContentLength, sizeof(szContentLength), "Content-Length: %I64d", lFileSize);
	headers = curl_slist_append(headers, szContentLength);

	//Content-type 
	WCHAR lpszFileName[MAX_PATH];
	HelperFunctions::GetFileNameFromFilePath(lpszFileFullPathName, lpszFileName, MAX_PATH); 
	
	CHAR szContentType[256]={0};  //like:  "Content-Type:application/pdf
	LPCSTR szMimeType =  HelperFunctions::GetMimeTypeFromFileName(lpszFileName); 
	strcpy_s(szContentType, "Content-Type: ");
	strcat_s(szContentType, szMimeType);
	
	headers = curl_slist_append(headers, szContentType);

	//content-Range
	char szContentRange[128] = {0};

	if (lFileSize>0)
	{
		sprintf_s(szContentRange, sizeof(szContentRange), "Content-Range: bytes 0-%I64d/%I64d",  lFileSize-1, lFileSize); 
		headers = curl_slist_append(headers, szContentRange);
	}

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);   //use PUT method
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //  do not verify certificate
	curl_easy_setopt(curl, CURLOPT_URL, lpszUploadUri);  //set URL

	//set the read function
	struct TransferInfoStruct uploadInfo;
	uploadInfo.hFile = hFile;
	uploadInfo.uBytesTransferred =0;
	uploadInfo.hDlg = hMsg;
	uploadInfo.lStartTimeStamp = HelperFunctions::GetCurrentTimeStamp();
	uploadInfo.bIsBackground = bIsBackground;
	curl_easy_setopt(curl, CURLOPT_READFUNCTION,  GDocsAPI::readFileCallback);
	curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&uploadInfo );

	//set the response write function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);   
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);
	 
	CURLcode res = curl_easy_perform(curl);
	if ( res!= CURLE_OK)
	{
		//free the memory 
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headers)
			curl_slist_free_all(headers); 

		if (curl)
			curl_easy_cleanup(curl);

		::CloseHandle(hFile);

		throw std::runtime_error("Failed to upload file");
	}

	::CloseHandle(hFile);

	//get response info..	Google returns 201 for sucessful file creation.
	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

	if ((statusCode == 201) || (statusCode ==200) )  //upload 201 or update:200
	{
		if (pResponseDoc)
		{
			pResponseDoc->Clear();
			pResponseDoc->Parse(responseBuffer.memory, 0, TIXML_ENCODING_UTF8);
		}

		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headers)
			curl_slist_free_all(headers); 

		//if (curl)
		//	curl_easy_cleanup(curl);
	}
	//目前这个仍然有问题， 无法实现 断点续传 , 很多时候，休眠重启后 并不是返回503错误 ，而是  res!= CURLE_OK
	//else if (statusCode =503 )  //比如用户休眠了以后，就会发生这个错误，应该继续运行。,这个可以实现断点续传！！！！！！！！ 这个究竟有没有必要 ？？？反复测试之 !!!
	//{
	//	//continue to upload the unfinished part of the file content
	//	if (responseBuffer.memory)
	//		free(responseBuffer.memory);

	//	if (headers)
	//		curl_slist_free_all(headers); 



	//	//if (curl)
	//	//	curl_easy_reset(curl);
	//	//else
	//	//	curl = curl_easy_init();
	//	if (curl)
	//		curl_easy_cleanup(curl);
	//	curl = curl_easy_init();

	//	INT64 lLastByte = getResumableRange(curl, lFileSize, lpszUploadUri, LEN_URI);   // if Range :0-123. then the lastByte = 124.
	//	
	//	resumeUploadFileContent(curl, lpszFileFullPathName, lFileSize, lpszUploadUri, lLastByte, hMsg, pResponseDoc);
	//}
	else
	{
		char szErrMsg[512]={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);

		if (strlen(szErrMsg)<3)
			sprintf_s(szErrMsg, 512, "Upload failed, Error Code: %d", statusCode);

		if (strstr(szErrMsg, "etagsMismatch"))
			strcpy_s(szErrMsg, "File update failed: modification conflict - this file has been modified by someone else, please redownload it and try again");

		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headers)
			curl_slist_free_all(headers); 

		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(szErrMsg);
	}
}


/*******************************************************************************
Upload file to Google docs, including converted file and unconverted file.  
Use Resumable upload method, but do not split the file content into chunks  --- upload the whole file one time
Params:
	[in]lpszFileFullPathName: the name of the file, including the path. for example: L "e:\test\doc\aaa.txt"
	[in]lpszResumableCreateMediaUrl:   <link rel="http://schemas.google.com/g/2005#resumable-create-media" type="application/atom+xml" href="https://docs.google.com/feeds/upload/create-session/default/private/full"/>
	[in]hMsg:  the progressbar  message window
	[in]bConvert: TRUE: convert to Google format, FALSE: not convert, keep the original format. 
	[out] pResponseDoc: the TinyXml Document object for <entry> response.
	[in]bIsBackground: if the upload is running in background without any progress window, TRUE.     if running in foreground, FALSE.  background的upload不能cancel 

Exception:
	runtime_error:

if file conversion failed, retry with "convert=flase"

**********************************************************************************************/
void GDocsAPI::UploadFile( LPCWSTR lpszFileFullPathName, LPCSTR lpszResumableCreateMediaUrl, HWND hMsg, BOOL bConvert, TiXmlDocument* pResponseDoc, BOOL bIsBackground)
{
	assertUserLoggedIn(); 
	
	//arguments checking
	if (!lpszFileFullPathName || wcslen(lpszFileFullPathName) ==0 ) 
	{
		throw std::runtime_error("the file name is invalid!");
	}
	
	//get the filename from file full path
	WCHAR lpszFileName[MAX_PATH];
	HelperFunctions::GetFileNameFromFilePath(lpszFileFullPathName, lpszFileName, MAX_PATH); 
	
	//check the size of file: different type has different max size for uploading  
	INT64 lFileSize = HelperFunctions::GetFileSize(lpszFileFullPathName);   	

	if (lFileSize > (m_pUser->GetQuotaBytesTotal()-m_pUser->GetQuotaBytesUsed()))  //quota exceeded!
	{
		throw std::runtime_error(" Your Google space quota is not enough.");
	}

	if (lFileSize > m_pUser->GetMaxUploadSizeForFile())   // default: 10GB  
		throw std::runtime_error(" File is too large to upload."); 

	// if bConvert=TRUE,  only files with specific extensions and smaller size can be converted. Size limitation is different for different file types. If filesize exceed max limit, upload it in original format(NOT convert)	// if bConvert = FALSE,  usually file size does not exceed 10GB
	if (bConvert)
	{
		if (!isFileConvertable(m_pUser, lpszFileName, lFileSize))
			bConvert = FALSE;
	}

	//do resumable upload
	CURL* curl = curl_easy_init();     //this curl handle is used in 2 requests.
	if (!curl)
	{
		throw std::runtime_error("Connection failed");  	
	}
	
	char szUploadUri[LEN_URI]={0};
	
	//For bConvert==TURE, if file convertion upload failed, try non-convert upload   	
	if (bConvert)  
	{
		initiateResumableUploadRequest(curl, lpszResumableCreateMediaUrl, lpszFileName, lFileSize, TRUE, szUploadUri); 

		curl_easy_reset(curl);

		try
		{
			uploadFileContent(curl, lpszFileFullPathName, lFileSize, szUploadUri,  hMsg,  pResponseDoc, bIsBackground);
		}
		catch(...)
		{
			//reset the progressbar
			::PostMessage(hMsg, UWM_PROGRESS, (WPARAM)0, (LPARAM)0);   //send user-defined message to progress bar dialog

			curl = curl_easy_init();
			initiateResumableUploadRequest(curl, lpszResumableCreateMediaUrl, lpszFileName, lFileSize, FALSE, szUploadUri);
			curl_easy_reset(curl);
			uploadFileContent(curl, lpszFileFullPathName, lFileSize, szUploadUri,  hMsg,  pResponseDoc, bIsBackground);
		}
	}
	else
	{
			initiateResumableUploadRequest(curl, lpszResumableCreateMediaUrl, lpszFileName, lFileSize, FALSE, szUploadUri);
			curl_easy_reset(curl);
			uploadFileContent(curl, lpszFileFullPathName, lFileSize, szUploadUri,  hMsg,  pResponseDoc, bIsBackground);
	}

	if (curl)
		curl_easy_cleanup(curl);
}

/*******************************************************
update the file content with a new revision
Params:
	[in]lpszFileFullPathName: the name of the file, including the path. for example: L "e:\test\doc\aaa.txt"
	[in]lpszResumableEditMediaUrl:    <link rel="http://schemas.google.com/g/2005#resumable-edit-media" type="application/atom+xml" href="https://docs.google.com/feeds/upload/create-session/default/private/full/file%3A0B7CmbVXdOi7mM2MwZThiODctNDUyZS00Y2QyLWI0OWUtZTNkMDJhZjEyNjVk"/>
	[in]lpszGDETag: the GD:ETag of the original entry.  in case overwrite other's modification, 目前由于GoogleDocsAPI的bug,只能用NULL
	[in]hMsg:  the progressbar  message window,  it can be NULL if no progress message need,  
	[out] pResponseDoc: the TinyXml Document object for <entry> response.

Exception:
	runtime_error:
*********************************************************/
void GDocsAPI::UpdateFile(LPCWSTR lpszFileFullPathName, LPCSTR lpszResumableEditMediaUrl, LPCSTR lpszGDEtag, BOOL bCreateNewRevision, HWND hMsg, TiXmlDocument* pResponseDoc)
{
	assertUserLoggedIn();

	//arguments checking
	if (!lpszFileFullPathName || wcslen(lpszFileFullPathName) ==0 ) 
	{
		throw std::runtime_error("the file name is invalid!");
	}

	if (!lpszResumableEditMediaUrl)  // if you do not have "edit" right for the file, resumableEditMedia link is NULL!
	{
		throw std::runtime_error("you cannot edit the file.");
	}

	INT64 lFileSize = 	HelperFunctions::GetFileSize(lpszFileFullPathName);   	

	CURL* curl = curl_easy_init();
	if (!curl)
	{
		throw std::runtime_error("Connection initialization failed.");  	
	}

	WCHAR lpszFileName[MAX_PATH];
	HelperFunctions::GetFileNameFromFilePath(lpszFileFullPathName, lpszFileName, MAX_PATH); 

	CHAR szContentType[256]={0};  //like:  "Content-Type:application/pdf
	LPCSTR szMimeType =  HelperFunctions::GetMimeTypeFromFileName(lpszFileName); 
	strcpy_s(szContentType, "Content-Type: ");
	strcat_s(szContentType, szMimeType);

	curl_slist* headers = generateUploadRequestHeader(curl,  lpszGDEtag, NULL, szMimeType, lFileSize);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //  do not verify certificate
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);   //use PUT method

	if (bCreateNewRevision)
	{
		int nDim = strlen(lpszResumableEditMediaUrl) +32;
		char* pszPutUrl = new char[nDim]();
		strcpy_s(pszPutUrl, nDim, lpszResumableEditMediaUrl);
		strcat_s(pszPutUrl, nDim, "?new-revision=true");
		curl_easy_setopt(curl, CURLOPT_URL, pszPutUrl);
		delete[] pszPutUrl;
	}
	else
	{
		curl_easy_setopt(curl, CURLOPT_URL, lpszResumableEditMediaUrl);
	}
	
	//set the response write function to get the response HEADER
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, GDocsAPI::responseWriteCallback);   
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&responseBuffer);

	struct MemoryStruct  nullBuffer;
	nullBuffer.memory = NULL;
	nullBuffer.size =0;

	curl_easy_setopt(curl, CURLOPT_READFUNCTION,  GDocsAPI::readCallback);
	curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&nullBuffer); 
	 
	CURLcode res = curl_easy_perform(curl);
	if ( res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headers)
			curl_slist_free_all(headers); 
		
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error("Failed to update file, update request failed.");
	}

	//get response info..	Google returns 200 for sucessful file resumable uploda request. format: "200 OK  Location:<upload_uri>"
	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
	
	char szUploadUri[LEN_URI]={0};

	if (statusCode == 200 )
	{
		char* pszUri = strstr(responseBuffer.memory, "Location:");
		if (pszUri)
		{
			pszUri +=10;
			char * pszUriEnd = strstr(pszUri, "\r\n");
			if (pszUriEnd)
			{
				pszUriEnd[0] = 0;
			}

			strcpy_s(szUploadUri, LEN_URI, pszUri);

			if (responseBuffer.memory)
				free(responseBuffer.memory);
			
			if (headers)
				curl_slist_free_all(headers); 
		}
		else
		{
			if (responseBuffer.memory)
				free(responseBuffer.memory);

			if (headers)
				curl_slist_free_all(headers); 

			if (curl)
				curl_easy_cleanup(curl);

			throw std::runtime_error("File upload failed: no upload uri is generated.");
		}
	}
	else
	{
		char szErrMsg[512]={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);
		
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headers)
			curl_slist_free_all(headers); 

		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(szErrMsg);
	}


	//reset
	curl_easy_reset(curl);

//??try catch
	uploadFileContent(curl, lpszFileFullPathName, lFileSize, szUploadUri, hMsg, pResponseDoc, TRUE);

	if (curl)
		curl_easy_cleanup(curl);

}


/*****************************************************************************
Export Google Docs file to other format file
Params:
	[in] lpszContentUrl:  the url in the Entry->content->src:  like this https://docs.google.com/feeds/download/documents/Export?docId=0AbCmbVXdOi7mZGhianRiZHFfNDRmdHR0Mnp3cA
											or like this https://doc-10-1k-docs.googleusercontent.com/docs/secure/p62krtm66k35ibqhev55ra8osq858oka/5he22kkft8bpnulrh282rrj13e6mmmqc/1277532000000/16813505762710726417/16813505762710726417/0B7CmbVXdOi7mNWE0MzI3MmEtZjc5OC00MjYyLTg1ODAtMDJjMTIxNjUzNzU5?h=16653014193614665626&e=download&gd=true
	[in] lpszFileType: "document" "spreadsheet"  "presentation" "drawing" "pdf" "file"  ,   this is from the gd:resourceId.  GDocsEntry::GetType();	
	[in] lpszFilePath: the Path that the exported file is being saved to. like d:\abc\xxx.jpg
	[in] hMsg:  the Handle of the Progress Dialog
	[in] lpszFormat: the format of the exported file, 
		For "pdf" and "file" type, this param should be NULL. other value is just meaningless
		For "document" , it could be doc , html, odt, pdf, png, rtf, txt, zip  OR NULL(NULL=doc )
		For "spredsheet", it could be xls, csv, pdf, ods, tsv, html  OR NULL (NULL=xls)
		For "presentation", it could be pdf, png, ppt, swf, txt  OR NULL(NULL = ppt)
		For "drawing", it could be jpeg,pdf,png,svg or NULL(NULL=jpeg)

This method can be used both to Google Docs file or Arbitrary File

******************************************************************************/
void GDocsAPI::ExportFile(LPCSTR lpszContentUrl, LPCSTR lpszFileType, LPCWSTR lpszFilePath, HWND hMsg, LPCSTR lpszFormat)
{
	assertUserLoggedIn(); 
	
	if (lpszFilePath == NULL)
		throw std::runtime_error("File name is invalid");

	CURL* curl = curl_easy_init(); 
	if (!curl)
		throw std::runtime_error("Connection failed");  	

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);  //not verify certificate!

	//set Append Header 
	struct curl_slist * headers= NULL;
	headers = curl_slist_append(headers, "GData-Version:3.0");
	int iAuthLen = strlen("Authorization: Bearer ") +strlen(m_lpszAccessToken) +8;
	CHAR* szAuthToken = new CHAR[iAuthLen]();
	strcpy_s(szAuthToken, iAuthLen, "Authorization: Bearer ");
	strcat_s(szAuthToken, iAuthLen, m_lpszAccessToken);
	headers = curl_slist_append(headers, szAuthToken);
	delete [] szAuthToken;
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	//set CURLOPT_URL
	if (strcmp(lpszFileType, "pdf")==0 || strcmp(lpszFileType, "file")==0) // arbitrary file or pdf file
	{
		curl_easy_setopt(curl, CURLOPT_URL, lpszContentUrl);
	}
	else //google docs file : document,spreadsheet, presentaion type
	{
		int iDimension = strlen(lpszContentUrl) + 64;  
		char* lpszUrl = new char[iDimension]();
		strcpy_s(lpszUrl, iDimension, lpszContentUrl);
		strcat_s(lpszUrl, iDimension, "&exportFormat=");

		if (lpszFormat != NULL)
		{
			strcat_s(lpszUrl, iDimension,lpszFormat);
			if (strcmp(lpszFileType, "document") == 0)   //document is special for a tempoary technical limitation, use &exprotForm=html&format=html
			{
				strcat_s(lpszUrl, iDimension, "&format=");
				strcat_s(lpszUrl, iDimension, lpszFormat);
			}
		}
		else 
		{	
			if (strcmp(lpszFileType, "document") == 0)   //document, this is special, use &exprotForm=html&format=html
				strcat_s(lpszUrl, iDimension,"doc&format=doc");
			else if (strcmp(lpszFileType, "presentation") == 0)   //presentation 
				strcat_s(lpszUrl, iDimension,"ppt");
			else if (strcmp(lpszFileType, "spreadsheet") == 0)   //presentation 
				strcat_s(lpszUrl, iDimension,"xls");
			else if (strcmp(lpszFileType, "drawing") == 0) //drawing
				strcat_s(lpszUrl, iDimension,"jpeg");
			else
				strcat_s(lpszUrl, iDimension,"html");   //for default, this should never happen
		}

		curl_easy_setopt(curl, CURLOPT_URL, lpszUrl);

		delete [] lpszUrl; 
	}


	//Set write callback function
	HANDLE hFile = ::CreateFile(lpszFilePath, GENERIC_READ|GENERIC_WRITE, 
							FILE_SHARE_READ,	NULL, CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL, NULL);  
	if (hFile == INVALID_HANDLE_VALUE)
	{
		if (curl)
			curl_easy_cleanup(curl);
		if (headers)
			curl_slist_free_all(headers);

		throw std::runtime_error("create file failed");
	}

	//set the write function
	struct TransferInfoStruct downloadInfo;
	downloadInfo.hFile = hFile;
	downloadInfo.uBytesTransferred =0;
	downloadInfo.hDlg = hMsg;
	downloadInfo.lStartTimeStamp = HelperFunctions::GetCurrentTimeStamp();
	downloadInfo.bIsBackground = FALSE;
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::WriteToFileCallback);    
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&downloadInfo);

	CURLcode res= curl_easy_perform(curl);
	if (res!= CURLE_OK)
	{
		CloseHandle(hFile);
		if (curl)
			curl_easy_cleanup(curl);
		if (headers)
			curl_slist_free_all(headers);

		throw std::runtime_error("File Downloading/Exporting Failed.");  
	}

	CloseHandle(hFile);

	//Handle errors
	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

	if (curl)
		curl_easy_cleanup(curl);
	if (headers)
		curl_slist_free_all(headers);

	if (statusCode >= 400)  //error happened.  =
	{
		throw std::runtime_error("File downloading/converting failed.");
	}
}

/*******************************************************************************************
Create a subFolder 
Params:
	[in] lpszFolderName: the name of the new folder to be created. no path included
	[in] lpszParentFolderContentLink:  如： https://docs.google.com/feeds/default/private/full/folder%3A@2DFGFGDF/content"
	[out] pResponseDoc: the xml entry for the newly created folder

*****************************************************************************************/
void GDocsAPI::CreateFolder(LPSTR lpszFolderName, LPCSTR lpszParentFolderContentLink, TiXmlDocument* pResponseDoc)
{
	assertUserLoggedIn(); 

	if (!lpszFolderName || strlen(lpszFolderName)==0)
	{
		throw std::runtime_error("Invalid folder name.");
	}

	CURL* curl = curl_easy_init();

	struct curl_slist * headerList = prepareCurl(curl, "application/atom+xml", NULL);

	//construct url
	if (!lpszParentFolderContentLink)
	{	
		curl_easy_setopt(curl, CURLOPT_URL, "https://docs.google.com/feeds/default/private/full");
	}
	else
	{
		curl_easy_setopt(curl, CURLOPT_URL, lpszParentFolderContentLink);
	}

	//set the response write function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);   
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	//construct Post Body
	int iSize = strlen(lpszFolderName)+512;
	//if there is a "&" in the lpszFoldername, convert it to "&amp;"! otherwise createfolder will fail.
	string strFolderName = lpszFolderName;
	strFolderName = HelperFunctions::StringReplace(strFolderName, "&", "&amp;" );

	char* lpszPostBody = new char[iSize]();
	strcpy_s(lpszPostBody, iSize, "<?xml version='1.0' encoding='UTF-8'?>");
	strcat_s(lpszPostBody, iSize, "<entry xmlns=\"http://www.w3.org/2005/Atom\">");
	strcat_s(lpszPostBody, iSize,  "<category scheme=\"http://schemas.google.com/g/2005#kind\" term=\"http://schemas.google.com/docs/2007#folder\"/>");
	strcat_s(lpszPostBody, iSize, "<title>");
	strcat_s(lpszPostBody, iSize, strFolderName.c_str());
	strcat_s(lpszPostBody, iSize,   "</title></entry>");

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, lpszPostBody);

	CURLcode res = curl_easy_perform(curl);
	delete [] lpszPostBody;	
	
	if (res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error("Create Folder Failed.");
	}
	
	//Google returns either an HTTP 201 created response for successful folder creation.
	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
	
	if (statusCode == 201)  //succeed!
	{
		pResponseDoc->Clear();
		pResponseDoc->Parse(responseBuffer.memory, 0, TIXML_ENCODING_UTF8);
		
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);
	}
	else	//error!
	{
		char szErrMsg[512]={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);

		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(szErrMsg);
	}
}


/**********************************************************
If the given Folder is the parent of the specified GDocsEntry, return TRUE,  otherwise, return FALSE
root collection is the parent for any item (folder or file)， so for root folder, always return TRUE
params:
	pDocsEntry : the pointer of one GDocsEntry object ( a folder or a file)
	lpszFolderFeed:  the Folder Feed (ContentSrc) of the folder

return : TURE or FALSE
********************************************************/
BOOL GDocsAPI::isParentOfEntry(GDocsEntry* pDocsEntry, LPCWSTR lpszFolderFeed)
{
	//if  lpszFolderFeed is root  , return TRUE
	if ( lpszFolderFeed == NULL)  //root
		return TRUE;

	if ( (wcsstr(lpszFolderFeed,  L"https://docs.google.com/feeds/default/private/full/folder%3Aroot") != NULL ) 
			|| (wcscmp(lpszFolderFeed, L"https://docs.google.com/feeds/default/private/full") == 0) )  //root
		return TRUE;

	for (vector<LinkParent>::iterator iter = pDocsEntry->m_vecParents.begin(); iter != pDocsEntry->m_vecParents.end(); iter++)
	{
		//FolderFeed(ContentSrc)比linkParent多一个“/contents", so we use wcsstr to compare
		if ( wcsstr( lpszFolderFeed, (*iter).lpszParentHref) != NULL)  //find it
			return TRUE;
	}

	return FALSE;
}


/*************************************************************************************************
Move an entry( a file or a folder) into another folder
params:
	[in]lpszResourceId: the resourceid of the entry to be moved;  like: file:0B7CmbVXdOi7mYTFjNWZmZTAtYjU2OS00M2JhLWFmZGQtZGI2ODRkYzBjN2My
	[in]lpszSrcFolderFeed: the folder feed(contentsrc link) of the source folder. like: https://docs.google.com/feeds/default/private/full/folder%3A0B7CmbVXdOi7mYjhkZDExNmEtMzdlMC00YjY3LThhNTItODg4ZjM0MjE1MzNl/contents
		For "ROOT collection":  	"https://docs.google.com/feeds/default/private/full/folder%3Aroot/contents" ,  or "root", 
		NOTE: ROOTCOLLECTION_FEED ("https://docs.google.com/feeds/default/private/full/folder%3Aroot/contents/-/folder")  is not a right argument

	[in]lpszDesFolderFeed: the folder feed of the destination folder, like:https://docs.google.com/feeds/default/private/full/folder%3A0B7CmbVXdOi7mYjhkZDExNmEtMzdlMC00YjY3LThhNTItODg4ZjM0MjE1MzNl/contents
		For "ROOT collection": 	"https://docs.google.com/feeds/default/private/full/folder%3Aroot/contents" ,   or "root"
		NOTE: ROOTCOLLECTION_FEED ("https://docs.google.com/feeds/default/private/full/folder%3Aroot/contents/-/folder")  is not a right argument

	[in]isRemovedFromSrcFolder:  TRUE if the entry need to be removed from the source folder.  TRUE for default

To Move an entry, TWO steps:
1. add the entry into a new destination folder
2. delete the entry in the source folder.

*********************************************************************************************/
void GDocsAPI::MoveEntry(LPCSTR lpszResourceId, LPCSTR lpszSrcFolderFeed, LPCSTR lpszDestFolderFeed, BOOL isRemovedFromSrcFolder)
{
	assertUserLoggedIn(); 

	if (!lpszResourceId || strlen(lpszResourceId)==0)
		throw std::runtime_error("Invalid Resource Id.");
	if (!lpszSrcFolderFeed || strlen(lpszSrcFolderFeed) ==0)
		throw std::runtime_error("Invalid Source Folder Feed.");
	if (!lpszDestFolderFeed || strlen(lpszDestFolderFeed) ==0)
		throw std::runtime_error("Invalid Destination Folder Feed.");

	//convert to normalized FolderFeed fomart
	CHAR szSrcFolderFeed[512]={0};
	CHAR szDestFolderFeed[512]={0};

	if ( strcmp(lpszSrcFolderFeed, "root") == 0 ) 
		strcpy_s(szSrcFolderFeed, "https://docs.google.com/feeds/default/private/full/folder%3Aroot/contents");
	else
		strcpy_s(szSrcFolderFeed, lpszSrcFolderFeed);

	if ( strcmp(lpszDestFolderFeed, "root") == 0 )
		strcpy_s(szDestFolderFeed, "https://docs.google.com/feeds/default/private/full/folder%3Aroot/contents");
	else
		strcpy_s(szDestFolderFeed, lpszDestFolderFeed);


	CURL* curl = curl_easy_init();
	curl_slist * headerList = prepareCurl(curl, "application/atom+xml", NULL);
	curl_easy_setopt(curl, CURLOPT_URL, szDestFolderFeed);

	//set the response write function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);   
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	//construct Post Body
	int iSize = strlen(lpszResourceId)+256;
	char* lpszPostBody = new char[iSize]();
	strcpy_s(lpszPostBody, iSize, "<?xml version='1.0' encoding='UTF-8'?>");
	strcat_s(lpszPostBody, iSize, "<entry xmlns=\"http://www.w3.org/2005/Atom\">");
	strcat_s(lpszPostBody, iSize,  "<id>https://docs.google.com/feeds/default/private/full/");
	strcat_s(lpszPostBody, iSize, lpszResourceId);
	strcat_s(lpszPostBody, iSize,   "</id></entry>");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, lpszPostBody);

	CURLcode res = curl_easy_perform(curl);
	delete [] lpszPostBody;

	if (res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error("Move Operation Failed.");
	}

	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
	if (statusCode >=400)  //error happens!
	{
		char szErrMsg[512] ={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);
		
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(szErrMsg);
	}
	else  
	{
		//有一种情况： 虽然返回值是201， 但并没有添加到destination folder,比如 父目录向子目录 拖动的情况，就不能成功，但返回201,这个时候要检查responseBuffer 中的link parent 
		TiXmlDocument* pResponseDoc = new TiXmlDocument();
		pResponseDoc->Parse(responseBuffer.memory, 0, TIXML_ENCODING_UTF8);

		TiXmlElement* pEntryElement = pResponseDoc->FirstChildElement("entry");
		if (pEntryElement)
		{
			GDocsEntry*  pDocsEntry = new GDocsEntry(pEntryElement); 

			int nDim = strlen(szDestFolderFeed)+1;
			WCHAR* lpszDestFolderFeed = new WCHAR[nDim]();
			MultiByteToWideChar(CP_UTF8, 0, szDestFolderFeed, -1, lpszDestFolderFeed, nDim);

			if ( !isParentOfEntry( pDocsEntry, lpszDestFolderFeed ) )  //事实上没有成功！！，尽管返回状态是对的
			{
				if (responseBuffer.memory)
					free(responseBuffer.memory);
				if (headerList)
					curl_slist_free_all(headerList);
				if (curl)
					curl_easy_cleanup(curl);

				delete lpszDestFolderFeed;
				delete pDocsEntry;
				delete pResponseDoc,

				throw std::runtime_error("failed to add to destination folder");
			}

			delete lpszDestFolderFeed;
			delete pDocsEntry;
		}
		delete pResponseDoc;
	}

	if (responseBuffer.memory)
		free(responseBuffer.memory);
	responseBuffer.memory = NULL;
	responseBuffer.size =0;
	if (headerList)
		curl_slist_free_all(headerList);

	//delet the original link : move the entry out of the original folder
	//DELETE https://docs.google.com/feeds/default/private/full/folder%3Afolder_id/contents/document%3Adocument_id
	if (isRemovedFromSrcFolder)
	{
		curl_easy_reset(curl);
		struct curl_slist * headers = prepareCurl(curl, NULL, "*");
		
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
 
		//constuct url 
		int nSize = strlen(lpszResourceId)+strlen(szSrcFolderFeed)+32;
		char* lpszDelUrl = new char[nSize]();
		strcpy_s(lpszDelUrl, nSize, szSrcFolderFeed);
		strcat_s(lpszDelUrl, nSize, "/");
		strcat_s(lpszDelUrl, nSize, lpszResourceId); 

		curl_easy_setopt(curl, CURLOPT_URL, lpszDelUrl);
		delete[] lpszDelUrl;

		curl_easy_perform(curl);  //there is no error handling for the second step, because the first step has finished.

		if (headers)
			curl_slist_free_all(headers);
	}
	if (curl)
		curl_easy_cleanup(curl);

}

/*****************************************************************************************
Delete /Trash a file or folder
If isDeleted is TRUE, then delete the entry completely
if isDeleted is FALSE, trash it. 
the linkedit does not work for file in subfolders , so we use ResourceId to construct the URL
Param:
	[in] lpszResourceId: the resourceId of the entry to be renamed.  like : document:0ca3b233209090...
	[in] isDeleted:   TRUE for delete, FALSE for trash.        The default is FALSE
注意：该方法不能直接使用EditLink, 有问题，必须用拼接的方式来处理！！！
***************************************************************************/
void GDocsAPI::DeleteEntry(LPSTR lpszResourceId, BOOL isDeleted)
{
	assertUserLoggedIn();
	if (!lpszResourceId || strlen(lpszResourceId)==0)
		throw std::runtime_error("Invalid Entry Id.");
	
	CURL* curl = curl_easy_init();
	curl_slist * headerList = prepareCurl(curl, NULL, "*");

	//get  Type and Id from the ResouceId 
	char szType[128]={0};
	strcpy_s(szType, 128, lpszResourceId);
	char* szEnd = strchr(szType,':');
	if (szEnd)
		szEnd[0]=0;  //NULL terminated
	
	char* lpszEntryId = strchr(lpszResourceId, ':');
	//LPSTR lpszEntryId = NULL;
	if (lpszEntryId)
	{
		lpszEntryId ++;  
	}
	
	//construct URL
	int nDim = strlen(lpszEntryId) + 256;
	char* lpszUrl = new char[nDim]();
	strcpy_s(lpszUrl,nDim, "https://docs.google.com/feeds/default/private/full/");
	strcat_s(lpszUrl, nDim, szType);
	strcat_s(lpszUrl, nDim, "%3A");
	strcat_s(lpszUrl, nDim, lpszEntryId);

	if (isDeleted)
	{
		strcat_s(lpszUrl, nDim, "?delete=true");
	}
	curl_easy_setopt(curl, CURLOPT_URL, lpszUrl);
	delete [] lpszUrl;

	//set the response write function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);   
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);
	
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

	CURLcode res = curl_easy_perform(curl);
	if (res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error("Move Operation Failed.");
	}
	
	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
	if (statusCode >=400)  //error happens!
	{
		char szErrMsg[512] ={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);
		
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(szErrMsg);
	}

	if (responseBuffer.memory)
		free(responseBuffer.memory);
	if (headerList)
		curl_slist_free_all(headerList);
	if (curl)
		curl_easy_cleanup(curl);
}

/********************************************************************************************************
Rename the Entry
Params:
	[in] lpszEntryLink: the edit link (m_pszLinkEdit) of the entry to be renamed. like: https://docs.google.com/feeds/default/private/full/document%3A19QlFP5je6Suh7OwnuoyC2Kru5id6UIGMBeaFwbdYQo0
	[in] lpszNewName: the new Name

using "Updating a document's metadata" ,  call updateMetadata() 
********************************************************************************************************/
void GDocsAPI::RenameEntry(LPCSTR lpszEditLink, LPCSTR lpszNewName)
{
	int nDim = strlen(lpszNewName)+64;

	//if there is a "&" in the lpszFoldername, convert it to "&amp;"! otherwise createfolder will fail.
	string strNewName = lpszNewName;
	strNewName = HelperFunctions::StringReplace(strNewName, "&", "&amp;");
	
	char* szTitle = new char[nDim]();
	strcpy_s(szTitle, nDim, "<title>");
	strcat_s(szTitle, nDim, strNewName.c_str());
	strcat_s(szTitle, nDim, "</title>");

	try
	{
		updateMetadata(lpszEditLink, szTitle);
	}
	catch (std::runtime_error e)
	{
		delete [] szTitle;
		throw e;
	}
	catch (...)
	{
		delete [] szTitle;
		throw std::runtime_error("Rename failed.");
	}

	delete [] szTitle;
}
	

/*************************************************************
Star / Unstar  the Entry
Params:
	[in] lpszEditLink: the  Edit link of the entry (m_pszLinkEdit)  href="https://docs.google.com/feeds/default/private/full/document%3A19QlFP5je6Suh7OwnuoyC2Kru5id6UIGMBeaFwbdYQo0"
	[in] lstarOrNot:  TRUE: star the entry         FALSE: unstar the entry

using "Updating a document's metadata" ,  call updateMetadata() 

****************************************************************/
void GDocsAPI::StarEntry(LPCSTR lpszEditLink, BOOL starOrNot)
{
	char* szStarMetadata = NULL;
	if (starOrNot)
		szStarMetadata = "<category scheme=\"http://schemas.google.com/g/2005/labels\" term=\"http://schemas.google.com/g/2005/labels#starred\" label=\"starred\"/>";
	else
		szStarMetadata = "<category scheme=\"http://schemas.google.com/g/2005/labels\" term=\"http://schemas.google.com/g/2005/labels#starred\" label=\"\"/>";
	
	try
	{	
		updateMetadata(lpszEditLink,szStarMetadata );
	}
	catch (std::runtime_error e)
	{
		throw e;
	}
	catch (...)
	{
		throw std::runtime_error("Star/UnStar failed.");
	}
}


/*************************************************************************************
Restore item form trashed collection
Params:
	[in] lpszEditLink: the  Edit link of the entry (m_pszLinkEdit)  href="https://docs.google.com/feeds/default/private/full/document%3A19QlFP5je6Suh7OwnuoyC2Kru5id6UIGMBeaFwbdYQo0"

using "Updating a document's metadata" ,  call updateMetadata() 

************************************************************************************/
void GDocsAPI::RestoreEntry(LPCSTR lpszEditLink)
{
	try
	{	
		updateMetadata(lpszEditLink, "<category scheme=\"http://schemas.google.com/g/2005/labels\" term=\"http://schemas.google.com/g/2005/labels#trashed\" label=\"\"/>");
	}
	catch (std::runtime_error e)
	{
		throw e;
	}
	catch (...)
	{
		throw std::runtime_error("Restore failed.");
	}
}

/*****************************************************************************************
Retrieve the ACL (Access Control List) information of the specific entry
Params:
	[in] lpszEntryId: the id of the entry
	[in] lpszType:  file, folder, document, spreadsheet, presentation, pdf
	[out] pResonseDoc:  the returned XML response describing the ACL of the entry.  

From the pResponseDoc, we can get the owner, collaborators/writers, viewers/readers  of the entry. 

******************************************************************************************/
void GDocsAPI::GetEntryACL(LPCSTR lpszEntryId, LPCSTR lpszType, TiXmlDocument * pXmlDoc)
{
	assertUserLoggedIn(); 

	CURL* curl = curl_easy_init(); 
	if (!curl)
		throw std::runtime_error("Connection failed");  	

	curl_slist * headerList= prepareCurl(curl);

	//construct url
	int iDim = strlen(lpszEntryId)+256;
	char* lpszUrl = new char[iDim]();
	strcpy_s(lpszUrl, iDim, "https://docs.google.com/feeds/default/private/full/");
	strcat_s(lpszUrl, iDim, lpszType);
	strcat_s(lpszUrl, iDim, "%3A");
	strcat_s(lpszUrl, iDim, lpszEntryId);
	strcat_s(lpszUrl, iDim, "/acl");

	curl_easy_setopt(curl, CURLOPT_URL, lpszUrl);
	delete [] lpszUrl;

	//Set write callback function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);    
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	//perform the request
	CURLcode res= curl_easy_perform(curl);
	if (res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headerList)
			curl_slist_free_all(headerList);	//free hader slist
		
		if (curl)
			curl_easy_cleanup(curl);		//free curl handle 

		throw std::runtime_error("Get ACL informaiton failed.");
	}

	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

	if (statusCode <400)  //succeed
	{
		pXmlDoc->Clear();
		pXmlDoc->Parse(responseBuffer.memory, 0, TIXML_ENCODING_UTF8);  

		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headerList)
			curl_slist_free_all(headerList);	//free hader slist
		
		if (curl)
			curl_easy_cleanup(curl);		//free curl handle 
	}
	else  //error happend
	{
		char szErrMsg[512]={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);

		if (responseBuffer.memory)
			free(responseBuffer.memory);

		if (headerList)
			curl_slist_free_all(headerList);	//free hader slist
		
		if (curl)
			curl_easy_cleanup(curl);		//free curl handle 
	
		throw std::runtime_error(szErrMsg);
	}		
	
}

/***************************************************************************************
Share this entry(either a file or a folder) to a User( either a specific user, a google group, or  a google apps domain， or even publish to all users)
Params:
	[in] lpszEntryId: the Id of the entry
	[in] lpszType: the Type of the entry:  file, folder, document, spreadsheet, presentation, pdf
	[in] lpszRole:  writer  or reader
	[in] lpszScope:		user:    a user
					group:	a google group , a group email address
					domain:	a google app domain (ususally for business users), 这个是否能成功要取决于 google domain的设置。
					default:	the public ( To all internet users)  ,  The document will be crawlable by search engrines, and anyone will be able to view or edit the document.
	[in] lpszUser:   for user: a user's email
				for group:  a group's email
				for domain:  the domain name, like "driveoncloud.com" 
				for default/public:  NULL, this parameter is ignored.

	目前存在的问题是： email无法设置subject和content,不知道什么时候可以。
****************************************************************************************/
void GDocsAPI::AddACLUser(LPCSTR lpszEntryId, LPCSTR lpszType, LPCSTR lpszRole, LPCSTR lpszScope, LPCSTR lpszUser/*, LPCSTR lpszSubject, LPCSTR lpszMessage*/)
{
	assertUserLoggedIn(); 

	if (!lpszEntryId || strlen(lpszEntryId)==0)
	{
		throw std::runtime_error("Invalid Entry Id.");
	}

	CURL* curl = curl_easy_init();
	struct curl_slist * headerList = prepareCurl(curl, "application/atom+xml", NULL);

	//construct url
	int iDim = strlen(lpszEntryId)+256;
	char* lpszUrl = new char[iDim]();
	strcpy_s(lpszUrl, iDim, "https://docs.google.com/feeds/default/private/full/");
	strcat_s(lpszUrl, iDim, lpszType);
	strcat_s(lpszUrl, iDim, "%3A");
	strcat_s(lpszUrl, iDim, lpszEntryId);
	strcat_s(lpszUrl, iDim, "/acl");

	curl_easy_setopt(curl, CURLOPT_URL, lpszUrl);
	delete [] lpszUrl;

	//set the response write function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);   
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	//construct post body
	int iSize =0;
	if (lpszUser)
		iSize = strlen(lpszUser)+512;
	else
		iSize = 512;

	char* lpszPostBody = new char[iSize]();
	strcpy_s(lpszPostBody, iSize, "<entry xmlns=\"http://www.w3.org/2005/Atom\"  xmlns:gAcl='http://schemas.google.com/acl/2007'>");
	strcat_s(lpszPostBody, iSize,  "<category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/acl/2007#accessRule'/>");
	strcat_s(lpszPostBody, iSize, "<gAcl:role value='");
	strcat_s(lpszPostBody, iSize, lpszRole);
	strcat_s(lpszPostBody, iSize,   "'/>");
	strcat_s(lpszPostBody, iSize,   "<gAcl:scope type='");
	strcat_s(lpszPostBody, iSize,   lpszScope);
	if (strcmp(lpszScope, "default") ==0)  //public sharing
	{
		strcat_s(lpszPostBody, iSize,   "'/>");
	}
	else
	{
		strcat_s(lpszPostBody, iSize,   "'  value='");
		strcat_s(lpszPostBody, iSize,   lpszUser);
		strcat_s(lpszPostBody, iSize,   "'/>");
	}
	strcat_s(lpszPostBody, iSize,   "</entry>");

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, lpszPostBody);

	CURLcode res = curl_easy_perform(curl);
	delete [] lpszPostBody;	
	
	if (res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error("Sharing failed.");
	}
	//Google returns either an HTTP 201 created response for successful folder creation.
	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
	
	if (statusCode <400)  //succeed!
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);
	}
	else	//error!
	{
		char szErrMsg[512]={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);

		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(szErrMsg);
	}
}

/*******************************************************************************************
Update sharing persmission, use HTTP PUT, change Role to "reader" or "writer"
Params:
	[in] lpszACLEditLink: the Link-edit  of the ACL entry.  
			like: https://docs.google.com/feeds/default/private/full/file%3A0B7CmbVXdOi7mMjM1ZGEwODEtOGVhYy00MmU2LWFmZTEtOTkxM2EzMTFhYmQ4/acl/group%3Agdocsdrive%40googlegroups.com
	[in] lpszGDEtag: the gd:etag of the entry. used to identify the specific entry. like: W/"DkYNQns8eit7ImA9Wx9QFko."
	[in] lpszScopeType: the gAcl:scope-type,   the value could be one of :  "user", "group", "domain", "default".
	
	[in] lpszScopeValue: the gAcl:scopy-value.    
				for user: a user's email
				for group:  a group's email
				for domain:  the domain name, like "driveoncloud.com" 
				for default/public:  NULL, this parameter is ignored. 
	[in] lpszNewRole: "writer" or "reader"

****************************************************************************************/
void GDocsAPI::UpdateACLUser(LPCSTR lpszACLEditLink,  LPCSTR lpszGDEtag, LPCSTR lpszScopeType, LPCSTR lpszScopeValue, LPCSTR lpszNewRole)
{
	assertUserLoggedIn(); 

	if (!lpszACLEditLink || strlen(lpszACLEditLink)==0)
		throw std::runtime_error("Invalid Entry link.");
	

	CURL* curl = curl_easy_init();
	curl_slist * headerList = prepareCurl(curl, "application/atom+xml", "*");

	curl_easy_setopt(curl,CURLOPT_UPLOAD, 1L);   //use PUT method

	curl_easy_setopt(curl, CURLOPT_URL, lpszACLEditLink);


	//xml body, set read functon
	int nDim = strlen(lpszNewRole) +384;
	char* lpszBody =  new char[nDim]();
	strcpy_s(lpszBody,  nDim, "<entry xmlns=\"http://www.w3.org/2005/Atom\"  xmlns:gAcl='http://schemas.google.com/acl/2007'   xmlns:gd='http://schemas.google.com/g/2005'  gd:etag='");
	strcat_s(lpszBody,  nDim,	 lpszGDEtag);
	strcat_s(lpszBody,  nDim, "'>");
	strcat_s(lpszBody,  nDim, "<category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/acl/2007#accessRule'/>");
	strcat_s(lpszBody,  nDim, "<gAcl:role value='");
	strcat_s(lpszBody,  nDim, lpszNewRole);
	strcat_s(lpszBody,  nDim, "'/>");
	strcat_s(lpszBody,  nDim, "<gAcl:scope type='");
	strcat_s(lpszBody,  nDim, lpszScopeType);
	strcat_s(lpszBody,  nDim, "' ");
	
	if ( strcmp(lpszScopeType, "default") != 0)   //default(public)type do not have scope value!
	{
		strcat_s(lpszBody,  nDim, "  value='");
		strcat_s(lpszBody,  nDim, lpszScopeValue);
		strcat_s(lpszBody,  nDim, "'");
	}
	strcat_s(lpszBody,  nDim, "/>");

	strcat_s(lpszBody,  nDim, "</entry>");

	struct MemoryStruct readBuffer;
	readBuffer.memory = lpszBody;
	readBuffer.size = strlen(lpszBody);

	curl_easy_setopt(curl, CURLOPT_READFUNCTION, readCallback);
	curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&readBuffer);

	//set the response write function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);   
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	CURLcode res = curl_easy_perform(curl);
	delete [] lpszBody;

	if (res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(" Operation Failed, Probably due to connection problem.");
	}

	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
	if (statusCode >=400)  //error happens!
	{
		char szErrMsg[512] ={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);
		
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(szErrMsg);
	}

	if (responseBuffer.memory)
		free(responseBuffer.memory);
	if (headerList)
		curl_slist_free_all(headerList);
	if (curl)
		curl_easy_cleanup(curl);
}

/********************************************************************************
Remove permisssions for this user, isssue a HTTP DELETE request to the same edit link that 
Params:
	[in] lpszACLEditLink: the Link-edit  of the ACL entry.  
			like: https://docs.google.com/feeds/default/private/full/file%3A0B7CmbVXdOi7mMjM1ZGEwODEtOGVhYy00MmU2LWFmZTEtOTkxM2EzMTFhYmQ4/acl/group%3Agdocsdrive%40googlegroups.com

******************************************************************************/
void GDocsAPI::DeleteACLUser(LPCSTR lpszACLEditLink)
{
	assertUserLoggedIn();

	if (!lpszACLEditLink || strlen(lpszACLEditLink)==0)
		throw std::runtime_error("Invalid ACL Entry Link.");


	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	curl_slist * headerList = prepareCurl(curl, "application/atom+xml", NULL);


	curl_easy_setopt(curl, CURLOPT_URL, lpszACLEditLink);
	

	//set the response write function
	struct MemoryStruct  responseBuffer;
	responseBuffer.memory = NULL;
	responseBuffer.size = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GDocsAPI::responseWriteCallback);   
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseBuffer);

	CURLcode res = curl_easy_perform(curl);

	if (res!= CURLE_OK)
	{
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error("Delete sharing permission failed.");
	}

	LONG statusCode =0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
	if (statusCode >=400)  //error happens!
	{
		char szErrMsg[512] ={0};
		getErrMsgFromResponse(responseBuffer.memory, szErrMsg, 512);
		
		if (responseBuffer.memory)
			free(responseBuffer.memory);
		if (headerList)
			curl_slist_free_all(headerList);
		if (curl)
			curl_easy_cleanup(curl);

		throw std::runtime_error(szErrMsg);
	}

	if (responseBuffer.memory)
		free(responseBuffer.memory);
	responseBuffer.memory = NULL;
	responseBuffer.size =0;
	if (headerList)
		curl_slist_free_all(headerList);
}

