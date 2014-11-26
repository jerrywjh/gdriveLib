/****************************************************************
This is the wrapper class for an xml GDocs entry element.
Author: Jerry wang 
Last modified at:  12/15, 2011
All right reserved.

NOTE: the relationship between pEntryElement and this class Object:
This class is just a wrapper for an Tinyxml entry element. It is NOT responsible for the 
memory management(creation or destroy) of pEntryElement, 
which means when this oject is destroyed, the pEntryElement is still alive. 
 
The xml entry element(pEntryElement) need to manage its life by itself.

Copy constructor and Assignment operator are also implemented.

All the members are WIDECHAR format, to meet the stupid MFC UI requirement.
*************************************************************/ 

#include "stdafx.h"
#include "HelperFunctions.h"
#include "tinyxml.h"
#include "GDocsEntry.h"

//constructor
GDocsEntry::GDocsEntry(TiXmlElement * pElement)
	:m_pszGDEtag(NULL),
	m_pszIDUrl(NULL),
	m_pszPublished(NULL),
	m_pszUpdated(NULL),
	m_pszAppEdited(NULL),
	m_bIsStarred(FALSE),
	m_bIsHidden(FALSE),
	m_bIsViewed(FALSE),
	m_pszTitle(NULL),
	m_pszContentType(NULL),
	m_pszContentSrc(NULL),
	m_pszLinkAlternate(NULL),
	m_pszLinkSelf(NULL),
	m_pszLinkEdit(NULL),
	m_pszLinkResumableEditMedia(NULL),
	m_pszLinkThumbNail(NULL),
	m_pszAuthorName(NULL),
	m_pszAuthorEmail(NULL),
	m_pszResourceId(NULL),
	m_pszType(NULL),
	m_pszID(NULL),
	m_pszLastModifiedByName(NULL),
	m_pszLastModifiedByEmail(NULL),
	m_pszQuotaBytesUsed(NULL),
	m_bIsWritersCanInvite(TRUE),
	m_pszFeedLinkACL(NULL),
	m_pszFeedLinkRevisions(NULL),
	m_pszMD5Checksum(NULL)
{
	//Get the gd:etag attribute of the entry.  NULL if not exist.
	const char* szGDEtag = pElement->Attribute("gd:etag");
	int nDim = strlen(szGDEtag)+1;
	m_pszGDEtag = new WCHAR[nDim]();
	MultiByteToWideChar(CP_UTF8, 0, szGDEtag, -1, m_pszGDEtag, nDim);
	
	//Get the ID link of the entry  , in  <id> element
	//like :   https://docs.google.com/feeds/id/file%3A0B7CmbVXdOi7mY2RmOGNiMGEtZWNlNC00NzNiLThkY2UtOGM5N2M5ZWE2NTg5
	TiXmlElement * pIdElement = pElement->FirstChildElement("id");
	if (pIdElement)
	{
		const char* szIDUrl = pIdElement->GetText();
		if (szIDUrl)
		{
			nDim = strlen(szIDUrl)+1;
			m_pszIDUrl = new WCHAR[nDim]();
			MultiByteToWideChar(CP_UTF8, 0, szIDUrl, -1, m_pszIDUrl, nDim);
		}
	}
	
	//Get published time;   in <published> element
	//convert :  "2010-05-24T15:09:21.687Z"  (ISO8601)  to "2010-05-24 15:09:21"
	TiXmlElement* pPublishedElement = pElement->FirstChildElement("published");
	if (pPublishedElement)
	{
		const char* szPublished = pPublishedElement->GetText();
		if (szPublished)
		{
			m_pszPublished = new WCHAR[32](); 
			HelperFunctions::GetFormattedLocalTime(szPublished, m_pszPublished, 32);
		}
	}
	
	//Get updated time;   in <updated> element	
	//convert :  "2010-05-24T15:09:21.687Z"  (ISO8601)  to "2010-05-24 15:09:21"
	TiXmlElement* pUpdatedElement = pElement->FirstChildElement("updated");
	if (pUpdatedElement)
	{
		const char* szUpdated = pUpdatedElement->GetText();
		if (szUpdated)
		{
			m_pszUpdated = new WCHAR[32]();
			HelperFunctions::GetFormattedLocalTime(szUpdated, m_pszUpdated, 32);
		}
	}
	
	//Get Entry's App:edited Date and Time
	TiXmlElement* pAppEditedElement = pElement->FirstChildElement("app:edited");
	if (pAppEditedElement)
	{
		const char* szAppEdited = pAppEditedElement->GetText();
		if (szAppEdited)
		{
			m_pszAppEdited = new WCHAR[32]();
			HelperFunctions::GetFormattedLocalTime(szAppEdited, m_pszAppEdited, 32);
		}
	}
	
	//m_ bIsStarred: TURE for starred entry, FALSE for unstarred.
	for (TiXmlElement *pCategoryElement = pElement->FirstChildElement("category");  pCategoryElement;  pCategoryElement=pCategoryElement->NextSiblingElement("category"))
	{
		if (strcmp(pCategoryElement->Attribute("label"), "starred")==0)
		{
			m_bIsStarred = TRUE;
			break;
		}
	}	
	
	//m_bIsHidden
	for (TiXmlElement *pCategoryElement = pElement->FirstChildElement("category");  pCategoryElement;  pCategoryElement=pCategoryElement->NextSiblingElement("category"))
	{
		if (strcmp(pCategoryElement->Attribute("label"), "hidden")==0)
		{
			m_bIsHidden = TRUE;
			break;
		}
	}
	
	//m_bIsViewed
	for (TiXmlElement *pCategoryElement = pElement->FirstChildElement("category");  pCategoryElement;  pCategoryElement=pCategoryElement->NextSiblingElement("category"))
	{
		if (strcmp(pCategoryElement->Attribute("label"), "viewed")==0)
		{
			m_bIsViewed = TRUE;
			break;
		}
	}
	
	//m_pszTitle:  Get the file name.  NOTE: the title string is UTF-8 encoded.  it should be converted to Unicode format.
	TiXmlElement* pTitleElement = pElement->FirstChildElement("title");
	if (pTitleElement)
	{
		const char* szTitle = pTitleElement->GetText();
		if (szTitle)
		{
			nDim= strlen(szTitle)+1;
			m_pszTitle = new WCHAR[nDim]();
			MultiByteToWideChar(CP_UTF8, 0, szTitle,-1, m_pszTitle, nDim);
		}
	}
	
	//Get the Content type of the entry, like: "text/html" , "image/jpeg"....
	//this is used to download Non-GoogleDocs file (like pdf, exe, rar.....),usually, this method is used with  "GetContentSrc"
	TiXmlElement *pContentElement = pElement->FirstChildElement("content");
	if (pContentElement)
	{
		const char* szContentType = pContentElement->Attribute("type");
		if (szContentType)
		{
			nDim = strlen(szContentType)+1;
			m_pszContentType = new WCHAR[nDim]();
			MultiByteToWideChar(CP_UTF8, 0, szContentType, -1, m_pszContentType, nDim);
		}
		
		//Get the content src of the entry,    in <content src=" sdfsdf"> element ,This is usually used in downloading or exporting file
		const char* szContentSrc = pContentElement->Attribute("src");
		if (szContentSrc)
		{
			nDim = strlen(szContentSrc)+1;
			m_pszContentSrc = new WCHAR[nDim]();
			MultiByteToWideChar(CP_UTF8, 0, szContentSrc, -1, m_pszContentSrc, nDim);
		}
	}
	
	
	//Get the parent folder link. If this entry is located in Root,  return NULL, otherwise, return the link to parent 
	//This is especially useful in returning to upper level(parent folder)
	for (TiXmlElement *pLinkElement = pElement->FirstChildElement("link");  pLinkElement;  pLinkElement=pLinkElement->NextSiblingElement("link"))
	{
		if (strcmp(pLinkElement->Attribute("rel"), "http://schemas.google.com/docs/2007#parent")==0)
		{
			LinkParent linkParent;

			const char* szLinkParentHref = pLinkElement->Attribute("href");
			if (szLinkParentHref)
			{
				nDim = strlen(szLinkParentHref)+1;
				WCHAR* pszParentHref = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szLinkParentHref, -1, pszParentHref, nDim);
				linkParent.lpszParentHref = pszParentHref;
			}

			const char* szLinkParentTitle = pLinkElement->Attribute("title");
			if (szLinkParentTitle)
			{
				nDim = strlen(szLinkParentTitle)+1;
				WCHAR* pszLinkParentTitle = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szLinkParentTitle, -1, pszLinkParentTitle, nDim);
				linkParent.lpszParentTitle = pszLinkParentTitle;
			}
			
			m_vecParents.push_back(linkParent);
		}
	}
	
	//Get the alternate link url, Is alternate link has the same effect as self link???
	for (TiXmlElement *pLinkElement = pElement->FirstChildElement("link");  pLinkElement;  pLinkElement=pLinkElement->NextSiblingElement("link"))
	{
		if (strcmp(pLinkElement->Attribute("rel"), "alternate")==0)
		{
			const char* szLinkAlternate = pLinkElement->Attribute("href");
			if (szLinkAlternate)
			{
				nDim = strlen(szLinkAlternate)+1;
				m_pszLinkAlternate = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szLinkAlternate, -1, m_pszLinkAlternate, nDim);
			}
			break;
		}
	}

	//get the self link url
	for (TiXmlElement *pLinkElement = pElement->FirstChildElement("link");  pLinkElement;  pLinkElement=pLinkElement->NextSiblingElement("link"))
	{
		if (strcmp(pLinkElement->Attribute("rel"), "self")==0)
		{
			const char* szLinkSelf = pLinkElement->Attribute("href");
			if (szLinkSelf)
			{
				nDim = strlen(szLinkSelf)+1;
				m_pszLinkSelf = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szLinkSelf, -1, m_pszLinkSelf, nDim);
			}
			break;
		}
	}
	
	//Get the edit link of this entry.  <link rel="edit" ....
	for (TiXmlElement *pLinkElement = pElement->FirstChildElement("link");  pLinkElement;  pLinkElement=pLinkElement->NextSiblingElement("link"))
	{
		if (strcmp(pLinkElement->Attribute("rel"), "edit")==0)
		{
			const char* szLinkEdit = pLinkElement->Attribute("href");
			if (szLinkEdit)
			{
				nDim = strlen(szLinkEdit)+1;
				m_pszLinkEdit = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szLinkEdit, -1, m_pszLinkEdit, nDim);
			}
			break;
		}
	}

	//Get the Resumable-edit-media link
	for (TiXmlElement *pLinkElement = pElement->FirstChildElement("link");  pLinkElement;  pLinkElement=pLinkElement->NextSiblingElement("link"))
	{
		if (strcmp(pLinkElement->Attribute("rel"), "http://schemas.google.com/g/2005#resumable-edit-media")==0)
		{
			const char* szLinkResumableEditMediaHref = pLinkElement->Attribute("href");
			if (szLinkResumableEditMediaHref)
			{
				nDim = strlen(szLinkResumableEditMediaHref)+1;
				m_pszLinkResumableEditMedia = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szLinkResumableEditMediaHref, -1, m_pszLinkResumableEditMedia, nDim);
			}				
			break;
		}
	}

	//Get the ThumbNail
	for (TiXmlElement *pLinkElement = pElement->FirstChildElement("link");  pLinkElement;  pLinkElement=pLinkElement->NextSiblingElement("link"))
	{
		if (strcmp(pLinkElement->Attribute("rel"), "http://schemas.google.com/docs/2007/thumbnail")==0)
		{
			const char* szThumbNail = pLinkElement->Attribute("href");
			if (szThumbNail)
			{
				nDim = strlen(szThumbNail)+1;
				m_pszLinkThumbNail = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szThumbNail, -1, m_pszLinkThumbNail, nDim);
			}				
			break;
		}
	}	

	//Get the author name of the file/folder
	TiXmlElement * pAuthorElement = pElement->FirstChildElement("author");
	if (pAuthorElement)
	{
		TiXmlElement * pAuthorNameElement = pAuthorElement->FirstChildElement("name");
		if (pAuthorNameElement)
		{	
			const char* szAuthorName = pAuthorNameElement->GetText();
			if (szAuthorName)
			{
				nDim = strlen(szAuthorName)+1;
				m_pszAuthorName = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szAuthorName, -1, m_pszAuthorName, nDim);
			}
		}
		
		//Get the Author email of the file/folder
		TiXmlElement * pAuthorEmailElement = pAuthorElement->FirstChildElement("email");
		if (pAuthorEmailElement)
		{	
			const char* szAuthorEmail = pAuthorEmailElement->GetText();
			if (szAuthorEmail)
			{
				nDim = strlen(szAuthorEmail)+1;
				m_pszAuthorEmail = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szAuthorEmail,-1, m_pszAuthorEmail, nDim);
			}
		}
	}
	
	
	//Get ResourceId of the entry, like: "file:0B7CmbVXdOi7mMjZmMzZhOWYtOGM4MS00Mzk4LTg3ZGItMGU3MTdmMzc3Yjlj"
	//The resouceId includes both type and entryid.  
	//ResourceId is used in almost every request, so this is a very useful function
	TiXmlElement* pResourceIdElement = pElement->FirstChildElement("gd:resourceId");
	if (pResourceIdElement)
	{
		const char* szResourceId = pResourceIdElement->GetText();
		if (szResourceId)
		{
			nDim = strlen(szResourceId)+1;
			m_pszResourceId = new WCHAR[nDim]();
			MultiByteToWideChar(CP_UTF8,0, szResourceId, -1, m_pszResourceId, nDim);
		}
	}
	
	//Get the TYPE of the entry. like: folder, file, document, spreadsheet, presentation,drawing, PDF...
	//get Type from the REsoureId,
	if (m_pszResourceId)
	{
		const WCHAR* szSeperator =wcschr(m_pszResourceId, L':');
		int iDimension = szSeperator - m_pszResourceId+1;
		m_pszType = new WCHAR[iDimension]();
		wcsncpy_s(m_pszType,iDimension, m_pszResourceId, iDimension-1);
		m_pszType[iDimension-1]=L'\0';
	}
	
	//Get the Entry's ID
	if (m_pszResourceId)
	{
		WCHAR* szID= wcschr(m_pszResourceId, L':');
		m_pszID= ++szID;
	}
	
	//get the last writter's name
	TiXmlElement * pLastModifiedElement = pElement->FirstChildElement("gd:lastModifiedBy");
	if (pLastModifiedElement)
	{
		TiXmlElement * pNameElement = pLastModifiedElement->FirstChildElement("name");
		if (pNameElement)
		{	
			const char*  szLastModifiedByName = pNameElement->GetText();
			if (szLastModifiedByName)
			{
				nDim = strlen(szLastModifiedByName)+1;
				m_pszLastModifiedByName = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szLastModifiedByName, -1, m_pszLastModifiedByName, nDim);
			}
		}
		
		//get last writter's email
		TiXmlElement * pEmailElement = pLastModifiedElement->FirstChildElement("email");
		if (pEmailElement)
		{	
			const char* szLastModifiedByEmail = pEmailElement->GetText();
			if (szLastModifiedByEmail)
			{
				nDim = strlen(szLastModifiedByEmail)+1;
				m_pszLastModifiedByEmail = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szLastModifiedByEmail, -1, m_pszLastModifiedByEmail, nDim);
			}
		}
	}	
	
	//Get the size of the file. Folder:NULL. 	 for document, spreadsheet, presentation, drawing type: "0"
	if (wcscmp(m_pszType, L"folder") != 0)
	{
		TiXmlElement *pQuotaBytesUsed = pElement->FirstChildElement("gd:quotaBytesUsed");
		if (pQuotaBytesUsed)
		{
			const char* szQuotaBytesUsed = pQuotaBytesUsed->GetText();
			if (szQuotaBytesUsed)
			{
				nDim = strlen(szQuotaBytesUsed)+1;
				m_pszQuotaBytesUsed = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szQuotaBytesUsed, -1, m_pszQuotaBytesUsed, nDim);
			}
		}
	}

	//convert size/QuotaBytesUsed to 234,556,777 format
	if (wcscmp(m_pszType, L"folder") != 0)
	{
		int nFileSize = 0;
		if (m_pszQuotaBytesUsed)
		{
			nFileSize = _wtoi(m_pszQuotaBytesUsed);
		}
		HelperFunctions::NumberToBuffer(nFileSize, m_pszQuotaBytesUsedFormated, 3, L',');
	}
	else //folder ,do not display size
	{
		m_pszQuotaBytesUsedFormated[0] =0;
	}
	
	//IsWritersCaninvite: TRUE or FALSE
	TiXmlElement * pWritersCanInvite = pElement->FirstChildElement("docs:writersCanInvite");
	if (pWritersCanInvite)
	{
		if (strcmp(pWritersCanInvite->Attribute("value"), "true")==0)
			m_bIsWritersCanInvite = TRUE;
		else
			m_bIsWritersCanInvite = FALSE;
	}
	 
	
	//get the feedlink of ACL (Access Control List)
	for (TiXmlElement *pFeedLinkElement = pElement->FirstChildElement("gd:feedLink");  pFeedLinkElement;  pFeedLinkElement=pFeedLinkElement->NextSiblingElement("gd:feedLink"))
	{
		if (strstr(pFeedLinkElement->Attribute("rel"), "#accessControlList"))  
		{
			const char* szFeedLinkACL =  pFeedLinkElement->Attribute("href");
			if (szFeedLinkACL)
			{
				nDim = strlen(szFeedLinkACL)+1;
				m_pszFeedLinkACL = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szFeedLinkACL, -1, m_pszFeedLinkACL, nDim);
			}
		}
	}
	
	//get the feedlink of revisions .this is only meaningful for document, presentaion and spreadsheet,   for other types, NULL 
	for (TiXmlElement *pFeedLinkElement = pElement->FirstChildElement("gd:feedLink");  pFeedLinkElement;  pFeedLinkElement=pFeedLinkElement->NextSiblingElement("gd:feedLink"))
	{
		if (strstr(pFeedLinkElement->Attribute("rel"), "revisions"))  
		{
			const char* szFeedLinkRevisions = pFeedLinkElement->Attribute("href");
			if (szFeedLinkRevisions)
			{
				nDim = strlen(szFeedLinkRevisions)+1;
				m_pszFeedLinkRevisions = new WCHAR[nDim]();
				MultiByteToWideChar(CP_UTF8, 0, szFeedLinkRevisions, -1, m_pszFeedLinkRevisions, nDim);
			}
		}
	}

	//get MD5checksum , only meaningful for file.  NULL for other types
	TiXmlElement* pMD5ChecksumElement = pElement->FirstChildElement("docs:md5Checksum");
	if (pMD5ChecksumElement)
	{
		const char* szMD5Checksum = pMD5ChecksumElement->GetText();
		if (szMD5Checksum)
		{
			nDim= strlen(szMD5Checksum)+1;
			m_pszMD5Checksum = new WCHAR[nDim]();
			MultiByteToWideChar(CP_UTF8, 0, szMD5Checksum,-1, m_pszMD5Checksum, nDim);
		}
	}	
}

/************************************
destructor
NOTE: we do not manage the life of pEntryElement, 
so here we simply set m_pEntryElement to NULL and do not delete it
**********************************************************/
GDocsEntry::~GDocsEntry(void)
{
	if (m_pszGDEtag)
	{
		delete [] m_pszGDEtag;
		m_pszGDEtag = NULL;
	}	
	
	if (m_pszIDUrl)
	{
		delete [] m_pszIDUrl;
		m_pszIDUrl = NULL;
	}
	
	if (m_pszPublished)
	{
		delete[] m_pszPublished;
		m_pszPublished = NULL;
	}
	
	if (m_pszUpdated)
	{
		delete [] m_pszUpdated;
		m_pszUpdated = NULL;
	}
	
	if (m_pszAppEdited)
	{
		delete [] m_pszAppEdited;
		m_pszAppEdited = NULL;
	}
	
	if (m_pszTitle)
	{
		delete [] m_pszTitle;
		m_pszTitle = NULL;
	}
	
	if (m_pszContentType)
	{
		delete [] m_pszContentType;
		m_pszContentType = NULL;
	}
	
	if (m_pszContentSrc)
	{
		delete [] m_pszContentSrc;
		m_pszContentSrc = NULL;
	}

	//delete all parents
	for (vector<LinkParent>::iterator iter = m_vecParents.begin(); iter != m_vecParents.end(); iter++)
	{
		delete (*iter).lpszParentHref;
		delete (*iter).lpszParentTitle;
	}
	m_vecParents.clear();

	
	if( m_pszLinkAlternate)
	{
		delete [] m_pszLinkAlternate;
		m_pszLinkAlternate = NULL;
	}
	
	if (m_pszLinkSelf)
	{
		delete [] m_pszLinkSelf;
		m_pszLinkSelf = NULL;
	}
	
	if (m_pszLinkEdit)
	{
		delete [] m_pszLinkEdit;
		m_pszLinkEdit = NULL;
	}
	
	if (m_pszLinkResumableEditMedia)
	{
		delete [] m_pszLinkResumableEditMedia;
		m_pszLinkResumableEditMedia = NULL;
	}

	if (m_pszLinkThumbNail)
	{
		delete [] m_pszLinkThumbNail;
		m_pszLinkThumbNail = NULL;
	}
	
	if (m_pszAuthorName)
	{
		delete [] m_pszAuthorName;
		m_pszAuthorName = NULL;
	}
	
	if (m_pszAuthorEmail)
	{
		delete [] m_pszAuthorEmail;
		m_pszAuthorEmail = NULL;
	}
	
	if (m_pszResourceId)
	{
		delete [] m_pszResourceId;
		m_pszResourceId = NULL;
	}

	if (m_pszType)
	{
		delete [] m_pszType;
		m_pszType = NULL;
	}
	
	//m_pszId is not need to be freed
	
	if (m_pszLastModifiedByName)
	{
		delete [] m_pszLastModifiedByName;
		m_pszLastModifiedByName = NULL;
	}
	
	if (m_pszLastModifiedByEmail)
	{
		delete [] m_pszLastModifiedByEmail;
		m_pszLastModifiedByEmail = NULL;
	}
	
	if (m_pszQuotaBytesUsed)
	{
		delete [] m_pszQuotaBytesUsed;
		m_pszQuotaBytesUsed = NULL;
	}
	
	if (m_pszFeedLinkACL)
	{
		delete [] m_pszFeedLinkACL;
		m_pszFeedLinkACL = NULL;
	}
	
	if (m_pszFeedLinkRevisions)
	{
		delete [] m_pszFeedLinkRevisions;
		m_pszFeedLinkRevisions = NULL;
	}

	if (m_pszMD5Checksum)
	{
		delete [] m_pszMD5Checksum;
		m_pszMD5Checksum = NULL;
	}
}



/************************************************
Copy constructor
Sometime we need to copy this object(not transfter the pointer) to avoid it is released in another place,
like in Copy, Cut &Paste opeartions

for pointer members, the copy constructor allocates a new element 
and copies the vaue form the object is it copying
**********************************************/
GDocsEntry::GDocsEntry(const GDocsEntry& entry)
{
	//gd:etag
	if (entry.m_pszGDEtag)
	{
		int nDim = wcslen(entry.m_pszGDEtag) +1;
		m_pszGDEtag = new WCHAR[nDim]();
		wcscpy_s(m_pszGDEtag, nDim, entry.m_pszGDEtag);
	}
	else
	{
		m_pszGDEtag = NULL;
	}

	//m_pszIDUrl
	if (entry.m_pszIDUrl)
	{
		int nDim = wcslen(entry.m_pszIDUrl)+1;
		m_pszIDUrl = new WCHAR[nDim]();
		wcscpy_s(m_pszIDUrl, nDim, entry.m_pszIDUrl);
	}
	else
	{
		m_pszIDUrl = NULL;
	}

	//m_pszPublished
	if (entry.m_pszPublished)
	{
		int nDim = wcslen(entry.m_pszPublished) +1;
		m_pszPublished = new WCHAR[nDim]();
		wcscpy_s(m_pszPublished, nDim, entry.m_pszPublished);
	}
	else
	{
		m_pszPublished = NULL;
	}

	//m_pszUpdated
	if (entry.m_pszUpdated)
	{
		int nDim = wcslen(entry.m_pszUpdated) +1;
		m_pszUpdated = new WCHAR[nDim]();
		wcscpy_s(m_pszUpdated, nDim, entry.m_pszUpdated);
	}
	else
	{
		m_pszUpdated = NULL;
	}

	//m_pszAppEdited;
	if (entry.m_pszAppEdited)
	{
		int nDim = wcslen(entry.m_pszAppEdited) +1;
		m_pszAppEdited = new WCHAR[nDim]();
		wcscpy_s(m_pszAppEdited, nDim, entry.m_pszAppEdited);
	}
	else
	{
		m_pszAppEdited = NULL;
	}

	m_bIsStarred = entry.m_bIsStarred;
	m_bIsHidden = entry.m_bIsHidden;
	m_bIsViewed = entry.m_bIsViewed;

	//m_pszTitle
	if (entry.m_pszTitle)
	{
		int nDim = wcslen(entry.m_pszTitle) +1;
		m_pszTitle = new WCHAR[nDim]();
		wcscpy_s(m_pszTitle, nDim, entry.m_pszTitle);
	}
	else
	{
		m_pszTitle = NULL;
	}

	//m_pszContentType
	if (entry.m_pszContentType)
	{
		int nDim = wcslen(entry.m_pszContentType) +1;
		m_pszContentType = new WCHAR[nDim]();
		wcscpy_s(m_pszContentType, nDim, entry.m_pszContentType);
	}

	//m_pszContentSrc
	if (entry.m_pszContentSrc)
	{
		int nDim = wcslen(entry.m_pszContentSrc) +1;
		m_pszContentSrc = new WCHAR[nDim]();
		wcscpy_s(m_pszContentSrc, nDim, entry.m_pszContentSrc);
	}
	else
	{
		m_pszContentSrc = NULL;
	}

	//m_vecParents (vector)
	for (vector<LinkParent>::const_iterator  iter = entry.m_vecParents.begin(); iter != entry.m_vecParents.end(); iter++)
	{
		LinkParent linkParent;
		if ( iter->lpszParentHref )
		{
			int nDim = wcslen(iter->lpszParentHref) +1;
			linkParent.lpszParentHref = new WCHAR[nDim]();
			wcscpy_s(linkParent.lpszParentHref, nDim, iter->lpszParentHref);
		}
		else
		{
			linkParent.lpszParentHref = NULL;
		}

		if (iter->lpszParentTitle )
		{
			int nDim = wcslen(iter->lpszParentTitle)+1;
			linkParent.lpszParentTitle = new WCHAR[nDim]();
			wcscpy_s(linkParent.lpszParentTitle, nDim, iter->lpszParentTitle);
		}
		else
		{
			linkParent.lpszParentTitle = NULL;
		}

		m_vecParents.push_back(linkParent);
	}

	//m_pszLinkAlternate
	if (entry.m_pszLinkAlternate)
	{
		int nDim = wcslen(entry.m_pszLinkAlternate) +1;
		m_pszLinkAlternate = new WCHAR[nDim]();
		wcscpy_s(m_pszLinkAlternate, nDim, entry.m_pszLinkAlternate);
	}
	else
	{
		m_pszLinkAlternate = NULL;
	}

	//m_pszLinkSelf
	if (entry.m_pszLinkSelf)
	{
		int nDim = wcslen(entry.m_pszLinkSelf) +1;
		m_pszLinkSelf = new WCHAR[nDim]();
		wcscpy_s(m_pszLinkSelf, nDim, entry.m_pszLinkSelf);
	}
	else
	{
		m_pszLinkSelf = NULL;
	}

	//m_pszLinkEdit
	if (entry.m_pszLinkEdit)
	{
		int nDim = wcslen(entry.m_pszLinkEdit) +1;
		m_pszLinkEdit = new WCHAR[nDim]();
		wcscpy_s(m_pszLinkEdit, nDim, entry.m_pszLinkEdit);
	}
	else
	{
		m_pszLinkEdit = NULL;
	}

	//m_pszLinkResumableEditMedia
	if (entry.m_pszLinkResumableEditMedia)
	{
		int nDim = wcslen(entry.m_pszLinkResumableEditMedia) +1;
		m_pszLinkResumableEditMedia = new WCHAR[nDim]();
		wcscpy_s(m_pszLinkResumableEditMedia, nDim, entry.m_pszLinkResumableEditMedia);
	}
	else
	{
		m_pszLinkResumableEditMedia = NULL;
	}

	//m_pszLinkThumbNail
	if (entry.m_pszLinkThumbNail)
	{
		int nDim = wcslen(entry.m_pszLinkThumbNail) +1;
		m_pszLinkThumbNail = new WCHAR[nDim]();
		wcscpy_s(m_pszLinkThumbNail, nDim, entry.m_pszLinkThumbNail);
	}
	else
	{
		m_pszLinkThumbNail = NULL;
	}

	//m_pszAuthorName
	if (entry.m_pszAuthorName)
	{
		int nDim = wcslen(entry.m_pszAuthorName) +1;
		m_pszAuthorName = new WCHAR[nDim]();
		wcscpy_s(m_pszAuthorName, nDim, entry.m_pszAuthorName);
	}
	else
	{
		m_pszAuthorName = NULL;
	}

	//m_pszAuthorEmail
	if (entry.m_pszAuthorEmail)
	{
		int nDim = wcslen(entry.m_pszAuthorEmail) +1;
		m_pszAuthorEmail = new WCHAR[nDim]();
		wcscpy_s(m_pszAuthorEmail, nDim, entry.m_pszAuthorEmail);
	}
	else
	{
		m_pszAuthorEmail = NULL;
	}

	//m_pszResourceId
	if (entry.m_pszResourceId)
	{
		int nDim = wcslen(entry.m_pszResourceId) +1;
		m_pszResourceId = new WCHAR[nDim]();
		wcscpy_s(m_pszResourceId, nDim, entry.m_pszResourceId);
	}
	else
	{
		m_pszResourceId = NULL;
	}

	//m_pszType
	if (entry.m_pszType)
	{
		int nDim = wcslen(entry.m_pszType) +1;
		m_pszType = new WCHAR[nDim]();
		wcscpy_s(m_pszType, nDim, entry.m_pszType);
	}
	else
	{
		m_pszType = NULL;
	}

	//m_pszID ,  it is associate with m_pszResourceID,!
	if (m_pszResourceId)
	{
		WCHAR* szID= wcschr(m_pszResourceId, L':');
		m_pszID= ++szID;
	}

	//m_pszLastModifiedByName
	if (entry.m_pszLastModifiedByName)
	{
		int nDim = wcslen(entry.m_pszLastModifiedByName) +1;
		m_pszLastModifiedByName = new WCHAR[nDim]();
		wcscpy_s(m_pszLastModifiedByName, nDim, entry.m_pszLastModifiedByName);
	}
	else
	{
		m_pszLastModifiedByName = NULL;
	}

	//m_pszLastModifiedByEmail
	if (entry.m_pszLastModifiedByEmail)
	{
		int nDim = wcslen(entry.m_pszLastModifiedByEmail) +1;
		m_pszLastModifiedByEmail = new WCHAR[nDim]();
		wcscpy_s(m_pszLastModifiedByEmail, nDim, entry.m_pszLastModifiedByEmail);
	}
	else
	{
		m_pszLastModifiedByEmail = NULL;
	}

	//m_pszQuotaBytesUsed
	if (entry.m_pszQuotaBytesUsed)
	{
		int nDim = wcslen(entry.m_pszQuotaBytesUsed) +1;
		m_pszQuotaBytesUsed = new WCHAR[nDim]();
		wcscpy_s(m_pszQuotaBytesUsed, nDim, entry.m_pszQuotaBytesUsed);
	}
	else
	{
		m_pszQuotaBytesUsed = NULL;
	}

	//m_pszQuotaBytesUsedFormated
	wcscpy_s(m_pszQuotaBytesUsedFormated, 32, entry.m_pszQuotaBytesUsedFormated);

	//m_bIsWritersCanInvite
	m_bIsWritersCanInvite = entry.m_bIsWritersCanInvite;

	//m_pszFeedLinkACL
	if (entry.m_pszFeedLinkACL)
	{
		int nDim = wcslen(entry.m_pszFeedLinkACL) +1;
		m_pszFeedLinkACL = new WCHAR[nDim]();
		wcscpy_s(m_pszFeedLinkACL, nDim, entry.m_pszFeedLinkACL);
	}
	else
	{
		m_pszFeedLinkACL = NULL;
	}

	//m_pszFeedLinkRevisions
	if (entry.m_pszFeedLinkRevisions)
	{
		int nDim = wcslen(entry.m_pszFeedLinkRevisions) +1;
		m_pszFeedLinkRevisions = new WCHAR[nDim]();
		wcscpy_s(m_pszFeedLinkRevisions, nDim, entry.m_pszFeedLinkRevisions);
	}
	else
	{
		m_pszFeedLinkRevisions = NULL;
	}

	//m_pszMD5Checksum
	if (entry.m_pszMD5Checksum)
	{
		int nDim = wcslen(entry.m_pszMD5Checksum) +1;
		m_pszMD5Checksum = new WCHAR[nDim]();
		wcscpy_s(m_pszMD5Checksum, nDim, entry.m_pszMD5Checksum);
	}
	else
	{
		m_pszMD5Checksum = NULL;
	}
}


/*************************************************
assignment operator

destroys the existing object it holods and 
copies the value from its right-hand operand into its left-hand operand
***************************************************/
GDocsEntry& GDocsEntry::operator= (const GDocsEntry& rhs)
{
	if (&rhs != this)
	{
		//m_pszGDEtag
		delete[] m_pszGDEtag;

		if (rhs.m_pszGDEtag)
		{
			int nDim = wcslen(rhs.m_pszGDEtag)+1;
			m_pszGDEtag = new WCHAR[nDim]();
			wcscpy_s(m_pszGDEtag, nDim, rhs.m_pszGDEtag);
		}
		else
		{
			m_pszGDEtag = NULL;
		}

		//m_pszIDUrl
		delete[] m_pszIDUrl;

		if (rhs.m_pszIDUrl)
		{
			int nDim = wcslen(rhs.m_pszIDUrl)+1;
			m_pszIDUrl = new WCHAR[nDim]();
			wcscpy_s(m_pszIDUrl, nDim, rhs.m_pszIDUrl);
		}
		else
		{
			m_pszIDUrl = NULL;
		}

		//m_pszPublished
		delete[] m_pszPublished;

		if (rhs.m_pszPublished)
		{
			int nDim = wcslen(rhs.m_pszPublished)+1;
			m_pszPublished = new WCHAR[nDim]();
			wcscpy_s(m_pszPublished, nDim, rhs.m_pszPublished);
		}
		else
		{
			m_pszPublished = NULL;
		}

		//m_pszUpdated
		delete[] m_pszUpdated;

		if (rhs.m_pszUpdated)
		{
			int nDim = wcslen(rhs.m_pszUpdated)+1;
			m_pszUpdated = new WCHAR[nDim]();
			wcscpy_s(m_pszUpdated, nDim, rhs.m_pszUpdated);
		}
		else
		{
			m_pszUpdated = NULL;
		}

		//m_pszAppEdited
		delete[] m_pszAppEdited;

		if (rhs.m_pszAppEdited)
		{
			int nDim = wcslen(rhs.m_pszAppEdited)+1;
			m_pszAppEdited = new WCHAR[nDim]();
			wcscpy_s(m_pszAppEdited, nDim, rhs.m_pszAppEdited);
		}
		else
		{
			m_pszAppEdited = NULL;
		}

		// BOOL
		m_bIsStarred = rhs.m_bIsStarred;
		m_bIsHidden = rhs.m_bIsHidden;
		m_bIsViewed = rhs.m_bIsViewed;

		//m_pszTitle
		delete[] m_pszTitle;

		if (rhs.m_pszTitle)
		{
			int nDim = wcslen(rhs.m_pszTitle)+1;
			m_pszTitle = new WCHAR[nDim]();
			wcscpy_s(m_pszTitle, nDim, rhs.m_pszTitle);
		}
		else
		{
			m_pszTitle = NULL;
		}

		//m_pszContentType
		delete[] m_pszContentType;

		if (rhs.m_pszContentType)
		{
			int nDim = wcslen(rhs.m_pszContentType)+1;
			m_pszContentType = new WCHAR[nDim]();
			wcscpy_s(m_pszContentType, nDim, rhs.m_pszContentType);
		}
		else
		{
			m_pszContentType = NULL;
		}

		//m_pszContentSrc
		delete[] m_pszContentSrc;

		if (rhs.m_pszContentSrc)
		{
			int nDim = wcslen(rhs.m_pszContentSrc)+1;
			m_pszContentSrc = new WCHAR[nDim]();
			wcscpy_s(m_pszContentSrc, nDim, rhs.m_pszContentSrc);
		}
		else
		{
			m_pszContentSrc = NULL;
		}

		//m_vecParents
		for (vector<LinkParent>::const_iterator iter = m_vecParents.begin(); iter != m_vecParents.end(); iter++)
		{
			delete (*iter).lpszParentHref;
			delete (*iter).lpszParentTitle;
		}
		m_vecParents.clear();

		for (vector<LinkParent>::const_iterator  iter = rhs.m_vecParents.begin(); iter != rhs.m_vecParents.end(); iter++)
		{
			LinkParent linkParent;
			if ( iter->lpszParentHref )
			{
				int nDim = wcslen(iter->lpszParentHref) +1;
				linkParent.lpszParentHref = new WCHAR[nDim]();
				wcscpy_s(linkParent.lpszParentHref, nDim, iter->lpszParentHref);
			}
			else
			{
				linkParent.lpszParentHref = NULL;
			}

			if (iter->lpszParentTitle )
			{
				int nDim = wcslen(iter->lpszParentTitle)+1;
				linkParent.lpszParentTitle = new WCHAR[nDim]();
				wcscpy_s(linkParent.lpszParentTitle, nDim, iter->lpszParentTitle);
			}
			else
			{
				linkParent.lpszParentTitle = NULL;
			}

			m_vecParents.push_back(linkParent);
		}

		//m_pszLinkAlternate
		delete[] m_pszLinkAlternate;

		if (rhs.m_pszLinkAlternate)
		{
			int nDim = wcslen(rhs.m_pszLinkAlternate)+1;
			m_pszLinkAlternate = new WCHAR[nDim]();
			wcscpy_s(m_pszLinkAlternate, nDim, rhs.m_pszLinkAlternate);
		}
		else
		{
			m_pszLinkAlternate = NULL;
		}

		//m_pszLinkSelf
		delete[] m_pszLinkSelf;

		if (rhs.m_pszLinkSelf)
		{
			int nDim = wcslen(rhs.m_pszLinkSelf)+1;
			m_pszLinkSelf = new WCHAR[nDim]();
			wcscpy_s(m_pszLinkSelf, nDim, rhs.m_pszLinkSelf);
		}
		else
		{
			m_pszLinkSelf = NULL;
		}

		//m_pszLinkEdit
		delete[] m_pszLinkEdit;

		if (rhs.m_pszLinkEdit)
		{
			int nDim = wcslen(rhs.m_pszLinkEdit)+1;
			m_pszLinkEdit = new WCHAR[nDim]();
			wcscpy_s(m_pszLinkEdit, nDim, rhs.m_pszLinkEdit);
		}
		else
		{
			m_pszLinkEdit = NULL;
		}

		//m_pszLinkResumableEditMedia
		delete[] m_pszLinkResumableEditMedia;

		if (rhs.m_pszLinkResumableEditMedia)
		{
			int nDim = wcslen(rhs.m_pszLinkResumableEditMedia)+1;
			m_pszLinkResumableEditMedia = new WCHAR[nDim]();
			wcscpy_s(m_pszLinkResumableEditMedia, nDim, rhs.m_pszLinkResumableEditMedia);
		}
		else
		{
			m_pszLinkResumableEditMedia = NULL;
		}

		//m_pszLinkThumbNail
		delete[] m_pszLinkThumbNail;

		if (rhs.m_pszLinkThumbNail)
		{
			int nDim = wcslen(rhs.m_pszLinkThumbNail)+1;
			m_pszLinkThumbNail = new WCHAR[nDim]();
			wcscpy_s(m_pszLinkThumbNail, nDim, rhs.m_pszLinkThumbNail);
		}
		else
		{
			m_pszLinkThumbNail = NULL;
		}

		//m_pszAuthorName
		delete[] m_pszAuthorName;

		if (rhs.m_pszAuthorName)
		{
			int nDim = wcslen(rhs.m_pszAuthorName)+1;
			m_pszAuthorName = new WCHAR[nDim]();
			wcscpy_s(m_pszAuthorName, nDim, rhs.m_pszAuthorName);
		}
		else
		{
			m_pszAuthorName = NULL;
		}

		//m_pszAuthorEmail
		delete[] m_pszAuthorEmail;

		if (rhs.m_pszAuthorEmail)
		{
			int nDim = wcslen(rhs.m_pszAuthorEmail)+1;
			m_pszAuthorEmail = new WCHAR[nDim]();
			wcscpy_s(m_pszAuthorEmail, nDim, rhs.m_pszAuthorEmail);
		}
		else
		{
			m_pszAuthorEmail = NULL;
		}

		//m_pszResourceId
		delete[] m_pszResourceId;

		if (rhs.m_pszResourceId)
		{
			int nDim = wcslen(rhs.m_pszResourceId)+1;
			m_pszResourceId = new WCHAR[nDim]();
			wcscpy_s(m_pszResourceId, nDim, rhs.m_pszResourceId);
		}
		else
		{
			m_pszResourceId = NULL;
		}

		//m_pszType
		delete[] m_pszType;

		if (rhs.m_pszType)
		{
			int nDim = wcslen(rhs.m_pszType)+1;
			m_pszType = new WCHAR[nDim]();
			wcscpy_s(m_pszType, nDim, rhs.m_pszType);
		}
		else
		{
			m_pszType = NULL;
		}

		//m_pszID,  cannot delete it and copy it !! get it from m_pszResourceId.
		if (m_pszResourceId)
		{
			WCHAR* szID= wcschr(m_pszResourceId, L':');
			m_pszID= ++szID;
		}

		//m_pszLastModifiedByName
		delete[] m_pszLastModifiedByName;

		if (rhs.m_pszLastModifiedByName)
		{
			int nDim = wcslen(rhs.m_pszLastModifiedByName)+1;
			m_pszLastModifiedByName = new WCHAR[nDim]();
			wcscpy_s(m_pszLastModifiedByName, nDim, rhs.m_pszLastModifiedByName);
		}
		else
		{
			m_pszLastModifiedByName = NULL;
		}

		//m_pszLastModifiedByEmail
		delete[] m_pszLastModifiedByEmail;

		if (rhs.m_pszLastModifiedByEmail)
		{
			int nDim = wcslen(rhs.m_pszLastModifiedByEmail)+1;
			m_pszLastModifiedByEmail = new WCHAR[nDim]();
			wcscpy_s(m_pszLastModifiedByEmail, nDim, rhs.m_pszLastModifiedByEmail);
		}
		else
		{
			m_pszLastModifiedByEmail = NULL;
		}

		//m_pszQuotaBytesUsed
		delete[] m_pszQuotaBytesUsed;

		if (rhs.m_pszQuotaBytesUsed)
		{
			int nDim = wcslen(rhs.m_pszQuotaBytesUsed)+1;
			m_pszQuotaBytesUsed = new WCHAR[nDim]();
			wcscpy_s(m_pszQuotaBytesUsed, nDim, rhs.m_pszQuotaBytesUsed);
		}
		else
		{
			m_pszQuotaBytesUsed = NULL;
		}

		//m_pszQuotaBytesUsedFormated
		wcscpy_s(m_pszQuotaBytesUsedFormated, 32, rhs.m_pszQuotaBytesUsedFormated);

		//m_bIsWritersCanInvite
		m_bIsWritersCanInvite = rhs.m_bIsWritersCanInvite;

		//m_pszFeedLinkACL
		delete[] m_pszFeedLinkACL;

		if (rhs.m_pszFeedLinkACL)
		{
			int nDim = wcslen(rhs.m_pszFeedLinkACL)+1;
			m_pszFeedLinkACL = new WCHAR[nDim]();
			wcscpy_s(m_pszFeedLinkACL, nDim, rhs.m_pszFeedLinkACL);
		}
		else
		{
			m_pszFeedLinkACL = NULL;
		}

		//m_pszFeedLinkRevisions
		delete[] m_pszFeedLinkRevisions;

		if (rhs.m_pszFeedLinkRevisions)
		{
			int nDim = wcslen(rhs.m_pszFeedLinkRevisions)+1;
			m_pszFeedLinkRevisions = new WCHAR[nDim]();
			wcscpy_s(m_pszFeedLinkRevisions, nDim, rhs.m_pszFeedLinkRevisions);
		}
		else
		{
			m_pszFeedLinkRevisions = NULL;
		}

		//m_pszMD5Checksum
		delete[] m_pszMD5Checksum;

		if (rhs.m_pszMD5Checksum)
		{
			int nDim = wcslen(rhs.m_pszMD5Checksum)+1;
			m_pszMD5Checksum = new WCHAR[nDim]();
			wcscpy_s(m_pszMD5Checksum, nDim, rhs.m_pszMD5Checksum);
		}
		else
		{
			m_pszMD5Checksum = NULL;
		}
	}

	return *this;
}
