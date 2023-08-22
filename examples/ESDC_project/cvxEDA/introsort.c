/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * introsort.c
 *
 * Code generation for function 'introsort'
 *
 */

/* Include files */
#include "introsort.h"
#include "cvxEDA_data.h"
#include "cvxEDA_types.h"
#include "heapsort.h"
#include "insertionsort.h"
#include "rt_nonfinite.h"

/* Type Definitions */
#ifndef typedef_struct_T
#define typedef_struct_T

typedef struct {
  int xstart;
  int xend;
  int depth;
} struct_T;

#endif                                 /*typedef_struct_T*/

/* Function Definitions */
void b_introsort(emxArray_int32_T *x, int xend, const cell_wrap_3
                 cmp_tunableEnvironment[2])
{
  struct_T st_d_data[120];
  struct_T frame;
  int MAXDEPTH;
  int exitg2;
  int exitg3;
  int i;
  int j;
  int pivot;
  int pmax;
  int pmin;
  int pow2p;
  int st_n;
  boolean_T exitg1;
  boolean_T varargout_1;
  if (1 < xend) {
    if (xend <= 32) {
      b_insertionsort(x, 1, xend, cmp_tunableEnvironment);
    } else {
      pmax = 31;
      pmin = 0;
      exitg1 = false;
      while ((!exitg1) && (pmax - pmin > 1)) {
        j = (pmin + pmax) >> 1;
        pow2p = 1 << j;
        if (pow2p == xend) {
          pmax = j;
          exitg1 = true;
        } else if (pow2p > xend) {
          pmax = j;
        } else {
          pmin = j;
        }
      }

      MAXDEPTH = (pmax - 1) << 1;
      frame.xstart = 1;
      frame.xend = xend;
      frame.depth = 0;
      pmax = MAXDEPTH << 1;
      for (i = 0; i < pmax; i++) {
        st_d_data[i] = frame;
      }

      st_d_data[0] = frame;
      st_n = 1;
      while (st_n > 0) {
        frame = st_d_data[st_n - 1];
        st_n--;
        i = frame.xend - frame.xstart;
        if (i + 1 <= 32) {
          b_insertionsort(x, frame.xstart, frame.xend, cmp_tunableEnvironment);
        } else if (frame.depth == MAXDEPTH) {
          c_heapsort(x, frame.xstart, frame.xend, cmp_tunableEnvironment);
        } else {
          pow2p = (frame.xstart + i / 2) - 1;
          i = x->data[frame.xstart - 1];
          pmax = cmp_tunableEnvironment[0].f1->data[x->data[pow2p] - 1];
          pmin = cmp_tunableEnvironment[0].f1->data[i - 1];
          if (pmax < pmin) {
            varargout_1 = true;
          } else if (pmax == pmin) {
            varargout_1 = (cmp_tunableEnvironment[1].f1->data[x->data[pow2p] - 1]
                           < cmp_tunableEnvironment[1].f1->data[i - 1]);
          } else {
            varargout_1 = false;
          }

          if (varargout_1) {
            x->data[frame.xstart - 1] = x->data[pow2p];
            x->data[pow2p] = i;
          }

          if (cmp_tunableEnvironment[0].f1->data[x->data[frame.xend - 1] - 1] <
              cmp_tunableEnvironment[0].f1->data[x->data[frame.xstart - 1] - 1])
          {
            varargout_1 = true;
          } else if (cmp_tunableEnvironment[0].f1->data[x->data[frame.xend - 1]
                     - 1] == cmp_tunableEnvironment[0].f1->data[x->
                     data[frame.xstart - 1] - 1]) {
            varargout_1 = (cmp_tunableEnvironment[1].f1->data[x->data[frame.xend
                           - 1] - 1] < cmp_tunableEnvironment[1].f1->data
                           [x->data[frame.xstart - 1] - 1]);
          } else {
            varargout_1 = false;
          }

          if (varargout_1) {
            pmax = x->data[frame.xstart - 1];
            x->data[frame.xstart - 1] = x->data[frame.xend - 1];
            x->data[frame.xend - 1] = pmax;
          }

          if (cmp_tunableEnvironment[0].f1->data[x->data[frame.xend - 1] - 1] <
              cmp_tunableEnvironment[0].f1->data[x->data[pow2p] - 1]) {
            varargout_1 = true;
          } else if (cmp_tunableEnvironment[0].f1->data[x->data[frame.xend - 1]
                     - 1] == cmp_tunableEnvironment[0].f1->data[x->data[pow2p] -
                     1]) {
            varargout_1 = (cmp_tunableEnvironment[1].f1->data[x->data[frame.xend
                           - 1] - 1] < cmp_tunableEnvironment[1].f1->data
                           [x->data[pow2p] - 1]);
          } else {
            varargout_1 = false;
          }

          if (varargout_1) {
            pmax = x->data[pow2p];
            x->data[pow2p] = x->data[frame.xend - 1];
            x->data[frame.xend - 1] = pmax;
          }

          pivot = x->data[pow2p] - 1;
          x->data[pow2p] = x->data[frame.xend - 2];
          x->data[frame.xend - 2] = pivot + 1;
          pmin = frame.xstart - 1;
          j = frame.xend - 2;
          do {
            exitg2 = 0;
            pmin++;
            do {
              exitg3 = 0;
              i = cmp_tunableEnvironment[0].f1->data[x->data[pmin] - 1];
              if (i < cmp_tunableEnvironment[0].f1->data[pivot]) {
                varargout_1 = true;
              } else if (i == cmp_tunableEnvironment[0].f1->data[pivot]) {
                varargout_1 = (cmp_tunableEnvironment[1].f1->data[x->data[pmin]
                               - 1] < cmp_tunableEnvironment[1].f1->data[pivot]);
              } else {
                varargout_1 = false;
              }

              if (varargout_1) {
                pmin++;
              } else {
                exitg3 = 1;
              }
            } while (exitg3 == 0);

            j--;
            do {
              exitg3 = 0;
              i = cmp_tunableEnvironment[0].f1->data[x->data[j] - 1];
              if (cmp_tunableEnvironment[0].f1->data[pivot] < i) {
                varargout_1 = true;
              } else if (cmp_tunableEnvironment[0].f1->data[pivot] == i) {
                varargout_1 = (cmp_tunableEnvironment[1].f1->data[pivot] <
                               cmp_tunableEnvironment[1].f1->data[x->data[j] - 1]);
              } else {
                varargout_1 = false;
              }

              if (varargout_1) {
                j--;
              } else {
                exitg3 = 1;
              }
            } while (exitg3 == 0);

            if (pmin + 1 >= j + 1) {
              exitg2 = 1;
            } else {
              pmax = x->data[pmin];
              x->data[pmin] = x->data[j];
              x->data[j] = pmax;
            }
          } while (exitg2 == 0);

          x->data[frame.xend - 2] = x->data[pmin];
          x->data[pmin] = pivot + 1;
          if (pmin + 2 < frame.xend) {
            st_d_data[st_n].xstart = pmin + 2;
            st_d_data[st_n].xend = frame.xend;
            st_d_data[st_n].depth = frame.depth + 1;
            st_n++;
          }

          if (frame.xstart < pmin + 1) {
            st_d_data[st_n].xstart = frame.xstart;
            st_d_data[st_n].xend = pmin + 1;
            st_d_data[st_n].depth = frame.depth + 1;
            st_n++;
          }
        }
      }
    }
  }
}

void c_introsort(emxArray_int32_T *x, int xstart, int xend)
{
  struct_T st_d_data[120];
  struct_T frame;
  int MAXDEPTH;
  int exitg2;
  int nsort;
  int pivot;
  int pmax;
  int pmin;
  int pow2p;
  boolean_T exitg1;
  if (xstart < xend) {
    nsort = (xend - xstart) + 1;
    if (nsort <= 32) {
      c_insertionsort(x, xstart, xend);
    } else {
      pmax = 31;
      pmin = 0;
      exitg1 = false;
      while ((!exitg1) && (pmax - pmin > 1)) {
        pivot = (pmin + pmax) >> 1;
        pow2p = 1 << pivot;
        if (pow2p == nsort) {
          pmax = pivot;
          exitg1 = true;
        } else if (pow2p > nsort) {
          pmax = pivot;
        } else {
          pmin = pivot;
        }
      }

      MAXDEPTH = (pmax - 1) << 1;
      frame.xstart = xstart;
      frame.xend = xend;
      frame.depth = 0;
      nsort = MAXDEPTH << 1;
      for (pmax = 0; pmax < nsort; pmax++) {
        st_d_data[pmax] = frame;
      }

      st_d_data[0] = frame;
      pow2p = 1;
      while (pow2p > 0) {
        frame = st_d_data[pow2p - 1];
        pow2p--;
        pmax = frame.xend - frame.xstart;
        if (pmax + 1 <= 32) {
          c_insertionsort(x, frame.xstart, frame.xend);
        } else if (frame.depth == MAXDEPTH) {
          d_heapsort(x, frame.xstart, frame.xend);
        } else {
          nsort = (frame.xstart + pmax / 2) - 1;
          pmax = x->data[frame.xstart - 1];
          if (x->data[nsort] < pmax) {
            x->data[frame.xstart - 1] = x->data[nsort];
            x->data[nsort] = pmax;
          }

          if (x->data[frame.xend - 1] < x->data[frame.xstart - 1]) {
            pmin = x->data[frame.xstart - 1];
            x->data[frame.xstart - 1] = x->data[frame.xend - 1];
            x->data[frame.xend - 1] = pmin;
          }

          if (x->data[frame.xend - 1] < x->data[nsort]) {
            pmin = x->data[nsort];
            x->data[nsort] = x->data[frame.xend - 1];
            x->data[frame.xend - 1] = pmin;
          }

          pivot = x->data[nsort];
          x->data[nsort] = x->data[frame.xend - 2];
          x->data[frame.xend - 2] = pivot;
          nsort = frame.xstart - 1;
          pmax = frame.xend - 2;
          do {
            exitg2 = 0;
            for (nsort++; x->data[nsort] < pivot; nsort++) {
            }

            for (pmax--; pivot < x->data[pmax]; pmax--) {
            }

            if (nsort + 1 >= pmax + 1) {
              exitg2 = 1;
            } else {
              pmin = x->data[nsort];
              x->data[nsort] = x->data[pmax];
              x->data[pmax] = pmin;
            }
          } while (exitg2 == 0);

          x->data[frame.xend - 2] = x->data[nsort];
          x->data[nsort] = pivot;
          if (nsort + 2 < frame.xend) {
            st_d_data[pow2p].xstart = nsort + 2;
            st_d_data[pow2p].xend = frame.xend;
            st_d_data[pow2p].depth = frame.depth + 1;
            pow2p++;
          }

          if (frame.xstart < nsort + 1) {
            st_d_data[pow2p].xstart = frame.xstart;
            st_d_data[pow2p].xend = nsort + 1;
            st_d_data[pow2p].depth = frame.depth + 1;
            pow2p++;
          }
        }
      }
    }
  }
}

void introsort(int x[354])
{
  struct_T st_d[32];
  struct_T expl_temp;
  int ai;
  int aj;
  int exitg1;
  int exitg2;
  int i;
  int j;
  int pivot;
  int s_depth;
  int st_n;
  int t;
  int xmid;
  boolean_T varargout_1;
  for (i = 0; i < 32; i++) {
    st_d[i].xstart = 1;
    st_d[i].xend = 354;
    st_d[i].depth = 0;
  }

  st_d[0].xstart = 1;
  st_d[0].xend = 354;
  st_d[0].depth = 0;
  st_n = 1;
  while (st_n > 0) {
    expl_temp = st_d[st_n - 1];
    s_depth = st_d[st_n - 1].depth;
    st_n--;
    t = expl_temp.xend - expl_temp.xstart;
    if (t + 1 <= 32) {
      insertionsort(x, expl_temp.xstart, expl_temp.xend);
    } else if (expl_temp.depth == 16) {
      b_heapsort(x, expl_temp.xstart, expl_temp.xend);
    } else {
      xmid = (expl_temp.xstart + t / 2) - 1;
      ai = iv[x[xmid] - 1];
      t = x[expl_temp.xstart - 1];
      aj = iv[t - 1];
      if (ai < aj) {
        varargout_1 = true;
      } else if (ai == aj) {
        varargout_1 = (iv1[x[xmid] - 1] < iv1[t - 1]);
      } else {
        varargout_1 = false;
      }

      if (varargout_1) {
        x[expl_temp.xstart - 1] = x[xmid];
        x[xmid] = t;
      }

      j = x[expl_temp.xend - 1];
      ai = iv[j - 1];
      t = x[expl_temp.xstart - 1];
      aj = iv[t - 1];
      if (ai < aj) {
        varargout_1 = true;
      } else if (ai == aj) {
        varargout_1 = (iv1[j - 1] < iv1[t - 1]);
      } else {
        varargout_1 = false;
      }

      if (varargout_1) {
        x[expl_temp.xstart - 1] = j;
        x[expl_temp.xend - 1] = t;
      }

      ai = iv[x[expl_temp.xend - 1] - 1];
      aj = iv[x[xmid] - 1];
      if (ai < aj) {
        varargout_1 = true;
      } else if (ai == aj) {
        varargout_1 = (iv1[x[expl_temp.xend - 1] - 1] < iv1[x[xmid] - 1]);
      } else {
        varargout_1 = false;
      }

      if (varargout_1) {
        t = x[xmid];
        x[xmid] = x[expl_temp.xend - 1];
        x[expl_temp.xend - 1] = t;
      }

      pivot = x[xmid] - 1;
      x[xmid] = x[expl_temp.xend - 2];
      x[expl_temp.xend - 2] = pivot + 1;
      i = expl_temp.xstart - 1;
      j = expl_temp.xend - 2;
      xmid = iv[pivot];
      do {
        exitg1 = 0;
        i++;
        do {
          exitg2 = 0;
          ai = iv[x[i] - 1];
          if (ai < xmid) {
            varargout_1 = true;
          } else if (ai == xmid) {
            varargout_1 = (iv1[x[i] - 1] < iv1[pivot]);
          } else {
            varargout_1 = false;
          }

          if (varargout_1) {
            i++;
          } else {
            exitg2 = 1;
          }
        } while (exitg2 == 0);

        j--;
        do {
          exitg2 = 0;
          aj = iv[x[j] - 1];
          if (xmid < aj) {
            varargout_1 = true;
          } else if (xmid == aj) {
            varargout_1 = (iv1[pivot] < iv1[x[j] - 1]);
          } else {
            varargout_1 = false;
          }

          if (varargout_1) {
            j--;
          } else {
            exitg2 = 1;
          }
        } while (exitg2 == 0);

        if (i + 1 >= j + 1) {
          exitg1 = 1;
        } else {
          t = x[i];
          x[i] = x[j];
          x[j] = t;
        }
      } while (exitg1 == 0);

      x[expl_temp.xend - 2] = x[i];
      x[i] = pivot + 1;
      if (i + 2 < expl_temp.xend) {
        st_d[st_n].xstart = i + 2;
        st_d[st_n].xend = expl_temp.xend;
        st_d[st_n].depth = s_depth + 1;
        st_n++;
      }

      if (expl_temp.xstart < i + 1) {
        st_d[st_n].xstart = expl_temp.xstart;
        st_d[st_n].xend = i + 1;
        st_d[st_n].depth = s_depth + 1;
        st_n++;
      }
    }
  }
}

/* End of code generation (introsort.c) */
