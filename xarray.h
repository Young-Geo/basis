//  Created by 胡兵兵 on 16/4/1.
//  Copyright © 2016年 xxx_orgnization. All rights reserved.
//

#ifndef _H_XARRAY_H_
#define _H_XARRAY_H_
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#include "afx.h"
#include "xlog.h"
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#define CELL_SIZE 256
#define CELL_SIZE_2n 8
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
typedef struct _xarray_pos
{
  unsigned char sizeofv ;
  
  char * array ;
  unsigned int array_size ;
  unsigned short array_count ;
  
  unsigned int size ;
} xarray_pos ;
typedef struct _xarray
{
  xarray_pos * positive ;
  xarray_pos * nagetive ;
} xarray ;
////////////////////////////////////////////////////////////////////////////
void xarrayp_set ( xarray_pos* arrp , int i , void * v ) ;
int  xarrayp_get ( xarray_pos* arrp , int i , void * v ) ;
#define xarrayp_size(ARRP) ((ARRP) ->size)
////////////////////////////////////////////////////////////////////////////
xarray * xarray_alloc ( int sizeofv ) ;
void xarray_free ( xarray ** parr ) ;
////////////////////////////////////////////////////////////////////////////
#define xarray_set(ARR,I,V) if((I) < 0) xarrayp_set((ARR) ->nagetive, -(I), (V)) ; \
else xarrayp_set((ARR) ->positive, (I), (V)) ;
#define xarray_get(ARR,I,V) (((I) < 0) ? xarrayp_get((ARR) ->nagetive, -(I), (V)) : \
xarrayp_get((ARR) ->positive, (I), (V)))
void xarray_left_right ( xarray * arr , int * left , int * right ) ;
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
typedef struct _xarray2
{
  xarray * arr2 ;
  unsigned char sizeofv ;
} xarray2 ;
////////////////////////////////////////////////////////////////////////////
xarray2 * xarray2_alloc ( int sizeofv ) ;
void xarray2_free ( xarray2 ** parr2 ) ;
////////////////////////////////////////////////////////////////////////////
void xarray2_set ( xarray2 * arr2 , int i , int j , void * v ) ;
int  xarray2_get ( xarray2 * arr2 , int i , int j , void * v ) ;
void xarray2_left_right ( xarray2 * arr2 , int * left , int * right ) ;
void xarray2_bottom_top ( xarray2 * arr2 , int i , int * bottom , int * top ) ;
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////