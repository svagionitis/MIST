// generated by Fast Light User Interface Designer (fluid) version 1.0104

#ifndef ct_image_window_h
#define ct_image_window_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Button.H>

class parameter_window {
public:
  parameter_window();
  Fl_Double_Window *param_window;
  Fl_Value_Input *width;
  Fl_Value_Input *height;
  Fl_Value_Input *depth;
  Fl_Value_Input *sizeX;
  Fl_Value_Input *sizeY;
  Fl_Value_Input *sizeZ;
  Fl_Value_Input *offset;
  Fl_Return_Button *OK;
private:
  inline void cb_OK_i(Fl_Return_Button*, void*);
  static void cb_OK(Fl_Return_Button*, void*);
public:
  Fl_Button *Cancel;
private:
  inline void cb_Cancel_i(Fl_Button*, void*);
  static void cb_Cancel(Fl_Button*, void*);
  int val;
public:
  int value();
  void hide();
  bool show();
};
#include <FL/Fl_Group.H>
#include "raw_test.h"
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>

class ct_image_window {
public:
  ct_image_window( );
  Fl_Double_Window *image_window;
  ct_draw_area *ct_image;
  static Fl_Menu_Item menu_[];
private:
  inline void cb_Open_i(Fl_Menu_*, void*);
  static void cb_Open(Fl_Menu_*, void*);
  inline void cb_Save_i(Fl_Menu_*, void*);
  static void cb_Save(Fl_Menu_*, void*);
  inline void cb_Median_i(Fl_Menu_*, void*);
  static void cb_Median(Fl_Menu_*, void*);
  inline void cb_Median1_i(Fl_Menu_*, void*);
  static void cb_Median1(Fl_Menu_*, void*);
  inline void cb_Median2_i(Fl_Menu_*, void*);
  static void cb_Median2(Fl_Menu_*, void*);
public:
  Fl_Value_Slider *WL;
private:
  inline void cb_WL_i(Fl_Value_Slider*, void*);
  static void cb_WL(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *WW;
private:
  inline void cb_WW_i(Fl_Value_Slider*, void*);
  static void cb_WW(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *Indx;
private:
  inline void cb_Indx_i(Fl_Value_Slider*, void*);
  static void cb_Indx(Fl_Value_Slider*, void*);
public:
  void show();
};
#endif
