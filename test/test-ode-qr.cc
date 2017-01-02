#include <iostream>
#include <limits>

#include <kv/ode-qr.hpp>


namespace ub = boost::numeric::ublas;

typedef kv::interval<double> itvd;


class Func {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x, T t){
		ub::vector<T> y(2);

		y(0) = x(1); y(1) = - x(0);

		return y;
	}
};

class Lorenz {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x, T t){
		ub::vector<T> y(3);

		y(0) = 10. * ( x(1) - x(0) );
		y(1) = 28. * x(0) - x(1) - x(0) * x(2);
		y(2) = (-8./3.) * x(2) + x(0) * x(1);

		return y;
	}
};

class VdP {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x, T t){
		ub::vector<T> y(2);

		y(0) = x(1);
		y(1) = 10000.* (1. - x(0)*x(0))*x(1) - x(0);

		return y;
	}
};

class Nobi {
	public:
	template <class T> ub::vector<T> operator() (ub::vector<T> x, T t){
		ub::vector<T> y(2);

		y(0) = x(1);
		y(1) = x(0) - x(0)*x(0)*x(0);

		return y;
	}
};

int main()
{
	int i;
	ub::vector<double> x;
	ub::vector<itvd> ix;
	ub::vector< kv::autodif<itvd> > dx;
	bool r;

	itvd end;

	x.resize(3);
	x(0) = 15.; x(1) = 15.; x(2) = 36.;

	std::cout.precision(17);

	ix = x;
	end = 1.;
	r = kv::odelong_qr(Lorenz(), ix, itvd(0.), end, 12, 2, 1);
	if (!r) {
		std::cout << "No Solution\n";
	} else {
		std::cout << ix << "\n";
		std::cout << end << "\n";
	}

	ix = x;
	dx = kv::autodif<itvd>::init(ix);
	end = 1.;
	r = kv::odelong_qr(Lorenz(), dx, itvd(0.), end, 12, 2, 1);
	if (!r) {
		std::cout << "No Solution\n";
	} else {
		std::cout << dx << "\n";
		std::cout << end << "\n";
	}
}