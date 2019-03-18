#include <omp.h>
#ifndef taskminerutils
#define taskminerutils
static int taskminer_depth_cutoff = 0;
#define DEPTH_CUTOFF omp_get_num_threads()
extern char cutoff_test;
#endif
//------------------------------------------------------------------------------------------------------------------------------
// Samuel Williams
// SWWilliams@lbl.gov
// Lawrence Berkeley National Lab
//------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

//------------------------------------------------------------------------------------------------------------------------------
#include "timer.h"
#include "defines.h"
#include "box.h"
#include "mg.h"
//------------------------------------------------------------------------------------------------------------------------------
#include "exchange_boundary.inc"
#include "lambda.inc"
#include "jacobi.inc"
//#include "operators.ompif/gsrb.inc"
//#include "operators.ompif/chebyshev.inc"
#include "apply_op.inc"
#include "residual.inc"
#include "restriction.inc"
#include "interpolation.inc"
#include "misc.inc"
#include "matmul.inc"
#include "problem1.inc"
//#include "operators.ompif/problem2.inc"
//------------------------------------------------------------------------------------------------------------------------------

int fooTM() {
  return 1;
}

