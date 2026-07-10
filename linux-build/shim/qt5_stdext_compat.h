// qt5_stdext_compat.h
//
// Build-time compatibility shim for the CKPE msvc-wine cross-build. NOT part of
// the Windows/MSBuild build and never included by project sources — it is
// force-included (/FI) only on the CKPE.Starfield target via linux-build/CMakeLists.txt.
//
// Qt 5.12's qcompilerdetection.h unconditionally defines, under _MSC_VER:
//     QT_MAKE_CHECKED_ARRAY_ITERATOR(x, N) -> stdext::make_checked_array_iterator(x, size_t(N))
//     QT_MAKE_UNCHECKED_ARRAY_ITERATOR(x)  -> stdext::make_unchecked_array_iterator(x)
// The Microsoft STL removed these long-deprecated stdext helpers (present through
// _MSVC_STL_UPDATE 202503L, gone by 202604L / MSVC 14.51), so real cl.exe from a
// recent toolset can't compile those Qt headers.
//
// This shim restores just those two factory functions, returning the raw pointer
// exactly as Qt's own non-MSVC fallback does (QT_MAKE_CHECKED_ARRAY_ITERATOR(x,N)
// -> (x)). It is gated on the STL version so that on older toolsets that still
// ship the real stdext helpers this header defines nothing and cannot clash.

#pragma once

// Pull in _MSVC_STL_UPDATE before testing it.
#include <cstddef>

#if defined(_MSVC_STL_UPDATE) && _MSVC_STL_UPDATE >= 202604L

namespace stdext {

    template <class T>
    inline T* make_checked_array_iterator(T* ptr, std::size_t /*size*/, std::size_t /*index*/ = 0) noexcept {
        return ptr;
    }

    template <class T>
    inline T* make_unchecked_array_iterator(T* ptr) noexcept {
        return ptr;
    }

} // namespace stdext

#endif // _MSVC_STL_UPDATE >= 202604L
