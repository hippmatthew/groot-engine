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
    ge::vec2 exp2 = ge::vec2(num1, num2) + ge::vec2(num3, num4);
    ge::vec2 res2 = ge::vec2(num1 + num3, num2 + num4);

    ge::vec3 exp3 = ge::vec3(num1, num2, num3) + ge::vec3(num4, num1, num2);
    ge::vec3 res3 = ge::vec3(num1 + num4, num2 + num1, num3 + num2);

    ge::vec4 exp4 = ge::vec4(num1, num3, num3, num4) + ge::vec4(num2, num4, num2, num1);
    ge::vec4 res4 = ge::vec4(num1 + num2, num3 + num4, num3 + num2, num4 + num1);

    CHECK( tests::error(exp2, res2) <= tests::g_tolerance );
    CHECK( tests::error(exp3, res3) <= tests::g_tolerance );
    CHECK( tests::error(exp4, res4) <= tests::g_tolerance );
  }

  SECTION( "add_assignment" ) {
    ge::vec2 res2(num1, num2);
    ge::vec2 exp2(num1 + num3, num2 + num4);
    res2 += ge::vec2(num3, num4);

    ge::vec3 res3(num1, num2, num3);
    ge::vec3 exp3(num1 + num4, num2 + num1, num3 + num2);
    res3 += ge::vec3(num4, num1, num2);

    ge::vec4 res4(num1, num3, num3, num4);
    ge::vec4 exp4(num1 + num2, num3 + num4, num3 + num2, num4 + num1);
    res4 += ge::vec4(num2, num4, num2, num1);

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "subtract") {
    ge::vec2 res2 = ge::vec2(num1, num2) - ge::vec2(num3, num4);
    ge::vec2 exp2 = ge::vec2(num1 - num3, num2 - num4);

    ge::vec3 res3 = ge::vec3(num1, num2, num3) - ge::vec3(num4, num1, num2);
    ge::vec3 exp3 = ge::vec3(num1 - num4, num2 - num1, num3 - num2);

    ge::vec4 res4 = ge::vec4(num1, num3, num3, num4) - ge::vec4(num2, num4, num2, num1);
    ge::vec4 exp4 = ge::vec4(num1 - num2, num3 - num4, num3 - num2, num4 - num1);

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "subtract_assignment" ) {
    ge::vec2 res2(num1, num2);
    ge::vec2 exp2(num1 - num3, num2 - num4);
    res2 -= ge::vec2(num3, num4);

    ge::vec3 res3(num1, num2, num3);
    ge::vec3 exp3(num1 - num4, num2 - num1, num3 - num2);
    res3 -= ge::vec3(num4, num1, num2);

    ge::vec4 res4(num1, num3, num3, num4);
    ge::vec4 exp4(num1 - num2, num3 - num4, num3 - num2, num4 - num1);
    res4 -= ge::vec4(num2, num4, num2, num1);

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "scalar_multiplication" ) {
    ge::vec2 res2 = num1 * ge::vec2(num2, num3);
    ge::vec2 exp2 = ge::vec2(num1 * num2, num1 * num3);

    ge::vec3 res3 = num1 * ge::vec3(num2, num3, num4);
    ge::vec3 exp3 = ge::vec3(num1 * num2, num1 * num3, num1 * num4);

    ge::vec4 res4 = num1 * ge::vec4(num1, num2, num3, num4);
    ge::vec4 exp4 = ge::vec4(num1 * num1, num1 * num2, num1 * num3, num1 * num4);

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "scalar_multiplication_assignment" ) {
    ge::vec2 res2 = ge::vec2(num2, num3);
    ge::vec2 exp2 = ge::vec2(num1 * num2, num1 * num3);
    res2 *= num1;

    ge::vec3 res3 = ge::vec3(num2, num3, num4);
    ge::vec3 exp3 = ge::vec3(num1 * num2, num1 * num3, num1 * num4);
    res3 *= num1;

    ge::vec4 res4 = ge::vec4(num1, num2, num3, num4);
    ge::vec4 exp4 = ge::vec4(num1 * num1, num1 * num2, num1 * num3, num1 * num4);
    res4 *= num1;

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "scalar_division" ) {
    ge::vec2 res2 = ge::vec2(num2, num3) / num1;
    ge::vec2 exp2 = ge::vec2(num2 / num1, num3 / num1);

    ge::vec3 res3 = ge::vec3(num2, num3, num4) / num1;
    ge::vec3 exp3 = ge::vec3(num2 / num1, num3 / num1, num4 / num1);

    ge::vec4 res4 = ge::vec4(num1, num2, num3, num4) / num1;
    ge::vec4 exp4 = ge::vec4(num1 / num1, num2 / num1, num3 / num1, num4 / num1);

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "scalar_division_assignment" ) {
    ge::vec2 res2 = ge::vec2(num2, num3);
    ge::vec2 exp2 = ge::vec2(num2 / num1, num3 / num1);
    res2 /= num1;

    ge::vec3 res3 = ge::vec3(num2, num3, num4);
    ge::vec3 exp3 = ge::vec3(num2 / num1, num3 / num1, num4 / num1);
    res3 /= num1;

    ge::vec4 res4 = ge::vec4(num1, num2, num3, num4);
    ge::vec4 exp4 = ge::vec4(num1 / num1, num2 / num1, num3 / num1, num4 / num1);
    res4 /= num1;

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "scalar_product" ) {
    float res2 = ge::vec2(num1, num2) * ge::vec2(num3, num4);
    float exp2 = num1 * num3 + num2 * num4;

    float res3 = ge::vec3(num1, num2, num3) * ge::vec3(num4, num1, num2);
    float exp3 = num1 * num4 + num2 * num1 + num3 * num2;

    float res4 = ge::vec4(num1, num3, num3, num4) * ge::vec4(num2, num4, num2, num1);
    float exp4 = num1 * num2 + num3 * num4 + num3 * num2 + num4 * num1;

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "vector_product" ) {
    ge::vec3 u(num1, num2, num3);
    ge::vec3 v(num4, num1, num2);

    ge::vec3 res = u.cross(v);
    ge::vec3 exp(num2 * num2 - num3 * num1, num3 * num4 - num1 * num2, num1 * num1 - num2 * num4);

    float err = tests::error(res, exp);
    CHECK( err <= tests::g_tolerance );
  }

  SECTION( "comparison_operators" ) {
    while (num1 == num2)
      num2 = random();

    float smaller = num1 < num2 ? num1 : num2;
    float larger = num1 < num2 ? num2 : num1;

    ge::vec4 u(smaller), v(larger);

    CHECK( u.xy() == ge::vec2(smaller) );
    CHECK( u.xyz() == ge::vec3(smaller) );
    CHECK( u == ge::vec4(smaller) );

    CHECK( u.xy() != v.xy() );
    CHECK( u.xyz() != v.xyz() );
    CHECK( u != v );

    CHECK( u.xy() < v.xy() );
    CHECK( u.xyz() < v.xyz() );
    CHECK( u < v );

    CHECK( u.xy() <= ge::vec2(smaller) );
    CHECK( u.xy() <= v.xy() );
    CHECK( u.xyz() <= ge::vec3(smaller) );
    CHECK( u.xyz() <= v.xyz());
    CHECK( u <= ge::vec4(smaller) );
    CHECK( u <= v);

    CHECK( v.xy() > u.xy() );
    CHECK( v.xyz() > u.xyz());
    CHECK( v > u );

    CHECK( v.xy() >= ge::vec2(larger) );
    CHECK( v.xy() >= u.xy() );
    CHECK( v.xyz() >= ge::vec3(larger) );
    CHECK( v.xyz() >= u.xyz() );
    CHECK( v >= ge::vec4(larger) );
    CHECK( v >= u );
  }
}

TEST_CASE( "mat", "[unit][linalg]" ) {
  tests::Random random;

  float num1 = random();
  float num2 = random();
  float num3 = random();
  float num4 = random();

  SECTION( "add" ) {
    ge::mat2 p2(
      ge::vec2(num1, num3),
      ge::vec2(num3, num4)
    );

    ge::mat2 q2(
      ge::vec2(num2, num4),
      ge::vec2(num2, num1)
    );

    ge::mat2 exp2 = ge::mat2(
      ge::vec2(num1 + num2, num3 + num4),
      ge::vec2(num3 + num2, num4 + num1)
    );

    ge::mat3 p3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    );

    ge::mat3 q3(
      ge::vec3(num2, num3, num4),
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2)
    );

    ge::mat3 exp3 = ge::mat3(
      ge::vec3(num1 + num2, num2 + num3, num3 + num4),
      ge::vec3(num4 + num1, num1 + num2, num2 + num3),
      ge::vec3(num3 + num4, num4 + num1, num1 + num2)
    );

    ge::mat4 p4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num2, num1),
      ge::vec4(num4, num1, num2, num3)
    );

    ge::mat4 q4(
      ge::vec4(num4, num3, num2, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num1, num2, num3, num4)
    );

    ge::mat4 exp4(
      ge::vec4(num1 + num4, num2 + num3, num3 + num2, num4 + num1),
      ge::vec4(num2 + num3, num3 + num4, num4 + num1, num1 + num2),
      ge::vec4(num3 + num2, num4 + num3, num2 + num4, num1 + num1),
      ge::vec4(num4 + num1, num1 + num2, num2 + num3, num3 + num4)
    );

    CHECK( tests::error(p2 + q2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(p3 + q3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(p4 + q4, exp4) <= tests::g_tolerance );
  }

  SECTION( "add_assignment" ) {
    ge::mat2 res2(
      ge::vec2(num1, num3),
      ge::vec2(num3, num4)
    );

    ge::mat2 q2(
      ge::vec2(num2, num4),
      ge::vec2(num2, num1)
    );

    ge::mat2 exp2 = ge::mat2(
      ge::vec2(num1 + num2, num3 + num4),
      ge::vec2(num3 + num2, num4 + num1)
    );

    ge::mat3 res3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    );

    ge::mat3 q3(
      ge::vec3(num2, num3, num4),
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2)
    );

    ge::mat3 exp3 = ge::mat3(
      ge::vec3(num1 + num2, num2 + num3, num3 + num4),
      ge::vec3(num4 + num1, num1 + num2, num2 + num3),
      ge::vec3(num3 + num4, num4 + num1, num1 + num2)
    );

    ge::mat4 res4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num2, num1),
      ge::vec4(num4, num1, num2, num3)
    );

    ge::mat4 q4(
      ge::vec4(num4, num3, num2, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num1, num2, num3, num4)
    );

    ge::mat4 exp4(
      ge::vec4(num1 + num4, num2 + num3, num3 + num2, num4 + num1),
      ge::vec4(num2 + num3, num3 + num4, num4 + num1, num1 + num2),
      ge::vec4(num3 + num2, num4 + num3, num2 + num4, num1 + num1),
      ge::vec4(num4 + num1, num1 + num2, num2 + num3, num3 + num4)
    );

    res2 += q2;
    res3 += q3;
    res4 += q4;

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "subtract" ) {
    ge::mat2 p2(
      ge::vec2(num1, num3),
      ge::vec2(num3, num4)
    );

    ge::mat2 q2(
      ge::vec2(num2, num4),
      ge::vec2(num2, num1)
    );

    ge::mat2 exp2 = ge::mat2(
      ge::vec2(num1 - num2, num3 - num4),
      ge::vec2(num3 - num2, num4 - num1)
    );

    ge::mat3 p3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    );

    ge::mat3 q3(
      ge::vec3(num2, num3, num4),
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2)
    );

    ge::mat3 exp3 = ge::mat3(
      ge::vec3(num1 - num2, num2 - num3, num3 - num4),
      ge::vec3(num4 - num1, num1 - num2, num2 - num3),
      ge::vec3(num3 - num4, num4 - num1, num1 - num2)
    );

    ge::mat4 p4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num4, num1, num2, num3)
    );

    ge::mat4 q4(
      ge::vec4(num4, num3, num2, num1),
      ge::vec4(num3, num2, num1, num4),
      ge::vec4(num2, num1, num4, num3),
      ge::vec4(num1, num4, num3, num2)
    );

    ge::mat4 exp4(
      ge::vec4(num1 - num4, num2 - num3, num3 - num2, num4 - num1),
      ge::vec4(num2 - num3, num3 - num2, num4 - num1, num1 - num4),
      ge::vec4(num3 - num2, num4 - num1, num1 - num4, num2 - num3),
      ge::vec4(num4 - num1, num1 - num4, num2 - num3, num3 - num2)
    );

    CHECK( tests::error(p2 - q2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(p3 - q3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(p4 - q4, exp4) <= tests::g_tolerance );
  }

  SECTION( "subtract_assignment" ) {
    ge::mat2 res2(
      ge::vec2(num1, num3),
      ge::vec2(num3, num4)
    );

    ge::mat2 q2(
      ge::vec2(num2, num4),
      ge::vec2(num2, num1)
    );

    ge::mat2 exp2 = ge::mat2(
      ge::vec2(num1 - num2, num3 - num4),
      ge::vec2(num3 - num2, num4 - num1)
    );

    ge::mat3 res3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    );

    ge::mat3 q3(
      ge::vec3(num2, num3, num4),
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2)
    );

    ge::mat3 exp3 = ge::mat3(
      ge::vec3(num1 - num2, num2 - num3, num3 - num4),
      ge::vec3(num4 - num1, num1 - num2, num2 - num3),
      ge::vec3(num3 - num4, num4 - num1, num1 - num2)
    );

    ge::mat4 res4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num2, num1),
      ge::vec4(num4, num1, num2, num3)
    );

    ge::mat4 q4(
      ge::vec4(num4, num3, num2, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num1, num2, num3, num4)
    );

    ge::mat4 exp4(
      ge::vec4(num1 - num4, num2 - num3, num3 - num2, num4 - num1),
      ge::vec4(num2 - num3, num3 - num4, num4 - num1, num1 - num2),
      ge::vec4(num3 - num2, num4 - num3, num2 - num4, num1 - num1),
      ge::vec4(num4 - num1, num1 - num2, num2 - num3, num3 - num4)
    );

    res2 -= q2;
    res3 -= q3;
    res4 -= q4;

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "matrix_multiplication" ) {
    ge::mat2 m2(num1);
    ge::mat2 exp2(2 * num1 * num1);

    ge::mat3 m3(num2);
    ge::mat3 exp3(3 * num2 * num2);

    ge::mat4 m4(num3);
    ge::mat4 exp4(4 * num3 * num3);

    CHECK( tests::error(m2 * m2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(m3 * m3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(m4 * m4, exp4) <= tests::g_tolerance );
  }

  SECTION( "matrix_multiplication_assignment" ) {
    ge::mat2 res2(num1);
    ge::mat2 exp2(2 * num1 * num1);
    res2 *= res2;

    ge::mat3 res3(num2);
    ge::mat3 exp3(3 * num2 * num2);
    res3 *= res3;

    ge::mat4 res4(num3);
    ge::mat4 exp4(4 * num3 * num3);
    res4 *= res4;

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "vector_multiplication" ) {
    ge::mat2 m2(
      ge::vec2(num1, num2),
      ge::vec2(num3, num4)
    );
    ge::vec2 v2(num1, num2);
    ge::vec2 exp2(num1 * num1 + num2 * num2, num3 * num1 + num4 * num2);

    ge::mat3 m3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    );
    ge::vec3 v3(num1, num2, num3);
    ge::vec3 exp3(
      num1 * num1 + num2 * num2 + num3 * num3,
      num4 * num1 + num1 * num2 + num2 * num3,
      num3 * num1 + num4 * num2 + num1 * num3
    );

    ge::mat4 m4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num4, num1, num2, num3)
    );
    ge::vec4 v4(num1, num2, num3, num4);
    ge::vec4 exp4(
      num1 * num1 + num2 * num2 + num3 * num3 + num4 * num4,
      num2 * num1 + num3 * num2 + num4 * num3 + num1 * num4,
      num3 * num1 + num4 * num2 + num1 * num3 + num2 * num4,
      num4 * num1 + num1 * num2 + num2 * num3 + num3 * num4
    );

    CHECK( tests::error(m2 * v2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(m3 * v3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(m4 * v4, exp4) <= tests::g_tolerance );
  }

  SECTION( "scalar_mult" ) {
    ge::mat2 res2 = ge::mat2(
      ge::vec2(num1, num2),
      ge::vec2(num3, num4)
    ) * num1;

    ge::mat2 exp2(
      ge::vec2(num1 * num1, num2 * num1),
      ge::vec2(num3 * num1, num4 * num1)
    );

    ge::mat3 res3 = ge::mat3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    ) * num1;

    ge::mat3 exp3(
      ge::vec3(num1 * num1, num2 * num1, num3 * num1),
      ge::vec3(num4 * num1, num1 * num1, num2 * num1),
      ge::vec3(num3 * num1, num4 * num1, num1 * num1)
    );

    ge::mat4 res4 = ge::mat4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num4, num1, num2, num3)
    ) * num1;

    ge::mat4 exp4(
      ge::vec4(num1 * num1, num2 * num1, num3 * num1, num4 * num1),
      ge::vec4(num2 * num1, num3 * num1, num4 * num1, num1 * num1),
      ge::vec4(num3 * num1, num4 * num1, num1 * num1, num2 * num1),
      ge::vec4(num4 * num1, num1 * num1, num2 * num1, num3 * num1)
    );

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "scalar_mult_assignment" ) {
    ge::mat2 res2(
      ge::vec2(num1, num2),
      ge::vec2(num3, num4)
    );
    res2 *= num1;

    ge::mat2 exp2(
      ge::vec2(num1 * num1, num2 * num1),
      ge::vec2(num3 * num1, num4 * num1)
    );

    ge::mat3 res3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    );
    res3 *= num1;

    ge::mat3 exp3(
      ge::vec3(num1 * num1, num2 * num1, num3 * num1),
      ge::vec3(num4 * num1, num1 * num1, num2 * num1),
      ge::vec3(num3 * num1, num4 * num1, num1 * num1)
    );

    ge::mat4 res4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num4, num1, num2, num3)
    );
    res4 *= num1;

    ge::mat4 exp4(
      ge::vec4(num1 * num1, num2 * num1, num3 * num1, num4 * num1),
      ge::vec4(num2 * num1, num3 * num1, num4 * num1, num1 * num1),
      ge::vec4(num3 * num1, num4 * num1, num1 * num1, num2 * num1),
      ge::vec4(num4 * num1, num1 * num1, num2 * num1, num3 * num1)
    );

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "scalar_div" ) {
    ge::mat2 res2 = ge::mat2(
      ge::vec2(num1, num2),
      ge::vec2(num3, num4)
    ) / num1;

    ge::mat2 exp2(
      ge::vec2(num1 / num1, num2 / num1),
      ge::vec2(num3 / num1, num4 / num1)
    );

    ge::mat3 res3 = ge::mat3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    ) / num1;

    ge::mat3 exp3(
      ge::vec3(num1 / num1, num2 / num1, num3 / num1),
      ge::vec3(num4 / num1, num1 / num1, num2 / num1),
      ge::vec3(num3 / num1, num4 / num1, num1 / num1)
    );

    ge::mat4 res4 = ge::mat4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num4, num1, num2, num3)
    ) / num1;

    ge::mat4 exp4(
      ge::vec4(num1 / num1, num2 / num1, num3 / num1, num4 / num1),
      ge::vec4(num2 / num1, num3 / num1, num4 / num1, num1 / num1),
      ge::vec4(num3 / num1, num4 / num1, num1 / num1, num2 / num1),
      ge::vec4(num4 / num1, num1 / num1, num2 / num1, num3 / num1)
    );

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "scalar_div_assignment" ) {
    ge::mat2 res2(
      ge::vec2(num1, num2),
      ge::vec2(num3, num4)
    );
    res2 /= num1;

    ge::mat2 exp2(
      ge::vec2(num1 / num1, num2 / num1),
      ge::vec2(num3 / num1, num4 / num1)
    );

    ge::mat3 res3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    );
    res3 /= num1;

    ge::mat3 exp3(
      ge::vec3(num1 / num1, num2 / num1, num3 / num1),
      ge::vec3(num4 / num1, num1 / num1, num2 / num1),
      ge::vec3(num3 / num1, num4 / num1, num1 / num1)
    );

    ge::mat4 res4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num4, num1, num2, num3)
    );
    res4 /= num1;

    ge::mat4 exp4(
      ge::vec4(num1 / num1, num2 / num1, num3 / num1, num4 / num1),
      ge::vec4(num2 / num1, num3 / num1, num4 / num1, num1 / num1),
      ge::vec4(num3 / num1, num4 / num1, num1 / num1, num2 / num1),
      ge::vec4(num4 / num1, num1 / num1, num2 / num1, num3 / num1)
    );

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }

  SECTION( "comparison_operators" ) {
    while (num1 == num2)
      num2 = random();

    float smaller = num1 < num2 ? num1 : num2;
    float larger = num1 < num2 ? num2 : num1;

    ge::mat4 p(smaller), q(larger);

    CHECK( ge::mat2(p) == ge::mat2(smaller) );
    CHECK( ge::mat3(p) == ge::mat3(smaller) );
    CHECK( p == ge::mat4(smaller) );

    CHECK( ge::mat2(p) != ge::mat2(q) );
    CHECK( ge::mat3(p) != ge::mat3(q) );
    CHECK( p != q );

    CHECK( ge::mat2(p) < ge::mat2(q) );
    CHECK( ge::mat3(p) < ge::mat3(q) );
    CHECK( p < q );

    CHECK( ge::mat2(p) <= ge::mat2(smaller) );
    CHECK( ge::mat2(p) <= ge::mat2(q) );
    CHECK( ge::mat3(p) <= ge::mat3(smaller) );
    CHECK( ge::mat3(p) <= ge::mat3(q) );
    CHECK( p <= ge::mat4(smaller) );
    CHECK( p <= q );

    CHECK( ge::mat2(q) > ge::mat2(p) );
    CHECK( ge::mat3(q) > ge::mat3(p) );
    CHECK( q > p );

    CHECK( ge::mat2(q) >= ge::mat2(larger) );
    CHECK( ge::mat2(q) >= ge::mat2(p) );
    CHECK( ge::mat3(q) >= ge::mat3(larger) );
    CHECK( ge::mat3(q) >= ge::mat3(p) );
    CHECK( q >= ge::mat4(larger) );
    CHECK( q >= p );
  }

  SECTION( "identity" ) {
    ge::mat2 m2(
      ge::vec2(num1, num2),
      ge::vec2(num3, num4)
    );

    ge::mat3 m3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    );

    ge::mat4 m4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num4, num1, num2, num3)
    );

    CHECK( tests::error(ge::mat2<>::identity() * m2, m2) <= tests::g_tolerance );
    CHECK( tests::error(ge::mat3::identity() * m3, m3) <= tests::g_tolerance );
    CHECK( tests::error(ge::mat4::identity() * m4, m4) <= tests::g_tolerance );
  }

  SECTION( "rotate" ) {
    ge::vec3 x(1.0, 0.0, 0.0);
    ge::vec3 y(0.0, 1.0, 0.0);
    ge::vec3 z(0.0, 0.0, 1.0);

    ge::mat2 rot2D = ge::mat2<>::rotation(std::numbers::pi / 2);
    ge::mat3 xRot = ge::mat3::rotation(ge::vec3(std::numbers::pi / 2, 0.0, 0.0));
    ge::mat3 yRot = ge::mat3::rotation(ge::vec3(0.0, std::numbers::pi / 2, 0.0));
    ge::mat3 zRot = ge::mat3::rotation(ge::vec3(0.0, 0.0, std::numbers::pi / 2));

    ge::mat4 homoRot = ge::mat4::rotation(ge::vec3(0.0, 0.0, std::numbers::pi / 2));

    CHECK( tests::error(rot2D * x.xy(), y.xy()) <= tests::g_tolerance );
    CHECK( tests::error(xRot * y, z) <= tests::g_tolerance );
    CHECK( tests::error(yRot * x, -z) <= tests::g_tolerance );
    CHECK( tests::error(zRot * x, y) <= tests::g_tolerance );
    CHECK( tests::error(homoRot * ge::vec4(x, 1.0), ge::vec4(y, 1.0)) <= tests::g_tolerance );
  }

  SECTION( "scale" ) {
    ge::vec4 v(1.0);

    ge::mat2 scale2D = ge::mat2<>::scale(ge::vec2(num1, num2));
    ge::mat3 scale3D = ge::mat3::scale(ge::vec3(num1, num2, num3));
    ge::mat4 scaleHomo = ge::mat4::scale(ge::vec3(num1, num2, num3));

    CHECK( tests::error(scale2D * v.xy(), ge::vec2(num1, num2)) <= tests::g_tolerance );
    CHECK( tests::error(scale3D * v.xyz(), ge::vec3(num1, num2, num3)) <= tests::g_tolerance );
    CHECK( tests::error(scaleHomo * v, ge::vec4(num1, num2, num3, 1.0)) <= tests::g_tolerance );
  }

  SECTION( "translate" ) {
    ge::vec4 v(ge::vec3(0.0), 1.0);
    ge::mat4 t = ge::mat4::translation(ge::vec3(1.0, 1.0, 1.0));

    CHECK( tests::error(t * v, ge::vec4(1.0)) <= tests::g_tolerance );
  }

  SECTION( "determinant" ) {
    CHECK( tests::error(ge::mat2(1.0).determinant(), 0.0) <= tests::g_tolerance );
    CHECK( tests::error(ge::mat3(1.0).determinant(), 0.0) <= tests::g_tolerance );
    CHECK( tests::error(ge::mat4(1.0).determinant(), 0.0) <= tests::g_tolerance );
  }

  SECTION( "transpose" ) {
    ge::mat2 res2 = ge::mat2(
      ge::vec2(num1, num2),
      ge::vec2(num3, num4)
    ).transpose();

    ge::mat2 exp2(
      ge::vec2(num1, num3),
      ge::vec2(num2, num4)
    );

    ge::mat3 res3 = ge::mat3(
      ge::vec3(num1, num2, num3),
      ge::vec3(num4, num1, num2),
      ge::vec3(num3, num4, num1)
    ).transpose();

    ge::mat3 exp3(
      ge::vec3(num1, num4, num3),
      ge::vec3(num2, num1, num4),
      ge::vec3(num3, num2, num1)
    );

    ge::mat4 exp4 = ge::mat4(
      ge::vec4(num1, num2, num3, num4),
      ge::vec4(num2, num3, num4, num1),
      ge::vec4(num3, num4, num1, num2),
      ge::vec4(num4, num1, num2, num3)
    );

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(exp4.transpose(), exp4) <= tests::g_tolerance);
  }

  SECTION( "inverse" ) {
    CHECK( ge::mat2(1.0).inverse() == std::nullopt );
    CHECK( ge::mat3(1.0).inverse() == std::nullopt );
    CHECK( ge::mat4(1.0).inverse() == std::nullopt );
    CHECK( ge::mat2<>::identity().inverse() == ge::mat2<>::identity() );
    CHECK( ge::mat3::identity().inverse() == ge::mat3::identity() );
    CHECK( ge::mat4::identity().inverse() == ge::mat4::identity() );

    ge::mat2 res2 = ge::mat2(
      ge::vec2(1.0, 1.0),
      ge::vec2(-1.0, 1.0)
    ).inverse().value();

    ge::mat2 exp2(
      ge::vec2(0.5, -0.5),
      ge::vec2(0.5, 0.5)
    );

    ge::mat3 res3 = ge::mat3(
      ge::vec3(1.0, 2.0, 2.0),
      ge::vec3(-1.0, 1.0, 2.0),
      ge::vec3(-1.0, -1.0, 1.0)
    ).inverse().value();

    ge::mat3 exp3(
      ge::vec3(0.6, -0.8, 0.4),
      ge::vec3(-0.2, 0.6, -0.8),
      ge::vec3(0.4, -0.2, 0.6)
    );

    ge::mat4 res4 = ge::mat4(
      ge::vec4(1.0),
      ge::vec4(-1.0, 1.0, 1.0, 1.0),
      ge::vec4(ge::vec2(-1.0), 1.0, 1.0),
      ge::vec4(ge::vec3(-1.0), 1.0)
    ).inverse().value();

    ge::mat4 exp4(
      ge::vec4(0.5, -0.5, 0.0, 0.0),
      ge::vec4(0.0, 0.5, -0.5, 0.0),
      ge::vec4(0.0, 0.0, 0.5, -0.5),
      ge::vec4(0.5, 0.0, 0.0, 0.5)
    );

    CHECK( tests::error(res2, exp2) <= tests::g_tolerance );
    CHECK( tests::error(res3, exp3) <= tests::g_tolerance );
    CHECK( tests::error(res4, exp4) <= tests::g_tolerance );
  }
}