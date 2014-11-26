#pragma once
#include "stdafx.h"
#include "tinyxml.h"
class GACLUser
{
public:
	GACLUser(TiXmlElement* pEntryElement);
	GACLUser(LPCSTR lpszRole, LPCSTR lpszScopeType, LPCSTR lpszScopeValue);
	~GACLUser(void);

private:
	// do not allow copy and assignment
	GACLUser(const GACLUser&){}
	GACLUser& operator=(const GACLUser&) {}

public:
	LPCSTR GetGdEtag();
	LPCSTR GetId();
	LPCSTR GetUpdated();
	LPCSTR GetAppEdited();
	LPCSTR GetCategoryScheme();
	LPCSTR GetCategoryTerm();
	LPCSTR GetTitle();
	LPCSTR GetLinkSelf();
	LPCSTR GetLinkEdit();
	LPCSTR GetRole();
	void SetRole(LPCSTR lpszRole);
	LPCSTR GetScopeType();
	LPCSTR GetScopeValue();

private:
	char* m_gdEtag;
	char* m_id;
	char* m_updated;
	char* m_appEdited;
	char* m_categoryScheme;
	char* m_categoryTerm;
	char* m_title;
	char* m_linkSelf;
	char* m_linkEdit;
	char* m_role;
	char* m_scopeType;
	char* m_scopeValue;
};

