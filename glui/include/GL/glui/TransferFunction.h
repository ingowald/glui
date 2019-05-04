/****************************************************************************

  GLUI_Context *User Interface Toolkit
  ---------------------------

     glui.h - Main (and only) external header for 
        GLUI_Context *User Interface Toolkit

          --------------------------------------------------

  Copyright (c) 1998 Paul Rademacher

  WWW:    http://sourceforge.net/projects/glui/
  Forums: http://sourceforge.net/forum/?group_id=92496

  This software is provided 'as-is', without any express or implied 
  warranty. In no event will the authors be held liable for any damages 
  arising from the use of this software. 

  Permission is granted to anyone to use this software for any purpose, 
  including commercial applications, and to alter it and redistribute it 
  freely, subject to the following restrictions: 

  1. The origin of this software must not be misrepresented; you must not 
  claim that you wrote the original software. If you use this software 
  in a product, an acknowledgment in the product documentation would be 
  appreciated but is not required. 
  2. Altered source versions must be plainly marked as such, and must not be 
  misrepresented as being the original software. 
  3. This notice may not be removed or altered from any source distribution. 

*****************************************************************************/

#pragma once

namespace glui {

  /*!  An onscreen translation controller--allows the user to interact
    with a 3D translation.
  */
  class GLUI_API GLUI_TransferFunction : public GLUI_Control
  {
  public:
    int  mouse_down_handler( int local_x, int local_y );
    int  mouse_up_handler( int local_x, int local_y, bool inside );
    int  mouse_held_down_handler( int local_x, int local_y, bool inside );
    // int  iaction_special_handler( int key, int modifiers );
    // void iaction_init( void ) { }
    // void iaction_draw_active_area_persp( void );
    // void iaction_draw_active_area_ortho( void );
    // void iaction_dump( FILE *output );

    // void set_speed( float s ) { scale_factor = s; }

    // void setup_texture( void );
    // void setup_lights( void );
    // void draw_2d_arrow( int radius, int filled, int orientation ); 
    // void draw_2d_x_arrows( int radius );
    // void draw_2d_y_arrows( int radius );
    // void draw_2d_z_arrows( int radius );
    // void draw_2d_xy_arrows( int radius );
    // void drawXF();
    virtual void draw( int x, int y ) override;
    
    int  get_mouse_code( int x, int y );

    /* Float array is either a single float (for single-axis controls),
       or two floats for X and Y (if an XY controller) */

    void  set_one_val( float val, int index );

    GLUI_TransferFunction(GLUI_Node *parent,
                          const char *name,
                          float *live_array,
                          int live_array_size,
                          int id=-1,
                          CallBack callback=CallBack()	);
    GLUI_TransferFunction( void ) { common_init(); }

    int lastMouseDownPosX { -1 };
    int lastMouseDownPosY { -1 };
    // int down_x, down_y;
    // float scale_factor;
    // GLUquadricObj *quadObj;
    // int   trans_mouse_code;
    // float orig_x, orig_y, orig_z;
    // int   locked { GLUI_TRANSLATION_LOCK_NONE };

    float *xf_alpha { nullptr };
    int    xf_alpha_size { 0 };

    
  protected:
    static const int outer_border_width = 2;
    static const int inner_border_width = 2;
    static const int total_border_width = outer_border_width+inner_border_width;
    
    void common_init() {
      glui_format_str( name, "TransferFunction: %p", this );
      w                   = 6*GLUI_MOUSE_INTERACTION_WIDTH;
      h                   = 3*GLUI_MOUSE_INTERACTION_HEIGHT;
      can_activate        = true;
      live_type           = GLUI_LIVE_NONE;//FLOAT_ARRAY;
      float_array_size    = 0;//128;
      alignment           = GLUI_ALIGN_CENTER;
      // scale_factor        = 1.0;
      // quadObj             = NULL;
      // trans_mouse_code    = GLUI_TRANSLATION_MOUSE_NONE;
    }
  };

}
