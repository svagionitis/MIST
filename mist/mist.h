/****************************************************************************************************************************
**
**         MIST ( Media Integration Standard Toolkit )
**
**         We defined following template classes.
**
**          array  : template class of one dimensionl array with STL support.
**          array1 : template class of one dimensionl array containing resolution of each value.
**          array2 : template class of two dimensionl array containing resolution of each pixel
**          array3 : template class of three dimensionl array containing resolution of each voxel
**          matrix : template class of matrix with its operation, and impremented using teqnique of an expression template.
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
**              Copyright ***********************.
**                    All Rights Reserved.
**
****************************************************************************************************************************/



#ifndef __INCLUDE_MIST_H__
#define __INCLUDE_MIST_H__

#include "config/mist_conf.h"
#include "config/mist_alloc.h"
#include "config/iterator.h"


// mist名前空間の始まり
_MIST_BEGIN



// １次元配列
template < class T, class Allocator = std::allocator< T > >
class array
{
public:
	typedef Allocator allocator_type;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;

	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;
	typedef mist_iterator1< T, ptrdiff_t, pointer, const_reference > const_iterator;
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;

private:
	mist_allocator< T, Allocator > allocator_;
	size_type size_;

protected:
	T* data_;

public:
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

	void swap( array &a )
	{
		size_type _dmy_size = size_;
		size_ = a.size_;
		a.size_ = _dmy_size;

		value_type *dmy_data_ = data_;
		data_ = a.data_;
		a.data_ = dmy_data_;
	}

	void clear( )
	{
		allocator_.deallocate_objects( data_, size_ );
		size_ = 0;
		data_ = NULL;
	}

	void fill( )
	{
		allocator_.fill_objects( data_, size_ );
	}

	void fill( const value_type &val )
	{
		allocator_.fill_objects( data_, size_, val );
	}

	bool empty( ) const { return( size_ == 0 ); }

	size_type size( ) const { return( size_ ); }
	size_type size1( ) const { return( size_ ); }
	size_type size2( ) const { return( 1 ); }
	size_type size3( ) const { return( 1 ); }
	size_type width( ) const { return( size_ ); }
	size_type height( ) const { return( 1 ); }
	size_type depth( ) const { return( 1 ); }

	double reso1( double r1 ){ return( 1.0 ); }
	double reso1( ) const { return( 1.0 ); }
	double reso2( double r2 ){ return( 1.0 ); }
	double reso2( ) const { return( 1.0 ); }
	double reso3( double r3 ){ return( 1.0 ); }
	double reso3( ) const { return( 1.0 ); }

	size_type byte( ) const { return( size_ * sizeof( value_type ) ); }

	// 順方向のランダムアクセスイテレータを返す
	iterator begin( ){ return( iterator( paccess( 0 ), 1 ) ); }
	const_iterator begin( ) const { return( const_iterator( paccess( 0 ), 1 ) ); }

	iterator end( ){ return( iterator( paccess( size( ) ), 1 ) ); }
	const_iterator end( ) const { return( const_iterator( paccess( size( ) ), 1 ) ); }


	// 逆方向のランダムアクセスイテレータを返す
	reverse_iterator rbegin( ){ return( reverse_iterator( end( ) ) ); }
	const_reverse_iterator rbegin( ) const { return( const_reverse_iterator( end( ) ) ); }

	reverse_iterator rend( ){ return( reverse_iterator( begin( ) ) ); }
	const_reverse_iterator rend( ) const { return( const_reverse_iterator( begin( ) ) ); }


	size_type max_size( ) const { return( allocator_.max_size( ) ); }



private: // サポートしないＳＴＬの関数（実装・使用しちゃだめ）
	iterator erase( iterator i );
	iterator erase( iterator s, iterator e );
	iterator insert( iterator i, const value_type &val );
	void insert( iterator i, size_type num, const value_type &val );

public:
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
	pointer paccess( size_type index )
	{
		return( data_ + index );
	}
	const_pointer paccess( size_type index ) const
	{
		return( data_ + index );
	}

public:
	reference at( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}

	const_reference at( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}

	reference operator ()( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}

	const_reference operator ()( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}

	reference operator []( size_type index )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}

	const_reference operator []( size_type index ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}

public:
	// 構築
	array( ) : allocator_( ), size_( 0 ), data_( NULL ){}
	explicit array( const Allocator &a ) : allocator_( a ), size_( 0 ), data_( NULL ){}

	explicit array( size_type num ) : allocator_( ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
	}
	array( size_type num, const Allocator &a ) : allocator_( a ), size_( num ), data_( allocator_.allocate_objects( size_ ) )
	{
		data_ = allocator_.allocate_objects( size_ );
	}

	array( size_type num, const value_type &val ) : allocator_( ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
	}
	array( size_type num, const value_type &val, const Allocator &a ) : allocator_( a ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
	}

	array( const_iterator s, const_iterator e ) : allocator_( ), size_( e - s ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( s, e );
	}
	array( const_iterator s, const_iterator e, const Allocator &a ) : allocator_( a ), size_( e - s ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( s, e );
	}

	template < class TT, class AAlocator >
	array( const array< TT, AAlocator > &o ) : allocator_( ), size_( o.size( ) ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		for( size_type i = 0 ; i < size_ ; i++ ) data_[i] = static_cast< value_type >( o[i] );
	}

	array( const array< T, Allocator > &o ) : allocator_( o.allocator_ ), size_( o.size_ ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		allocator_.copy_objects( o.data_, size_, data_ );
	}

	// 消滅
	~array( )
	{
		clear( );
	}
};



// １次元配列
template < class T, class Allocator = std::allocator< T > >
class array1 : public array< T, Allocator >
{
public:
	typedef Allocator allocator_type;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;

	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;
	typedef mist_iterator1< T, ptrdiff_t, pointer, const_reference > const_iterator;
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;

private:
	typedef array< T, Allocator > base;
	double reso1_;

public:
	double reso1( double r1 ){ return( reso1_ = r1 ); }
	double reso1( ) const { return( reso1_ ); }

/************************************************************************************************************
**
**      X方向に対する順方向・逆方向の反復子
**
************************************************************************************************************/
	// 順方向のランダムアクセスイテレータを返す
	iterator x_begin( ){ return( begin( ) ); }
	const_iterator x_begin( ) const { return( begin( ) ); }

	iterator x_end( ){ return( end( ) ); }
	const_iterator x_end( ) const { return( end( ) ); }


	// 逆方向のランダムアクセスイテレータを返す
	reverse_iterator x_rbegin( ){ return( rbegin( ) ); }
	const_reverse_iterator x_rbegin( ) const { return( rbegin( ) ); }

	reverse_iterator x_rend( ){ return( rend( ) ); }
	const_reverse_iterator x_rend( ) const { return( rend( )  ); }


public: // 配列に対する算術演算
	void swap( array1 &a )
	{
		base::swap( a );
		double dmy_reso1_ = reso1_;
		reso1_ = a.reso1_;
		a.reso1_ = dmy_reso1_;
	}

public:
	template < class TT, class AAlocator >
	const array1& operator =( const array1< TT, AAlocator > &o )
	{
		base::operator =( o );
		reso1_ = o.reso1( );

		return( *this );
	}

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


// ２次元配列
template < class T, class Allocator = std::allocator< T > >
class array2 : public array1< T, Allocator >
{
public:
	typedef Allocator allocator_type;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;

	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;
	typedef mist_iterator1< T, ptrdiff_t, pointer, const_reference > const_iterator;
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;

private:
	typedef array1< T, Allocator > base;
	size_type size1_;
	size_type size2_;
	double reso2_;

public:
	void resize( size_type num1, size_type num2 )
	{
		base::resize( num1 * num2 );
		size1_ = num1;
		size2_ = num2;
	}

	void resize( size_type num1, size_type num2, const value_type &val )
	{
		base::resize( num1 * num2, val );
		size1_ = num1;
		size2_ = num2;
	}

	void swap( array2 &a )
	{
		base::swap( a );

		double dmy_reso1_ = reso1_;
		double dmy_reso2_ = reso2_;
		reso1_ = a.reso1_;
		reso2_ = a.reso2_;
		a.reso1_ = dmy_reso1_;
		a.reso2_ = dmy_reso2_;

		size_type _dmy_size1 = size1_;
		size_type _dmy_size2 = size2_;
		size1_ = m.size1_;
		size2_ = m.size2_;
		m.size1_ = _dmy_size1;
		m.size2_ = _dmy_size2;
	}

	void clear( )
	{
		base::clear( );
		size1_ = size2_ = 0;
	}

	size_type size1( )  const { return( size1_ ); }
	size_type size2( )  const { return( size2_ ); }
	size_type width( )  const { return( size1_ ); }
	size_type height( ) const { return( size2_ ); }

	double reso2( double r2 ){ return( reso2_ = r2 ); }
	double reso2( ) const { return( reso2_ ); }


/************************************************************************************************************
**
**      X軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	// 順方向のランダムアクセスイテレータを返す
	iterator x_begin( size_type i ){ return( iterator( paccess( i, 0 ), width( ) ) ); }
	const_iterator x_begin( size_type i ) const { return( const_iterator( paccess( i, 0 ), width( ) ) ); }

	iterator x_end( size_type i ){ return( iterator( paccess( i, height( ) ), width( ) ) ); }
	const_iterator x_end( size_type i ) const { return( const_iterator( paccess( i, height( ) ), width( ) ) ); }


	// 逆方向のランダムアクセスイテレータを返す
	reverse_iterator x_rbegin( size_type i ){ return( reverse_iterator( x_end( i ) ) ); }
	const_reverse_iterator x_rbegin( size_type i ) const { return( const_reverse_iterator( x_end( i ) ) ); }

	reverse_iterator x_rend( size_type i ){ return( reverse_iterator( x_begin( i ) ) ); }
	const_reverse_iterator x_rend( size_type i ) const { return( const_reverse_iterator( x_begin( i ) ) ); }

/************************************************************************************************************
**
**      Y軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	// 順方向のランダムアクセスイテレータを返す
	iterator y_begin( size_type j ){ return( iterator( paccess( 0, j ), 1 ) ); }
	const_iterator y_begin( size_type j ) const { return( const_iterator( paccess( 0, j ), 1 ) ); }

	iterator y_end( size_type j ){ return( iterator( paccess( width( ), j ), 1 ) ); }
	const_iterator y_end( size_type j ) const { return( const_iterator( paccess( width( ), j ), 1 ) ); }


	// 逆方向のランダムアクセスイテレータを返す
	reverse_iterator y_rbegin( size_type j ){ return( reverse_iterator( y_end( j ) ) ); }
	const_reverse_iterator y_rbegin( size_type j ) const { return( const_reverse_iterator( y_end( j ) ) ); }

	reverse_iterator y_rend( size_type j ){ return( reverse_iterator( y_begin( j ) ) ); }
	const_reverse_iterator y_rend( size_type j ) const { return( const_reverse_iterator( y_begin( j ) ) ); }


public:
	template < class TT, class AAlocator >
	const array2& operator =( const array2< TT, AAlocator > &o )
	{
		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );
		reso2_ = o.reso2( );

		return( *this );
	}

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
		return( data_ + i + j * size1_ );
	}
	const_pointer paccess( size_type i, size_type j ) const
	{
		return( data_ + i + j * size1_ );
	}

public:
	reference at( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( data_[ i + j * size1_ ] );
	}

	const_reference at( size_type index, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( data_[ i + j * size1_ ] );
	}

	reference operator ()( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( data_[ i + j * size1_ ] );
	}

	const_reference operator ()( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( data_[ i + j * size1_ ] );
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




// ３次元配列
template < class T, class Allocator = std::allocator< T > >
class array3 : public array2< T, Allocator >
{
public:
	typedef Allocator allocator_type;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;

	typedef mist_iterator2< T, ptrdiff_t, pointer, reference > iterator;
	typedef mist_iterator2< T, ptrdiff_t, pointer, const_reference > const_iterator;
	typedef mist_reverse_iterator< mist_iterator2< T, ptrdiff_t, pointer, reference > > reverse_iterator;
	typedef mist_reverse_iterator< mist_iterator2< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;

private:
	typedef array2< T, Allocator > base;
	size_type size1_;
	size_type size2_;
	size_type size3_;
	double reso3_;

public:
	void resize( size_type num1, size_type num2, size_type num3 )
	{
		base::resize( num1 * num2, num3 );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
	}

	void resize( size_type num1, size_type num2, size_type num3, const value_type &val )
	{
		base::resize( num1 * num2, num3, val );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
	}

	void swap( array3 &a )
	{
		base::swap( a );

		double dmy_reso1_ = reso1_;
		double dmy_reso2_ = reso2_;
		double dmy_reso3_ = reso3_;
		reso1_ = a.reso1_;
		reso2_ = a.reso2_;
		reso3_ = a.reso3_;
		a.reso1_ = dmy_reso1_;
		a.reso2_ = dmy_reso2_;
		a.reso3_ = dmy_reso3_;

		size_type _dmy_size1 = size1_;
		size_type _dmy_size2 = size2_;
		size_type _dmy_size3 = size3_;
		size1_ = m.size1_;
		size2_ = m.size2_;
		size3_ = m.size3_;
		m.size1_ = _dmy_size1;
		m.size2_ = _dmy_size2;
		m.size3_ = _dmy_size3;
	}

	void clear( )
	{
		base::clear( );
		size1_ = size2_ = size3_ = 0;
	}

	size_type size1( ) const { return( size1_ ); }
	size_type size2( ) const { return( size2_ ); }
	size_type size3( ) const { return( size3_ ); }
	size_type width( ) const { return( size1_ ); }
	size_type height( ) const { return( size2_ ); }
	size_type depth( ) const { return( size3_ ); }

	double reso3( double r3 ){ return( reso3_ = r3 ); }
	double reso3( ) const { return( reso3_ ); }

	// 順方向のランダムアクセスイテレータを返す
	iterator begin( ){ return( iterator( paccess( 0, 0, 0 ), 0, size( ), 0 ) ); }
	const_iterator begin( ) const { return( const_iterator( paccess( 0, 0, 0 ), 0, size( ), 0 ) ); }

	iterator end( ){ return( iterator( paccess( 0, 0, 0 ), size( ), size( ), 0 ) ); }
	const_iterator end( ) const { return( const_iterator( paccess( 0, 0, 0 ), size( ), size( ), 0 ) ); }


	// 逆方向のランダムアクセスイテレータを返す
	reverse_iterator rbegin( ){ return( reverse_iterator( end( ) ) ); }
	const_reverse_iterator rbegin( ) const { return( const_reverse_iterator( end( ) ) ); }

	reverse_iterator rend( ){ return( reverse_iterator( begin( ) ) ); }
	const_reverse_iterator rend( ) const { return( const_reverse_iterator( begin( ) ) ); }

/************************************************************************************************************
**
**      X軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/
	// 順方向のランダムアクセスイテレータを返す
	iterator x_begin( size_type i ){ return( iterator( paccess( i, 0, 0 ), 0, 1, width( ) ) ); }
	const_iterator x_begin( size_type i ) const { return( const_iterator( paccess( i, 0, 0 ), 0, 1, width( ) ) ); }

	iterator x_end( size_type i ){ return( iterator( paccess( i, 0, 0 ), height( ) * depth( ), 1, width( ) ) ); }
	const_iterator x_end( size_type i ) const { return( const_iterator( paccess( i, 0, 0 ), height( ) * depth( ), 1, width( ) ) ); }


	// 逆方向のランダムアクセスイテレータを返す
	reverse_iterator x_rbegin( size_type i ){ return( reverse_iterator( x_end( i ) ) ); }
	const_reverse_iterator x_rbegin( size_type i ) const { return( const_reverse_iterator( x_end( i ) ) ); }

	reverse_iterator x_rend( size_type i ){ return( reverse_iterator( x_begin( i ) ) ); }
	const_reverse_iterator x_rend( size_type i ) const { return( const_reverse_iterator( x_begin( i ) ) ); }


/************************************************************************************************************
**
**      Y軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/
	// 順方向のランダムアクセスイテレータを返す
	iterator y_begin( size_type j ){ return( iterator( paccess( 0, j, 0 ), 0, height( ), width( ) * height( ) ) ); }
	const_iterator y_begin( size_type j ) const { return( const_iterator( paccess( 0, j, 0 ), 0, height( ), width( ) * height( ) ) ); }

	iterator y_end( size_type j ){ return( iterator( paccess( 0, j, 0 ), width( ) * depth( ), height( ), width( ) * height( ) ) ); }
	const_iterator y_end( size_type j ) const { return( const_iterator( paccess( 0, j, 0 ), width( ) * depth( ), height( ), width( ) * height( ) ) ); }


	// 逆方向のランダムアクセスイテレータを返す
	reverse_iterator y_rbegin( size_type j ){ return( reverse_iterator( y_end( j ) ) ); }
	const_reverse_iterator y_rbegin( size_type j ) const { return( const_reverse_iterator( y_end( j ) ) ); }

	reverse_iterator y_rend( size_type j ){ return( reverse_iterator( y_begin( j ) ) ); }
	const_reverse_iterator y_rend( size_type j ) const { return( const_reverse_iterator( y_begin( j ) ) ); }


/************************************************************************************************************
**
**      Z軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/
	// 順方向のランダムアクセスイテレータを返す
	iterator z_begin( size_type k ){ return( iterator( paccess( 0, 0, k ), 0, 1, 1 ) ); }
	const_iterator z_begin( size_type k ) const { return( const_iterator( paccess( 0, 0, k ), 0, 1, 1 ) ); }

	iterator z_end( size_type k ){ return( iterator( paccess( 0, 0, k ), width( ) * height( ), 1, 1 ) ); }
	const_iterator z_end( size_type k ) const { return( const_iterator( paccess( 0, 0, k ), width( ) * height( ), 1, 1 ) ); }


	// 逆方向のランダムアクセスイテレータを返す
	reverse_iterator z_rbegin( size_type k ){ return( reverse_iterator( z_end( k ) ) ); }
	const_reverse_iterator z_rbegin( size_type k ) const { return( const_reverse_iterator( z_end( k )) ); }

	reverse_iterator z_rend( size_type k ){ return( reverse_iterator( z_begin( k ) ) ); }
	const_reverse_iterator z_rend( size_type k ) const { return( const_reverse_iterator( z_begin( k ) ) ); }


public:
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
		return( data_ + i + ( j + k * size2_ ) * size1_ );
	}
	const_pointer paccess( size_type i, size_type j, size_type k ) const
	{
		return( data_ + i + ( j + k * size2_ ) * size1_ );
	}

public:
	reference at( size_type i, size_type j, size_type k )
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( data_[ i + ( j + k * size2_ ) * size1_ ] );
	}

	const_reference at( size_type index, size_type j, size_type k ) const
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( data_[ i + ( j + k * size2_ ) * size1_ ] );
	}

	reference operator ()( size_type i, size_type j, size_type k )
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( data_[ i + ( j + k * size2_ ) * size1_ ] );
	}

	const_reference operator ()( size_type i, size_type j, size_type k ) const
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( data_[ i + ( j + k * size2_ ) * size1_ ] );
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
