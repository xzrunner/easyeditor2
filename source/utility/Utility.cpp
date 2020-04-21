#include "ee2/Utility.h"

namespace ee2
{

ur2::RenderState Utility::GetRenderState2D()
{
    ur2::RenderState rs;

    rs.depth_test.enabled = false;
    rs.facet_culling.enabled = false;

    rs.blending.enabled = true;
    rs.blending.separately = false;
    rs.blending.src = ur2::BlendingFactor::One;
    rs.blending.dst = ur2::BlendingFactor::OneMinusSrcAlpha;
    rs.blending.equation = ur2::BlendEquation::Add;

    return rs;
}

}