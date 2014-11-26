#include "StdAfx.h"
#include "GDocsException.h"

//constructor 
//GDocsUnsupportTypeException::GDocsUnsupportTypeException(const char*  message)
//	:std::runtime_error(message)
//{
//}


CaptchaRequiredException::CaptchaRequiredException(const char* message)
	:std::runtime_error(message)
{
}

UploadedFileGetInfoException::UploadedFileGetInfoException(const char *message)
	:std::runtime_error(message)
{
}
