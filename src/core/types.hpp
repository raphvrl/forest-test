#pragma once

#include <cstdint>
#include <string>
#include <limits>

#define UNUSED(x) (void)(x)

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

using usize = std::size_t;

constexpr i8 I8_MIN = std::numeric_limits<i8>::min();
constexpr i8 I8_MAX = std::numeric_limits<i8>::max();
constexpr i16 I16_MIN = std::numeric_limits<i16>::min();
constexpr i16 I16_MAX = std::numeric_limits<i16>::max();
constexpr i32 I32_MIN = std::numeric_limits<i32>::min();
constexpr i32 I32_MAX = std::numeric_limits<i32>::max();
constexpr i64 I64_MIN = std::numeric_limits<i64>::min();
constexpr i64 I64_MAX = std::numeric_limits<i64>::max();

constexpr u8 U8_MAX = std::numeric_limits<u8>::max();
constexpr u16 U16_MAX = std::numeric_limits<u16>::max();
constexpr u32 U32_MAX = std::numeric_limits<u32>::max();
constexpr u64 U64_MAX = std::numeric_limits<u64>::max();

constexpr f64 F64_MIN = std::numeric_limits<f64>::lowest();
constexpr f64 F64_MAX = std::numeric_limits<f64>::max();
constexpr f64 F64_EPSILON = std::numeric_limits<f64>::epsilon();

constexpr f32 F32_MIN = std::numeric_limits<f32>::lowest();
constexpr f32 F32_MAX = std::numeric_limits<f32>::max();
constexpr f32 F32_EPSILON = std::numeric_limits<f32>::epsilon();

constexpr usize USIZE_MIN = std::numeric_limits<usize>::min();
constexpr usize USIZE_MAX = std::numeric_limits<usize>::max();

