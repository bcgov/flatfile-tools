#include"glut.h"

GLvoid resize(GLsizei w, GLsizei h){
  GLfloat ratio;
  glViewport(0,0,w,h);
  // Select projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  getMatrix();

  // Adjust viewing volume (orthographic)
  // If taller than wide adjust y
  if(w < h){
    ratio = (GLfloat) h/ (GLfloat) w;
    glOrtho(-1.0f,1.0f,-1.0f*ratio,1.0f*ratio,-1.0f,1.0f);
    _bottom = -1.*ratio; _top = 1.*ratio;
    _left = -1.; _right = 1.;
  }
  // If wider than tall adjust x
  else if (h <= w){
    ratio = (GLfloat) w/ (GLfloat) h;
    glOrtho(-1.0f*ratio,1.0f*ratio,-1.0f,1.0f,-1.0f,1.0f);
    _left = -1.*ratio; _right = 1.*ratio;
    _bottom = -1.; _top = 1.;
  }
  glMatrixMode(GL_MODELVIEW);

}

GLvoid initializeGL(GLsizei width, GLsizei height){
}

void polarView(GLdouble radius, GLdouble twist, GLdouble latitude,
GLdouble longitude){
  glTranslated(0.0, 0.0, -radius);
  glRotated(-twist, 0.0, 0.0, 1.0);
  glRotated(-latitude, 1.0, 0.0, 0.0);
  glRotated(longitude, 0.0, 0.0, 1.0);
}

GLvoid draw(int p_select){
  if(sphere_pos.size() < 1){
    sphere_pos.push_back(vec3(1,1,1));
    sphere_col.push_back(vec3(1,0,0));
  }
  if(sphere_pos.size() < 2){
    sphere_pos.push_back(vec3(2,2,2));
    sphere_col.push_back(vec3(0,1,0));

    arrow_head.push_back(0);
    arrow_tail.push_back(1);
    arrow_col.push_back(vec3(0,0,1));
  }

  unsigned int ci = 0;
  for(vector<vec3>::iterator it = sphere_pos.begin(); it != sphere_pos.end(); it++){
    if(myPickNames.size() < 1) sphere_col[ci].color();
    else{
      if(myPickNames.count(ci) > 0) sphere_col[ci].invert();
      else sphere_col[ci].color();
    }
    glPushMatrix();
    vec3 tx(sphere_pos[ci] - rX);
    tx.translate();
    if(p_select) glPushName(ci);
      draw_sphere();
    if(p_select) glPopName();
    glPopMatrix();
    ci ++;
  }

  ci = 0;
  for(vector<vec3>::iterator it = arrow_col.begin(); it != arrow_col.end(); it++){

    vec3 Mx1(sphere_pos[arrow_head[ci]] - rX);
    vec3 Mx2(sphere_pos[arrow_tail[ci]] - rX);

    vec3 dx(Mx2 - Mx1);
    float len = dx.length();
    float tPL = ARROWHEAD_LENGTH;
    vec3 tx( dx - (dx*(tPL/len)));
    vec3 normalV( -dx.y, dx.x, 0.);
    normalV = normalV / normalV.length();
    float tNormal = ARROWHEAD_WIDTH;
    vec3 leftP( tx + ( normalV*tNormal));
    vec3 rightP( tx - ( normalV*tNormal));

    vec3 nV2( tx.cross(normalV));
    nV2 = nV2 / nV2.length();
    vec3 leftP2( tx + ( nV2*tNormal));
    vec3 rightP2( tx - ( nV2*tNormal));

    glLineWidth(1.5);
    arrow_col[ci].color();

    glPushMatrix();
    glBegin(GL_LINES); Mx1.vertex(); Mx2.vertex(); glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(Mx1.x, Mx1.y, Mx1.z);
    glBegin(GL_TRIANGLES); tx.vertex(); leftP.vertex(); dx.vertex(); glEnd();
    glBegin(GL_TRIANGLES); tx.vertex(); rightP.vertex(); dx.vertex(); glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(Mx1.x, Mx1.y, Mx1.z);
    glBegin(GL_TRIANGLES); tx.vertex(); leftP2.vertex(); dx.vertex(); glEnd();
    glBegin(GL_TRIANGLES); tx.vertex(); rightP2.vertex(); dx.vertex(); glEnd();
    glPopMatrix();

    ci ++;
  }

  if(true){
    glPushMatrix();
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(0.-rX.x, 0-rX.y, 0-rX.z);
    glVertex3f(1.-rX.x, 0-rX.y ,0- rX.z);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    glVertex3f(0.-rX.x, 0.-rX.y, 0-rX.z);
    glVertex3f(0.-rX.x,1.-rX.y ,0- rX.z);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    glVertex3f(0.-rX.x, 0-rX.y, 0. -rX.z);
    glVertex3f(0.-rX.x, 0-rX.y ,1.- rX.z);
    glEnd();
    glPopMatrix();
  }
}

GLvoid drawScene(int p_select){

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  draw(p_select);

  glFlush();
  SWAPBUFFERS;

}
