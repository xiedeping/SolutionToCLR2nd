#pragma once

// 从数组A中找出中位数
// 平均时间复杂度是 O(n)，最坏时间复杂度是O(n)
int median(int A[], int n);

// 从有序数组A和B中找出中位数
// 平均时间复杂度O(lgN)，最坏时间复杂度O(lgN)
int median_two_array(int A[], int B[], int n);