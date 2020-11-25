/***********************************************************************
  rnav.h: an include file for rnav.c
***********************************************************************/
#define Ratom 1.0         /* RGB color of an atom */
#define Gatom 1.0
#define Batom 1.0

typedef struct {          /* Atom data type */
  float crd[3];
  char type;
  char basetype;
} AtomType;

int nlon=18, nlat=9;      /* Number of polygons for a sphere in the 
                             longitudinal & lateral directions */
float atom_radius = 0.2;  /* Atomic radius in Lennard-Jones unit */
double* radius_list;
int winx=640, winy=640;   /* Window size */
float min_ext[3], max_ext[3];  
                          /* Range of atomic coordinates:
                             (left,lower,back), (right,top,front) */
int natoms;               /* number of atoms */

AtomType *atoms;          /* array of atoms */
float eye[3];             /* position of eye point */
float center[3];          /* position of look reference point */
float up[3];              /* up direction for camera */
