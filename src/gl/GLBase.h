#pragma once

#include "../Base.h"
#include "ContextBase.h"
#include "GLIncludes.h"

class IGLObject
{
	IGLObject();

protected:
	std::shared_ptr<ContextBase> glContext;

	IGLObject(std::shared_ptr<ContextBase> context);
};

//------------------------------------------------------------------------------------------------------

class GLException : public InternalException
{
public:
	GLException(const char * errorString) throw();
	GLException(const std::string & errorString) throw();
};