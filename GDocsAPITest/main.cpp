// GDocsAPITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GDocsAPITest.h"

int _tmain(int argc, _TCHAR* argv[])
{

	GDocsAPITest * pAPITest = new GDocsAPITest();

	//pAPITest->LoginTest();   //passed!!!
	//pAPITest->GetEntryListTest();  //passed !!!
//	pAPITest->UpdateFileTest();
	//	pAPITest->GetEntryListTest();  //passed !!!
	//pAPITest->FileMIMETypeTest();  //passed
	//pAPITest->UpLoadFileTest();   //passed, �μ�ProgressMsgTest��Ŀ
	//pAPITest->MoveEntryTest();
//	pAPITest->CreateFolderTest();


	pAPITest->RestoreFromTrashTest();

	//pAPITest->CopyConstructorTest();


	return 0;
}

