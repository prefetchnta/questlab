//
// Created by tunm on 2023/1/25.
//
#pragma once

#ifndef ZEPHYRLPR_PLATE_RECOGNITION_TOKENIZE_H
#define ZEPHYRLPR_PLATE_RECOGNITION_TOKENIZE_H

#include <iostream>

static const std::vector<std::string> TOKENIZE = {
        "blank", "'", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "J",
        "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "云", "京", "冀", "吉", "学", "宁",
        "川", "挂", "新", "晋", "桂", "民", "沪", "津", "浙", "渝", "港", "湘", "琼", "甘", "皖", "粤", "航", "苏", "蒙", "藏", "警", "豫",
        "贵", "赣", "辽", "鄂", "闽", "陕", "青", "鲁", "黑", "领", "使", "澳"};

#endif //ZEPHYRLPR_PLATE_RECOGNITION_TOKENIZE_H
