#include "gtest/gtest.h"

#include "vec3.h"

TEST(vec3, shouldCreateDefault) {
	const vec3 v;

	EXPECT_EQ(v.e[0], 0);
	EXPECT_EQ(v.e[1], 0);
	EXPECT_EQ(v.e[2], 0);
}

TEST(vec3, shouldCreateCustom) {
	const vec3 v{1, 2, 3};

	EXPECT_EQ(v.e[0], 1);
	EXPECT_EQ(v.e[1], 2);
	EXPECT_EQ(v.e[2], 3);
}

TEST(vec3, shouldAccessThroughGetters) {
	const vec3 v{1, 2, 3};

	EXPECT_EQ(v.x(), 1);
	EXPECT_EQ(v.y(), 2);
	EXPECT_EQ(v.z(), 3);
}

TEST(vec3, shouldAddWithOther) {
	vec3 v{1, 2, 3};
	const vec3 u{1, 2, 3};

	v += u;

	EXPECT_EQ(v.x(), 2);
	EXPECT_EQ(v.y(), 4);
	EXPECT_EQ(v.z(), 6);
}

TEST(vec3, shouldSubtractWithOther) {
	vec3 v{1, 2, 3};
	const vec3 u{1, 2, 3};

	v = v - u;

	EXPECT_EQ(v.x(), 0);
	EXPECT_EQ(v.y(), 0);
	EXPECT_EQ(v.z(), 0);
}

TEST(vec3, shouldMultiplyWithOther) {
	vec3 v{1, 2, 3};
	const vec3 u{4, 5, 6};

	v = v * u;

	EXPECT_EQ(v.x(), 4);
	EXPECT_EQ(v.y(), 10);
	EXPECT_EQ(v.z(), 18);
}

TEST(vec3, shouldMultiplyScalar) {
	{
		vec3 v{1, 2, 3};

		v = 5 * v;

		EXPECT_EQ(v.x(), 5);
		EXPECT_EQ(v.y(), 10);
		EXPECT_EQ(v.z(), 15);
	}

	{
		vec3 v{1, 2, 3};

		v = v * 5;

		EXPECT_EQ(v.x(), 5);
		EXPECT_EQ(v.y(), 10);
		EXPECT_EQ(v.z(), 15);
	}

	{
		vec3 v{1, 2, 3};

		v *= 5;

		EXPECT_EQ(v.x(), 5);
		EXPECT_EQ(v.y(), 10);
		EXPECT_EQ(v.z(), 15);
	}
}

TEST(vec3, shouldDoValidDotProduct) {
	vec3 v{1, 2, 3};
	vec3 u{4, 5, 6};

	constexpr int expected = 1 * 4 + 2 * 5 + 3 * 6;

	{
		const auto result = dot(u, v);

		EXPECT_EQ(result, expected);
	}

	{
		const auto result = dot(v, u);

		EXPECT_EQ(result, expected);
	}
}

TEST(vec3, shouldDoValidCrossProduct) {
	vec3 v{1, 2, 3};
	vec3 u{4, 5, 6};

	const auto result = cross(u, v);
	const vec3 expected{5 * 3 - 6 * 2, 6 * 1 - 4 * 3, 4 * 2 - 5 * 1};

	EXPECT_EQ(result.x(), expected.x());
	EXPECT_EQ(result.y(), expected.y());
	EXPECT_EQ(result.z(), expected.z());
}