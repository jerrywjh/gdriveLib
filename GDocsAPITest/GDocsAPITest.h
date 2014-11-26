#pragma once
#include "GDocsAPI.h"

class GDocsAPITest
{
public:
	GDocsAPITest(void);
	~GDocsAPITest(void);

public:
	BOOL LoginTest();    //passed
	//BOOL GetUserInfoTest();  //passed.
	BOOL GetEntryListTest();  //passed.

	BOOL FileMIMETypeTest(); //passed

	BOOL UpLoadFileTest(); //passed
	BOOL ExportFileTest();//passed
	BOOL CreateFolderTest();//美修改,不测试了
	BOOL MoveEntryTest();//passed
	BOOL DeleteEntryTest();
	BOOL RenameEntryTest();
	BOOL StarEntryTest();
	BOOL HideEntryTest();

	BOOL GetEntryACLTest();
	BOOL AddACLUserTest();
	BOOL UpdateACLUserTest();
	BOOL DeleteACLUserTest();


	BOOL UpdateFileTest();

	BOOL CopyConstructorTest();

	BOOL RestoreFromTrashTest();


private:
	BOOL login();
	GDocsAPI* m_pGDocsApi;

};
