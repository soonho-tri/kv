#include <boost/numeric/ublas/vector.hpp>
#include <kv/interval.hpp>
#include <kv/rdouble.hpp>


namespace ub = boost::numeric::ublas;

class Bellido {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(9);
		T z1,z2,z3,z4,z5,z6,z7,z8,z9;
		z1 = x(0); z2 = x(1); z3 = x(2); z4 = x(3);
		z5 = x(4); z6 = x(5); z7 = x(6); z8 = x(7);
		z9 = x(8);

		y(0) = (z1-6.)*(z1-6.)+z2*z2+z3*z3-104.;
		y(1) = z4*z4+(z5-6.)*(z5-6.)+z6*z6-104.;
		y(2) = z7*z7+(z8-12.)*(z8-12.)+(z9-6.)*(z9-6.)-80.;
		y(3) = z1*(z4-6.)+z5*(z2-6.)+z3*z6-52.;
		y(4) = z1*(z7-6.)+z8*(z2-12.)+z9*(z3-6.)+64.;
		y(5) = z4*z7+z8*(z5-12.)+z9*(z6-6.)-6.*z5+32.;
		y(6) = 2.*z2+2.*z3-2.*z6-z4-z5-z7-z9+18.;
		y(7) = z1+z2+2.*z3+2.*z4+2.*z6-2.*z7+z8-z9-38.;
		y(8) = z1+z3+z5-z6+2.*z7-2.*z8-2.*z4+8.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(9);
		for (i=0; i<9; i++) {
			x(i).assign(-1e8, 1e8);
		}
	}
};


class Bronstein {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(3);
		T X = x(0);
		T Y = x(1);
		T z = x(2);

		y(0) = X*X + Y*Y + z*z - 36.;
		y(1) = X + Y - z;
		y(2) = X*Y + z*z - 1.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(3);
		for (i=0; i<3; i++) {
			x(i).assign(-1e8, 1e8);
		}
	}
};


class Caprasse {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(4);
		T X = x(0);
		T Y = x(1);
		T z = x(2);
		T t = x(3);

		y(0) = Y*Y*z + 2.*X*Y*t - 2.*X - z;
		y(1) = -X*X*X*z + 4.*X*Y*Y*z + 4.*X*X*Y*t + 2.*Y*Y*Y*t + 4.*X*X - 10.*Y*Y + 4.*X*z - 10.*Y*t + 2.;
		y(2) = 2.*Y*z*t + X*t*t - X - 2.*z;
		y(3) = -X*z*z*z + 4.*Y*z*z*t + 4.*X*z*t*t + 2.*Y*t*t*t + 4.*X*z + 4.*z*z - 10.*Y*t - 10.*t*t + 2.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(4);
		for (i=0; i<4; i++) {
			x(i).assign(-10., 10.);
		}
	}
};


class Celestial {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(3);
		T p = x(0);
		T s = x(1);
		T phi = x(2);

		T p2 = p*p;
		T p3 = p2*p;

		T s2 = s*s;
		T s3 = s2*s;
		T s4 = s3*s;
		T s5 = s4*s;

		T phi2 = phi*phi;
		T phi3 = phi2*phi;

		y(0) = -6.*p3 + 4.*p3*phi3 + 15.* phi3*s3*p - 3.* phi3*s5 - 12.*phi3*s*p2 -3.*phi3*s*p + phi3*s3;
		y(1) = -9.*phi3*s2*p - 5.*phi3*s2 - 6.*s*p3 + 3.*phi3*s4 + 5.*phi3*p;
		y(2) = -12.*s2*p - 6.*s2 + 3.*s4 + 4.*phi2 + 3. + 12.*p;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(3);
		for (i=0; i<3; i++) {
			x(i).assign(0.001, 1000.);
		}
	}
};



class Cyclo {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(3);
		T X = x(0);
		T Y = x(1);
		T z = x(2);

		y(0) = -Y*Y*z*z - Y*Y + 24.*Y*z - z*z - 13.;
		y(1) = -X*X*z*z - X*X + 24.*X*z - z*z - 13.;
		y(2) = -X*X*Y*Y - X*X + 24.*X*Y - Y*Y - 13.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(3);
		for (i=0; i<3; i++) {
			x(i).assign(0., 100000.);
		}
	}
};


class Eco9 {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(8);
		T x1 = x(0);
		T x2 = x(1);
		T x3 = x(2);
		T x4 = x(3);
		T x5 = x(4);
		T x6 = x(5);
		T x7 = x(6);
		T x8 = x(7);

		y(0) = x1 + x2*(x1 + x3) + x4*(x3 + x5) + x6*(x5 + x7) - ( x8*((1./8.) - x7));
		y(1) = x2 + x3*(x1 + x5) + x4*(x2 + x6) + x5*x7 - ( x8*((2./8.) - x6));
		y(2) = x3*(1. + x6) + x4*(x1 + x7) + x2*x5 -( x8*((3./8.) - x5));
		y(3) = x4 + x1*x5 + x2*x6 + x3*x7 -( x8*((4./8.) - x4));
		y(4) = x5 + x1*x6 + x2*x7 -( x8*((5./8.) - x3));
		y(5) = x6 + x1*x7 -( x8*((6./8.) - x2));
		y(6) = x7 -( x8*((7./8.) - x1));
		y(7) = x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 +1.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(8);
		for (i=0; i<8; i++) {
			x(i).assign(-100., 100.);
		}
	}
};


class Freudenstein {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(2);

		y(0) = -13. + x(0) + ((5.-x(1)) * x(1) - 2.) * x(1);
		y(1) = -29. + x(0) + ((x(1) + 1.) * x(1) - 14.) * x(1);

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(2);
		for (i=0; i<2; i++) {
			x(i).assign(-1e8, 1e8);
		}
	}
};


class Geneig {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(6);
		T x1 = x(0);
		T x2 = x(1);
		T x3 = x(2);
		T x4 = x(3);
		T x5 = x(4);
		T x6 = x(5);

		y(0) = -10.*x1*x6*x6 + 2.*x2*x6*x6 - x3*x6*x6 + x4*x6*x6 + 3.*x5*x6*x6 + x1*x6 + 2.*x2*x6 + x3*x6 + 2.*x4*x6 + x5*x6 + 10.*x1 + 2.*x2 - x3 + 2.*x4 - 2.*x5;

		y(1) = 2.*x1*x6*x6 - 11.*x2*x6*x6 + 2.*x3*x6*x6 - 2.*x4*x6*x6 + x5*x6*x6 + 2.*x1*x6 + x2*x6 + 2.*x3*x6 + x4*x6 + 3.*x5*x6 + 2.*x1+ 9.*x2 + 3.*x3 - x4 - 2.*x5;

		y(2) = -x1*x6*x6 + 2.*x2*x6*x6 - 12.*x3*x6*x6 - x4*x6*x6 + x5*x6*x6 + x1*x6 + 2.*x2*x6 - 2.*x4*x6 - 2.*x5*x6 - x1 + 3.*x2 + 10.*x3+ 2.*x4 - x5;

		y(3) = x1*x6*x6 - 2.*x2*x6*x6 - x3*x6*x6 - 10.*x4*x6*x6 + 2.*x5*x6*x6 + 2.*x1*x6 + x2*x6 - 2.*x3*x6 + 2.*x4*x6 + 3.*x5*x6 + 2.*x1 - x2 + 2.*x3 + 12.*x4 + x5;

		y(4) = 3.*x1*x6*x6 + x2*x6*x6 + x3*x6*x6 + 2.*x4*x6*x6 - 11.*x5*x6*x6 + x1*x6 + 3.*x2*x6 - 2.*x3*x6 + 3.*x4*x6 + 3.*x5*x6 - 2.*x1 - 2.*x2 - x3 + x4 + 10.*x5;

		y(5) = x1+x2+x3+x4+x5-1.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(6);
		for (i=0; i<6; i++) {
			x(i).assign(-1e8, 1e8);
		}
	}
};


class Geneig2 {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(6);
		T x1 = x(0);
		T x2 = x(1);
		T x3 = x(2);
		T x4 = x(3);
		T x5 = x(4);
		T x6 = x(5);

		T x62 = x6*x6;

		y(0) = (-10.*x1 + 2.*x2 - x3 + x4 + 3.*x5) * x62 + (x1 + 2.*x2 + x3 + 2.*x4 + x5) * x6 + 10.*x1 + 2.*x2 - x3 + 2.*x4 - 2.*x5;

		y(1) = (2.*x1 - 11.*x2 + 2.*x3 - 2.*x4 + x5) * x62 + (2.*x1 + x2 + 2.*x3 + x4 + 3.*x5) * x6 + 2.*x1+ 9.*x2 + 3.*x3 - x4 - 2.*x5;

		y(2) = (-x1 + 2.*x2 - 12.*x3 - x4 + x5) * x62 + (x1 + 2.*x2 - 2.*x4 - 2.*x5) * x6 - x1 + 3.*x2 + 10.*x3+ 2.*x4 - x5;

		y(3) = (x1 - 2.*x2 - x3 - 10.*x4 + 2.*x5) * x62 + (2.*x1 + x2 - 2.*x3 + 2.*x4 + 3.*x5) * x6 + 2.*x1 - x2 + 2.*x3 + 12.*x4 + x5;

		y(4) = (3.*x1 + x2 + x3 + 2.*x4 - 11.*x5) * x62 + (x1 + 3.*x2 - 2.*x3 + 3.*x4 + 3.*x5) * x6 - 2.*x1 - 2.*x2 - x3 + x4 + 10.*x5;

		y(5) = x1+x2+x3+x4+x5-1.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(6);
		for (i=0; i<6; i++) {
			x(i).assign(-1e8, 1e8);
		}
	}
};


class Geneig3 {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(6);
		T x1 = x(0);
		T x2 = x(1);
		T x3 = x(2);
		T x4 = x(3);
		T x5 = x(4);
		T x6 = x(5);

		y(0) = ((-10.*x1 + 2.*x2 - x3 + x4 + 3.*x5) * x6 + x1 + 2.*x2 + x3 + 2.*x4 + x5) * x6 + 10.*x1 + 2.*x2 - x3 + 2.*x4 - 2.*x5;

		y(1) = ((2.*x1 - 11.*x2 + 2.*x3 - 2.*x4 + x5) * x6 + 2.*x1 + x2 + 2.*x3 + x4 + 3.*x5) * x6 + 2.*x1+ 9.*x2 + 3.*x3 - x4 - 2.*x5;

		y(2) = ((-x1 + 2.*x2 - 12.*x3 - x4 + x5) * x6 + x1 + 2.*x2 - 2.*x4 - 2.*x5) * x6 - x1 + 3.*x2 + 10.*x3+ 2.*x4 - x5;

		y(3) = ((x1 - 2.*x2 - x3 - 10.*x4 + 2.*x5) * x6 + 2.*x1 + x2 - 2.*x3 + 2.*x4 + 3.*x5) * x6 + 2.*x1 - x2 + 2.*x3 + 12.*x4 + x5;

		y(4) = ((3.*x1 + x2 + x3 + 2.*x4 - 11.*x5) * x6 + x1 + 3.*x2 - 2.*x3 + 3.*x4 + 3.*x5) * x6 - 2.*x1 - 2.*x2 - x3 + x4 + 10.*x5;

		y(5) = x1+x2+x3+x4+x5-1.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(6);
		for (i=0; i<6; i++) {
			x(i).assign(-1e8, 1e8);
		}
	}
};


class Himmelblau {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(2);

		y(0) = -42.* x(0) + 2.* x(1)*x(1) + 4.*x(0)*x(1) + 4.*x(0)*x(0)*x(0) - 14.;
		y(1) = -26.* x(1) + 2.* x(0)*x(0) + 4.*x(0)*x(1) + 4.*x(1)*x(1)*x(1) - 22.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(2);
		for (i=0; i<2; i++) {
			x(i).assign(-1e8, 1e8);
		}
	}
};


class Kincox {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(4);
		T c1, c2, s1, s2;
		c1 = x(0); c2 = x(1); s1 = x(2); s2 = x(3);

		y(0) = -1. + 6. * (c1*c2 - s1*s2) + 10.*c1;
		y(1) = -4. + 6. * (c1*s2 + c2*s1) + 10.*s1;
		y(2) = c1*c1 + s1*s1 - 1.;
		y(3) = c2*c2 + s2*s2 - 1.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(4);
		for (i=0; i<4; i++) {
			x(i).assign(-1., 1.);
		}
	}
};


class Redeco8 {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(8);
		T x1 = x(0);
		T x2 = x(1);
		T x3 = x(2);
		T x4 = x(3);
		T x5 = x(4);
		T x6 = x(5);
		T x7 = x(6);
		T u8 = x(7);

		y(0) = x1 + x1*x2 + x2*x3 + x3*x4 + x4*x5 + x5*x6 + x6*x7 - 1.*u8;
		y(1) = x2 + x1*x3 + x2*x4 + x3*x5 + x4*x6 + x5*x7 - 2.*u8;
		y(2) = x3 + x1*x4 + x2*x5 + x3*x6 + x4*x7 - 3.*u8;
		y(3) = x4 + x1*x5 + x2*x6 + x3*x7 - 4.*u8;
		y(4) = x5 + x1*x6 + x2*x7 - 5.*u8;
		y(5) = x6 + x1*x7 - 6.*u8;
		y(6) = x7 - 7.*u8;
		y(7) = x1 + x2 + x3 + x4 + x5 + x6 + x7 + 1.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(8);
		for (i=0; i<8; i++) {
			x(i).assign(-1e8, 1e8);
		}
	}
};


class Stenger {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(2);

		y(0) = x(0)*x(0) - 4.*x(1);
		y(1) = x(1)*x(1) - 2.*x(0) + 4.*x(1);

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(2);
		for (i=0; i<2; i++) {
			x(i).assign(-1e8, 1e8);
		}
	}
};


class Yamamura1 {
	template <class T> T g(T x){
		return 2.5 * x*x*x - 10.5 * x*x + 11.8 * x;
	}
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		int n = x.size();
		ub::vector<T> y(n);
		int i;
		T s;

		s = 0.;
		for (i=0; i<n; i++) s += x(i);

		for (i=0; i<n; i++) {
			y(i) = g(x(i)) + s - (i+1.);
		}

		return y;
	}

	template<class T>
	void range(int n, ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(n);
		for (i=0; i<n; i++) {
			x(i).assign(-1e8, 1e8);
		}
	}
};

class Math_Maple {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(2);
		T cx, cy, sx, sy;

		cx = cos(x(0)); cy = cos(x(1));
		sx = sin(x(0)); sy = sin(x(1));

		y(0) = -sx * cy - 2. * cx * sy;
		y(1) = -cx * sy - 2. * sx * cy;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(2);
		for (i=0; i<2; i++) {
			x(i).assign(0., kv::constants< kv::interval<T> >::pi().upper() * 2.);
		}
	}
};

class Collins {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(1);
		T x2, x3, x4, x5, x6;

		x2 = x(0) * x(0);
		x3 = x2 * x(0);
		x4 = x3 * x(0);
		x5 = x4 * x(0);
		x6 = x5 * x(0);

		y(0) = 3.9852 - 10.039 * x2 + 7.2338 * x4 - 1.17775 * x6
		       + (-8.8575*x(0) + 20.091 * x3 - 11.177 * x5) * sqrt(1. - x2);

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(1);
		x(0).assign(-1., 1.);
	}
};

class Math_Num1 {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(1);
		T ch, c;

		ch = cosh(x(0));
		c = cos(x(0));

		y(0) = 2.77675 * x(0) * x(0) * x(0) * (1. + c * ch)
		       - (sin(x(0)) * ch - sinh(x(0)) * c);

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(1);
		x(0).assign(-100., 100.);
	}
};

class Xu {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(2);

		y(0) = 2. * sin(x(0)) + 0.8 * cos(2. * x(0)) + 7. * sin(x(1)) - x(0);
		y(1) = 4. * sin(2. * x(0)) + 1.4 * sin(3. * x(1)) + 3.1 * cos(2. * x(1)) - x(1);

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(2);
		for (i=0; i<2; i++) {
			x(i).assign(-20., 20.);
		}
	}
};

class Box3 {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(3);

		y(0) = exp(-0.1 * x(0)) - exp(-0.1 * x(1)) - x(2) * (exp(-0.1) - exp(-1.));
		y(1) = exp(-0.2 * x(0)) - exp(-0.2 * x(1)) - x(2) * (exp(-0.2) - exp(-2.));
		y(2) = exp(-0.3 * x(0)) - exp(-0.3 * x(1)) - x(2) * (exp(-0.3) - exp(-3.));

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(3);
		x(0).assign(-100., 100.);
		x(1).assign(-100., 100.);
		x(2).assign(0.1, 100.);
	}
};

class Rump_univariate {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x){
		ub::vector<T> y(1);

		y(0) = exp(x(0)) - 2. * x(0) - 1.;

		return y;
	}

	template<class T>
	void range(ub::vector< kv::interval<T> >& x) {
		int i;

		x.resize(1);
		x(0).assign(-1e8, 1e8);
	}
};
