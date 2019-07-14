/* g++ graphics.cpp -lopengl32 -lglu32 -lgdi32
test opengl graphics without any extra drivers or libraries */
#include<GL/gl.h>
#include<GL/glu.h>
#include<windows.h>
#include<fstream>
#include<iostream>
#include<math.h>
#include<cfloat>
#include<set>
#include<vector>
using namespace std;

/* Windows globals, defines, and prototypes */
CHAR window_name[]="glut";
HDC ghDC;
HGLRC ghRC;
HWND ghWnd;

#define SWAPBUFFERS SwapBuffers(ghDC)
#define MAXBUFFERSIZE 1024

#define ARROWHEAD_LENGTH 0.25
#define ARROWHEAD_WIDTH 0.175
#define SPHERE_RADIUS 0.25

class vec3{
  public:
  float x, y, z;
  vec3( float xx, float yy, float zz){
    x = xx; y = yy; z = zz;
  }
  vec3(){
  }
  void init(float X, float Y, float Z){
    x=X; y=Y; z=Z;
  }
  void init(vec3 & a){
    x=a.x; y=a.y; z=a.z;
  }

  vec3( const vec3 & other){
    x = other.x; y = other.y; z = other.z;
  }
  vec3 & operator=( vec3 & rhs ){
    x = rhs.x; y = rhs.y; z = rhs.z;
  }
  float dot( vec3 & other){
    return x*other.x + y*other.y + z*other.z;
  }
  vec3 & cross( vec3 & other){
    vec3 * ret = new vec3();
    ret->x = y * other.z - z * other.y;
    ret->y = z * other.x - x * other.z;
    ret->z = x * other.y - y * other.x;
    return *ret;
  }
  int operator==(vec3 rhs){
    return ( x==rhs.x && y==rhs.y && z == rhs.z);
  }
  vec3 & operator+(vec3 rhs){
    vec3 * ret = new vec3(x + rhs.x, y+rhs.y, z+rhs.z);
    return *ret;
  }
  vec3 operator-(vec3 rhs){
    vec3* ret = new vec3( x-rhs.x, y-rhs.y, z-rhs.z);
    return *ret;
  }
  vec3 & operator+(float s){
    vec3* ret = new vec3( x+s,y+s,z+s);
    return *ret;
  }
  vec3 & operator-(float s){
    vec3* ret = new vec3( x-s,y-s,z-s);
    return *ret;
  }
  vec3 & operator*(float s){
    vec3* ret = new vec3( x*s, y*s, z*s);
    return *ret;
  }
  vec3 & operator/(float s){
    vec3* ret = new vec3( x/s, y/s ,z/s);
    return *ret;
  }
  float length(){
    return sqrt( x*x +y*y +z*z);
  }
  inline void vertex(){
    glVertex3f( x, y, z);
  }
  inline void color(){
    glColor3f(x, y, z);
  }
  inline void invert(){
    glColor3f(1. - x, 1. - y, 1. - z);
  }
  inline void translate(){
    glTranslatef(x, y, z);
  }

  inline void zero(){
    x = y = z = 0.;
  }

}
;

vec3 rX;
void draw_sphere(){
  const float GL_PI = 3.141592;
  GLfloat alpha, beta, x, y, z; // Storage for coordinates and angles
  GLfloat _radius = SPHERE_RADIUS;
  int gradation = 7;

  for (alpha = 0.0; alpha < GL_PI; alpha += GL_PI/gradation){
    glBegin(GL_TRIANGLE_STRIP);
    for (beta = 0.0; beta < 2.01*GL_PI; beta += GL_PI/gradation){
      //glBegin(GL_LINES);
      x = _radius*cos(beta)*sin(alpha);
      y = _radius*sin(beta)*sin(alpha);
      z = _radius*cos(alpha);
      glVertex3f(x, y, z);
      x = _radius*cos(beta)*sin(alpha + GL_PI/gradation);
      y = _radius*sin(beta)*sin(alpha + GL_PI/gradation);
      z = _radius*cos(alpha + GL_PI/gradation);
      glVertex3f(x, y, z);
      // glEnd();
    }
    glEnd();
  }
}

std::set<GLint> myPickNames;
vector<vec3> sphere_pos;
vector<vec3> sphere_col;

vector<unsigned int> arrow_head;
vector<unsigned int> arrow_tail;
vector<vec3> arrow_col;

int SHIFT_KEY;
int CONTROL_KEY;

float myLeft, myRight, myBottom, myTop, myZNear, myZFar;
int _mouseX,_mouseY, _mouseLeft, _mouseMiddle, _mouseRight;
double _left,_right, _top,_bottom,_near,_far,_zNear,_zFar, _dragPosX, _dragPosY, _dragPosZ;
double _matrix[16];
double _matrixInverse[16];
GLfloat zprReferencePoint[4];

static unsigned int WIDTH;
static unsigned int HEIGHT;
void set_width(unsigned int w){
  WIDTH = w;
}
void set_height(unsigned int h){
  HEIGHT = h;
}

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL bSetupPixelFormat(HDC);

/* OpenGL globals, defines, and prototypes */
GLfloat latitude, longitude, latinc, longinc;
GLdouble radius;

GLvoid resize(GLsizei, GLsizei);
GLvoid initializeGL(GLsizei, GLsizei);
GLvoid drawScene(int p_select); //GLvoid);
void polarView(GLdouble, GLdouble, GLdouble, GLdouble);

BOOL bSetupPixelFormat(HDC hdc){
  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
    1, // version number
    PFD_DRAW_TO_WINDOW | // support window
    PFD_SUPPORT_OPENGL | // support OpenGL
    PFD_DOUBLEBUFFER, // double buffered
    PFD_TYPE_RGBA, // RGBA type
    24, // 24-bit color depth
    0, 0, 0, 0, 0, 0, // color bits ignored
    0, // no alpha buffer
    0, // shift bit ignored
    0, // no accumulation buffer
    0, 0, 0, 0, // accum bits ignored
    32, // 32-bit z-buffer
    0, // no stencil buffer
    0, // no auxiliary buffer
    PFD_MAIN_PLANE, // main layer
    0, // reserved
    0, 0, 0 // layer masks ignored
  }
  ;

  int pixelformat;
  pixelformat = ChoosePixelFormat(hdc, &pfd);
  if(pixelformat ==0){
    MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
    return FALSE;
  }
  if(SetPixelFormat(hdc, pixelformat, &pfd) == FALSE){
    MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
    return FALSE;
  }
  return TRUE;
}

void invertMatrix(const GLdouble *m, GLdouble *out );
void getMatrix();
void processHits(GLint hits, GLuint buffer[]);
void zprPick(GLdouble x, GLdouble y,GLdouble delX, GLdouble delY);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

  rX.zero();
  SHIFT_KEY = false;

  _mouseLeft = _mouseRight = false;

  for(int i=0; i<4; i++) zprReferencePoint[i] = 0.;

  _mouseX = 0;
  _mouseY = 0;
  _mouseLeft = 0;
  _mouseMiddle = 0;
  _mouseRight = 0;
  _dragPosX = 0.0;
  _dragPosY = 0.0;
  _dragPosZ = 0.0;
  myLeft= 0.;//FLT_MAX;
  myRight=0.;//FLT_MIN;
  myBottom=0.;//FLT_MAX;
  myTop=0.;//FLT_MIN;
  myZNear= -10.;//0.;
  myZFar=10.;//1.;

  set_width(500);
  set_height(500);

  zprReferencePoint[0] = 0.;
  zprReferencePoint[1] = 0.;
  zprReferencePoint[2] = 0.;
  zprReferencePoint[3] = 0.;
  getMatrix();

  MSG msg;
  WNDCLASS wndclass;

  /* Register the frame class */
  wndclass.style = 0;
  wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = hInstance;
  wndclass.hIcon = LoadIcon (hInstance, window_name);
  wndclass.hCursor = LoadCursor (NULL,IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wndclass.lpszMenuName = window_name;
  wndclass.lpszClassName = window_name;

  if(!RegisterClass(&wndclass)) return FALSE;

  /* Create the frame */
  ghWnd = CreateWindow (window_name, "OpenGL", WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

  /* make sure window was created */
  if (!ghWnd) return FALSE;

  /* show and update main window */
  ShowWindow (ghWnd, nCmdShow);
  UpdateWindow (ghWnd);

  /* animation loop */
  while (1) {
    while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE){
      if (GetMessage(&msg, NULL, 0, 0) ){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      else{
        return TRUE;
      }
    }
    drawScene(false);
  }
}

void setOrthographicProjection() {
  int h = HEIGHT;
  int w = WIDTH;
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0., (float)w, 0., (float)h);
  glScalef(1., -1., 1.);
  glTranslatef(0, -1.*(float)h, 0);
  glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection(){
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void pos(double *px,double *py,double *pz,const int x,const int y,const int *viewport){
  /*
  * Use the ortho projection and viewport information
  * to map from mouse co-ordinates back into world
  * co-ordinates
  * */
  *px = (double)(x-viewport[0])/(double)(viewport[2]);
  *py = (double)(y-viewport[1])/(double)(viewport[3]);

  *px = _left + (*px)*(_right-_left);
  *py = _top + (*py)*(_bottom-_top);
  *pz = _zNear;
}

double vlen(double x,double y,double z){
  return sqrt(x*x+y*y+z*z);
}

GLvoid draw(int p_select);

void zprMotion(GLint x, GLint y){
  bool changed = false;
  const int dx = x - _mouseX;
  const int dy = y - _mouseY;
  if( dx==0 and dy==0) return;

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT,viewport);
  double px,py,pz;
  pos(&px,&py,&pz,x,y,viewport);

  if(CONTROL_KEY && _mouseLeft){
    if(myPickNames.size() == 1){
    set<GLint>::iterator it = myPickNames.begin();
    // glPlottable * a = (* (myGraphicsAsAFunctionOfGLName.find( *it ))).second;

    vec3 * a = &sphere_pos[*it]; // sphere centre coord, for name *it

    GLdouble proj[16]; // vars
    GLdouble model[16];
    GLint view[4];
    GLdouble nearx,neary,nearz;

    // proj., model, view mats
    glGetDoublev(GL_PROJECTION_MATRIX,proj);
    glGetDoublev(GL_MODELVIEW_MATRIX,model);
    glGetIntegerv(GL_VIEWPORT,view);

    float screendx=(float)x-(float)_mouseX;
    float screendy=(float)y-(float)_mouseY;
    // cout << "pick i: " << *it << " drag " <<  screendx <<","<<screendy<<".." << endl;
    double mx, my, mz, vx, vy, vz;
    mx = (double) a->x; //a->x.x; //_GetX();
    my = (double) a->y;//a->x.y; //GetY();
    mz = (double) a->z;//a->x.z; //GetZ();

    //world xyz onto screen xyz
    gluProject(mx,my,mz,model,proj,view,&vx,&vy,&vz);

    float screeny = vy - screendy; // - (float)screeny;
    float screenx = vx + screendx;

    //screen xyz onto world xyz
    gluUnProject(screenx, screeny, vz, model,proj,view,&nearx,&neary,&nearz);
    vec3 newa((float)nearx - mx, (float)neary -my, (float)nearz-mz);
    vec3 translation(a->x + newa.x, a->y + newa.y, a->z + newa.z);
    a->x = translation.x;
    a->y = translation.y;
    a->z = translation.z;

    draw(false);
	//SWAPBUFFERS;
    // a->Update = true; glutPostRedisplay(); display(); glutSwapBuffers();
    _dragPosX = px;
    _dragPosY = py;
    _dragPosZ = pz;
    _mouseX = x;
    _mouseY = y;
    changed = true;
    }
  }
  else{
    if(myPickNames.size() < 1){
      if(_mouseLeft && _mouseRight){
        double s = exp((double)dy*0.01);
        glTranslatef( zprReferencePoint[0], zprReferencePoint[1], zprReferencePoint[2]);
        glScalef(s,s,s);
        glTranslatef(-zprReferencePoint[0],-zprReferencePoint[1],-zprReferencePoint[2]);
        changed = true;
      }
      else if(_mouseLeft){
        double ax,ay,az,bx,by,bz,angle;
        ax = dy;
        ay = dx;
        az = 0.0;
        angle = vlen(ax,ay,az)/(double)(viewport[2]+1)*180.0;
        /* Use inverse matrix to determine local axis of rotation */
        bx = _matrixInverse[0]*ax + _matrixInverse[4]*ay + _matrixInverse[8] *az;
        by = _matrixInverse[1]*ax + _matrixInverse[5]*ay + _matrixInverse[9] *az;
        bz = _matrixInverse[2]*ax + _matrixInverse[6]*ay + _matrixInverse[10]*az;
        glTranslatef( zprReferencePoint[0], zprReferencePoint[1], zprReferencePoint[2]);
        glRotatef(angle,bx,by,bz);
        glTranslatef(-zprReferencePoint[0],-zprReferencePoint[1],-zprReferencePoint[2]);
        changed = true;
      }
      else if(_mouseRight){
        double px,py,pz;
        glMatrixMode(GL_MODELVIEW);
        pos(&px,&py,&pz,x,y,viewport);
        glLoadIdentity();
        glTranslatef(px-_dragPosX,py-_dragPosY,pz-_dragPosZ);
        glMultMatrixd(_matrix);
        _dragPosX = px; _dragPosY = py; _dragPosZ = pz;
        changed = true;
      }
          _mouseX = x;
    _mouseY = y;
    }

  }

  if(changed){
    getMatrix();
    //SWAPBUFFERS; ////glutPostRedisplay();
  }
}

void getMatrix(){
  glGetDoublev(GL_MODELVIEW_MATRIX,_matrix);
  invertMatrix(_matrix,_matrixInverse);
}

void invertMatrix(const GLdouble *m, GLdouble *out ){
  #define MAT(m,r,c) (m)[(c)*4+(r)]
  #define m11 MAT(m,0,0)
  #define m12 MAT(m,0,1)
  #define m13 MAT(m,0,2)
  #define m14 MAT(m,0,3)
  #define m21 MAT(m,1,0)
  #define m22 MAT(m,1,1)
  #define m23 MAT(m,1,2)
  #define m24 MAT(m,1,3)
  #define m31 MAT(m,2,0)
  #define m32 MAT(m,2,1)
  #define m33 MAT(m,2,2)
  #define m34 MAT(m,2,3)
  #define m41 MAT(m,3,0)
  #define m42 MAT(m,3,1)
  #define m43 MAT(m,3,2)
  #define m44 MAT(m,3,3)

  GLdouble det;
  GLdouble d12, d13, d23, d24, d34, d41;
  GLdouble tmp[16];

  d12 = (m31*m42-m41*m32);
  d13 = (m31*m43-m41*m33);
  d23 = (m32*m43-m42*m33);
  d24 = (m32*m44-m42*m34);
  d34 = (m33*m44-m43*m34);
  d41 = (m34*m41-m44*m31);

  tmp[0] = (m22 * d34 - m23 * d24 + m24 * d23);
  tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
  tmp[2] = (m21 * d24 + m22 * d41 + m24 * d12);
  tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

  det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];
  if(det != 0){
    GLdouble invDet = 1.0 / det;

    tmp[0] *= invDet;
    tmp[1] *= invDet;
    tmp[2] *= invDet;
    tmp[3] *= invDet;

    tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
    tmp[5] = (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
    tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
    tmp[7] = (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

    /* Pre-compute 2x2 dets for first two rows when computing */
    /* cofactors of last two rows. */
    d12 = m11*m22-m21*m12;
    d13 = m11*m23-m21*m13;
    d23 = m12*m23-m22*m13;
    d24 = m12*m24-m22*m14;
    d34 = m13*m24-m23*m14;
    d41 = m14*m21-m24*m11;

    tmp[8] = (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
    tmp[9] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
    tmp[10] = (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
    tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
    tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
    tmp[13] = (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
    tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
    tmp[15] = (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

    memcpy(out, tmp, 16*sizeof(GLdouble));

  }
  #undef m11
  #undef m12
  #undef m13
  #undef m14
  #undef m21
  #undef m22
  #undef m23
  #undef m24
  #undef m31
  #undef m32
  #undef m33
  #undef m34
  #undef m41
  #undef m42
  #undef m43
  #undef m44
  #undef MAT
}

void glut_keyboard_func(GLint wParam);

GLint mouseX, mouseY;

/* main window procedure */
LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  GLint mouseX, mouseY;
  LONG lRet = 1;
  PAINTSTRUCT ps;
  RECT rect;

   mouseX = LOWORD(lParam);
   mouseY = HIWORD(lParam);

  switch (uMsg) {
    case WM_CREATE:
    ghDC = GetDC(hWnd);
    if (!bSetupPixelFormat(ghDC))
    PostQuitMessage(0);

    ghRC = wglCreateContext(ghDC);
    wglMakeCurrent(ghDC, ghRC);
    GetClientRect(hWnd, &rect);
    initializeGL(rect.right, rect.bottom);
    break;

    case WM_PAINT:
    BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    break;

    case WM_SIZE:
    GetClientRect(hWnd, &rect);
    resize(rect.right, rect.bottom);
    WIDTH = rect.right;
    HEIGHT =rect.bottom;
    break;

    case WM_CLOSE:
    if (ghRC) wglDeleteContext(ghRC);
    if (ghDC) ReleaseDC(hWnd, ghDC);
    ghRC = 0;
    ghDC = 0;
    DestroyWindow (hWnd);
    break;

    case WM_DESTROY:
    if (ghRC) wglDeleteContext(ghRC);
    if (ghDC) ReleaseDC(hWnd, ghDC);
    PostQuitMessage(0);
    break;

    case WM_KEYDOWN:{
      switch (wParam) {
        case VK_SHIFT: SHIFT_KEY = true; break;
        case VK_CONTROL: CONTROL_KEY = true; break;
        case VK_LEFT: break;
        case VK_RIGHT: break;
        case VK_UP: break;
        case VK_DOWN: break;
        case VK_ESCAPE: exit(1);
      }
    }

    cout <<"key press: " << wParam << endl;
    break;

    case WM_KEYUP:{
      switch (wParam) {
        case VK_SHIFT: SHIFT_KEY = false; break;
        case VK_CONTROL: CONTROL_KEY = false; break;
        case VK_LEFT: break;
        case VK_RIGHT: break;
        case VK_UP: break;
        case VK_DOWN: break;
        case VK_ESCAPE: exit(1);
      }
    }

    break;

    case WM_LBUTTONDOWN:{
      _mouseLeft = true;
      mouseX = LOWORD(lParam);
      mouseY = HIWORD(lParam);
      GLint x = mouseX;
      GLint y = mouseY;
      GLint viewport[4];
      zprPick(mouseX,HEIGHT-1-mouseY,3,3);
      glGetIntegerv(GL_VIEWPORT,viewport);
      pos(&_dragPosX,&_dragPosY,&_dragPosZ,x,y,viewport);
      // glutPostRedisplay();
      _mouseX = mouseX;
      _mouseY = mouseY;
    }
    break;

    case WM_RBUTTONDOWN: _mouseRight = true;
    mouseX = LOWORD(lParam);
    mouseY = HIWORD(lParam);
    break;
    case WM_LBUTTONUP: _mouseLeft = false;
    mouseX = LOWORD(lParam);
    mouseY = HIWORD(lParam);
    myPickNames.clear();
    break;
    case WM_RBUTTONUP: _mouseRight = false;
    mouseX = LOWORD(lParam);
    mouseY = HIWORD(lParam);
    break;

    case WM_MOUSEMOVE:
    mouseX = LOWORD(lParam);
    mouseY = HIWORD(lParam);
    zprMotion(mouseX, mouseY);
    break;

    case WM_MOUSEWHEEL:
    break;

    default:
    lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
    break;
  }

  return lRet;
}

void reset_view(){

  _mouseX = 0;
  _mouseY = 0;
  _mouseLeft = 0;//false;
  _mouseMiddle = 0;//false;
  _mouseRight = 0;//false;
  _dragPosX = 0.0;
  _dragPosY = 0.0;
  _dragPosZ = 0.0;
  rX.init(0., 0., 0.);
  zprReferencePoint[0] = 0.;
  zprReferencePoint[1] = 0.;
  zprReferencePoint[2] = 0.;
  zprReferencePoint[3] = 0.;
  getMatrix();

}

void processHits(GLint hits, GLuint buffer[]){
  myPickNames.clear();

  unsigned int i, j;
  GLuint names, *ptr, minZ,*ptrNames, numberOfNames;
  if(hits <=0 ) return;
  //printf ("hits = %d names:{", hits);
  ptr = (GLuint *) buffer;
  minZ = 0xffffffff;
  for (i = 0; i < hits; i++) {
    //printf("(i%d)",i);
    names = *ptr;
    ptr++;
    GLint mindepth = *ptr; ptr++;
    GLint maxdepth = *ptr; ptr++;
    for(j=0; j<names; j++){
      GLint name = *ptr;
      // printf(",%d",name);
      if(name>=0){
        myPickNames.insert(name);
        if(SHIFT_KEY) rX = sphere_pos[name];
      }
      ptr++;
    }
  }
  //printf ("}\n");
}

GLvoid draw(int p_select);

void zprPick(GLdouble x, GLdouble y,GLdouble delX, GLdouble delY){
  GLuint buffer[MAXBUFFERSIZE];
  const int bufferSize = sizeof(buffer)/sizeof(GLuint);

  GLint viewport[4];
  GLdouble projection[16];

  GLint hits;
  GLint i,j,k;

  GLint min = -1;
  GLuint minZ = -1;

  glSelectBuffer(bufferSize,buffer); /* Selection buffer for hit records */
  glRenderMode(GL_SELECT); /* OpenGL selection mode */
  glInitNames();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix(); /* Push current projection matrix */
  glGetIntegerv(GL_VIEWPORT,viewport); /* Get the current viewport size */
  glGetDoublev(GL_PROJECTION_MATRIX,projection); /* Get the projection matrix */
  glLoadIdentity(); /* Reset the projection matrix */
  gluPickMatrix(x,y,delX,delY,viewport); /* Set the picking matrix */
  glMultMatrixd(projection); /* Apply projection matrix */

  glMatrixMode(GL_MODELVIEW);

  draw(true);

    // Draw the scene in selection mode
    hits = glRenderMode(GL_RENDER); /* Return to normal rendering mode */
    processHits( hits, buffer);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); /* Restore projection matrix */
    glMatrixMode(GL_MODELVIEW);
    return;
  }

  class glPlottable{

    };
