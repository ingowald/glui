/****************************************************************************

  GLUI_Context *User Interface Toolkit
  ---------------------------

     glui_translation - GLUI_TransferFunction control class


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

#include "GL/glui.h"
#include "GL/glui/TransferFunction.h"
#include "glui_internal.h"
#include "algebra3.h"

namespace glui {
  /********************** GLUI_TransferFunction::GLUI_TransferFunction() ***/

  GLUI_TransferFunction::GLUI_TransferFunction(GLUI_Node *parent,
                                               const char *name,
                                               float *live_array,
                                               int live_array_size,
                                               int id,
                                               CallBack cb)
  {
    common_init();

    xf_alpha_size = live_array_size;
    if (live_array)
      xf_alpha = live_array;
    else
      xf_alpha = new float[live_array_size];
    
    user_id    = id;
    set_name( name );
    callback    = cb;
    parent->add_control( this );

    for (int i=0;i<live_array_size;i++)
      live_array[i] = i/float(live_array_size-1);

    init_live();
  }

  /********************** GLUI_TransferFunction::iaction_mouse_down_handler() ***/
  /*  These are really in local coords (5/10/99)                            */

  int    GLUI_TransferFunction::iaction_mouse_down_handler( int local_x,
                                                       int local_y )
  {
    int center_x, center_y;

    down_x = local_x;
    down_y = local_y;

    glutSetCursor( GLUT_CURSOR_LEFT_RIGHT );
    orig_x = float_array_val[0];

    trans_mouse_code = 1;
    redraw();

    return false;
  }


  /*********************** GLUI_TransferFunction::iaction_mouse_up_handler() **********/

  int    GLUI_TransferFunction::iaction_mouse_up_handler( int local_x, int local_y,
                                                     bool inside )
  {
    trans_mouse_code = GLUI_TRANSLATION_MOUSE_NONE;
    locked = GLUI_TRANSLATION_LOCK_NONE;

    redraw();

    return false;
  }


  /******************* GLUI_TransferFunction::iaction_mouse_held_down_handler() ******/

  int    GLUI_TransferFunction::iaction_mouse_held_down_handler( int local_x, int local_y,
                                                            bool inside)
  {
    float x_off, y_off;
    float off_array[2];

    x_off = scale_factor * (float)(local_x - down_x);
    y_off = -scale_factor * (float)(local_y - down_y);

    if ( context->curr_modifiers & GLUT_ACTIVE_SHIFT ) {
      x_off *= 100.0f;
      y_off *= 100.0f;
    }
    else if ( context->curr_modifiers & GLUT_ACTIVE_CTRL ) {
      x_off *= .01f;
      y_off *= .01f;
    }


      off_array[0] = x_off + orig_x;

    set_float_array_val( (float*) &off_array[0] );

    return false;
  }


  /******************** GLUI_TransferFunction::iaction_draw_active_area_persp() **************/

  void    GLUI_TransferFunction::iaction_draw_active_area_persp( void )
  {
  }


  /******************** GLUI_TransferFunction::iaction_draw_active_area_ortho() **********/

  void    GLUI_TransferFunction::iaction_draw_active_area_ortho( void )
  {
    /********* Draw emboss circles around arcball control *********/
    float radius;
    radius = (float)(h-22)/2.0;  /*  MIN((float)w/2.0, (float)h/2.0); */
    glLineWidth( 1.0 );

    draw_emboss_box((int)-radius-2, (int)radius+2,
                    (int)-radius-2, (int)radius+2);
    
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    drawXF();
    glPopMatrix();
  }


  /******************************** GLUI_TransferFunction::iaction_dump() **********/

  void     GLUI_TransferFunction::iaction_dump( FILE *output )
  {
  }


  /******************** GLUI_TransferFunction::iaction_special_handler() **********/

  int    GLUI_TransferFunction::iaction_special_handler( int key,int modifiers )
  {

    return false;
  }




  void GLUI_TransferFunction::drawXF()
  {

    glLineWidth( 1.0 );
    /*** Draw arrow outline ***/
    glBegin( GL_LINES );

    PRINT(float_array_size);
    // glMatrixMode(GL_MODELVIEW);
    // glTranslatef(-50,-50,0);
    for (int i=0;i<xf_alpha_size;i++) {
      glColor3f(1,0,0);
      glVertex2f(i-64,128-64);
      glVertex2f(i-64,128-64-128*xf_alpha[i]);
    }

    glEnd();
    
    printf("drawing xf\n");
  }

  /*************************** GLUI_TransferFunction::get_mouse_code() *************/

  int    GLUI_TransferFunction::get_mouse_code( int x, int y )
  {
    if ( x == 0 AND y < 0 )
      return GLUI_TRANSLATION_MOUSE_DOWN;
    else if ( x == 0 AND y > 0 )
      return GLUI_TRANSLATION_MOUSE_UP;
    else if ( x > 0 AND y == 0 )
      return GLUI_TRANSLATION_MOUSE_LEFT;
    else if ( x < 0 AND y == 0 )
      return GLUI_TRANSLATION_MOUSE_RIGHT;
    else if ( x < 0 AND y < 0 )
      return GLUI_TRANSLATION_MOUSE_DOWN_LEFT;
    else if ( x < 0 AND y > 0 )
      return GLUI_TRANSLATION_MOUSE_DOWN_RIGHT;
    else if ( x > 0 AND y < 0 )
      return GLUI_TRANSLATION_MOUSE_UP_LEFT;
    else if ( x > 0 AND y > 0 )
      return GLUI_TRANSLATION_MOUSE_UP_RIGHT;


    return GLUI_TRANSLATION_MOUSE_NONE;
  }


  /******************************* GLUI_TransferFunction::set_one_val() ****/

  void  GLUI_TransferFunction::set_one_val( float val, int index )
  {
    float_array_val[index] = val;    /* set value in array              */

    /*** The code below is like output_live, except it only operates on
         a single member of the float array (given by 'index') instead of
         outputting the entire array   ****/

    if ( ptr_val == NULL OR NOT live_inited )
      return;

    float *fp = (float*) ptr_val;
    fp[index]                    = float_array_val[index];
    last_live_float_array[index] = float_array_val[index];

    /** Update the main gfx window? **/
    if (context) {
      context->post_update_main_gfx();
    }
  }
}
