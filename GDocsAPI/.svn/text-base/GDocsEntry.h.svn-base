#pragma once
#include <vector>
#include "stdafx.h"

class TiXmlElement;

struct LinkParent
{
	WCHAR* lpszParentHref;
	WCHAR* lpszParentTitle;
};
/***************************************************
the wrapper class for an  entry  item(TiXmlElement entry)  
****************************************************/
class GDocsEntry
{
public:
	GDocsEntry (TiXmlElement * pEntryElement);
	~GDocsEntry(void);
	
	GDocsEntry(const GDocsEntry&);				//copy constructor
	GDocsEntry& operator= (const GDocsEntry&);	//assignment operator

public:	
	//the gd:etag attribute of the entry.  NULL if not exist.
	WCHAR*  m_pszGDEtag;
	
	// the ID link of the entry  , in  <id> element, like :   https://docs.google.com/feeds/id/file%3A0B7CmbVXdOi7mY2RmOGNiMGEtZWNlNC00NzNiLThkY2UtOGM5N2M5ZWE2NTg5
	WCHAR*  m_pszIDUrl;
	
	//the published time;   in <published> element, Format : "2010-05-24 15:09:21"
	WCHAR*	m_pszPublished;
	
	//the updated time;   in <updated> element	, Format:  "2010-05-24 15:09:21"
	WCHAR*	m_pszUpdated;
	
	// Entry's App:edited Date and Time, in <App:edit>element , Format:  "2010-05-24 15:09:21"
	WCHAR*	m_pszAppEdited;
		
	BOOL	m_bIsStarred;
	BOOL	m_bIsHidden;
	BOOL	m_bIsViewed;
	
	// the file name of the Entry
	WCHAR*	m_pszTitle;
	
	// Content type of the entry, like: "text/html" , "image/jpeg".... .  this is used to download Non-GoogleDocs file (like pdf, exe, rar.....),usually, this is used with  "m_pszContentSrc"
	WCHAR*	m_pszContentType;
	
	//ContentSrc is a very importan link: For folders: if you request the ContentSrc url, you get the entry list of the folder. For files, if you request this url, you can download/export the file.  
	//format : https://doc-0g-1k-docs.googleusercontent.com/docs/securesc/p62krtm66k35ibqhev55ra8osq858oka/ucsrhmmpr5jca5r6t8lodhdneeslb5a3/1313035200000/16813505762710726417/16813505762710726417/0B7CmbVXdOi7mODU4NDcxNjAtMDBhMi00YWNmLWIyYzMtN2FmMWVkZmE3MTQ1?h=16653014193614665626&e=download&gd=true
	WCHAR*	m_pszContentSrc;		

	//the parents links or the entry.  One entry can have  0, 1, or more parents.
	std::vector<LinkParent>  m_vecParents;

	//link at which you can open this document in a web browser
	WCHAR*	m_pszLinkAlternate;  

	//link at which you can fetch only this entry
	WCHAR*	m_pszLinkSelf;

	//link at which you can PUT updates to this entry
	WCHAR*	m_pszLinkEdit;

	//link at which you can PUT resumable updates to this entry's content
	WCHAR*  m_pszLinkResumableEditMedia;

	//link at which you can fetch a thumbnail of this resource,  NOT all entries have thumbnail, only some of them have
	WCHAR*  m_pszLinkThumbNail;

	//the author name
	WCHAR*	m_pszAuthorName;
	//the author's email
	WCHAR*	m_pszAuthorEmail;
	
	//resourceId: like:presentation:0AbVj86DJgTM6ZGNqYm1tc3BfMTY0OWR2ODl3Y3E
	WCHAR*	m_pszResourceId;
	WCHAR*	m_pszType;		//folder,file.  document, spreadsheet, PDF....
	WCHAR*	m_pszID;			//entry id, like "0B7CmbVXdOi7mY2RmOGNiMGEtZWNlNC00NzNiLThkY2UtOGM5N2M5ZWE2NTg5"

	//information about  the user who last modified this entry.
	WCHAR*	m_pszLastModifiedByName;
	WCHAR*	m_pszLastModifiedByEmail;
	
	//number of butes of the quota this document used.  Native Google Docs use 0 bytes.
	WCHAR*	m_pszQuotaBytesUsed;  //use string format! not integer , NOTICE: for folder, this node  is NULL!
	WCHAR m_pszQuotaBytesUsedFormated[32]; // formated string of size , like " 23,564,667"  NULL for folder, "0" for document,spreadsheet, presentation, drawing 

	//TRUE if writters can invite other users to view and edit this document
	BOOL	m_bIsWritersCanInvite;

	//link at which you can create, retrieve, update and delete ACL entries for this document.
	WCHAR*	m_pszFeedLinkACL;

	//Link at which you can create, retrieve, update and delete revisions of this document.
	WCHAR*	m_pszFeedLinkRevisions;

	//MD5 checksum used to verify the contents of this file.   Only some File type entry has this
	WCHAR* m_pszMD5Checksum;
};
