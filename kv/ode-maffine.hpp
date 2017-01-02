/*
 * Copyright (c) 2013 Masahide Kashiwagi (kashi@waseda.jp)
 */

#ifndef ODE_MAFFINE_HPP
#define ODE_MAFFINE_HPP

// ODE using Affine and Mean Value Form

#include <iostream>
#include <list>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <kv/interval.hpp>
#include <kv/rdouble.hpp>
#include <kv/interval-vector.hpp>
#include <kv/affine.hpp>
#include <kv/ode.hpp>
#include <kv/ode-autodif.hpp>
#include <boost/tuple/tuple.hpp>

namespace ub = boost::numeric::ublas;

namespace kv {

template <class T, class F>
int
ode_maffine(F f, ub::vector< affine<T> >& init, const interval<T>& start, interval<T>& end, int order, bool autostep = true, int iter_max = 2, int ep_reduce = 0, int ep_reduce_limit = 0, ub::matrix< interval<T> >* mat = NULL, ub::vector< psa< interval<T> > >* result_psa = NULL)
{
	int n = init.size();
	int i, j;

	ub::vector< interval<T> > c;
	ub::vector< interval<T> > fc;
	ub::vector< interval<T> > I;
	ub::vector< autodif< interval<T> > > Iad;

	ub::vector< interval<T> > result_i;
	ub::matrix< interval<T> > result_d;

	ub::vector< affine<T> > result;

	ub::vector< psa< autodif< interval<T> > > > *result_tmp_p;
	ub::vector< psa< autodif< interval<T> > > > result_tmp;


	int maxnum_save;

	affine<T> s1, s2;
	interval<T> s2i;
	ub::vector< affine<T> > s1_save;
	ub::vector< interval<T> > s2i_save;

	int r;

	int ret_val;
	interval<T> end2 = end;

	I.resize(n);
	c.resize(n);
	for (i=0; i<n; i++) {
		I(i) = to_interval(init(i));
		c(i) = mid(I(i));
	}

	// result_psaの情報が必要なら準備
	if (result_psa != NULL) {
		result_tmp_p = &result_tmp;
	} else {
		result_tmp_p = NULL;
	}

	Iad = autodif< interval<T> >::init(I);
	// 自動的にautodif対応のものが呼ばれるはず
	r = ode(f, Iad, start, end2, order, autostep, iter_max, result_tmp_p);
	if (r == 0) return 0;
	ret_val = r;

	// result_tmpからautodif情報を外し、*result_psaに格納。
	if (result_psa != NULL) {
		for (i=0; i<n; i++) {
			for (j=0; j<result_tmp(i).v.size(); j++) {
				(*result_psa)(i).v(j) = result_tmp(i).v(j).v;
			}
		}
	}

	fc = c;
	// step幅は上のodeと同じでないとまずい。
	// 上が通れば条件の緩いこちらは通るかと思ったが、
	// 通らない例があった。本質的には通るはずなので、次数を
	// 増やしてでも無理矢理通す。
	int order2 = order;
	while (1) {
		r = ode(f, fc, start, end2, order2, false, iter_max);
		if (r != 0) break;
		order2++;
		std::cout << "increase order: " << order2 << "\n";
	}

	autodif< interval<T> >::split(Iad, result_i, result_d);

	if (ep_reduce == 0) {
		maxnum_save = affine<T>::maxnum();
	}

	result = fc + prod(result_d, init - c);

	if (ep_reduce == 0) {
		s1_save.resize(n);
		s2i_save.resize(n);
		for (i=0; i<n; i++) {
			split(result(i), maxnum_save, s1, s2);
			s2i = to_interval(s2);
			s1_save(i) = s1;
			s2i_save(i) = s2i;
		}

		affine<T>::maxnum() = maxnum_save;
		for (i=0; i<n; i++) {
			s1_save(i).resize();
			result(i) = append(s1_save(i), (affine<T>)s2i_save(i));
		}
	} else {
		epsilon_reduce(result, ep_reduce, ep_reduce_limit);
	}

	init = result;
	if (ret_val == 1) end = end2;
	if (mat != NULL) *mat = result_d;

	return ret_val;
}

template <class T, class F>
int
odelong_maffine(
	F f,
	ub::vector< affine<T> >& init,
	const interval<T>& start,
	interval<T>& end,
	int order,
	int iter_max = 2,
	int verbose = 0,
	int ep_reduce = 0,
	int ep_reduce_limit = 0,
	std::list< boost::tuple< interval<T>, interval<T>, ub::vector< psa< interval<T> > > > >* result_psa = NULL,
	ub::matrix< interval<T> >* mat = NULL
) {
	int s = init.size();
	ub::vector< affine<T> > x;
	interval<T> t, t1;
	int r;
	ub::matrix< interval<T> > M, M_tmp;
	ub::matrix< interval<T> >* M_p;
	int ret_val = 0;

	ub::vector< psa< interval<T> > > result_tmp;
	ub::vector< psa< interval<T> > >* result_tmp_p;


	if (mat == NULL) {
		M_p = NULL;
	} else {
		M_p = &M_tmp;
		M = ub::identity_matrix< interval<T> >(s);
	}

	if (result_psa != NULL) {
		result_tmp_p = &result_tmp;
	} else {
		result_tmp_p = NULL;
	}

	x = init;
	t = start;
	while (1) {
		t1 = end;

		r = ode_maffine(f, x, t, t1, order, true, iter_max, ep_reduce, ep_reduce_limit, M_p, result_tmp_p);
		if (r == 0) {
			if (ret_val == 1) {
				init = x;
				if (mat != NULL) *mat = M;
				end = t;
			}
			return ret_val;
		}
		ret_val = 1;
		if (mat != NULL) M = prod(M_tmp, M);
		if (result_psa != NULL) {
			(*result_psa).push_back(boost::make_tuple(t, t1, result_tmp));
		}
		if (verbose == 1) {
			std::cout << "t: " << t1 << "\n";
			std::cout << to_interval(x) << "\n";
		}
		if (r == 2) {
			init = x;
			if (mat != NULL) *mat = M;
			return 2;
		}
		t = t1;
	}
}

template <class T, class F>
int
odelong_maffine(
	F f,
	ub::vector< interval<T> >& init,
	const interval<T>& start,
	interval<T>& end,
	int order,
	int iter_max = 2,
	int verbose = 0,
	int ep_reduce = 0,
	int ep_reduce_limit = 0,
	std::list< boost::tuple< interval<T>, interval<T>, ub::vector< psa< interval<T> > > > >* result_psa = NULL
) {
	int s = init.size();
	int i;
	ub::vector< affine<T> > x;
	int maxnum_save;
	int r;
	interval<T> end2 = end;

	maxnum_save = affine<T>::maxnum();
	affine<T>::maxnum() = 0;

	x = init;

	r = odelong_maffine(f, x, start, end2, order, iter_max, verbose, ep_reduce, ep_reduce_limit, result_psa);

	affine<T>::maxnum() = maxnum_save;

	if (r == 0) return 0;

	for (i=0; i<s; i++) init(i) = to_interval(x(i));
	if (r == 1) end = end2;

	return r;
}

template <class T, class F>
int
odelong_maffine(
	F f,
	ub::vector< autodif< interval<T> > >& init,
	const interval<T>& start,
	interval<T>& end,
	int order,
	int iter_max = 2,
	int verbose = 0,
	int ep_reduce = 0,
	int ep_reduce_limit = 0,
	std::list< boost::tuple< interval<T>, interval<T>, ub::vector< psa< interval<T> > > > >* result_psa = NULL
) {
	int s = init.size();
	int i, j;
	ub::vector< interval<T> > xi;
	ub::vector< affine<T> > x;
	ub::matrix< interval<T> > M, M_tmp;
	int maxnum_save;
	int r;
	interval<T> end2 = end;

	autodif< interval<T> >::split(init, xi, M);
	int s2 = M.size2();

	maxnum_save = affine<T>::maxnum();
	affine<T>::maxnum() = 0;

	x = xi;

	r = odelong_maffine(f, x, start, end2, order, iter_max, verbose, ep_reduce, ep_reduce_limit, result_psa, &M_tmp);

	affine<T>::maxnum() = maxnum_save;

	if (r == 0) return 0;

	M = prod(M_tmp, M);

	for (i=0; i<s; i++) {
		init(i).v = to_interval(x(i));
		init(i).d.resize(s2);
		for (j=0; j<s2; j++) {
			init(i).d(j) = M(i, j);
		}
	}
	
	if (r == 1) end = end2;

	return r;
}

} // namespace kv

#endif // ODE_MAFFINE_HPP