#include <iostream>
#include <kv/defint.hpp>

namespace ub = boost::numeric::ublas;

typedef kv::interval<double> itvd;

/*
 *  Knut Petras: "Principles of verified numerical integration".
 * QUADPACK returns wrong solution.
 */

class Petras {
	public:
	template <class T> T operator() (T x) {
		return 5. * sin(x) + (9.*x-4.)*(9*x-8.)*(3*x-4.)*(9.*x-10.)*(kv::constants<T>::pi() - 2.*x)/(1.+(90.*x-110.)*(90.*x-110.)*(90.*x-110.)*(90.*x-110.));
	}
};

/*
 *  http://www.ti3.tuhh.de/intlab/demos/html/dtaylor.html
 */

class DTaylor {
	public:
	template <class T> T operator() (T x) {
		static T pi(kv::constants<itvd>::pi());
		return sin(pi * x) - sin(x);
	}
};

class Sqrt {
	public:
	template <class T> T operator() (T x) {
		return sqrt(x);
	}
};

/*
 *  \int_{-1}^1 f(x)dx = pi - 2
 */

class Func1 {
	public:
	template <class T> T operator() (T x) {
		return (1. - x * x) / (1 + x * x);
	}
};

/* 
 * S.M. Rump: Verification methods: Rigorous results using floating-point arithmetic, Acta Numerica, 19, pp. 287-449, 2010
 * http://www.ti3.tu-harburg.de/paper/rump/Ru10.pdf
 * p.86
 */

class Rump1 {
	public:
	template <class T> T operator() (T x) {
		return sin(x + exp(x));
	}
};

class Nanbu {
	public:
	template <class T> T operator() (T x) {
		return log(cos(sqrt(6. * pow(x, 4) + 2. * pow(x, 3) + 1)) + x) / exp(sin(sqrt(pow(x, 2) + 4. * x)) + pow(x, 2));
	}
};


int main() {
	std::cout.precision(17);

	std::cout << kv::defint_autostep(Petras(), (itvd)0., kv::constants<itvd>::pi(), 12) << "\n";
	std::cout << kv::defint_autostep(DTaylor(), (itvd)0., (itvd)20., 12) << "\n";
	std::cout << kv::defint_autostep(Sqrt(), (itvd)"0.0001", (itvd)2., 12) << "\n";
	std::cout << kv::defint_autostep(Func1(), (itvd)(-1.), (itvd)1., 12) << "\n";
	std::cout << kv::defint_autostep(Rump1(), (itvd)(0.), (itvd)8., 12) << "\n";
	std::cout << kv::defint_autostep(Nanbu(), (itvd)(0.5), (itvd)4., 10) << "\n";
}
