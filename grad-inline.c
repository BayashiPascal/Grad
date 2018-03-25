// ============ GRAD-INLINE.C ================

// ------------- GradCell

// ================ Functions implementation ====================

// Get the user data of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
void* GradCellData(GradCell* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return that->_data;
}

// Set the user data of the GradCell 'that' to 'data'
#if BUILDMODE != 0
inline
#endif 
void GradCellSetData(GradCell* that, void* data) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  that->_data = data;
}

// Get the position of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GradCellPos(GradCell* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return &(that->_pos);
}

// Get the index of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
int GradCellGetId(GradCell* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return that->_id;
}

// Get the index of 'iLink'-th link of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
int GradCellGetLink(GradCell* that, int iLink) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (iLink < 0 || iLink >= that->_nbLink) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'iLink' is invalid (0<=%d<%d)", 
      iLink, that->_nbLink);
    PBErrCatch(GradErr);
  }
#endif
  return that->_links[iLink];
}

// Set the index of 'iLink'-th link of the GradCell 'that' to 'iCell'
#if BUILDMODE != 0
inline
#endif 
void GradCellSetLink(GradCell* that, int iLink, int iCell) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (iLink < 0 || iLink >= that->_nbLink) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'iLink' is invalid (0<=%d<%d)", 
      iLink, that->_nbLink);
    PBErrCatch(GradErr);
  }
  if (iCell < -1) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'iCell' is invalid (-1<=%d)", iCell);
    PBErrCatch(GradErr);
  }
#endif
  that->_links[iLink] = iCell;
}

// Get the number of links of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
int GradCellGetNbLink(GradCell* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return that->_nbLink;
}

// Get the value of 'iLink'-th link of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
float GradCellLinkVal(GradCell* that, int iLink) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (iLink < 0 || iLink >= that->_nbLink) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'iLink' is invalid (0<=%d<%d)", 
      iLink, that->_nbLink);
    PBErrCatch(GradErr);
  }
#endif
  return that->_linksVal[iLink];
}

// Set the value of 'iLink'-th link of the GradCell 'that' to 'val'
#if BUILDMODE != 0
inline
#endif 
void GradCellSetLinkVal(GradCell* that, int iLink, float val) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (iLink < 0 || iLink >= that->_nbLink) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'iLink' is invalid (0<=%d<%d)", 
      iLink, that->_nbLink);
    PBErrCatch(GradErr);
  }
#endif
  that->_linksVal[iLink] = val;
}

// Get the flood value of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
int GradCellGetFlood(GradCell* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return that->_flood;
}

// Set the flood value of the GradCell 'that' to 'iSource'
#if BUILDMODE != 0
inline
#endif 
void GradCellSetFlood(GradCell* that, int iSource) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  that->_flood = iSource;
}

// Get the flag blocked of the GradCell 'that'
#if BUILDMODE != 0
inline
#endif 
bool GradCellIsBlocked(GradCell* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return that->_flagBlocked;
}

// Set the flag blocked of the GradCell 'that' to 'flag'
#if BUILDMODE != 0
inline
#endif 
void GradCellSetBlocked(GradCell* that, bool flag) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  that->_flagBlocked = flag;
}

// ------------- Grad

// ================ Functions implementation ====================

// Get the GradCell at index 'iCell' in the Grad 'that'
#if BUILDMODE != 0
inline
#endif 
GradCell* _GradCellAtIndex(Grad* that, int iCell) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (iCell < 0 || iCell >= GradGetArea(that)) {
    GradErr->_type = PBErrTypeInvalidArg;
    sprintf(GradErr->_msg, "'iCell' is invalid (0<=%d<%d)", 
      iCell, GradGetArea(that));
    PBErrCatch(GradErr);
  }
#endif
  return that->_cells + iCell;
}

// Get the GradCell at position 'pos' int the Grad 'that'
#if BUILDMODE != 0
inline
#endif 
GradCell* _GradCellAtPos(Grad* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
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
  return that->_cells + GradPosToIndex(that, pos);
}

// Get the GradType of the Grad 'that'
#if BUILDMODE != 0
inline
#endif 
GradType _GradGetType(Grad* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return that->_type;
}

// Get the GradHexaType of the GradHexa 'that'
#if BUILDMODE != 0
inline
#endif 
GradHexaType GradHexaGetType(GradHexa* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return that->_type;
}

// Get the number of cells (area) of the Grad 'that'
#if BUILDMODE != 0
inline
#endif 
int _GradGetArea(Grad* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return VecGet(GradDim(that), 0) * VecGet(GradDim(that), 1);
}

// Get the dimensions of the Grad 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* _GradDim(Grad* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return &(that->_dim);
}

// Check if the position 'pos' is inside the GradSquare 'that'
#if BUILDMODE != 0
inline
#endif 
bool _GradIsPosInside(Grad* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  if (VecGet(pos, 0) < 0 || VecGet(pos, 1) < 0 ||
    VecGet(pos, 0) >= VecGet(GradDim(that), 0) || 
    VecGet(pos, 1) >= VecGet(GradDim(that), 1)) {
    return false;
  } else {
    return true;
  }
}

// Set the flag blocked of all cells in the Grad 'that' to false
#if BUILDMODE != 0
inline
#endif 
void _GradResetFlagBlocked(Grad* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  for (int iCell = GradGetArea(that); iCell--;)
    GradCellSetBlocked(GradCellAt(that, iCell), false);
}

// Return true if the GradSquare 'that' has diagonal link
// Return false else
#if BUILDMODE != 0
inline
#endif 
bool GradSquareHasDiagonalLink(GradSquare* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
#endif
  return that->_diagLink;
}

// Remove the link from cell at position 'fromCell' to cell at 
// position 'toCell' in the Grad 'that'
// If 'symmetric' equals true the symetric link is removed too
// (only if the link from 'fromCell' exists)
#if BUILDMODE != 0
inline
#endif 
void _GradRemoveLinkPos(Grad* that, VecShort2D* fromCell, 
  VecShort2D* toCell, bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (fromCell == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'fromCell' is null");
    PBErrCatch(GradErr);
  }
  if (toCell == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'toCell' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Get the index of 'fromCell' and 'toCell'
  int from = GradPosToIndex(that, fromCell);
  int to = GradPosToIndex(that, toCell);
  // Remove the link
  _GradRemoveLinkIndex(that, from, to, symmetric);
}

// Remove the link from cell at position 'fromCell' toward direction 
// 'dir' in the Grad 'that'
// If 'symmetric' equals true the symetric link is removed too
// (only if the link from 'fromCell' exists)
#if BUILDMODE != 0
inline
#endif 
void _GradRemoveDirPos(Grad* that, VecShort2D* fromCell, int dir,
  bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (fromCell == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'fromCell' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Get the index of 'fromCell'
  int from = GradPosToIndex(that, fromCell);
  // Remove the link
  _GradRemoveDirIndex(that, from, dir, symmetric);
}

// Remove all the links from cell at position 'fromCell' in 
// the Grad 'that'
// If 'symmetric' equals true the symetric links are removed too
// (only if the link from 'fromCell' exists)
#if BUILDMODE != 0
inline
#endif 
void _GradRemoveAllLinkPos(Grad* that, VecShort2D* fromCell, 
  bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (fromCell == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'fromCell' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Get the index of 'fromCell'
  int from = GradPosToIndex(that, fromCell);
  // Remove the link
  _GradRemoveAllLinkIndex(that, from, symmetric);
}

// Add the link from cell at position 'fromCell' to cell at 
// position 'toCell' in the Grad 'that'
// If the cells are not neighbours do nothing
// If 'symmetric' equals true the symetric link is added too
#if BUILDMODE != 0
inline
#endif 
void _GradAddLinkPos(Grad* that, VecShort2D* fromCell, 
  VecShort2D* toCell, bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (fromCell == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'fromCell' is null");
    PBErrCatch(GradErr);
  }
  if (toCell == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'toCell' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Get the index of 'fromCell' and 'toCell'
  int from = GradPosToIndex(that, fromCell);
  int to = GradPosToIndex(that, toCell);
  // Remove the link
  _GradAddLinkIndex(that, from, to, symmetric);
}

// Add the link from cell at position 'fromCell' toward direction 
// 'dir' in the Grad 'that'
// If the cells are not neighbours do nothing
// If 'symmetric' equals true the symetric link is added too
#if BUILDMODE != 0
inline
#endif 
void _GradAddDirPos(Grad* that, VecShort2D* fromCell, int dir,
  bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (fromCell == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'fromCell' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Get the index of 'fromCell' and 'toCell'
  int from = GradPosToIndex(that, fromCell);
  // Remove the link
  _GradAddDirIndex(that, from, dir, symmetric);
}

// Add all the links from cell at position 'fromCell' in 
// the Grad 'that'
// If 'symmetric' equals true the symetric links are removed too
#if BUILDMODE != 0
inline
#endif 
void _GradAddAllLinkPos(Grad* that, VecShort2D* fromCell, 
  bool symmetric) {
#if BUILDMODE == 0
  if (that == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'that' is null");
    PBErrCatch(GradErr);
  }
  if (fromCell == NULL) {
    GradErr->_type = PBErrTypeNullPointer;
    sprintf(GradErr->_msg, "'fromCell' is null");
    PBErrCatch(GradErr);
  }
#endif
  // Get the index of 'fromCell' and 'toCell'
  int from = GradPosToIndex(that, fromCell);
  // Remove the link
  _GradAddAllLinkIndex(that, from, symmetric);
}
