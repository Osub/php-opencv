#include "../../php_opencv.h"
#include "opencv_imgproc.h"
#include <opencv2/imgproc.hpp>
#include "core/opencv_mat.h"
#include "core/opencv_type.h"


void opencv_imgproc_init(int module_number)
{
    opencv_color_conversion_code_init(module_number);
    opencv_line_type_init(module_number);
}

/**
 * CV\cvtColor
 * @param execute_data
 * @param return_value
 */
PHP_FUNCTION(opencv_cv_t_color){
    long code;
    Mat dstImg;
    zval *mat_zval;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "Ol", &mat_zval,opencv_mat_ce, &code) == FAILURE) {
        RETURN_NULL();
    }
    //get src mat object from mat_zval
    opencv_mat_object *src_obj = Z_PHP_MAT_OBJ_P(mat_zval);
    cvtColor(*(src_obj->mat),dstImg,code);

    //new PHP Mat bing cv::cvtColor dstImg.
    zval instance;
    object_init_ex(&instance,opencv_mat_ce);
    opencv_mat_object *dst_obj = Z_PHP_MAT_OBJ_P(&instance);

    dst_obj->mat=new Mat(dstImg);

    //update php Mat object property
    zend_update_property_long(opencv_mat_ce, &instance, "rows", sizeof("rows")-1, dst_obj->mat->rows);
    zend_update_property_long(opencv_mat_ce, &instance, "cols", sizeof("cols")-1, dst_obj->mat->cols);
    zend_update_property_long(opencv_mat_ce, &instance, "type", sizeof("type")-1, dst_obj->mat->type());

    RETURN_ZVAL(&instance,0,0); //return php Mat object
}

/**
 * CV\ellipse
 * @param CV\Mat $img, Mat of original picture
 * @param CV\Point $center, Center Point of the ellipse
 * @param CV\Size $size, axes Half of the size of the ellipse main axes.
 * @param int $angle
 * @param int $startAngle
 * @param int $endAngle
 * @param Scalar $scalar
 * @param int thickness,
 * @param int lineType, line type:FILLED=-1,LINE_4=4,LINE_8=8,LINE_AA=16
 * @param int shift
 */
PHP_FUNCTION(opencv_ellipse){

    long angle, startAngle, endAngle, thickness = 1, lineType = LINE_8, shift = 0;
    zval *mat_zval, *point_zval, *size_zval, *scalar_zval;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "OOOlllO|lll",
                              &mat_zval, opencv_mat_ce,
                              &point_zval, opencv_point_ce,
                              &size_zval, opencv_size_ce,
                              &angle, &startAngle, &endAngle,
                              &scalar_zval, opencv_scalar_ce,
                              &thickness, &lineType, &shift) == FAILURE) {
        RETURN_NULL();
    }

    opencv_mat_object *mat_obj = Z_PHP_MAT_OBJ_P(mat_zval);
    opencv_point_object *point_obj = Z_PHP_POINT_OBJ_P(point_zval);
    opencv_size_object *size_obj = Z_PHP_SIZE_OBJ_P(size_zval);
    opencv_scalar_object *scalar_obj = Z_PHP_SCALAR_OBJ_P(scalar_zval);
    ellipse(*(mat_obj->mat), *(point_obj->point), *(size_obj->size), angle, startAngle, endAngle, *(scalar_obj->scalar), thickness, lineType ,shift);

    RETURN_NULL();
}

/**
 * CV\circle
 */
PHP_FUNCTION(opencv_circle){

    long radius, thickness = 1, lineType = LINE_8, shift = 0;
    zval *mat_zval, *point_zval, *size_zval, *scalar_zval;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "OOlO|lll",
                              &mat_zval, opencv_mat_ce,
                              &point_zval, opencv_point_ce,
                              &radius,
                              &scalar_zval, opencv_scalar_ce,
                              &thickness, &lineType, &shift) == FAILURE) {
        RETURN_NULL();
    }

    opencv_mat_object *mat_obj = Z_PHP_MAT_OBJ_P(mat_zval);
    opencv_point_object *point_obj = Z_PHP_POINT_OBJ_P(point_zval);
    opencv_scalar_object *scalar_obj = Z_PHP_SCALAR_OBJ_P(scalar_zval);

    circle(*(mat_obj->mat),*(point_obj->point), radius, *(scalar_obj->scalar),thickness,lineType ,shift);

    RETURN_NULL();
}


/**
 * color conversion code in CV\cvtColor,opencv enum ColorConversionCodes
 * @param module_number
 */
void opencv_color_conversion_code_init(int module_number){

    //!< add alpha channel to RGB or BGR image, equal to 0
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2BGRA", COLOR_BGR2BGRA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2RGBA", COLOR_RGB2RGBA, CONST_CS | CONST_PERSISTENT);

    //!< remove alpha channel from RGB or BGR image, equal to 1
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGRA2BGR", COLOR_BGRA2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGBA2RGB", COLOR_RGBA2RGB, CONST_CS | CONST_PERSISTENT);

    //!< convert between RGB and BGR color spaces (with or without alpha channel) ,equal to 2
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2RGBA", COLOR_BGR2RGBA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2BGRA", COLOR_RGB2BGRA, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGBA2BGR", COLOR_RGBA2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGRA2RGB", COLOR_BGRA2RGB, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2RGB", COLOR_BGR2RGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2BGR", COLOR_RGB2BGR, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGRA2RGBA", COLOR_BGRA2RGBA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGBA2BGRA", COLOR_RGBA2BGRA, CONST_CS | CONST_PERSISTENT);

    //!< convert between RGB/BGR and grayscale, @ref color_convert_rgb_gray "color conversions",equal to 6
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2GRAY", COLOR_BGR2GRAY, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2GRAY", COLOR_RGB2GRAY, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_GRAY2BGR", COLOR_GRAY2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_GRAY2RGB", COLOR_GRAY2RGB, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_GRAY2BGRA", COLOR_GRAY2BGRA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_GRAY2RGBA", COLOR_GRAY2RGBA, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGRA2GRAY", COLOR_BGRA2GRAY, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGBA2GRAY", COLOR_RGBA2GRAY, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2BGR565", COLOR_BGR2BGR565, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2BGR565", COLOR_RGB2BGR565, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR5652BGR", COLOR_BGR5652BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR5652RGB", COLOR_BGR5652RGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGRA2BGR565", COLOR_BGRA2BGR565, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR5652BGRA", COLOR_BGR5652BGRA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR5652RGBA", COLOR_BGR5652RGBA, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_GRAY2BGR565", COLOR_GRAY2BGR565, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR5652GRAY", COLOR_BGR5652GRAY, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2BGR555", COLOR_BGR2BGR555, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2BGR555", COLOR_RGB2BGR555, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR5552BGR", COLOR_BGR5552BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR5552RGB", COLOR_BGR5552RGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGRA2BGR555", COLOR_BGRA2BGR555, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGBA2BGR555", COLOR_RGBA2BGR555, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR5552BGRA", COLOR_BGR5552BGRA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR5552RGBA", COLOR_BGR5552RGBA, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_GRAY2BGR555", COLOR_GRAY2BGR555, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR5552GRAY", COLOR_BGR5552GRAY, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2XYZ", COLOR_BGR2XYZ, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2XYZ", COLOR_RGB2XYZ, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_XYZ2BGR", COLOR_XYZ2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_XYZ2RGB", COLOR_XYZ2RGB, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2YCrCb", COLOR_BGR2YCrCb, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2YCrCb", COLOR_RGB2YCrCb, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YCrCb2BGR", COLOR_YCrCb2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YCrCb2RGB", COLOR_YCrCb2RGB, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2HSV", COLOR_BGR2HSV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2HSV", COLOR_RGB2HSV, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2Lab", COLOR_BGR2Lab, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2Lab", COLOR_RGB2Lab, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2Luv", COLOR_BGR2Luv, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2Luv", COLOR_RGB2Luv, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2HLS", COLOR_BGR2HLS, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2HLS", COLOR_RGB2HLS, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_HSV2BGR", COLOR_HSV2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_HSV2RGB", COLOR_HSV2RGB, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_Lab2BGR", COLOR_Lab2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_Lab2RGB", COLOR_Lab2RGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_Luv2BGR", COLOR_Luv2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_Luv2RGB", COLOR_Luv2RGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_HLS2BGR", COLOR_HLS2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_HLS2RGB", COLOR_HLS2RGB, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2HSV_FULL", COLOR_BGR2HSV_FULL, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2HSV_FULL", COLOR_RGB2HSV_FULL, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2HLS_FULL", COLOR_BGR2HLS_FULL, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2HLS_FULL", COLOR_RGB2HLS_FULL, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_HSV2BGR_FULL", COLOR_HSV2BGR_FULL, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_HSV2RGB_FULL", COLOR_HSV2RGB_FULL, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_HLS2BGR_FULL", COLOR_HLS2BGR_FULL, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_HLS2RGB_FULL", COLOR_HLS2RGB_FULL, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_LBGR2Lab", COLOR_LBGR2Lab, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_LRGB2Lab", COLOR_LRGB2Lab, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_LBGR2Luv", COLOR_LBGR2Luv, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_LRGB2Luv", COLOR_LRGB2Luv, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_Lab2LBGR", COLOR_Lab2LBGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_Lab2LRGB", COLOR_Lab2LRGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_Luv2LBGR", COLOR_Luv2LBGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_Luv2LRGB", COLOR_Luv2LRGB, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2YUV", COLOR_BGR2YUV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2YUV", COLOR_RGB2YUV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR", COLOR_YUV2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB", COLOR_YUV2RGB, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_NV12", COLOR_YUV2RGB_NV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_NV12", COLOR_YUV2BGR_NV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_NV21", COLOR_YUV2RGB_NV21, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_NV21", COLOR_YUV2BGR_NV21, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV420sp2RGB", COLOR_YUV420sp2RGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV420sp2BGR", COLOR_YUV420sp2BGR, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_NV12", COLOR_YUV2RGBA_NV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_NV12", COLOR_YUV2BGRA_NV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_NV21", COLOR_YUV2RGBA_NV21, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_NV21", COLOR_YUV2BGRA_NV21, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV420sp2RGBA", COLOR_YUV420sp2RGBA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV420sp2BGRA", COLOR_YUV420sp2BGRA, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_YV12", COLOR_YUV2RGB_YV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_YV12", COLOR_YUV2BGR_YV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_IYUV", COLOR_YUV2RGB_IYUV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_IYUV", COLOR_YUV2BGR_IYUV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_I420", COLOR_YUV2RGB_I420, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_I420", COLOR_YUV2BGR_I420, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV420p2RGB", COLOR_YUV420p2RGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV420p2BGR", COLOR_YUV420p2BGR, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_YV12", COLOR_YUV2RGBA_YV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_YV12", COLOR_YUV2BGRA_YV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_IYUV", COLOR_YUV2RGBA_IYUV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_IYUV", COLOR_YUV2BGRA_IYUV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_I420", COLOR_YUV2RGBA_I420, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_I420", COLOR_YUV2BGRA_I420, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV420p2RGBA", COLOR_YUV420p2RGBA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV420p2BGRA", COLOR_YUV420p2BGRA, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_420", COLOR_YUV2GRAY_420, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_NV21", COLOR_YUV2GRAY_NV21, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_NV12", COLOR_YUV2GRAY_NV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_YV12", COLOR_YUV2GRAY_YV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_IYUV", COLOR_YUV2GRAY_IYUV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_I420", COLOR_YUV2GRAY_I420, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV420sp2GRAY", COLOR_YUV420sp2GRAY, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV420p2GRAY", COLOR_YUV420p2GRAY, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_UYVY", COLOR_YUV2RGB_UYVY, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_UYVY", COLOR_YUV2BGR_UYVY, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_Y422", COLOR_YUV2RGB_Y422, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_Y422", COLOR_YUV2BGR_Y422, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_UYNV", COLOR_YUV2RGB_UYNV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_UYNV", COLOR_YUV2BGR_UYNV, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_UYVY", COLOR_YUV2RGBA_UYVY, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_UYVY", COLOR_YUV2BGRA_UYVY, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_Y422", COLOR_YUV2RGBA_Y422, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_Y422", COLOR_YUV2BGRA_Y422, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_UYNV", COLOR_YUV2RGBA_UYNV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_UYNV", COLOR_YUV2BGRA_UYNV, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_YUY2", COLOR_YUV2RGB_YUY2, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_YUY2", COLOR_YUV2BGR_YUY2, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_YVYU", COLOR_YUV2RGB_YVYU, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_YVYU", COLOR_YUV2BGR_YVYU, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_YUYV", COLOR_YUV2RGB_YUYV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_YUYV", COLOR_YUV2BGR_YUYV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGB_YUNV", COLOR_YUV2RGB_YUNV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGR_YUNV", COLOR_YUV2BGR_YUNV, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_YUY2", COLOR_YUV2RGBA_YUY2, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_YUY2", COLOR_YUV2BGRA_YUY2, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_YVYU", COLOR_YUV2RGBA_YVYU, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_YVYU", COLOR_YUV2BGRA_YVYU, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_YUYV", COLOR_YUV2RGBA_YUYV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_YUYV", COLOR_YUV2BGRA_YUYV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2RGBA_YUNV", COLOR_YUV2RGBA_YUNV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2BGRA_YUNV", COLOR_YUV2BGRA_YUNV, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_UYVY", COLOR_YUV2GRAY_UYVY, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_YUY2", COLOR_YUV2GRAY_YUY2, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_Y422", COLOR_YUV2GRAY_Y422, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_UYNV", COLOR_YUV2GRAY_UYNV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_YVYU", COLOR_YUV2GRAY_YVYU, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_YUYV", COLOR_YUV2GRAY_YUYV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_YUV2GRAY_YUNV", COLOR_YUV2GRAY_YUNV, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGBA2mRGBA", COLOR_RGBA2mRGBA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_mRGBA2RGBA", COLOR_mRGBA2RGBA, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2YUV_I420", COLOR_RGB2YUV_I420, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2YUV_I420", COLOR_BGR2YUV_I420, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2YUV_IYUV", COLOR_RGB2YUV_IYUV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2YUV_IYUV", COLOR_BGR2YUV_IYUV, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGBA2YUV_I420", COLOR_RGBA2YUV_I420, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGRA2YUV_I420", COLOR_BGRA2YUV_I420, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGBA2YUV_IYUV", COLOR_RGBA2YUV_IYUV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGRA2YUV_IYUV", COLOR_BGRA2YUV_IYUV, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGB2YUV_YV12", COLOR_RGB2YUV_YV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGR2YUV_YV12", COLOR_BGR2YUV_YV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_RGBA2YUV_YV12", COLOR_RGBA2YUV_YV12, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BGRA2YUV_YV12", COLOR_BGRA2YUV_YV12, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerBG2BGR", COLOR_BayerBG2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGB2BGR", COLOR_BayerGB2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerRG2BGR", COLOR_BayerRG2BGR, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGR2BGR", COLOR_BayerGR2BGR, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerBG2RGB", COLOR_BayerBG2RGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGB2RGB", COLOR_BayerGB2RGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerRG2RGB", COLOR_BayerRG2RGB, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGR2RGB", COLOR_BayerGR2RGB, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerBG2GRAY", COLOR_BayerBG2GRAY, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGB2GRAY", COLOR_BayerGB2GRAY, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerRG2GRAY", COLOR_BayerRG2GRAY, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGR2GRAY", COLOR_BayerGR2GRAY, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerBG2BGR_VNG", COLOR_BayerBG2BGR_VNG, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGB2BGR_VNG", COLOR_BayerGB2BGR_VNG, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerRG2BGR_VNG", COLOR_BayerRG2BGR_VNG, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGR2BGR_VNG", COLOR_BayerGR2BGR_VNG, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerBG2RGB_VNG", COLOR_BayerBG2RGB_VNG, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGB2RGB_VNG", COLOR_BayerGB2RGB_VNG, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerRG2RGB_VNG", COLOR_BayerRG2RGB_VNG, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGR2RGB_VNG", COLOR_BayerGR2RGB_VNG, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerBG2BGR_EA", COLOR_BayerBG2BGR_EA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGB2BGR_EA", COLOR_BayerGB2BGR_EA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerRG2BGR_EA", COLOR_BayerRG2BGR_EA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGR2BGR_EA", COLOR_BayerGR2BGR_EA, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerBG2RGB_EA", COLOR_BayerBG2RGB_EA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGB2RGB_EA", COLOR_BayerGB2RGB_EA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerRG2RGB_EA", COLOR_BayerRG2RGB_EA, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_BayerGR2RGB_EA", COLOR_BayerGR2RGB_EA, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "COLOR_COLORCVT_MAX", COLOR_COLORCVT_MAX, CONST_CS | CONST_PERSISTENT);
}

void opencv_line_type_init(int module_number){
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "FILLED", FILLED, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "LINE_4", LINE_4, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "LINE_8", LINE_8, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "LINE_AA", LINE_AA, CONST_CS | CONST_PERSISTENT);
}