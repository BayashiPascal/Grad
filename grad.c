// ============ GRAD.C ================

// ================= Include =================

#include "grad.h"
#if BUILDMODE == 0
#include "grad-inline.c"
#endif

// ------------- GradCell

// ================ Functions declaration ====================

// ================ Functions implementation ====================

// Create a new GradCell with index 'id', position 'pos' and 
// 'nbLink' links
GradCell* GradCellCreate(const int id, const int nbLink, 
  const VecShort2D* const pos) {
#if BUILDMODE == 0
  if (id < 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'id' is invalid (%d>=0)", id);
    PBErrCatch(GradErr);
  }
  if (nbLink < 0 || nbLink > GRAD_NBMAXLINK) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'nbLink' is invalid (0<=%d<=%d)", 
      nbLink, GRAD_NBMAXLINK);
    PBErrCatch(GradErr);
  }
  if (pos == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'pos' is null");
    PBErrCatch(GradErr);
  }
  if (VecGet(pos, 0) < 0 || VecGet(pos, 1) < 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'pos' is invalid ((0,0)<=(%d,%d))", 
      VecGet(pos, 0), VecGet(pos, 1));
    PBErrCatch(GradErr);
  }
#endif
  // Allocate memory
  GradCell* that = PBErrMalloc(GradErr, sizeof(GradCell));
  // Set properties
  *that = GradCellCreateStatic(id, nbLink, pos);
  // Return the new GradCell
  return that;
}

// Free the memory used by the GradCell 'that'
void GradCellFree(GradCell** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Fee memory
  free(*that);
  *that = NULL;
}

// Create a new static GradCell with index 'id', position 'pos' 
// and 'nbLink' links
GradCell GradCellCreateStatic(const int id, const int nbLink, 
  const VecShort2D* const pos) {
#if BUILDMODE == 0
  if (id < 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'id' is invalid (%d>=0)", id);
    PBErrCatch(GradErr);
  }
  if (nbLink < 0 || nbLink > GRAD_NBMAXLINK) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'nbLink' is invalid (0<=%d<=%d)", 
      nbLink, GRAD_NBMAXLINK);
    PBErrCatch(GradErr);
  }
  if (pos == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'pos' is null");
    PBErrCatch(GradErr);
  }
  if (VecGet(pos, 0) < 0 || VecGet(pos, 1) < 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'pos' is invalid ((0,0)<=(%d,%d))", 
      VecGet(pos, 0), VecGet(pos, 1));
    PBErrCatch(GradErr);
  }
#endif
  // Declare the new GradCell
  GradCell that;
  // Set properties
  that._id = id;
  that._data = NULL;
  that._pos = *pos;
  for (int iLink = GRAD_NBMAXLINK; iLink--;) {
    that._links[iLink] = -1;
    that._linksVal[iLink] = 1.0;
  }
  that._nbLink = nbLink;
  that._flood = -1;
  that._flagBlocked = false;
  // Return the new GradCell
  return that;
}

// ------------- Grad

int GradDeltaSquare[16] = 
  {0,-1, 1,0, 0,1, -1,0, -1,-1, 1,-1, 1,1, -1,1};
int GradDeltaAEvenQ[12] = {0,-1, 1,-1, 1,0, 0,1, -1,0, -1,-1};
int GradDeltaBEvenQ[12] = {0,-1, 1,0, 1,1, 0,1, -1,1, -1,0};
int GradDeltaAEvenR[12] = {-1,-1, 0,-1, 1,0, 0,1, -1,1, -1,0};
int GradDeltaBEvenR[12] = {0,-1, 1,-1, 1,0, 1,1, 0,1, -1,0};
int GradDeltaAOddQ[12] = {0,-1, 1,0, 1,1, 0,1, -1,1, -1,0};
int GradDeltaBOddQ[12] = {0,-1, 1,-1, 1,0, 0,1, -1,0, -1,-1};
int GradDeltaAOddR[12] = {0,-1, 1,-1, 1,0, 1,1, 0,1, -1,0};
int GradDeltaBOddR[12] = {-1,-1, 0,-1, 1,0, 0,1, -1,1, -1,0};

// ================ Functions declaration ====================

// Create a new static Grad with dimensions 'dim' and type 'type' with 
// cells of 'nbLink' sides
Grad GradCreateStatic(const VecShort2D* const dim, const GradType type, 
  const int nbLink);

// Free memory used by the properties of the Grad 'that'
void GradFreeStatic(Grad* const that);

// Create a new GradHexa with dimensions 'dim' and type 'type'
GradHexa* GradHexaCreate(const VecShort2D* const dim, 
  const GradHexaType type);

// Get the appropriate deltas of positions according to the type of the
// Grad 'that' and the position 'pos'
int* _GradGetDelta(const Grad* const that, const VecShort2D* const pos);

// ================= Polymorphism ==================

#define GradGetDelta(Grad_, Pos) _Generic(Grad_, \
  Grad*: _GradGetDelta, \
  GradSquare*: _GradGetDelta, \
  GradHexa*: _GradGetDelta, \
  default: PBErrInvalidPolymorphism)((Grad*)(Grad_), Pos)

// ================ Functions implementation ====================

// Create a new static Grad with dimensions 'dim' and type 'type' with 
// cells of 'nbLink' sides
Grad GradCreateStatic(const VecShort2D* const dim, const GradType type, 
  const int nbLink) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'dim' is null");
    PBErrCatch(GradErr);
  }
  if (VecGet(dim, 0) <= 0 || VecGet(dim, 1) <= 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'dim' is invalid ((0,0)<(%d,%d))", 
      VecGet(dim, 0), VecGet(dim, 1));
    PBErrCatch(GradErr);
  }
  if (nbLink < 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'nbLink' is invalid (0<=%d)", nbLink);
    PBErrCatch(GradErr);
  }
#endif
  // Declare the new Grad
  Grad that;
  // Set properties
  that._type = type;
  that._dim = *dim;
  int area = GradGetArea(&that);
  that._cells = PBErrMalloc(GradErr, sizeof(GradCell) * area);
  VecShort2D pos = VecShortCreateStatic2D();
  int iCell = 0;
  // Loop on cells
  do {
    // Initialise the cell
    that._cells[iCell] = GradCellCreateStatic(iCell, nbLink, &pos);
    ++iCell;
  } while (VecPStep(&pos, dim));
  // Return the new Grad
  return that;
}

// Free memory used by the properties of the Grad 'that'
void GradFreeStatic(Grad* const that) {
  // Check arguments
  if (that == NULL)
    // Nothing to do
    return;
  // Free memory
  free(that->_cells);
}

// Get the appropriate deltas of positions according to the type of the
// Grad 'that' and the position 'pos'
int* _GradGetDelta(const Grad* const that, const VecShort2D* const pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (pos == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'pos' is null");
    PBErrCatch(GradErr);
  }
#endif
  if (GradGetType(that) == GradTypeSquare) {
    return GradDeltaSquare;
  } else if (GradGetType(that) == GradTypeHexa) {
    if (GradHexaGetType((GradHexa*)that) == GradHexaTypeEvenQ) {
      if ((VecGet(pos, 0) % 2) != 0)
        return GradDeltaAEvenQ;
      else
        return GradDeltaBEvenQ;
    } else if (GradHexaGetType((GradHexa*)that) == GradHexaTypeEvenR) {
      if ((VecGet(pos, 1) % 2) != 0)
        return GradDeltaAEvenR;
      else
        return GradDeltaBEvenR;
    } else if (GradHexaGetType((GradHexa*)that) == GradHexaTypeOddQ) {
      if ((VecGet(pos, 0) % 2) != 0)
        return GradDeltaAOddQ;
      else
        return GradDeltaBOddQ;
    } else if (GradHexaGetType((GradHexa*)that) == GradHexaTypeOddR) {
      if ((VecGet(pos, 1) % 2) != 0)
        return GradDeltaAOddR;
      else
        return GradDeltaBOddR;
    }
  }
  return NULL;
}

// Create a new GradSquare of dimensions 'dim' and diagonal links 
// allowed if 'diagLink' equals true
GradSquare* GradSquareCreate(const VecShort2D* const dim, 
  const bool diagLink) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'dim' is null");
    PBErrCatch(GradErr);
  }
  if (VecGet(dim, 0) <= 0 || VecGet(dim, 1) <= 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'dim' is invalid ((0,0)<(%d,%d))", 
      VecGet(dim, 0), VecGet(dim, 1));
    PBErrCatch(GradErr);
  }
#endif
  // Allocate memory
  GradSquare *that = PBErrMalloc(GradErr, sizeof(GradSquare));
  // Set properties
  int nbLink = (diagLink ? 8 : 4);
  that->_grad = GradCreateStatic(dim, GradTypeSquare, nbLink);
  that->_diagLink = diagLink;
  // Loop on cells to initialise the links
  VecShort2D pos = VecShortCreateStatic2D();
  int* delta = GradGetDelta(that, &pos);
  VecShort2D p = VecShortCreateStatic2D();
  do {
    // Initialise the links of the cell
    for (int iLink = nbLink; iLink--;) {
      VecSet(&p, 0, VecGet(&pos, 0) + delta[iLink * 2]);
      VecSet(&p, 1, VecGet(&pos, 1) + delta[iLink * 2 + 1]);
      if (GradIsPosInside(that, &p))
        GradCellSetLink(GradCellAt(that, &pos), iLink, 
          GradCellGetId(GradCellAt(that, &p)));
      if (iLink >= 4)
        GradCellSetLinkVal(GradCellAt(that, &pos), iLink, 
          PBMATH_SQRTTWO);
    }
  } while (VecStep(&pos, dim));
  // Return the new Grad
  return that;
}

// Free the memory used by the GradSquare 'that'
void GradSquareFree(GradSquare** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Fee memory
  GradFreeStatic(&((*that)->_grad));
  free(*that);
  *that = NULL;
}

// Create a new GradHexa with dimensions 'dim' and type 'type'
GradHexa* GradHexaCreate(const VecShort2D* const dim, 
  const GradHexaType type) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'dim' is null");
    PBErrCatch(GradErr);
  }
  if (VecGet(dim, 0) <= 0 || VecGet(dim, 1) <= 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'dim' is invalid ((0,0)<(%d,%d))", 
      VecGet(dim, 0), VecGet(dim, 1));
    PBErrCatch(GradErr);
  }
#endif
  // Allocate memory
  GradHexa* that = PBErrMalloc(GradErr, sizeof(GradHexa));
  // Set properties
  that->_grad = GradCreateStatic(dim, GradTypeHexa, 6);
  that->_type = type;
  // Return the new Grad
  return that;
}

// Create a new GradHexa of dimensions 'dim' and orientation odd-r
GradHexa* GradHexaCreateOddR(const VecShort2D* const dim) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'dim' is null");
    PBErrCatch(GradErr);
  }
  if (VecGet(dim, 0) <= 0 || VecGet(dim, 1) <= 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'dim' is invalid ((0,0)<(%d,%d))", 
      VecGet(dim, 0), VecGet(dim, 1));
    PBErrCatch(GradErr);
  }
#endif
  // Create the GradHexa
  GradHexa* that = GradHexaCreate(dim, GradHexaTypeOddR);
  // Loop on cells to initialise the links
  VecShort2D pos = VecShortCreateStatic2D();
  VecShort2D p = VecShortCreateStatic2D();
  int* delta = NULL;
  do {
    // Initialise the links of the cell
    delta = GradGetDelta(that, &pos);
    for (int iLink = 6; iLink--;) {
      VecSet(&p, 0, VecGet(&pos, 0) + delta[iLink * 2]);
      VecSet(&p, 1, VecGet(&pos, 1) + delta[iLink * 2 + 1]);
      if (GradIsPosInside(that, &p))
        GradCellSetLink(GradCellAt(that, &pos), iLink, 
          GradCellGetId(GradCellAt(that, &p)));
    }
  } while (VecStep(&pos, dim));
  // Return the new Grad
  return that;
}

// Create a new GradHexa of dimensions 'dim' and orientation even-r
GradHexa* GradHexaCreateEvenR(const VecShort2D* const dim) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'dim' is null");
    PBErrCatch(GradErr);
  }
  if (VecGet(dim, 0) <= 0 || VecGet(dim, 1) <= 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'dim' is invalid ((0,0)<(%d,%d))", 
      VecGet(dim, 0), VecGet(dim, 1));
    PBErrCatch(GradErr);
  }
#endif
  // Create the GradHexa
  GradHexa* that = GradHexaCreate(dim, GradHexaTypeEvenR);
  // Loop on cells to initialise the links
  VecShort2D pos = VecShortCreateStatic2D();
  VecShort2D p = VecShortCreateStatic2D();
  int* delta = NULL;
  do {
    // Initialise the links of the cell
    delta = GradGetDelta(that, &pos);
    for (int iLink = 6; iLink--;) {
      VecSet(&p, 0, VecGet(&pos, 0) + delta[iLink * 2]);
      VecSet(&p, 1, VecGet(&pos, 1) + delta[iLink * 2 + 1]);
      if (GradIsPosInside(that, &p))
        GradCellSetLink(GradCellAt(that, &pos), iLink, 
          GradCellGetId(GradCellAt(that, &p)));
    }
  } while (VecStep(&pos, dim));
  // Return the new Grad
  return that;
}

// Create a new GradHexa of dimensions 'dim' and orientation odd-q
GradHexa* GradHexaCreateOddQ(const VecShort2D* const dim) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'dim' is null");
    PBErrCatch(GradErr);
  }
  if (VecGet(dim, 0) <= 0 || VecGet(dim, 1) <= 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'dim' is invalid ((0,0)<(%d,%d))", 
      VecGet(dim, 0), VecGet(dim, 1));
    PBErrCatch(GradErr);
  }
#endif
  // Create the GradHexa
  GradHexa* that = GradHexaCreate(dim, GradHexaTypeOddQ);
  // Loop on cells to initialise the links
  VecShort2D pos = VecShortCreateStatic2D();
  VecShort2D p = VecShortCreateStatic2D();
  int* delta = NULL;
  do {
    // Initialise the links of the cell
    delta = GradGetDelta(that, &pos);
    for (int iLink = 6; iLink--;) {
      VecSet(&p, 0, VecGet(&pos, 0) + delta[iLink * 2]);
      VecSet(&p, 1, VecGet(&pos, 1) + delta[iLink * 2 + 1]);
      if (GradIsPosInside(that, &p))
        GradCellSetLink(GradCellAt(that, &pos), iLink, 
          GradCellGetId(GradCellAt(that, &p)));
    }
  } while (VecStep(&pos, dim));
  // Return the new Grad
  return that;
}

// Create a new GradHexa of dimensions 'dim' and orientation even-q
GradHexa* GradHexaCreateEvenQ(const VecShort2D* const dim) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'dim' is null");
    PBErrCatch(GradErr);
  }
  if (VecGet(dim, 0) <= 0 || VecGet(dim, 1) <= 0) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'dim' is invalid ((0,0)<(%d,%d))", 
      VecGet(dim, 0), VecGet(dim, 1));
    PBErrCatch(GradErr);
  }
#endif
  // Create the GradHexa
  GradHexa* that = GradHexaCreate(dim, GradHexaTypeEvenQ);
  // Loop on cells to initialise the links
  VecShort2D pos = VecShortCreateStatic2D();
  VecShort2D p = VecShortCreateStatic2D();
  int* delta = NULL;
  do {
    // Initialise the links of the cell
    delta = GradGetDelta(that, &pos);
    for (int iLink = 6; iLink--;) {
      VecSet(&p, 0, VecGet(&pos, 0) + delta[iLink * 2]);
      VecSet(&p, 1, VecGet(&pos, 1) + delta[iLink * 2 + 1]);
      if (GradIsPosInside(that, &p))
        GradCellSetLink(GradCellAt(that, &pos), iLink, 
          GradCellGetId(GradCellAt(that, &p)));
    }
  } while (VecStep(&pos, dim));
  // Return the new Grad
  return that;
}

// Free the memory used by the GradHexa 'that'
void GradHexaFree(GradHexa** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Fee memory
  GradFreeStatic(&((*that)->_grad));
  free(*that);
  *that = NULL;
}

// Get the GradCell at position 'pos' int the GradHexa 'that'
GradCell* _GradHexaCellAtPos(GradHexa* const that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (pos == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'pos' is null");
    PBErrCatch(GradErr);
  }
  if (VecGet(pos, 0) < 0 || VecGet(pos, 1) < 0 ||
    VecGet(pos, 0) >= VecGet(GradDim(that), 0) || 
    VecGet(pos, 1) >= VecGet(GradDim(that), 1)) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'pos' is invalid ((0,0)<=(%d,%d)<(%d,%d))", 
      VecGet(pos, 0), VecGet(pos, 1), 
      VecGet(GradDim(that), 0), VecGet(GradDim(that), 1));
    PBErrCatch(GradErr);
  }
#endif
(void)that;(void)pos;
  // Return the result
  return NULL;
}

// Get the look up table for distance between each pair of cell of the 
// Grad 'that'
// Return a MatFloat where first index is the 'from' cell's index
// and second index is the 'to' cell index
// Distances in the matrix are equal to the sum of the value of links 
// between cells 
// Negative distance means there is no path for the pair of cell
MatFloat* _GradGetLookupTableMinDist(const Grad* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Get the area of the grad
  int area = GradGetArea(that);
  // Create the result matrix
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, area); VecSet(&dim, 1, area);
  MatFloat* table = MatFloatCreate(&dim);
  // Initialise the table
  for (int iCell = area * area; iCell--;)
    table->_val[iCell] = -1.0;
  VecShort2D pair = VecShortCreateStatic2D();
  for (int iCell = area; iCell--;) {
    VecSet(&pair, 0, iCell);
    GradCell* cellFrom = GradCellAt(that, iCell);
    if (!GradCellIsBlocked(cellFrom)) {
      for (int iLink = GradCellGetNbLink(cellFrom); iLink--;) {
        int link = GradCellGetLink(cellFrom, iLink);
        if (link != -1 &&
          !GradCellIsBlocked(GradCellAt(that, link))) {
          VecSet(&pair, 1, GradCellGetLink(cellFrom, iLink));
          MatSet(table, &pair, GradCellLinkVal(cellFrom, iLink));
        }
      }
    }
  }
  // Loop until there is no more modification or we reach area steps
  int nbStep = 0;
  bool flagModif;
  VecShort2D pairA = VecShortCreateStatic2D();
  VecShort2D pairB = VecShortCreateStatic2D();
  do {
    // Reset the flag for modification
    flagModif = false;
    // For each pair of cell
    VecSetNull(&pair);
    do {
      // If it's not a pair on the diagonal
      if (VecGet(&pair, 0) != VecGet(&pair, 1)) {
        // Search the minimum dist for this pair via another cell
        float min = -1.0;
        VecSet(&pairA, 0, VecGet(&pair, 0));
        VecSet(&pairB, 1, VecGet(&pair, 1));
        for (int k = area; k--;) {
          // If the other cell is different than the one in the 
          // current pair
          if (k != VecGet(&pair, 0) && k != VecGet(&pair, 1)) {
            VecSet(&pairA, 1, k);
            VecSet(&pairB, 0, k);
            // If the path through this other cell exists
            if (MatGet(table, &pairA) >= 0.0 && 
              MatGet(table, &pairB) >= 0.0) {
              float d = MatGet(table, &pairA) + MatGet(table, &pairB);
              if (min < 0.0 || min > d)
                min = d;
            }
          }
        }
        // If there was a path via another cell and this path is
        // shorter than the current one or there is no current one
        if (min >= 0.0 && 
          (MatGet(table, &pair) < 0.0 || MatGet(table, &pair) > min)) {
          // Update the min distance
          MatSet(table, &pair, min);
        }
      }
    } while(VecStep(&pair, &dim));
    // Increment the number of steps
    ++nbStep;
  } while (nbStep < area && flagModif);
  // Return the result
  return table;
}

// Get the path from cell at index 'from' to cell at index 'to' in 
// the Grad 'that' using the A* algorithm and the look up table 'lookup'
// for distance estimation between cells
// Return a VecShort of position (index) ordered from 'from' to 'to'
// Return NULL if there is no path
VecShort* _GradGetPath(const Grad* const that, const int from, 
  const int to, const MatFloat* const lookup) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (lookup == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'lookup' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Declare a vector to onsult the lookup table
  VecShort2D iLookUp = VecShortCreateStatic2D();
  // Get the estimated distance of the path
  VecSet(&iLookUp, 0, from); VecSet(&iLookUp, 1, to);
  float dist = MatGet(lookup, &iLookUp);
  // Get the starting cell
  GradCell* cell = GradCellAt(that, from);
  // Declare a GSet of GradCell for computation
  GSet openList = GSetCreateStatic();
  // Declare a GSet to memorize the path
  GSet path = GSetCreateStatic();
  // Init the GSet with the starting cell
  GSetPush(&openList, cell);
  // Get the area of the grad
  int area = GradGetArea(that);
  // Declare arrays for computation
  float* f = PBErrMalloc(GradErr, sizeof(float) * area);
  float* g = PBErrMalloc(GradErr, sizeof(float) * area);
  float* h = PBErrMalloc(GradErr, sizeof(float) * area);
  bool* flagOpen = PBErrMalloc(GradErr, sizeof(bool) * area);
  bool* flagClose = PBErrMalloc(GradErr, sizeof(bool) * area);
  int* prev = PBErrMalloc(GradErr, sizeof(int) * area);
  // Init the arrays
  for (int i = area; i--;) {
    f[i] = dist;
    g[i] = 0.0;
    h[i] = dist;
    flagOpen[i] = false;
    flagClose[i] = false;
    prev[i] = -1;
  }
  flagOpen[from] = true;
  // Loop until we have elements in the openList
  while (GSetNbElem(&openList) > 0) {
    cell = GSetPop(&openList);
    int iCell = GradCellGetId(cell);
    if (iCell == to) {
      while (iCell != -1) {
        GSetPush(&path, cell);
        iCell = prev[GradCellGetId(cell)];
        if (iCell != -1)
          cell = GradCellAt(that, iCell);
      }
      GSetFlush(&openList);
    } else {
      flagClose[iCell] = true;
      float curDist = g[iCell];
      for (int iDir = GradCellGetNbLink(cell); iDir--;) {
        int ncell = GradCellGetLink(cell, iDir);
        if (ncell != -1) {
          if (flagClose[ncell] == false) {
            GradCell* nextCell = GradCellAt(that, ncell);
            if (flagOpen[ncell] == false) {
              if (!GradCellIsBlocked(nextCell)) {
                VecSet(&iLookUp, 0, iCell);
                VecSet(&iLookUp, 1, ncell);
                g[ncell] = curDist + MatGet(lookup, &iLookUp);
                VecSet(&iLookUp, 0, ncell);
                VecSet(&iLookUp, 1, to);
                dist = MatGet(lookup, &iLookUp);
                h[ncell] = dist;
                f[ncell] = dist + g[ncell];
                GSetAddSort(&openList, nextCell, f[ncell]);
                flagOpen[ncell] = true;
                prev[ncell] = iCell;
              }
            } else {
              VecSet(&iLookUp, 0, iCell);
              VecSet(&iLookUp, 1, ncell);
              float ng = curDist + MatGet(lookup, &iLookUp);
              if (ng < g[ncell]) {
                GSetRemoveAll(&openList, nextCell);
                g[ncell] = ng;
                f[ncell] = g[ncell] + h[ncell];
                prev[ncell] = iCell;
                GSetAddSort(&openList, nextCell, f[ncell]);
              }
            }
          }
        }
      }
    }
  }
  // Free memory
  free(f);
  free(g);
  free(h);
  free(flagOpen);
  free(flagClose);
  free(prev);
  GSetFlush(&openList);
  // Return the result
  if (GSetNbElem(&path) == 0) {
    return NULL;
  } else {
    VecShort* res = VecShortCreate(GSetNbElem(&path));
    int i = 0;
    while(GSetNbElem(&path) > 0) {
      cell = GSetPop(&path);
      VecSet(res, i, GradCellGetId(cell));
      ++i;
    }
    return res;
  }
}

// Structure used for flooding
typedef struct GradFloodPod {
  GradCell* _cell;
  int _src;
  int _nbStep;
} GradFloodPod;

// Flood the Grad 'that' from positions (index) 'sources' up to a 
// maximum distance in link's value from the source 'distMax' or
// maximum distance in nb of cell from the source 'stepMax'
// If 'distMax' and/or 'stepMax' are/is negative(s) their is no limit 
// on the maximum distance/maximum number of steps
// The flood occurs in order consistent with the links' value 
// interpreted as distance
// The result is stored in the _flood property of the GradCell
// _flood == -1: not flooded, _flood >= 0: flooded by the _flood-th 
// source 
// Conflicting cells (several sources arriving at the same step to the 
// cell) are left undecided (_flood==-1)
void _GradFlood(Grad* const that, const VecShort* const sources, 
  const float distMax, const int stepMax) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (sources == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'sources' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Reset all the flood value to -1
  for (int iCell = GradGetArea(that); iCell--;)
    GradCellSetFlood(GradCellAt(that, iCell), -1);
  // Get the nb of sources
  int nbSrc = VecGetDim(sources);
  // Declare a set of GradFloodPod
  GSet set = GSetCreateStatic();
  // For each sources
  for (int iSource = nbSrc; iSource--;) {
    // Add the first cell in the set
    GradFloodPod* pod = PBErrMalloc(GradErr, sizeof(GradFloodPod));
    pod->_src = iSource;
    pod->_cell = GradCellAt(that, VecGet(sources, iSource));
    pod->_nbStep = 0;
    GSetAddSort(&set, pod, 0.0);
  }
  // Loop until the set is empty (ie every cell has 
  // been flooded
  while (GSetNbElem(&set) > 0) {
    // Get the distance up to this cell
    float dist = GSetGetElem(&set, 0)->_sortVal;
    // Pop the cell
    GradFloodPod* pod = GSetPop(&set);
    // If the cell is inside the limit in nb of steps
    if (stepMax < 0 || pod->_nbStep <= stepMax) {
      // Declare a variable to manage conflict
      bool flagConflict = false;
      //if the set is not empty
      if (GSetNbElem(&set) > 0) {
        // Check references to this cell from other sources and 
        // eliminate n-uples
        GSetIterForward iter = GSetIterForwardCreateStatic(&set);
        bool skipStep; 
        do {
          skipStep = false; 
          GradFloodPod* podCheck = GSetIterGet(&iter);
          if (podCheck->_cell == pod->_cell) {
            float d = GSetIterGetElem(&iter)->_sortVal;
            if (podCheck->_src != pod->_src && ISEQUALF(d, dist))
              flagConflict = true;
            free(podCheck);
            skipStep = GSetIterRemoveElem(&iter);
          }
        } while (skipStep || GSetIterStep(&iter));
      }
      // If there was no conflict
      if (!flagConflict) {
        // Set the flood value of sources
        GradCellSetFlood(pod->_cell, pod->_src);
        // Loop on direction from this cell
        for (int iLink = GradCellGetNbLink(pod->_cell); iLink--;) {
          int toCell = GradCellGetLink(pod->_cell, iLink);
          // If there is a cell in this direction
          if (toCell != -1) {
            // Get the distance to this cell from the source
            float d = dist + GradCellLinkVal(pod->_cell, iLink);
            // If it's within the max distance
            if (distMax < 0.0 || d <= distMax) {
              GradCell* cellTo = GradCellAt(that, toCell);
              // If it's not yet flooded, not blocked, and not 
              // conflicting
              if (GradCellGetFlood(cellTo) == -1 &&
                !GradCellIsBlocked(cellTo)) {
                // Add a new pod to the GSet 'setOut'
                GradFloodPod* npod = 
                  PBErrMalloc(GradErr, sizeof(GradFloodPod));
                npod->_src = pod->_src;
                npod->_nbStep = pod->_nbStep + 1;
                npod->_cell = cellTo;
                GSetAddSort(&set, npod, d);
              }
            }
          }
        }
      }
    }
    // Free memory used by the GradFloodPod
    free(pod);
  }
  // Free memory
  while (GSetNbElem(&set) > 0) {
    GradFloodPod* pod = GSetPop(&set);
    free(pod);
  }
}

// Get the number of flooded cells from 'iSource'-th source in the Grad
// 'that'
int _GradGetFloodArea(const Grad* const that, const int iSource) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Declare a variable to memorize the result
  int nb = 0;
  // Loop on cells
  for (int iCell = GradGetArea(that); iCell--;) {
    // If the flood value of the cell is the serached value
    if (GradCellGetFlood(GradCellAt(that, iCell)) == iSource)
      // Increment the result
      ++nb;
  }
  // Return the result
  return nb;
}

// Clone the GradSquare 'that'
// The user data are not cloned but shared between the original and 
// its clone
GradSquare* GradSquareClone(const GradSquare* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Allocate memory
  GradSquare* clone = PBErrMalloc(GradErr, sizeof(GradSquare));
  // Copy the GradSquare
  *clone = *that;
  // Clone the GradCell
  clone->_grad._cells = PBErrMalloc(GradErr, 
    sizeof(GradCell) * GradGetArea(that));
  memcpy(clone->_grad._cells, that->_grad._cells, 
    sizeof(GradCell) * GradGetArea(that));
  // Return the clone
  return clone;
}

// Clone the GradHexa 'that'
// The user data are not cloned but shared between the original and 
// its clone
GradHexa* GradHexaClone(const GradHexa* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Allocate memory
  GradHexa* clone = PBErrMalloc(GradErr, sizeof(GradHexa));
  // Copy the GradHexa
  *clone = *that;
  // Clone the GradCell
  clone->_grad._cells = PBErrMalloc(GradErr, 
    sizeof(GradCell) * GradGetArea(that));
  memcpy(clone->_grad._cells, that->_grad._cells, 
    sizeof(GradCell) * GradGetArea(that));
  // Return the clone
  return clone;
}

// Return true if the Grad 'that' is same as the Grad 'tho'
// Return false else
bool _GradIsSameAs(const Grad* const that, const Grad* const tho) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (tho == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'tho' is null");
    PBErrCatch(GradErr);
  }
#endif
  if (that->_type != tho->_type ||
    !VecIsEqual(&(that->_dim), &(tho->_dim)) ||
    (that->_type == GradTypeSquare && 
      ((GradSquare*)that)->_diagLink != ((GradSquare*)tho)->_diagLink) ||
    (that->_type == GradTypeHexa && 
      ((GradHexa*)that)->_type != ((GradHexa*)tho)->_type)) {
    return false;
  } else {
    for (int iCell = GradGetArea(that); iCell--;) {
      GradCell* cellA = GradCellAt(that, iCell);
      GradCell* cellB = GradCellAt(tho, iCell);
      if (cellA->_data != cellB->_data ||
        cellA->_id != cellB->_id ||
        !VecIsEqual(&(cellA->_pos), &(cellB->_pos)) ||
        cellA->_nbLink != cellB->_nbLink ||
        cellA->_flood != cellB->_flood ||
        cellA->_flagBlocked != cellB->_flagBlocked ||
        memcmp(cellA->_links, cellB->_links, 
          sizeof(int) * GRAD_NBMAXLINK) != 0 ||
        memcmp(cellA->_linksVal, cellB->_linksVal, 
          sizeof(float) * GRAD_NBMAXLINK) != 0) {
        return false;
      }
    }
    return true;
  }
}

// Remove the link from cell 'fromCell' to cell 'toCell' in the
// Grad 'that'
// If 'symmetric' equals true the symetric link is removed too
// (only if the link from 'fromCell' exists)
void _GradRemoveLinkIndex(Grad* const that, const int fromCell, 
  const int toCell, const bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Loop on links of the 'fromCell'
  GradCell* cell = GradCellAt(that, fromCell);
  for (int iLink = GradCellGetNbLink(cell); iLink--;) {
    // If it's the link toward the 'toCell' 
    if (GradCellGetLink(cell, iLink) == toCell) {
      // Remove it
      GradCellSetLink(cell, iLink, -1);
      // If we have to remove the symmetric link
      if (symmetric)
        _GradRemoveLinkIndex(that, toCell, fromCell, false);
      // Skip the end of the loop
      break;
    }
  }
}

// Remove the link from cell at position 'fromCell' toward direction 
// 'dir' in the Grad 'that'
// If 'symmetric' equals true the symetric link is removed too
// (only if the link from 'fromCell' exists)
void _GradRemoveDirIndex(Grad* const that, const int fromCell, 
  const int dir, const bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Get the cell
  GradCell* cell = GradCellAt(that, fromCell);
  // Get the neighbour cell
  int toCell = GradCellGetLink(cell, dir);
  // If there is a link in this direction
  if (toCell != -1) {
    // If we have to remove the symmetric link
    if (symmetric)
      _GradRemoveLinkIndex(that, toCell, fromCell, false);
    // Remove the link
    GradCellSetLink(cell, dir, -1);
  }
}

// Remove all the links from cell 'fromCell' in the Grad 'that'
// If 'symmetric' equals true the symetric links are removed too
// (only if the link from 'fromCell' exists)
void _GradRemoveAllLinkIndex(Grad* const that, const int fromCell, 
  const bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Loop on links of the 'fromCell'
  GradCell* cell = GradCellAt(that, fromCell);
  for (int iLink = GradCellGetNbLink(cell); iLink--;) {
    // Memorize the link
    int toCell = GradCellGetLink(cell, iLink);
    // Remove the link
    GradCellSetLink(cell, iLink, -1);
    // If we have to remove the symmetric link and it exists
    if (symmetric && toCell != -1)
      _GradRemoveLinkIndex(that, toCell, fromCell, false);
  }
}

// Add the link from cell 'fromCell' to cell 'toCell' in the
// Grad 'that'
// If the cells are not neighbours do nothing
// If 'symmetric' equals true the symetric link is added too
void _GradAddLinkIndex(Grad* const that, int fromCell, int toCell, 
  bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Declare pointer to the cells
  GradCell* cell = GradCellAt(that, fromCell);
  GradCell* cellTo = GradCellAt(that, toCell);
  // Declare variable for computation
  VecShort2D p = VecShortCreateStatic2D();
  // Get the table of delta position given the type of Grad
  int* delta = GradGetDelta(that, GradCellPos(cell));
  // Loop on links of the 'fromCell'
  for (int iLink = GradCellGetNbLink(cell); iLink--;) {
    // Get the position in this direction
    for (int i = 2; i--;)
      VecSet(&p, i, VecGet(GradCellPos(cell), i) + delta[2 * iLink + i]);
    // If it's the link toward the 'toCell' 
    if (VecIsEqual(&p, GradCellPos(cellTo))) {
      // Add it
      GradCellSetLink(cell, iLink, toCell);
      // If we have to add the symmetric link
      if (symmetric)
        _GradAddLinkIndex(that, toCell, fromCell, false);
      // Skip the end of the loop
      break;
    }
  }
}

// Add the link from cell at position 'fromCell' toward direction 
// 'dir' in the Grad 'that'
// If the cells are not neighbours do nothing
// If 'symmetric' equals true the symetric link is added too
void _GradAddDirIndex(Grad* const that, int const fromCell, 
  const int dir, const bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Get the cell
  GradCell* cell = GradCellAt(that, fromCell);
  // Get the delta pos
  int* delta = GradGetDelta(that, GradCellPos(cell));
  // Get the neighbour cell pos
  VecShort2D p = VecShortCreateStatic2D();
  for (int i = 2; i--;)
    VecSet(&p, i, VecGet(GradCellPos(cell), i) + delta[2 * dir + i]);
  // If the neighbour cell exists
  if (GradIsPosInside(that, &p)) {
    // Get the neighbour cell
    GradCell* cellTo = GradCellAt(that, &p);
    int toCell = GradCellGetId(cellTo);
    // Set the link
    GradCellSetLink(cell, dir, toCell);
    // If we have to add the symmetric link
    if (symmetric)
      GradAddLinkTo(that, toCell, fromCell, false);
  }
}

// Add all the links from cell 'fromCell' in the Grad 'that'
// If 'symmetric' equals true the symetric links are removed too
void _GradAddAllLinkIndex(Grad* const that, const int fromCell, 
  const bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Declare pointer to the cell
  GradCell* cell = GradCellAt(that, fromCell);
  // Declare variable for computation
  VecShort2D p = VecShortCreateStatic2D();
  // Get the table of delta position given the type of Grad
  int* delta = GradGetDelta(that, GradCellPos(cell));
  // Loop on links of the 'fromCell'
  for (int iLink = GradCellGetNbLink(cell); iLink--;) {
    // Get the position in this direction
    for (int i = 2; i--;)
      VecSet(&p, i, VecGet(GradCellPos(cell), i) + delta[2 * iLink + i]);
    // If the position is inside the Grad
    if (GradIsPosInside(that, &p)) {
      GradCell* cellTo = GradCellAt(that, &p);
      int toCell = GradCellGetId(cellTo);
      // Add the link
      GradCellSetLink(cell, iLink, toCell);
      // If we have to add the symmetric link
      if (symmetric)
        _GradAddLinkIndex(that, toCell, fromCell, false);
    }
  }
}


