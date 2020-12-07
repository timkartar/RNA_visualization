/***********************************************************************
  Program rnav.c--representation of RNAs.
  Required files
    atomv.h:   Include file
    md.conf:   MD configuration file containing atomic coordinates
***********************************************************************/
#include "rnav.h"
#include <stdio.h>
#include <math.h>
#include <OpenGL/gl.h>    /* Header file for the OpenGL library */
#include <OpenGL/glu.h>   /* Header file for the GLu library */
#include <GLUT/glut.h>    /* Header file for the GLut library */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

GLuint sphereid_XVEC;          /* display-list id of atom sphere geom */
GLuint sphereid_YVEC;          /* display-list id of atom sphere geom */
GLuint sphereid_SUGR;          /* display-list id of atom sphere geom */
GLuint sphereid_PHOS;          /* display-list id of atom sphere geom */
GLuint sphereid_A;          /* display-list id of atom sphere geom */
GLuint sphereid_U;          /* display-list id of atom sphere geom */
GLuint sphereid_G;          /* display-list id of atom sphere geom */
GLuint sphereid_C;          /* display-list id of atom sphere geom */


GLuint atomsid;           /* display-list id of all atoms */
GLdouble fovy, aspect, near_clip, far_clip;
                          /* parameters for gluPerspective() */

char path[100];           //directory for pdb files, read from cmd line
char pdb_file[100];          //path to specific pdb file
char prefix[] = "init_";  //used to create paths to files
char suffix[] = ".pdb";



FILE *fp;                 /* pointer to open an MD-configuration file */

/* Function prototypes ************************************************/
void reshape(int, int);
void makeFastNiceSphere(GLuint, double);
void makeAtoms(void);
void makeCurframeGeom(void);
void drawScene(void);
void display(void);
void initView(float *, float *);
void readConf(void);
// void readPDB();
// void readPDB(char []);
//void makeradiuslist(void);
/**********************************************************************/
void reshape (int w, int h) {
/***********************************************************************
  Callback for glutReshapeFunc()
***********************************************************************/
  /* set the GL viewport to match the full size of the window */
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  aspect = w/(float)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovy,aspect,near_clip,far_clip);
  glMatrixMode(GL_MODELVIEW);
}

/**********************************************************************/
void makeFastNiceSphere(GLuint listid, double radius) {
/***********************************************************************
Called once to generate and compile sphere geometry into the given
display list id.
***********************************************************************/
  int i,j;
  float lon,lat;
  float loninc,latinc;
  float x,y,z;

  loninc = 2*M_PI/nlon;
  latinc = M_PI/nlat;

  glNewList(listid,GL_COMPILE);

    /* South-pole triangular fan */
    glBegin(GL_TRIANGLE_FAN);
      glNormal3f(0,-1,0);
      glVertex3f(0,-radius,0);
      lon = 0;
      lat = -M_PI/2 + latinc;
      y = sin(lat);
      for (i=0; i<=nlon; i++) {
        x = cos(lon)*cos(lat);
        z = -sin(lon)*cos(lat);
        glNormal3f(x,y,z);
        glVertex3f(x*radius,y*radius,z*radius);
        lon += loninc;
      }
    glEnd();

    /* Quadrilateral stripes to cover the sphere */
    for (j=1; j<nlat-1; j++) {
      lon = 0;
      glBegin(GL_QUAD_STRIP);
        for (i=0; i<=nlon; i++) {
          x = cos(lon)*cos(lat);
          y = sin(lat);
          z = -sin(lon)*cos(lat);
          glNormal3f(x,y,z);
          glVertex3f(x*radius,y*radius,z*radius);
          x = cos(lon)*cos(lat+latinc);
          y = sin(lat+latinc);
          z = -sin(lon)*cos(lat+latinc);
          glNormal3f(x,y,z);
          glVertex3f(x*radius,y*radius,z*radius);
          lon += loninc;
        }
      glEnd();
      lat += latinc;
    }

    /* North-pole triangular fan */
    glBegin(GL_TRIANGLE_FAN);
      glNormal3f(0,1,0);
      glVertex3f(0,radius,0);
      y = sin(lat);
      lon = 0;
      for (i=0; i<=nlon; i++) {
        x = cos(lon)*cos(lat);
        z = -sin(lon)*cos(lat);
        glNormal3f(x,y,z);
        glVertex3f(x*radius,y*radius,z*radius);
        lon += loninc;
      }
    glEnd();

  glEndList();
}
/*
void makeradiuslist(){
    int i;
    radius_list = (double*) malloc(natoms*sizeof(double));
    for (i=0; i < natoms; i++) {
        if(atoms[i].type == 'X') radius_list[i] = 0.2;
        else if (atoms[i].type == 'Y') radius_list[i] = 0.2;
        else if (atoms[i].type == 'S') radius_list[i] = 1;
        else if (atoms[i].type == 'P') radius_list[i] = 0.5;
        else if (atoms[i].type == 'B') {
            if (atoms[i].basetype == 'A') radius_list[i] = 1;
            if (atoms[i].basetype == 'U') radius_list[i] = 1;
            if (atoms[i].basetype == 'G') radius_list[i] = 1;
            if (atoms[i].basetype == 'C') radius_list[i] = 1;
        }
    else  radius_list[i] = 1;
    }

}
*/
/**********************************************************************/
void makeAtoms() {
/***********************************************************************
  Makes display-list of all atoms in the current frame using spheres.
***********************************************************************/
  int i;
  float rval,gval,bval;
/////////////////////////////////////////////////////////////////////////////////////////
  glNewList(atomsid, GL_COMPILE);
  rval = Ratom; gval = Gatom; bval = Batom;  /* RGB color of an atom */
  for (i=0; i < natoms; i++) {
    //sphereid = glGenLists(1);
    //makeFastNiceSphere(sphereid,radius_list[i]);
    glPushMatrix();
    glTranslatef(atoms[i].crd[0],atoms[i].crd[1],atoms[i].crd[2]);
    if(atoms[i].type == 'X') {glColor3f(0.1*rval,0.5*gval,0.5*bval);glCallList(sphereid_XVEC);}
    else if (atoms[i].type == 'Y') {glColor3f(0.1*rval,0.1*gval,0.5*bval);glCallList(sphereid_YVEC);}
    else if (atoms[i].type == 'S') {glColor3f(1*rval,0*gval,0*bval);glCallList(sphereid_SUGR);}
    else if (atoms[i].type == 'P') {glColor3f(0*rval,1*gval,0*bval);glCallList(sphereid_PHOS);}
    else if (atoms[i].type == 'B') {
        if (atoms[i].basetype == 'A') {glColor3f(0*rval,0*gval,1*bval);glCallList(sphereid_A);}
        if (atoms[i].basetype == 'U') {glColor3f(0*rval,1*gval,1*bval);glCallList(sphereid_U);}
        if (atoms[i].basetype == 'G') {glColor3f(1*rval,0*gval,1*bval);glCallList(sphereid_G);}
        if (atoms[i].basetype == 'C') {glColor3f(1*rval,1*gval,0*bval);glCallList(sphereid_C);}
    }
    else  {glColor3f(gval,rval,bval);glCallList(sphereid_XVEC);}
    //glCallList(sphereid);
    glPopMatrix();
  }
  glEndList();
}
////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************/
void makeCurframeGeom() {
/***********************************************************************
  Reads the atoms information for the current time frame and makes the
  display-list of all the atoms' geometry.
***********************************************************************/
  makeAtoms();
}

/**********************************************************************/
void drawScene() {
/***********************************************************************
  Called by display() to draw the view of the current scene.
***********************************************************************/
  /* Define viewing transformation */
  gluLookAt(
    (GLdouble)eye[0],(GLdouble)eye[1],(GLdouble)eye[2],
    (GLdouble)center[0],(GLdouble)center[1],(GLdouble)center[2],
    (GLdouble)up[0],(GLdouble)up[1],(GLdouble)up[2]);
  glCallList(atomsid);
}

/**********************************************************************/
void display() {
/***********************************************************************
  Callback for glutDisplayFunc().  It clears the frame and depth
  buffers and draws the atoms in the current frame.
***********************************************************************/
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  drawScene();
  glutSwapBuffers();
}

/**********************************************************************/
void initView (float *min_ext, float *max_ext) {
/***********************************************************************
  Initializes global viewing, lighting, and projection values.
***********************************************************************/
  GLfloat light_diffuse[]   = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position1[] = {0.5, 0.5, 1.0, 0.0};
  float dif_ext[3],dis;
  int i;

  /* Define normal light */
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
  glLightfv(GL_LIGHT0,GL_POSITION,light_position1);

  /* Enable a single OpenGL light */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  /* Use depth buffering for hidden surface elimination */
  glEnable(GL_DEPTH_TEST);

  /* get diagonal and average distance of extent */
  for (i=0; i<3; i++) dif_ext[i] = max_ext[i] - min_ext[i];
  dis = 0.0;
  for (i=0; i<3; i++) dis += dif_ext[i]*dif_ext[i];
  dis = (float)sqrt((double)dis);

  /* set center in world space */
  for (i=0; i<3; i++) center[i] = min_ext[i] + dif_ext[i]/2.0;
//////////////////////////////////////////////////////////////////////////
  /* set initial eye & look at location in world space */
  eye[0] = center[0] + 1.5*dis;
  eye[1] = center[1] + 1.5*dis;
  eye[2] = center[2] + 1.5*dis;
  up[0] = 0.0;
  up[1] = 0.0;
  up[2] = (min_ext[2] + max_ext[2])/2;
//////////////////////////////////////////////////////////////////////////
  /* set parameters for gluPerspective() */
  /* Near- & far clip-plane distances */
  near_clip = (GLdouble)( 0.5*(dis-0.5*dif_ext[2]) );
  far_clip  = (GLdouble)( 2.0*(dis+0.5*dif_ext[2]) );
  /* Field of view */
  fovy = (GLdouble)( 0.5*dif_ext[1]/(dis-0.5*dif_ext[2]) );
  fovy = (GLdouble)( 2*atan((double)fovy)/M_PI*180.0 );
  fovy = (GLdouble)(1.2*fovy);

  /* Enable the color material mode */
  glEnable(GL_COLOR_MATERIAL);
}

/**********************************************************************/
void readConf() {
/***********************************************************************
Read atomic coordinates from an MD-configuration file & allocates
necessary arrays.
***********************************************************************/
  int l, j;

  /* Open an MD-configuration file */
  fp = fopen("md.conf","r");
   /* Read the # of atoms */
  fscanf(fp,"%d",&natoms);
  /* allocate atoms array */
  atoms = (AtomType *) malloc(sizeof(AtomType)*natoms);
  /* Maximum & minimum extent of system in angstroms */
  for (l=0; l<3; l++) fscanf(fp,"%f%f",&min_ext[l],&max_ext[l]);
  /* Atomic coordinates */
  for (j=0; j<natoms; j++)
    fscanf(fp,"%f %f %f",&(atoms[j].crd[0]),&(atoms[j].crd[1]),
                         &(atoms[j].crd[2]));
  fclose(fp);
}
//////////////////////////////////////////////////////////////////////////////
// void readPDB(char *path){
void readPDB(char *path){
/***********************************************************************
Read coarse-grain atomic coordinates and atom type from a PDB file & allocates
necessary arrays.
***********************************************************************/
    int l, j;
    // fp = fopen("./pdb_file/test/init_0.pdb","r");
    fp = fopen(path, "r");
    /* The part below calculates number of lines,
     * (important )for our purposes number of lines = number of atoms*/
    natoms = 0;
    char c;
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            natoms = natoms + 1;
    // Close the file
    fclose(fp);
    atoms = (AtomType *) malloc(sizeof(AtomType)*natoms);
    fp = fopen(path, "r");
    // fp = fopen("./pdb_file/test/init_0.pdb","r");
    for (l=0; l<3; l++) {min_ext[l] = 9999; max_ext[l] = -999;}
    // min_ext[2] = -2; max_ext[2] = 310;
    char * dummy = (char *)malloc(5 * sizeof(char));
    int dummy1, dummy2, dummy3;
    for (j= 0; j<natoms; j++){
        //atoms[j].type = (char *)malloc(5 * sizeof(char));
        //atoms[j].type[4] = "\0";
        fscanf(fp, "%s %d %s %s %d %d %f %f %f", &dummy, &dummy1, &(atoms[j].type),
                &(atoms[j].basetype), &dummy1, &dummy2,
                &(atoms[j].crd[0]),&(atoms[j].crd[1]),
                &(atoms[j].crd[2]));
        for (l=0; l<3; l++) {
            if (min_ext[l] > atoms[j].crd[l])
                min_ext[l] = atoms[j].crd[l];
            else if(max_ext[l] < atoms[j].crd[l])
                max_ext[l] = atoms[j].crd[l];
        }
    }
    fclose(fp);
    for (l=0; l<2; l++) {
        min_ext[l] -= 2;
        max_ext[l] += 2;
    }

}

int cur_pdb_idx = 1;
int max_pdb_idx;
void animate(){/* Callback function for idle events */

  if (cur_pdb_idx < max_pdb_idx){
    sprintf(pdb_file,"%s%s%d%s",path,prefix,cur_pdb_idx,suffix);
    printf(pdb_file);
    printf("\n");

    //Read new pdb file, redraw, and update
    readPDB(pdb_file);
    makeCurframeGeom(); /* Redraw the scene */
    glutPostRedisplay(); /*Force display event */

    //increment and sleep (in microseconds)
    ++cur_pdb_idx;
    usleep(333*1000);
  }
}

////////////////////////////////////////////////////////////////////////////////
/**********************************************************************/
int main(int argc, char **argv) {
/**********************************************************************/
  glutInit(&argc, argv);
  printf("given dir: %s\n", argv[1]);       //checking first argument correctly saved
  strcpy(path,argv[1]);                     //saving directory path to "path" var

  if (path[strlen(path)-1] != "/"){
    strcat(path,"/");           //add trailing "/" if needed
    printf("using dir: %s\n", path);
  }
  char first_pdb[100];          //var for first pdb to be read
  sprintf(first_pdb, "%s%s%d%s",path,prefix,0,suffix);
  printf("%s\n",first_pdb);

  if (argc > 2) {
      max_pdb_idx = atoi(argv[2]);
  }
  else {
      max_pdb_idx = 1;
  }

  /* Read atomic coordinates from an MD-configuration file */
  //readConf();
  //readPDB(argv[argc-1]);
  readPDB(first_pdb);

  /* Set up an window */
  /* Initialize display mode */
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
  /* Specify window size */
  glutInitWindowSize(winx, winy);
  /* Open window */
  glutCreateWindow("RNA Animation");

  /* Initialize view */
  initView(min_ext, max_ext);

  /* Set a glut callback functions */
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
/////////////////////////////////////////////////////////////////////////////
  /* generate an OpenGL display list for single sphere */
  sphereid_XVEC = glGenLists(1);
  sphereid_YVEC = glGenLists(1);
  sphereid_SUGR = glGenLists(1);
  sphereid_PHOS = glGenLists(1);
  sphereid_A = glGenLists(1);
  sphereid_U = glGenLists(1);
  sphereid_G = glGenLists(1);
  sphereid_C = glGenLists(1);
  makeFastNiceSphere(sphereid_XVEC,atom_radius);
  makeFastNiceSphere(sphereid_YVEC,atom_radius);
  makeFastNiceSphere(sphereid_SUGR,atom_radius*15);
  makeFastNiceSphere(sphereid_PHOS,atom_radius*5);
  makeFastNiceSphere(sphereid_A,atom_radius*15);
  makeFastNiceSphere(sphereid_U,atom_radius*15);
  makeFastNiceSphere(sphereid_G,atom_radius*15);
  makeFastNiceSphere(sphereid_C,atom_radius*15);
//////////////////////////////////////////////////////////////////////////////
  /* generate an OpenGL display list for the atoms' geometry */
  atomsid = glGenLists(1);
  /* make the geometry of the current frame's atoms */
  makeCurframeGeom();

  /* Start main display loop */
  glutIdleFunc(animate); //set to call animate when idle
  glutMainLoop();

  return 0;
}
/**********************************************************************/
