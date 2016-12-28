#pragma once

#include <math.h>

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

typedef struct Vec Vec;
typedef struct Mat Mat;
typedef struct Qtr Qtr;


/*******************************************************************************
 * Matrix type and matrix operations.
*******************************************************************************/

/**
 * Mat - 4x4 matrix.
 */
struct Mat {
	float data[16];
};

void
mat_mul(const Mat *a, const Mat *b, Mat *r_m);

void
mat_imul(Mat *m, const Mat *other);

void
mat_mulv(const Mat *m, const Vec *v, Vec *r_v);

void
mat_rotate(Mat *m, float x, float y, float z, float angle);

void
mat_rotatev(Mat *m, const Vec *v, float angle);

void
mat_rotateq(Mat *m, const Qtr *q);

Qtr
mat_get_rotation(const Mat *m);

void
mat_scale(Mat *m, float sx, float sy, float sz);

void
mat_scalev(Mat *m, const Vec *sv);

Vec
mat_get_scale(const Mat *m);

void
mat_translate(Mat *m, float tx, float ty, float tz);

void
mat_translatev(Mat *m, const Vec *tv);

Vec
mat_get_translation(const Mat *m);

void
mat_lookat(
	Mat *m,
	float eye_x, float eye_y, float eye_z,
	float center_x, float center_y, float center_z,
	float up_x, float up_y, float up_z
);

void
mat_lookatv(Mat *m, const Vec *eye, const Vec *center, const Vec *up);

void
mat_ortho(Mat *m, float l, float r, float t, float b, float n, float f);

void
mat_persp(Mat *m, float fovy, float aspect, float n, float f);

void
mat_ident(Mat *m);

int
mat_inverse(Mat *m, Mat *out_m);

void
mat_transpose(Mat *m, Mat *out_m);

/*******************************************************************************
 * Vector type and vector operations.
*******************************************************************************/

/**
 * Vec - 4D vector.
 */
struct Vec {
	float data[4];
};

Vec
vec(float x, float y, float z, float w);

void
vec_mulf(const Vec *v, float scalar, Vec *r_v);

void
vec_imulf(Vec *v, float scalar);

void
vec_add(const Vec *a, const Vec *b, Vec *r_v);

void
vec_iadd(Vec *v, const Vec *other);

void
vec_addf(const Vec *v, float scalar, Vec *r_v);

void
vec_iaddf(Vec *v, float scalar);

void
vec_sub(const Vec *a, const Vec *b, Vec *r_v);

void
vec_isub(Vec *v, const Vec *b);

void
vec_subf(const Vec *v, float scalar, Vec *r_v);

void
vec_isubf(Vec *v, float scalar);

float
vec_dot(const Vec *a, const Vec *b);

float
vec_mag(const Vec *v);

void
vec_cross(const Vec *a, const Vec *b, Vec *r_v);

void
vec_norm(Vec *v);

void
vec_clamp(Vec *v, float value);

void
vec_lerp(const Vec *a, const Vec *b, float t, Vec *r_v);

/*******************************************************************************
 * Quaternion type and quaternion operations
*******************************************************************************/
struct Qtr {
	float data[4];
};

Qtr
qtr(float w, float x, float y, float z);

void
qtr_rotate(Qtr *q, float x, float y, float z, float angle);

void
qtr_rotatev(Qtr *q, const Vec *axis, float angle);

void
qtr_add(const Qtr *a, const Qtr *b, Qtr *r_q);

void
qtr_iadd(Qtr *q, const Qtr *other);

void
qtr_mul(const Qtr *a, const Qtr *b, Qtr *r_q);

void
qtr_imul(Qtr *q, const Qtr *other);

void
qtr_mulf(const Qtr *a, float scalar, Qtr *r_q);

void
qtr_imulf(Qtr *q, float scalar);

void
qtr_norm(Qtr *a);

void
qtr_lerp(const Qtr *a, const Qtr *b, float t, Qtr *r_q);
