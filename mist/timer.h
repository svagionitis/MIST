/// @file mist/timer.h
//!
//! @brief 経過時間を計算するクラス
//!
#ifndef __INCLUDE_MIST_TIMER__
#define __INCLUDE_MIST_TIMER__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0

#include <windows.h>
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib" )

#else

//#include <time.h>
#include <sys/time.h>

#endif


// mist名前空間の始まり
_MIST_BEGIN


/// 時間計測を行うためのクラス
//! 
//! timerクラスを生成時から時間計測を開始する．
//! 
//! @par 複数箇所で時間計測を行う場合は，以下のように記述する
//! @code タイマーオブジェクトの利用方法
//! {
//!     mist::timer t;
//! 
//!     ... 何らかの計算 ...
//! 
//!     std::cout << "計算時間: " << t << std::endl;
//! }
//! @endcode
//!
class timer
{
public:
	/// @brief 時間計測を行うためのクラスのコンストラクタ．
	//! 
	//! 生成時に，自動的にタイマを初期化する．
	//! 
	timer( )
	{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		_start_time = timeGetTime( );
#else
		gettimeofday( &_start_time, NULL );
#endif
	} // postcondition: elapsed()==0

	/// @brief 強制的に，タイマーをクリアする．
	void reset( )
	{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		_start_time = timeGetTime( );
#else
		gettimeofday( &_start_time, NULL );
#endif
	}

	/// @brief 時間計測開始時からの経過時間をミリ秒単位で返す．
	//! 
	//! @return ミリ秒単位での経過時間
	//! 
	double elapse( ) const
	{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		return( static_cast< double >( timeGetTime( ) - _start_time ) / 1000.0 );
#else
		timeval  _end_time;
		gettimeofday( &_end_time, NULL );
		return( static_cast< double >( _end_time.tv_sec - _start_time.tv_sec ) + static_cast< double >( _end_time.tv_usec - _start_time.tv_usec ) / 1000000.0 );
#endif
	}

private:
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	DWORD		_start_time;	///< @brief 時間計測開始時刻を保持する変数（Windows用）
#else
	timeval		_start_time;	///< @brief 時間計測開始時刻を保持する変数（Linux用）
#endif

};


/// @brief 経過時間を標準出力に出力する．
//! 
//! @param[in,out] out … 引数の説明
//! @param[in]     t   … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline ::std::ostream &operator <<( ::std::ostream &out, const timer &t )
{
	out << t.elapse( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_TIMER__
