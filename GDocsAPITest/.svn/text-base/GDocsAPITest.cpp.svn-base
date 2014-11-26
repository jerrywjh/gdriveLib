#include "StdAfx.h"
#include <iostream>
//#include <fstream>
#include "GDocsAPI.h"
#include "GDocsUser.h"
#include "GDocsAPITest.h"
#include "HelperFunctions.h"
#include "GDocsEntry.h"

using namespace std;
volatile LONG	g_bCancelTransfering=FALSE;		// TREUE, if user canceled the uploading/download process. 


GDocsAPITest::GDocsAPITest(void)//: m_pGDocsApi(NULL)
{
	m_pGDocsApi = new GDocsAPI();
}

GDocsAPITest::~GDocsAPITest(void)
{
	if (m_pGDocsApi)
		delete m_pGDocsApi;
}

BOOL GDocsAPITest::login()
{
	string strAuthCode;
	::ShellExecute(NULL, L"open", L"https://accounts.google.com/o/oauth2/auth?scope=https%3A%2F%2Fdocs%2Egoogle%2Ecom%2Ffeeds%2F&redirect_uri=urn:ietf:wg:oauth:2.0:oob&response_type=code&client_id=848488221964.apps.googleusercontent.com", NULL, NULL, SW_SHOWNORMAL);
	cout<<"copy the auth code to D;:\authcode.xml file , after that ,input Y and presss enter";
	cin>>strAuthCode;

	TiXmlDocument* pXmlDoc = new TiXmlDocument();
	pXmlDoc->LoadFile("authcode.xml");
	TiXmlElement* pElement = pXmlDoc->FirstChildElement("code");

	LPCSTR lpszAuthCode = pElement->GetText();

	int iLen = strlen(lpszAuthCode)+1;
	WCHAR* lpszWAuthCode = new WCHAR[iLen]();
	MultiByteToWideChar(CP_UTF8, 0, lpszAuthCode, -1, lpszWAuthCode, iLen);
	m_pGDocsApi->SetAuthCode(lpszWAuthCode);
	delete [] lpszWAuthCode;

	return m_pGDocsApi->GetTokensFromAuthCode();

 



	//return TRUE;
}

BOOL GDocsAPITest::LoginTest()
{
	//test for jerrywjh@gmail.com , passed.
 //	m_pGDocsApi = new GDocsAPI();

	//test for weird password, passed.
	//m_pGDocsApi = new GDocsAPI("justinmorse007@gmail.com", "justin_morse&.007+");

	//test for Google app user, passed
	//m_pGDocsApi = new GDocsAPI("wjh@gdocsapp.com", "qian_ying&");

	//test for Google app administrator, passed
	//m_pGDocsApi = new GDocsAPI("admin@gdocsapp.com", "jerrywjh");

	//test for 2 steps verification,  passed
	//m_pGDocsApi = new GDocsAPI("wangjianhua99@gmail.com", "oaetkxxhbmgbxyth");   //???这个密码基本没有什么用吧

	//m_pGDocsApi->ApiLogin("writely"); //doc
	//m_pGDocsApi->ApiLogin("wise");  //spreadsheet

	//LPCSTR szDocToken =  m_pGDocsApi->GetUser()->GetTokenForDocs();
	//LPCSTR szSpreadSheetToken = m_pGDocsApi->GetUser()->GetTokenForSpreadSheets();
	//m_pGDocsApi->GetUserInfo();

	//if ((szDocToken == NULL) || (szSpreadSheetToken == NULL))
	//{
	//	cout<< " login test failed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	//}
	//else
	//{
	//	cout<<"Login succeeded!"<<endl;
	//	cout<< "Google Docs Token: "<<szDocToken<<endl;
	//	cout<< "Google SpreadSheet Token:" << szSpreadSheetToken<<endl;
	//	cout<< "Email:" <<m_pGDocsApi->GetUser()->GetEmail()<<endl;
	//	cout<< "Password:" <<m_pGDocsApi->GetUser()->GetPassword()<<endl;
	//	cout<< "Name:" <<m_pGDocsApi->GetUser()->GetName()<<endl;
	//	cout<< "QuotaBytesTotal:" <<m_pGDocsApi->GetUser()->GetQuotaBytesTotal()<<endl;
	//	cout<< "QuotaBytesUsed:" <<m_pGDocsApi->GetUser()->GetQuotaBytesUsed()<<endl;
	//	cout<< "QuotaBytesUsedInTrash:" <<m_pGDocsApi->GetUser()->GetQuotaBytesUsedInTrash()<<endl;
	//	cout<< "MaxUploadSizeForDocument:" <<m_pGDocsApi->GetUser()->GetMaxUploadSizeForDocument()<<endl;
	//	cout<< "MaxUploadSizeForSpreadsheet:" <<m_pGDocsApi->GetUser()->GetMaxUploadSizeForSpreadsheet()<<endl;
	//	cout<< "MaxUploadSizeForPresentation:" <<m_pGDocsApi->GetUser()->GetMaxUploadSizeForPresentation()<<endl;
	//	cout<< "MaxUploadSizeForDrawing:" <<m_pGDocsApi->GetUser()->GetMaxUploadSizeForDrawing()<<endl;
	//	cout<< "MaxUploadSizeForPdf:" <<m_pGDocsApi->GetUser()->GetMaxUploadSizeForPdf()<<endl;
	//	cout<< "MaxUploadSizeForFile:" <<m_pGDocsApi->GetUser()->GetMaxUploadSizeForFile()<<endl;
	//	cout<< "Test passed  ................."<<endl;
	//}


	//
	//////bad input test  , captcha!!
	//delete m_pGDocsApi;
	//try
	//{
	//	//T 可以产生Captcha 的请求。。。
 //		//m_pGDocsApi = new GDocsAPI("jerrywjhdsf@gmail.com", "qy3142asdf80");
	//	m_pGDocsApi = new GDocsAPI(NULL, NULL);
	//	
	//	m_pGDocsApi->ApiLogin("writely");
	//	m_pGDocsApi->ApiLogin("wise");
	//}
	//catch(std::runtime_error& e)
	//{
	//	cout<<e.what()<<endl;
	//}
	//catch (...)
	//{
	//	cout<<"login failed"<<endl;
	//}

	//// NULL input test
	//try
	//{
	//	m_pGDocsApi->Login(NULL, NULL);
	//}
	//catch(std::exception e)
	//{
	//	cout<<e.what()<<endl;
	//}
	//catch(...)
	//{
	//	cout<<"Login failed"<<endl;
	//}

	////wrong password input test
	//try
	//{
	//	m_pGDocsApi->Login("jerrywjh@gmail.com", "314sdf280");
	//}
	//catch(std::exception e)
	//{
	//	cout<<e.what()<<endl;
	//}
	//catch (...)
	//{
	//	cout<<"login failed"<<endl;
	//}
	return TRUE;
}


//test GetEntryList. passed
BOOL GDocsAPITest::GetEntryListTest()
{
	login();
	
	TiXmlDocument * pXmlDoc = new TiXmlDocument();

	//lastest 100 items
	//m_pGDocsApi->GetEntryList("https://docs.google.com/feeds/default/private/full?showfolders=true", pXmlDoc);


	//get trashed items
	m_pGDocsApi->GetEntryList("https://docs.google.com/feeds/default/private/full/-/trashed?showfolders=true",pXmlDoc);
	
	//entrylist for aaaa folder
	//m_pGDocsApi->GetEntryList("https://docs.google.com/feeds/default/private/full/folder%3A0B7CmbVXdOi7mYzhlM2NjODgtNzlkZC00ZGZlLWFhN2EtMWJlODA3NmY0YjIy/contents", pXmlDoc);

	//get all documetn
	//m_pGDocsApi->GetEntryList("https://docs.google.com/feeds/default/private/full/-/document?max-results=5", pXmlDoc);
	//m_pGDocsApi->GetEntryList("https://docs.google.com/feeds/default/private/full/-/-mine/folder", pXmlDoc);

	pXmlDoc->SaveFile("d:/trashed.xml");
	
	if (pXmlDoc)
	{
		TiXmlElement* pFeedElement = pXmlDoc->FirstChildElement("feed");
		
		for (TiXmlElement* pElement = pFeedElement->FirstChildElement("entry"); pElement; pElement = pElement->NextSiblingElement("entry"))
		{
			GDocsEntry* pDocsEntry = new GDocsEntry(pElement);

			cout<<"The information of the entry" <<endl<<endl;
			wcout<<L" GDEtag :"	<< pDocsEntry->m_pszGDEtag<<endl;
			wcout<<L" IDURL :"		<< pDocsEntry->m_pszIDUrl <<endl;
			wcout<<L" Published :" << pDocsEntry->m_pszPublished <<endl;
			wcout<<L" Updated :"	<< pDocsEntry->m_pszUpdated<<endl;
			wcout<<L" AppEdited :" << pDocsEntry->m_pszAppEdited<<endl;
			wcout<<L" IsStarred :" << pDocsEntry->m_bIsStarred<<endl;
			wcout<<L" IsHidden :" << pDocsEntry->m_bIsHidden<<endl;
			wcout<<L" IsViewed :" << pDocsEntry->m_bIsViewed<<endl;
			wcout<<L" Title :"		<< pDocsEntry->m_pszTitle<<endl;
			wcout<<L" ContentType :" << pDocsEntry->m_pszContentType<<endl;
			wcout<<L" Content Src :" << pDocsEntry->m_pszContentSrc<<endl;
			wcout<<L" LinkAlternate :" << pDocsEntry->m_pszLinkAlternate<<endl;
			wcout<<L" ResumeableEditMedia :"	<< (pDocsEntry->m_pszLinkResumableEditMedia?pDocsEntry->m_pszLinkResumableEditMedia:L"")<<endl;
			wcout<<L" AuthorName :"	<< pDocsEntry->m_pszAuthorName<<endl;
			wcout<<L" AuthorEmail :"	<< pDocsEntry->m_pszAuthorEmail<<endl;
			wcout<<L" ResourceID :"	<< pDocsEntry->m_pszResourceId<<endl;
			wcout<<L" Type :"	<< pDocsEntry->m_pszType<<endl;
			wcout<<L" ID :"	<< pDocsEntry->m_pszID<<endl;
			wcout<<L" LastModifiedName :"	<< pDocsEntry->m_pszLastModifiedByName<<endl;
			wcout<<L" LastModifiedEmail :"	<< pDocsEntry->m_pszLastModifiedByEmail<<endl;
			wcout<<L" QuotaByteUsed :"	<< (pDocsEntry->m_pszQuotaBytesUsed?pDocsEntry->m_pszQuotaBytesUsed:L"")<<endl;
			wcout<<L" QuotaByteUsedFormat :"	<<(pDocsEntry->m_pszQuotaBytesUsed? pDocsEntry->m_pszQuotaBytesUsedFormated:L"")<<endl;
			wcout<<L" ACL Link :"	<< pDocsEntry->m_pszFeedLinkACL<<endl;
			wcout<<L" Revision Link :"	<< (pDocsEntry->m_pszFeedLinkRevisions?pDocsEntry->m_pszFeedLinkRevisions:L"")<<endl;
			wcout<<L" MD5CheckSum :"	<< (pDocsEntry->m_pszMD5Checksum? pDocsEntry->m_pszMD5Checksum:L"")<<endl;
	
			wcout<<"................................................................................"<<endl;
		}
	}


	delete pXmlDoc;
	return TRUE;
}


//file mime type and convert type test, Helperfuncion 里面方法的测试


BOOL  folderTest(LPCWSTR szFolderPath)
{

	WIN32_FIND_DATA fileInfo;
	WCHAR szPattern[MAX_PATH]={0};

	wcscpy_s(szPattern, MAX_PATH, szFolderPath);
	wcscat_s(szPattern, MAX_PATH, L"\\*.*");

	HANDLE hFile = ::FindFirstFile(szPattern, &fileInfo);
	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ( (_wcsicmp(fileInfo.cFileName, L".") !=0) && (_wcsicmp(fileInfo.cFileName, L"..") != 0) &&(_wcsicmp(fileInfo.cFileName, L"") !=0) )
			{
				WCHAR szFilePathName[MAX_PATH]={0};
				wcscpy_s(szFilePathName, MAX_PATH, szFolderPath);
				wcscat_s(szFilePathName, MAX_PATH, L"\\");
				wcscat_s(szFilePathName, MAX_PATH, fileInfo.cFileName);


				if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					folderTest(szFilePathName);
				}
				else
				{
					WCHAR lpszFileName[MAX_PATH]={0};
					HelperFunctions::GetFileNameFromFilePath(szFilePathName, lpszFileName, MAX_PATH);
					wcout<<endl;

					if (HelperFunctions::GetMimeTypeFromFileName(lpszFileName) == NULL)
					{
						wcout<<L"File:  "<<szFilePathName<<L"   Mime Type: " <<"Original Stream"<<endl;
					}
					else
					{
						wcout<<L"File:  "<<szFilePathName<<L"   Mime Type: " <<HelperFunctions::GetMimeTypeFromFileName(lpszFileName)<<endl;
					}
					wcout<<L"Can be converted to : ";
					if (HelperFunctions::CanBeConvertedToDocument(lpszFileName))
						wcout<<L"DOCUMENT ";
					if (HelperFunctions::CanBeConvertedToSpreadsheet(lpszFileName))
						wcout<<L"SPREADSHEET ";
					if (HelperFunctions::CanBeConvertedToPresentation(lpszFileName))
						wcout<<L"PRESENTATION ";
					if (HelperFunctions::CanBeConvertedToDrawing(lpszFileName))
						wcout<<L"DRAWING ";

					wcout<<endl;

				}
			}

		}while (::FindNextFile(hFile, &fileInfo) == TRUE);

	}
	return TRUE;
}

BOOL GDocsAPITest::FileMIMETypeTest()
{

//	folderTest( L"e:\\Ebook\\java");

	
	//document
	LPWSTR lpszFileName1= L"aaa.htm";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName1), "text/html") !=0)
	{
		cout<<"Wrong for " <<lpszFileName1<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToDocument(lpszFileName1))
	{
		cout<<"Wrong for " <<lpszFileName1 << "Convert"<<endl;
	}

	lpszFileName1= L"aaa.html";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName1), "text/html") !=0)
	{
		cout<<"Wrong for " <<lpszFileName1<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToDocument(lpszFileName1))
	{
		cout<<"Wrong for " <<lpszFileName1 << "Convert"<<endl;
	}


	lpszFileName1= L"aaa.doc";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName1) , "application/msword") !=0)
	{
		cout<<"Wrong for " <<lpszFileName1<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToDocument(lpszFileName1))
	{
		cout<<"Wrong for " <<lpszFileName1 << "Convert"<<endl;
	}

	lpszFileName1= L"aaa.rtf";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName1) , "application/rtf") !=0)
	{
		cout<<"Wrong for " <<lpszFileName1<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToDocument(lpszFileName1))
	{
		cout<<"Wrong for " <<lpszFileName1 << "Convert"<<endl;
	}


	lpszFileName1= L"aaa.sxw";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName1), "application/vnd.sun.xml.writer") !=0)
	{
		cout<<"Wrong for " <<lpszFileName1<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToDocument(lpszFileName1))
	{
		cout<<"Wrong for " <<lpszFileName1 << "Convert"<<endl;
	}


	
	lpszFileName1= L"aaa.txt";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName1) , "text/plain") !=0)
	{
		cout<<"Wrong for " <<lpszFileName1<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToDocument(lpszFileName1))
	{
		cout<<"Wrong for " <<lpszFileName1 << "Convert"<<endl;
	}

	lpszFileName1= L"aaa.docx";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName1) , "application/vnd.openxmlformats-officedocument.wordprocessingml.document") !=0)
	{
		cout<<"Wrong for " <<lpszFileName1<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToDocument(lpszFileName1))
	{
		cout<<"Wrong for " <<lpszFileName1 << "Convert"<<endl;
	}

	//spreadsheet
	LPWSTR lpszFileName2= L"aaa.tsv";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName2) , "text/tab-separated-values") !=0)
	{
		cout<<"Wrong for " <<lpszFileName2<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToSpreadsheet(lpszFileName2))
	{
		cout<<"Wrong for " <<lpszFileName2 << "Convert"<<endl;
	}

	lpszFileName2= L"aaa.csv";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName2) , "text/csv") !=0)
	{
		cout<<"Wrong for " <<lpszFileName2<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToSpreadsheet(lpszFileName2))
	{
		cout<<"Wrong for " <<lpszFileName2 << "Convert"<<endl;
	}

	lpszFileName2= L"aaa.xlsx";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName2) , "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")!=0)
	{
		cout<<"Wrong for " <<lpszFileName2<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToSpreadsheet(lpszFileName2))
	{
		cout<<"Wrong for " <<lpszFileName2 << "Convert"<<endl;
	}

	lpszFileName2= L"aaa.ods";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName2) , "application/x-vnd.oasis.opendocument.spreadsheet") !=0)
	{
		cout<<"Wrong for " <<lpszFileName2<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToSpreadsheet(lpszFileName2))
	{
		cout<<"Wrong for " <<lpszFileName2 << "Convert"<<endl;
	}

	lpszFileName2= L"aaa.xls";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName2) , "application/vnd.ms-excel") !=0)
	{
		cout<<"Wrong for " <<lpszFileName2<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToSpreadsheet(lpszFileName2))
	{
		cout<<"Wrong for " <<lpszFileName2 << "Convert"<<endl;
	}





	//presentation
	LPWSTR lpszFileName3= L"aaa.ppt";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName3) , "application/vnd.ms-powerpoint") !=0)
	{
		cout<<"Wrong for " <<lpszFileName3<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToPresentation(lpszFileName3))
	{
		cout<<"Wrong for " <<lpszFileName3 << "Convert"<<endl;
	}


	lpszFileName3= L"aaa.pps";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName3) , "application/vnd.ms-powerpoint") !=0)
	{
		cout<<"Wrong for " <<lpszFileName3<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToPresentation(lpszFileName3))
	{
		cout<<"Wrong for " <<lpszFileName3 << "Convert"<<endl;
	}

	
	lpszFileName3= L"aaa.pptx";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName3) , "application/vnd.openxmlformats-officedocument.presentationml.presentation") !=0)
	{
		cout<<"Wrong for " <<lpszFileName3<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToPresentation(lpszFileName3))
	{
		cout<<"Wrong for " <<lpszFileName3 << "Convert"<<endl;
	}

	//drawing
	LPWSTR lpszFileName4= L"aaa.wmfsf";
	if (strcmp(HelperFunctions::GetMimeTypeFromFileName(lpszFileName4) , "application/x-msmetafile") !=0)
	{
		cout<<"Wrong for " <<lpszFileName4<<"  MIME"<<endl;
	}
	if (!HelperFunctions::CanBeConvertedToDrawing(lpszFileName4))
	{
		cout<<"Wrong for " <<lpszFileName4 << "Convert"<<endl;
	}





	//pdf



	//video



	//audio






	//CanBeConvetedToDocument() test
	

	//HelperFunctions::HelperFunctions::CanBeConvertedToSpreadsheet


	//CanBeConvertedToPresentation


	// CanBeConvertedToDrawing

	//HelperFunctions::GetMimeTypeFromFileName
	
	return TRUE;

}

//upload file test
BOOL GDocsAPITest::UpLoadFileTest()   //采用 ProgressMsgTest来测试了 
{
	login();
//	m_pGDocsApi->GetUserInfo();
	
	TiXmlDocument * pXmlDoc = new TiXmlDocument();

	//upload to root
	//m_pGDocsApi->UploadFile(L"d:/aaa.pdf", "https://docs.google.com/feeds/upload/create-session/default/private/full", NULL, FALSE, pXmlDoc);
	m_pGDocsApi->UploadFile(L"d:/软件大赛.xls", "https://docs.google.com/feeds/upload/create-session/default/private/full", NULL, TRUE, pXmlDoc, TRUE);
	
//	pXmlDoc->SaveFile("d:/uploadedfile.xml");

	int i=0;
	return TRUE;
}


BOOL GDocsAPITest::UpdateFileTest()
{
	login();

	TiXmlDocument* pXmlDoc = new TiXmlDocument();

	m_pGDocsApi->UpdateFile(L"d:\\222.doc", "https://docs.google.com/feeds/upload/create-session/default/private/full/file%3A0B7CmbVXdOi7mMjYyOTQ2ZWMtYWFmZS00YjE3LWE0ZDAtMmE4MGQ4NzYyNWQ3",  "\"GlwZFR4NBCt7ImBr\"", TRUE, NULL, pXmlDoc );

	pXmlDoc->SaveFile("d:/update.xml");

	//void UpdateFile( LPCWSTR lpszFileFullPathName, LPCSTR lpszResumableEditMediaUrl, LPCSTR lpszGDEtag, BOOL bCreateNewRevision, HWND hMsg, TiXmlDocument* pResponseDoc);

	return TRUE;
}



BOOL GDocsAPITest::ExportFileTest()
{
	login();

		
	TiXmlDocument * pXmlDoc = new TiXmlDocument();

	//lastest 100 items
	//m_pGDocsApi->GetEntryList("https://docs.google.com/feeds/default/private/full", pXmlDoc);
	//pXmlDoc->SaveFile("d:/entrylist.xml");

	//m_pGDocsApi->GetUserInfo();
//	TiXmlDocument * pXmlDoc = new TiXmlDocument();
//???是不是auth scope的问题，好像是 
	//export arbitrary file  semianl report
	m_pGDocsApi->ExportFile("https://doc-10-58-docs.googleusercontent.com/docs/securesc/p62krtm66k35ibqhev55ra8osq858oka/imnrkltc38mu43g22nd82l1abmnjgkgg/1313749800000/01410112657118034025/16813505762710726417/0B7Vj86DJgTM6NjIwYmY0ODAtM2QxOC00YTUwLWFmODgtOWE5NDc0NjE4MGMy?h=16653014193614665626&e=download&gd=true", "file", L"d:\\Seminar Report GDocsDrive.docx", NULL, NULL);
	
	//acknowlegdementgdocscrive.com : document  to pdf
	m_pGDocsApi->ExportFile("https://docs.google.com/feeds/download/documents/export/Export?id=1899-1kuD0D_RtEcG97z5JwKeO8k6x8xt5Y4s1z5Npv0", "document",  L"d:/bbb.pdf", NULL, "pdf");

	//spreadsheet 王建华  to csv
	m_pGDocsApi->ExportFile("https://spreadsheets.google.com/feeds/download/spreadsheets/Export?key=0ArCmbVXdOi7mdG5sSEs0Mk9yQWp5Sms0cHJNeUU0Q0E", "spreadsheet", L"d:/ccc.xls", NULL, "xls");

	int i=0;

	return TRUE;
}



BOOL GDocsAPITest::CreateFolderTest()
{
	login();
	TiXmlDocument * pXmlDoc = new TiXmlDocument();

	m_pGDocsApi->CreateFolder("abcdefg哈哈", "https://docs.google.com/feeds/default/private/full", pXmlDoc);
	
	pXmlDoc->SaveFile("abcde.xml");
	return TRUE;
}



BOOL GDocsAPITest::MoveEntryTest()
{

	login();
	//把“droptarget.zip"从 backup转移到root
	////m_pGDocsApi->MoveEntry("file:0B7CmbVXdOi7mMjJmZTljZTctMjYyMi00MzY2LTg3MWYtNmVhYmE4NDYwZTlj", 
	//	"https://docs.google.com/feeds/default/private/full/folder%3A0B7CmbVXdOi7mMzUyZGZlNGYtN2M4YS00ZjdhLWE0ZGYtMTA0Mzk1ZWEyMWJi/contents",
	//	"root");

//把droptarget.zip再从root中 COPY回Backup
	/*m_pGDocsApi->MoveEntry("file:0B7CmbVXdOi7mMjJmZTljZTctMjYyMi00MzY2LTg3MWYtNmVhYmE4NDYwZTlj", 
		"https://docs.google.com/feeds/default/private/full/folder%3Aroot/contents",
		"https://docs.google.com/feeds/default/private/full/folder%3A0B7CmbVXdOi7mMzUyZGZlNGYtN2M4YS00ZjdhLWE0ZGYtMTA0Mzk1ZWEyMWJi/contents", 
		FALSE);*/


	//把droptarget.zip从backup拷贝到beatutypage

m_pGDocsApi->MoveEntry("file:0B7CmbVXdOi7mMjJmZTljZTctMjYyMi00MzY2LTg3MWYtNmVhYmE4NDYwZTlj", 
		"https://docs.google.com/feeds/default/private/full/folder%3A0B7CmbVXdOi7mMzUyZGZlNGYtN2M4YS00ZjdhLWE0ZGYtMTA0Mzk1ZWEyMWJi/contents", 
		"https://docs.google.com/feeds/default/private/full/folder%3A0B7CmbVXdOi7mNWJiOTUxNDMtZjE2MS00NTg1LWEzNDctNmM4NzQyNDhjMDE3/contents",
		FALSE);




	//把REgoperation.exe 从aaaa移动到root
//	m_pGDocsApi->MoveEntry("0B7CmbVXdOi7mNDA5OTBlNTktMzQxYy00YmFkLThkN2UtY2M2YzhlM2NjNDVi","file","root","0B7CmbVXdOi7mYzhlM2NjODgtNzlkZC00ZGZlLWFhN2EtMWJlODA3NmY0YjIy", TRUE);
	//移到运动会芊芊
	//m_pGDocsApi->MoveEntry("0B7CmbVXdOi7mNDA5OTBlNTktMzQxYy00YmFkLThkN2UtY2M2YzhlM2NjNDVi","file","0B7CmbVXdOi7mMzRlMGRkOWMtNTUwNS00Y2UwLThjMzYtYWE2ODA0ZGQ4ZThh","root", FALSE);
	//再次复制到aaaa
	//m_pGDocsApi->MoveEntry("0B7CmbVXdOi7mNDA5OTBlNTktMzQxYy00YmFkLThkN2UtY2M2YzhlM2NjNDVi","file","0B7CmbVXdOi7mYzhlM2NjODgtNzlkZC00ZGZlLWFhN2EtMWJlODA3NmY0YjIy","0B7CmbVXdOi7mMzRlMGRkOWMtNTUwNS00Y2UwLThjMzYtYWE2ODA0ZGQ4ZThh", FALSE);

	//把0099.jpg从photo移到aabbcc .passed
	//m_pGDocsApi->MoveEntry("0B7CmbVXdOi7mOTNkYjJlZWUtZjY1OC00YmM3LWE2YWQtN2QyNDU5ZmVmZmVi", "file", "0B7CmbVXdOi7mZGRmODg1NzUtN2YyMi00ODI0LWEyOTktNDBhZDc5OTFmMTQ4","0B7CmbVXdOi7mNDk0MTgyMDMtNTA5Yy00N2ZkLWEyYjgtYjg0NTljNjk1YzBi");

	//把uptest111 从root移到Docs,这个不行 
//	m_pGDocsApi->MoveEntry("0ArCmbVXdOi7mdDVKZkszMGpzSHlLdzRZN2pROUdOaXc","spreadsheet","0B7CmbVXdOi7mN2ZlNDE1YTktNjQ0Ni00M2Y0LWJhOTctZjA0MWM1MTU1OWNi", "root");

	//把目录aabbcc移到Docs  passed
	//m_pGDocsApi->MoveEntry("0B7CmbVXdOi7mZGRmODg1NzUtN2YyMi00ODI0LWEyOTktNDBhZDc5OTFmMTQ4","folder","0B7CmbVXdOi7mN2ZlNDE1YTktNjQ0Ni00M2Y0LWJhOTctZjA0MWM1MTU1OWNi", "root");
//
	//把目录aabbcc再移出来        passed
//	m_pGDocsApi->MoveEntry("0B7CmbVXdOi7mZGRmODg1NzUtN2YyMi00ODI0LWEyOTktNDBhZDc5OTFmMTQ4","folder", "root","0B7CmbVXdOi7mN2ZlNDE1YTktNjQ0Ni00M2Y0LWJhOTctZjA0MWM1MTU1OWNi");

	//把0578从photo/芊芊中移到aabbcc. passed
	//m_pGDocsApi->MoveEntry("0B7CmbVXdOi7mNGRmMWFiOGItNDNhZS00OGI0LThmNDctYzcxMGJlZDhmZTJj","file", "root","0B7CmbVXdOi7mZGRmODg1NzUtN2YyMi00ODI0LWEyOTktNDBhZDc5OTFmMTQ4");
//
	return TRUE;
}



BOOL GDocsAPITest::CopyConstructorTest()
{
	login();
	
		GDocsEntry* ppp ;

	TiXmlDocument * pXmlDoc = new TiXmlDocument();

	m_pGDocsApi->GetEntryList("https://docs.google.com/feeds/default/private/full/", pXmlDoc);

	TiXmlElement* pFeedElement = pXmlDoc->FirstChildElement("feed");

	TiXmlElement* pElement = pFeedElement->FirstChildElement("entry");


//		pElement = pElement->NextSiblingElement("entry");


		GDocsEntry*  docsEntry= new GDocsEntry(pElement);

		{
   	
		GDocsEntry*  copyEntry = new GDocsEntry(*docsEntry);



		ppp= copyEntry;

		if ( wcscmp(copyEntry->m_pszGDEtag,  docsEntry->m_pszGDEtag) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszIDUrl, docsEntry->m_pszIDUrl) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszPublished, docsEntry->m_pszPublished) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszUpdated, docsEntry->m_pszUpdated) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}


		if (wcscmp(copyEntry->m_pszAppEdited, docsEntry->m_pszAppEdited) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (copyEntry->m_bIsStarred != docsEntry->m_bIsStarred)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (copyEntry->m_bIsHidden != docsEntry->m_bIsHidden)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (copyEntry->m_bIsViewed != docsEntry->m_bIsViewed)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszTitle, docsEntry->m_pszTitle) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszContentType, docsEntry->m_pszContentType) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszContentSrc, docsEntry->m_pszContentSrc) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (copyEntry->m_vecParents.size() != docsEntry->m_vecParents.size() )
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		vector<LinkParent>::const_iterator  iter1 	= docsEntry->m_vecParents.begin();

		for (vector<LinkParent>::const_iterator  iter2 = copyEntry->m_vecParents.begin(); iter2 != copyEntry->m_vecParents.end(); iter2++)
		{

			if (wcscmp(iter1->lpszParentHref, iter2->lpszParentHref) != 0)
				cout<< "Wrong!!!!!!"<<endl;

			if (wcscmp(iter1->lpszParentTitle, iter2->lpszParentTitle) != 0)
				cout<< "Wrong!!!!!!"<<endl;

			iter1++;
		}


		if (wcscmp(copyEntry->m_pszLinkAlternate, docsEntry->m_pszLinkAlternate) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszLinkSelf, docsEntry->m_pszLinkSelf) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszLinkEdit, docsEntry->m_pszLinkEdit) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}


		if (wcscmp(copyEntry->m_pszLinkResumableEditMedia, docsEntry->m_pszLinkResumableEditMedia) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}


		if (docsEntry->m_pszLinkThumbNail)
		{
			if (wcscmp(copyEntry->m_pszLinkThumbNail, docsEntry->m_pszLinkThumbNail) != 0)
			{
				cout<< "Wrong!!!!!!"<<endl;
			}
		}

		if (wcscmp(copyEntry->m_pszAuthorName, docsEntry->m_pszAuthorName) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}


		if (wcscmp(copyEntry->m_pszAuthorEmail, docsEntry->m_pszAuthorEmail) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}


		if (wcscmp(copyEntry->m_pszResourceId, docsEntry->m_pszResourceId) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}





		if (wcscmp(copyEntry->m_pszType, docsEntry->m_pszType) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}



		if (wcscmp(copyEntry->m_pszID, docsEntry->m_pszID) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}



		if (wcscmp(copyEntry->m_pszLastModifiedByName, docsEntry->m_pszLastModifiedByName) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}



		if (wcscmp(copyEntry->m_pszLastModifiedByEmail, docsEntry->m_pszLastModifiedByEmail) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszQuotaBytesUsed, docsEntry->m_pszQuotaBytesUsed) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszQuotaBytesUsedFormated, docsEntry->m_pszQuotaBytesUsedFormated) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}


		if ( copyEntry->m_bIsWritersCanInvite != docsEntry->m_bIsWritersCanInvite)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszFeedLinkACL, docsEntry->m_pszFeedLinkACL) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		if (wcscmp(copyEntry->m_pszFeedLinkRevisions, docsEntry->m_pszFeedLinkRevisions) != 0)
		{
			cout<< "Wrong!!!!!!"<<endl;
		}

		cout<<"done"<<endl;


		if (docsEntry->m_pszMD5Checksum)
		{
			if (wcscmp(copyEntry->m_pszMD5Checksum, docsEntry->m_pszMD5Checksum) != 0)
			{
				cout<< "Wrong!!!!!!"<<endl;
			}
		}

		delete docsEntry;

	
	}

	/*pElement = pElement->NextSiblingElement("entry");

	docsEntry(pElement);
*/

int i=0;



return TRUE;

}



BOOL GDocsAPITest::RestoreFromTrashTest()
{
	login();

	m_pGDocsApi->RestoreEntry("https://docs.google.com/feeds/default/private/full/presentation%3A0AbCmbVXdOi7mZGhianRiZHFfMTc2eHFzM2d0ZmI");


	return TRUE;

}