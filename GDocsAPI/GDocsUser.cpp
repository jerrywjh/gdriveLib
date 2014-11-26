/*************************************************************************
	File: GDocsUser.cpp
	Author: Jerry Wang. Beyond Gao
	Version: 2.0 
	Date: 2012/1/18
*****************************************************************************/

#include "StdAfx.h"
#include "GDocsUser.h"
/******************************************************************************
Function:   GDocsUser Constructor
Description:  
		The constructor of the class. After the construction, you get a GDocsUser object without any Token,
		which means this user is not loggedin.  
		Only after calling the Login function ,  the GDocsUser gets AccessToken, and has a loggedin state.

Other:
		Dynamic array is used to allocate memory to the member variables of this class, which can avoid
		buffer-overflow bugs for fixed array.  It also can save memory!

****************************************************************************/
GDocsUser::GDocsUser()
	:m_pszEmail(NULL),
	m_pszName(NULL),
	m_quotaBytesTotal(1073741824),
	m_quotaBytesUsed(0),
	m_quotaBytesUsedInTrash(0),
	m_maxUploadSizeForDocument(26214400),
	m_maxUploadSizeForSpreadsheet(10485760),
	m_maxUploadSizeForPresentation(10485760),
	m_maxUploadSizeForDrawing(2097152),
	m_maxUploadSizeForPdf(10737418240),
	m_maxUploadSizeForFile(10737418240)
{		
}

/******************************************************************************
Function:   GDocsUser Destructor
Description:  
		The destructor of the class. 
		In this function, we  release all the allocated memory of the member variables. 

****************************************************************************/
GDocsUser::~GDocsUser(void)
{
	if (m_pszEmail)
	{
		delete []  m_pszEmail;
	}

	if (m_pszName)
	{
		delete [] m_pszName;
	}
} 



/****************************************************************************
About SetXXX function:
	If the member variables has been initialized before, delete the old value, then copy the new value into it.
********************************************************************************/
LPCSTR GDocsUser::GetEmail()
{
	return m_pszEmail;
}

void GDocsUser::SetEmail(LPCSTR lpszEmail)
{
	if (m_pszEmail)
	{
		delete []  m_pszEmail;
	}
	if (lpszEmail == NULL)
	{
		m_pszEmail = NULL;
	}
	else
	{
		int nDimension = strlen(lpszEmail) +1;
		m_pszEmail = new CHAR[nDimension]();
		strcpy_s(m_pszEmail, nDimension, lpszEmail);
	}
}

LPCSTR GDocsUser::GetName()
{
	return m_pszName;
}

void GDocsUser::SetName(LPCSTR lpszName)
{
	if (m_pszName)
	{
		delete m_pszName;
		m_pszName = NULL;
	}

	if(lpszName != NULL)
	{
		int nDimension = strlen(lpszName) +1;
		m_pszName = new CHAR[nDimension];
		strcpy_s(m_pszName, nDimension, lpszName);
	}
}

INT64 GDocsUser::GetQuotaBytesTotal()
{
	return m_quotaBytesTotal;
}

void GDocsUser::SetQuotaBytesTotal(INT64 uQuotaBytesTotal)
{
	m_quotaBytesTotal = uQuotaBytesTotal;
}

INT64 GDocsUser::GetQuotaBytesUsed()
{
	return m_quotaBytesUsed;
}

void GDocsUser::SetQuotaBytesUsed(INT64 uQuotaBytesUsed)
{
	m_quotaBytesUsed = uQuotaBytesUsed;
}

INT64 GDocsUser::GetQuotaBytesUsedInTrash()
{
	return m_quotaBytesUsedInTrash;
}

void GDocsUser::SetQuotaBytesUsedInTrash(INT64 uQuotaBytesUsedInTrash)
{
	m_quotaBytesUsedInTrash = uQuotaBytesUsedInTrash;
}

INT64 GDocsUser::GetMaxUploadSizeForDocument()
{
	return m_maxUploadSizeForDocument;
}

void GDocsUser::SetMaxUploadSizeForDocument(INT64 uMaxUploadSizeForDocument)
{
	m_maxUploadSizeForDocument = uMaxUploadSizeForDocument;
}

INT64 GDocsUser::GetMaxUploadSizeForSpreadsheet()
{
	return m_maxUploadSizeForSpreadsheet;
}

void GDocsUser::SetMaxUploadSizeForSpreadsheet(INT64 uMaxUploadSizeForSpreadsheet)
{
	m_maxUploadSizeForSpreadsheet = uMaxUploadSizeForSpreadsheet;
}

INT64 GDocsUser::GetMaxUploadSizeForPresentation()
{
	return m_maxUploadSizeForPresentation;
}

void GDocsUser::SetMaxUploadSizeForPresentation(INT64 uMaxUploadSizeForPresentation)
{
	m_maxUploadSizeForPresentation = uMaxUploadSizeForPresentation;
}

INT64 GDocsUser::GetMaxUploadSizeForDrawing()
{
	return m_maxUploadSizeForDrawing;
}

void GDocsUser::SetMaxUploadSizeForDrawing(INT64 uMaxUploadSizeForDrawing)
{
	m_maxUploadSizeForDrawing = uMaxUploadSizeForDrawing;
}

INT64 GDocsUser::GetMaxUploadSizeForPdf()
{
	return m_maxUploadSizeForPdf;
}

void GDocsUser::SetMaxUploadSizeForPdf(INT64 uMaxUploadSizeForPdf)
{
	m_maxUploadSizeForPdf = uMaxUploadSizeForPdf;
}

INT64 GDocsUser::GetMaxUploadSizeForFile()
{
	return m_maxUploadSizeForFile;
}

void GDocsUser::SetMaxUploadSizeForFile(INT64 uMaxUploadSizeForFile)
{
	m_maxUploadSizeForFile = uMaxUploadSizeForFile;
}

