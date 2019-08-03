#include"glut.h"
GLvoid resize(GLsizei w, GLsizei h){
  GLfloat ratio;
  glViewport(0, 0, w, h);
  // Select projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  getMatrix();

  // Adjust viewing volume (orthographic)
  // If taller than wide adjust y
  if(w < h){
    ratio = (GLfloat) h/ (GLfloat) w;
    glOrtho(-1.0f, 1.0f, -1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f);
    _bottom = -1. * ratio; _top = 1. * ratio;
    _left = -1.; _right = 1.;
  }
  // If wider than tall adjust x
  else if (h <= w){
    ratio = (GLfloat) w / (GLfloat) h;
    glOrtho(-1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f, -1.0f, 1.0f);
    _left = -1. * ratio; _right = 1. * ratio;
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
inline float sgn(float x){
  return x < 0 ? -1.: 1.;
}

inline float clamp(float x, float T){
  return (abs(x) > abs(T)) ? sgn(x) * abs(T) : x;
}
inline vec3 clamp(vec3& x, float T){
  return vec3(clamp(x.x, T), clamp(x.y, T), clamp(x.z, T));
}

void iterate(){
  long unsigned int n_arrow = arrow_head.size();
  if(n_arrow != arrow_tail.size()) err("arrow array size mismatch");
  long unsigned int i, j, src, dst;

  float xmin, xmax, ymin, ymax, zmin;
  xmin = ymin = FLT_MAX;
  xmax = ymax = FLT_MIN;
  for0(i, sphere_pos.size()){
    float x = sphere_pos[i].x;
    float y = sphere_pos[i].y;
    if(x < xmin) xmin = x;
    if(x > xmax) xmax = x;
    if(y < ymin) ymin = y;
    if(y > ymax) ymax = y;
  }

  for0(i, sphere_pos.size()){

    sphere_pos[i].x -= xmin;
    sphere_pos[i].x *= (1. / (xmax-xmin));

    sphere_pos[i].y -= ymin;
    sphere_pos[i].y *= (1./(ymax-ymin));

  }

  return;

  // sphere repulsion force (should multicore these ASAP)
  float d_max = 1.;
  float f = 0.05;
  for0(i, n_arrow){
    src = arrow_tail[i];
    dst = arrow_head[i];
    vec3 d(sphere_pos[src] - sphere_pos[dst]);
    if(d.length() < 50. * SPHERE_RADIUS){
      d = d * 0.00003;

      sphere_pos[src] += d;
      sphere_pos[dst] -= d;
    }
    else{
      // arrow spring force
      float fd = d.length() - 5. * SPHERE_RADIUS;
      fd = sqrt(fd * fd);
      vec3 ux((d/d.length()) * 0.025 * fd);
      if(ux.length() > SPHERE_RADIUS / 2.){
        sphere_pos[src] -= ux;
        sphere_pos[dst] += ux;
      }
    }
  }
}

/* 
void setOrthographicProjection(){
  int h = HEIGHT;
  int w = WIDTH;
  glMatrixMode(Gl_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2d(0, (float)w, 0., (float)h);
  glScalef(1., -1., 1.);
  glTranslatef(0, -1.*(float)h, 0);
  glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection(){
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}*/

void renderBitmapString(float x, float y, char *string){
char *c;
glRasterPos2f(x,y);
/*
HDC ghDC;
HGLRC ghRC;
  */
  SelectObject(ghDC, GetStockObject(SYSTEM_FONT));
  wglUseFontBitmaps(ghDC, 0, 255, 1000);
  
  glListBase(1000);
  glCallLists(24, GL_UNSIGNED_BYTE, "Hello Windows OpenGL!");
}

void drawString(){
  renderBitmapString(20, 20, NULL);
}


/* rendering method */
GLvoid draw(int p_select){
  iterate();
  int draw_text = true;

  int debug = false;
  if(debug){
    if(sphere_pos.size() < 1){
      // add two spheres!
      sphere_pos.push_back(vec3(2, 2, 2)); sphere_col.push_back(vec3(2, 0, 0));
      sphere_pos.push_back(vec3(3, 3, 3)); sphere_col.push_back(vec3(0, 2, 0));

      // add one arrow!
      arrow_head.push_back(0); arrow_tail.push_back(1); arrow_col.push_back(vec3(0, 0, 2));
    }
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

    if(draw_text){
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();

	drawString();
	glPopMatrix();
	resetPerspectiveProjection();
    }

    ci ++;
  }

  ci = 0;
  for(vector<vec3>::iterator it = arrow_col.begin(); it != arrow_col.end(); it++){

    // sphere centres
    vec3 Mx1(sphere_pos[arrow_head[ci]] - rX);
    vec3 Mx2(sphere_pos[arrow_tail[ci]] - rX);

    vec3 dx(Mx2 - Mx1);
    float len = dx.length();

    // from first sphere centre to outside of second sphere
    dx = dx * ((len - SPHERE_RADIUS) / len); // * dx;
    float tPL = ARROWHEAD_LENGTH;
    vec3 tx(dx - (dx * (tPL / len)));
    vec3 normalV(-dx.y, dx.x, 0.);
    normalV = normalV / normalV.length();
    float tNormal = ARROWHEAD_WIDTH;
    vec3 leftP(tx + ( normalV*tNormal));
    vec3 rightP(tx - ( normalV*tNormal));

    vec3 nV2( tx.cross(normalV));
    nV2 = nV2 / nV2.length();
    vec3 leftP2(tx + ( nV2 * tNormal));
    vec3 rightP2(tx - ( nV2 * tNormal));

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

  if(debug){
    // draw axes
    glPushMatrix();
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(0. - rX.x, 0 - rX.y, 0 - rX.z);
    glVertex3f(1. - rX.x, 0 - rX.y ,0 - rX.z);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    glVertex3f(0. - rX.x, 0. - rX.y, 0 - rX.z);
    glVertex3f(0. - rX.x, 1. - rX.y, 0 - rX.z);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    glVertex3f(0. - rX.x, 0 - rX.y, 0. - rX.z);
    glVertex3f(0. - rX.x, 0 - rX.y, 1. - rX.z);
    glEnd();
    glPopMatrix();
  }
}

GLvoid drawScene(int p_select){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw(p_select);
  glFlush();
  SWAPBUFFERS;
}

priority_queue<d_idx> s_dmat;
void read_data(str fn){
  csv.clear();

  while(s_dmat.size() > 0)s_dmat.pop(); // s_dmat.clear();

  skip_factor = 1000; // 1: don't skip any data
  // open the input data
  ifstream g(fn);
  if(!g.is_open()) err("failed to opend input file");
  string s;

  // read header
  getline(g, hdr); // csv.push_back(s);

  if(skip_factor ==1){
    while(getline(g, s)){
      csv.push_back(s);
    }
  }
  else{
    long unsigned int ci = 0;
    while(getline(g, s)){
      if(ci % skip_factor == 0){
        csv.push_back(s);
      }
      ci += 1;
    }
  }
  g.close();
  cout << "fields: " << hdr << endl;
  cout << "rcords: " << csv.size() << endl; // first record is header
  // n.b. need to project that onto the data
}

void cprint(str s){
  pthread_mutex_lock(&print_mutex);
  cout << s << endl;
  pthread_mutex_unlock(&print_mutex);
}

void * dmat_threadfun(void * arg){
  vector<str> fields(split(hdr));
  int n_fields = fields.size();

  float d;

  long unsigned int jobs_by_me = 0;
  long unsigned int my_next_j;
  long unsigned int n_records = csv.size();
  long k = (long)arg;
  cprint(str("dmat_threadfun(") + std::to_string(k) + str(")"));

  while(1){
    // try to pick up another job
    pthread_mutex_lock(&next_j_mutex);
    my_next_j = next_j ++; // index of data this thread should pick up if it can
    pthread_mutex_unlock(&next_j_mutex);

    if(my_next_j > n_records - 1){
      cprint(str("\texit thread ") + to_string(k) + str(" njob ") + to_string(jobs_by_me));
      return NULL;
    }
    else{
      jobs_by_me += 1;
    }

    cprint(str("  worker: ") + to_string(k) + str(" pickup: ") + to_string(my_next_j));

    // calculate a row of the distance matrix

    unsigned int i, m;
    unsigned int k;
    //vector<float> tmp;

    str u,v;
    str us("unknown");

    for0(i, n_records){
      // distance function
      d = 0.;
      for0(m, n_fields){
        u = data[my_next_j * n_fields + m];
        v = data[i * n_fields + m];

        // can't get similarity from unknown
        if (u == v && u != us && v != us){
        }
        else{
          d += 1.;
        }
      }
      dmat[my_next_j * n_records + i] = d;
      //tmp.push_back(
      //cprint(str("\n\n") + to_string(d) + str("\n") + csv[i] + str("\n") + csv[my_next_j]);

    }
    pthread_mutex_lock(&print_mutex);
    //cout << "w" << k << " j" << my_next_j << " " << hdr << " " << tmp << endl;
    pthread_mutex_unlock(&print_mutex);
  }
}

void sort_dmat(){
  long unsigned int n_records = csv.size();

  // make sure the queue is clear
  while(s_dmat.size() > 0) s_dmat.pop();

  cout << "\n\nsorting.." << endl;
  unsigned long int i, j, k;
  for0(i, n_records){
    for0(j, i){
      k = j * n_records + i;
      //cout << "push " << dmat[k] << " " << k << " j " << j << " i " << i << endl;
      s_dmat.push(d_idx(dmat[k], k));
    }
  }
  cout << "done sorting" << endl;
  // next step: hierarchical clustering
}

void calc_dmat(){
  cout << "calc_dmat.." << endl;

  // if(data != NULL) delete(data);

  long unsigned int j, i;
  long unsigned int n_records = csv.size();
  cout << "n_records " << n_records << endl;
  vector<str> fields(split(hdr));
  int n_fields = fields.size();
  cout << "n_fields " <<n_fields << endl;
  long unsigned int n_strings = n_records * n_fields;

  cout << "allocate memory for split strings.." << endl;
  data = new string[n_strings];
  for0(j, n_strings) data[j] = str("");

  cout << "splitting csv.." << endl;
  vector<str> words;
  for0(j, n_records){
    words = split(csv[j]);
    if(words.size() != n_fields) err("unexpected number of fields");
    for0(i, n_fields){
      trim(words[i]);
      lower(words[i]);
      data[(j*n_fields) + i] = words[i];
      // cout << i << " " << data[(j*n_fields) + i] << " " << words << endl;
    }
  }

  cprint(str("csv splitting complete.."));

  long unsigned int dmat_size = csv.size() * csv.size();
  dmat = (float *) (void *) balloc(sizeof(float) * dmat_size);
  for0(j, dmat_size) dmat[j] = 0.;

  next_j = 0; // put a lock on this variable

  int numCPU;
  #ifdef WIN32
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  numCPU = sysinfo.dwNumberOfProcessors;
  #else
  numCPU = sysconf(_SC_NPROCESSORS_ONLN);
  #endif
  cout << "Number of cores: " << numCPU << endl;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  // set up mutexes
  pthread_mutex_init(&print_mutex, NULL);
  pthread_mutex_init(&next_j_mutex, NULL);

  // allocate threads
  pthread_t * my_pthread = new pthread_t[numCPU];
  for0(j, numCPU){
    long k = j;
    pthread_create(&my_pthread[j], &attr, dmat_threadfun, (void *) k);
  }

  // wait for threads to finish
  for0(j, numCPU){
    pthread_join(my_pthread[j], NULL);
  }
  //pthread_mutex_destroy(&next_j_mutex);
  //pthread_exit(NULL);
  //delete my_pthread;
  cout << "end dmat_calc()" << endl;
}

void hclust(){
  sphere_pos.clear();
  sphere_col.clear();
  arrow_head.clear();
  arrow_tail.clear();
  arrow_col.clear();

  float R_FACT = 0.; //125.;
  float z = R_FACT * (float)rand() / (float) RAND_MAX;

  vec3 pink(0, 1, 0); //vec3 pink(1., .00, .00);
  vec3 antipink(1. - pink.x, 1. - pink.y, 1. - pink.z);
  //vec3 antipink(0, 1, 0);
  vec3 line_colour(0., 0., 1.);

  vec3 leaf_colour(antipink);
  vec3 node_colour(pink);

  int debug = false;
  unsigned long int i, j;
  unsigned long int n_records = csv.size();

  unsigned long int my_label;
  unsigned long int next_label = n_records; // progressively inclusive labels will start on new numerological turf
  if(debug) cout << "==============================" << endl;

  if(debug) cout << "next_label " << next_label << " final label should be <= (n^2 - n) / 2 = " << (n_records * n_records - n_records ) / 2 << endl;
  map<unsigned long int, unsigned long int> meta;
  map<unsigned long int, unsigned long int> up;

  map<unsigned long int, unsigned long int> count;

  // idea: iterative incarnations of recursive or hierarchical things-- conceptual unification of iteration and hierarchy
  // dictionary (non-pointer) implementation of disjoint-set forest

  // seed the "simulation" (bad terminology)
  for0(i, n_records){
    meta[i] = up[i] = i;
    sphere_pos.push_back(vec3(0, (float)i, z)); sphere_col.push_back(leaf_colour);//vec3(1., .1, .5));
    z =R_FACT* (float)rand() / (float)RAND_MAX;
    count[i] = 1;
  }

  // n.b. in "top" all keys will eventually all map to the same value,
  // whereas label maps to the next most inclusive node (value) AKA set (up arrow)

  long unsigned int iter = 1;
  stack<d_idx> tmp;
  while(s_dmat.size() > 0){
    cout << iter << "------------------------------" << endl;
    float x = s_dmat.top().d;

    map<unsigned long int, unsigned long int> meta_p;
    meta_p.clear();

    if(debug) cout << "meta: " << meta << endl;
    while(s_dmat.size() > 0 && s_dmat.top().d == x){
      d_idx a(s_dmat.top());
      i = a.idx % n_records;
      j = a.idx / n_records;
      if(debug) cout << "pull " << s_dmat.top().d << " " << j << "," << i << " mj, mi=" << meta[j] << "," << meta[i] << endl; //<< s_dmat.top().idx << endl;
      s_dmat.pop();

      unsigned long int topi = meta[i];
      while(topi != meta[topi]){
        if(debug) cout << "\tflatten: " << topi << "," << meta[topi] << " --> " << meta[topi] << endl;
        topi = meta[topi];
      }
      unsigned long int topj = meta[j];
      while(topj != meta[topj]){
        if(debug) cout << "\tflatten: " << topj << "," << meta[topj] << " --> " << meta[topj] << endl;
        topj = meta[topj];
      }

      if(meta_p.count(topi) < 1 && meta_p.count(topj) < 1){
        my_label = next_label ++;
        /* meta_p[i] = meta_p[j] = */ meta_p[topi] = meta_p[topj] = my_label;
        if(debug) cout << "\tnew_label " << topi << ", " << topj << " --> " << my_label << endl;
        meta_p[my_label] = my_label;
        sphere_pos.push_back(vec3((float)iter, (float)i, z)); sphere_col.push_back(node_colour); //vec3(0., 1.0, .0));
        z = R_FACT * (float)rand() / (float)RAND_MAX;
	count[my_label] = count[topi] + count[topj];
      }
      else if(meta_p.count(topi) < 1){
        meta_p[topi] = meta_p[topj]; cout << "\t" << topi << " --> " << meta_p[topj] << endl;
      }
      else if(meta_p.count(topj) < 1){
        meta_p[topj] = meta_p[topi]; cout << "\t" << topj << " --> " << meta_p[topi] << endl;
      }
      else{
        if(meta_p[topi] == meta_p[topj]){
          if(debug) cout << "\tpass: " << meta_p[topi] << " " << meta_p[topj] << endl;
        }
        else{
          if(debug) cout << "\trela: " << meta_p[topi] << " " << meta_p[topj] << " --> " << max(meta_p[topi], meta_p[topj]) << endl;
          meta_p[topi] = meta_p[topj] = max(meta_p[topi], meta_p[topj]);
        }
      }

    }
    if(debug) cout << "relabel: " << endl;
    for(map<unsigned long int, unsigned long int>::iterator it = meta_p.begin(); it != meta_p.end(); it ++){
      meta[it->first] = it->second;
      cout << it->first << " --> " << it->second << endl;
      if(it->first != it->second){
        arrow_head.push_back(it->first); arrow_tail.push_back(it->second); arrow_col.push_back(line_colour); //vec3(.0, .0, 1.));
      }
    }

    if(debug) cout <<"*eta: " << meta << endl;
    iter += 1;
  }
}

void cluster(){
  str fn(data_files[cur_data_file]);
  strip(fn);
  cout << "filename: " << fn << endl;

  cout << "read data.." << endl;
  read_data(fn); // read the data

  cout << "calc dmat.." << endl;
  calc_dmat(); // calculate the distance matrix

  cout << "sort dmat.." << endl;
  sort_dmat(); // sort the distance matrix

  cout << "hclust.." << endl;
  hclust(); // hierarchical clustering on the distance matrix
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
  data = NULL;
  cur_data_file = 0; // start with the first file
  str fn("");
  str fns("");

  /* check to see if we can get the input data */
  ifstream f("./.vis");
  if(!f.is_open()) err("failed to open input file: .vis");

  std::getline(f, fns);
  data_files = split(fns);
  cout << "data files: " << data_files << endl;
  fn = data_files[0];
  cout << "fn: " << fn << endl;
  f.close();

  cluster();

  rX.zero();
  myZFar = 10.;
  myZNear = -10.;
  SHIFT_KEY = false;
  _mouseLeft = _mouseRight = false;
  _dragPosX = _dragPosY = _dragPosZ = 0.0;
  myLeft = myRight = myBottom = myTop = 0.;
  for(int i = 0; i < 4; i++) zprReferencePoint[i] = 0.;
  _mouseX = _mouseY = _mouseLeft = _mouseMiddle = _mouseRight = 0;

  set_width(1111);
  set_height(1111);
  zprReferencePoint[0] = zprReferencePoint[1] = zprReferencePoint[2] = zprReferencePoint[3] = 0.;
  getMatrix();

  MSG msg;
  WNDCLASS wndclass;

  /* Register the frame class */
  wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
  wndclass.style = wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
  wndclass.hInstance = hInstance;
  wndclass.hIcon = LoadIcon (hInstance, window_name);
  wndclass.hCursor = LoadCursor (NULL,IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wndclass.lpszMenuName = wndclass.lpszClassName = window_name;

  if(!RegisterClass(&wndclass)) return FALSE;

  /* Create the frame */
  ghWnd = CreateWindow (window_name, "OpenGL: shift-click to re-centre, ctrl to drag", WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);
  if (!ghWnd) return FALSE; // confirm window created
  ShowWindow (ghWnd, nCmdShow); // show and update main window
  UpdateWindow (ghWnd);

  /*
GLfloat lightpos[] = {.5, 1., 1., 0.};
glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
*/

/* animation loop */
while(1){
  while(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE){
    if(GetMessage(&msg, NULL, 0, 0)){
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else return TRUE;
  }
  drawScene(false);
}
}
