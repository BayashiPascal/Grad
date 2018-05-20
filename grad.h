// ============ GRAD.H ================

#ifndef GRAD_H
#define GRAD_H

// https://www.redblobgames.com/grids/hexagons/

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "pberr.h"
#include "pbmath.h"
#include "gset.h"

// ================= Define ==================

#define GRAD_NBMAXLINK 8
#define GRAD_DEFAUTLINKVAL 1.0

#define GradPosToIndex(Grad_, Pos) \
  (VecGet(Pos, 1) * VecGet(GradDim(Grad_), 0) + VecGet(Pos, 0))

// ------------- GradCell

// ================= Data structure ===================

typedef struct GradCell {
  // Index in the grad
  int _id;
  // Position in the grad
  VecShort2D _pos;
  // Index in the grad of the neighbour cells
  // -1 means no link
  int _links[GRAD_NBMAXLINK];
  // Nb of links
  int _nbLink;
  // Links' value
  float _linksVal[GRAD_NBMAXLINK];
  // Flood value (_flood == -1: not flooded, _flood >= 0: flooded by 
  // the _flood-th source 
  int _flood;
  // Flag to mark blocked cell in lookuptable, flood and getPath
  // if this flag equals true the cell is not explored
  bool _flagBlocked;
  // User data
  void* _data;
} GradCell;

// ================ Functions declaration ====================

// Create a new GradCell with index 'id', position 'pos' and 
// 'nbLink' links
GradCell* GradCellCreate(const int id, const int nbLink, 
  const VecShort2D* const pos);

// Free the memory used by the GradCell 'that'
void GradCellFree(GradCell** that);

// Create a new static GradCell with index 'id', position 'pos'
// and 'nbLink' links
GradCell GradCellCreateStatic(const int id, const int nbLink, 
  const VecShort2D* const pos);

// Get the user data of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
void* GradCellData(const GradCell* const that);

// Set the user data of the GradCell 'that' to 'data'
#if BUILDMODE != 0
inline
#endif 
void GradCellSetData(GradCell* const that, void* const data);

// Get the position of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GradCellPos(const GradCell* const that);

// Get the index of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
int GradCellGetId(const GradCell* const that);

// Get the index of 'iLink'-th link of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
int GradCellGetLink(const GradCell* const that, const int iLink);

// Set the index of 'iLink'-th link of the GradCell 'that' to 'iCell'
#if BUILDMODE != 0
inline
#endif 
void GradCellSetLink(GradCell* const that, const int iLink, 
  const int iCell);

// Get the number of links of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
int GradCellGetNbLink(const GradCell* const that);

// Get the value of 'iLink'-th link of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
float GradCellLinkVal(const GradCell* const that, const int iLink);

// Set the value of 'iLink'-th link of the GradCell 'that' to 'val'
#if BUILDMODE != 0
inline
#endif 
void GradCellSetLinkVal(GradCell* const that, const int iLink, 
  const float val);

// Get the flood value of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
int GradCellGetFlood(const GradCell* const that);

// Set the flood value of the GradCell 'that' to 'iSource'
#if BUILDMODE != 0
inline
#endif 
void GradCellSetFlood(GradCell* const that, const int iSource);

// Get the flag blocked of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
bool GradCellIsBlocked(const GradCell* const that);

// Set the flag blocked of the GradCell 'that' to 'flag'
#if BUILDMODE != 0
inline
#endif 
void GradCellSetBlocked(GradCell* const that, const bool flag);

// ------------- Grad

// ================= Data structure ===================

typedef enum GradType {
  GradTypeSquare, GradTypeHexa
} GradType;

typedef enum GradHexaType {
  GradHexaTypeOddR, GradHexaTypeEvenR, GradHexaTypeOddQ, GradHexaTypeEvenQ
} GradHexaType;

typedef struct Grad {
  // Type
  GradType _type;
  // Cells
  GradCell* _cells;
  // Dimensions
  VecShort2D _dim;
} Grad;

// Direction for x toward east and y toward south
typedef enum GradSquareDir {
  GradSquareDirN, GradSquareDirE, GradSquareDirS, GradSquareDirW, 
  GradSquareDirNW, GradSquareDirNE, GradSquareDirSE, GradSquareDirSW
} GradSquareDir;

typedef struct GradSquare {
  // Grad
  Grad _grad;
  // Flag to memorize if diagonal links are allowed
  bool _diagLink;
} GradSquare;

// Direction for x toward east and y toward south
typedef enum GradHexaOddDir {
  GradHexaOddDirNW, GradHexaOddDirNE, GradHexaOddDirE, 
  GradHexaOddDirSE, GradHexaOddDirSW, GradHexaOddDirW
} GradHexaOddDir;

typedef enum GradHexaEvenDir {
  GradHexaEvenDirN, GradHexaEvenDirNE, GradHexaEvenDirSE, 
  GradHexaEvenDirS, GradHexaEvenDirSW, GradHexaEvenDirNW
} GradHexaEvenDir;

typedef struct GradHexa {
  // Grad
  Grad _grad;
  // Type
  GradHexaType _type;
} GradHexa;

// ================ Functions declaration ====================

// Create a new GradSquare of dimensions 'dim' and diagonal links 
// allowed if 'diagLink' equals true
GradSquare* GradSquareCreate(const VecShort2D* const dim, 
  const bool diagLink);

// Free the memory used by the GradSquare 'that'
void GradSquareFree(GradSquare** that);

// Create a new GradHexa of dimensions 'dim' and orientation odd-r
GradHexa* GradHexaCreateOddR(const VecShort2D* const dim);

// Create a new GradHexa of dimensions 'dim' and orientation even-r
GradHexa* GradHexaCreateEvenR(const VecShort2D* const dim);

// Create a new GradHexa of dimensions 'dim' and orientation odd-q
GradHexa* GradHexaCreateOddQ(const VecShort2D* const dim);

// Create a new GradHexa of dimensions 'dim' and orientation even-q
GradHexa* GradHexaCreateEvenQ(const VecShort2D* const dim);

// Free the memory used by the GradHexa 'that'
void GradHexaFree(GradHexa** that);

// Get the GradCell at index 'iCell' in the Grad 'that'
#if BUILDMODE != 0
inline
#endif 
GradCell* _GradCellAtIndex(const Grad* const that, const int iCell);

// Get the GradCell at position 'pos' int the Grad 'that'
#if BUILDMODE != 0
inline
#endif 
GradCell* _GradCellAtPos(const Grad* const that, 
  const VecShort2D* const pos);

// Get the GradType of the Grad 'that'
#if BUILDMODE != 0
inline
#endif 
GradType _GradGetType(const Grad* const that);

// Get the GradHexaType of the GradHexa 'that'
#if BUILDMODE != 0
inline
#endif 
GradHexaType GradHexaGetType(const GradHexa* const that);

// Return true if the GradSquare 'that' has diagonal link
// Return false else
#if BUILDMODE != 0
inline
#endif 
bool GradSquareHasDiagonalLink(const GradSquare* const that);

// Get the number of cells (area) of the Grad 'that'
#if BUILDMODE != 0
inline
#endif 
int _GradGetArea(const Grad* const that);

// Get the look up table for distance between each pair of cell of the 
// Grad 'that'
// Return a MatFloat where first index is the 'from' cell's index
// and second index is the 'to' cell index
// Distances in the matrix are equal to the sum of the value of links 
// between cells 
// Negative distance means there is no path for the pair of cell
MatFloat* _GradGetLookupTableMinDist(const Grad* const that);

// Get the path from cell at index 'from' to cell at index 'to' in 
// the Grad 'that' using the A* algorithm and the look up table 'lookup'
// for distance estimation between cells
// Return a VecShort of position (index) ordered from 'from' to 'to'
// Return NULL if there is no path
VecShort* _GradGetPath(const Grad* const that, const int from, 
  const int to, const MatFloat* const lookup);

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
  const float distMax, const int stepMax);

// Get the number of flooded cells from 'iSource'-th source in the Grad
// 'that'
int _GradGetFloodArea(const Grad* const that, const int iSource);

// Get the dimensions of the Grad 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* _GradDim(const Grad* const that);

// Check if the position 'pos' is inside the GradSquare 'that'
#if BUILDMODE != 0
inline
#endif 
bool _GradIsPosInside(const Grad* const that, 
  const VecShort2D* const pos);

// Clone the GradSquare 'that'
// The user data are not cloned but shared between the original and 
// its clone
GradSquare* GradSquareClone(const GradSquare* const that);

// Clone the GradHexa 'that'
// The user data are not cloned but shared between the original and 
// its clone
GradHexa* GradHexaClone(const GradHexa* const that);

// Return true if the Grad 'that' is same as the Grad 'tho'
// Return false else
bool _GradIsSameAs(const Grad* const that, const Grad* const tho);

// Set the flag blocked of all cells in the Grad 'that' to false
#if BUILDMODE != 0
inline
#endif 
void _GradResetFlagBlocked(Grad* const that);

// Remove the link from cell 'fromCell' to cell 'toCell' in the
// Grad 'that'
// If 'symmetric' equals true the symetric link is removed too
// (only if the link from 'fromCell' exists)
void _GradRemoveLinkIndex(Grad* const that, const int fromCell, 
  const int toCell, const bool symmetric);

// Remove the link from cell at position 'fromCell' to cell at 
// position 'toCell' in the Grad 'that'
// If 'symmetric' equals true the symetric link is removed too
// (only if the link from 'fromCell' exists)
#if BUILDMODE != 0
inline
#endif 
void _GradRemoveLinkPos(Grad* const that, 
  const VecShort2D* const fromCell, const VecShort2D* const toCell, 
  const bool symmetric);

// Remove the link from cell at position 'fromCell' toward direction 
// 'dir' in the Grad 'that'
// If 'symmetric' equals true the symetric link is removed too
// (only if the link from 'fromCell' exists)
void _GradRemoveDirIndex(Grad* const that, const int fromCell, 
  const int dir, const bool symmetric);

// Remove the link from cell at position 'fromCell' toward direction 
// 'dir' in the Grad 'that'
// If 'symmetric' equals true the symetric link is removed too
// (only if the link from 'fromCell' exists)
#if BUILDMODE != 0
inline
#endif 
void _GradRemoveDirPos(Grad* const that, 
  const VecShort2D* const fromCell, const int dir, const bool symmetric);

// Remove all the links from cell 'fromCell' in the Grad 'that'
// If 'symmetric' equals true the symetric links are removed too
// (only if the link from 'fromCell' exists)
void _GradRemoveAllLinkIndex(Grad* const that, const int fromCell, 
  const bool symmetric);

// Remove all the links from cell at position 'fromCell' in 
// the Grad 'that'
// If 'symmetric' equals true the symetric links are removed too
// (only if the link from 'fromCell' exists)
#if BUILDMODE != 0
inline
#endif 
void _GradRemoveAllLinkPos(Grad* const that, 
  const VecShort2D* const fromCell, const bool symmetric);

// Add the link from cell 'fromCell' to cell 'toCell' in the
// Grad 'that'
// If the cells are not neighbours do nothing
// If 'symmetric' equals true the symetric link is added too
void _GradAddLinkIndex(Grad* const that, const int fromCell, 
  const int toCell, const bool symmetric);

// Add the link from cell at position 'fromCell' to cell at 
// position 'toCell' in the Grad 'that'
// If the cells are not neighbours do nothing
// If 'symmetric' equals true the symetric link is added too
#if BUILDMODE != 0
inline
#endif 
void _GradAddLinkPos(Grad* const that, const VecShort2D* const fromCell, 
  const VecShort2D* const toCell, const bool symmetric);

// Add the link from cell at position 'fromCell' toward direction 
// 'dir' in the Grad 'that'
// If the cells are not neighbours do nothing
// If 'symmetric' equals true the symetric link is added too
void _GradAddDirIndex(Grad* const that, const int fromCell, const int dir,
  const bool symmetric);

// Add the link from cell at position 'fromCell' toward direction 
// 'dir' in the Grad 'that'
// If the cells are not neighbours do nothing
// If 'symmetric' equals true the symetric link is added too
#if BUILDMODE != 0
inline
#endif 
void _GradAddDirPos(Grad* const that, const VecShort2D* const fromCell, 
  const int dir, const bool symmetric);

// Add all the links from cell 'fromCell' in the Grad 'that'
// If 'symmetric' equals true the symetric links are removed too
void _GradAddAllLinkIndex(Grad* const that, const int fromCell, 
  const bool symmetric);

// Add all the links from cell at position 'fromCell' in 
// the Grad 'that'
// If 'symmetric' equals true the symetric links are removed too
#if BUILDMODE != 0
inline
#endif 
void _GradAddAllLinkPos(Grad* const that, 
  const VecShort2D* const fromCell, const bool symmetric);

// ================= Polymorphism ==================

#define GradCellAt(Grad_, Pos) _Generic(Grad_, \
  Grad*: _Generic(Pos, \
    short: _GradCellAtIndex, \
    int: _GradCellAtIndex, \
    VecShort2D*: _GradCellAtPos, \
    default: PBErrInvalidPolymorphism), \
  GradSquare*: _Generic(Pos, \
    short: _GradCellAtIndex, \
    int: _GradCellAtIndex, \
    VecShort2D*: _GradCellAtPos, \
    default: PBErrInvalidPolymorphism), \
  GradHexa*: _Generic(Pos, \
    short: _GradCellAtIndex, \
    int: _GradCellAtIndex, \
    VecShort2D*: _GradCellAtPos, \
    default: PBErrInvalidPolymorphism), \
  const Grad*: _Generic(Pos, \
    short: _GradCellAtIndex, \
    int: _GradCellAtIndex, \
    VecShort2D*: _GradCellAtPos, \
    default: PBErrInvalidPolymorphism), \
  const GradSquare*: _Generic(Pos, \
    short: _GradCellAtIndex, \
    int: _GradCellAtIndex, \
    VecShort2D*: _GradCellAtPos, \
    default: PBErrInvalidPolymorphism), \
  const GradHexa*: _Generic(Pos, \
    short: _GradCellAtIndex, \
    int: _GradCellAtIndex, \
    VecShort2D*: _GradCellAtPos, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism)((const Grad*)(Grad_), Pos)

#define GradGetArea(Grad_) _Generic(Grad_, \
  Grad*: _GradGetArea, \
  GradSquare*: _GradGetArea, \
  GradHexa*: _GradGetArea, \
  const Grad*: _GradGetArea, \
  const GradSquare*: _GradGetArea, \
  const GradHexa*: _GradGetArea, \
  default: PBErrInvalidPolymorphism)((const Grad*)(Grad_))

#define GradGetLookupTableMinDist(Grad_) _Generic(Grad_, \
  Grad*: _GradGetLookupTableMinDist, \
  GradSquare*: _GradGetLookupTableMinDist, \
  GradHexa*: _GradGetLookupTableMinDist, \
  const Grad*: _GradGetLookupTableMinDist, \
  const GradSquare*: _GradGetLookupTableMinDist, \
  const GradHexa*: _GradGetLookupTableMinDist, \
  default: PBErrInvalidPolymorphism)((const Grad*)(Grad_))

#define GradGetPath(Grad_, From, To, Dist) _Generic(Grad_, \
  Grad*: _GradGetPath, \
  GradSquare*: _GradGetPath, \
  GradHexa*: _GradGetPath, \
  const Grad*: _GradGetPath, \
  const GradSquare*: _GradGetPath, \
  const GradHexa*: _GradGetPath, \
  default: PBErrInvalidPolymorphism)((const Grad*)(Grad_), From, To, Dist)

#define GradFlood(Grad_, Sources, DistMax, StepMax) _Generic(Grad_, \
  Grad*: _GradFlood, \
  GradSquare*: _GradFlood, \
  GradHexa*: _GradFlood, \
  default: PBErrInvalidPolymorphism)((Grad*)(Grad_), Sources, \
    DistMax, StepMax)

#define GradGetFloodArea(Grad_, Source) _Generic(Grad_, \
  Grad*: _GradGetFloodArea, \
  GradSquare*: _GradGetFloodArea, \
  GradHexa*: _GradGetFloodArea, \
  const Grad*: _GradGetFloodArea, \
  const GradSquare*: _GradGetFloodArea, \
  const GradHexa*: _GradGetFloodArea, \
  default: PBErrInvalidPolymorphism)((const Grad*)(Grad_), Source)

#define GradDim(Grad_) _Generic(Grad_, \
  Grad*: _GradDim, \
  GradSquare*: _GradDim, \
  GradHexa*: _GradDim, \
  const Grad*: _GradDim, \
  const GradSquare*: _GradDim, \
  const GradHexa*: _GradDim, \
  default: PBErrInvalidPolymorphism)((const Grad*)(Grad_))

#define GradIsPosInside(Grad_, Pos) _Generic(Grad_, \
  Grad*: _GradIsPosInside, \
  GradSquare*: _GradIsPosInside, \
  GradHexa*: _GradIsPosInside, \
  const Grad*: _GradIsPosInside, \
  const GradSquare*: _GradIsPosInside, \
  const GradHexa*: _GradIsPosInside, \
  default: PBErrInvalidPolymorphism)((const Grad*)(Grad_), Pos)

#define GradClone(Grad_) _Generic(Grad_, \
  GradSquare*: GradSquareClone, \
  GradHexa*: GradHexaClone, \
  const GradSquare*: GradSquareClone, \
  const GradHexa*: GradHexaClone, \
  default: PBErrInvalidPolymorphism)(Grad_)

#define GradIsSameAs(GradA, GradB) _Generic(GradA, \
  GradSquare*: _Generic(GradB, \
    GradSquare*: _GradIsSameAs, \
    GradHexa*: _GradIsSameAs, \
    const GradSquare*: _GradIsSameAs, \
    const GradHexa*: _GradIsSameAs, \
    default: PBErrInvalidPolymorphism), \
  GradHexa*: _Generic(GradB, \
    GradSquare*: _GradIsSameAs, \
    GradHexa*: _GradIsSameAs, \
    const GradSquare*: _GradIsSameAs, \
    const GradHexa*: _GradIsSameAs, \
    default: PBErrInvalidPolymorphism), \
  const GradSquare*: _Generic(GradB, \
    GradSquare*: _GradIsSameAs, \
    GradHexa*: _GradIsSameAs, \
    const GradSquare*: _GradIsSameAs, \
    const GradHexa*: _GradIsSameAs, \
    default: PBErrInvalidPolymorphism), \
  const GradHexa*: _Generic(GradB, \
    GradSquare*: _GradIsSameAs, \
    GradHexa*: _GradIsSameAs, \
    const GradSquare*: _GradIsSameAs, \
    const GradHexa*: _GradIsSameAs, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism)((const Grad*)(GradA), \
    (const Grad*)(GradB))

#define GradResetFlagBlocked(Grad_) _Generic(Grad_, \
  Grad*: _GradResetFlagBlocked, \
  GradSquare*: _GradResetFlagBlocked, \
  GradHexa*: _GradResetFlagBlocked, \
  default: PBErrInvalidPolymorphism)((Grad*)(Grad_))

#define GradFree(Grad_) _Generic(Grad_, \
  GradSquare**: GradSquareFree, \
  GradHexa**: GradHexaFree, \
  default: PBErrInvalidPolymorphism)(Grad_)

#define GradGetType(Grad_) _Generic(Grad_, \
  Grad*: _GradGetType, \
  GradSquare*: _GradGetType, \
  GradHexa*: _GradGetType, \
  const Grad*: _GradGetType, \
  const GradSquare*: _GradGetType, \
  const GradHexa*: _GradGetType, \
  default: PBErrInvalidPolymorphism)((const Grad*)(Grad_))

#define GradRemoveLinkTo(Grad_, From, To, Sym) _Generic(Grad_, \
  Grad*: _Generic (From, \
    int: _GradRemoveLinkIndex, \
    VecShort2D*: _GradRemoveLinkPos, \
    default: PBErrInvalidPolymorphism), \
  GradSquare*: _Generic (From, \
    int: _GradRemoveLinkIndex, \
    VecShort2D*: _GradRemoveLinkPos, \
    default: PBErrInvalidPolymorphism), \
  GradHexa*: _Generic (From, \
    int: _GradRemoveLinkIndex, \
    VecShort2D*: _GradRemoveLinkPos, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism) ((Grad*)(Grad_), From, To, Sym)

#define GradRemoveLinkToward(Grad_, From, Toward, Sym) _Generic(Grad_, \
  Grad*: _Generic (From, \
    int: _GradRemoveDirIndex, \
    VecShort2D*: _GradRemoveDirPos, \
    default: PBErrInvalidPolymorphism), \
  GradSquare*: _Generic (From, \
    int: _GradRemoveDirIndex, \
    VecShort2D*: _GradRemoveDirPos, \
    default: PBErrInvalidPolymorphism), \
  GradHexa*: _Generic (From, \
    int: _GradRemoveDirIndex, \
    VecShort2D*: _GradRemoveDirPos, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism) ((Grad*)(Grad_), From, Toward, Sym)

#define GradRemoveAllLink(Grad_, From, Sym) _Generic(Grad_, \
  Grad*: _Generic (From, \
    int: _GradRemoveAllLinkIndex, \
    VecShort2D*: _GradRemoveAllLinkPos, \
    default: PBErrInvalidPolymorphism), \
  GradSquare*: _Generic (From, \
    int: _GradRemoveAllLinkIndex, \
    VecShort2D*: _GradRemoveAllLinkPos, \
    default: PBErrInvalidPolymorphism), \
  GradHexa*: _Generic (From, \
    int: _GradRemoveAllLinkIndex, \
    VecShort2D*: _GradRemoveAllLinkPos, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism) ((Grad*)(Grad_), From, Sym)

#define GradAddLinkTo(Grad_, From, To, Sym) _Generic(Grad_, \
  Grad*: _Generic (From, \
    int: _GradAddLinkIndex, \
    VecShort2D*: _GradAddLinkPos, \
    default: PBErrInvalidPolymorphism), \
  GradSquare*: _Generic (From, \
    int: _GradAddLinkIndex, \
    VecShort2D*: _GradAddLinkPos, \
    default: PBErrInvalidPolymorphism), \
  GradHexa*: _Generic (From, \
    int: _GradAddLinkIndex, \
    VecShort2D*: _GradAddLinkPos, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism) ((Grad*)(Grad_), From, To, Sym)

#define GradAddLinkToward(Grad_, From, Toward, Sym) _Generic(Grad_, \
  Grad*: _Generic (From, \
    int: _GradAddDirIndex, \
    VecShort2D*: _GradAddDirPos, \
    default: PBErrInvalidPolymorphism), \
  GradSquare*: _Generic (From, \
    int: _GradAddDirIndex, \
    VecShort2D*: _GradAddDirPos, \
    default: PBErrInvalidPolymorphism), \
  GradHexa*: _Generic (From, \
    int: _GradAddDirIndex, \
    VecShort2D*: _GradAddDirPos, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism) ((Grad*)(Grad_), From, Toward, Sym)

#define GradAddAllLink(Grad_, From, Sym) _Generic(Grad_, \
  Grad*: _Generic (From, \
    int: _GradAddAllLinkIndex, \
    VecShort2D*: _GradAddAllLinkPos, \
    default: PBErrInvalidPolymorphism), \
  GradSquare*: _Generic (From, \
    int: _GradAddAllLinkIndex, \
    VecShort2D*: _GradAddAllLinkPos, \
    default: PBErrInvalidPolymorphism), \
  GradHexa*: _Generic (From, \
    int: _GradAddAllLinkIndex, \
    VecShort2D*: _GradAddAllLinkPos, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism) ((Grad*)(Grad_), From, Sym)

// ================ Inliner ====================

#if BUILDMODE != 0
#include "grad-inline.c"
#endif


#endif
