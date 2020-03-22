#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "pberr.h"
#include "grad.h"

#define RANDOMSEED 0

void UnitTestGradCellCreateFree() {
  VecShort2D pos = VecShortCreateStatic2D();
  VecSet(&pos, 0, 3.0); VecSet(&pos, 1, 4.0);
  GradCell* cell = GradCellCreate(1, 2, &pos);
  if (cell == NULL ||
    cell->_id != 1 ||
    cell->_nbLink != 2 ||
    cell->_data != NULL ||
    cell->_flood != -1 ||
    cell->_flagBlocked != false ||
    VecIsEqual(&(cell->_pos), &pos) == false) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellCreate failed");
    PBErrCatch(GradErr);
  }
  for (int iLink = GRAD_NBMAXLINK; iLink--;) {
    if (cell->_links[iLink] != -1 ||
      ISEQUALF(cell->_linksVal[iLink], 1.0) == false) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradCellCreate failed");
      PBErrCatch(GradErr);
    }
  }
  GradCellFree(&cell);
  if (cell != NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellFree failed");
    PBErrCatch(GradErr);
  }
  printf("UnitTestGradCellCreateFree OK\n");
}

void UnitTestGradCellGetSet() {
  VecShort2D pos = VecShortCreateStatic2D();
  VecSet(&pos, 0, 3.0); VecSet(&pos, 1, 4.0);
  GradCell* cell = GradCellCreate(1, 2, &pos);
  if (GradCellGetId(cell) != cell->_id) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellGetId failed");
    PBErrCatch(GradErr);
  }
  if (GradCellGetFlood(cell) != cell->_flood) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellGetFlood failed");
    PBErrCatch(GradErr);
  }
  if (GradCellGetNbLink(cell) != cell->_nbLink) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellGetFlood failed");
    PBErrCatch(GradErr);
  }
  if (GradCellIsBlocked(cell) != cell->_flagBlocked) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellIsBlocked failed");
    PBErrCatch(GradErr);
  }
  if (GradCellData(cell) != cell->_data) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellData failed");
    PBErrCatch(GradErr);
  }
  if (GradCellGetLink(cell, 1) != cell->_links[1]) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellLink failed");
    PBErrCatch(GradErr);
  }
  if (GradCellLinkVal(cell, 1) != cell->_linksVal[1]) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellLinkVal failed");
    PBErrCatch(GradErr);
  }
  int val;
  GradCellSetData(cell, &val);
  if (GradCellData(cell) != &val) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellSetData failed");
    PBErrCatch(GradErr);
  }
  GradCellSetLink(cell, 1, 2);
  if (GradCellGetLink(cell, 1) != 2) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellSetLink failed");
    PBErrCatch(GradErr);
  }
  GradCellSetLinkVal(cell, 1, 2.0);
  if (ISEQUALF(GradCellLinkVal(cell, 1), 2.0) == false) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellSetLinkVal failed");
    PBErrCatch(GradErr);
  }
  GradCellSetBlocked(cell, true);
  if (GradCellIsBlocked(cell) == false) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellSetBlocked failed");
    PBErrCatch(GradErr);
  }
  GradCellSetFlood(cell, 1);
  if (GradCellGetFlood(cell) != 1) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellSetFlood failed");
    PBErrCatch(GradErr);
  }
  GradCellFree(&cell);
  printf("UnitTestGradCellGetSet OK\n");
}

void UnitTestGradCell() {
  UnitTestGradCellCreateFree();
  UnitTestGradCellGetSet();
  printf("UnitTestGradCell OK\n");
}

void UnitTestGradCreateFree() {
  bool diagLink = true;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  GradSquare* gradSquare = GradSquareCreate(&dim, diagLink);
  if (gradSquare == NULL ||
    VecIsEqual(&(gradSquare->_grad._dim), &dim) == false ||
    gradSquare->_diagLink != diagLink ||
    gradSquare->_grad._type != GradTypeSquare ||
    gradSquare->_grad._cells == NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradSquareCreate failed");
    PBErrCatch(GradErr);
  }
  int iCell = 0;
  VecShort2D pos = VecShortCreateStatic2D();
  do {
    if (VecIsEqual(GradCellPos(gradSquare->_grad._cells + iCell), 
      &pos) == false) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradSquareCreate failed");
      PBErrCatch(GradErr);
    }
    ++iCell;
  } while (VecPStep(&pos, &dim));
  int checkA[48] = {
    -1, 1, 2, -1, -1, -1, 3, -1, 
    -1, -1, 3, 0, -1, -1, -1, 2, 
    0, 3, 4, -1, -1, 1, 5, -1, 
    1, -1, 5, 2, 0, -1, -1, 4, 
    2, 5, -1, -1, -1, 3, -1, -1, 
    3, -1, -1, 4, 2, -1, -1, -1
    };
  int iCheck = 0;
  for (int iCell = 0; iCell < 6; ++iCell) {
    if (gradSquare->_grad._cells[iCell]._nbLink != 8) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradSquareCreate failed");
      PBErrCatch(GradErr);
    }
    for (int iLink = 0; iLink < 8; ++iLink) {
      if (gradSquare->_grad._cells[iCell]._links[iLink] != 
        checkA[iCheck] ||
        (iLink < 4 && 
        ISEQUALF(gradSquare->_grad._cells[iCell]._linksVal[iLink],
        1.0) == false) ||
        (iLink >= 4 && 
        ISEQUALF(gradSquare->_grad._cells[iCell]._linksVal[iLink],
        PBMATH_SQRTTWO) == false)) {
        GradErr->_type = PBErrTypeUnitTestFailed;
        sprintf(GradErr->_msg, "GradSquareCreate failed");
        PBErrCatch(GradErr);
      }
      ++iCheck;
    }
  }
  GradSquareFree(&gradSquare);
  diagLink = false;
  gradSquare = GradSquareCreate(&dim, diagLink);
  if (gradSquare == NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradSquareCreate failed");
    PBErrCatch(GradErr);
  }
  int checkB[24] = {
    -1, 1, 2, -1,
    -1, -1, 3, 0,
    0, 3, 4, -1,
    1, -1, 5, 2,
    2, 5, -1, -1,
    3, -1, -1, 4
    };
  iCheck = 0;
  for (int iCell = 0; iCell < 6; ++iCell) {
    if (gradSquare->_grad._cells[iCell]._nbLink != 4) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradSquareCreate failed");
      PBErrCatch(GradErr);
    }
    for (int iLink = 0; iLink < 4; ++iLink) {
      if (gradSquare->_grad._cells[iCell]._links[iLink] != 
        checkB[iCheck] ||
        ISEQUALF(gradSquare->_grad._cells[iCell]._linksVal[iLink],
        1.0) == false) {
        GradErr->_type = PBErrTypeUnitTestFailed;
        sprintf(GradErr->_msg, "GradSquareCreate failed");
        PBErrCatch(GradErr);
      }
      ++iCheck;
    }
  }
  GradSquareFree(&gradSquare);
  if (gradSquare != NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradSquareFree failed");
    PBErrCatch(GradErr);
  }
  GradHexa* gradHexa = GradHexaCreateOddR(&dim);
  if (gradHexa == NULL ||
    VecIsEqual(&(gradHexa->_grad._dim), &dim) == false ||
    gradHexa->_grad._type != GradTypeHexa ||
    gradHexa->_type != GradHexaTypeOddR ||
    gradHexa->_grad._cells == NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradHexaCreateOddR failed");
    PBErrCatch(GradErr);
  }
  int checkC[36] = {
    -1, -1, 1, 2, -1, -1, 
    -1, -1, -1, 3, 2, 0, 
    0, 1, 3, 5, 4, -1, 
    1, -1, -1, -1, 5, 2, 
    -1, 2, 5, -1, -1, -1, 
    2, 3, -1, -1, -1, 4
    };
  iCheck = 0;
  for (int iCell = 0; iCell < 6; ++iCell) {
    if (gradHexa->_grad._cells[iCell]._nbLink != 6) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradHexaCreateOddRfailed");
      PBErrCatch(GradErr);
    }
    for (int iLink = 0; iLink < 6; ++iLink) {
      if (gradHexa->_grad._cells[iCell]._links[iLink] != 
        checkC[iCheck] ||
        ISEQUALF(gradHexa->_grad._cells[iCell]._linksVal[iLink],
        1.0) == false) {
        GradErr->_type = PBErrTypeUnitTestFailed;
        sprintf(GradErr->_msg, "GradHexaCreateOddR failed");
        PBErrCatch(GradErr);
      }
      ++iCheck;
    }
  }
  GradHexaFree(&gradHexa);
  gradHexa = GradHexaCreateEvenR(&dim);
  if (gradHexa == NULL ||
    VecIsEqual(&(gradHexa->_grad._dim), &dim) == false ||
    gradHexa->_grad._type != GradTypeHexa ||
    gradHexa->_type != GradHexaTypeEvenR ||
    gradHexa->_grad._cells == NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradHexaCreateEvenR failed");
    PBErrCatch(GradErr);
  }
  int checkD[36] = {
    -1, -1, 1, 3, 2, -1, 
    -1, -1, -1, -1, 3, 0, 
    -1, 0, 3, 4, -1, -1, 
    0, 1, -1, 5, 4, 2, 
    2, 3, 5, -1, -1, -1, 
    3, -1, -1, -1, -1, 4
    };
  iCheck = 0;
  for (int iCell = 0; iCell < 6; ++iCell) {
    if (gradHexa->_grad._cells[iCell]._nbLink != 6) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradHexaCreateEvenRfailed");
      PBErrCatch(GradErr);
    }
    for (int iLink = 0; iLink < 6; ++iLink) {
      if (gradHexa->_grad._cells[iCell]._links[iLink] != 
        checkD[iCheck] ||
        ISEQUALF(gradHexa->_grad._cells[iCell]._linksVal[iLink],
        1.0) == false) {
        GradErr->_type = PBErrTypeUnitTestFailed;
        sprintf(GradErr->_msg, "GradHexaCreateEvenR failed");
        PBErrCatch(GradErr);
      }
      ++iCheck;
    }
  }
  GradHexaFree(&gradHexa);
  gradHexa = GradHexaCreateOddQ(&dim);
  if (gradHexa == NULL ||
    VecIsEqual(&(gradHexa->_grad._dim), &dim) == false ||
    gradHexa->_grad._type != GradTypeHexa ||
    gradHexa->_type != GradHexaTypeOddQ ||
    gradHexa->_grad._cells == NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradHexaCreateOddQ failed");
    PBErrCatch(GradErr);
  }
  int checkE[36] = {
    -1, -1, 1, 2, -1, -1, 
    -1, -1, -1, 3, 2, 0, 
    0, 1, 3, 4, -1, -1, 
    1, -1, -1, 5, 4, 2, 
    2, 3, 5, -1, -1, -1, 
    3, -1, -1, -1, -1, 4
    };
  iCheck = 0;
  for (int iCell = 0; iCell < 6; ++iCell) {
    if (gradHexa->_grad._cells[iCell]._nbLink != 6) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradHexaCreateOddQfailed");
      PBErrCatch(GradErr);
    }
    for (int iLink = 0; iLink < 6; ++iLink) {
      if (gradHexa->_grad._cells[iCell]._links[iLink] != 
        checkE[iCheck] ||
        ISEQUALF(gradHexa->_grad._cells[iCell]._linksVal[iLink],
        1.0) == false) {
        GradErr->_type = PBErrTypeUnitTestFailed;
        sprintf(GradErr->_msg, "GradHexaCreateOddQ failed");
        PBErrCatch(GradErr);
      }
      ++iCheck;
    }
  }
  GradHexaFree(&gradHexa);
  gradHexa = GradHexaCreateEvenQ(&dim);
  if (gradHexa == NULL ||
    VecIsEqual(&(gradHexa->_grad._dim), &dim) == false ||
    gradHexa->_grad._type != GradTypeHexa ||
    gradHexa->_type != GradHexaTypeEvenQ ||
    gradHexa->_grad._cells == NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradHexaCreateEvenQ failed");
    PBErrCatch(GradErr);
  }
  int checkF[36] = {
    -1, 1, 3, 2, -1, -1, 
    -1, -1, -1, 3, 0, -1, 
    0, 3, 5, 4, -1, -1, 
    1, -1, -1, 5, 2, 0, 
    2, 5, -1, -1, -1, -1, 
    3, -1, -1, -1, 4, 2
    };
  iCheck = 0;
  for (int iCell = 0; iCell < 6; ++iCell) {
    if (gradHexa->_grad._cells[iCell]._nbLink != 6) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradHexaCreateEvenQfailed");
      PBErrCatch(GradErr);
    }
    for (int iLink = 0; iLink < 6; ++iLink) {
      if (gradHexa->_grad._cells[iCell]._links[iLink] != 
        checkF[iCheck] ||
        ISEQUALF(gradHexa->_grad._cells[iCell]._linksVal[iLink],
        1.0) == false) {
        GradErr->_type = PBErrTypeUnitTestFailed;
        sprintf(GradErr->_msg, "GradHexaCreateEvenQ failed");
        PBErrCatch(GradErr);
      }
      ++iCheck;
    }
  }
  GradHexaFree(&gradHexa);
  if (gradSquare != NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradHexaFree failed");
    PBErrCatch(GradErr);
  }
  printf("UnitTestGradCreateFree OK\n");
}

void UnitTestGradCloneIsSame() {
  bool diagLink = true;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  GradSquare* gradSquare = GradSquareCreate(&dim, diagLink);
  GradSquare* cloneSquare = GradSquareClone(gradSquare);
  if (cloneSquare == NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradSquareClone failed");
    PBErrCatch(GradErr);
  }
  if (GradIsSameAs(gradSquare, cloneSquare) == false) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradIsSameAs failed");
    PBErrCatch(GradErr);
  }
  GradFree(&gradSquare);
  GradFree(&cloneSquare);
  GradHexa* gradHexa = GradHexaCreateOddQ(&dim);
  GradHexa* cloneHexa = GradHexaClone(gradHexa);
  if (cloneHexa == NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradHexaClone failed");
    PBErrCatch(GradErr);
  }
  if (GradIsSameAs(gradHexa, cloneHexa) == false) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradIsSameAs failed");
    PBErrCatch(GradErr);
  }
  GradFree(&gradHexa);
  GradFree(&cloneHexa);
  printf("UnitTestGradCloneIsSame OK\n");
}

void UnitTestGradGetSet() {
  bool diagLink = true;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  GradSquare* gradSquare = GradSquareCreate(&dim, diagLink);
  GradHexa* gradHexa = GradHexaCreateOddQ(&dim);
  if (GradGetArea(gradSquare) != 6) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradGetArea failed");
    PBErrCatch(GradErr);
  }
  if (GradCellAt(gradSquare, 1L) != gradSquare->_grad._cells + 1) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradGetCell failed");
    PBErrCatch(GradErr);
  }
  VecShort2D pos = VecShortCreateStatic2D();
  VecSet(&pos, 0, 1); VecSet(&pos, 1, 2);
  if (GradCellAt(gradSquare, &pos) != gradSquare->_grad._cells + 5) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradGetCell failed");
    PBErrCatch(GradErr);
  }
  if (GradGetType(gradSquare) != GradTypeSquare) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradGetType failed");
    PBErrCatch(GradErr);
  }
  if (GradHexaGetType(gradHexa) != GradHexaTypeOddQ) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradHexaGetType failed");
    PBErrCatch(GradErr);
  }
  if (GradSquareHasDiagonalLink(gradSquare) != diagLink) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradSquareHasDiagonalLink failed");
    PBErrCatch(GradErr);
  }
  if (GradCellNeighbour(gradSquare, GradCellAt(gradSquare, 0L), GradSquareDirE) != GradCellAt(gradSquare, 1L)) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradCellNeighbour failed");
    PBErrCatch(GradErr);
  }
  GradFree(&gradSquare);
  GradFree(&gradHexa);
  printf("UnitTestGradGetSet OK\n");
}

void UnitTestGradResetFlagBlocked() {
  bool diagLink = true;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  GradSquare* grad = GradSquareCreate(&dim, diagLink);
  for (long iCell = GradGetArea(grad); iCell--;)
    GradCellSetBlocked(GradCellAt(grad, iCell), true);
  GradResetFlagBlocked(grad);
  for (long iCell = GradGetArea(grad); iCell--;)
    if (GradCellIsBlocked(GradCellAt(grad, iCell)) != false) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradResetFlagBlocked failed");
      PBErrCatch(GradErr);
    }
  GradFree(&grad);
  printf("UnitTestGradResetFlagBlocked OK\n");
}

void UnitTestGradEditLinks() {
  bool diagLink = true;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  GradSquare* grad = GradSquareCreate(&dim, diagLink);
  GradRemoveLinkTo(grad, 0L, 1L, false);
  if (GradCellGetLink(GradCellAt(grad, 0L), GradSquareDirE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 1L), GradSquareDirW) != 0) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradRemoveLinkTo failed");
    PBErrCatch(GradErr);
  }
  GradAddLinkTo(grad, 0L, 1L, false);
  if (GradCellGetLink(GradCellAt(grad, 0L), GradSquareDirE) != 1 ||
    GradCellGetLink(GradCellAt(grad, 1L), GradSquareDirW) != 0) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradAddLinkTo failed");
    PBErrCatch(GradErr);
  }
  GradRemoveLinkTo(grad, 2L, 3L, true);
  if (GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 3L), GradSquareDirW) != -1) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradRemoveLinkTo failed");
    PBErrCatch(GradErr);
  }
  GradAddLinkTo(grad, 2L, 3L, true);
  if (GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirE) != 3 ||
    GradCellGetLink(GradCellAt(grad, 3L), GradSquareDirW) != 2) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradAddLinkTo failed");
    PBErrCatch(GradErr);
  }
  VecShort2D from = VecShortCreateStatic2D();
  VecSet(&from, 0, 0); VecSet(&from, 1, 2);
  VecShort2D to = VecShortCreateStatic2D();
  VecSet(&to, 0, 1); VecSet(&to, 1, 2);
  GradRemoveLinkTo(grad, &from, &to, true);
  if (GradCellGetLink(GradCellAt(grad, 4L), GradSquareDirE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 5L), GradSquareDirW) != -1) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradRemoveLinkTo failed");
    PBErrCatch(GradErr);
  }
  GradAddLinkTo(grad, &from, &to, true);
  if (GradCellGetLink(GradCellAt(grad, 4L), GradSquareDirE) != 5 ||
    GradCellGetLink(GradCellAt(grad, 5L), GradSquareDirW) != 4) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradAddLinkTo failed");
    PBErrCatch(GradErr);
  }
  GradRemoveLinkToward(grad, 0L, GradSquareDirE, false);
  if (GradCellGetLink(GradCellAt(grad, 0L), GradSquareDirE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 1L), GradSquareDirW) != 0) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradRemoveLinkToward failed");
    PBErrCatch(GradErr);
  }
  GradAddLinkToward(grad, 0L, GradSquareDirE, false);
  if (GradCellGetLink(GradCellAt(grad, 0L), GradSquareDirE) != 1 ||
    GradCellGetLink(GradCellAt(grad, 1L), GradSquareDirW) != 0) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradAddLinkToward failed");
    PBErrCatch(GradErr);
  }
  GradRemoveLinkToward(grad, 2L, GradSquareDirE, true);
  if (GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 3L), GradSquareDirW) != -1) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradRemoveLinkToward failed");
    PBErrCatch(GradErr);
  }
  GradAddLinkToward(grad, 2L, GradSquareDirE, true);
  if (GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirE) != 3 ||
    GradCellGetLink(GradCellAt(grad, 3L), GradSquareDirW) != 2) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradAddLinkToward failed");
    PBErrCatch(GradErr);
  }
  GradRemoveLinkToward(grad, &from, GradSquareDirE, true);
  if (GradCellGetLink(GradCellAt(grad, 4L), GradSquareDirE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 5L), GradSquareDirW) != -1) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradRemoveLinkToward failed");
    PBErrCatch(GradErr);
  }
  GradAddLinkToward(grad, &from, GradSquareDirE, true);
  if (GradCellGetLink(GradCellAt(grad, 4L), GradSquareDirE) != 5 ||
    GradCellGetLink(GradCellAt(grad, 5L), GradSquareDirW) != 4) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradAddLinkToward failed");
    PBErrCatch(GradErr);
  }
  GradRemoveAllLink(grad, 2L, false);
  if (GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirN) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirNE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirSE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirS) != -1 ||
    GradCellGetLink(GradCellAt(grad, 0L), GradSquareDirS) != 2 ||
    GradCellGetLink(GradCellAt(grad, 1L), GradSquareDirSW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 3L), GradSquareDirW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 5L), GradSquareDirNW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 4L), GradSquareDirN) != 2) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradRemoveAllLink failed");
    PBErrCatch(GradErr);
  }
  GradAddAllLink(grad, 2L, false);
  if (GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirN) != 0 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirNE) != 1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirE) != 3 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirSE) != 5 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirS) != 4 ||
    GradCellGetLink(GradCellAt(grad, 0L), GradSquareDirS) != 2 ||
    GradCellGetLink(GradCellAt(grad, 1L), GradSquareDirSW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 3L), GradSquareDirW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 5L), GradSquareDirNW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 4L), GradSquareDirN) != 2) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradAddAllLink failed");
    PBErrCatch(GradErr);
  }
  GradRemoveAllLink(grad, 2L, true);
  if (GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirN) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirNE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirSE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirS) != -1 ||
    GradCellGetLink(GradCellAt(grad, 0L), GradSquareDirS) != -1 ||
    GradCellGetLink(GradCellAt(grad, 1L), GradSquareDirSW) != -1 ||
    GradCellGetLink(GradCellAt(grad, 3L), GradSquareDirW) != -1 ||
    GradCellGetLink(GradCellAt(grad, 5L), GradSquareDirNW) != -1 ||
    GradCellGetLink(GradCellAt(grad, 4L), GradSquareDirN) != -1) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradRemoveAllLink failed");
    PBErrCatch(GradErr);
  }
  GradAddAllLink(grad, 2L, true);
  if (GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirN) != 0 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirNE) != 1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirE) != 3 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirSE) != 5 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirS) != 4 ||
    GradCellGetLink(GradCellAt(grad, 0L), GradSquareDirS) != 2 ||
    GradCellGetLink(GradCellAt(grad, 1L), GradSquareDirSW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 3L), GradSquareDirW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 5L), GradSquareDirNW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 4L), GradSquareDirN) != 2) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradAddAllLink failed");
    PBErrCatch(GradErr);
  }
  VecSet(&from, 0, 0); VecSet(&from, 1, 1);
  GradRemoveAllLink(grad, &from, false);
  if (GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirN) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirNE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirSE) != -1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirS) != -1 ||
    GradCellGetLink(GradCellAt(grad, 0L), GradSquareDirS) != 2 ||
    GradCellGetLink(GradCellAt(grad, 1L), GradSquareDirSW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 3L), GradSquareDirW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 5L), GradSquareDirNW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 4L), GradSquareDirN) != 2) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradRemoveAllLink failed");
    PBErrCatch(GradErr);
  }
  GradAddAllLink(grad, &from, false);
  if (GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirN) != 0 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirNE) != 1 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirE) != 3 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirSE) != 5 ||
    GradCellGetLink(GradCellAt(grad, 2L), GradSquareDirS) != 4 ||
    GradCellGetLink(GradCellAt(grad, 0L), GradSquareDirS) != 2 ||
    GradCellGetLink(GradCellAt(grad, 1L), GradSquareDirSW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 3L), GradSquareDirW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 5L), GradSquareDirNW) != 2 ||
    GradCellGetLink(GradCellAt(grad, 4L), GradSquareDirN) != 2) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradAddAllLink failed");
    PBErrCatch(GradErr);
  }
  GradFree(&grad);
  printf("UnitTestGradEditLinks OK\n");
}

void UnitTestGradLookupTable() {
  bool diagLink = true;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  GradSquare* grad = GradSquareCreate(&dim, diagLink);
  GradCellSetBlocked(GradCellAt(grad, 2L), true);
  MatFloat* table = GradGetLookupTableMinDist(grad);
  if (table == NULL) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradGetLookupTableMinDist failed");
    PBErrCatch(GradErr);
  }
  float check[36] = {
    -1.000000, 1.000000, -1.000000, 1.414214, 2.828427, 2.414214, 1.000000, -1.000000, -1.000000, 1.000000, 2.414214, 2.000000, -1.000000, -1.000000, -1.000000, -1.000000, -1.000000, -1.000000, 1.414214, 1.000000, -1.000000, -1.000000, 1.414214, 1.000000, 2.828427, 2.414214, -1.000000, 1.414214, -1.000000, 1.000000, 2.414214, 2.000000, -1.000000, 1.000000, 1.000000, -1.000000
    };
  for (int i = 0; i < 36; ++i) {
    if (ISEQUALF(table->_val[i], check[i]) == false) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradGetLookupTableMinDist failed");
      PBErrCatch(GradErr);
    }
  }
  GradFree(&grad);
  MatFree(&table);
  printf("UnitTestGradLookupTable OK\n");
}

void UnitTestGradFlood() {
  bool diagLink = true;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 10); VecSet(&dim, 1, 10);
  GradSquare* grad = GradSquareCreate(&dim, diagLink);
  VecShort2D sources = VecShortCreateStatic2D();
  VecSet(&sources, 0, 12); VecSet(&sources, 1, 98);
  GradCellSetBlocked(GradCellAt(grad, 92L), true);
  GradCellSetBlocked(GradCellAt(grad, 32L), true);
  GradRemoveAllLink(grad, 8L, true);
  GradRemoveAllLink(grad, 18L, true);
  GradRemoveAllLink(grad, 19L, true);
  float distMax = 20.0;
  int stepMax = 20;
  GradFlood(grad, (VecShort*)&sources, distMax, stepMax);
  VecShort2D pos = VecShortCreateStatic2D();
  int check[100] = {
    0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 
    0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 
    0, 0, -1, 0, 0, 0, 0, 0, 1, 1, 
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 
    0, 0, -1, 1, 1, 1, 1, 1, 1, 1, 
    0, -1, 1, 1, 1, 1, 1, 1, 1, 1, 
    -1, 1, -1, 1, 1, 1, 1, 1, 1, 1
    };
  int iCheck = 0;
  do {
    GradCell* cell = GradCellAt(grad, &pos);
    printf("%2d, ", GradCellGetFlood(cell));
    if (VecGet(&pos, 0) == 9) printf("\n");
    if (GradCellGetFlood(cell) != check[iCheck]) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradFlood failed");
      //PBErrCatch(GradErr);
    }
    ++iCheck;
  } while (VecPStep(&pos, &dim));
  int floodArea = GradGetFloodArea(grad, 0);
  if (floodArea != 52) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradFloodArea failed");
    PBErrCatch(GradErr);
  }
  floodArea = GradGetFloodArea(grad, 1);
  if (floodArea != 38) {
    GradErr->_type = PBErrTypeUnitTestFailed;
    sprintf(GradErr->_msg, "GradFloodArea failed");
    PBErrCatch(GradErr);
  }

  GradFree(&grad);
  printf("UnitTestGradFlood OK\n");
}

void UnitTestGradGetPath() {
  bool diagLink = true;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 10); VecSet(&dim, 1, 10);
  GradSquare* grad = GradSquareCreate(&dim, diagLink);
  GradRemoveAllLink(grad, 51L, true);
  GradRemoveAllLink(grad, 52L, true);
  GradRemoveAllLink(grad, 53L, true);
  GradRemoveAllLink(grad, 54L, true);
  GradCellSetBlocked(GradCellAt(grad, 55L), true);
  GradCellSetBlocked(GradCellAt(grad, 56L), true);
  GradRemoveAllLink(grad, 58L, true);
  GradRemoveAllLink(grad, 59L, true);
  MatFloat* lookUp = GradGetLookupTableMinDist(grad);
  long from = 12;
  long to = 85;
  VecShort* path = GradGetPath(grad, from, to, lookUp);
  VecPrint(path, stdout); printf("\n");
  int check[9] = {12,23,24,35,46,57,66,75,85};
  for (int i = 9; i--;) {
    if (VecGet(path, i) != check[i]) {
      GradErr->_type = PBErrTypeUnitTestFailed;
      sprintf(GradErr->_msg, "GradGetPath failed");
      PBErrCatch(GradErr);
    }
  }
  VecFree(&path);
  MatFree(&lookUp);
  GradFree(&grad);
  printf("UnitTestGradGetPath OK\n");
}

void UnitTestGrad() {
  UnitTestGradCreateFree();
  UnitTestGradCloneIsSame();
  UnitTestGradGetSet();
  UnitTestGradResetFlagBlocked();
  UnitTestGradEditLinks();
  UnitTestGradLookupTable();
  UnitTestGradFlood();
  UnitTestGradGetPath();

  printf("UnitTestGrad OK\n");
}

void UnitTestAll() {
  UnitTestGradCell();
  UnitTestGrad();
  printf("UnitTestAll OK\n");
}

int main() {
  UnitTestAll();
  // Return success code
  return 0;
}

