#ifndef MINSC_SUPPORT_WRAP_CTYPE_H_
#define MINSC_SUPPORT_WRAP_CTYPE_H_

#include <ctype.h>
#include <stdbool.h>

#define wrap_isalnum(c) ((bool)isalnum(c))
#define wrap_isalpha(c) ((bool)isalpha(c))
#define wrap_isblank(c) ((bool)isblank(c))
#define wrap_iscntrl(c) ((bool)iscntrl(c))
#define wrap_isdigit(c) ((bool)isdigit(c))
#define wrap_isgraph(c) ((bool)isgraph(c))
#define wrap_islower(c) ((bool)islower(c))
#define wrap_isprint(c) ((bool)isprint(c))
#define wrap_ispunct(c) ((bool)ispunct(c))
#define wrap_isspace(c) ((bool)isspace(c))
#define wrap_isupper(c) ((bool)isupper(c))
#define wrap_isxdigit(c) ((bool)isxdigit(c))
#define wrap_tolower(c) ((int)tolower(c))
#define wrap_toupper(c) ((int)toupper(c))

#endif  // MINSC_SUPPORT_WRAP_CTYPE_H_
