#pragma once
#include "winpaint.h"
#include <cmath>
#include <cstring>
#include <utility>

class vector3 {
public:
	double x, y, z;
	vector3() { x = y = z = 0; }
	vector3(const double&_x, const double&_y, const double&_z) {
		x = _x; y = _y; z = _z;
	}
	vector3 operator +=(const vector3&s) {
		x += s.x; y += s.y; z += s.z;
		return vector3(x, y, z);
	}
	vector3 operator -=(const vector3&s) {
		x -= s.x; y -= s.y; z -= s.z;
		return vector3(x, y, z);
	}
	vector3 operator *=(const double&s) { x *= s; y *= s; z *= s; }
	vector3 operator /=(const double&s) { x /= s; y /= s; z /= s; }
};
vector3 operator +(vector3 a, vector3 b) { return vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
vector3 operator -(vector3 a, vector3 b) { return vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
vector3 operator *(vector3 a, double b) { return vector3(a.x*b, a.y*b, a.z*b); }
vector3 operator /(vector3 a, double b) { return vector3(a.x / b, a.y / b, a.z / b); }

class plane {
public:
	bool used;
	double A, B, C, D;
	plane() { A = B = C = D = 0; used = 0; }
	plane(const double&a, const double&b, const double&c, const double&d) {
		A = a; B = b; C = c; D = d; used = 1;
	}
	plane(const vector3&a, const vector3&b, const vector3&c) {
		A = +a.y*b.z - a.y*c.z - b.y*a.z + b.y*c.z + c.y*a.z - c.y*b.z;
		B = -a.x*b.z + a.x*c.z + b.x*a.z - b.x*c.z - c.x*a.z + c.x*b.z;
		C = +a.x*b.y - a.x*c.y - b.x*a.y + b.x*c.y + c.x*a.y - c.x*b.y;
		D = -a.x*b.y*c.z + a.x*c.y*b.z + b.x*a.y*c.z - b.x*c.y*a.z - c.x*a.y*b.z + c.x*b.y*a.z;
	}
};

double _atan2(double a, double b) {
	return atan2(a, b);
}
#define getshowx(a)(_winw/2+a.x/a.y*200)
#define getshowy(a)(_winh/2-a.z/a.y*200)
struct PAIRDC {
	double fi;
	COLORREF se;
} *__Mp;
template <int N> class rect4 {
private:
	double b[N][4];
	int x_[N], y_[N];
	void _insert(vector3 a, vector3 b, vector3 c, COLORREF fc) {
		if (getshowy(b) < getshowy(a))swap(a, b);
		if (getshowy(c) < getshowy(a))swap(a, c);
		if (getshowy(c) < getshowy(b))swap(b, c);
		int ax = getshowx(a), ay = getshowy(a);
		int bx = getshowx(b), by = getshowy(b);
		int cx = getshowx(c), cy = getshowy(c);
		double az = a.y, bz = b.y, cz = c.y;
		for (int i = max(0, ay); i < min(_winh, cy + 1); ++i) {
			if (i < by) {
				int ix1 = ax + round(1.0*(i - ay)*(bx - ax) / (by - ay));
				int ix2 = ax + round(1.0*(i - ay)*(cx - ax) / (cy - ay));
				double iz1 = az + 1.0*(i - ay)*(bz - az) / (by - ay);
				double iz2 = az + 1.0*(i - ay)*(cz - az) / (cy - ay);
				if (ix1 > ix2)swap(ix1, ix2), swap(iz1, iz2);
				//i,   ix1,ix2,iz1,iz2
				for (int X = max(0,ix1); X < min(_winw,ix2+1); ++X) {
					int I = i * _winw + X;
					double Z = iz1 + (iz2 - iz1)*(X - ix1) / (ix2 - ix1);
					if (Z < __Mp[I].fi && Z >= 20) {
						(*(__Mp+I)).fi = Z;
						(*(__Mp+I)).se = fc;
					}
				}
			}
			if (i >= by) {
				int ix1 = cx + round(1.0*(i - cy)*(bx - cx) / (by - cy));
				int ix2 = cx + round(1.0*(i - cy)*(ax - cx) / (ay - cy));
				double iz1 = cz + 1.0*(i - cy)*(bz - cz) / (by - cy);
				double iz2 = cz + 1.0*(i - cy)*(az - cz) / (ay - cy);
				if (ix1 > ix2)swap(ix1, ix2), swap(iz1, iz2);
				for (int X = max(0, ix1); X < min(_winw, ix2 + 1); ++X) {
					int I = i * _winw + X;
					double Z = iz1 + (iz2 - iz1)*(X - ix1) / (ix2 - ix1);
					if (Z < __Mp[I].fi && Z >= 20) {
						(*(__Mp+I)).fi = Z;
						(*(__Mp+I)).se = fc;
					}
				}
			}
		}
	}
public:
	double a[N][4];
	COLORREF c[N/3];
	rect4() { memset(a, 0, sizeof a); }
	void init() { memset(a, 0, sizeof a); }
	void operator *=(rect4<4> s) {
		memset(b, 0, sizeof b);
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < 4; ++j)
				for (int k = 0; k < 4; ++k)
					b[i][j] += a[i][k] * s.a[k][j];
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < 4; ++j)
				a[i][j] = b[i][j];
	}
	void setdot(int k, double x, double y, double z) {
		a[k][0] = x; a[k][1] = y; a[k][2] = z; a[k][3] = 1;
	}
	void setdot(int k, vector3 a) {
		setdot(k, a.x, a.y, a.z);
	}
	void settriangle(int k, vector3 x, vector3 y, vector3 z, COLORREF fc) {
		setdot(k * 3 + 0, x);
		setdot(k * 3 + 1, y);
		setdot(k * 3 + 2, z);
		c[k] = fc;
	}
	void setfc(int k, COLORREF fc) {
		c[k] = fc;
	}
	void paint() {
		__Mp = new PAIRDC[_winw*_winh];
		for (int i = 0; i < _winw*_winh; ++i)
			(*(__Mp+i)).fi=1e18, (*(__Mp+i)).se=BLACK;
		for (int i = 0; i < N; i += 3) {
			vector3 a1(a[i][0], a[i][1], a[i][2]);
			vector3 a2(a[i + 1][0], a[i + 1][1], a[i + 1][2]);
			vector3 a3(a[i + 2][0], a[i + 2][1], a[i + 2][2]);
			if (a1.y < 0 && a2.y < 0 && a3.y < 0)continue;
			if (a1.y > 0 && a2.y > 0 && a3.y > 0) {
				_insert(a1, a2, a3, c[i / 3]);
				continue;
			}
		}
		
		beginPdot();
		for (int j = 0, I = 0; j<_winh; ++j)
			for (int i = 0; i < _winw; ++i, ++I)
				Pdot(i, j, (*(__Mp+I)).se);
		endPdot();
		
		delete []__Mp;
	}
};
typedef rect4<4> trect4, *lptrect4;
template <int N> rect4<N> operator *(rect4<N> a, trect4 b) {
	rect4<N> c;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				c.a[i][j] += a.a[i][k] * b.a[k][j];
			}
		}
	}
	return c;
}
trect4 getrotate(double x, double y, double z, double n) {
	trect4 s;
	s.init();
	double r = sqrt(x*x + y * y + z * z);
	x /= r; y /= r; z /= r;
	double cn = cos(n), sn = sin(n);
	s.a[0][0] = x * x*(1 - cn) + cn; s.a[0][1] = x * y*(1 - cn) + z * sn; s.a[0][2] = x * z*(1 - cn) - y * sn;
	s.a[1][0] = x * y*(1 - cn) - z * sn; s.a[1][1] = y * y*(1 - cn) + cn; s.a[1][2] = y * z*(1 - cn) + x * sn;
	s.a[2][0] = x * z*(1 - cn) + y * sn; s.a[2][1] = y * z*(1 - cn) - x * sn; s.a[2][2] = z * z*(1 - cn) + cn;
	s.a[3][3] = 1;
	return s;
}
trect4 getmove(double x, double y, double z) {
	trect4 s;
	s.init();
	s.a[0][0] = 1;
	s.a[1][1] = 1;
	s.a[2][2] = 1;
	s.a[3][0] = x;
	s.a[3][1] = y;
	s.a[3][2] = z;
	s.a[3][3] = 1;
	return s;
}
trect4 getidentity() {
	trect4 s;
	s.init();
	s.a[0][0] = 1;
	s.a[1][1] = 1;
	s.a[2][2] = 1;
	s.a[3][3] = 1;
	return s;
}