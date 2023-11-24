#pragma once

#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "PlatformDetection.h"

#define BIT(x) (1 << x)

namespace TrashEngine {


#ifdef NS_PLATFORM_WINDOWS
#ifdef NS_BUILD_SHARED
#ifdef NS_BUILD_ENGINE
#define NS_API __declspec(dllexport)
#else
#define NS_API __declspec(dllimport)
#endif
#elif NS_BUILD_STATIC
#define NS_API
#else
#define NS_API
#endif

#elif defined(NS_PLATFORM_LINUX)
#ifdef NS_BUILD_DLL
#ifdef NS_BUILD_ENGINE
#define NS_API __attribute((visibility("default")))
#else
#define NS_API
#endif
#elif defined(NS_BUILD_STATIC)
#define NS_API
#else
#define NS_API
#endif
#endif

	// unique_ptr
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// ??????ï¼?é¡?ä¼¼const CLASS &a = b
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<class T, class U>
	Ref<T> StaticCastRef(const Ref<U>& r) noexcept
	{
		auto p = static_cast<typename Ref<T>::element_type*>(r.get());
		return Ref<T>{r, p};
	}

	// All "common" platforms use the same size for char, short and int
	// (basically there are 3 types for 3 sizes, so no other match is possible),
	// we can use them without doing any kind of check

	// 8 bits integer types
	typedef int8_t Int8;
	typedef uint8_t Uint8;

	// 16 bits integer types
	typedef int16_t Int16;
	typedef uint16_t Uint16;

	// 32 bits integer types
	typedef int32_t Int32;
	typedef uint32_t Uint32;

	// 64 bits integer types
#if defined(_MSC_VER)
	typedef signed   __int64 Int64;
	typedef unsigned __int64 Uint64;
#else
	typedef signed   long long Int64;
	typedef unsigned long long Uint64;
#endif

	// result value
	template <typename T>
	struct ResultValue
	{
		ResultValue(bool r, T& v) : result(r), value(v)
		{
		}
		ResultValue(bool r, T&& v) : result(r), value(std::move(v))
		{
		}
		bool result;
		T value;

	};


}


