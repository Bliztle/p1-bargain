#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H
#include <stdio.h>

#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
# define __ASSERT_FUNCTION	__func__
#else
# define __ASSERT_FUNCTION	((const char *) 0)
#endif
# define __ASSERT_VOID_CAST (void)

# define assert(expr) \
    ((expr)							      \
     ? __ASSERT_VOID_CAST (0)					      \
     : __assert_fail (#expr, __FILE__, __LINE__, __ASSERT_FUNCTION))

#define __assert_fail(expr, file, line, func)				\
  do {									\
    fprintf (stderr, "%s:%u: %s: Assertion `%s' failed.",		\
             file, line, expr, func);}					\

#endif