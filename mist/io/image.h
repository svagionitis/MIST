/// @file mist/io/image.h
//!
//! @brief BMP，PNG，PNM，TIFF，JPEGの画像を読み書きするためのライブラリ
//!
#ifndef __INCLUDE_MIST_IMAGE__
#define __INCLUDE_MIST_IMAGE__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_JPEG__
#include "jpeg.h"
#endif

#ifndef __INCLUDE_MIST_BMP__
#include "bmp.h"
#endif

#ifndef __INCLUDE_MIST_PNG__
#include "png.h"
#endif

#if defined( _MIST_GIF_SUPPORT_ ) && _MIST_GIF_SUPPORT_ != 0 && !defined( __INCLUDE_MIST_GIF__ )
#include "gif.h"
#endif

#ifndef __INCLUDE_MIST_TIFF__
#include "tiff.h"
#endif

#ifndef __INCLUDE_MIST_PNM__
#include "pnm.h"
#endif

#ifndef __INCLUDE_MIST_TGA__
#include "tga.h"
#endif

#include <iostream>
#include <string>


// mist名前空間の始まり
_MIST_BEGIN


namespace __image_controller__
{
	inline std::string to_lower_case( const std::string &str )
	{
		std::string s = "";
		for( std::string::size_type i = 0 ; i < str.size( ) ; i++ )
		{
			s += static_cast< char >( tolower( str[ i ] ) );
		}
		return( s );
	}

	inline std::string get_ext( const std::string &str )
	{
		std::string::size_type index = str.rfind( '.' );
		if( index == str.npos )
		{
			return( "" );
		}
		return( str.substr( index ) );
	}
}


//! @addtogroup image_group 画像入出力
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/image.h>
//! @endcode
//!
//!  @{



/// @brief JPEG，TIFF，PNG，BMP，PNM，GIF 画像を拡張子に基づいてMISTコンテナに読み込む
//! 
//! @attention 拡張子に基づいて画像形式を判別するため，正しいファイル拡張子が付いている必要あり
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
inline bool read_image( mist::array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename mist::array2< T, Allocator >::size_type size_type;

	std::string ext = __image_controller__::to_lower_case( __image_controller__::get_ext( filename ) );
	if( ext == "" )
	{
		return( false );
	}

	bool ret = false;
	if( ext == ".jpeg" || ext == ".jpg" )
	{
		ret = mist::read_jpeg( image, filename );
	}
	else if( ext == ".bmp" )
	{
		ret = mist::read_bmp( image, filename );
	}
	else if( ext == ".tiff" || ext == ".tif" )
	{
		ret = mist::read_tiff( image, filename );
	}
	else if( ext == ".png" )
	{
		ret = mist::read_png( image, filename );
	}
	else if( ext == ".pbm" || ext == ".pgm" || ext == ".ppm" || ext == ".pnm" )
	{
		ret = mist::read_pnm( image, filename );
	}
	else if( ext == ".tga" )
	{
		ret = mist::read_tga( image, filename );
	}
#if defined( _MIST_GIF_SUPPORT_ ) && _MIST_GIF_SUPPORT_ != 0
	else if( ext == ".gif" )
	{
		ret = mist::read_gif( image, filename );
	}
#endif
	else
	{
		ret = false;
	}
	return( ret );
}


/// @brief MISTコンテナの画像をTIFF形式でJPEG，TIFF，PNG，BMP，PNM，GIF 画像形式でファイルに出力する
//! 
//! @attention 拡張子に基づいて画像形式を判別するため，正しいファイル拡張子が付いている必要あり
//! @attention 出力されるファイルの圧縮率は，各 read_jpeg 等のデフォルト値が用いられる
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
inline bool write_image( mist::array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename mist::array2< T, Allocator >::size_type size_type;

	std::string ext = __image_controller__::to_lower_case( __image_controller__::get_ext( filename ) );
	if( ext == "" )
	{
		return( false );
	}

	bool ret = false;
	if( ext == ".jpeg" || ext == ".jpg" )
	{
		ret = mist::write_jpeg( image, filename );
	}
	else if( ext == ".bmp" )
	{
		ret = mist::write_bmp( image, filename );
	}
	else if( ext == ".tiff" || ext == ".tif" )
	{
		ret = mist::write_tiff( image, filename );
	}
	else if( ext == ".png" )
	{
		ret = mist::write_png( image, filename );
	}
	else if( ext == ".pbm" )
	{
		ret = mist::write_pnm( image, filename, 1 );
	}
	else if( ext == ".pgm" )
	{
		ret = mist::write_pnm( image, filename, 2 );
	}
	else if( ext == ".ppm" || ext == ".pnm" )
	{
		ret = mist::write_pnm( image, filename, 3 );
	}
	else if( ext == ".tga" )
	{
		ret = mist::write_tga( image, filename );
	}
#if defined( _MIST_GIF_SUPPORT_ ) && _MIST_GIF_SUPPORT_ != 0
	else if( ext == ".gif" )
	{
		ret = mist::write_gif( image, filename );
	}
#endif
	else
	{
		ret = false;
	}
	return( ret );
}


/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_IMAGE__
