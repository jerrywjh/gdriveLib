/*************************************************************************
	File: GDocsUser.h
	Author: Jerry Wang , Beyond Gao.
	Version: 2.0 
	Date: 2012/1/18
	Description:
			GDocsUser is the wrapper class around  Google user account object, like email, name, quota...

*****************************************************************************/
#pragma once

class GDocsUser
{
private:
	CHAR * m_pszEmail;						// do not use Array type, use pointer is more flexible and can avoid buffer overflow.
	CHAR *	m_pszName;					// the name of the user , like Jerry Wang
	INT64		m_quotaBytesTotal;
	INT64		m_quotaBytesUsed;
	INT64		m_quotaBytesUsedInTrash;
	INT64		m_maxUploadSizeForDocument;
	INT64		m_maxUploadSizeForSpreadsheet;
	INT64		m_maxUploadSizeForPresentation;
	INT64		m_maxUploadSizeForDrawing;
	INT64		m_maxUploadSizeForPdf;
	INT64		m_maxUploadSizeForFile;

public:
	GDocsUser();
	~GDocsUser(void);

public:
	LPCSTR GetEmail();
	void SetEmail(LPCSTR lpszEmail);

	LPCSTR GetName();
	void SetName(LPCSTR lpszName);

	INT64 GetQuotaBytesTotal();
	void SetQuotaBytesTotal(INT64 uQuotaBytesTotal);
	
	INT64 GetQuotaBytesUsed();
	void SetQuotaBytesUsed(INT64 uQuotaBytesUsed);

	INT64 GetQuotaBytesUsedInTrash();
	void SetQuotaBytesUsedInTrash(INT64 uQuotaBytesUsedInTrash);

	INT64 GetMaxUploadSizeForDocument();
	void SetMaxUploadSizeForDocument(INT64 uMaxUploadSizeForDocument);

	INT64 GetMaxUploadSizeForSpreadsheet();
	void SetMaxUploadSizeForSpreadsheet(INT64 uMaxUploadSizeForSpreadsheet);

	INT64 GetMaxUploadSizeForPresentation();
	void SetMaxUploadSizeForPresentation(INT64 uMaxUploadSizeForPresentation);

	INT64 GetMaxUploadSizeForDrawing();
	void SetMaxUploadSizeForDrawing(INT64 uMaxUploadSizeForDrawing);

	INT64 GetMaxUploadSizeForPdf();
	void SetMaxUploadSizeForPdf(INT64 uMaxUploadSizeForPdf);

	INT64 GetMaxUploadSizeForFile();
	void SetMaxUploadSizeForFile(INT64 uMaxUploadSizeForFile);
};
