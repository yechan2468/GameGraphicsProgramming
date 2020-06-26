#ifndef HW3_SKELETON_H
#define HW3_SKELETON_H

#include "global.h"

const vector<string> jNames = {
        "Root",
        "Spine",
        "LThigh",
        "LCalf",
        "LFoot",
        "LToe0",
        "LToe0Nub",
        "RThigh",
        "RCalf",
        "RFoot",
        "RToe0",
        "RToe0Nub",
        "Spine1",
        "Neck",
        "Head",
        "HeadNub",
        "LClavicle",
        "LUpperArm",
        "LForearm",
        "LHand",
        "LFinger0",
        "LFinger0Nub",
        "RClavicle",
        "RUpperArm",
        "RForearm",
        "RHand",
        "RFinger0",
        "RFinger0Nub"
};


const vector<int> jParents = {
        -1,
        0,
        1,
        2,
        3,
        4,
        5,
        1,
        7,
        8,
        9,
        10,
        1,
        12,
        13,
        14,
        12,
        16,
        17,
        18,
        19,
        20,
        12,
        22,
        23,
        24,
        25,
        26
};

const vector<vec3> jOffset = {
        {0.000000f, 0.000008f, 0.000000f},
        {0.000000f, 10.168694f, -0.018712f},
        {8.844464f, -10.168707f, 0.018716f},
        {5.943175f, -46.621994f, -3.110340f},
        {3.939913f, -37.841415f, -7.384387f},
        {4.120415f, -10.932304f, 16.735615f},
        {1.377033f, 0.000000f, 6.009923f},
        {-8.844464f, -10.168707f, 0.018717f},
        {-5.943184f, -46.622002f, -3.110341f},
        {-3.939919f, -37.841408f, -7.384384f},
        {-4.120415f, -10.932304f, 16.735619f},
        {-1.377035f, 0.000000f, 6.009924f},
        {-0.000000f, 23.504765f, -3.057652f},
        {0.000000f, 22.894543f, -1.826048f},
        {0.000000f, 9.471176f, 1.317306f},
        {0.000000f, 23.197586f, -0.000031f},
        {8.784907f, 21.797897f, -4.749337f},
        {10.402839f, -4.881275f, 0.391647f},
        {12.499695f, -23.236025f, 2.804428f},
        {11.471571f, -22.124975f, 9.105472f},
        {3.267452f, -8.647340f, 6.064074f},
        {0.816860f, -2.161825f, 1.516018f},
        {-8.784905f, 21.797897f, -4.749337f},
        {-10.402844f, -4.881279f, 0.391648f},
        {-12.499710f, -23.236027f, 2.804428f},
        {-11.471552f, -22.124952f, 9.105466f},
        {-3.267452f, -8.647338f, 6.064076f},
        {-0.816853f, -2.161837f, 1.516022f},
};

#endif //HW3_SKELETON_H