/*
 * Copyright (c) 2013-2014 Masahide Kashiwagi (kashi@waseda.jp)
 */

#ifndef GAMMA_HPP
#define GAMMA_HPP

#include <kv/defint.hpp>
#include <kv/defint-singular.hpp>

namespace kv {

// gamma function

template <class TT> class Gamma {
	public:
	TT x;
	
	Gamma(TT x) : x(x) {}

	template <class T> T operator() (T t) {
		return pow(t, (T)x - 1) * exp(-t);
	}
};

class Gamma_nopower {
	public:
	template <class T> T operator() (T t) {
		return exp(-t);
	}
};

#define GAMMA_TH1 0.25
#define GAMMA_TH2 40.
#define GAMMA_ORDER 14

// 1 \le x \le 2
template <class T>
interval<T> gamma_r(const interval<T>& x) {
	interval<T> result;

	result = defint_power(Gamma_nopower(), interval<T>(0.), interval<T>(GAMMA_TH1), GAMMA_ORDER, x - 1);

	result += defint_autostep(Gamma< interval<T> >(x), interval<T>(GAMMA_TH1), interval<T>(GAMMA_TH2), GAMMA_ORDER);

	result += interval<T>(0., (exp(-interval<T>(GAMMA_TH2)) * (interval<T>(GAMMA_TH2) + 1.)).upper());

	return result;
}

template <class T>
interval<T> gamma_point(const interval<T>& x) {
	T y;
	int i;
	interval<T> r;

	y = floor(x.lower());

	if (y >= 2.) {
		r = gamma_r(x - (y - 1.));
		for (i=y-1; i>=1; i--) {
			r *= x - i;
		}
		return r;
	} else if (y <= 0.) {
		r = gamma_r(x - (y - 1.));
		for (i=0.; i>=y; i--) {
			r /= x - i;
		}
		return r;
	} else {
		return gamma_r(x);
	}
}

template <class T> interval<T> digamma_zero(T x);

template <class T>
interval<T> gamma(const interval<T>& x) {
	interval<T> tmp, m;
	// below is from http://oeis.org/A030169
	static const interval<T> m2 = constants< interval<T> >::str("1.4616321449683623412626595423257213284681962040064463512959884085987864403538018102430749927337255", "1.4616321449683623412626595423257213284681962040064463512959884085987864403538018102430749927337256");

	if (x.lower() > 0.) {
		m = digamma_zero(x.lower());
		if (zero_in(m)) { // error
			m = m2;
		}
		tmp = interval<T>::hull(gamma_point(interval<T>(x.lower())), gamma_point(interval<T>(x.upper())));
		if (overlap(x, m)) {
			return interval<T>::hull(tmp, gamma_point(m));
		} else {
			return tmp;
		} 
	}

	if (floor(x.lower()) != floor(x.upper())) {
		return interval<T>::whole();
	}
	if (floor(x.lower()) == x.lower()) {
		return interval<T>::whole();
	}

	m = digamma_zero(x.lower());
	if (zero_in(m)) { // error
		return gamma_point(x);
	}
	tmp = interval<T>::hull(gamma_point(interval<T>(x.lower())), gamma_point(interval<T>(x.upper())));
	if (overlap(x, m)) {
		return interval<T>::hull(tmp, gamma_point(m));
	} else {
		return tmp;
	} 
}

template <class T>
interval<T> lgamma_point(const interval<T>& x) {
	T y;
	int i;
	interval<T> r;

	y = floor(x.lower());

	if (y >= 2.) {
		r = log(gamma_r(x - (y - 1.)));
		for (i=y-1; i>=1; i--) {
			r += log(x - i);
		}
		return r;
	} else if (y <= 0.) {
		r = log(gamma_r(x - (y - 1.)));
		for (i=0.; i>=y; i--) {
			r -= log(abs(x - i));
		}
		return r;
	} else {
		return log(gamma_r(x));
	}
}

template <class T>
interval<T> lgamma(const interval<T>& x) {
	interval<T> tmp, m;
	// below is from http://oeis.org/A030169
	static const interval<T> m2 = constants< interval<T> >::str("1.4616321449683623412626595423257213284681962040064463512959884085987864403538018102430749927337255", "1.4616321449683623412626595423257213284681962040064463512959884085987864403538018102430749927337256");

	if (x.lower() > 0.) {
		m = digamma_zero(x.lower());
		if (zero_in(m)) { // error
			m = m2;
		}
		tmp = interval<T>::hull(lgamma_point(interval<T>(x.lower())), lgamma_point(interval<T>(x.upper())));
		if (overlap(x, m)) {
			return interval<T>::hull(tmp, lgamma_point(m));
		} else {
			return tmp;
		} 
	}

	if (floor(x.lower()) != floor(x.upper())) {
		return interval<T>::whole();
	}
	if (floor(x.lower()) == x.lower()) {
		return interval<T>::whole();
	}

	m = digamma_zero(x.lower());
	if (zero_in(m)) { // error
		return lgamma_point(x);
	}
	tmp = interval<T>::hull(lgamma_point(interval<T>(x.lower())), lgamma_point(interval<T>(x.upper())));
	if (overlap(x, m)) {
		return interval<T>::hull(tmp, lgamma_point(m));
	} else {
		return tmp;
	} 
}


// digamma function

template <class TT> struct Digamma_0 {
	TT x;
	Digamma_0(TT x) : x(x) {}
	template <class T> T operator() (const T& t) {
		return div_tn(exp(-t) - exp(-x * t) / div_tn(1-exp(-t), 1), 1);
	}
};

template <class TT> struct Digamma {
	TT x;
	Digamma(TT x) : x(x) {}
	template <class T> T operator() (const T& t) {
		return (exp(-t) - exp(-x * t) / ((1-exp(-t)) / t)) / t;
	}
};

#define DIGAMMA_TH1 0.125
#define DIGAMMA_TH2 35
#define DIGAMMA_ORDER 14

// work for x > 0
// return high precision output if x is in [1,2]
template <class T> interval<T> digamma_plus(const interval<T>& x) {
	interval<T> result, tmp;
	
	result = defint_singular(Digamma_0< interval<T> >(x), (interval<T>)0., (interval<T>)DIGAMMA_TH1, DIGAMMA_ORDER);

	result += defint_autostep(Digamma< interval<T> >(x), (interval<T>)DIGAMMA_TH1, (interval<T>)DIGAMMA_TH2, DIGAMMA_ORDER);

	tmp = exp(- interval<T>(DIGAMMA_TH2));
	result += interval<T>::hull(- 1. / ((1. - tmp) * x) * exp(- x * DIGAMMA_TH2), tmp / DIGAMMA_TH2);

	return result;
}


/*
 * digamma for small interval
 */

template <class T> interval<T> digamma_point(const interval<T>& x) {
	T tmp;
	int i;
	interval<T> r;

	tmp = floor(x.lower());

	if (tmp == 1.) {
		return digamma_plus(x);
	} 

	r = digamma_plus(x - tmp + 1.);
	if (tmp > 1.) {
		for (i = tmp-1; i>=1; i--) {
			r += 1. / (x - i);
		}
	} else {
		for (i = tmp-1; i<=-1; i++) {
			r -= 1. / (x - i - 1.);
		}
	}

	return r;
}

template <class T> interval<T> digamma(const interval<T>& x) {
	if (x.lower() < 0. && floor(x.lower()) != floor(x.upper())) {
		return interval<T>::whole();
	}
	return interval<T>::hull(digamma_point(interval<T>(x.lower())), digamma_point(interval<T>(x.upper())));
}

// trigamma function

template <class TT> struct Trigamma_0 {
	TT x;
	Trigamma_0(TT x) : x(x) {}
	template <class T> T operator() (const T& t) {
		return exp(-x * t) / div_tn(1 - exp(-t), 1);
	}
};

template <class TT> struct Trigamma {
	TT x;
	Trigamma(TT x) : x(x) {}
	template <class T> T operator() (const T& t) {
		// return (t * exp(-x * t)) / (1 - exp(-t));
		return exp(-x * t) / ((1 - exp(-t)) / t);
	}
};

#define TRIGAMMA_TH1 0.125
#define TRIGAMMA_TH2 35
#define TRIGAMMA_ORDER 14

// work for x > 0
// return high precision output if x is in [1,2]
template <class T> interval<T> trigamma_plus(const interval<T>& x) {
	interval<T> result, tmp;
	
	result = defint_singular(Trigamma_0< interval<T> >(x), (interval<T>)0., (interval<T>)TRIGAMMA_TH1, TRIGAMMA_ORDER);

	result += defint_autostep(Trigamma< interval<T> >(x), (interval<T>)TRIGAMMA_TH1, (interval<T>)TRIGAMMA_TH2, TRIGAMMA_ORDER);

	tmp = DIGAMMA_TH2 * x;
	result += interval<T>::hull(0., exp(-tmp) * (tmp + 1) / (x * x));

	return result;
}

/*
 * trigamma for small interval
 */

template <class T> interval<T> trigamma_point(const interval<T>& x) {
	T tmp;
	int i;
	interval<T> r;

	tmp = floor(x.lower());

	if (tmp == 1.) {
		return trigamma_plus(x);
	} 

	r = trigamma_plus(x - tmp + 1.);
	if (tmp > 1.) {
		for (i = tmp-1; i>=1; i--) {
			r -= 1. / ((x - i) * (x - i));
		}
	} else {
		for (i = tmp-1; i<=-1; i++) {
			r += 1. / ((x - i - 1.) * (x - i - 1.));
		}
	}

	return r;
}

template <class T> interval<T> trigamma(const interval<T>& x) {
	if (x.lower() > 0.) {
		return interval<T>::hull(trigamma_point(interval<T>(x.upper())), trigamma_point(interval<T>(x.lower())));
	}
	if (floor(x.lower()) != floor(x.upper())) {
		return interval<T>(0., std::numeric_limits<T>::infinity());
	}
	if (floor(x.lower()) == x.lower()) {
		return interval<T>(0., std::numeric_limits<T>::infinity());
	}
	return trigamma_point(interval<T>(x));
}


// calculate zero of digamma using Newton's method and Krawczyk method
//   result is cached
//   returns [0,0] if cannot calculate verified solution

#define DIGAMMA_ZERO_MAX 300

template <class T> interval<T> digamma_zero(T x) {
	T d, d2, R;
	interval<T> I, K, fc, Rfc;
	int i, n;
	double dn;
	static bool is_calculated[DIGAMMA_ZERO_MAX + 1] = {};
	static interval<T> cache[DIGAMMA_ZERO_MAX + 1];

	if (x > 0.) {
		n = 0;
	} else {
		n = -(int)floor((double)x);
	}

	if (n <= DIGAMMA_ZERO_MAX && is_calculated[n] == true) {
		return cache[n];
	}

	// set initial value
	if (x > 0.) {
		x = 1.4616;
	} else {
		// this approximation is from Wikipedia:
		// http://en.wikipedia.org/wiki/Digamma_function
		// I cannot find the true source of this formula.
		dn = n;
		x = (T)(-dn + atan(M_PI / (log(dn) + 1/(8 * dn))) / M_PI);
	}

	for (i=0; i<15; i++) {
		d = mid(digamma(interval<T>(x))) / mid(kv::trigamma(interval<T>(x)));
		x -= d;
		// std::cout << i << ":" << x << "\n";
		using std::abs;
		if (abs(d) <= abs(x) * std::numeric_limits<T>::epsilon() * 3.) break;
	}

	R = 1. / mid(trigamma(interval<T>(x)));
	fc = digamma(interval<T>(x));
	Rfc = R * fc;
	d = 2. * mag(Rfc);
	using std::abs;
	d2 = abs(x) * std::numeric_limits<T>::epsilon() * 3.;
	if (d2 > d) d = d2;
	I = x + kv::interval<T>(-d, d);
	K = x - Rfc + (1 - R * trigamma(kv::interval<T>(I))) * (I - x);
	if (!proper_subset(K, I)) {
		K = 0.;
	}

	is_calculated[n] = true;
	cache[n] = K;

	return K;
}

} // namespace kv

#endif // GAMMA_HPP
