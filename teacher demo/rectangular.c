#include "myhead.h"
/*for float data*/

void rectangular( m, n, lda, pos, matrix )
int m, n, lda;
MPI_Aint pos;
MPI_Datatype *matrix;
{
  MPI_Datatype temp, dtp[2];
  int blk[2];
  MPI_Aint dsp[2];

  MPI_Type_vector( m, n, lda, MPI_FLOAT, \
                   &temp );  

  blk[0] = 1; blk[1] = 1;
  dsp[0] = 0; dsp[1] = pos;
  /*如果你的衍生类型的组件是任意的存储序列，那么默认的长度将会几乎没有任何用处。
	在这些情况下，你可以通过在你的衍生类型定义里插入MPI_LB和MPI_UB类型的组件来控制类型的边界，从而控制类型的长度。
	一种最简单的解决方案是为你映射的类型的数组的低边界和高边界定基址，
	指明数组里第一个元素为类型的低边界，第二个元素的地址为类型的高边界。*/
  dtp[0] = temp; dtp[1] = MPI_UB;

  MPI_Type_struct( 2, blk, dsp, dtp, matrix);
  MPI_Type_extent( temp, &dsp[0] );
  MPI_Type_extent( *matrix, &dsp[1] );
  printf("\nThe extents are : %d and %d \n", dsp[0], dsp[1]);

  return;
}
