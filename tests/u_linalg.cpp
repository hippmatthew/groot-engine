#include "src/include/linalg.hpp"
#include "tests/utility.hpp"

#include <catch2/catch_test_macros.hpp>

#include <numbers>

TEST_CASE( "vec", "[unit][linalg]" ) {
  tests::Random random;

  float num1 = random();
  float num2 = random();
  float num3 = random();
  float num4 = random();

  SECTION( "add" ) {
    CHECK( ge::vec<2>(num1, num2) + ge::vec<2>(num3, num4) == ge::vec<2>(num1 + num3, num2 + num4) );
  }

  SECTION( "add_assignment" ) {
    ge::vec<2> u(num1, num2);
    ge::vec<2> v(num3, num4);

    u += v;

    CHECK( u == ge::vec<2>(num1 + num3, num2 + num4) );
  }

  SECTION( "subtract") {
    CHECK( ge::vec<2>(num1, num2) - ge::vec<2>(num3, num4) == ge::vec<2>(num1 - num3, num2 - num4) );
  }

  SECTION( "subtract_assignment" ) {
    ge::vec<2> u(num1, num2);
    ge::vec<2> v(num3, num4);

    u -= v;

    CHECK( u == ge::vec<2>(num1 - num3, num2 - num4) );
  }

  SECTION( "scalar_multiplication" ) {
    CHECK( num1 * ge::vec<2>(num2,  num3) == ge::vec<2>(num1 * num2, num1 * num3) );
  }

  SECTION( "scalar_multiplication_assignment" ) {
    ge::vec<2> u(num1, num2);

    u *= num3;

    CHECK( u == ge::vec<2>(num3 * num1, num3 * num2) );
  }

  SECTION( "scalar_division" ) {
    double inv = 1 / num3;

    ge::vec<2> u = ge::vec<2>(num1, num2) / num3;
    ge::vec<2> v(num1 * inv, num2 * inv);

    float err = abs(abs(u[0]) - abs(v[0])) / abs(v[0]);
    CHECK( err < 0.01 );

    err = abs(abs(u[1]) - abs(v[1])) / abs(v[1]);
    CHECK( err < 0.01 );
  }

  SECTION( "scalar_division_assignment" ) {
    ge::vec<2> u(num1, num2);
    double inv = 1 / num3;

    u /= num3;
    ge::vec<2> v(num1 * inv, num2 * inv);

    float err = abs(abs(u[0]) - abs(v[0])) / abs(v[0]);
    CHECK( err < 0.01 );

    err = abs(abs(u[1]) - abs(v[1])) / abs(v[1]);
    CHECK( err < 0.01 );
  }

  SECTION( "scalar_product" ) {
    float res = ge::vec<2>(num1, num2) * ge::vec<2>(num3, num4);
    float expected = num1 * num3 + num2 * num4;

    float float_error = abs(res - expected) / abs(expected);

    CHECK( float_error < 0.01);
  }

  SECTION( "vector_product" ) {
    ge::vec<3> u(num1, num2, num3);
    ge::vec<3> v(num4, num1, num2);
    ge::vec<3> w(num2 * num2 - num3 * num1, num3 * num4 - num1 * num2, num1 * num1 - num2 * num4);

    CHECK( u.cross(v) == w );
  }

  SECTION( "comparison_operators" ) {
    ge::vec<2> u(1, 1);
    ge::vec<2> v(2, 2);

    CHECK( u == u );
    CHECK( u < v );
    CHECK( u <= u );
    CHECK( v > u );
    CHECK( v>= v );
  }

  SECTION( "zero" ) {
    CHECK( ge::vec<2>::zero() == ge::vec<2>(0) );
  }

  SECTION( "index_access" ) {
    ge::vec<4> u(num1, num2, num3, num4);

    CHECK( u[0] == num1 );
    CHECK( u[1] == num2 );
    CHECK( u[2] == num3 );
    CHECK( u[3] == num4 );
  }
}

TEST_CASE( "mat", "[unit][linalg]" ) {
  tests::Random random;

  float num1 = random();
  float num2 = random();
  float num3 = random();

  SECTION( "add" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num2, num3),
      ge::vec<2>(num1, num2)
    );

    ge::mat<2> r(
      ge::vec<2>(num1 + num2, num2 + num3),
      ge::vec<2>(num3 + num1, num1 + num2)
    );

    CHECK( p + q == r );
  }

  SECTION( "add_assignment" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num2, num3),
      ge::vec<2>(num1, num2)
    );

    ge::mat<2> r(
      ge::vec<2>(num1 + num2, num2 + num3),
      ge::vec<2>(num3 + num1, num1 + num2)
    );

    p += q;

    CHECK( p == r );
  }

  SECTION( "subtract" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num2, num3),
      ge::vec<2>(num1, num2)
    );

    ge::mat<2> r(
      ge::vec<2>(num1 - num2, num2 - num3),
      ge::vec<2>(num3 - num1, num1 - num2)
    );

    CHECK( p - q == r );
  }

  SECTION( "subtract_assignment" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num2, num3),
      ge::vec<2>(num1, num2)
    );

    ge::mat<2> r(
      ge::vec<2>(num1 - num2, num2 - num3),
      ge::vec<2>(num3 - num1, num1 - num2)
    );

    p -= q;

    CHECK( p == r );
  }

  SECTION( "matrix_multiplication" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num2, num3),
      ge::vec<2>(num1, num2)
    );

    ge::mat<2> r(
      ge::vec<2>(2 * num1 * num2, num1 * num3 + num2 * num2),
      ge::vec<2>(num3 * num2 + num1 * num1, num3 * num3 + num1 * num2)
    );

    ge::mat<2> m = p * q;

    for (unsigned int i = 0; i < 2; ++i) {
      for (unsigned int j = 0; j < 2; ++j) {
        float err = abs(abs(m[i][j]) - abs(r[i][j])) / abs(r[i][j]);
        CHECK( err < 0.01 );
      }
    }
  }

  SECTION( "matrix_multiplication_assignment" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num2, num3),
      ge::vec<2>(num1, num2)
    );

    ge::mat<2> r(
      ge::vec<2>(2 * num1 * num2, num1 * num3 + num2 * num2),
      ge::vec<2>(num3 * num2 + num1 * num1, num3 * num3 + num1 * num2)
    );

    p *= q;

    for (unsigned int i = 0; i < 2; ++i) {
      for (unsigned int j = 0; j < 2; ++j) {
        float err = abs(abs(p[i][j]) - abs(r[i][j])) / abs(r[i][j]);
        CHECK( err < 0.01 );
      }
    }
  }

  SECTION( "vector_multiplication" ) {
    ge::mat<2> m(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::vec<2> v(num2, num3);
    ge::vec<2> w = m * v;

    ge::vec<2> r(num1 * num2 + num2 * num3, num3 * num2 + num1 * num3);

    for (unsigned int i = 0; i < 2; ++i) {
      float err = abs(abs(w[i]) - abs(r[i])) / abs(r[i]);
      CHECK( err < 0.01 );
    }
  }

  SECTION( "scalar_mult" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num2 * num1, num2 * num2),
      ge::vec<2>(num2 * num3, num2 * num1)
    );

    CHECK( num2 * p == q );
  }

  SECTION( "scalar_mult_assignment" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num2 * num1, num2 * num2),
      ge::vec<2>(num2 * num3, num2 * num1)
    );

    p *= num2;

    CHECK( p == q );
  }

  SECTION( "scalar_div" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num1 / num2, num2 / num2),
      ge::vec<2>(num3 / num2, num1 / num2)
    );

    ge::mat<2> r = p / num2;

    for (unsigned int i = 0; i < 2; ++i) {
      for (unsigned int j = 0; j < 2; ++j) {
        float err = abs(abs(r[i][j]) - abs(q[i][j])) / abs(q[i][j]);
        CHECK( err < 0.01 );
      }
    }
  }

  SECTION( "scalar_div_assignment" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num1 / num2, num2 / num2),
      ge::vec<2>(num3 / num2, num1 / num2)
    );

    p /= num2;

    CHECK( p == q );
  }

  SECTION( "comparison_operators" ) {
    ge::mat<2> p(
      ge::vec<2>(1, 1),
      ge::vec<2>(1, 1)
    );

    ge::mat<2> q(
      ge::vec<2>(2, 2),
      ge::vec<2>(2, 2)
    );

    CHECK( p == p );
    CHECK( p < q );
    CHECK( p <= p );
    CHECK( q > p );
    CHECK( q >= q );
  }

  SECTION( "identity" ) {
    CHECK( ge::mat<2>::identity()[0][0] == 1.0f );
    CHECK( ge::mat<2>::identity()[1][1] == 1.0f );
  }

  SECTION( "rotate" ) {
    ge::vec<4> x(1, 0, 0, 1);
    ge::vec<4> y(0, 1, 0, 1);
    ge::mat<4> r = ge::mat<4>::rotate(ge::vec<3>(0, 0, std::numbers::pi / 2));

    x = r * x;
    y = r * y;

    float errX = abs(x[1] - 1);
    float errY = abs(abs(y[0]) - 1);

    CHECK( errX < 0.01 );
    CHECK( errY < 0.01 );
  }

  SECTION( "translate" ) {
    ge::vec<4> r = ge::vec<4>(0, 0, 0, 1);
    ge::mat<4> t = ge::mat<4>::translate(ge::vec<3>(1, 1, 1));

    r = t * r;

    CHECK( r == ge::vec<4>(1, 1, 1, 1) );
  }

  SECTION( "scale" ) {
    ge::vec<4> r = ge::vec<4>(1, 1, 1, 1);
    ge::mat<4> s = ge::mat<4>::scale(ge::vec<3>(1, 2, 3));

    r = s * r;

    CHECK( r == ge::vec<4>(1, 2, 3, 1) );
  }

  SECTION( "transpose" ) {
    ge::mat<2> p(
      ge::vec<2>(num1, num2),
      ge::vec<2>(num3, num1)
    );

    ge::mat<2> q(
      ge::vec<2>(num1, num3),
      ge::vec<2>(num2, num1)
    );

    CHECK( p.transpose() == q );
  }
}