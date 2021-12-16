// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenColorIO Project.


#ifndef INCLUDED_OCIO_IMATH_H
#define INCLUDED_OCIO_IMATH_H

#define OCIO_USE_IMATH_HALF 0

#if OCIO_USE_IMATH_HALF
#   include <Imath/half.h>
#else
#   include <OpenEXR/half.h>
#endif

#endif
