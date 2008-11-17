/* 
 * xgetcc.c - get X cursor's geometry/color/position.
 *
 * Most of the code here were shamelessly stolen^H^H^H^H^H^Hcopied
 * from ImageMagick's xwindow.c  (See LICENSE).
 *
 * Copyright (C) 2008 Nazri Ramliy.
 *
 * Tue Oct 21 06:40:22 MYT 2008
 */ 

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int show_color_at(int x, int y) {
  Window root_window;
	Display *display = XOpenDisplay(NULL);

  XWindowAttributes wa;

  root_window=XRootWindow(display,XDefaultScreen(display));
  Status s = XGetWindowAttributes(display, root_window, &wa);

  if (s == BadDrawable) {
    fprintf(stderr, "bad drawable\n");
    exit(1);
  }
  if (s == BadWindow) {
    fprintf(stderr, "bad window\n");
    exit(1);
  }

  XColor color;
  XImage *ximage;
  ximage = XGetImage(display,root_window,x,y,1,1,AllPlanes,ZPixmap);
  if (ximage == (XImage *) NULL) {
    fprintf(stderr, "Got null image!\n");
    exit(1);
  }
  color.pixel=XGetPixel(ximage,0,0);
  XDestroyImage(ximage);
  /*
    Match color against the color database.
  */
  (void) XQueryColor(display,wa.colormap,&color);
  int red = color.red / 257;
  int green = color.green / 257;
  int blue = color.blue / 257;
  printf("#%02x%02x%02x", red, green, blue);
  return 1;
}

typedef struct _RectangleInfo
{
  unsigned long
    width,
    height;

  long
    x,
    y;
} RectangleInfo;

typedef enum
{
  UndefinedEvents,
  NoEvents = 0x0000,
  TraceEvent = 0x0001,
  AnnotateEvent = 0x0002,
  BlobEvent = 0x0004,
  CacheEvent = 0x0008,
  CoderEvent = 0x0010,
  ConfigureEvent = 0x0020,
  DeprecateEvent = 0x0040,
  DrawEvent = 0x0080,
  ExceptionEvent = 0x0100,
  LocaleEvent = 0x0200,
  ModuleEvent = 0x0400,
  ResourceEvent = 0x0800,
  TransformEvent = 0x1000,
  UserEvent = 0x2000,
  WandEvent = 0x4000,
  X11Event = 0x8000,
  AllEvents = 0x7fffffff
} LogEventType;
/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   X G e t S u b w i n d o w                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  XGetSubwindow() returns the subwindow of a window chosen the user with the
%  pointer and a button press.
%
%  The format of the XGetSubwindow method is:
%
%      Window XGetSubwindow(Display *display,Window window,int x,int y)
%
%  A description of each parameter follows:
%
%    o subwindow: XGetSubwindow() returns NULL if no subwindow is found
%      otherwise the subwindow is returned.
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o window: Specifies a pointer to a Window.
%
%    o x: the x coordinate of the pointer relative to the origin of the
%      window.
%
%    o y: the y coordinate of the pointer relative to the origin of the
%      window.
%
%
*/
static Window XGetSubwindow(Display *display,Window window,int x,int y)
{
  int
    x_offset,
    y_offset;

  Status
    status;

  Window
    source_window,
    target_window;

  assert(display != (Display *) NULL);
  source_window=XRootWindow(display,XDefaultScreen(display));
  if (window == (Window) NULL)
    return(source_window);
  target_window=window;
  for ( ; ; )
  {
    status=XTranslateCoordinates(display,source_window,window,x,y,
      &x_offset,&y_offset,&target_window);
    if (status != True)
      break;
    if (target_window == (Window) NULL)
      break;
    source_window=window;
    window=target_window;
    x=x_offset;
    y=y_offset;
  }
  if (target_window == (Window) NULL)
    target_window=window;
  return(target_window);
}
/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X M a k e C u r s o r                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  XMakeCursor() creates a crosshairs X11 cursor.
%
%  The format of the XMakeCursor method is:
%
%      Cursor XMakeCursor(Display *display,Window window,Colormap colormap,
%        char *background_color,char *foreground_color)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o window: Specifies the ID of the window for which the cursor is
%      assigned.
%
%    o colormap: Specifies the ID of the colormap from which the background
%      and foreground color will be retrieved.
%
%    o background_color: Specifies the color to use for the cursor background.
%
%    o foreground_color: Specifies the color to use for the cursor foreground.
%
*/
Cursor XMakeCursor(Display *display,Window window,
  Colormap colormap,char *background_color,char *foreground_color)
{
#define scope_height 17
#define scope_x_hot 8
#define scope_y_hot 8
#define scope_width 17

  static const unsigned char
    scope_bits[] =
    {
      0x80, 0x03, 0x00, 0x80, 0x02, 0x00, 0x80, 0x02, 0x00, 0x80, 0x02,
      0x00, 0x80, 0x02, 0x00, 0x80, 0x02, 0x00, 0x80, 0x02, 0x00, 0x7f,
      0xfc, 0x01, 0x01, 0x00, 0x01, 0x7f, 0xfc, 0x01, 0x80, 0x02, 0x00,
      0x80, 0x02, 0x00, 0x80, 0x02, 0x00, 0x80, 0x02, 0x00, 0x80, 0x02,
      0x00, 0x80, 0x02, 0x00, 0x80, 0x03, 0x00
    },
    scope_mask_bits[] =
    {
      0xc0, 0x07, 0x00, 0xc0, 0x07, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06,
      0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xff, 0xfe, 0x01, 0x7f,
      0xfc, 0x01, 0x03, 0x80, 0x01, 0x7f, 0xfc, 0x01, 0xff, 0xfe, 0x01,
      0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06, 0x00, 0xc0, 0x06,
      0x00, 0xc0, 0x07, 0x00, 0xc0, 0x07, 0x00
    };

  Cursor
    cursor;

  Pixmap
    mask,
    source;

  XColor
    background,
    foreground;

  assert(display != (Display *) NULL);
  assert(window != (Window) NULL);
  assert(colormap != (Colormap) NULL);
  assert(background_color != (char *) NULL);
  assert(foreground_color != (char *) NULL);
  //(void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",background_color);
  source=XCreateBitmapFromData(display,window,(char *) scope_bits,scope_width,
    scope_height);
  mask=XCreateBitmapFromData(display,window,(char *) scope_mask_bits,
    scope_width,scope_height);
  if ((source == (Pixmap) NULL) || (mask == (Pixmap) NULL))
    {
      printf("UnableToCreatePixmap\n");
      return((Cursor) NULL);
    }
  (void) XParseColor(display,colormap,background_color,&background);
  (void) XParseColor(display,colormap,foreground_color,&foreground);
  cursor=XCreatePixmapCursor(display,source,mask,&foreground,&background,
    scope_x_hot,scope_y_hot);
  (void) XFreePixmap(display,source);
  (void) XFreePixmap(display,mask);
  return(cursor);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X S e l e c t W i n d o w                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  XSelectWindow() allows a user to select a window using the mouse.  If the
%  mouse moves, a cropping rectangle is drawn and the extents of the rectangle
%  is returned in the crop_info structure.
%
%  The format of the XSelectWindow function is:
%
%      target_window=XSelectWindow(display,crop_info)
%
%  A description of each parameter follows:
%
%    o window: XSelectWindow returns the window id.
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o crop_info: Specifies a pointer to a RectangleInfo structure.  It
%      contains the extents of any cropping rectangle.
%
%
*/
static Window XSelectWindow(Display *display,RectangleInfo *crop_info)
{
#define MinimumCropArea  (unsigned int) 9

  Cursor
    target_cursor;

  GC
    annotate_context;

  int
    presses,
    x_offset,
    y_offset;

  Status
    status;

  Window
    root_window,
    target_window;

  XEvent
    event;

  XGCValues
    context_values;

  /*
    Initialize graphic context.
  */
  root_window=XRootWindow(display,XDefaultScreen(display));
  context_values.background=XBlackPixel(display,XDefaultScreen(display));
  context_values.foreground=XWhitePixel(display,XDefaultScreen(display));
  context_values.function=GXinvert;
  context_values.plane_mask=
    context_values.background ^ context_values.foreground;
  context_values.subwindow_mode=IncludeInferiors;
  annotate_context=XCreateGC(display,root_window,(unsigned long) (GCBackground |
    GCForeground | GCFunction | GCSubwindowMode),&context_values);
  if (annotate_context == (GC) NULL)
    return(False);
  /*
    Grab the pointer using target cursor.
  */
  target_cursor=XMakeCursor(display,root_window,XDefaultColormap(display,
    XDefaultScreen(display)),(char * ) "white",(char * ) "black");
  status=XGrabPointer(display,root_window,False,(unsigned int)
    (ButtonPressMask | ButtonReleaseMask | ButtonMotionMask),GrabModeSync,
    GrabModeAsync,root_window,target_cursor,CurrentTime);
  if (status != GrabSuccess)
    {
      printf("UnableToGrabMouse\n");
      return((Window) NULL);
    }
  /*
    Select a window.
  */
  crop_info->width=0;
  crop_info->height=0;
  presses=0;
  target_window=(Window) NULL;
  x_offset=0;
  y_offset=0;
  do
  {
    if ((crop_info->width*crop_info->height) >= MinimumCropArea)
      (void) XDrawRectangle(display,root_window,annotate_context,
        (int) crop_info->x,(int) crop_info->y,(unsigned int) crop_info->width-1,
        (unsigned int) crop_info->height-1);
    /*
      Allow another event.
    */
    (void) XAllowEvents(display,SyncPointer,CurrentTime);
    (void) XWindowEvent(display,root_window,ButtonPressMask |
      ButtonReleaseMask | ButtonMotionMask,&event);
    if ((crop_info->width*crop_info->height) >= MinimumCropArea)
      (void) XDrawRectangle(display,root_window,annotate_context,
        (int) crop_info->x,(int) crop_info->y,(unsigned int) crop_info->width-1,
        (unsigned int) crop_info->height-1);
    switch (event.type)
    {
      case ButtonPress:
      {
        target_window=XGetSubwindow(display,event.xbutton.subwindow,
          event.xbutton.x,event.xbutton.y);
        if (target_window == (Window) NULL)
          target_window=root_window;
        x_offset=event.xbutton.x_root;
        y_offset=event.xbutton.y_root;
        crop_info->x=x_offset;
        crop_info->y=y_offset;
        crop_info->width=0;
        crop_info->height=0;
        presses++;
        break;
      }
      case ButtonRelease:
      {
        presses--;
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event)) ;
        crop_info->x=event.xmotion.x;
        crop_info->y=event.xmotion.y;
        /*
          Check boundary conditions.
        */
        if ((int) crop_info->x < x_offset)
          crop_info->width=(unsigned int) (x_offset-crop_info->x);
        else
          {
            crop_info->width=(unsigned int) (crop_info->x-x_offset);
            crop_info->x=x_offset;
          }
        if ((int) crop_info->y < y_offset)
          crop_info->height=(unsigned int) (y_offset-crop_info->y);
        else
          {
            crop_info->height=(unsigned int) (crop_info->y-y_offset);
            crop_info->y=y_offset;
          }
      }
      default:
        break;
    }
  } while ((target_window == (Window) NULL) || (presses > 0));
  (void) XUngrabPointer(display,CurrentTime);
  (void) XFreeCursor(display,target_cursor);
  (void) XFreeGC(display,annotate_context);
  if ((crop_info->width*crop_info->height) < MinimumCropArea)
    {
      crop_info->width=0;
      crop_info->height=0;
    }
  if ((crop_info->width != 0) && (crop_info->height != 0))
    target_window=root_window;
  return(target_window);
}

void showCropInfo(RectangleInfo *crop_info) {
  int tlx = crop_info->x;
  int tly = crop_info->y;
  int brx = tlx+crop_info->width;
  int bry = tly+crop_info->height;
  if(tlx == brx && tly == bry) {
    printf("%3d %3d ", tlx, tly);
    show_color_at(tlx, tly);
	printf("\n");
  } else {
    printf("%dx%d+%d+%d", crop_info->width, crop_info->height,
      crop_info->x, crop_info->y);
	printf("\n");
  }
}

void showColor(RectangleInfo *crop_info) {
  int tlx = crop_info->x;
  int tly = crop_info->y;
  int brx = tlx+crop_info->width;
  int bry = tly+crop_info->height;
  show_color_at(tlx, tly);
  printf("\n");
}

void showPosition(RectangleInfo *crop_info) {
  int tlx = crop_info->x;
  int tly = crop_info->y;
  printf("%3d %3d ", tlx, tly);
  printf("\n");
}

void showGeometry(RectangleInfo *crop_info) {
  int tlx = crop_info->x;
  int tly = crop_info->y;
  int brx = tlx+crop_info->width;
  int bry = tly+crop_info->height;
  if(tlx == brx && tly == bry) {
    printf("%3d %3d ", tlx, tly);
  	show_color_at(tlx, tly);
  } else {
    printf("%dx%d+%d+%d", crop_info->width, crop_info->height,
      crop_info->x, crop_info->y);
  }
  printf("\n");
}

void do_get_color() {
  RectangleInfo crop_info;
  Window w = XSelectWindow(XOpenDisplay(NULL), &crop_info);
  showColor(&crop_info);
}

void do_get_position() {
  RectangleInfo crop_info;
  Window w = XSelectWindow(XOpenDisplay(NULL), &crop_info);
  showPosition(&crop_info);
}

void do_get_geometry() {
  Window a;
  Display *display = XOpenDisplay(NULL);
  RectangleInfo crop_info;
  a = XSelectWindow(display, &crop_info);
  showGeometry(&crop_info);
  XEvent event;
  // The following has a side effect of clearing the rectangle, which is what
  // we want here.
  (void) XCheckWindowEvent(display,XRootWindow(display,XDefaultScreen(display)),
		  ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,&event);
}

void do_forever() {
  Window a;
  Display *display = XOpenDisplay(NULL);
  RectangleInfo crop_info;
  while(1) {
    a = XSelectWindow(display, &crop_info);
    showCropInfo(&crop_info);
  }
}

void usage() {
	printf("Usage: xgetcc [-g|-c|-p]\n");
	printf("\txgetcc -g  - get geometry - click or selection\n");
	printf("\txgetcc -c  - get color\n");
	printf("\txgetcc -p  - get position\n");
	printf("\txgetcc -v  - show version\n");
	printf("\txgetcc -h  - show this message\n");
	printf("\txgetcc     - run forever. Show mouse click/geometry of selection until killed.\n");
	exit(0);
	// TODO:
	//printf("xgetc -c -x 144  - get color along x's 144, y=0,maxy
	//printf("xgetc -c -y 144  - get color along y's 144, x=0,maxx
	//printf("xgetc -c -x 144 -y 144  - get color at x 144, y 144
}

const char xgetc_version_string[] = "0.1";

void do_show_version() {
	printf("xgetc %s\n", xgetc_version_string);
	printf("Author: Nazri Ramliy\n");
}

int main(int argc, char *argv[]) {
	if (1 == argc) {
		do_forever();
	} else while (1 < argc) {
		const char *arg = argv[1];
		if (!strcmp(arg, "-c"))
			do_get_color();
		if (!strcmp(arg, "-v"))
			do_show_version();
		else if (!strcmp(arg, "-g"))
			do_get_geometry();
		else if (!strcmp(arg, "-p"))
			do_get_position();
		else
			usage();
		argv++; argc--;
	}
	return 0;
}
