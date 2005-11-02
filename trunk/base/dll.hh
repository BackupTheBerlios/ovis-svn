#ifndef OVIS_BASE_DLL_HH_INCLUDED
#define OVIS_BASE_DLL_HH_INCLUDED

#if defined(WIN32) && !defined(ION_STATICLIB)

#ifdef ION_EXPORTS
#define OVIS_API __declspec(dllexport)
#else
#define OVIS_API __declspec(dllimport)
#endif

#else
#define OVIS_API
#endif

#endif
