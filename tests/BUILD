load("//third_party/http/ThrowTheSwitch/Unity:unity.bzl", "unity_test")

unity_test(
    size = "small",
    cexception = False,
    file_name = "test_DoubleMatrix.c",
    deps = [
        "//lib/misc",
        "//src:dm_io",
        "//src:dm_math",
        "//src:dm_matrix",
    ],
)

unity_test(
    size = "small",
    cexception = False,
    file_name = "test_DoubleVector.c",
    deps = [
        "//lib/misc",
        "//src:dm_io",
        "//src:dm_math",
        "//src:dm_matrix",
    ],
)

# generate_a_unity_test_for_every_file(
#     name = "all",
#     cexception = False,
#     file_list = glob(["test_*.c"]),
#     deps = [
#         "//lib/misc",
#         "//src:dm_io",
#         "//src:dm_math",
#         "//src:dm_matrix",
#     ],
# )
