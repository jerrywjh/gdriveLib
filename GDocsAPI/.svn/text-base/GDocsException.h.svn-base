#pragma once
#include "stdafx.h"

///***************************************************************************
//This exception is used for Unsuported operation/converting of Google Docs, 
//especially for uploading files.
//It could be "unsupported file type/extension", "file is too big to be converted",  "conversion failed"........
//**************************************************************************/
//class GDocsUnsupportTypeException : public std::runtime_error   
//{
//public:
//	explicit GDocsUnsupportTypeException(const char*  message);
//};


/**************************************************************************
When login, sometime Cpatcha is required, 
**************************************************************************/
class CaptchaRequiredException :public std::runtime_error
{
public: 
	explicit CaptchaRequiredException(const char* message);
};

/*****************************************************************
获取刚刚upload的文件的xml信息出错 ， 这是个不是非常严重的错误，可以通过refresh来解决，但要给用户提示
****************************************************************/
class UploadedFileGetInfoException :public std::runtime_error
{
public:
	explicit UploadedFileGetInfoException(const char* message);
};