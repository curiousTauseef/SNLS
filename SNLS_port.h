#ifndef SNLS_port_h__
#define SNLS_port_h__

#if HAVE_MSLIB

#include "MS_port.h"
#include "MS_Log.h"
#ifdef __cuda_host_only__
#define SNLS_FAIL(loc,str) MS_Fail(loc,str);
#endif

#else
/* HAVE_MSLIB */

/*
 * used to do "typedef double double", but have switched to just using double
 */

#ifdef __cuda_host_only__
#include <stdio.h>
#include <exception>
#include <stdexcept>
#define SNLS_FAIL(loc,str) throw std::runtime_error(std::string("at ") + std::string(loc) + std::string(" failure : ") + std::string(str)) ;
#else
#define SNLS_FAIL(loc,str) printf("ERROR : SNLS failure in %s : %s\n",loc,str) ;
#endif

#endif
/* HAVE_MSLIB */

#if defined(_WIN32) && __INTEL_COMPILER
#include <mathimf.h>
#else
#include <math.h>
#endif

#ifdef WIN32
#define not !
#endif

#endif
/* SNLS_port_h__ */
