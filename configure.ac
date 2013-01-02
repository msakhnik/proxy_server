AC_PREREQ(2.59)

AC_INIT([proxy_server],[1.0],[kskster@gmail.com])

AM_INIT_AUTOMAKE([1.9])

AC_PROG_CXX

AC_PROG_CPP

AC_ARG_ENABLE(unittests,
   [  --enable-unittests      build unittests],
   ENABLE_UNITTEST=$enableval)

AC_SUBST([pkgconfigdir])
AC_MSG_NOTICE([pkgconfig directory is ${pkgconfigdir}])

AC_OUTPUT([Makefile src/Makefile])
