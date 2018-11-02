/*=============================================================================
   Copyright (c) 2016-2018 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(INFRA_SUPPORT_APRIL_10_2016)
#define INFRA_SUPPORT_APRIL_10_2016

#include <cfloat>
#include <cstdint>
#include <cassert>
#include <chrono>

namespace cycfi
{
   ////////////////////////////////////////////////////////////////////////////
   // Time
   ////////////////////////////////////////////////////////////////////////////
   using duration       = std::chrono::duration<double>;
   using microseconds   = std::chrono::duration<double, std::micro>;
   using milliseconds   = std::chrono::duration<double, std::milli>;
   using seconds        = std::chrono::duration<double>;
   using minutes        = std::chrono::duration<double, std::ratio<60>>;
   using hours          = std::chrono::duration<double, std::ratio<60*60>>;
   using time_point     = std::chrono::time_point<std::chrono::steady_clock, duration>;

   ////////////////////////////////////////////////////////////////////////////
   // Metaprogramming utilities
   ////////////////////////////////////////////////////////////////////////////
   template <typename T, typename... Rest>
   struct is_arithmetic
   {
      static constexpr bool value
         = std::is_arithmetic<T>::value && is_arithmetic<Rest...>::value;
   };

   template <typename T>
   struct is_arithmetic<T>
   {
      static constexpr bool value = std::is_arithmetic<T>::value;
   };

   namespace detail
   {
      template <uint32_t bits>
      struct int_that_fits_impl { using type = void; };

      template <>
      struct int_that_fits_impl<8> { using type = std::int8_t; };

      template <>
      struct int_that_fits_impl<16> { using type = std::int16_t; };

      template <>
      struct int_that_fits_impl<32> { using type = std::int32_t; };

      template <>
      struct int_that_fits_impl<64> { using type = std::int64_t; };

      template <uint32_t bits>
      struct uint_that_fits_impl { using type = void; };

      template <>
      struct uint_that_fits_impl<8> { using type = std::uint8_t; };

      template <>
      struct uint_that_fits_impl<16> { using type = std::uint16_t; };

      template <>
      struct uint_that_fits_impl<32> { using type = std::uint32_t; };

      template <>
      struct uint_that_fits_impl<64> { using type = uint64_t; };

      constexpr std::uint32_t size_that_fits_int(uint32_t bits)
      {
         if (bits <= 8)
            return 8;
         else if (bits <= 16)
            return 16;
         else if (bits <= 32)
            return 32;
         return 0;
      }
   }

   template <uint32_t bits>
   struct int_that_fits
     : detail::int_that_fits_impl<detail::size_that_fits_int(bits)>
   {
      static_assert(std::is_same<typename int_that_fits<bits>::type, void>::value,
         "Error: No int type fits specified number of bits."
      );
   };

   template <uint32_t bits>
   struct uint_that_fits
     : detail::uint_that_fits_impl<detail::size_that_fits_int(bits)>
   {
      static_assert(std::is_same<typename uint_that_fits<bits>::type, void>::value,
         "Error: No int type fits specified number of bits."
      );
   };

   ////////////////////////////////////////////////////////////////////////////
   // Constants
	////////////////////////////////////////////////////////////////////////////
   template <typename T>
   struct int_traits;

   template <>
   struct int_traits<uint32_t>
   {
      static constexpr std::uint32_t max = 4294967295;
      static constexpr std::uint32_t min = 0;
   };

   template <>
   struct int_traits<int32_t>
   {
      static constexpr int32_t max = 2147483647;
      static constexpr int32_t min = -2147483648;
   };

   template <>
   struct int_traits<uint16_t>
   {
      static constexpr uint16_t max = 65535;
      static constexpr uint16_t min = 0;
   };

   template <>
   struct int_traits<int16_t>
   {
      static constexpr int16_t max = 32767;
      static constexpr int16_t min = -32768;
   };

   template <>
   struct int_traits<uint8_t>
   {
      static constexpr uint8_t max = 255;
      static constexpr uint8_t min = 0;
   };

   template <>
   struct int_traits<int8_t>
   {
      static constexpr uint8_t max = 127;
      static constexpr uint8_t min = -128;
   };

   template <typename T>
   constexpr T int_max()
   {
      return int_traits<T>::max;
   }

   template <typename T>
   constexpr T int_min()
   {
      return int_traits<T>::min;
   }

   ////////////////////////////////////////////////////////////////////////////
   // integer and binary functions
   ////////////////////////////////////////////////////////////////////////////
   constexpr int16_t promote(int8_t i)
   {
      return i;
   }

   constexpr uint16_t promote(uint8_t i)
   {
      return i;
   }

   constexpr int32_t promote(int16_t i)
   {
      return i;
   }

   constexpr std::uint32_t promote(uint16_t i)
   {
      return i;
   }

   constexpr int64_t promote(int32_t i)
   {
      return i;
   }

   constexpr uint64_t promote(uint32_t i)
   {
      return i;
   }

   constexpr float promote(float i)
   {
      return i;
   }

   constexpr double promote(double i)
   {
      return i;
   }

   template <typename T>
   constexpr T pow2(std::size_t n)
   {
      return (n == 0)? T(1) : T(2) * pow2<T>(n-1);
   }

   // This is needed to force compile-time evaluation
   template <typename T, size_t n>
   struct static_pow2
   {
      constexpr static T val = pow2<T>(n);
   };

   // smallest power of 2 that fits n
   template <typename T>
   constexpr T smallest_pow2(T n, T m = 1)
   {
      return (m < n)? smallest_pow2(n, m << 1) : m;
   }

   template <typename T>
   constexpr bool is_pow2(T n)
   {
      return (n & (n - 1)) == 0;
   }

   ////////////////////////////////////////////////////////////////////////////
   // static int types
	////////////////////////////////////////////////////////////////////////////
   template <typename T, T value_>
   struct static_int
   {
      static constexpr T value =  value_;
   };

   template <int i>
   using int_ = static_int<int, i>;

   template <std::size_t i>
   using uint_ = static_int<std::size_t, i>;

   template <int8_t i>
   using int8_ = static_int<int8_t, i>;

   template <uint8_t i>
   using uint8_ = static_int<uint8_t, i>;

   template <int16_t i>
   using int16_ = static_int<int16_t, i>;

   template <uint16_t i>
   using uint16_ = static_int<uint16_t, i>;

   template <int32_t i>
   using int32_ = static_int<int32_t, i>;

   template <uint32_t i>
   using uint32_ = static_int<uint32_t, i>;

   template <int64_t i>
   using int64_ = static_int<int64_t, i>;

   template <uint64_t i>
   using uint64_ = static_int<uint64_t, i>;
}
#endif
