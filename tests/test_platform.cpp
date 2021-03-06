#include "test_platform.h"

#include "CL/cl.h"

START_TEST (test_get_platform_ids)
{
    cl_platform_id platform = 0;
    cl_uint num_platforms = 0;
    cl_int result = 0;
    
    result = clGetPlatformIDs(0, &platform, &num_platforms);
    fail_if(
        result != CL_INVALID_VALUE,
        "num_entries cannot be NULL when *platforms is provided"
    );
    
    result = clGetPlatformIDs(0, 0, 0);
    fail_if(
        result != CL_INVALID_VALUE,
        "Both num_platforms and *platforms cannot be NULL at the same time"
    );
    
    result = clGetPlatformIDs(1, 0, &num_platforms);
    fail_if(
        result != CL_SUCCESS || num_platforms == 0,
        "When *platforms is NULL, success and put the number of platforms in num_platforms"
    );
    
    result = clGetPlatformIDs(1, &platform, &num_platforms);
    fail_if(
        result != CL_SUCCESS,
        "It's bad to fail when the function is used in the most common sense"
    );
    fail_if(
        platform != 0,
        "Currently, we have only one platform, default, identified by NULL"
    );
}
END_TEST

START_TEST (test_get_platform_info)
{
    cl_platform_id platform = 0;
    cl_uint num_platforms = 0;
    cl_int result = 0;
    char *buf[100];
    size_t buf_len = 0;
    
    result = clGetPlatformIDs(1, &platform, &num_platforms);
    fail_if(
        result != CL_SUCCESS,
        "It's bad to fail when the function is used in the most common sense"
    );
    
    result = clGetPlatformInfo((_cl_platform_id *) -1, CL_PLATFORM_PROFILE, sizeof(buf), buf, &buf_len);
    fail_if(
        result != CL_INVALID_PLATFORM,
        "-1 is not a valid platform"
    );
    
    result = clGetPlatformInfo(platform, 1337, sizeof(buf), buf, &buf_len);
    fail_if(
        result != CL_INVALID_VALUE,
        "1337 is not a valid param_name"
    );
    
    result = clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, 0, buf, &buf_len);
    fail_if(
        result != CL_INVALID_VALUE,
        "param_value_size cannot be NULL when a buffer is supplied"
    );
    
    result = clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, 0, 0, &buf_len);
    fail_if(
        result != CL_SUCCESS || buf_len == 0,
        "We are allowed not to pass a buffer. The function must fill param_value_size_ret"
    );
    
    result = clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, sizeof(buf), buf, &buf_len);
    fail_if(
        result != CL_SUCCESS || buf_len == 0,
        "It's bad to fail when the function is used in the most common sense"
    );
}
END_TEST

TCase *cl_platform_tcase_create(void)
{
    TCase *tc = NULL;
    tc = tcase_create("platform");
    tcase_add_test(tc, test_get_platform_ids);
    tcase_add_test(tc, test_get_platform_info);
    return tc;
}
