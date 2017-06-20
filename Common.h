#ifndef _COMMON_H_
#define _COMMON_H_

#include <cmath>
#include <string>
#include <ctime>

#ifdef _WIN32
#include <direct.h>
#include <errno.h>
#define STRCMP_IGNORE_CASE(a,b) _strcmpi(a,b)
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#define STRCMP_IGNORE_CASE(a,b) strcasecmp(a,b)
#endif

using namespace std;

#include "MSReader.h"
#include "Spectrum.h"
#include "MSObject.h"
#include <vector>
#include <cfloat>

#define mango_version "2017.01 rev. 0 beta 2"
#define copyright "(c) University of Washington"

// Redefined how the bin offset is interpreted and applied.  The valid range for the offset is
// now between 0.0 and 1.0 and scales to the binWidth.
#define BIN(dMass) (int)(dMass*g_staticParams.dInverseBinWidth + g_staticParams.dOneMinusBinOffset)

#define isEqual(x, y) (std::abs(x-y) <= ( (std::abs(x) > std::abs(y) ? std::abs(y) : std::abs(x)) * FLT_EPSILON))

using namespace MSToolkit;
#define logout(szString) fputs(szString, stdout)
#define logerr(szString) fputs(szString, stderr)

#endif // _COMMON_H_
