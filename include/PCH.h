#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMMNOSOUND

#include "RE/Starfield.h"
#include "SFSE/SFSE.h"

#include <ClibUtil/simpleINI.hpp>
#include <ClibUtil/string.hpp>
#include <ankerl/unordered_dense.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <xbyak/xbyak.h>

namespace logger = SFSE::log;
namespace string = clib_util::string;
namespace ini = clib_util::ini;

using namespace std::literals;

namespace stl
{
	void asm_replace(std::uintptr_t a_from, std::size_t a_size, std::uintptr_t a_to);

	template <class T>
	void asm_replace(std::uintptr_t a_from)
	{
		asm_replace(a_from, T::size, reinterpret_cast<std::uintptr_t>(T::func));
	}

	template <class T, class U>
	constexpr void write_vfunc(const U a_id) noexcept
		requires(std::is_same_v<U, REL::ID> || std::is_same_v<U, REL::Offset>)
	{
		static REL::Relocation vtbl{ a_id };
		T::func = vtbl.write_vfunc(T::idx, T::thunk);
	}

	template <class T, class U>
	constexpr void write_vfunc(const REL::Relocation<U> a_id) noexcept
		requires(std::is_same_v<typename REL::Relocation<U>::value_type, std::uintptr_t>)
	{
		T::func = a_id.write_vfunc(T::idx, T::thunk);
	}

	template <class To, class From>
	constexpr void write_vfunc(const std::size_t a_vtableIdx = 0) noexcept
	{
		write_vfunc<From>(To::VTABLE[a_vtableIdx]);
	}

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = REL::GetTrampoline();
		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}

	template <class T>
	void write_thunk_jump(std::uintptr_t a_src)
	{
		auto& trampoline = REL::GetTrampoline();
		T::func = trampoline.write_jmp<5>(a_src, T::thunk);
	}

	inline std::string as_string(std::string_view a_view)
	{
		return { a_view.data(), a_view.size() };
	}
}

#define DLLEXPORT extern "C" [[maybe_unused]] __declspec(dllexport)

#include "Version.h"
