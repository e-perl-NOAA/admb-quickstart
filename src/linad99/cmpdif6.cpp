/*
 * $Id$
 *
 * Author: David Fournier
 * Copyright (c) 2008-2012 Regents of the University of California
 */
/**
 * \file
 * Description not yet available.
 */
#include <fvar.hpp>

#ifdef __TURBOC__
  #pragma hdrstop
  #include <iostream.h>
#endif

#ifdef __ZTC__
  #include <iostream.hpp>
#endif

#include <string.h>

/**
 * Saves size and address information for a dmatrix to adjoint data file.
 */
void dmatrix::save_dmatrix_position() const
{
  gradient_structure::get_fp()->save_dmatrix_position(*this);
}
void DF_FILE::save_dmatrix_position(const dmatrix& m)
{
  constexpr size_t wsize=sizeof(int);
  constexpr size_t wsize1=sizeof(void*);

  dmatrix_position tmp(m);

  int min=m.rowmin();
  int max=m.rowmax();
  for (int i=min;i<=max;i++)
  {
    fwrite(&(tmp.lb(i)),wsize);
    fwrite(&(tmp.ub(i)),wsize);
    fwrite(&(tmp.ptr(i)),wsize1);
  }
  fwrite(&(tmp.row_min),wsize);
  fwrite(&(tmp.row_max),wsize);
}

/**
 * Description not yet available.
 * \param
 */
void d3_array::save_d3_array_position(DF_FILE* fp) const
{
  // saves the size and address information for a dvar_vector
  int mmin=indexmin();
  int mmax=indexmax();
  constexpr size_t wsize = sizeof(int);
/*
  dmatrix_position tmp(*this);
  const int wsize=sizeof(int);
  const int wsize1=sizeof(void*);

  for (int i=min;i<=max;i++)
  {
    gradient_structure::get_fp()->fwrite(&(tmp.lb(i)),wsize);
    gradient_structure::get_fp()->fwrite(&(tmp.ub(i)),wsize);
    gradient_structure::get_fp()->fwrite(&(tmp.ptr(i)),wsize1);
   }
*/
  fp->fwrite(&(mmin),wsize);
  fp->fwrite(&(mmax),wsize);
}

/**
 * Description not yet available.
 * \param
 */
d3_array_position restore_d3_array_position()
{
  return gradient_structure::get_fp()->restore_d3_array_position();
}
d3_array_position DF_FILE::restore_d3_array_position()
{
  constexpr size_t wsize = sizeof(int);

  // saves the size and address information for a dvar_vector
  int mmin;
  int mmax;
  fread(&mmax,wsize);
  fread(&mmin,wsize);
  d3_array_position tmp(mmin,mmax);
  return tmp;
}

/**
 * Reads back the size and address information for a dvar_matrix,
 * restores the size, address, and value information for a dvar_vector
 */
dvar_matrix_position restore_dvar_matrix_position(DF_FILE* fp)
{
  return gradient_structure::get_fp()->restore_dvar_matrix_position();
}
dvar_matrix_position DF_FILE::restore_dvar_matrix_position()
{
  constexpr size_t wsize = sizeof(int);
  constexpr size_t wsize2 = sizeof(void*);

  int min;
  int max;
  fread(&max, wsize);
  fread(&min, wsize);
  dvar_matrix_position tmp(min,max);
  // cout << "tmp.ptr= " << tmp.ptr ;
  for (int i=max;i>=min;i--)
  {
    fread(&(tmp.ptr(i)), wsize2);
    fread(&(tmp.ub(i)), wsize);
    fread(&(tmp.lb(i)), wsize);
  }
  return tmp;
}

/**
 * Description not yet available.
 * \param
 */
dmatrix_position restore_dmatrix_position()
{
  return gradient_structure::get_fp()->restore_dmatrix_position();
}
dmatrix_position DF_FILE::restore_dmatrix_position()
{
  // reads back the size and address information for a dvar_matrix
  // restores the size, address, and value information for a dvar_vector
  int min;
  int max;
  fread(&max,sizeof(int));
  fread(&min,sizeof(int));
  dmatrix_position tmp(min,max);
  // cout << "tmp.ptr= " << tmp.ptr ;
  for (int i=max;i>=min;i--)
  {
    fread(&(tmp.ptr(i)),sizeof(void*));
    fread(&(tmp.ub(i)),sizeof(int));
    fread(&(tmp.lb(i)),sizeof(int));
  }
  return tmp;
}

/**
 * Description not yet available.
 * \param
 */
dmatrix restore_dvar_matrix_derivatives(const dvar_matrix_position& _pos)
{
  dvar_matrix_position& pos= (dvar_matrix_position&) _pos;
  dmatrix tmp(pos);
  for (int i=pos.row_max;i>=pos.row_min;i--)
  {
    tmp(i)=restore_dvar_vector_derivatives(pos(i));
  }
  return tmp;
}

/**
 * Description not yet available.
 * \param
 */
dmatrix restore_dvar_matrix_der_nozero(const dvar_matrix_position& _pos)
{
  dvar_matrix_position& pos= (dvar_matrix_position&) _pos;
  dmatrix tmp(pos);
  for (int i=pos.row_max;i>=pos.row_min;i--)
  {
    tmp(i)=restore_dvar_vector_der_nozero(pos(i));
  }
  return tmp;
}

/**
 * Description not yet available.
 * \param
 */
dvector restore_dvar_matrix_derivative_row(const dvar_matrix_position& _pos,
  const int& ii)
{
  dvar_matrix_position& pos= (dvar_matrix_position&) _pos;
  dvector tmp=restore_dvar_vector_derivatives(pos(ii));
  return tmp;
}

/**
 * Description not yet available.
 * \param
 */
dvector restore_dvar_matrix_derivative_column(const dvar_matrix_position& _pos,
  const int& ii)
{
  dvar_matrix_position& pos= (dvar_matrix_position&) _pos;
  dvector tmpvec(pos.rowmin(),pos.rowmax());
  int min=tmpvec.indexmin();
  int max=tmpvec.indexmax();
  for (int i=min;i<=max;i++)
  {
    tmpvec(i)=((pos(i)).va)[ii].xvalue();
    ((pos(i)).va)[ii].xvalue()=0.;
  }
  return tmpvec;
}

/**
 * Description not yet available.
 * \param
 */
dvar_vector nograd_assign(dvector tmp)
{
  kkludge_object kg;
  int min=tmp.indexmin();
  int max=tmp.indexmax();
  dvar_vector out(min,max,kg); // dvar_vector constructor that
                               // doesn't do any initialization
  for (int i=min;i<=max;i++)
  {
    value(out(i))=tmp(i);
  }
  return out;
}

/**
 * Description not yet available.
 * \param
 */
dvar_matrix nograd_assign(const dmatrix& m)
{
  // cout << "Entering nograd assign"<<endl;
  //kkludge_object kg;
  int nrl=m.rowmin();
  int nrh=m.rowmax();
  ivector ncl(nrl,nrh);
  ivector nch(nrl,nrh);
  int i;
  for (i=nrl;i<=nrh;i++)
  {
    ncl(i)=m(i).indexmin();
    nch(i)=m(i).indexmax();
  }
  dvar_matrix out(nrl,nrh,ncl,nch); // dvar_matrix constructor that
                                       // doesn't do any initialization
  for (i=nrl;i<=nrh;i++)
  {
    for (int j=ncl(i);j<=nch(i);j++)
    {
      value(out(i,j))=m(i,j);
    }
    // out(i)=nograd_assign(m(i));
  }
  // cout << "Leaving nograd assign"<<endl;
  return out;
}

/**
 * Description not yet available.
 * \param
 */
dvar_matrix nograd_assign_trans(const dmatrix& m)
{
  // cout << "Entering nograd assign"<<endl;
  //kkludge_object kg;
  int nrl=m.rowmin();
  int nrh=m.rowmax();
  ivector ncl(nrl,nrh);
  ivector nch(nrl,nrh);
  int i;
  for (i=nrl;i<=nrh;i++)
  {
    ncl(i)=m(i).indexmin();
    nch(i)=m(i).indexmax();
  }
  dvar_matrix out(nrl,nrh,ncl,nch); // dvar_matrix constructor that
                                       // doesn't do any initialization
  for (i=nrl;i<=nrh;i++)
  {
    for (int j=ncl(i);j<=nch(i);j++)
    {
      value(out(j,i))=m(i,j);
    }
    // out(i)=nograd_assign(m(i));
  }
  // cout << "Leaving nograd assign"<<endl;
  return out;
}

/**
 * Description not yet available.
 * \param
 */
void nograd_assign_column(const dvar_matrix& m, const dvector& v, const int& ii)
{
  // cout << "Entering nograd assign"<<endl;
  //kkludge_object kg;
  if (ii<m.colmin()||ii>m.colmax()  ||
   (v.indexmin()!=m.rowmin()) ||
   (v.indexmax()!=m.rowmax()) )
  {
    cerr << "Error -- Index out of bounds in\n"
      "void nograd_assign(const dvar_matrix& m,const dvector& v, const int& ii)"
      << endl;
    ad_exit(1);
  }
  int min=v.indexmin();
  int max=v.indexmax();
  for (int j=min;j<=max;j++)
  {
    value(m(j,ii))=v(j);
  }
    // out(i)=nograd_assign(m(i));
}

/**
 * Description not yet available.
 * \param
 */
  d3_array_position::d3_array_position(int mmin,int mmax) :
    min(mmin), max(mmax)
  {}
