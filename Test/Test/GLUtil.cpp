#include "GLUtil.h"

bool OutErrorMsg(const char* message)
{
	MessageBox(NULL,message,NULL,NULL);

	return false;
}