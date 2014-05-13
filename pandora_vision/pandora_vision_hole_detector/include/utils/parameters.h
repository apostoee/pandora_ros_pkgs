/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2014, P.A.N.D.O.R.A. Team.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Alexandros Philotheou, Manos Tsardoulias
 *********************************************************************/

#ifndef UTILS_PARAMETERS_H
#define UTILS_PARAMETERS_H

#include "utils/defines.h"
#include "utils/parameters.h"
#include <dynamic_reconfigure/server.h>
#include <pandora_vision_hole_detector/depth_cfgConfig.h>
#include <pandora_vision_hole_detector/hole_fusion_cfgConfig.h>
#include <pandora_vision_hole_detector/rgb_cfgConfig.h>

namespace pandora_vision
{
  /**
    @struct Parameters
    @brief Provides flexibility by parameterizing variables needed by the
    hole detector package
   **/
  struct Parameters
  {
    // Topics' names

    // The input, depth registered, point cloud topic
    static std::string hole_detector_input_topic;

    // The topic where the synchronizer node pubishes the
    // synchronized depth image, extracted from the input point cloud
    static std::string depth_image_topic;

    // The topic where the synchronizer node pubishes the
    // synchronized rgb image, extracted from the input point cloud
    static std::string rgb_image_topic;

    // The topic where the hole fusion publishes messages that unlock the
    // synchronizer node
    static std::string synchronizer_unlock_topic;

    // The topic where the synchronized node publishes the input
    // point cloud to the hole fusion node
    static std::string point_cloud_internal_topic;

    // The topic where the depth node publishes the candidate holes found
    static std::string depth_candidate_holes_topic;

    // The topic where the rgb node publishes the candidate holes found
    static std::string rgb_candidate_holes_topic;

    // The topic where the hole detector package publishes information
    // about the holes that considered valid
    static std::string hole_detector_output_topic;

    // The topic where the hole detector package publishes enhancement
    // information about the holes considered valid
    static std::string enhanced_holes_topic;

    // Show the depth image that arrives in the depth node
    static bool show_depth_image;

    // Show the rgb image that arrives in the rgb node
    static bool show_rgb_image;

    // Show the holes that each of the depth and RGB nodes transmit to the
    // hole fusion node, on top of their respective origin images
    static bool show_respective_holes;

    // The product of this package: valid holes
    static bool show_final_holes;

    // Depth and RGB images' representation method.
    // 0 if image used is used as obtained from the image sensor
    // 1 through wavelet analysis
    static int image_representation_method;

    // Edge detection parameters
    static int edge_detection_method;

    // When mixed edge detection is selected, this toggle switch
    // is needed in order to shift execution from one edge detector
    // to the other.
    // 1 for the Scharr edge detector,
    // 2 for the Sobel edge detector
    static int mixed_edges_toggle_switch;

    // canny parameters
    static int canny_ratio;
    static int canny_kernel_size;
    static int canny_low_threshold;
    static int canny_blur_noise_kernel_size;

    static float contrast_enhance_alpha;
    static float contrast_enhance_beta;

    // Threshold parameters
    static int threshold_lower_value;

    // Blob detection parameters
    static int blob_min_threshold;
    static int blob_max_threshold;
    static int blob_threshold_step;
    static int blob_min_area;
    static int blob_max_area;
    static double blob_min_convexity;
    static double blob_max_convexity;
    static double blob_min_inertia_ratio;
    static double blob_max_circularity;
    static double blob_min_circularity;
    static bool blob_filter_by_color;
    static bool blob_filter_by_circularity;

    // Bounding boxes parameters

    // The bounding box detection method
    // 0 for detecting by means of brushfire starting
    // from the keypoint of the blob
    // 1 for detecting by means of contours around the edges of the blob
    static int bounding_box_detection_method;

    // When using raycast instead of brushfire to find the (approximate here)
    // outline of blobs, raycast_keypoint_partitions dictates the number of
    // rays, or equivalently, the number of partitions in which the blob is
    // partitioned in search of the blob's borders
    static int raycast_keypoint_partitions;

    //<! Loose ends connection parameters
    static int AB_to_MO_ratio;
    static int minimum_curve_points;

    //// Interpolation parameters

    // The interpolation method for noise removal
    // 0 for averaging the pixel's neighbor values
    // 1 for brushfire near
    // 2 for brushfire far
    static int interpolation_method;

    // Hole checkers and their thresholds`
    static int run_checker_depth_diff;
    static float checker_depth_diff_threshold;

    static int run_checker_depth_area;
    static float checker_depth_area_threshold;

    static int run_checker_brushfire_outline_to_rectangle;
    static float checker_brushfire_outline_to_rectangle_threshold;

    static int run_checker_outline_of_rectangle;
    static float checker_outline_of_rectangle_threshold;

    static int run_checker_depth_homogeneity;
    static float checker_depth_homogeneity_threshold;

    static int rectangle_inflation_size;
    static float holes_gaussian_mean;
    static float holes_gaussian_stddev;

    static int run_checker_color_homogeneity;
    static float checker_color_homogeneity_threshold;

    static int run_checker_luminosity_diff;
    static float checker_luminosity_diff_threshold;

    static int run_checker_texture_diff;
    static float checker_texture_diff_threshold;

    static int run_checker_texture_backproject;
    static float checker_texture_backproject_threshold;

    // Plane detection
    static int segmentation_method;
    static int max_iterations;
    static double num_points_to_exclude;
    static double point_to_plane_distance_threshold;

    // Method to scale the CV_32FC1 image to CV_8UC1
    static int scale_method;

    // Debug
    static bool debug_show_find_holes;
    static int debug_show_find_holes_size;

    static bool debug_show_denoise_edges;
    static int debug_show_denoise_edges_size;

    static bool debug_print_connect_pairs;
    static bool debug_show_connect_pairs;
    static int debug_show_connect_pairs_size;

    static bool debug_show_get_shapes_clear_border;
    static int debug_show_get_shapes_clear_border_size;

    static bool debug_show_check_holes;
    static int debug_show_check_holes_size;

    static bool debug_show_merge_holes;
    static int debug_show_merge_holes_size;

    // Texture parameters
    // The threshold for texture matching
    static float match_texture_threshold;

    // Color homogeneity parameters
    static int num_bins_threshold;
    static int non_zero_points_in_box_blob_histogram;

    // Histogram parameters
    static int number_of_hue_bins;
    static int number_of_saturation_bins;
    static int number_of_value_bins;
    static int secondary_channel;


    // Holes connection - merger
    static float connect_holes_min_distance;
    static float connect_holes_max_distance;


    // Holes validity thresholds
    // Normal : when depth analysis is applicable
    static float holes_validity_threshold_normal;

    // Urgent : when depth analysis is not applicable, we can only rely
    // on RGB analysis
    static float holes_validity_threshold_urgent;

    // The depth sensor's horizontal field of view
    static float horizontal_field_of_view;

    // The depth sensor's vertical field of view
    static float vertical_field_of_view;

  };

} // namespace pandora_vision

#endif  // UTILS_PARAMETERS_H
