#include <gtest/gtest.h>

#include "utils/ray.h"

TEST(ray, shouldComputePositiveAt) {
	const ray r(point3(0, 0, 0), vec3(1, 2, 3));

	{
		const auto result = r.at(1);
		EXPECT_EQ(result.x(), 1);
		EXPECT_EQ(result.y(), 2);
		EXPECT_EQ(result.z(), 3);
	}

	{
		const auto result = r.at(-1);
		EXPECT_EQ(result.x(), -1);
		EXPECT_EQ(result.y(), -2);
		EXPECT_EQ(result.z(), -3);
	}
}
