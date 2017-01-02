/*
 * Copyright (c) 2013 Masahide Kashiwagi (kashi@waseda.jp)
 */

#ifndef ODE_AUTODIF_HPP
#define ODE_AUTODIF_HPP

// ODE (input and output : autodif type)

#include <iostream>
#include <cmath>
#include <limits>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <kv/interval.hpp>
#include <kv/rdouble.hpp>
#include <kv/interval-vector.hpp>
#include <kv/make-candidate.hpp>
#include <kv/psa.hpp>
#include <kv/autodif.hpp>


#ifndef ODE_FAST
#define ODE_FAST 1
#endif

#ifndef RESTART_MAX
#define RESTART_MAX 1
#endif

#ifndef TOL1
#define TOL1 0.1
#endif

#if 0
#ifndef TOL2
#define TOL2 100.
#endif

#ifndef TOL3
#define TOL3 0.01
#endif
#endif


namespace ub = boost::numeric::ublas;


namespace kv {


template <class T, class F>
int
ode(F f, ub::vector< autodif< interval<T> > >& init, const interval<T>& start, interval<T>& end, int order, bool autostep = true, int iter_max = 2, ub::vector< psa< autodif< interval<T> > > >* result_psa = NULL) {
	int n = init.size();
	int i, j, k, km;

	ub::vector< psa< autodif< interval<T> > > > x, y;
	psa< autodif< interval<T> > > torg;
	psa< autodif< interval<T> > > t;

	ub::vector< psa< autodif< interval<T> > > > z, w;
	autodif< interval<T> > wmz;
	autodif< interval<T> > evalz;

	psa< autodif< interval<T> > > temp;
	T m, m_tmp;
	ub::vector<T> newton_step;

	bool flag, resized;

	interval<T> deltat;
	ub::vector< autodif< interval<T> > > result, new_init;

	T radius, radius_tmp;
	T tolerance;
	int n_rad;

	int ret_val;
	interval<T> end2;
	int restart;

	ub::matrix< interval<T> > save;

	bool save_mode, save_uh, save_rh;

	new_init = autodif< interval<T> >::compress(init, save);

	m = std::numeric_limits<T>::epsilon();
	for (i=0; i<n; i++) {
		m_tmp = norm(new_init(i).v) * std::numeric_limits<T>::epsilon();
		if (m_tmp > m) m = m_tmp;
		m_tmp = rad(new_init(i).v) * TOL1;
		if (m_tmp > m) m = m_tmp;
		new_init(i).d.resize(n);
		for (j=0; j<n; j++) {
			m_tmp = norm(new_init(i).d(j)) * std::numeric_limits<T>::epsilon();
			m_tmp = rad(new_init(i).d(j)) * TOL1;
			if (m_tmp > m) m = m_tmp;
		}
	}
	tolerance = m;

	x = new_init;
	torg.v.resize(2);
	torg.v(0) = start; torg.v(1) = 1.;

	save_mode = psa< autodif< interval<T> > >::mode();
	save_uh = psa< autodif< interval<T> > >::use_history();
	save_rh = psa< autodif< interval<T> > >::record_history();
	psa< autodif< interval<T> > >::mode() = 1;
	psa< autodif< interval<T> > >::use_history() = false;
	psa< autodif< interval<T> > >::record_history() = false;
	#if ODE_FAST == 1
	psa< autodif< interval<T> > >::record_history() = true;
	psa< autodif< interval<T> > >::history().clear();
	#endif
	for (j=0; j<order; j++) {
		#if ODE_FAST == 1
		if (j == 1) psa< autodif< interval<T> > >::use_history() = true;
		#endif
		t = setorder(torg, j);
		y = f(x, t);
		for (i=0; i<n; i++) {
			y(i) = integrate(y(i));
			y(i) = setorder(y(i), j+1);
		}
		x = new_init + y;
	}

	if (autostep) {
		radius = 0.;
		n_rad = 0;
		for (j = order; j>=1; j--) {
			m = 0.;
			for (i=0; i<n; i++) {
				// m_tmp = norm(x(i).v(j).v);
				m_tmp = mid(x(i).v(j).v);
				if (m_tmp < 0.) m_tmp = -m_tmp;
				if (m_tmp > m) m = m_tmp;
				// 微分項は考慮しない手もある。
				#ifdef IGNORE_DIF_PART
				#else
				km = x(i).v(j).d.size();
				for (k=0; k<km; k++) {
					// m_tmp = norm(x(i).v(j).d(k));
					m_tmp = mid(x(i).v(j).d(k));
					if (m_tmp > m) m = m_tmp;
				}
				#endif
			}
			if (m == 0.) continue;
			radius_tmp = std::pow((double)m, 1./j);
			if (radius_tmp > radius) radius = radius_tmp;
			n_rad++;
			if (n_rad == 2) break;
		}
		radius = std::pow((double)tolerance, 1./order) / radius;
	}

	psa< autodif< interval<T> > >::mode() = 2;

	restart = 0;
	resized = false;

	while (true) {
		if (autostep) {
			end2 = mid(start + radius);
			if (end2 >= end.lower()) {
				end2 = end;
				ret_val = 2;
			} else {
				ret_val = 1;
			}
		} else {
			end2 = end;
			ret_val = 2;
		}
		deltat = end2 - start;

		psa< autodif< interval<T> > >::domain() = interval<T>(0., deltat.upper());

		z = x;
		t = setorder(torg, order);

		w = f(z, t);

		for (i=0; i<n; i++) {
			temp = integrate(w(i));
			w(i) = setorder(temp, order);
		}
		w = new_init + w;

		newton_step.resize(n + n * n);
		k = 0;
		for (i=0; i<n; i++) {
			wmz = w(i).v(order) - z(i).v(order);
			newton_step(k++) = norm(wmz.v);
			km = wmz.d.size();
			for (j=0; j<km; j++) {
				newton_step(k++) = norm(wmz.d(j));
			}
			for (j=km; j<n; j++) newton_step(k++) = 0.;
		}
		make_candidate(newton_step);
		k = 0;
		for (i=0; i<n; i++) {
			z(i).v(order).v += newton_step(k++) * interval<T>(-1., 1.);
			z(i).v(order).d.resize(n);
			for (j=0; j<n; j++) {
				z(i).v(order).d(j) += newton_step(k++) * interval<T>(-1., 1.);
			}
		}

		if (autostep && resized == false) {
			resized = true;
			m = 0.;
			for (i=0; i<n; i++) {
				evalz = eval(z(i), autodif< interval<T> >(deltat));
				m_tmp = rad(evalz.v) - rad(new_init(i).v);
				if (m_tmp > m) m = m_tmp;
				evalz.d.resize(n);
				for (j=0; j<n; j++) {
					m_tmp = rad(evalz.d(j)) - rad(new_init(i).d(j));
					if (m_tmp > m) m = m_tmp;
				}
			}
			m = m / tolerance;
			#if 0
			if (m > TOL2) m = TOL2;
			if (m < TOL3) m = TOL3;
			#endif
			radius /= std::pow((double)m, 1. / order);
			continue;
		}

		w = f(z, t);
		for (i=0; i<n; i++) {
			temp = integrate(w(i));
			w(i) = setorder(temp, order);
		}
		w = new_init + w;

		flag = true;
		for (i=0; i<n; i++) {
			flag = flag && subset(w(i).v(order).v, z(i).v(order).v);
			w(i).v(order).d.resize(n);
			for (j=0; j<n; j++) {
				flag = flag && subset(w(i).v(order).d(j), z(i).v(order).d(j));
			}
		}
		if (flag) break;

		if (!autostep || restart >= RESTART_MAX) {
			ret_val = 0;
			break;
		}
		radius *= 0.5;
		restart++;
	}

	if (ret_val != 0) {
		for (k=0; k<iter_max; k++) {
			z = w;
			w = f(z, t);
			for (i=0; i<n; i++) {
				temp = integrate(w(i));
				w(i) = setorder(temp, order);
			}
			w = new_init + w;
			for (i=0; i<n; i++) {
				w(i).v(order).v = intersect(w(i).v(order).v, z(i).v(order).v);
				w(i).v(order).d.resize(n);
				for (j=0; j<n; j++) {
					w(i).v(order).d(j) = intersect(w(i).v(order).d(j), z(i).v(order).d(j));
				}
			}
		}

		for (i=0; i<n; i++) {
			for (j=0; j<=order; j++) {
				w(i).v(j).d.resize(n);
				w(i).v(j) = autodif< interval<T> >::expand(w(i).v(j), save);
			}
		}

		result.resize(n);
		for (i=0; i<n; i++) {
			result(i) = eval(w(i), (autodif< interval<T> >)deltat);
		}

		init = result;
		if (ret_val == 1) end = end2;
		if (result_psa != NULL) *result_psa = w;
	}

	psa< autodif< interval<T> > >::mode() = save_mode;
	psa< autodif< interval<T> > >::use_history() = save_uh;
	psa< autodif< interval<T> > >::record_history() = save_rh;

	return ret_val;
}


template <class T, class F>
int
odelong(F f, ub::vector< autodif< interval<T> > >& init, const interval<T>& start, interval<T>& end, int order, int iter_max = 2, int verbose = 0) {

	ub::vector< autodif < interval<T> > > x;
	interval<T> t, t1;
	int r;
	int ret_val = 0;

	x = init;
	t = start;
	while (1) {
		t1 = end;

		r = ode(f, x, t, t1, order, true, iter_max);
		if (r == 0) {
			if (ret_val == 1) {
				init = x;
				end = t;
			}
			return ret_val;
		}
		ret_val = 1;
		if (verbose == 1) {
			std::cout << "t: " << t1 << "\n";
			std::cout << x << "\n";
		}
		if (r == 2) {
			init = x;
			return 2;
		}
		t = t1;
	}
}

} // namespace kv

#endif // ODE_AUTODIF_HPP