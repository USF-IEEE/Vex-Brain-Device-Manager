/**
 * @file TerriBull.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief TerriBull Single Robotics Library implementation designed to run on
 * the University of South Florida VEX BullBots seamlessly across all devices.
 *
 * @version 0.1
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __TERRIBULL__
#define __TERRIBULL__

#include "../pros/apix.h"
#include <list>
#include <vector>
#include <map>
#include <string>
#include <cmath>

#define MIN(a, b) (((a) < (b))? (a) : (b))
#define MAX(a, b) (((a) > (b))? (a) : (b))

class ConfigurationParser;
/**
 * @brief TerriBull Robotics V5 VEX Library Built on-top of PROS
*/
namespace TerriBull {
    /**
     * @brief TerriBull Robotics Utilities and Class Predeclarations
     *
     */
    
    extern pros::Imu mu;

    };
    #ifndef __TERRIBULL_INCLUDES__
    #define __TERRIBULL_INCLUDES__

    #include "../api.h"
    #endif

#endif