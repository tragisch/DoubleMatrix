#include "matrix.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

static void test_matric_create(void) {
  DoubleMatrix *mat = newDoubleMatrix();
  TEST_FAIL();
}

int main(void) {
  UnityBegin("matrix.c");

  RUN_TEST(test_matric_create);

  return UnityEnd();
}
