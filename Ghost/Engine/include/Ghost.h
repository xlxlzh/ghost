#ifndef _GHOST_H_

#define _GHOST_H_

#ifdef GHOST_LIB_STATIC
#define GHOST_API
#else
#ifdef GHOST_EXPORT
#define GHOST_API __declspec(dllexport)
#else
#define GHOST_API __declspec(dllimport)
#endif
#endif

#endif // !_GHOST_H_
