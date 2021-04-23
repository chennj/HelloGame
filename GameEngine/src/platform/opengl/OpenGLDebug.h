#pragma once

namespace SOMEENGINE
{
	#define ASSERT(x) if(!(x)) __debugbreak();
	#if 1
	#define GLCall(x) x;
	#else
		#define GLCall(x) GLClearError();\
			x;\
			ASSERT(GLLogCall(#x, __FILE__, __LINE__))
	#endif
	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
}