#include "matlib.h"
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

void
mat_mul(const Mat *a, const Mat *b, Mat *r)
{
	memset(r, 0, sizeof(Mat));
	for (uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 4; j++) {
			float sum = 0;
			for (uint8_t k = 0; k < 4; k++) {
				sum += a->data[i * 4 + k] * b->data[k * 4 + j];
			}
			r->data[i * 4 + j] = sum;
		}
	}
}

void
mat_imul(Mat *m, const Mat *other)
{
	Mat tmp;
	mat_mul(m, other, &tmp);
	*m = tmp;
}

void
mat_mulv(const Mat *m, const Vec *v, Vec *r_v)
{
	memset(r_v, 0, sizeof(Vec));
	for (uint8_t i = 0; i < 4; i++) {
		float sum = 0;
		for (uint8_t j = 0; j < 4; j++) {
			sum += m->data[i * 4 + j] * v->data[j];
		}
		r_v->data[i] = sum;
	}
}

void
mat_rotate(Mat *m, float x, float y, float z, float angle)
{
	Vec v = {{x, y, z}};
	mat_rotatev(m, &v, angle);
}

void
mat_rotatev(Mat *m, const Vec *v, float angle)
{
	Mat rm, tmp;
	mat_ident(&rm);

	const float x = v->data[0];
	const float y = v->data[1];
	const float z = v->data[2];
	const float sin_a = sin(angle);
	const float cos_a = cos(angle);
	const float k = 1 - cos(angle);

	rm.data[0] = cos_a + k * x * x;
	rm.data[1] = k * x * y - z * sin_a;
	rm.data[2] = k * x * z + y * sin_a;
	rm.data[4] = k * x * y + z * sin_a;
	rm.data[5] = cos_a + k * y * y;
	rm.data[6] = k * y * z - x * sin_a;
	rm.data[8] = k * x * z - y * sin_a;
	rm.data[9] = k * y * z + x * sin_a;
	rm.data[10] = cos_a + k * z * z;
	rm.data[15] = 1.0f;

	mat_mul(m, &rm, &tmp);
	*m = tmp;
}

void
mat_rotateq(Mat *m, const Qtr *q)
{
	float w = q->data[0], x = q->data[1], y = q->data[2], z = q->data[3];
	Mat tmp, rm = {{
		1.0 - 2.0 * (y * y + z * z), 2.0 * (x * y - z * w),       2.0 * (x * z + y * w),       0.0,
		2.0 * (x * y + z * w),       1.0 - 2.0 * (x * x + z * z), 2.0 * (y * z - x * w),       0.0,
		2.0 * (x * z - y * w),       2.0 * (y * z + x * w),       1.0 - 2.0 * (x * x + y * y), 0.0,
		0.0,                         0.0,                         0.0,                         1.0
	}};
	mat_mul(m, &rm, &tmp);
	*m = tmp;
}


void
mat_scale(Mat *m, float sx, float sy, float sz)
{
	Mat sm, tmp;
	mat_ident(&sm);
	sm.data[0] = sx;
	sm.data[5] = sy;
	sm.data[10] = sz;

	mat_mul(m, &sm, &tmp);
	*m = tmp;
}

void
mat_scalev(Mat *m, const Vec *sv)
{
	mat_scale(m, sv->data[0], sv->data[1], sv->data[2]);
}

Vec
mat_get_scale(const Mat *m)
{
	Vec vx = vec(m->data[0], m->data[4], m->data[8], 0);
	Vec vy = vec(m->data[1], m->data[5], m->data[9], 0);
	Vec vz = vec(m->data[2], m->data[6], m->data[10], 0);
	return vec(vec_mag(&vx), vec_mag(&vy), vec_mag(&vz), 0);
}

Vec
mat_get_translation(const Mat *m)
{
	Vec o = vec(0, 0, 0, 1);
	Vec pos, result;
	mat_mulv(m, &o, &pos);
	vec_mulf(&pos, -1.0f, &result);
	return result;
}

Qtr
mat_get_rotation(const Mat *m)
{
	const float *mat = m->data;
	float t = 1 + mat[0] + mat[5] + mat[10], s, x, y, z, w;
	if (fabs(t) > 0.00000001) {
		s = sqrt(t) * 2;
		x = (mat[9] - mat[6]) / s;
		y = (mat[2] - mat[8]) / s;
		z = (mat[4] - mat[1]) / s;
		w = 0.25 * s;
	} else if (mat[0] > mat[5] && mat[0] > mat[10])  {
		s = sqrt(1.0 + mat[0] - mat[5] - mat[10]) * 2;
		x = 0.25 * s;
		y = (mat[4] + mat[1]) / s;
		z = (mat[2] + mat[8]) / s;
		w = (mat[9] - mat[6]) / s;
	} else if (mat[5] > mat[10]) {
		s = sqrt(1.0 + mat[5] - mat[0] - mat[10]) * 2;
		x = (mat[4] + mat[1]) / s;
		y = 0.25 * s;
		z = (mat[9] + mat[6]) / s;
		w = (mat[2] - mat[8]) / s;
	} else {
		s = sqrt(1.0 + mat[10] - mat[0] - mat[5]) * 2;
		x = (mat[2] + mat[8]) / s;
		y = (mat[9] + mat[6]) / s;
		z = 0.25 * s;
		w = (mat[4] - mat[1]) / s;
	}

	return qtr(w, x, y, z);
}

void
mat_translate(Mat *m, float tx, float ty, float tz)
{
	Mat tm, tmp;
	mat_ident(&tm);
	tm.data[3] = tx;
	tm.data[7] = ty;
	tm.data[11] = tz;

	mat_mul(m, &tm, &tmp);
	*m = tmp;
}

void
mat_translatev(Mat *m, const Vec *tv)
{
	mat_translate(m, tv->data[0], tv->data[1], tv->data[2]);
}

void
mat_ident(Mat *m)
{
	memset(m, 0, sizeof(Mat));
	m->data[0] = m->data[5] = m->data[10] = m->data[15] = 1;
}

int
mat_inverse(Mat *m, Mat *out_m)
{
	float inv[16], det;
	float *mdata = m->data;
	float *out_mdata = out_m->data;

	inv[0] = mdata[5]  * mdata[10] * mdata[15] -
	         mdata[5]  * mdata[11] * mdata[14] -
	         mdata[9]  * mdata[6]  * mdata[15] +
	         mdata[9]  * mdata[7]  * mdata[14] +
	         mdata[13] * mdata[6]  * mdata[11] -
	         mdata[13] * mdata[7]  * mdata[10];

	inv[4] = -mdata[4]  * mdata[10] * mdata[15] +
	         mdata[4]  * mdata[11] * mdata[14] +
	         mdata[8]  * mdata[6]  * mdata[15] -
	         mdata[8]  * mdata[7]  * mdata[14] -
	         mdata[12] * mdata[6]  * mdata[11] +
	         mdata[12] * mdata[7]  * mdata[10];

	inv[8] = mdata[4]  * mdata[9] * mdata[15] -
	         mdata[4]  * mdata[11] * mdata[13] -
	         mdata[8]  * mdata[5] * mdata[15] +
	         mdata[8]  * mdata[7] * mdata[13] +
	         mdata[12] * mdata[5] * mdata[11] -
	         mdata[12] * mdata[7] * mdata[9];

	inv[12] = -mdata[4]  * mdata[9] * mdata[14] +
	         mdata[4]  * mdata[10] * mdata[13] +
	         mdata[8]  * mdata[5] * mdata[14] -
	         mdata[8]  * mdata[6] * mdata[13] -
	         mdata[12] * mdata[5] * mdata[10] +
	         mdata[12] * mdata[6] * mdata[9];

	inv[1] = -mdata[1]  * mdata[10] * mdata[15] +
	         mdata[1]  * mdata[11] * mdata[14] +
	         mdata[9]  * mdata[2] * mdata[15] -
	         mdata[9]  * mdata[3] * mdata[14] -
	         mdata[13] * mdata[2] * mdata[11] +
	         mdata[13] * mdata[3] * mdata[10];

	inv[5] = mdata[0]  * mdata[10] * mdata[15] -
	         mdata[0]  * mdata[11] * mdata[14] -
	         mdata[8]  * mdata[2] * mdata[15] +
	         mdata[8]  * mdata[3] * mdata[14] +
	         mdata[12] * mdata[2] * mdata[11] -
	         mdata[12] * mdata[3] * mdata[10];

	inv[9] = -mdata[0]  * mdata[9] * mdata[15] +
	         mdata[0]  * mdata[11] * mdata[13] +
	         mdata[8]  * mdata[1] * mdata[15] -
	         mdata[8]  * mdata[3] * mdata[13] -
	         mdata[12] * mdata[1] * mdata[11] +
	         mdata[12] * mdata[3] * mdata[9];

	inv[13] = mdata[0]  * mdata[9] * mdata[14] -
	          mdata[0]  * mdata[10] * mdata[13] -
	          mdata[8]  * mdata[1] * mdata[14] +
	          mdata[8]  * mdata[2] * mdata[13] +
	          mdata[12] * mdata[1] * mdata[10] -
	          mdata[12] * mdata[2] * mdata[9];

	inv[2] = mdata[1]  * mdata[6] * mdata[15] -
	         mdata[1]  * mdata[7] * mdata[14] -
	         mdata[5]  * mdata[2] * mdata[15] +
	         mdata[5]  * mdata[3] * mdata[14] +
	         mdata[13] * mdata[2] * mdata[7] -
	         mdata[13] * mdata[3] * mdata[6];

	inv[6] = -mdata[0]  * mdata[6] * mdata[15] +
	         mdata[0]  * mdata[7] * mdata[14] +
	         mdata[4]  * mdata[2] * mdata[15] -
	         mdata[4]  * mdata[3] * mdata[14] -
	         mdata[12] * mdata[2] * mdata[7] +
	         mdata[12] * mdata[3] * mdata[6];

	inv[10] = mdata[0]  * mdata[5] * mdata[15] -
	          mdata[0]  * mdata[7] * mdata[13] -
	          mdata[4]  * mdata[1] * mdata[15] +
	          mdata[4]  * mdata[3] * mdata[13] +
	          mdata[12] * mdata[1] * mdata[7] -
	          mdata[12] * mdata[3] * mdata[5];

	inv[14] = -mdata[0]  * mdata[5] * mdata[14] +
	          mdata[0]  * mdata[6] * mdata[13] +
	          mdata[4]  * mdata[1] * mdata[14] -
	          mdata[4]  * mdata[2] * mdata[13] -
	          mdata[12] * mdata[1] * mdata[6] +
	          mdata[12] * mdata[2] * mdata[5];

	inv[3] = -mdata[1] * mdata[6] * mdata[11] +
	         mdata[1] * mdata[7] * mdata[10] +
	         mdata[5] * mdata[2] * mdata[11] -
	         mdata[5] * mdata[3] * mdata[10] -
	         mdata[9] * mdata[2] * mdata[7] +
	         mdata[9] * mdata[3] * mdata[6];

	inv[7] = mdata[0] * mdata[6] * mdata[11] -
	         mdata[0] * mdata[7] * mdata[10] -
	         mdata[4] * mdata[2] * mdata[11] +
	         mdata[4] * mdata[3] * mdata[10] +
	         mdata[8] * mdata[2] * mdata[7] -
	         mdata[8] * mdata[3] * mdata[6];

	inv[11] = -mdata[0] * mdata[5] * mdata[11] +
	          mdata[0] * mdata[7] * mdata[9] +
	          mdata[4] * mdata[1] * mdata[11] -
	          mdata[4] * mdata[3] * mdata[9] -
	          mdata[8] * mdata[1] * mdata[7] +
	          mdata[8] * mdata[3] * mdata[5];

	inv[15] = mdata[0] * mdata[5] * mdata[10] -
	          mdata[0] * mdata[6] * mdata[9] -
		  mdata[4] * mdata[1] * mdata[10] +
		  mdata[4] * mdata[2] * mdata[9] +
		  mdata[8] * mdata[1] * mdata[6] -
		  mdata[8] * mdata[2] * mdata[5];

	det = mdata[0] * inv[0] + mdata[1] * inv[4] +
	      mdata[2] * inv[8] + mdata[3] * inv[12];
	if (det == 0)
		return 0;

	det = 1.0 / det;

	for (int i = 0; i < 16; i++)
		out_mdata[i] = inv[i] * det;

	return 1;
}


void
mat_transpose(Mat *m, Mat *out_m)
{
	for (short i = 0; i < 4; i++) {
		for (short j = 0; j < 4; j++) {
			out_m->data[i * 4 + j] = m->data[j * 4 + i];
		}
	}
}

void
mat_lookat(
	Mat *m,
	float eye_x, float eye_y, float eye_z,
	float center_x, float center_y, float center_z,
	float up_x, float up_y, float up_z
) {
	Vec eye = vec(eye_x, eye_y, eye_z, 0);
	Vec center = vec(center_x, center_y, center_z, 0);
	Vec up = vec(up_x, up_y, up_z, 0);
	mat_lookatv(m, &eye, &center, &up);
}

void
mat_lookatv(Mat *m, const Vec *eye, const Vec *center, const Vec *up)
{
	Vec z;
	vec_sub(center, eye, &z);
	vec_norm(&z);

	Vec up_norm;
	memcpy(&up_norm, up, sizeof(Vec));
	vec_norm(&up_norm);

	Vec x;
	vec_cross(&z, &up_norm, &x);
	vec_norm(&x);

	Vec y;
	vec_cross(&x, &z, &y);
	vec_norm(&y);

	Mat lookat = {{
		 x.data[0],  x.data[1],  x.data[2], 0.0,
		 y.data[0],  y.data[1],  y.data[2], 0.0,
		-z.data[0], -z.data[1], -z.data[2], 0.0,
		0,          0,          0,          1
	}};
	memcpy(m, &lookat, sizeof(Mat));
}

void
mat_ortho(Mat *m, float l, float r, float t, float b, float n, float f)
{
	float x = 2.0f / (r - l);
	float y = 2.0f / (t - b);
	float z = -2.0f / (f - n);

	float tx = -(r + l) / (r - l);
	float ty = -(t + b) / (t - b);
	float tz = -(f + n) / (f - n);

	Mat ortho = {{
		x, 0, 0, tx,
		0, y, 0, ty,
		0, 0, z, tz,
		0, 0, 0, 1
	}};
	memcpy(m, &ortho, sizeof(Mat));
}

void
mat_persp(Mat *m, float fovy, float aspect, float n, float f)
{
	fovy = M_PI / 180.0 * fovy;
	float y = 1.0 / (fovy / 2.0);
	float x = y / aspect;
	float z = (f + n) / (n - f);
	float tz = (2 * f * n) / (n - f);
	Mat persp = {{
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, tz,
		0, 0, -1, 0
	}};
	memcpy(m, &persp, sizeof(Mat));
}

Vec
vec(float x, float y, float z, float w)
{
	Vec v = {{ x, y, z, w }};
	return v;
}

void
vec_addf(const Vec *v, float scalar, Vec *r_v)
{
	r_v->data[0] = v->data[0] + scalar;
	r_v->data[1] = v->data[1] + scalar;
	r_v->data[2] = v->data[2] + scalar;
	r_v->data[3] = v->data[3] + scalar;
}

void
vec_iaddf(Vec *v, float scalar)
{
	vec_addf(v, scalar, v);
}

void
vec_add(const Vec *a, const Vec *b, Vec *r_v)
{
	r_v->data[0] = a->data[0] + b->data[0];
	r_v->data[1] = a->data[1] + b->data[1];
	r_v->data[2] = a->data[2] + b->data[2];
	r_v->data[3] = a->data[3] + b->data[3];
}

void
vec_iadd(Vec *v, const Vec *other)
{
	vec_add(v, other, v);
}

void
vec_subf(const Vec *v, float scalar, Vec *r_v)
{
	r_v->data[0] = v->data[0] - scalar;
	r_v->data[1] = v->data[1] - scalar;
	r_v->data[2] = v->data[2] - scalar;
	r_v->data[3] = v->data[3] - scalar;
}

void
vec_isubf(Vec *v, float scalar)
{
	vec_subf(v, scalar, v);
}

void
vec_sub(const Vec *a, const Vec *b, Vec *r_v)
{
	r_v->data[0] = a->data[0] - b->data[0];
	r_v->data[1] = a->data[1] - b->data[1];
	r_v->data[2] = a->data[2] - b->data[2];
	r_v->data[3] = a->data[3] - b->data[3];
}

void
vec_isub(Vec *v, const Vec *other)
{
	vec_sub(v, other, v);
}

void
vec_mulf(const Vec *v, float scalar, Vec *r_v)
{
	r_v->data[0] = v->data[0] * scalar;
	r_v->data[1] = v->data[1] * scalar;
	r_v->data[2] = v->data[2] * scalar;
	r_v->data[3] = v->data[3] * scalar;
}

void
vec_imulf(Vec *v, float scalar)
{
	vec_mulf(v, scalar, v);
}

float
vec_dot(const Vec *a, const Vec *b)
{
	float sum = 0;
	for (uint8_t i = 0; i < 3; i++) {
		sum += a->data[i] * b->data[i];
	}
	return sum;
}

float
vec_mag(const Vec *v)
{
	float x = v->data[0], y = v->data[1], z = v->data[2];
	return sqrt(x * x + y * y + z * z);
}

void
vec_norm(Vec *v)
{
	vec_mulf(v, 1.0f / vec_mag(v), v);

}

void
vec_clamp(Vec *v, float value)
{
	if (vec_mag(v) > value) {
		vec_norm(v);
		vec_imulf(v, value);
	}
}

void
vec_cross(const Vec *a, const Vec *b, Vec *r_v)
{
	r_v->data[0] = a->data[1] * b->data[2] - a->data[2] * b->data[1];
	r_v->data[1] = a->data[2] * b->data[0] - a->data[0] * b->data[2];
	r_v->data[2] = a->data[0] * b->data[1] - a->data[1] * b->data[0];
	r_v->data[3] = 0;  // no cross product exists for 4D vectors
}

void
vec_lerp(const Vec *a, const Vec *b, float t, Vec *r_v)
{
	Vec at, bt;
	vec_mulf(a, 1 - t, &at);
	vec_mulf(b, t, &bt);
	vec_add(&at, &bt, r_v);
}


Qtr
qtr(float w, float x, float y, float z)
{
	Qtr q = {{w, x, y, z}};
	return q;
}

void
qtr_rotate(Qtr *q, float x, float y, float z, float angle)
{
	float s = sin(angle / 2.0f);
	q->data[0] = cos(angle / 2.0f);
	q->data[1] = x * s;
	q->data[2] = y * s;
	q->data[3] = z * s;
}

void
qtr_rotatev(Qtr *q, const Vec *axis, float angle)
{
	float s = sin(angle / 2.0f);
	q->data[0] = cos(angle / 2.0f);
	q->data[1] = axis->data[0] * s;
	q->data[2] = axis->data[1] * s;
	q->data[3] = axis->data[2] * s;
}

void
qtr_mulf(const Qtr *a, float scalar, Qtr *r_q)
{
	r_q->data[0] = a->data[0] * scalar;
	r_q->data[1] = a->data[1] * scalar;
	r_q->data[2] = a->data[2] * scalar;
	r_q->data[3] = a->data[3] * scalar;
}

void
qtr_imulf(Qtr *q, float scalar)
{
	q->data[0] *= scalar;
	q->data[1] *= scalar;
	q->data[2] *= scalar;
	q->data[3] *= scalar;
}

void
qtr_mul(const Qtr *a, const Qtr *b, Qtr *r_q)
{
	r_q->data[1] =  a->data[1] * b->data[0] + a->data[2] * b->data[3] - a->data[3] * b->data[2] + a->data[0] * b->data[1];
	r_q->data[2] = -a->data[1] * b->data[3] + a->data[2] * b->data[0] + a->data[3] * b->data[1] + a->data[0] * b->data[2];
	r_q->data[3] =  a->data[1] * b->data[2] - a->data[2] * b->data[1] + a->data[3] * b->data[0] + a->data[0] * b->data[3];
	r_q->data[0] = -a->data[1] * b->data[1] - a->data[2] * b->data[2] - a->data[3] * b->data[3] + a->data[0] * b->data[0];
}

void
qtr_imul(Qtr *q, const Qtr *other)
{
	Qtr tmp;
	qtr_mul(q, other, &tmp);
	*q = tmp;
}

void
qtr_add(const Qtr *a, const Qtr *b, Qtr *r_q)
{
	r_q->data[0] = a->data[0] + b->data[0];
	r_q->data[1] = a->data[1] + b->data[1];
	r_q->data[2] = a->data[2] + b->data[2];
	r_q->data[3] = a->data[3] + b->data[3];
}

void
qtr_iadd(Qtr *q, const Qtr *other)
{
	q->data[0] += other->data[0];
	q->data[1] += other->data[1];
	q->data[2] += other->data[2];
	q->data[3] += other->data[3];
}

void
qtr_norm(Qtr *q)
{
	float n = sqrt(
		q->data[0] * q->data[0] +
		q->data[1] * q->data[1] +
		q->data[2] * q->data[2] +
		q->data[3] * q->data[3]
	);
	q->data[0] /= n;
	q->data[1] /= n;
	q->data[2] /= n;
	q->data[3] /= n;
}

void
qtr_lerp(const Qtr *a, const Qtr *b, float t, Qtr *r_q)
{
	Qtr at, bt;
	qtr_mulf(a, 1 - t, &at);
	qtr_mulf(b, t, &bt);
	qtr_add(&at, &bt, r_q);
	qtr_norm(r_q);
}

AABB
aabb(const Vec *v1, const Vec *v2)
{
	float xmin = v1->data[0], ymin = v1->data[1], zmin = v1->data[2];
	float xmax = v1->data[0], ymax = v1->data[1], zmax = v1->data[2];
	const Vec *v[] = { v1, v2 };
	for (short i = 0; i < 2; i++) {
		xmin = fmin(xmin, v[i]->data[0]);
		xmax = fmax(xmax, v[i]->data[0]);
		ymin = fmin(ymin, v[i]->data[1]);
		ymax = fmax(ymax, v[i]->data[1]);
		zmin = fmin(zmin, v[i]->data[2]);
		zmax = fmax(zmax, v[i]->data[2]);
	}
	AABB bb = {
		.near = vec(xmax, ymax, zmax, 0),
		.far = vec(xmin, ymin, zmin, 0),
	};
	return bb;
}

AABB
aabb_container(AABB *bblist, unsigned int size)
{
	float xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = xmax = ymin = ymax = zmin = zmax = 0;

	int first_run = 1;
	for (unsigned int k = 0; k < size; k++) {
		const Vec *v[] = { &bblist[k].far, &bblist[k].near };
		if (first_run) {
			xmin = v[0]->data[0];
			xmax = v[1]->data[0];
			ymin = v[0]->data[1];
			ymax = v[1]->data[1];
			zmin = v[0]->data[2];
			zmax = v[1]->data[2];
			first_run = 0;
			continue;
		}
		for (short i = 0; i < 2; i++) {
			xmin = fmin(xmin, v[i]->data[0]);
			xmax = fmax(xmax, v[i]->data[0]);
			ymin = fmin(ymin, v[i]->data[1]);
			ymax = fmax(ymax, v[i]->data[1]);
			zmin = fmin(zmin, v[i]->data[2]);
			zmax = fmax(zmax, v[i]->data[2]);
		}
	}
	AABB bb = {
		.near = vec(xmax, ymax, zmax, 0),
		.far = vec(xmin, ymin, zmin, 0),
	};
	return bb;
}

void
aabb_transform(AABB *bb, const Mat *mat)
{
	Vec v1, v2;
	mat_mulv(mat, &bb->near, &v1);
	mat_mulv(mat, &bb->far, &v2);

	AABB bb1 = aabb(&v1, &v2);
	*bb = bb1;
}
