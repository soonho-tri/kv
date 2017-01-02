#include "interval.hpp"
#include "rdouble.hpp"
#include "dd.hpp"
#include "rdd.hpp"

typedef kv::dd dd;
typedef kv::interval<dd> idd;

int main()
{
	std::cout.precision(32);
	dd x, y, z;
	int i;

	x = 1.;
	y = 2.;
	z = x + y;
	std::cout << z << "\n";
	std::cout << z / 7. << "\n";
	std::cout << z / (-7.) << "\n";
	std::cout << sqrt(y) << "\n";
	std::cout << abs(z / (-7.)) << "\n";
	std::cout << floor(y + z / 7.) << "\n";
	std::cout << frexp(z / 7., &i) << "\n";
	std::cout << i << "\n";

	std::cout << idd(1.) / idd(10.) << "\n";
	std::cout << idd("0.1") << "\n";
	std::cout << exp(idd(0.125, 0.25)) << "\n";

	std::cout << (idd(0.) < (dd(1.))) << "\n";

	idd p, q, r;

	p = 1.;
	q = "0.1";
	std::cout << q << "\n";
	r = p * "0.1";
	std::cout << r << "\n";

	std::cout << (r < "0.100001") << "\n";
	std::cout << sqrt(dd(2.)) << "\n";
	std::cout << sqrt(idd(2.)) << "\n";
	std::cout << "11.8" * p << "\n";
}