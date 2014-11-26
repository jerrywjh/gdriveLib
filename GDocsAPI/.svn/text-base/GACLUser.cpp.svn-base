#include "StdAfx.h"
#include "GACLUser.h"

/**********************************************
This constructor is used to create(actually get) a ACLuser object from ACL response XML file
Params:
	[in]pEntryElement: the corresponding acl entry element

This construction is used to get ACL user from GetACLUser request.
************************************************/
GACLUser::GACLUser(TiXmlElement* pEntryElement):
	m_gdEtag(NULL),
	m_id(NULL),
	m_updated(NULL),
	m_appEdited(NULL),
	m_categoryScheme(NULL),
	m_categoryTerm(NULL),
	m_title(NULL),
	m_linkSelf(NULL),
	m_linkEdit(NULL),
	m_role(NULL),
	m_scopeType(NULL),
	m_scopeValue(NULL)
{
	if (pEntryElement == NULL)
		throw std::runtime_error("the Acl user entry is invalid.");

	//gdetag
	const char* lpszGdEtag = pEntryElement->Attribute("gd:etag");
	if (lpszGdEtag)
	{
		int nDim = strlen(lpszGdEtag)+1;
		m_gdEtag = new char[nDim]();
		strcpy_s(m_gdEtag, nDim, lpszGdEtag);
	}

	//id
	TiXmlElement* pIdElement =  pEntryElement->FirstChildElement("id");
	if (pIdElement)
	{
		const char* lpszId = pIdElement->GetText();
		if (lpszId)
		{
			int nDim = strlen(lpszId)+1;
			m_id = new char[nDim]();
			strcpy_s(m_id, nDim, lpszId);
		}
	}

	//updated
	TiXmlElement* pUpdatedElement =  pEntryElement->FirstChildElement("updated");
	if (pUpdatedElement)
	{
		const char* lpszUpdated = pUpdatedElement->GetText();
		if (lpszUpdated)
		{
			int nDim = strlen(lpszUpdated)+1;
			m_updated = new char[nDim]();
			strcpy_s(m_updated, nDim, lpszUpdated);
		}
	}

	//edited
	TiXmlElement* pEditedElement =  pEntryElement->FirstChildElement("app:edited");
	if (pEditedElement)
	{
		const char* lpszEdited= pEditedElement->GetText();
		if (lpszEdited)
		{
			int nDim = strlen(lpszEdited)+1;
			m_appEdited = new char[nDim]();
			strcpy_s(m_appEdited, nDim, lpszEdited);
		}
	}

	//category: Scheme  and term
	TiXmlElement* pCategoryElement = pEntryElement->FirstChildElement("category ");
	if (pCategoryElement)
	{
		const char* const lpszScheme = pCategoryElement->Attribute("scheme");
		if (lpszScheme)
		{
			int nDim = strlen(lpszScheme)+1;
			m_categoryScheme = new char[nDim]();
			strcpy_s(m_categoryScheme, nDim, lpszScheme);
		}
		const char* const lpszTerm = pCategoryElement->Attribute("term");
		if (lpszTerm)
		{
			int nDim = strlen(lpszTerm)+1;
			m_categoryTerm = new char[nDim]();
			strcpy_s(m_categoryTerm, nDim, lpszTerm);
		}
	}

	//title 
	TiXmlElement* pTitleElement = pEntryElement->FirstChildElement("title");
	if (pTitleElement)
	{
		const char* const lpszTitle = pTitleElement->GetText();
		if (lpszTitle)
		{
			int nDim = strlen(lpszTitle)+1;
			m_title = new char[nDim]();
			strcpy_s(m_title, nDim, lpszTitle);
		}
	}

	//linkself, linkEdit
	TiXmlElement* pLinkElement = pEntryElement->FirstChildElement("link");
	for( pLinkElement; pLinkElement; pLinkElement=pLinkElement->NextSiblingElement("link") )
	{
		if (strcmp( pLinkElement->Attribute("rel"), "self")==0)
		{
			const char* const lpszLinkSelf = pLinkElement->Attribute("href");
			if (lpszLinkSelf)
			{
				int nDim = strlen(lpszLinkSelf)+1;
				m_linkSelf = new char[nDim]();
				strcpy_s(m_linkSelf, nDim, lpszLinkSelf);
			}
		} 

		if (strcmp( pLinkElement->Attribute("rel"), "edit")==0)
		{
			const char* const lpszLinkEdit = pLinkElement->Attribute("href");
			if (lpszLinkEdit)
			{
				int nDim = strlen(lpszLinkEdit)+1;
				m_linkEdit = new char[nDim]();
				strcpy_s(m_linkEdit, nDim, lpszLinkEdit);
			}
		} 
	}

	//role
	TiXmlElement* pRoleElement = pEntryElement->FirstChildElement("gAcl:role");
	if (pRoleElement)
	{
		const char* const lpszRole = pRoleElement->Attribute("value");
		if (lpszRole)
		{
			int nDim = strlen(lpszRole)+1;
			m_role = new char[nDim]();
			strcpy_s(m_role, nDim, lpszRole);
		}
	}

	//scopeType, scopeValue(userEmail)
	TiXmlElement* pScopeElement = pEntryElement->FirstChildElement("gAcl:scope");
	if (pScopeElement)
	{
		const char* const lpszScopeType = pScopeElement->Attribute("type");
		if (lpszScopeType)
		{
			int nDim = strlen(lpszScopeType)+1;
			m_scopeType = new char[nDim]();
			strcpy_s(m_scopeType, nDim, lpszScopeType);
		}

		const char* const lpszScopeValue = pScopeElement->Attribute("value");
		if (lpszScopeValue)
		{
			int nDim = strlen(lpszScopeValue)+1;
			m_scopeValue = new char[nDim]() ;
			strcpy_s(m_scopeValue, nDim, lpszScopeValue);
		}
	}
}

/*****************************************************
This constructor is used to construct ACL user Object from Scratch,
That is , create a totally new ACL user,
After this construction, only m_role, m_scopeType, m_scopeValue is initialized, other fields are still NULL
this construction is used in Create Totally new ACL user .
******************************************************/
GACLUser::GACLUser(LPCSTR lpszRole, LPCSTR lpszScopeType, LPCSTR lpszScopeValue):
	m_gdEtag(NULL),
	m_id(NULL),
	m_updated(NULL),
	m_appEdited(NULL),
	m_categoryScheme(NULL),
	m_categoryTerm(NULL),
	m_title(NULL),
	m_linkSelf(NULL),
	m_linkEdit(NULL),
	m_role(NULL),
	m_scopeType(NULL),
	m_scopeValue(NULL)
{
	if (lpszRole)
	{
		int nDim = strlen(lpszRole)+1;
		m_role = new char[nDim]();
		strcpy_s(m_role, nDim, lpszRole);
	}
	if (lpszScopeType)
	{
		int nDim = strlen(lpszScopeType)+1;
		m_scopeType = new char[nDim]();
		strcpy_s(m_scopeType, nDim, lpszScopeType);
	}
	if (lpszScopeValue)
	{
		int nDim = strlen(lpszScopeValue)+1;
		m_scopeValue = new char[nDim]();
		strcpy_s(m_scopeValue, nDim, lpszScopeValue);
	}
}


/******************************************
Destructor, release all memory
********************************************/
GACLUser::~GACLUser(void)
{
	if (m_gdEtag)
		delete [] m_gdEtag;
	if (m_id)
		delete [] m_id;
	if (m_updated)
		delete [] m_updated;
	if (m_appEdited)
		delete [] m_appEdited;
	if (m_categoryScheme)
		delete [] m_categoryScheme;
	if (m_title)
		delete [] m_title;
	if (m_linkSelf)
		delete [] m_linkSelf;
	if (m_linkEdit)
		delete [] m_linkEdit;
	if (m_role)
		delete [] m_role;
	if (m_scopeType)
		delete [] m_scopeType;
	if (m_scopeValue)
		delete [] m_scopeValue;
		
}

/****************************************************************
Getters and Settters

Only role can be changed by the user, so there is only one Setter: SetRole();
****************************************************************/
LPCSTR GACLUser::GetGdEtag()
{
	return m_gdEtag;
}

LPCSTR GACLUser::GetId()
{
	return m_id;
}
	
LPCSTR GACLUser::GetUpdated()
{
	return m_updated;
}
	
LPCSTR GACLUser::GetAppEdited()
{
	return m_appEdited;
}

LPCSTR GACLUser::GetCategoryScheme()
{
	return m_categoryScheme;
}

LPCSTR GACLUser::GetCategoryTerm()
{
	return m_categoryTerm;
}

LPCSTR GACLUser::GetTitle()
{
	return m_title;
}
	
LPCSTR GACLUser::GetLinkSelf()
{
	return m_linkSelf;
}
	
LPCSTR GACLUser::GetLinkEdit()
{
	return m_linkEdit;
}
	
LPCSTR GACLUser::GetRole()
{
	return m_role;
}

//change the Role	
void GACLUser::SetRole(LPCSTR lpszRole)
{
	if (m_role)
		delete [] m_role;

	if (lpszRole == NULL)
	{
		m_role = NULL;
	}
	else
	{
		int nDim = strlen(lpszRole)+1;
		m_role = new char[nDim]();
		strcpy_s(m_role, nDim, lpszRole);
	}
}
	
LPCSTR GACLUser::GetScopeType()
{
	return m_scopeType;
}
	
LPCSTR GACLUser::GetScopeValue()
{
	return m_scopeValue;
}
