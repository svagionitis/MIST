/****************************************************************************************************************************
**       MIST ( Media Integration Standard Toolkit )
**
**        We defined following template classes.
**
**          array  : template class of one dimensionl array with STL support.
**          array1 : template class of one dimensionl array containing resolution.
**          array2 : template class of two dimensionl array containing pixel resolution.
**          array3 : template class of three dimensionl array containing voxel resolution.
**          matrix : template class of matrix with its operation, and impremented using expression template teqnique.
**          vector : template class of vector with its operation, and impremented using expression template teqnique.
**
**
**         We developed these programs since 2003/09/05.
**
**             $LastChangedDate$
**             $LastChangedRevision$
**             $LastChangedBy$
**             $HeadURL$
**
**
**              Copyright MIST Project Team.
**                    All Rights Reserved.
**
****************************************************************************************************************************/



/// @file mist/mist.h
//!
//! @brief MISTの基本となる音声・画像を扱うコンテナ
//!
//! MISTで提供する全てのアルゴリズム等で利用するコンテナ（STL風）の集まりです．
//! 基本的には，STL（Standard Template Library）で提供されているコンテナとほぼ互換性を持っています．
//! 特徴としては，コンテナ内のデータはメモリ空間上で連続となるように実装されています．
//! そして，STLのアルゴリズム等で用いるイテレータの内，ランダムアクセスイテレータを実装してあります．
//! イテレータの詳細に関しては，STLの解説本に譲ります．
//! また，MISTのコンテナでは組み込み型とユーザー定義型で，利用するメモリ操作を区別しています．
//! そうすることで，コンテナの代入等で若干高速な実行が可能となっています．
//!
//! MISTプロジェクトで提供するコンテナは，以下の合計6つのコンテナで構成されています．
//!
//! - 基底1次元配列（STL風の1次元配列用コンテナ）
//! - 解像度付1次元配列（各要素の解像度のデータを保持，例えば1mmなど）
//! - 解像度付2次元配列
//! - 解像度付3次元配列
//! - 行列
//! - ベクトル
//!

#ifndef __INCLUDE_MIST_H__
#define __INCLUDE_MIST_H__

#include "config/mist_conf.h"
#include "config/mist_alloc.h"
#include "config/iterator.h"


// mist名前空間の始まり
_MIST_BEGIN



/// @brief 要素数が可変の1次元配列
//! 
//! 全てのMISTコンテナの基本となるクラス
//! 
//! @param T         … MISTのコンテナ内に格納するデータ型
//! @param Allocator … MISTコンテナが利用するアロケータ型．省略した場合は，STLのデフォルトアロケータを使用する
//! 
template < class T, class Allocator = std::allocator< T > >
class array
{
public:
	typedef Allocator allocator_type;								///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	/// @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef mist_iterator1< T, ptrdiff_t, pointer, const_reference > const_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;

private:
	mist_allocator< T, Allocator > allocator_;		///< @brief 各コンテナで利用するアロケータオブジェクト
	size_type size_;								///< @brief コンテナに格納されている要素数

protected:
	T* data_;	///< @brief コンテナで扱うデータの先頭を表すポインタ

public:
	/// @brief コンテナ内の要素数を変更する
	//! 
	//! コンテナのサイズを num 個に変更する．
	//! 元のコンテナサイズと異なる場合は，自動的にサイズを調整する．
	//! 全ての要素の値はデフォルトコンストラクタで初期化される．
	//! また，組み込み型（int や double など）の場合は，全ての要素を0で初期化する．
	//! 
	//! @param[in] num  … リサイズ後のコンテナ内の要素数
	//! 
	void resize( size_type num )
	{
		if( size_ < num )
		{
			allocator_.deallocate_objects( data_, size_ );
			size_ = num;
			data_ = allocator_.allocate_objects( size_ );
		}
		else if( size_ > num )
		{
			data_ = allocator_.trim_objects( data_, size_, num );
			size_ = num;
		}
	}


	/// @brief コンテナ内の要素数を変更する
	//! 
	//! 要素数を num 個に変更し，全ての要素を値 val で初期化する．
	//! 
	//! @param[in] num … リサイズ後のコンテナ内の要素数
	//! @param[in] val … リサイズ後に各要素を初期化する値
	//! 
	void resize( size_type num, const value_type &val )
	{
		if( size_ < num )
		{
			allocator_.deallocate_objects( data_, size_ );
			size_ = num;
			data_ = allocator_.allocate_objects( size_, val );
		}
		else if( size_ > num )
		{
			data_ = allocator_.trim_objects( data_, size_, num, val );
			size_ = num;
		}
	}


	/// @brief コンテナ内の全ての内容を入れ替える．
	//! 
	//! 入れ替え元のコンテナ a の中身と全て入れ替える
	//! 
	//! @param[in] a  … 内容を入れ替える対象
	//! 
	void swap( array &a )
	{
		size_type _dmy_size = size_;
		size_ = a.size_;
		a.size_ = _dmy_size;

		value_type *dmy_data_ = data_;
		data_ = a.data_;
		a.data_ = dmy_data_;
	}


	/// @brief コンテナの要素を空にする
	//! 
	//! コンテナに格納されているデータを全て削除し，コンテナを空（要素数0）にする
	//! 
	void clear( )
	{
		allocator_.deallocate_objects( data_, size_ );
		size_ = 0;
		data_ = NULL;
	}


	/// @brief コンテナ内のデータ要素をデフォルト値で初期化する
	//! 
	//! デフォルトコンストラクタの値で初期化する．
	//! 組み込み型（int や double など）の場合は，全ての要素を0で初期化する．
	//! 
	void fill( )
	{
		allocator_.fill_objects( data_, size_ );
	}


	/// @brief コンテナ内のデータ要素を指定された値で初期化する
	//! 
	//! 全ての要素を値 val で初期化する．
	//! 
	//! @param[in] val … 要素を初期化する値
	//! 
	void fill( const value_type &val )
	{
		allocator_.fill_objects( data_, size_, val );
	}


	/// @brief コンテナが空かどうかを判定
	//! 
	//! @retval true  … コンテナの要素数が空（0）の場合
	//! @retval false … コンテナの要素数が空（0）でない場合
	//! 
	bool empty( ) const { return( size_ == 0 ); }


	size_type size( ) const { return( size_ ); }	///< @brief コンテナに格納されているデータ数を返す
	size_type size1( ) const { return( size_ ); }	///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type size2( ) const { return( 1 ); }		//< @brief Y軸方向のコンテナに格納されているデータ数を返す（常に1を返す）
	size_type size3( ) const { return( 1 ); }		//< @brief Z軸方向のコンテナに格納されているデータ数を返す（常に1を返す）
	size_type width( ) const { return( size_ ); }	///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type height( ) const { return( 1 ); }		//< @brief Y軸方向のコンテナに格納されているデータ数を返す（常に1を返す）
	size_type depth( ) const { return( 1 ); }		//< @brief Z軸方向のコンテナに格納されているデータ数を返す（常に1を返す）

	double reso1( double r1 ){ return( 1.0 ); }		//< @brief X軸方向の解像度を返す
	double reso1( ) const { return( 1.0 ); }		//< @brief X軸方向の解像度を設定する
	double reso2( double r2 ){ return( 1.0 ); }		//< @brief Y軸方向の解像度を返す
	double reso2( ) const { return( 1.0 ); }		//< @brief Y軸方向の解像度を設定する
	double reso3( double r3 ){ return( 1.0 ); }		//< @brief Z軸方向の解像度を返す
	double reso3( ) const { return( 1.0 ); }		//< @brief Z軸方向の解像度を設定する）


	/// @brief コンテナ内の要素が占めるデータ量をバイト単位で返す
	size_type byte( ) const { return( size_ * sizeof( value_type ) ); }


	/// @brief コンテナの先頭を指すランダムアクセスイテレータを返す
	//!
	//! @code 順方向ランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.begin( );
	//! for( ; ite1 != a.end( ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator begin( ){ return( iterator( paccess( 0 ), 1 ) ); }

	/// @brief コンテナの先頭を指すコンスト型のコンテナを操作するランダムアクセスイテレータを返す
	const_iterator begin( ) const { return( const_iterator( paccess( 0 ), 1 ) ); }


	/// @brief コンテナの末尾を指すランダムアクセスイテレータを返す
	iterator end( ){ return( iterator( paccess( size( ) ), 1 ) ); }

	/// @brief コンテナの末尾を指すコンスト型のコンテナを操作するランダムアクセスイテレータを返す
	const_iterator end( ) const { return( const_iterator( paccess( size( ) ), 1 ) ); }



	/// @brief コンテナの末尾を指す逆方向ランダムアクセスイテレータを返す
	//!
	//! @code 逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.rbegin( );
	//! for( ; ite2 != a.rend( ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator rbegin( ){ return( reverse_iterator( end( ) ) ); }

	/// @brief コンテナの末尾を指すコンスト型のコンテナを操作する逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator rbegin( ) const { return( const_reverse_iterator( end( ) ) ); }


	/// @brief コンテナの先頭を指す逆方向ランダムアクセスイテレータを返す
	reverse_iterator rend( ){ return( reverse_iterator( begin( ) ) ); }

	/// @brief コンテナの先頭を指すコンスト型のコンテナを操作する逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator rend( ) const { return( const_reverse_iterator( begin( ) ) ); }


	/// @brief 使用しているアロケータが確保可能なメモリの最大値を返す
	size_type max_size( ) const { return( allocator_.max_size( ) ); }



private: // サポートしないＳＴＬの関数（実装・使用しちゃだめ）
	iterator erase( iterator i );
	iterator erase( iterator s, iterator e );
	iterator insert( iterator i, const value_type &val );
	void insert( iterator i, size_type num, const value_type &val );

public:
	/// @brief 要素の型が異なるコンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! コピー元とコピー先でデータ型（array< data > の data）が異なる場合の代入を行う．
	//! 
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class TT, class AAlocator >
	const array& operator =( const array< TT, AAlocator >  &o )
	{
		if( size_ > o.size( ) )
		{
			data_ = allocator_.trim_objects( data_, size_, o.size( ) );
			size_ = o.size( );
		}
		else if( size_ < o.size( ) )
		{
			allocator_.deallocate_objects( data_, size_ );
			size_ = o.size( );
			data_ = allocator_.allocate_objects( size_ );
		}
		for( size_type i = 0 ; i < size_ ; i++ ) data_[i] = static_cast< value_type >( o[i] );

		return( *this );
	}

	/// @brief 要素の型が異なるコンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元のコンテナ
	//! @return       … 自分自身
	//! 
	const array& operator =( const array  &o )
	{
		if( this == &o ) return( *this );

		if( size_ > o.size_ )
		{
			data_ = allocator_.trim_objects( data_, size_, o.size_ );
			size_ = o.size_;
		}
		else if( size_ < o.size_ )
		{
			allocator_.deallocate_objects( data_, size_ );
			size_ = o.size_;
			data_ = allocator_.allocate_objects( size_ );
		}
		allocator_.copy_objects( o.data_, size_, data_ );

		return( *this );
	}

// コンテナ内の要素へのアクセス演算子
protected:
	/// @brief index で示される位置の要素のポインタを返す
	//! @param[in] index  … コンテナ内の要素位置
	//! @return           … 指定された要素を示すポインタ
	pointer paccess( size_type index )
	{
		return( data_ + index );
	}
	/// @brief index で示される位置の要素の const ポインタを返す
	//! @param[in] index  … コンテナ内の要素位置
	//! @return           … 指定された要素を示す const ポインタ
	const_pointer paccess( size_type index ) const
	{
		return( data_ + index );
	}

public:
	/// @brief index で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	/// @param[in] dmy1  … 使用しない
	/// @param[in] dmy2  … 使用しない
	/// @return          … 指定された要素を示す参照
	//!
	reference at( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}


	/// @brief index で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	/// @param[in] dmy1  … 使用しない
	/// @param[in] dmy2  … 使用しない
	/// @return          … 指定された要素を示す const 参照
	//!
	const_reference at( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}


	/// @brief index で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	/// @param[in] dmy1  … 使用しない
	/// @param[in] dmy2  … 使用しない
	/// @return          … 指定された要素を示す参照
	//!
	reference operator ()( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}


	/// @brief index で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	/// @param[in] dmy1  … 使用しない
	/// @param[in] dmy2  … 使用しない
	/// @return          … 指定された要素を示す const 参照
	//!
	const_reference operator ()( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}


	/// @brief index で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	/// @param[in] dmy1  … 使用しない
	/// @param[in] dmy2  … 使用しない
	/// @return          … 指定された要素を示す参照
	//!
	reference operator []( size_type index )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}


	/// @brief index で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	/// @param[in] dmy1  … 使用しない
	/// @param[in] dmy2  … 使用しない
	/// @return          … 指定された要素を示す const 参照
	//!
	const_reference operator []( size_type index ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}

public:
	/// @brief ディフォルトコンストラクタ．要素数 0 のコンテナを作成する
	array( ) : allocator_( ), size_( 0 ), data_( NULL ){}

	explicit array( const Allocator &a ) : allocator_( a ), size_( 0 ), data_( NULL ){}


	/// @brief 要素数 num 個のコンテナを作成し，デフォルト値で要素を初期化する
	explicit array( size_type num ) : allocator_( ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
	}
	/// @brief 要素数 num 個のコンテナを作成し，アロケータ a のコピーを利用する
	array( size_type num, const Allocator &a ) : allocator_( a ), size_( num ), data_( allocator_.allocate_objects( size_ ) )
	{
		data_ = allocator_.allocate_objects( size_ );
	}

	/// @brief 要素数 num 個のコンテナを作成し，値 val で初期化する
	array( size_type num, const value_type &val ) : allocator_( ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
	}
	/// @brief 要素数 num 個のコンテナを作成し，値 val で初期化し，アロケータ a のコピーを利用する
	array( size_type num, const value_type &val, const Allocator &a ) : allocator_( a ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
	}

	/// @brief イテレータ s と e の範囲の値を用いて，配列を初期化する
	array( const_iterator s, const_iterator e ) : allocator_( ), size_( e - s ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( s, e );
	}
	/// @brief イテレータ s と e の範囲の値を用いて，配列を初期化し，アロケータ a のコピーを利用する
	array( const_iterator s, const_iterator e, const Allocator &a ) : allocator_( a ), size_( e - s ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( s, e );
	}

	/// @brief 他の array 配列で要素の型が異なるものから同じ要素数の配列を作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < class TT, class AAlocator >
	array( const array< TT, AAlocator > &o ) : allocator_( ), size_( o.size( ) ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		for( size_type i = 0 ; i < size_ ; i++ ) data_[i] = static_cast< value_type >( o[i] );
	}

	/// @brief 他の array 配列で同じ要素型のものを用いて初期化する
	array( const array< T, Allocator > &o ) : allocator_( o.allocator_ ), size_( o.size_ ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		allocator_.copy_objects( o.data_, size_, data_ );
	}

	/// @brief コンテナが利用しているリソースを全て開放する
	~array( )
	{
		clear( );
	}
};



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator = std::allocator< T > >
class array1 : public array< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	/// @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef mist_iterator1< T, ptrdiff_t, pointer, const_reference > const_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;


protected:
	typedef array< T, Allocator > base;
	double reso1_;

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	double reso1( double r1 ){ return( reso1_ = r1 ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	double reso1( ) const { return( reso1_ ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void reso( double r1 ){ reso1_ = r1; }


/************************************************************************************************************
**
**      X方向に対する順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator x_begin( ){ return( base::begin( ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_iterator x_begin( ) const { return( base::begin( ) ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator x_end( ){ return( base::end( ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_iterator x_end( ) const { return( base::end( ) ); }



	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator x_rbegin( ){ return( base::rbegin( ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reverse_iterator x_rbegin( ) const { return( base::rbegin( ) ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator x_rend( ){ return( base::rend( ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reverse_iterator x_rend( ) const { return( base::rend( )  ); }


public: // 配列に対する算術演算

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void swap( array1 &a )
	{
		base::swap( a );
		double dmy_reso1_ = reso1_;
		reso1_ = a.reso1_;
		a.reso1_ = dmy_reso1_;
	}

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class TT, class AAlocator >
	const array1& operator =( const array1< TT, AAlocator > &o )
	{
		base::operator =( o );
		reso1_ = o.reso1( );

		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const array1& operator =( const array1 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		reso1_ = o.reso1_;

		return( *this );
	}

public:
	// 構築
	array1( ) : base( ), reso1_( 1.0 ) {}
	explicit array1( const Allocator &a ) : base( a ), reso1_( 1.0 ) {}

	explicit array1( size_type num ) : base( num ), reso1_( 1.0 ) {}
	array1( size_type num, double r1 ) : base( num ), reso1_( r1 ) {}
	array1( size_type num, const Allocator &a ) : base( num, a ), reso1_( 1.0 ) {}
	array1( size_type num, double r1, const Allocator &a ) : base( num, a ), reso1_( r1 ) {}

	array1( size_type num, double r1, const value_type &val ) : base( num, val ), reso1_( r1 ) {}
	array1( size_type num, const value_type &val, const Allocator &a ) : base( num, val, a ), reso1_( 1.0 ) {}
	array1( size_type num, double r1, const value_type &val, const Allocator &a ) : base( num, val, a ), reso1_( r1 ) {}

	template < class TT, class AAlocator >
	explicit array1( const array1< TT, AAlocator > &o ) : base( o ), reso1_( o.reso1( ) ) {}

	array1( const array1< T, Allocator > &o ) : base( o ), reso1_( o.reso1_ ) {}
};



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator = std::allocator< T > >
class array2 : public array1< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	/// @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef mist_iterator1< T, ptrdiff_t, pointer, const_reference > const_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;


protected:
	typedef array1< T, Allocator > base;
	size_type size1_;
	size_type size2_;
	double reso2_;

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2 )
	{
		base::resize( num1 * num2 );
		size1_ = num1;
		size2_ = num2;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2, const value_type &val )
	{
		base::resize( num1 * num2, val );
		size1_ = num1;
		size2_ = num2;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void swap( array2 &a )
	{
		base::swap( a );

		double dmy_reso2_ = reso2_;
		reso2_ = a.reso2_;
		a.reso2_ = dmy_reso2_;

		size_type _dmy_size1 = size1_;
		size_type _dmy_size2 = size2_;
		size1_ = a.size1_;
		size2_ = a.size2_;
		a.size1_ = _dmy_size1;
		a.size2_ = _dmy_size2;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = 0;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	size_type size1( )  const { return( size1_ ); }
	size_type size2( )  const { return( size2_ ); }
	size_type width( )  const { return( size1_ ); }
	size_type height( ) const { return( size2_ ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	double reso2( double r2 ){ return( reso2_ = r2 ); }
	double reso2( ) const { return( reso2_ ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void reso( double r1, double r2 ){ base::reso1_ = r1; reso2_ = r2; }



/************************************************************************************************************
**
**      X軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator x_begin( size_type i ){ return( iterator( paccess( i, 0 ), width( ) ) ); }
	const_iterator x_begin( size_type i ) const { return( const_iterator( paccess( i, 0 ), width( ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator x_end( size_type i ){ return( iterator( paccess( i, height( ) ), width( ) ) ); }
	const_iterator x_end( size_type i ) const { return( const_iterator( paccess( i, height( ) ), width( ) ) ); }


	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator x_rbegin( size_type i ){ return( reverse_iterator( x_end( i ) ) ); }
	const_reverse_iterator x_rbegin( size_type i ) const { return( const_reverse_iterator( x_end( i ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator x_rend( size_type i ){ return( reverse_iterator( x_begin( i ) ) ); }
	const_reverse_iterator x_rend( size_type i ) const { return( const_reverse_iterator( x_begin( i ) ) ); }

/************************************************************************************************************
**
**      Y軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator y_begin( size_type j ){ return( iterator( paccess( 0, j ), 1 ) ); }
	const_iterator y_begin( size_type j ) const { return( const_iterator( paccess( 0, j ), 1 ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator y_end( size_type j ){ return( iterator( paccess( width( ), j ), 1 ) ); }
	const_iterator y_end( size_type j ) const { return( const_iterator( paccess( width( ), j ), 1 ) ); }


	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator y_rbegin( size_type j ){ return( reverse_iterator( y_end( j ) ) ); }
	const_reverse_iterator y_rbegin( size_type j ) const { return( const_reverse_iterator( y_end( j ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator y_rend( size_type j ){ return( reverse_iterator( y_begin( j ) ) ); }
	const_reverse_iterator y_rend( size_type j ) const { return( const_reverse_iterator( y_begin( j ) ) ); }


public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class TT, class AAlocator >
	const array2& operator =( const array2< TT, AAlocator > &o )
	{
		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );
		reso2_ = o.reso2( );

		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const array2& operator =( const array2 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1_;
		size2_ = o.size2_;
		reso2_ = o.reso2_;

		return( *this );
	}

// 要素へのアクセス
protected:
	pointer paccess( size_type i, size_type j )
	{
		return( base::data_ + i + j * size1_ );
	}
	const_pointer paccess( size_type i, size_type j ) const
	{
		return( base::data_ + i + j * size1_ );
	}

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reference at( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reference at( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reference operator ()( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reference operator ()( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


public:
	// 構築
	array2( ) : base( ), size1_( 0 ), size2_( 0 ), reso2_( 1.0 ) {}
	explicit array2( const Allocator &a ) : base( a ), size1_( 0 ), size2_( 0 ), reso2_( 1.0 ) {}

	array2( size_type num1, size_type num2 ) : base( num1 * num2 ), size1_( num1 ), size2_( num2 ), reso2_( 1.0 ) {}
	array2( size_type num1, size_type num2, double r1, double r2 ) : base( num1 * num2, r1 ), size1_( num1 ), size2_( num2 ), reso2_( r2 ) {}
	array2( size_type num1, size_type num2, const Allocator &a ) : base( num1 * num2, a ), size1_( num1 ), size2_( num2 ), reso2_( 1.0 ) {}
	array2( size_type num1, size_type num2, double r1, double r2, const Allocator &a ) : base( num1 * num2, r1, a ), size1_( num1 ), size2_( num2 ), reso2_( r2 ) {}

	array2( size_type num1, size_type num2, const value_type &val ) : base( num1 * num2, val ), size1_( num1 ), size2_( num2 ), reso2_( 1.0 ) {}
	array2( size_type num1, size_type num2, double r1, double r2, const value_type &val ) : base( num1 * num2, r1, val ), size1_( num1 ), size2_( num2 ), reso2_( r2 ) {}
	array2( size_type num1, size_type num2, const value_type &val, const Allocator &a ) : base( num1 * num2, val, a ), size1_( num1 ), size2_( num2 ), reso2_( 1.0 ) {}
	array2( size_type num1, size_type num2, double r1, double r2, const value_type &val, const Allocator &a ) : base( num1 * num2, r1, val, a ), size1_( num1 ), size2_( num2 ), reso2_( r2 ) {}

	template < class TT, class AAlocator >
	array2( const array2< TT, AAlocator > &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ), reso2_( o.reso2( ) ) {}

	array2( const array2< T, Allocator > &o ) : base( o ), size1_( o.size1_ ), size2_( o.size2_ ), reso2_( o.reso2_ ) {}
};





/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator = std::allocator< T > >
class array3 : public array2< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	/// @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef mist_iterator2< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef mist_iterator2< T, ptrdiff_t, pointer, const_reference > const_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef mist_reverse_iterator< mist_iterator2< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef mist_reverse_iterator< mist_iterator2< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;


protected:
	typedef array2< T, Allocator > base;
	size_type size1_;
	size_type size2_;
	size_type size3_;
	double reso3_;

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2, size_type num3 )
	{
		base::resize( num1 * num2, num3 );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2, size_type num3, const value_type &val )
	{
		base::resize( num1 * num2, num3, val );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void swap( array3 &a )
	{
		base::swap( a );

		double dmy_reso3_ = reso3_;
		reso3_ = a.reso3_;
		a.reso3_ = dmy_reso3_;

		size_type _dmy_size1 = size1_;
		size_type _dmy_size2 = size2_;
		size_type _dmy_size3 = size3_;
		size1_ = a.size1_;
		size2_ = a.size2_;
		size3_ = a.size3_;
		a.size1_ = _dmy_size1;
		a.size2_ = _dmy_size2;
		a.size3_ = _dmy_size3;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = size3_ = 0;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	size_type size1( ) const { return( size1_ ); }
	size_type size2( ) const { return( size2_ ); }
	size_type size3( ) const { return( size3_ ); }
	size_type width( ) const { return( size1_ ); }
	size_type height( ) const { return( size2_ ); }
	size_type depth( ) const { return( size3_ ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	double reso3( double r3 ){ return( reso3_ = r3 ); }
	double reso3( ) const { return( reso3_ ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void reso( double r1, double r2, double r3 ){ base::reso1_ = r1; base::reso2_ = r2; reso3_ = r3; }



	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator begin( ){ return( iterator( paccess( 0, 0, 0 ), 0, base::size( ), 0 ) ); }
	const_iterator begin( ) const { return( const_iterator( paccess( 0, 0, 0 ), 0, base::size( ), 0 ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator end( ){ return( iterator( paccess( 0, 0, 0 ), base::size( ), base::size( ), 0 ) ); }
	const_iterator end( ) const { return( const_iterator( paccess( 0, 0, 0 ), base::size( ), base::size( ), 0 ) ); }


	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator rbegin( ){ return( reverse_iterator( end( ) ) ); }
	const_reverse_iterator rbegin( ) const { return( const_reverse_iterator( end( ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator rend( ){ return( reverse_iterator( begin( ) ) ); }
	const_reverse_iterator rend( ) const { return( const_reverse_iterator( begin( ) ) ); }

/************************************************************************************************************
**
**      X軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator x_begin( size_type i ){ return( iterator( paccess( i, 0, 0 ), 0, 1, width( ) ) ); }
	const_iterator x_begin( size_type i ) const { return( const_iterator( paccess( i, 0, 0 ), 0, 1, width( ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator x_end( size_type i ){ return( iterator( paccess( i, 0, 0 ), height( ) * depth( ), 1, width( ) ) ); }
	const_iterator x_end( size_type i ) const { return( const_iterator( paccess( i, 0, 0 ), height( ) * depth( ), 1, width( ) ) ); }



	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator x_rbegin( size_type i ){ return( reverse_iterator( x_end( i ) ) ); }
	const_reverse_iterator x_rbegin( size_type i ) const { return( const_reverse_iterator( x_end( i ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator x_rend( size_type i ){ return( reverse_iterator( x_begin( i ) ) ); }
	const_reverse_iterator x_rend( size_type i ) const { return( const_reverse_iterator( x_begin( i ) ) ); }


/************************************************************************************************************
**
**      Y軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator y_begin( size_type j ){ return( iterator( paccess( 0, j, 0 ), 0, height( ), width( ) * height( ) ) ); }
	const_iterator y_begin( size_type j ) const { return( const_iterator( paccess( 0, j, 0 ), 0, height( ), width( ) * height( ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator y_end( size_type j ){ return( iterator( paccess( 0, j, 0 ), width( ) * depth( ), height( ), width( ) * height( ) ) ); }
	const_iterator y_end( size_type j ) const { return( const_iterator( paccess( 0, j, 0 ), width( ) * depth( ), height( ), width( ) * height( ) ) ); }


	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator y_rbegin( size_type j ){ return( reverse_iterator( y_end( j ) ) ); }
	const_reverse_iterator y_rbegin( size_type j ) const { return( const_reverse_iterator( y_end( j ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator y_rend( size_type j ){ return( reverse_iterator( y_begin( j ) ) ); }
	const_reverse_iterator y_rend( size_type j ) const { return( const_reverse_iterator( y_begin( j ) ) ); }


/************************************************************************************************************
**
**      Z軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator z_begin( size_type k ){ return( iterator( paccess( 0, 0, k ), 0, 1, 1 ) ); }
	const_iterator z_begin( size_type k ) const { return( const_iterator( paccess( 0, 0, k ), 0, 1, 1 ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	iterator z_end( size_type k ){ return( iterator( paccess( 0, 0, k ), width( ) * height( ), 1, 1 ) ); }
	const_iterator z_end( size_type k ) const { return( const_iterator( paccess( 0, 0, k ), width( ) * height( ), 1, 1 ) ); }


	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator z_rbegin( size_type k ){ return( reverse_iterator( z_end( k ) ) ); }
	const_reverse_iterator z_rbegin( size_type k ) const { return( const_reverse_iterator( z_end( k )) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reverse_iterator z_rend( size_type k ){ return( reverse_iterator( z_begin( k ) ) ); }
	const_reverse_iterator z_rend( size_type k ) const { return( const_reverse_iterator( z_begin( k ) ) ); }


public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class TT, class AAlocator >
	const array3& operator =( const array3< TT, AAlocator > &o )
	{
		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );
		size3_ = o.size3( );
		reso3_ = o.reso3( );

		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const array3& operator =( const array3 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1_;
		size2_ = o.size2_;
		size3_ = o.size3_;
		reso3_ = o.reso3_;

		return( *this );
	}


// 要素へのアクセス
protected:
	pointer paccess( size_type i, size_type j, size_type k )
	{
		return( base::data_ + i + ( j + k * size2_ ) * size1_ );
	}
	const_pointer paccess( size_type i, size_type j, size_type k ) const
	{
		return( base::data_ + i + ( j + k * size2_ ) * size1_ );
	}

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reference at( size_type i, size_type j, size_type k )
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reference at( size_type i, size_type j, size_type k ) const
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reference operator ()( size_type i, size_type j, size_type k )
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reference operator ()( size_type i, size_type j, size_type k ) const
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


public:
	// 構築
	array3( ) : base( ), size1_( 0 ), size2_( 0 ), size3_( 0 ), reso3_( 1.0 ) {}
	explicit array3( const Allocator &a ) : base( a ), size1_( 0 ), size2_( 0 ), size3_( 0 ), reso3_( 1.0 ) {}

	array3( size_type num1, size_type num2, size_type num3 ) : base( num1 * num2, num3 ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( 1.0 ) {}
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3 ) : base( num1 * num2, num3, r1, r2 ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( r3 ) {}
	array3( size_type num1, size_type num2, size_type num3, const Allocator &a ) : base( num1 * num2, num3, a ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( 1.0 ) {}
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const Allocator &a ) : base( num1 * num2, num3, r1, r2, a ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( r3 ) {}

	array3( size_type num1, size_type num2, size_type num3, const value_type &val ) : base( num1 * num2, num3, val ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( 1.0 ) {}
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const value_type &val ) : base( num1 * num2, num3, r1, r2, val ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( r3 ) {}
	array3( size_type num1, size_type num2, size_type num3, const value_type &val, const Allocator &a ) : base( num1 * num2, num3, val, a ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( 1.0 ) {}
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const value_type &val, const Allocator &a ) : base( num1 * num2, num3, r1, r2, val, a ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( r3 ) {}

	template < class TT, class AAlocator >
	array3( const array3< TT, AAlocator > &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( o.size3( ) ), reso3_( o.reso3( ) ) {}

	array3( const array3< T, Allocator > &o ) : base( o ), size1_( o.size1_ ), size2_( o.size2_ ), size3_( o.size3_ ), reso3_( o.reso3_ ) {}
};




/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//!
//! @return        … 戻り値の説明
//! 
template < class Array >
class marray : public Array
{
public:
	typedef typename Array::allocator_type allocator_type;		///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Array::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Array::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Array::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Array::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Array::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Array::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Array::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	typedef typename Array::iterator iterator;								///< @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef typename Array::const_iterator const_iterator;					///< @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef typename Array::reverse_iterator reverse_iterator;				///< @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef typename Array::const_reverse_iterator const_reverse_iterator;	///< @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版


protected:
	typedef Array base;
	size_type margin1_;
	size_type margin2_;
	size_type margin3_;

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1 )
	{
		base::resize( num1 + margin1_ * 2 );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2 )
	{
		base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2 );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2, size_type num3 )
	{
		base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2, num3 + margin3_ * 2 );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, const value_type &val )
	{
		base::resize( num1 + margin1_ * 2, val );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2, const value_type &val )
	{
		base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2, val );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2, size_type num3, const value_type &val )
	{
		base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2, num3 + margin3_ * 2, val );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	void swap( marray &a )
	{
		base::swap( a );

		size_type tmp = margin1_;
		margin1_ = a.margin1_;
		a.margin1_ = tmp;

		tmp = margin2_;
		margin2_ = a.margin2_;
		a.margin2_ = tmp;

		tmp = margin3_;
		margin3_ = a.margin3_;
		a.margin3_ = tmp;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	void clear( )
	{
		base::clear( );
		margin1_ = margin2_ = margin3_ = 0;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	void fill_margin( const value_type &val = value_type( ) )
	{
		base &o = *this;

		size_type i, j, k;
		for( k = 0 ; k < margin3( ) ; k++ )
		{
			for( j = 0 ; j < o.size2( ) ; j++ )
			{
				for( i = 0 ; i < o.size1( ) ; i++ )
				{
					o( i, j, k ) = val;
				}
			}
		}
		for( k = o.size3( ) - margin3( ) ; k < o.size3( ) ; k++ )
		{
			for( j = 0 ; j < o.size2( ) ; j++ )
			{
				for( i = 0 ; i < o.size1( ) ; i++ )
				{
					o( i, j, k ) = val;
				}
			}
		}

		for( j = 0 ; j < margin2( ) ; j++ )
		{
			for( k = 0 ; k < o.size3( ) ; k++ )
			{
				for( i = 0 ; i < o.size1( ) ; i++ )
				{
					o( i, j, k ) = val;
				}
			}
		}
		for( j = o.size2( ) - margin2( ) ; j < o.size2( ) ; j++ )
		{
			for( k = 0 ; k < o.size3( ) ; k++ )
			{
				for( i = 0 ; i < o.size1( ) ; i++ )
				{
					o( i, j, k ) = val;
				}
			}
		}

		for( i = 0 ; i < margin1( ) ; i++ )
		{
			for( k = 0 ; k < o.size3( ) ; k++ )
			{
				for( j = 0 ; j < o.size2( ) ; j++ )
				{
					o( i, j, k ) = val;
				}
			}
		}
		for( i = o.size1( ) - margin1( ) ; i < o.size1( ) ; i++ )
		{
			for( k = 0 ; k < o.size3( ) ; k++ )
			{
				for( j = 0 ; j < o.size2( ) ; j++ )
				{
					o( i, j, k ) = val;
				}
			}
		}
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	size_type size1( ) const { return( base::size1( ) - 2 * margin1_ ); }
	size_type size2( ) const { return( base::size2( ) - 2 * margin2_ ); }
	size_type size3( ) const { return( base::size3( ) - 2 * margin3_ ); }
	size_type width( ) const { return( size1( ) ); }
	size_type height( ) const { return( size2( ) ); }
	size_type depth( ) const { return( size3( ) ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	size_type margin1( ) const { return( margin1_ ); }
	size_type margin2( ) const { return( margin2_ ); }
	size_type margin3( ) const { return( margin3_ ); }

private:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const marray& copy( const array< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const marray& copy( const array1< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const marray& copy( const array2< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		for( size_type j = 0 ; j < h ; j++ )
		{
			for( size_type i = 0 ; i < w ; i++ )
			{
				( *this )( i, j ) = static_cast< value_type >( o( i, j ) );
			}
		}
		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const marray& copy( const array3< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		size_type d = depth( )  < o.depth( )  ? depth( )  : o.depth( );
		for( size_type k = 0 ; k < d ; k++ )
		{
			for( size_type j = 0 ; j < h ; j++ )
			{
				for( size_type i = 0 ; i < w ; i++ )
				{
					( *this )( i, j, k ) = static_cast< value_type >( o( i, j, k ) );
				}
			}
		}
		return( *this );
	}

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	const marray& operator =( const marray &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		margin1_ = o.margin1_;
		margin2_ = o.margin2_;
		margin3_ = o.margin3_;

		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array< T, Allocator > &o )
	{
		base::resize( o.size( ) + margin1_ * 2 );
		return( copy( o ) );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array1< T, Allocator > &o )
	{
		base::resize( o.size( ) + margin1_ * 2 );
		reso1( o.reso1( ) );
		return( copy( o ) );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array2< T, Allocator > &o )
	{
		base::resize( o.size1( ) + margin1_ * 2, o.size2( ) + margin2_ * 2 );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		return( copy( o ) );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array3< T, Allocator > &o )
	{
		base::resize( o.size1( ) + margin1_ * 2, o.size2( ) + margin2_ * 2, o.size3( ) + margin3_ * 2 );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		reso3( o.reso3( ) );
		return( copy( o ) );
	}

// 要素へのアクセス
public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reference at( difference_type i )
	{
		return( base::at( i + margin1_ ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reference at( difference_type i, difference_type j )
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reference at( difference_type i, difference_type j, difference_type k )
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reference at( difference_type i ) const
	{
		return( base::at( i + margin1_ ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reference at( difference_type i, difference_type j ) const
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reference at( difference_type i, difference_type j, difference_type k ) const
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reference operator ()( difference_type i )
	{
		return( base::at( i + margin1_ ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reference operator ()( difference_type i, difference_type j )
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	reference operator ()( difference_type i, difference_type j, difference_type k )
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reference operator ()( difference_type i ) const
	{
		return( base::at( i + margin1_ ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reference operator ()( difference_type i, difference_type j ) const
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const_reference operator ()( difference_type i, difference_type j, difference_type k ) const
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	marray( ) : base( ), margin1_( 0 ), margin2_( 0 ), margin3_( 0 ) {}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	marray( size_type margin ) : base( ), margin1_( margin ), margin2_( 0 ), margin3_( 0 ) {}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	marray( const marray &o ) : base( o ), margin1_( o.margin1( ) ), margin2_( o.margin2( ) ), margin3_( o.margin3( ) ) {}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	template < class T, class Allocator >
	marray( const array< T, Allocator > &o, size_type margin1, const value_type &val = value_type( 0 ) )
		: base( o.size( ) + margin1 * 2 ), margin1_( margin1 ), margin2_( 0 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	template < class T, class Allocator >
	marray( const array1< T, Allocator > &o, size_type margin1, const value_type &val = value_type( ) )
		: base( o.size( ) + margin1 * 2, o.reso1( ) ), margin1_( margin1 ), margin2_( 0 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	template < class T, class Allocator >
	marray( const array2< T, Allocator > &o, size_type margin1, size_type margin2, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin2 * 2, o.reso1( ), o.reso2( ) ), margin1_( margin1 ), margin2_( margin2 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	template < class T, class Allocator >
	marray( const array3< T, Allocator > &o, size_type margin1, size_type margin2, size_type margin3, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin2 * 2, o.size3( ) + margin3 * 2, o.reso1( ), o.reso2( ), o.reso3( ) ), margin1_( margin1 ), margin2_( margin2 ), margin3_( margin3 )
	{
		fill_margin( val );
		copy( o );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	template < class T, class Allocator >
	marray( const array2< T, Allocator > &o, size_type margin1, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin1 * 2, o.reso1( ), o.reso2( ) ), margin1_( margin1 ), margin2_( margin1 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	template < class T, class Allocator >
	marray( const array3< T, Allocator > &o, size_type margin1, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin1 * 2, o.size3( ) + margin1 * 2, o.reso1( ), o.reso2( ), o.reso3( ) ), margin1_( margin1 ), margin2_( margin1 ), margin3_( margin1 )
	{
		fill_margin( val );
		copy( o );
	}
};





/// @brief １辺が2の指数乗のバッファを持った画像を作成する
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class Array >
class buffered_array : public Array
{
public:
	typedef typename Array::allocator_type allocator_type;		///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Array::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Array::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Array::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Array::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Array::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Array::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Array::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	typedef typename Array::iterator iterator;								///< @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef typename Array::const_iterator const_iterator;					///< @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef typename Array::reverse_iterator reverse_iterator;				///< @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef typename Array::const_reverse_iterator const_reverse_iterator;	///< @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版

protected:
	typedef Array base;
	size_type size1_;
	size_type size2_;
	size_type size3_;

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	inline size_t floor_square_index( size_t v )
	{
		if( v == 0 )
		{
			return( 0 );
		}

		for( size_t i = 1, _2 = 2 ; i < 64 ; i++ )
		{
			if( v <= _2 )
			{
				return( i );
			}
			_2 *= 2;
		}

		return( 0 );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	inline size_t floor_square( size_t v )
	{
		if( v == 0 )
		{
			return( 0 );
		}

		for( size_t i = 1, _2 = 2 ; i < 64 ; i++ )
		{
			if( v <= _2 )
			{
				return( _2 );
			}
			_2 *= 2;
		}

		return( 0 );
	}

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1 )
	{
		size1_ = num1;
		base::resize( floor_square( size1_ ) );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2 )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size1_ = num1;
		size2_ = num2;
		s1 = s1 > s2 ? s1 : s2;
		base::resize( s1, s1 );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2, size_type num3 )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size_type s3 = floor_square( num3 );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
		s1 = s1 > s2 ? s1 : s2;
		s1 = s1 > s3 ? s1 : s3;
		base::resize( s1, s1, s1 );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, const value_type &val )
	{
		size1_ = num1;
		base::resize( floor_square( size1_ ), val );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2, const value_type &val )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size1_ = num1;
		size2_ = num2;
		s1 = s1 > s2 ? s1 : s2;
		base::resize( s1, s1, val );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void resize( size_type num1, size_type num2, size_type num3, const value_type &val )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size_type s3 = floor_square( num3 );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
		s1 = s1 > s2 ? s1 : s2;
		s1 = s1 > s3 ? s1 : s3;
		base::resize( s1, s1, s1, val );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void swap( buffered_array &a )
	{
		base::swap( a );

		size_type tmp = size1_;
		size1_ = a.size1_;
		a.size1_ = tmp;

		tmp = size2_;
		size2_ = a.size2_;
		a.size2_ = tmp;

		tmp = size3_;
		size3_ = a.size3_;
		a.size3_ = tmp;
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = size3_ = 0;
	}

	size_type size1( ) const { return( size1_ > 0 ? size1_ : base::size1( ) ); }
	size_type size2( ) const { return( size2_ > 0 ? size2_ : base::size2( ) ); }
	size_type size3( ) const { return( size3_ > 0 ? size3_ : base::size3( ) ); }
	size_type width( ) const { return( size1( ) ); }
	size_type height( ) const { return( size2( ) ); }
	size_type depth( ) const { return( size3( ) ); }


private:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array1< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array2< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		for( size_type j = 0 ; j < h ; j++ )
		{
			for( size_type i = 0 ; i < w ; i++ )
			{
				( *this )( i, j ) = static_cast< value_type >( o( i, j ) );
			}
		}
		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array3< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		size_type d = depth( )  < o.depth( )  ? depth( )  : o.depth( );
		for( size_type k = 0 ; k < d ; k++ )
		{
			for( size_type j = 0 ; j < h ; j++ )
			{
				for( size_type i = 0 ; i < w ; i++ )
				{
					( *this )( i, j, k ) = static_cast< value_type >( o( i, j, k ) );
				}
			}
		}
		return( *this );
	}


public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const buffered_array& operator =( const buffered_array &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1_;
		size2_ = o.size2_;
		size3_ = o.size3_;

		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array< T, Allocator > &o )
	{
		resize( o.size( ) );
		return( copy( o ) );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array1< T, Allocator > &o )
	{
		resize( o.size( ) );
		reso1( o.reso1( ) );
		return( copy( o ) );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array2< T, Allocator > &o )
	{
		resize( o.size1( ), o.size2( ) );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		return( copy( o ) );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array3< T, Allocator > &o )
	{
		resize( o.size1( ), o.size2( ), o.size3( ) );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		reso3( o.reso3( ) );
		return( copy( o ) );
	}

public:
	// 構築
	buffered_array( ) : base( ), size1_( 0 ), size2_( 0 ), size3_( 0 ) {}

	buffered_array( const buffered_array &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( o.size3( ) ) {}

	template < class T, class Allocator >
	buffered_array( const array< T, Allocator > &o )
		: base( floor_square( o.size( ) ) ), size1_( o.size( ) ), size2_( 0 ), size3_( 0 )
	{
		copy( o );
	}

	template < class T, class Allocator >
	buffered_array( const array1< T, Allocator > &o )
		: base( floor_square( o.size( ) ), o.reso1( ) ), size1_( o.size( ) ), size2_( 0 ), size3_( 0 )
	{
		copy( o );
	}

	template < class T, class Allocator >
	buffered_array( const array2< T, Allocator > &o )
		: base( floor_square( o.size1( ) ), floor_square( o.size2( ) ), o.reso1( ), o.reso2( ) ), size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( 0 )
	{
		copy( o );
	}

	template < class T, class Allocator >
	buffered_array( const array3< T, Allocator > &o )
		: base( floor_square( o.size1( ) ), floor_square( o.size2( ) ), floor_square( o.size3( ) ), o.reso1( ), o.reso2( ), o.reso3( ) ),
			size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( o.size3( ) )
	{
		copy( o );
	}
};


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array< T, Allocator > &a )
{
	typename array< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		out << a[i];
		if( i != a.size1( ) - 1 ) out << ", ";
	}
	out << std::endl;

	return( out );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array1< T, Allocator > &a )
{
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		out << a[i];
		if( i != a.size1( ) - 1 ) out << ", ";
	}
	out << std::endl;

	return( out );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array2< T, Allocator > &a )
{
	typename array2< T, Allocator >::size_type i, j;
	for( j = 0 ; j < a.size2( ) ; j++ )
	{
		for( i = 0 ; i < a.size1( ) ; i++ )
		{
			out << a( i, j );
			if( i != a.size1( ) - 1 ) out << ", ";
		}
		out << std::endl;
	}

	return( out );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array3< T, Allocator > &a )
{
	typename array3< T, Allocator >::size_type i, j, k;
	for( k = 0 ; k < a.size3( ) ; k++ )
	{
		for( j = 0 ; j < a.size2( ) ; j++ )
		{
			for( i = 0 ; i < a.size1( ) ; i++ )
			{
				out << a( i, j, k );
				if( i != a.size1( ) - 1 ) out << ", ";
			}
			out << std::endl;
		}
		if( k != a.size3( ) - 1 )
		{
			out << "----- separator -----";
		}
		out << std::endl;
	}

	return( out );
}



// 画像間演算
//#include "operator/operator_array.h"
//#include "operator/operator_array1.h"
//#include "operator/operator_array2.h"
//#include "operator/operator_array3.h"

// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_H__
