/// @file mist/limits.h
//!
//! @brief 各型に対する情報を取得するためのライブラリ
//!
#ifndef __INCLUDE_MIST_LIMITS__
#define __INCLUDE_MIST_LIMITS__

#include <limits.h>
#include <float.h>


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

// mist名前空間の始まり
_MIST_BEGIN


// numeric_limitsの特殊化用マクロ
#define _DEFINE_TYPE_LIMITS( _type_, _integer_, _signed_, _min_, _max_, _zero_ ) \
	template < >\
	struct type_limits< _type_ >\
	{\
		typedef _type_ value_type;\
		\
		enum{ is_integer = _integer_ };\
		enum{ is_signed  = _signed_ };\
		\
		static value_type minimum( ) { return(  _min_ ); }\
		static value_type maximum( ) { return(  _max_ ); }\
		static value_type zero( ){ return( _zero_ ); }\
	};\


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T >
struct type_limits
{
	typedef T value_type;	///< @typedef 情報を調べる対象となる型

	/// @enum 指定された型が整数かどうか
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	enum{ is_integer = false };

	/// @enum 指定された型が符号付かどうか
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	enum{ is_signed  = false };


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	static value_type minimum( ) { return( 0 ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	static value_type maximum( ) { return( 1 ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	static value_type zero( ){ return( 0 ); }
};


_DEFINE_TYPE_LIMITS(          char,  true,  true,  CHAR_MIN,  CHAR_MAX, 0x00 )
_DEFINE_TYPE_LIMITS( signed   char,  true,  true, SCHAR_MIN, SCHAR_MAX, 0x00 )
_DEFINE_TYPE_LIMITS( unsigned char,  true, false,      0x00, UCHAR_MAX, 0x00 )

_DEFINE_TYPE_LIMITS( signed   short, true,  true,  SHRT_MIN,  SHRT_MAX, 0x0000 )
_DEFINE_TYPE_LIMITS( unsigned short, true, false,    0x0000, USHRT_MAX, 0x0000 )

_DEFINE_TYPE_LIMITS( signed   int,   true,  true,   INT_MIN,   INT_MAX, 0 )
_DEFINE_TYPE_LIMITS( unsigned int,   true, false,         0,  UINT_MAX, 0 )

_DEFINE_TYPE_LIMITS( signed   long,  true,  true,  LONG_MIN,  LONG_MAX, 0 )
_DEFINE_TYPE_LIMITS( unsigned long,  true, false,         0, ULONG_MAX, 0 )

_DEFINE_TYPE_LIMITS(       float,   false,  true,   FLT_MIN,   FLT_MAX, 0.0f )
_DEFINE_TYPE_LIMITS(      double,   false,  true,   DBL_MIN,   DBL_MAX, 0 )
_DEFINE_TYPE_LIMITS( long double,   false,  true,  LDBL_MIN,  LDBL_MAX, 0 )


#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define _DEFINE_COLOR_TYPE_LIMITS( type ) \
	template < >\
	struct type_limits< rgb< type > >\
	{\
		typedef rgb< type > value_type;\
		typedef rgb< type >::value_type rgb_value_type;\
		\
		enum{ is_integer = type_limits< rgb_value_type >::is_integer };\
		enum{ is_signed  = type_limits< rgb_value_type >::is_signed };\
		\
		static value_type minimum( ) { return( rgb< type >( type_limits< rgb_value_type >::minimum( ) ) ); }\
		static value_type maximum( ) { return( rgb< type >( type_limits< rgb_value_type >::maximum( ) ) ); }\
		static value_type zero( ) { return( rgb< type >( type_limits< rgb_value_type >::zero( ) ) ); }\
	};\

	// 各型に対する特殊化
	_DEFINE_COLOR_TYPE_LIMITS(unsigned char)
	_DEFINE_COLOR_TYPE_LIMITS(unsigned short)
	_DEFINE_COLOR_TYPE_LIMITS(unsigned int)
	_DEFINE_COLOR_TYPE_LIMITS(unsigned long)
	_DEFINE_COLOR_TYPE_LIMITS(signed char)
	_DEFINE_COLOR_TYPE_LIMITS(signed short)
	_DEFINE_COLOR_TYPE_LIMITS(signed int)
	_DEFINE_COLOR_TYPE_LIMITS(signed long)
	_DEFINE_COLOR_TYPE_LIMITS(bool)
	_DEFINE_COLOR_TYPE_LIMITS(char)
	_DEFINE_COLOR_TYPE_LIMITS(float)
	_DEFINE_COLOR_TYPE_LIMITS(double)
	_DEFINE_COLOR_TYPE_LIMITS(long double)

	#undef _DEFINE_COLOR_TYPE_LIMITS

#else

	template < class T >
	struct type_limits< rgb< T > >
	{
		typedef rgb< T > value_type;
		typedef typename rgb< T >::value_type rgb_value_type;

		enum{ is_integer = type_limits< rgb_value_type >::is_integer };
		enum{ is_signed  = type_limits< rgb_value_type >::is_signed };

		static value_type minimum( ) { return( rgb< T >( type_limits< rgb_value_type >::minimum( ) ) ); }
		static value_type maximum( ) { return( rgb< T >( type_limits< rgb_value_type >::maximum( ) ) ); }
		static value_type zero( ) { return( rgb< T >( type_limits< rgb_value_type >::zero( ) ) ); }
	};

#endif


#undef _DEFINE_TYPE_LIMITS


namespace __limits_0_255__
{
	template < bool _ISCHAR_ >
	struct limits_0_255__
	{
		template < class T >
		static const T limits( const T &v )
		{
			return( v );
		}
	};

	template < >
	struct limits_0_255__< false >
	{
		template < class T >
		static const T limits( const T &v )
		{
			return( v < 0 ? 0 : ( v > 255 ? 255 : v ) );
		}
	};

	template < bool _ISCOLOR_ >
	struct limits_0_255
	{
		template < class T >
		static const T limits( const T &v )
		{
			return( limits_0_255__< is_char< T >::value >::limits( v ) );
		}
	};

	template < >
	struct limits_0_255< true >
	{
		template < class T >
		static const rgb< T > limits( const rgb< T > &v )
		{
			typedef limits_0_255__< is_char< T >::value > limits_0_255_;
			return( rgb< T >( limits_0_255_::limits( v.r ), limits_0_255_::limits( v.g ), limits_0_255_::limits( v.b ) ) );
		}
	};
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T >
inline const T limits_0_255( const T &v )
{
	return( __limits_0_255__::limits_0_255< is_color< T >::value >::limits( v ) );
}


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_LIMITS__
