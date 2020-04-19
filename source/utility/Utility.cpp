#include "ee2/Utility.h"

namespace ee2
{

ur2::RenderState Utility::GetRenderState2D()
{
    ur2::RenderState rs;

    rs.depth_test.enabled = false;
    rs.facet_culling.enabled = false;

    return rs;
}

}