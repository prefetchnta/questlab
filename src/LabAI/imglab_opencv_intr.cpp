/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2024-10-14
 *              #
 ================================================
        图像 LAB OpenCV 内部函数库
 ================================================
 */

#include "imglab_int.hpp"

/*
=======================================
    列表多边形生成
=======================================
*/
extern xpoly_lst_t
imglab_split_to_xpoly (
  __CR_IN__ std::vector<cv::Point>* pnts,
  __CR_IN__ size_t                  usize
    )
{
    size_t  count = pnts->size();

    if (count < usize || count % usize != 0)
        return (NULL);

    std::vector<std::vector<cv::Point> >*   list;

    list = new(std::nothrow) std::vector<std::vector<cv::Point> > ();
    if (list == NULL)
        return (NULL);
    list->reserve(count / usize);

    for (size_t idx = 0; idx < count; idx += usize)
    {
        std::vector<cv::Point>  pt (pnts->begin() + idx,
                                    pnts->begin() + idx + usize);
        list->push_back(pt);
    }
    return ((xpoly_lst_t)list);
}

/*
=======================================
    矩阵形式的坐标转点形式的坐标
=======================================
*/
extern void_t
imglab_mats_to_points (
  __CR_OT__ std::vector<cv::Point>*     pnts,
  __CR_IN__ const std::vector<cv::Mat>& mats
    )
{
    for (size_t ii = 0; ii < mats.size(); ii++) {
        if (mats[ii].dims != 2 || mats[ii].cols != 2)
            continue;
        for (int jj = 0; jj < mats[ii].rows; jj++)
        {
            cv::Point   tmp;

            tmp.x = (int)mats[ii].at<float>(jj, 0);
            tmp.y = (int)mats[ii].at<float>(jj, 1);
            pnts->push_back(tmp);
        }
    }
}
