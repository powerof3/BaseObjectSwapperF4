#pragma once

#define WIN32_LEAN_AND_MEAN

#define NOMB
#define NOMINMAX
#define NOSERVICE

#pragma warning(push)
#include "F4SE/F4SE.h"
#include "RE/Fallout.h"

#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif

#include <CLibUtil/distribution.hpp>
#include <CLibUtil/numeric.hpp>
#include <CLibUtil/rng.hpp>
#include <CLibUtil/simpleINI.hpp>
#include <CLibUtil/string.hpp>
#include <CLibUtil/singleton.hpp>
#include <ankerl/unordered_dense.h>
#include <srell.hpp>
#pragma warning(pop)

#define DLLEXPORT __declspec(dllexport)

namespace logger = F4SE::log;

using namespace std::literals;
using namespace clib_util;
using namespace clib_util::singleton;
using SeedRNG = clib_util::RNG;

// for visting variants
template <class... Ts>
struct overload : Ts...
{
	using Ts::operator()...;
};

using FormIDStr = std::variant<RE::FormID, std::string>;

template <class K, class D>
using Map = ankerl::unordered_dense::map<K, D>;
template <class T>
using Set = ankerl::unordered_dense::set<T>;

using FormIDSet = Set<RE::FormID>;
using FormIDOrSet = std::variant<RE::FormID, FormIDSet>;

template <class T>
using FormIDMap = Map<RE::FormID, T>;

namespace stl
{
	using namespace F4SE::stl;

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		F4SE::AllocTrampoline(14);

		auto& trampoline = F4SE::GetTrampoline();
		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}

	template <class F, size_t index, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[index] };
		T::func = vtbl.write_vfunc(T::size, T::thunk);
	}

	template <class F, class T>
	void write_vfunc()
	{
		write_vfunc<F, 0, T>();
	}
}

namespace RE
{
	constexpr float PI = static_cast<float>(3.1415926535897932);
	constexpr float TWO_PI = 2.0F * PI;
	
	using FormID = std::uint32_t;
	using RefHandle = std::uint32_t;
	using FormType = ENUM_FORM_ID;

	inline float deg_to_rad(float a_degrees)
	{
		return a_degrees * (PI / 180.0f);
	}

	inline float rad_to_deg(float a_radians)
	{
		return a_radians * (180.0f / PI);
	}
}

#include "Util.h"
#include "Version.h"
