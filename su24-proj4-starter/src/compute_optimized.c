#include <omp.h>
#include <x86intrin.h>

#include "compute.h"

int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix)
{
  int a_rows = a_matrix->rows;
  int a_cols = a_matrix->cols;
  int b_rows = b_matrix->rows;
  int b_cols = b_matrix->cols;

  int32_t *tmp_data = malloc(b_cols * b_rows * sizeof(int32_t));
  if (tmp_data == NULL)
    return -1;
#pragma omp parallel for collapse(2)
  for (int i = 0; i < b_rows; i++)
  {
    for (int j = 0; j < b_cols; j++)
    {
      tmp_data[i * b_cols + j] = b_matrix->data[(b_rows - i - 1) * b_cols + (b_cols - 1 - j)];
    }
  }
  matrix_t *rslt_matrix = malloc(sizeof(matrix_t));
  if (rslt_matrix == NULL)
  {
    free(tmp_data);
    return -1;
  }
  rslt_matrix->rows = a_rows - b_rows + 1;
  rslt_matrix->cols = a_cols - b_cols + 1;
  rslt_matrix->data = malloc(rslt_matrix->rows * rslt_matrix->cols * sizeof(int32_t));
  if (rslt_matrix->data == NULL)
  {
    free(tmp_data);
    free(rslt_matrix);
    return -1;
  }

  int block_size = b_cols * b_rows;
  int chunk = block_size / 8;
#pragma omp parallel
{
  int *windows = malloc(block_size * sizeof(int32_t));
  __m256i avx_a;
  __m256i avx_b;
  #pragma omp for collapse(2)
  for (int down = 0; down < rslt_matrix->rows; down++)
  {
    for (int right = 0; right < rslt_matrix->cols; right++)
    {
      __m256i sum = _mm256_setzero_si256();
      int x = 0, index = 0;
      
      for (int i = 0; i < b_rows; i++)
      {
        for (int j = 0; j < b_cols; j++)
        {
          windows[index++] = a_matrix->data[(i + down) * a_cols + j + right];
        }
      }
      for (int i = 0; i < chunk * 8; i += 8)
      {
        avx_a = _mm256_loadu_si256((__m256i *)&windows[i]);
        avx_b = _mm256_loadu_si256((__m256i *)&tmp_data[i]);
        avx_a = _mm256_mullo_epi32(avx_a, avx_b);
        sum = _mm256_add_epi32(avx_a, sum);
      }
      int32_t get[8];
      _mm256_storeu_si256((__m256i *)get, sum);
      for (int i = 0; i < 8; i++)
        x += get[i];
      for (int i = chunk * 8; i < block_size; i++)
      {
        x += windows[i] * tmp_data[i];
      }
      rslt_matrix->data[down * rslt_matrix->cols + right] = x;
    }
  }
  free(windows);
}
*output_matrix = rslt_matrix;
free(tmp_data);
return 0;
}

// Executes a task
int execute_task(task_t *task)
{
  matrix_t *a_matrix, *b_matrix, *output_matrix;

  char *a_matrix_path = get_a_matrix_path(task);
  if (read_matrix(a_matrix_path, &a_matrix))
  {
    printf("Error reading matrix from %s\n", a_matrix_path);
    return -1;
  }
  free(a_matrix_path);

  char *b_matrix_path = get_b_matrix_path(task);
  if (read_matrix(b_matrix_path, &b_matrix))
  {
    printf("Error reading matrix from %s\n", b_matrix_path);
    return -1;
  }
  free(b_matrix_path);

  if (convolve(a_matrix, b_matrix, &output_matrix))
  {
    printf("convolve returned a non-zero integer\n");
    return -1;
  }

  char *output_matrix_path = get_output_matrix_path(task);
  if (write_matrix(output_matrix_path, output_matrix))
  {
    printf("Error writing matrix to %s\n", output_matrix_path);
    return -1;
  }
  free(output_matrix_path);

  free(a_matrix->data);
  free(b_matrix->data);
  free(output_matrix->data);
  free(a_matrix);
  free(b_matrix);
  free(output_matrix);
  return 0;
}
