#pragma once

#if defined(BOT_LIB)
# define BOT_EXPORT __declspec(dllexport)
#else
# define BOT_EXPORT __declspec(dllimport)
#endif
