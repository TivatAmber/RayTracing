//
// Created by BYU on 2023/5/12.
//

#ifndef RAYTRACING_RTW_STB_IMAGE_H
#define RAYTRACING_RTW_STB_IMAGE_H

// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
    #pragma warning (push, 0)
#endif

#define STBI_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

// Restore warning levels.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
    #pragma warning (pop)
#endif

#endif //RAYTRACING_RTW_STB_IMAGE_H
