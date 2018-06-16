#define NDEBUG
#ifndef NDEBUG
#   define ASSERT(condition, message) \
	do { \
	if (! (condition)) { \
	std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
	<< " line " << __LINE__ << ": " << message << std::endl; \
	std::terminate(); \
	} \
	} while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif


#ifndef NDEBUG
#define GET_GL_ERROR(message) \
	do { \
	auto err = glGetError(); \
	while(err != GL_NO_ERROR) { \
	std::cerr << message << std::endl;\
	std::cerr << "GL Error found with error code " << err << std::endl; \
	}\
	} while (false)
#else
#define GL_GET_ERROR(message) do {} while (false)
#endif


/*
namespace debug
{
	static void getLastGLError(const std::string& message)
	{
		auto err = glGetError();
		if(err != GL_NO_ERROR) {
			cerr << message << endl;
			cerr << "GL Error found with error code " << endl;
		}
	}
};
*/
