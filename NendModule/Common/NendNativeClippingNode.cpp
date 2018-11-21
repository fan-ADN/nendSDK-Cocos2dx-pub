//
//  NendNativeClippingNode.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "NendLogger.h"
#include "NendNativeClippingNode.h"

USING_NS_CC;

using namespace nend_module;

static const auto kDefaultOrderWidth = 1.f;
static const auto kDefaultCornerSegments = 20;
static const auto kLimitRadius = 10.f;

#pragma mark - Internal
void appendCubicBezier(int startPoint, std::vector<Vec2> &verts, const Vec2 &from, const Vec2 &control1, const Vec2 &control2, const Vec2 &to, uint32_t segments)
{
    float t = 0;
    for (int i = 0; i < segments; i++) {
        float x = powf(1 - t, 3) * from.x + 3.0f * powf(1 - t, 2) * t * control1.x + 3.0f * (1 - t) * t * t * control2.x + t * t * t * to.x;
        float y = powf(1 - t, 3) * from.y + 3.0f * powf(1 - t, 2) * t * control1.y + 3.0f * (1 - t) * t * t * control2.y + t * t * t * to.y;
        verts[startPoint + i] = Vec2(x, y);
        t += 1.0f / segments;
    }
}

Size getActualClipSize(Size size)
{
    float contentScaleFactor = Director::getInstance()->getContentScaleFactor();
    float pixel_size_w = size.width * 2;
    float pixel_size_h = size.height * 2;
    Size clipSize = Size(pixel_size_w / contentScaleFactor, pixel_size_h / contentScaleFactor);
    return clipSize;
}

float checkLimitRadius(float radius)
{
    if (radius > kLimitRadius) {
        radius = kLimitRadius;
    }
    return radius;
}

DrawNode *createRoundedRectMaskNode(Size size, float radius, float borderWidth, int cornerSegments)
{
    Size clipSize = getActualClipSize(size);
    float cornerRadius = checkLimitRadius(radius);

    const float kappa = 0.552228474;
    float oneMinusKappa = (1.0f - kappa);

    // define corner control points
    std::vector<Vec2> verts(16);

    verts[0] = Vec2(0, cornerRadius);
    verts[1] = Vec2(0, cornerRadius * oneMinusKappa);
    verts[2] = Vec2(cornerRadius * oneMinusKappa, 0);
    verts[3] = Vec2(cornerRadius, 0);

    verts[4] = Vec2(clipSize.width - cornerRadius, 0);
    verts[5] = Vec2(clipSize.width - cornerRadius * oneMinusKappa, 0);
    verts[6] = Vec2(clipSize.width, cornerRadius * oneMinusKappa);
    verts[7] = Vec2(clipSize.width, cornerRadius);

    verts[8] = Vec2(clipSize.width, clipSize.height - cornerRadius);
    verts[9] = Vec2(clipSize.width, clipSize.height - cornerRadius * oneMinusKappa);
    verts[10] = Vec2(clipSize.width - cornerRadius * oneMinusKappa, clipSize.height);
    verts[11] = Vec2(clipSize.width - cornerRadius, clipSize.height);

    verts[12] = Vec2(cornerRadius, clipSize.height);
    verts[13] = Vec2(cornerRadius * oneMinusKappa, clipSize.height);
    verts[14] = Vec2(0, clipSize.height - cornerRadius * oneMinusKappa);
    verts[15] = Vec2(0, clipSize.height - cornerRadius);

    // result
    std::vector<Vec2> polyVerts(4 * cornerSegments + 1);

    // add corner arc segments
    appendCubicBezier(0 * cornerSegments, polyVerts, verts[0], verts[1], verts[2], verts[3], cornerSegments);
    appendCubicBezier(1 * cornerSegments, polyVerts, verts[4], verts[5], verts[6], verts[7], cornerSegments);
    appendCubicBezier(2 * cornerSegments, polyVerts, verts[8], verts[9], verts[10], verts[11], cornerSegments);
    appendCubicBezier(3 * cornerSegments, polyVerts, verts[12], verts[13], verts[14], verts[15], cornerSegments);
    // close path
    polyVerts[4 * cornerSegments] = verts[0];

    // draw final poly into mask
    auto maskNode = DrawNode::create();
    maskNode->drawPolygon(&polyVerts[0], static_cast<int>(polyVerts.size()), Color4F::WHITE, 0.0f, Color4F::WHITE);

    // return draw node as stencil
    return maskNode;
}

#pragma mark - Constructor / Destructor
NendNativeClippingNode::NendNativeClippingNode()
{
}

NendNativeClippingNode::~NendNativeClippingNode()
{
    NendLogger::logDebug(__FUNCTION__);
}

#pragma mark - Setter / Getter
void NendNativeClippingNode::setClipSize(Size clipSize)
{
    m_clipSize = clipSize;
}

Size NendNativeClippingNode::getClipSize()
{
    return m_clipSize;
}

void NendNativeClippingNode::setCornerRadius(float cornerRadius)
{
    m_cornerRadius = checkLimitRadius(cornerRadius);
}

float NendNativeClippingNode::getCornerRadius()
{
    return m_cornerRadius;
}

void NendNativeClippingNode::setAdSpritePorition(Point spritePoint)
{
    m_adSpritePoint = spritePoint;
}

Point NendNativeClippingNode::getAdSpritePorition()
{
    return m_adSpritePoint;
}

void NendNativeClippingNode::setClipType(NendNativeClipType type)
{
    m_clipType = type;
}

NendNativeClipType NendNativeClippingNode::getClipType()
{
    return m_clipType;
}

#pragma mark - create
NendNativeClippingNode *NendNativeClippingNode::create(const Size clipSize, const float radius, Sprite *sprite, NendNativeClipType clipType)
{
    NendNativeClippingNode *clipNode = new NendNativeClippingNode();

    if (clipNode->init()) {
        if (sprite) {
            clipNode->setStencil(createRoundedRectMaskNode(clipSize, radius, kDefaultOrderWidth, kDefaultCornerSegments));
            clipNode->setContentSize(clipSize);
            clipNode->setClipSize(clipSize);
            clipNode->setClipType(clipType);
            clipNode->setCornerRadius(radius);
            clipNode->initOptions(sprite, clipType);
        }
        clipNode->autorelease();
        return clipNode;
    }

    CC_SAFE_DELETE(clipNode);
    return NULL;
}

#pragma mark - initOptions
void NendNativeClippingNode::initOptions(Sprite *sprite, NendNativeClipType clipType)
{
    Size clipSize = getActualClipSize(m_clipSize);

    switch (clipType) {
        case NEND_CLIP_CENTER:
            sprite->setPosition(Point(clipSize.width / 2, clipSize.height / 2));
            sprite->setAnchorPoint(Point(0.5f, 0.5f));
            break;
        case NEND_CLIP_TOP_RIGHT:
            sprite->setPosition(Point(0, 0));
            sprite->setAnchorPoint(Point(0, 0));
            break;
        case NEND_CLIP_TOP_LEFT:
            sprite->setPosition(Point(clipSize.width, 0));
            sprite->setAnchorPoint(Point(1.f, 0));
            break;
        case NEND_CLIP_BOTTOM_RIGHT:
            sprite->setPosition(Point(0, clipSize.height));
            sprite->setAnchorPoint(Point(0, 1.f));
            break;
        case NEND_CLIP_BOTTOM_LEFT:
            sprite->setPosition(Point(clipSize.width, clipSize.height));
            sprite->setAnchorPoint(Point(1.f, 1.f));
            break;
        default:
            break;
    }
}
