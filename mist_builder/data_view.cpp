#include "config.h"

#include "data_view.h"


#include <mist/draw.h>


class nofocus_button : public FXButton
{
	// Macro for class hierarchy declarations
	FXDECLARE( nofocus_button )

public:
	typedef FXButton base;

protected:
	nofocus_button( ){}
	nofocus_button( const nofocus_button& ){}

public:
	/// Construct a slider widget
	nofocus_button( FXComposite *p, const FXString &text, FXIcon *ic = NULL, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = BUTTON_NORMAL,
					FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
					FXint pl = DEFAULT_PAD, FXint pr = DEFAULT_PAD,
					FXint pt = DEFAULT_PAD, FXint pb = DEFAULT_PAD )
					: base( p, text, ic, tgt, sel, opts, x, y, w, h, pl, pr, pt, pb ){ }

	virtual FXbool canFocus( ) const { return( false ); }
};

class nofocus_toggle_button : public FXToggleButton
{
	// Macro for class hierarchy declarations
	FXDECLARE( nofocus_toggle_button )

public:
	typedef FXToggleButton base;

protected:
	nofocus_toggle_button( ){}
	nofocus_toggle_button( const nofocus_button& ){}

public:
	/// Construct a slider widget
	nofocus_toggle_button( FXComposite *p, const FXString &text1, const FXString &text2, FXIcon *ic1 = NULL, FXIcon *ic2 = NULL,
					FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = TOGGLEBUTTON_NORMAL,
					FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
					FXint pl = DEFAULT_PAD, FXint pr = DEFAULT_PAD,
					FXint pt = DEFAULT_PAD, FXint pb = DEFAULT_PAD )
					: base( p, text1, text2, ic1, ic2, tgt, sel, opts, x, y, w, h, pl, pr, pt, pb ){ }

	virtual FXbool canFocus( ) const { return( false ); }
};


FXDEFMAP( data_view ) data_view_map[] =
	{
		//________Message_Type_____________________ID_______________Message_Handler_______
		FXMAPFUNC ( SEL_PAINT,				data_view::ID_CANVAS,			data_view::onPaint ),
		FXMAPFUNC ( SEL_LEFTBUTTONPRESS,	data_view::ID_CANVAS,			data_view::onMouseDown ),
		FXMAPFUNC ( SEL_RIGHTBUTTONPRESS,	data_view::ID_CANVAS,			data_view::onMouseDown ),
		FXMAPFUNC ( SEL_LEFTBUTTONRELEASE,	data_view::ID_CANVAS,			data_view::onMouseUp ),
		FXMAPFUNC ( SEL_RIGHTBUTTONRELEASE,	data_view::ID_CANVAS,			data_view::onMouseUp ),
		FXMAPFUNC ( SEL_MOTION,				data_view::ID_CANVAS,			data_view::onMouseMove ),
		FXMAPFUNC ( SEL_KEYPRESS,			data_view::ID_CANVAS,			data_view::onKeyDown ),
		FXMAPFUNC ( SEL_KEYRELEASE,			data_view::ID_CANVAS,			data_view::onKeyUp ),
		FXMAPFUNC ( SEL_COMMAND,			data_view::ID_INTERPOLATE_MODE,	data_view::onInterpolateChange ),
		FXMAPFUNC ( SEL_COMMAND,			data_view::ID_DRAW_CROSS,		data_view::onDrawCrossChange ),
		FXMAPFUNC ( SEL_CHANGED,			data_view::ID_ZOOM_SLIDER,		data_view::onZoomChanged ),
	};


FXIMPLEMENT( data_view, data_view::base, data_view_map, ARRAYNUMBER( data_view_map ) )

FXIMPLEMENT( invert_slider, invert_slider::base, 0, 0 )

FXIMPLEMENT( nofocus_button, nofocus_button::base, 0, 0 )

FXIMPLEMENT( nofocus_toggle_button, nofocus_toggle_button::base, 0, 0 )



/* created by reswrap from file drag.cur */
const unsigned char drag[]={
  0x00,0x00,0x02,0x00,0x01,0x00,0x20,0x20,0x00,0x00,0x08,0x00,0x0a,0x00,0x30,0x01,
  0x00,0x00,0x16,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x40,0x00,
  0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xf0,0x00,0x00,0x03,0xf0,0x00,0x00,0x07,0xf0,
  0x00,0x00,0x0f,0xf0,0x00,0x00,0x1f,0xf8,0x00,0x00,0x1f,0xf8,0x00,0x00,0x3f,0xfc,
  0x00,0x00,0x77,0xfc,0x00,0x00,0x67,0xfc,0x00,0x00,0x07,0xfe,0x00,0x00,0x0d,0xb6,
  0x00,0x00,0x0d,0xb2,0x00,0x00,0x19,0xb0,0x00,0x00,0x19,0xb0,0x00,0x00,0x01,0x80,
  0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xf8,0x07,0xff,0xff,0xf8,0x07,0xff,0xff,0xf0,0x07,
  0xff,0xff,0xe0,0x07,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,0x03,0xff,0xff,0x80,0x01,
  0xff,0xff,0x00,0x01,0xff,0xff,0x00,0x01,0xff,0xff,0x90,0x00,0xff,0xff,0xe0,0x00,
  0xff,0xff,0xe0,0x00,0xff,0xff,0xc0,0x05,0xff,0xff,0xc0,0x07,0xff,0xff,0xe4,0x0f,
  0xff,0xff,0xfe,0x7f,0xff,0xff
  };

/* created by reswrap from file dragging.cur */
const unsigned char dragging[]={
  0x00,0x00,0x02,0x00,0x01,0x00,0x20,0x20,0x00,0x00,0x08,0x00,0x0a,0x00,0x30,0x01,
  0x00,0x00,0x16,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x40,0x00,
  0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xf0,0x00,0x00,0x03,0xf0,0x00,0x00,0x07,0xf0,
  0x00,0x00,0x0f,0xf0,0x00,0x00,0x1f,0xf8,0x00,0x00,0x1f,0xf8,0x00,0x00,0x1f,0xfc,
  0x00,0x00,0x17,0xfc,0x00,0x00,0x07,0xfc,0x00,0x00,0x07,0xfc,0x00,0x00,0x0d,0xb4,
  0x00,0x00,0x0d,0xb0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xf8,0x07,0xff,0xff,0xf8,0x07,0xff,0xff,0xf0,0x07,
  0xff,0xff,0xe0,0x07,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,0x03,0xff,0xff,0xc0,0x01,
  0xff,0xff,0xc0,0x01,0xff,0xff,0xe0,0x01,0xff,0xff,0xf0,0x01,0xff,0xff,0xe0,0x01,
  0xff,0xff,0xe0,0x03,0xff,0xff,0xf2,0x4f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff
  };


/* created by reswrap from file cross.gif */
const unsigned char cross_icon[]={
  0x47,0x49,0x46,0x38,0x39,0x61,0x0f,0x00,0x0f,0x00,0x91,0x00,0x00,0x00,0x12,0xff,
  0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x21,0xf9,0x04,0x01,0x00,0x00,0x02,
  0x00,0x2c,0x00,0x00,0x00,0x00,0x0f,0x00,0x0f,0x00,0x00,0x02,0x1b,0x8c,0x1f,0x80,
  0xbb,0xca,0xef,0x5e,0x93,0x2c,0x52,0x63,0x6f,0xc6,0xa0,0xfb,0xff,0x41,0x17,0xb2,
  0x49,0xa5,0x38,0x62,0xa9,0xba,0x96,0x05,0x00,0x3b
  };



// Construct a data_view
data_view::data_view( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h, FXint pl, FXint pr, FXint pt, FXint pb, FXint hs, FXint vs )
				: base( p, opts, x, y, w, h, pl, pr, pt, pb, hs, vs ),
				zoom_( 1.0 ),
				interpolate_( true ),
				draw_cross_( false )
{
	cursors_.push_back( new FXCursor( getApp( ), CURSOR_ARROW ) );
	cursors_.push_back( new FXCURCursor( getApp( ), drag ) );
	cursors_.push_back( new FXCURCursor( getApp( ), dragging ) );

	icons_.push_back( new FXGIFIcon( getApp( ), cross_icon ) );


	// このウィンドウからメッセージを送る先の設定
	setTarget( tgt );
	setSelector( sel );

	// クライアント領域に部品を配置する
	FXHorizontalFrame *client = new FXHorizontalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0 );
	{
		// メインの描画部分と設定ダイアログ
		{
			glvisual_ = new FXGLVisual( getApp(), VISUAL_DOUBLEBUFFER );
			canvas_ = new FXGLCanvas( client, glvisual_, this, ID_CANVAS, LAYOUT_TOP | LAYOUT_LEFT | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0 );
		}

		// 左側の設定ダイアログを配置する
		{
			FXVerticalFrame *o = new FXVerticalFrame( client, LAYOUT_SIDE_RIGHT | LAYOUT_FILL_Y | LAYOUT_FIX_WIDTH, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0 );
			{
				FXSpring *o1 = new FXSpring( o, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
				{
					( new nofocus_toggle_button( o1, "S", "S", 0, 0, this, ID_INTERPOLATE_MODE, TOGGLEBUTTON_KEEPSTATE | FRAME_SUNKEN | LAYOUT_TOP | LAYOUT_FILL_X ) )->setState( interpolate_ );
					( new nofocus_toggle_button( o1, "", "", icons_[ 0 ], icons_[ 0 ], this, ID_DRAW_CROSS, TOGGLEBUTTON_KEEPSTATE | FRAME_SUNKEN | LAYOUT_TOP | LAYOUT_FILL_X ) )->setState( draw_cross_ );
				}
				FXSpring *o2 = new FXSpring( o, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
				{
					new FXHorizontalSeparator( o2 );
					zoom_slider_ = new invert_slider( o2, this, ID_ZOOM_SLIDER, LAYOUT_CENTER_X | LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | SLIDER_VERTICAL | SLIDER_INVERT, 0, 0, 10, 0, 0, 0, 0, 0 );
					FXint lo = 0;
					FXint hi = static_cast< FXint >( ( zoom_maximum - zoom_minimum ) / zoom_step + 1.0 );
					zoom_slider_->setRange( lo, hi );
					zoom_slider_->setValue( lo );
				}
			}
		}

	}
}

data_view::~data_view( )
{
	{
		std::vector< FXCursor * >::iterator ite = cursors_.begin( );
		for( ; ite != cursors_.end( ) ; ite++ )
		{
			delete *ite;
		}
	}

	{
		std::vector< FXIcon * >::iterator ite = icons_.begin( );
		for( ; ite != icons_.end( ) ; ite++ )
		{
			delete *ite;
		}
	}
}

void data_view::create( )
{
	base::create( );

	cursors_[ 0 ]->create( ); 
	cursors_[ 1 ]->create( );
	cursors_[ 2 ]->create( );
}



// Mouse button was pressed somewhere
long data_view::onMouseDown( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	mark_position_ = screen2point( point2( e.win_x, e.win_y ) );

	set_cursors( ( e.state & ALTMASK ) != 0 );

	return( 1 );
}

// The mouse has moved, draw a line
long data_view::onMouseMove( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	point2 pt = screen2point( point2( e.win_x, e.win_y ) );

	set_cursors( ( e.state & ALTMASK ) != 0 );

	if( e.state & ALTMASK && e.state & LEFTBUTTONMASK )
	{
		current_position_ += mark_position_ - pt;

		draw_image( );
	}

	return( 1 );
}

// The mouse button was released again
long data_view::onMouseUp( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	set_cursors( ( e.state & ALTMASK ) != 0 );

	return( 1 );
}

// The mouse button was released again
long data_view::onKeyDown( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	set_cursors( ( e.state & ALTMASK ) != 0 );

	return( 1 );
}

// The mouse button was released again
long data_view::onKeyUp( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	set_cursors( ( e.state & ALTMASK ) != 0 );

	return( 1 );
}

// Paint the canvas
long data_view::onPaint( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent *ev = ( FXEvent * )ptr;

	draw_image( );

	return( 1 );
}


long data_view::onZoomChanged( FXObject *obj, FXSelector sel, void *ptr )
{
	zoom_ = zoom_slider_->getValue( ) * zoom_step + zoom_minimum;
	draw_image( );

	return( 1 );
}


long data_view::onInterpolateChange( FXObject *obj, FXSelector sel, void *ptr )
{
	interpolate_ = !interpolate_;

	draw_image( );

	return( 1 );
}

long data_view::onDrawCrossChange( FXObject *obj, FXSelector sel, void *ptr )
{
	draw_cross_ = !draw_cross_;

	draw_image( );

	return( 1 );
}

void data_view::draw_image( )
{
	canvas_->makeCurrent( );

	mist::draw_image( buffer_, canvas_->getWidth( ), canvas_->getHeight( ),
						zoom_, current_position_.x, current_position_.y,
						background_color_.r, background_color_.g, background_color_.b,
						interpolate_ );

	if( draw_cross_ )
	{
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity( );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity( );
		glViewport( 0, 0, static_cast< GLint >( canvas_->getWidth( ) ), static_cast< GLint >( canvas_->getHeight( ) ) );

		// 画面の中央に＋印を描画する
		double len = 8.0;
		double x_unit = 1.0 / static_cast< double >( canvas_->getWidth( ) );
		double y_unit = 1.0 / static_cast< double >( canvas_->getHeight( ) );
		double x_len = len * x_unit;
		double y_len = len * y_unit;
		// カーソルの色を設定
		//glColor3d( 0.0, 0.0, 1.0 );
		glColor3d( 1.0, 1.0, 0.0 );

		glBegin( GL_LINES );
			glVertex2d( -x_len, 0.0 );
			glVertex2d(  x_len + x_unit / 0.5, 0.0 );
		glEnd( );
		glBegin( GL_LINES );
			glVertex2d( 0.0, -y_len );
			glVertex2d( 0.0,  y_len + y_unit / 0.5 );
		glEnd( );
	}

	canvas_->swapBuffers( );
}

void data_view::draw_image( const data_type &buf )
{
	//// 設定を初期値に戻す
	//zoom_ = 1.0;
	//zoom_slider_->setValue( 0 );

	//current_position_ = point2( 0, 0 );

	if( !buf.mono_image_.empty( ) )
	{
		buffer_ = buf.mono_image_;
	}
	else if( !buf.gray_image_.empty( ) )
	{
		buffer_ = buf.gray_image_;
	}
	else if( !buf.color_image_.empty( ) )
	{
		buffer_ = buf.color_image_;
	}
	else
	{
		buffer_.clear( );
	}

	draw_image( );
}


void data_view::draw_image( const filter *pf )
{
	draw_image( pf->data( ) );
}

void data_view::set_cursors( bool is_drag )
{
	if( is_drag )
	{
		canvas_->setDefaultCursor( cursors_[ 1 ] );
		canvas_->setDragCursor( cursors_[ 2 ] );
	}
	else
	{
		canvas_->setDefaultCursor( cursors_[ 0 ] );
		canvas_->setDragCursor( cursors_[ 0 ] );
	}
}

// 出力座標はスクリーン上の点で，単位は画素
point2 data_view::point2screen( const point2 &pt ) const
{
	// 画面座標から画像座標に変換する
	return( mist::point2screen( point2( pt.x, pt.y ), buffer_.width( ), buffer_.height( ),
					buffer_.reso2( ) / buffer_.reso1( ), canvas_->getWidth( ), canvas_->getHeight( ), zoom_, current_position_.x, current_position_.y ) );
}

// 入力座標はスクリーン上の点で，単位は画素
point2 data_view::screen2point( const point2 &pt ) const
{
	// 画面座標から画像座標に変換する
	return( mist::screen2point( pt, buffer_.width( ), buffer_.height( ), buffer_.reso2( ) / buffer_.reso1( ),
										canvas_->getWidth( ), canvas_->getHeight( ), zoom_, current_position_.x, current_position_.y ) );
}


